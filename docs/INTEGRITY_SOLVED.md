# P520 firmware — integrity check SOLVED (2026-10-19)

## Headline
The on-device integrity check that was rejecting the patched firmware is a **plain
32-bit additive byte-sum**, not a CRC and not a crypto signature. It is trivially
recomputable. The previous session's GF(2)/Gaussian-elimination effort against a
"CRC discrepancy" is **unnecessary** — you don't solve a linear system, you just add
up the bytes and write the result back.

## Exact spec (verified end-to-end)
- Field: 32-bit little-endian integer at **decrypted offset 0x4d80110**.
- Coverage: **sum(dec[0x50 : 0x4d80110]) & 0xffffffff**.
  (header [0:0x50] excluded; the field itself and the final 12 bytes excluded — they're 0.)
- Stored value in v1.1 = 0x0390f0d8; recomputed value matches exactly.
- NOT present anywhere in the image: CRC32 table (no 0xEDB88320 / 0x04C11DB7),
  no stored MD5 of the payload, no RSA/ECDSA/HMAC in the burn path. Single linear layer.
- The MD5 code (md5_init/append/finish, BurnHash*) is the Zburn *factory* SD-burn path
  (DownloadCard: last 0x10 bytes of a burn file = expected MD5). It is NOT the field on
  the retail v1.1 update image — that image's tail is the byte-sum word + zero padding,
  not a 16-byte hash. So the menu-update path is gated by the byte-sum.

## Cipher round-trip (lossless, proven)
plain[i] = cipher[i] ^ K0[i & 0xffff] ^ C[(i>>16) & 0xff];  header [0:0x20] stored plaintext.
encrypt(decrypt(firmware.bin)) == firmware.bin, 0 byte mismatches.

## Tool: scripts/reseal.py  (needs K0.bin + C.npy, both in bundle)
    python3 -I scripts/reseal.py decrypt  firmware.bin dec.bin
    python3 -I scripts/reseal.py checksum dec.bin                 # stored vs computed
    python3 -I scripts/reseal.py patch    dec.bin out.bin  0xADDR=HEXBYTES ...
    python3 -I scripts/reseal.py encrypt  dec.bin new_firmware.bin   # reseal + re-encrypt
Verified: reseal of untouched dec.bin reproduces firmware.bin byte-for-byte; a 1-byte
payload edit yields a correctly-updated checksum (delta == byte delta).

So: **any change to dec.bin can be made to pass the integrity check** by resealing.
What remains is knowing *which* bytes to change for RAW — see below.

## RAW/NRW status: latent, gated in COMPRESSED code
Good news: the firmware CONTAINS NRW support, so the feature is disabled-not-absent:
  fPCM_PTP_IsFileNRW (c0bd1c6c), fPCM_PTP_ImageFormatsWithNRW, g_usPCM_PTP_ImageFormatsWithNRW
  (c0dd11f0), and a UI asset gUI_ColorPaletteList16_NRW_CURSOR_Icon_0. Shared Coolpix codebase
  (P7700/P7800-class) with NRW compiled in.
Bad news for a pure-file patch: those VAs (0xc0bd.. / 0xc0dd..) live in the main-app
segments, which are the HW-inflate ("type-2") COMPRESSED blocks — all 646 blocks are type 2;
the loader has no stored/uncompressed passthrough. The resident uncompressed segment is only
va 0xc0000800..0xc014e858 (extracted as resident.bin) and the NRW gate is above that.
IpConfigGetFeature is the SoC HW feature mask (returns 0xffffffff) — not the NRW gate.

## The one remaining dependency (unchanged, now isolated)
To locate + flip the NRW gate you need the decompressed main app. Two routes:
  1. UART debug console (DebugConsoleMain) -> RAM-dump the running main app at its load VA.
     This gives the decompressed code to find the exact gate, AND confirms where it reads from.
  2. The HW-inflate codec crack (still the one wall; runs in silicon, not recoverable from
     the file alone).
IF the dump shows the gate is a capability byte in resident/uncompressed data or a data
table reachable in dec.bin, the patch is codec-free: edit the byte(s) + reseal.py. IF it's
a compiled code constant inside a compressed segment, you'd need to rebuild that segment
(needs the codec) OR load a patched copy another way. The RAM dump tells you which.

## What changed vs previous session
- "CRC discrepancy needs GF(2) solve" -> WRONG; it's a recomputable additive byte-sum. Done.
- Integrity is no longer a blocker. The project's sole open item is locating the NRW gate,
  which the UART RAM dump resolves (and which also sidesteps the codec entirely if the gate
  is data rather than compressed code).

## Reproduction environment (this session)
Ghidra 11.4.2 headless, MIPS:LE:32. Projects built:
  zburn.elf   (bootloader @0x450, 1876 funcs, full decompile -> zburn_all.c)
  resident.bin(uncompressed main-app segment, va 0xc0000800+0x14e058, symbols applied from map)
