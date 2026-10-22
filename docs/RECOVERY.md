# P520 brick recovery — interfaces, tooling, and an honest plan (2026-10-22)

A bad main-app build (the menu **label** patch, `0xc0ebf358=61000000`) makes the camera
bootloop: green ring on, then restart, forever. This documents how recovery works on this
hardware, what I reversed out of the bootloader, the parts you'd need, and — importantly —
the two things that are **not** knowable from the firmware alone.

## What is and isn't broken

* **Only the main application is bad.** The patch pipeline never touches the boot-ROM, the
  Zburn bootloader, or the flash partition table — it edits two/three main-app segments and
  reseals the byte-sum. So the SoC's low-level recovery path is intact.
* **But every normal reflash route runs *inside* the main app.** The retail
  "Setup → Firmware update" menu, the card-at-boot check, USB and UART — on the P520 these
  are all reached *after* the main app is up. The app crashes before any of them initialise,
  which is why: card-in-slot at power-on does nothing, USB enumerates nothing, and it
  bootloops in both shooting and playback boot. (All confirmed on your camera.)
* **Recovery therefore needs the SoC's *burn mode*** — the bootloader's own downloader, which
  runs before the main app. That's the only code path that can overwrite the bad app without
  the bad app's help.

## Board-level findings (from board photos, 2026-10-22)

Confirmed against mainboard photos + the firmware:

* **Zoran COACH SoC** (large BGA) + a **Micron memory BGA** beside it (marking `…D9P… 3Q68`).
  On this class of camera that Micron part is typically an **MCP — mobile DDR *and* NAND in
  one package.** The bootloader confirms NAND is the firmware medium: `BurnNand`,
  `NandReadElfCode`, `NandUserAreaBackup/Restore` are all present and the boot path reads the
  app ELF from NAND.
* **The firmware (bootloader + app + "internal memory") is in that NAND, inside a BGA.** There
  is **no socketed/8-pin SPI-NOR** holding the firmware. The small 8-pin chip top-left
  (`9PS3 326D7`) is not the boot source — it's almost certainly a small serial EEPROM for
  lens/AF *adjustment data*, not something you reflash to recover.
* **Consequence:** the cheap "clip a CH341A onto the flash and rewrite it" route does **not**
  apply here. Getting at the NAND directly means chip-off (hot-air removal, reball, and a NAND
  programmer that knows this exact MCP plus its ECC/bad-block layout) — advanced, risky, and
  the opposite of hassle-free. Don't go there.
* **The annotated photo** (red **"T"** + a yellow wire on a large round gold pad on the
  right-center edge; blue wires on bottom-edge pads) shows someone tapped the board's
  **factory test points**. Those big round edge pads are exactly where UART / reset / boot
  straps are brought out. That tap is the lead worth chasing — but I can't confirm from a
  photo which pad is TX/RX/GND vs. a strap. Treat "T" as a *candidate* TX and verify with a
  multimeter / the boot-log monitor before trusting it.

## The honest bottom line

Given the NAND-in-BGA reality, the two most sensible options are a **board swap** or a
**repair shop** — both avoid burn mode entirely:

* **Replacement mainboard (most hassle-free).** Working P520 mainboards show up on
  eBay/AliExpress. Swapping the board (FPC connectors + a few screws, no fine-pad soldering)
  gives you a **booting camera on stock firmware**, after which you re-apply the *safe* NRW
  build (value + neuter, **no** label patch) through the normal SD-card update menu. Caveat:
  the new board carries its own adjustment EEPROM, so AF/zoom calibration may be slightly off
  vs. your lens unit — usually fine in practice; a shop can fine-tune if needed.
* **Repair shop / Nikon service.** They have the factory jig + correct burn image and reflash
  in minutes. Get a quote before buying tools.

A full **DIY burn-mode unbrick** (below) is the "proper" fix and the most educational, but it
has two gaps that aren't knowable from firmware, so treat it as a project, not a quick fix:

### Gap 1 — entering burn mode is a *hardware* choice, not software

Burn mode is selected at power-on by a boot-strap input the bootloader reads from register
`_DAT_bfc37dfc` (`BurnMain` switches on it → card / UART / USB downloader). That input is a
**physical test point / strap resistor on the mainboard** — it is set by the board, not by
any button combo or flag I can patch. **Its location is not in the firmware** and I cannot
derive it. You'd get it from the P520 service manual, FCC-ID internal photos, or the
camera-hacking community (CHDK/“Zoran COACH” forums, the `ops` COACH tooling project).

### Gap 2 — the burn *image format* is a separate unknown

The downloaders expect a **factory burn image** (a `*.ELF` with a `0x55aa` header at offset
`0x3e` and a trailing MD5), **not** the SD-update `firmware.bin` we've been patching. The
SD-update package is decrypted and unpacked by the (now-crashing) app before anything is
burned. I have the *decrypted* main-app image but not a verified factory burn container, so a
blind DIY flash would also need that format reversed and — ideally — validated against a
known-good NAND dump. Without that validation, writing to flash over USB/UART risks making
things worse.

Because of Gap 2, the burn-client in `tools/p520_recover.py` is **experimental and
explicitly unvalidated**. The *monitor* side (reading the boot log) is safe and useful; the
*writing* side should not be trusted blind.

## What DIY *can* safely do today: watch the boot log (UART)

Even without solving the gaps, UART at **115200 8N1** gives you the serial console the
bootloader and app print to. That lets you:

1. **See the actual crash** and confirm the label patch is the cause (expected: a fault/assert
   right as the UI string table is read).
2. **Watch for any boot-time prompt** — some COACH builds drop to a monitor or offer a
   download window early in boot. If one appears, that could be a *software* recovery with no
   burn-mode strap needed. This is the best-case DIY outcome and costs almost nothing to check.

```
python3 tools/p520_recover.py monitor /dev/ttyUSB0     # then power the camera on
python3 tools/p520_recover.py usb-info                 # is it in USB burn mode? (0595:6161)
```

Finding the UART pads: they're 3.3 V TTL on the mainboard (idle-high TX). Locate via the
service manual, or probe candidate test pads with a multimeter for a line that sits ~3.3 V at
rest and bursts activity at power-on; GND against the battery/chassis ground. **Do not feed
5 V or RS-232 levels in — 3.3 V only**, or you kill the SoC.

## Affordable parts list (~$70–130 total)

You do **not** need a hot-air/BGA rework station — this is through-hole-ish pad soldering of a
few fine wires.

| Part | What to get | ~Price | Notes |
|---|---|---|---|
| Soldering iron | **Pinecil V2** (USB-C, needs a 45–65 W PD supply you likely own) — or a generic 936-type station | $35 / $25 | Pinecil is the sweet spot: precise, tiny tip, cheap. Either is fine. |
| USB-UART adapter | **CP2102** or **CH340** 3.3 V module | $5 | **Must support 3.3 V logic.** Many FTDI/CP2102 boards have a 3.3/5 V jumper — set 3.3 V. |
| Solder | 0.6–0.8 mm leaded 63/37 rosin-core | $8 | Leaded is far easier for fine work than lead-free. |
| Flux | No-clean liquid/gel flux pen | $7 | The single biggest quality-of-life upgrade for fine pads. |
| Fine wire | 30 AWG silicone or Kynar wire-wrap | $7 | For tapping tiny pads. |
| Tweezers | ESD fine-point | $6 | |
| Multimeter | Any basic DMM | $15 | To find GND and the 3.3 V UART line; verify before connecting. |
| Precision screwdrivers | Phillips #00/#000 set | $10 | To open the camera. |
| (Optional) Helping hands / cheap USB microscope | | $15–30 | Makes fine pads much easier. |

Skip for now (only needed if you later go the direct-flash route and know it's required): a
SOIC-8/TSOP clip or an external NAND/SPI programmer. Don't buy these until the community/service
manual confirms the flash type and that in-system programming is actually necessary.

## Recommended order of operations

1. **Get a repair quote first.** If it's reasonable, stop here — it's the safe path.
2. If going DIY: buy the parts list, open the camera, and **start with UART monitoring** at
   115200. Capture the full boot log and look for a crash signature and any download prompt.
3. Take that boot log + the "need burn-mode strap location for Zoran COACH P520" question to
   the service manual / CHDK / COACH-tooling community. The strap location is the one piece I
   can't give you.
4. Only once burn mode is confirmed entered (camera shows as `0595:6161` on USB, or the UART
   downloader responds) and the burn-image format is validated should you attempt a write.

## Verified stock image for reflashing

Use the **unmodified** v1.1 SD-update image you already have on your Hugging Face dataset
(`swarrce/Test`). Verify before use:

```
sha256sum firmware.bin
# must be: 631a4c6ab34e3be5c7ee1153ad1a7d93986da2853c79f572b63a3e23129b979c
```

(Note Gap 2: this is the SD-update package. If a repair shop reflashes, they use their own
factory image — you don't need to supply anything. For a DIY burn-mode write you'd first need
this converted to the factory burn container, which is still unsolved.)

## Reversed interface reference (from `zburn.elf`)

* **Burn-mode selector:** `BurnMain` reads `_DAT_bfc37dfc` → `DownloadCard` / `DownloadUart`
  / `DownloadUsb`.
* **USB:** Zoran `VID 0x0595 / PID 0x6161`; bulk `EP2` (OUT) / `EP3` (IN), 64-byte packets;
  `UsbClientCommand` cmd `0xF0` = write-flash-burst; `UsbClientRead` reads a 0x40-byte header
  and checks `0x55aa` magic.
* **UART:** `UartOpenFifo` baud `0x1c200` = 115200; `UartClientCommand` reads a 4-byte command
  (`[0x01,…,cmd,arg]`), acks `'T'` (0x54); cmd `0xF0` → `UartClientWrite` (1 KB) +
  `UartOnWriteFlashBurst`. Data framing is XMODEM-like (`0x02` data, `0x04` end, `0x43` 'C',
  `'T'` ack).
* **Card:** scans the card root for a `*.ELF` burn file with the `0x55aa` header + trailing MD5.

## After recovery

Reflash **stock**, confirm it boots, then use the **confirmed-working** NRW build — value +
conversion-neuter only, **no label patch**:

```
python3 tools/p520fw.py patch firmware.bin firmware_nrw.bin \
    0xc0ebf2f8=05000000 \
    0xc057634c=21108000
```

The "Image quality" menu row 2 will read "Normal" but store NRW+Fine (cosmetic only — the
live-view indicator shows NRW+Fine correctly). Use `tools/nrwfix.py` on the `.NRW` for desktop
converters. **Do not re-apply the label patch** (`0xc0ebf358`) — that is what bricked the
camera.
