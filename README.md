# Nikon COOLPIX P520 firmware RE — complete bundle

Target: `firmware.bin` = Nikon P520 v1.1 SD-update image (81,264,928 B).
Goal: enable RAW/NRW stills by patching the image and passing the on-device check.
Siblings also decrypted this project: P530 v1.1, P600 v1.2 (same COACH codec).

This archive gathers every script, decompile, and breakthrough from the sessions.
Large regenerable binaries (dec.bin, p530.bin, p600.bin) are NOT included — see
"Regenerate" below; everything needed to rebuild them is here.

## Status at a glance
- DECRYPTION — SOLVED, reproducible. Stream XOR: plain[i]=cipher[i]^K0[i&0xffff]^C[(i>>16)&0xff].
  header[0:0x20] stored plaintext. `tools/reseal.py decrypt` or `parsers/decrypt.py`.
- INTEGRITY — SOLVED (the big one). The on-device check is a plain 32-bit additive
  byte-sum at decrypted offset 0x4d80110 over dec[0x50:0x4d80110]. No CRC, no signature,
  no payload MD5. Patch -> recompute sum -> re-encrypt. `tools/reseal.py`. Proven to
  reproduce firmware.bin byte-for-byte; a 1-byte edit updates the sum correctly.
  (This retired the previous "CRC via GF(2)" wall entirely.)
- CONTAINER/FORMAT — fully mapped (ELF segments, 16-byte block header, A/B VLC tables,
  type-2 bitstream). 646 blocks in P520, 453 in P600.
- HW-INFLATE CODEC — NOT fully cracked, but reduced to ONE named unknown:
  * length codes = DEFLATE exactly (29/29 base,extra-bit pairs); leaf layout base[0:9] eb[10:13].
  * Q-node = balanced prefix code over its a+b children, min length b (Kraft-derived).
  * run-length-aware advance required (peek b bits, advance by true code length).
  * distance tree B = small values (0-55), small/custom window (NOT DEFLATE distances).
  * RESIDUAL UNKNOWN: the 0x2000 opcode-class action, coupled with exact bit walk.
- RAW GATE — latent in firmware (fPCM_PTP_IsFileNRW, g_usPCM_PTP_ImageFormatsWithNRW,
  NRW_CURSOR UI icon). All 14 NRW/quality gates live in COMPRESSED segments -> enabling
  RAW needs the codec, not a resident data patch.


## Layout
- docs/        SESSION_SUMMARY, FINDINGS (original) + INTEGRITY_SOLVED, CODEC_STATUS,
               MONITOR_USB_LEAD (the session breakthroughs).
- tools/       reseal.py (integrity, WORKING), p520_usb_monitor.py (USB monitor probe),
               p520tool.py (+README; USB/PTP/SCSI/UART prober), dec_sib.py (sibling decrypt).
- codec/       search2.py (full-block + oracle search, run-length-aware — the live harness),
               infl2.py/infl3.py/search.py/infl.py (earlier decoder attempts + MIPS oracle).
- parsers/     decrypt.py, zt.py (block/table parser), elfs.py, elfhdr.py, walkblocks.py.
- reverse/     Ghidra: Decomp.java, ApplySyms.java; decompiles zburn_all.c,
               res_inflate.c, res_inflate2.c, res_dcu.c, res_feat.c, res_mon.c;
               p520_symbols.map (72,760 syms), boot_syms.txt; resident.bin, zburn.elf.
- data/        K0.bin + C.npy (P520 cipher), p530/p600 .k0 + .C.npy (sibling cipher), Cmode.npy.
- experiments/ the full convention-search script set (canon*, decode*, qenum*, qesc, etc.).

## Getting started (firmware is not in git)
    pip install numpy capstone
    python3 -I tools/fetch_firmware.py              # download P520/P530/P600 from Nikon, unpack, decrypt
    python3 -I tools/fetch_firmware.py --local DIR  # offline: use F-P5x0-V1xW.exe / firmware.bin you already have
This puts the encrypted images in firmware/ (firmware.bin, p530/, p600/) and the decrypted
ones in work/ (dec.bin, p530.bin, p600.bin). Both dirs are gitignored. Every script finds them
through parsers/p520paths.py; override with P520_FIRMWARE / P520_DEC / P520_FW_DIR / P520_WORK.

Check the decrypt + integrity pipeline:
    python3 -I tools/reseal.py checksum work/dec.bin                      # expect MATCH 0x0390f0d8
    python3 -I tools/reseal.py encrypt  work/dec.bin /tmp/re.bin && cmp /tmp/re.bin firmware/firmware.bin

Ghidra re-import (if needed): import zburn.elf and resident.bin (BinaryLoader,
MIPS:LE:32, base 0xc0000800 for resident), run ApplySyms.java then Decomp.java.

Known gaps in the bundle: experiments/inf.py, inf2.py, dec1.py import local `enum`/`enum2`
modules that were not included. mainsyms.pkl is rebuilt on the fly from p520_symbols.map.
