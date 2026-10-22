# Nikon COOLPIX P520 firmware RE — session summary

Target: `firmware.bin` (= the official Nikon P520 v1.1 SD-update image,
81,264,928 bytes; the Win/Mac installers F-P520-V11W.exe / F-P520-V11M.dmg
unpack to exactly this file). Goal: enable RAW/NRW, which needs patching the
image and getting it past the on-device integrity check.

## TL;DR status
- Decryption: SOLVED and reproducible (`scripts/decrypt.py` → `dec.bin`).
- Container/format: fully mapped (blocks, VLC tables, ELF segments, header).
- Custom HW-inflate codec: NOT cracked — it runs on the SoC's silicon VLC
  engine; confirmed not standard DEFLATE; static analysis can't recover the
  per-bit convention from the file alone. This is the one hard wall.
- Integrity: it's MD5 (`BurnHash*`), NOT a crypto signature → forgeable by
  recompute. Key implication below.
- Full symbol map recovered: `p520_symbols.map` (72,760 funcs w/ addresses).
- USB tooling built: `p520tool.py` (probe PTP/SCSI/DFU + UART terminal).

## What's solved

### 1. Decryption (reproducible)
Stream cipher: `plain[i] = cipher[i] ^ K0[i & 0xffff] ^ C[(i>>16) & 0xff]`
- `K0` = the 64 KiB ciphertext block at file 0x150000 (ciphertext of an
  all-constant plaintext region; XOR-autocorr at lag 0x10000 = 1.0 there).
- `C[r]` = per-64KiB-chunk constant, period 256 in the chunk index, recovered
  by mode-voting residue class `chunk & 0xff` of `cipher_chunk ^ K0`.
- First 0x20 bytes ("P520…Ver.1.1") are plaintext, restored verbatim.
Validated: ELF/block structure parses, all 1292 Huffman tables give Kraft=1.
`scripts/decrypt.py` regenerates `dec.bin` from `firmware.bin` in seconds.

### 2. Container / block / table format (verified)
- Flash header (first 0x450): "hcemain.elf" image entry (offset 0x50, size
  ≈ whole file), then HW register-init pairs (b800/b8003exx + a deadbeef mark).
- 6 segment ELFs @0x360310+ (s1..s5b); each single PT_LOAD, p_offset 0x54,
  memsz = Σ block osz. 646 "type-2" HW-inflate blocks total.
- Block header (16 B): u32 type(=2), u32 osz, u32 clen, u16 aBytes, u16 bBytes;
  then A[] (lit/len VLC table), B[] (distance VLC table), C[] (bitstream).
- VLC node word (32-bit): tag=x&3 (3=leaf,1=Q,0=D); leaf val=(x>>2)&0x7fff
  top=x>>15; D/Q a=x>>15 b=(x>>11)&15 w=(x>>2)&0x1ff. D=dense 2^b table,
  Q=sparse a+b children. Tiles perfectly; every table Kraft=1, prefix-free.
- D-node bit convention PROVEN: MSB-first dense index (all 168,936 runs are
  power-of-two + aligned). Selector: D has a≤b, Q has a≥b.
- Symbol alphabet is RICHER than DEFLATE (this corrected a wrong earlier note):
  value = base(bits0-8) | eb(bits10-12 extra-bit count) | class bits
  0x200/0x2000/0x4000. ~85 literals+EOB, deflate-style length codes, plus a
  large 0x2000-class (161 syms, base 1-255) that is not plain deflate lit/len.

### 3. The hard wall: the inflate bit convention
Ruled out exhaustively this session:
- Tree-walk Q conventions (skewed/balanced/dense+escape) × stream orders ×
  bit orders × canonical rebuilds → max 1-3 leading literals, never decodes.
- Raw DEFLATE (`zlib -15`) on all 646 blocks + full-image scan → 0 real hits
  (apparent hits are degenerate single-byte garbage).
- HW driver: decode is pure silicon (InflateProcessorCreate opens DCU DRAM
  clients + ISRs; engine reads bitstream autonomously). Bit order is in the ASIC.
- Resident `CreateInflater`/`InflateElfStream` symbol slots hold rodata strings,
  not locatable code (image VA→file mapping is non-linear past ~0xc005xxxx).
Conclusion: not recoverable by static analysis of the file alone. Needs a RAM
dump (decompressed main app in RAM at boot) or silicon-level observation.

## What matters for the actual goal (RAW patch deployment)

### Integrity is MD5, not a signature
- `BurnHashInit/Update/Fanalize` = `md5_init/append/finish`; `DebugMD5ResultCompare`.
  Strings: "Verify binary data...", "Received hash" vs "Calculated hash",
  "Hash verify success!". Whole-image crypto-sig search: none (no RSA/ECDSA/HMAC).
- MD5 primitives are RESIDENT & readable: md5_init c005a500, md5_append c005a53c,
  md5_finish c005a680, DebugMD5sum c00624b4.
- IMPLICATION: a patched image passes if its MD5 is recomputed and written back —
  no key to forge. BUT MD5 is NON-LINEAR, so a GF(2)/Gaussian-elimination attack
  (which only works on a linear CRC) cannot forge it. If the check that rejects
  the RAW patch is this MD5 (or includes it), the fix is to recompute the MD5
  honestly, not solve a linear system. Possible there are two layers (container
  CRC that GF(2) handles + payload MD5 that must be recomputed).
- Not yet pinned: the exact MD5 field offset + the byte-range it covers. That
  logic is in `AppUpgradeFlashFromCard` (compressed main app) — needs the RAM
  dump to read. No stored MD5 of any raw compressed segment exists in the image,
  so the hash is over decompressed content or the update container.

### Deployment mechanism (confirmed, no hardware)
firmware.bin IS the SD update package: patch it → fix the MD5 field → copy the
firmware folder to the SD card root → camera Setup menu → Firmware version →
Update. The COACH "Recover Area" (PrintRecoverHeaderProperties / InitRecoverSection)
means the bootloader can fall back if the main image is bad → lower brick risk.

## USB findings (from the live camera + symbol map)
- Normal mode = PTP (VID 04B0). Vendor opcodes present: 0x90A1/0x90A2/0x90A3,
  0x9805 (=MTP GetObjectPropList); 0x9801-3 are MTP enhanced props. Handlers are
  in compressed code; none documented as memory-read. Unlikely to give a dump.
- USB command channels exist (HcePtpVendor, HceUsbMSCustomAction, UsbCmdDispatcher)
  but opcodes are in compressed code.
- No DFU on the Nikon (the "Camera DFU Device" in Zadig was the laptop webcam,
  Realtek 0BDA). Zburn USB recovery exists in ROM but its entry trigger is unknown.

## The one clean unlock
A **$2 USB-UART adapter (3.3V, CP2102/CH340)** → the firmware's debug console
(`DebugConsoleMain`, UART driver, "ENTERING DEBUG CONSOLE", memory-dump cmds).
A RAM dump at va 0x8014e900 gives the decompressed main app directly — which
makes the inflate crack unnecessary AND exposes the exact checksum/updater code
(pins the MD5 field for the RAW patch). Everything converges here.

## Files in this bundle
- `scripts/decrypt.py`      regenerate dec.bin from firmware.bin
- `scripts/zt.py`           block/table parser (edit path at top to your dec.bin)
- `scripts/*.py`            all the convention-search experiments (see FINDINGS)
- `p520_symbols.map`        72,760 symbols (addr + name) — the master reference
- `FINDINGS.md`             detailed technical notes
- `p520tool/p520tool.py`    USB PTP/SCSI/DFU prober + UART terminal (+README)
- `SESSION_SUMMARY.md`      this file

Note: `dec.bin` (78 MB, the decrypted image) is not bundled — it's regenerated
by `scripts/decrypt.py` from your `firmware.bin`. Ask if you want the 78 MB file
sent directly.
