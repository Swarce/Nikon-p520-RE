# COACH-emu lite — a diagnostic emulator for the P520 main app (2026-10-22)

`tools/coachemu.py` boots the decompressed main-app RAM image under
[Unicorn](https://www.unicorn-engine.org/) (MIPS32 little-endian) and runs the **real
firmware code**, symbolized, until it faults or hits one of the firmware's own asserts. It is
**not** a camera emulator — there is no sensor, lens, display, or NAND model. Its purpose is:

* **see what the firmware actually does** at boot, as a symbol-resolved trace;
* **capture the firmware's own panic/assert messages** (they name the file and the expected
  value, so bringup is nearly self-documenting);
* **test whether a patch changes behaviour** in software, before risking hardware.

Inspired by the NikonHacker "Dev Suite" FR emulator for the DSLRs — but those bodies run a
Fujitsu FR CPU, whereas COACH is MIPS32, so only the *method* carries over, not the code.

## How it boots (the non-obvious part)

`_start` (`0xc0000600`) programs the MMU itself: a 256 MB TLB page maps virtual `0xc0000000`
→ physical `0x80000000` (contiguous), with an uncached alias at `0xe0000000`. On hardware the
boot ROM sets up that mapping first so `_start` can even be fetched. We start cold, so:

1. Map physical RAM at `0x80000000` (256 MB) and load the image there; also mirror the low
   image at physical `0x0` so the KSEG1 alias is fetchable.
2. Begin execution at **`0xa0000600`** — `_start`'s KSEG1 alias (KSEG1 is fixed-translate, no
   TLB). `_start` does only CP0 ops until it `jr`s into `0xc0000xxx`, by which point its own
   `tlbwi` has created the mapping.
3. MMIO (KSEG1 hardware registers) is caught lazily and backed as RW memory. Registers the
   firmware reads back from a *different* address than it wrote are seeded from `MMIO_MODEL`.

## Boot progress

Each wall below was diagnosed from the firmware's own assert text or a symbol-resolved fault,
then cleared with a one-line model entry. Current reach, in order:

| stage | what happened | fix |
|---|---|---|
| `_start` | TLB is self-programmed (256MB pages, `0xc0000000`→`0x80000000`) | start at KSEG1 alias `0xa0000600` |
| `main → CguInit` | `CCguUnitMgmtImpl::VerifyUnits()` reads CGU_UNIT_EN1 (`0x1000800c`), wants `0x2000` | `MMIO_MODEL[0x1000800c]=0x2000` |
| `_gpr_shadow_check` | spins waiting for CP0 SRSCtl.HSS (shadow reg sets Unicorn lacks) | `CPU_SHIMS` sets HSS after the `mfc0` |
| `_tx_thread_create → stack_build` | system stack at top of RAM (`0xdfffff70`) | map full 512MB DRAM (TLB even+odd pair) |
| `tx_application_define → PoolInit → DcuGetDramAttributes` | reads DRAM-geometry index (`0x18003c10`), 0 = "Unknown main DRAM size" | `MMIO_MODEL[0x18003c10]=7` (512MB) |
| `PoolInit → CreateDirect` (null pool base) | ThreadX passes `_tx_initialize_unused_memory`=0 → null pool; root cause a bogus **EBase CPUNum** | EBase fix (below) |
| `CCguUnitMgmtImpl::VerifyUnits` (2nd pass) | CGU self-check expects a *dynamic* enable mask (`0x2000`→`0x2100`…) we can't mirror | neuter (`NEUTER_FUNCS`) |
| **ThreadX scheduler idle (current)** | threads init, run (`RES = NAND_NSR`), suspend; scheduler sits in the `wait` loop | needs a **timer-tick interrupt** |

So from a cold image it now runs through CPU init, the MMU bringup, clock/DRAM self-checks,
the ThreadX kernel, thread creation, memory-pool setup, **multithreaded scheduling**, and into
driver/resource init (NAND) — thousands of real instructions across several threads.

## The EBase CPUNum fix (the big unlock)

The null pool base traced to a **multi-core dispatch bug in the emulated CPU**: Unicorn reports
CP0 EBase (`r15` sel 1) `CPUNum` = `0x3ff`, but this is the main core (0). The firmware selects
per-core control blocks off `mfc0 rt,$15,1`; `_tx_initialize_low_level` branches on
`CPUNum==0` while `_tx_initialize_kernel_enter` branches on `CPUNum==1`, so a `CPUNum` that is
neither sends the writes and the reads to *different* blocks — `_tx_initialize_unused_memory`
was stored in the Cop block and read back as 0 from the main block, giving a null pool base
(the fault was `CreateDirect+0x8 sw $zero,0x10($a0)` with `$a0=0`). The emulator now scans the
image for every EBase read and masks `CPUNum` to 0 (247 sites), so the firmware consistently
sees core 0. This immediately got ThreadX scheduling real threads.

## The current wall: no timer tick

ThreadX has created its threads; they run, then suspend waiting on timers/events, and
`_tx_thread_schedule` sits in its `wait` loop because `_tx_thread_execute_ptr` is null and
nothing wakes it. The firmware needs a periodic **timer interrupt** (set up by `TimerInit`)
whose ISR calls `_tx_timer_interrupt` to expire sleeps and ready threads. Next step: deliver
that tick — drive the SoC/CP0 timer and have Unicorn vector the interrupt into the firmware's
handler, or invoke `_tx_timer_interrupt` periodically from the idle loop. This is a larger step
than the one-line models (interrupt delivery), and it unlocks the whole application layer.

Tooling added: `--itrace LO:HI` (instruction trace over a PC range, dumped on fault), a fault
register + `--peek` dump, `NEUTER_FUNCS` (force a self-check to return), and the EBase scan.

## Methodology (how to clear the next wall)

Fast, repeatable loop — most walls are one line:

1. Run; read the `[HALT]` message (names the check + expected value + source file) or the
   `[FAULT]` + recent-trace (names the function and the null/bad access).
2. If it's a readback: `--watch LO:HI` the register page to get the exact address + reading PC,
   disassemble that PC for the expected value, add it to `MMIO_MODEL`.
3. If it's a CPU-feature spin: add a `CPU_SHIMS` entry (like SRSCtl).
4. If it's a null call/structure: find the init that should have populated it.

## Usage

```
python3 -m pip install unicorn capstone
# regenerate the RAM image if needed:  python3 tools/p520fw.py unpack firmware.bin
python3 -I tools/coachemu.py --image unpacked/ram_c0000000.bin --syms p520_symbols.map
python3 -I tools/coachemu.py ... --mmio                       # log each MMIO page touched
python3 -I tools/coachemu.py ... --trace OsSysHaltEx,GdiGetTextDataById
python3 -I tools/coachemu.py ... --patch 0xc0ebf358=61000000  # see a patch's effect
```

## What it's good for vs. not

* **Good now:** execution tracing, assert/panic capture, fault→symbol localization, and
  comparing a patched vs. stock image's code path. Even with debug `Printf` gated off in the
  retail build, the symbol-resolved trace is the payoff.
* **Needs more work:** booting all the way to the GUI / string-table code (where the label
  patch crashes) requires modeling enough peripherals (CGU, DCU, SMDB, timers, ThreadX tick,
  NAND) to get past the self-checks. Two routes: (a) keep feeding `MMIO_MODEL` from the
  asserts until it boots; (b) skip full boot and invoke the suspect function directly with the
  loaded image once its data structures can be set up.
* **Not a goal:** real imaging (sensor/lens/pipeline/display). That's out of scope.

## Why this matters for the project

It turns "flash it and pray" into "run it and look." Once it reaches the UI code, we can
*prove* why the label patch (`0xc0ebf358=61…`) bricks the boot and validate a correct fix in
software — so the next hardware flash is one we've already watched succeed.
