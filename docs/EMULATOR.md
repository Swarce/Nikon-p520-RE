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

## Current state

Boots `_start → main → CguInit → CpuWdiDisable → DcuSlotMachineActivePage →
SmdbGetActiveSlotMacId`, then the firmware's self-check panics:

```
[HALT] OsSysHaltEx
   a0 -> 'Corruption of CGU_UNIT_EN1  expected: 2000  read: 0'
   a1 -> 'Os/Cgu/CguUnits.cpp'
```

i.e. it enabled a clock-gating unit and read back the status register, which our stub returns
as 0. This is the expected first wall: each peripheral the firmware verifies needs its
readback modeled. The assert messages hand us the expected values, so the path forward is
mechanical — add entries to `MMIO_MODEL` (and, where a register mirrors another, a small
read callback) until boot proceeds.

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
