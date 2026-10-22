# Zoran COACH HW-inflate format — reverse engineering status

Target: Nikon Coolpix P520 Ver.1.1 firmware (`firmware_1.bin`, 81,264,928 bytes).
Main-app segment ELFs carry Zoran hardware-inflate ("type-2") compressed payloads.

## Decryption (reproduced independently this session — VALIDATED)

Stream cipher is position-based:  `plain[i] = cipher[i] ^ K0[i & 0xffff] ^ C[(i>>16) & 0xff]`
- `K0` = the 64 KiB ciphertext block at file 0x150000 (ciphertext of an all-constant
  plaintext region; XOR-autocorrelation at lag 0x10000 hits 1.0 there).
- `C[r]` = per-64KiB-chunk constant, period 256 in the chunk index, recovered by
  mode-voting each residue class `chunk & 0xff` of `cipher_chunk ^ K0`.
- Header (first 0x20 bytes, "P520…Ver.1.1") is plaintext; restored verbatim.
Repro: `scripts/decrypt.py` → `dec.bin`. Validated: ELF at 0x360310 etc. parse, the
646-block type-2 chain closes exactly on every segment boundary, all 1292 Huffman
tables give Kraft sum = 1 (strong integrity check). dec.bin == previous session's image.

## Confirmed facts (high confidence)

### Container / block layout  (verified against firmware disassembly)
- Segment ELF @0x360310 etc: single PT_LOAD, p_offset=0x54, first block at ELF+0x54.
  memsz == sum of block `osz`. 646 type-2 blocks total across 6 segments (s1..s5b).
- Block header (16 bytes): u32 type(=2), u32 osz, u32 clen, u16 aBytes, u16 bBytes.
  Then A[] (aBytes), B[] (bBytes), C[] (clen = compressed bitstream).
- `GetCurrentBlockUncompressedSise` confirms header word[1] = uncompressed size (osz).

### Table node format  (each entry a 32-bit LE word; field split verified by tiling)
  tag = x & 3 :  3 = leaf, 1 = Q node, 0 = D node   (no tag==2)
  leaf : value = (x>>2) & 0x7fff ; topfield = x>>15
  D/Q  : a = x>>15 ; b = (x>>11)&15 ; w = (x>>2)&0x1ff
  D node: dense array children = T[w .. w+2^b-1]
  Q node: children = T[w+1 .. w+1+(a+b)-1]   (a+b children; w slot skipped)
- Tiling: every word of all 1292 tables used exactly once  → it IS a one-parent tree.

### D-node bit convention — PROVEN this session
- Across ALL 168,936 D-node leaf runs, every run of identical child words is a
  power of two AND aligned to its own length (i % run == 0). ⇒ MSB-first dense index:
  low (b−L) bits are don't-cares, high L bits select the leaf.
  Incremental code length at a D node:  L = b − log2(run);  code bits = offset >> (b−L).
- Structural selector: **D nodes always have a ≤ b; Q nodes always have a ≥ b.**
  (`a` behaves like accumulated prefix-depth, `b` like this node's dense width.)

### Symbol alphabet — RICHER THAN DEFLATE (corrects previous session)
Previous note "alphabet == DEFLATE (286 syms)" is WRONG and is the likely reason every
decode attempt failed. Real value layout (15-bit value field):
  base = value & 0x1ff (bits 0-8) ; eb = (value>>10)&7 (bits 10-12, extra-bit count) ;
  class bits = 0x200 (bit9), 0x2000 (bit13), 0x4000 (bit14).
Block-0 table A distinct-symbol classes (c14,c13,c9):
  (0,0,0) n=85  eb0  base 0..256   → literals (base<256) + EOB (base 256)
  (0,0,1) n=10  eb{0,4} base3..258 → deflate-style length codes (0x200 class)
  (0,1,0) n=161 eb0  base 1..255   → **unexplained large class** (0x2000, no extra bits)
  (0,1,1) n=10  eb{0,1,5} base6..227
  (1,0,0)/(1,0,1)/(1,1,1) smaller groups
Table B (distance): classes (0,0,0) n=23 base0..55, (0,1,0) n=6 base14..31.
Interpretation of the 0x2000 / 0x4000 classes is the open semantic question; it changes
what a decoded symbol MEANS (literal vs match-length vs ...), independent of the bit walk.

## The open problem (unchanged wall)
Reconstructed tree is a valid prefix code (tiles + Kraft=1 + prefix-free) but NO tree-walk
bit convention decodes block 0 to a plausible literal run. Exhaustively ruled out THIS session:
  Q shape models : fully-skewed unary (both polarities), balanced/complete (both child
                   orders), dense-b-bits + escape-to-skewed-tail (both escape positions)
  stream order   : per-byte msb/lsb, 16/32-bit LE word swaps
  D index order  : msb / lsb
  canonical rebuild from tree lengths, ordered by value and by deflate-index, msb/lsb
Best result: ≤1–3 leading literals before a bad back-reference. (Prev session's
"canonical → 80 bytes" NOT reproduced here; likely their lengths used a different, closer
Q-depth assignment — worth recovering.)
⇒ Strong evidence the silicon EXPANDS the compact table into its own bit-indexed lookup;
the per-bit mapping is not any simple walk of the compact tree.

## Decisive next step — read the hardware driver (located this session)
The decompressor runs from UNCOMPRESSED code (it cannot be compressed by itself). It lives
in the bootloader ELF @ file 0x450, section `.text` va 0xc00100e8 (file 0x1848), 0xa5e3c bytes.
26 refs to the inflate register block are at file 0x2a88c–0x2b3a4 (va ≈ 0xc0039124+).
  - register block base: lui 0xb800 + ori 0x3e80..0x3ef0  (0xb8003e80 … 0xb8003ef0)
  - generic reg WRITE helper: 0xc0039124(a0=reg, a1=val, a2=dbgstr, a3=line)
  - generic reg READ helper:  0xc00390a4(a0=reg, …)
  - field-decode wrappers (HW node/status → struct fields), e.g.:
      0xc0039ab8 : reg 0xb8003e94 → {bits0-5, bits8-13}
      0xc0039adc : reg 0xb8003ea4 → {0-5, 6-11, 12-17, 18-23, 24-29}  (five 6-bit fields)
      0xc0039b54 : reg 0xb8003ea8 → same 5×6-bit layout
      0xc00399bc : reg 0xb8003e84 → {bit0, bits4-9, bits12-17}
  - table loader (IP_LoadVlcTable) does NOT do a plain lw/sw copy; it iterates (ptr,count)
    entry structures and calls per-entry (e.g. loop @0xc0039d28 → 0xc0092068). Trace that
    callee + the IsrInf* bit-FIFO handlers to recover: (1) how A[0]/B[0] parameterize the HW,
    (2) the exact bit-reader order, (3) how a node word becomes a flat code. That is the
    convention. Symbol-name string tables (uncompressed) at file 0x1d76800 and 0x2025b00.

## Hardware reality (this session) — why SW disassembly can't finish it
- The inflate engine is a DMA/silicon unit. InflateProcessorCreate (bootloader, va≈0xc003af80+)
  opens 2 DCU DRAM client channels (loop @0xc003b02c, stride 0xd0) and registers ISRs; the
  engine reads the bitstream and writes decoded output to DRAM autonomously. There is no
  software bit-serial decoder, so the **bit→symbol order lives in the chip, not in code.**
- Two HW register blocks seen: control/log block lui 0xb800 + 0x3e80..0x3ef0 (wrapped by
  reg helpers write=0xc0039124(reg,val,__FILE__,line), read=0xc00390a4); a second block at
  0xb8030450/0460. The "set bit N in a bitmap" idiom (word=v>>5, bit=v&0x1f, dest|=1<<bit;
  list terminated by 0xffff) is used for HW init at 0xc003c030 — a generic mask builder,
  NOT the per-block VLC code expansion.
- The named inflate routines the earlier run cited (GetCurrentBlockUncompressedSise, IP_*,
  at main-app VAs ~0xc00caxxx with b9800024 regs) are in the MAIN-APP image, which is one
  of the COMPRESSED segments. Reading them requires already decompressing → circular. Those
  addresses are data, not code, at the same offsets in this (boot) image.
- Net: from the firmware file alone, the convention is not recoverable by static analysis.
  Realistic ways to finish: (a) RAM-dump the decompressed main app from a running P520 and
  read IP_LoadVlcTable's compact-node→VLC-memory expansion + the engine's bit-FIFO config;
  (b) drive the engine on hardware/emulation with a known table and observe the code map;
  (c) an existing Zoran COACH / COACH-era decompression tool. Pure brute force won't close it.

## Oracle anchor
Segment s1 block0 output[0] = va 0x8014e900 = symbol HceNikonCustomVega10ChInit.
(Its exact prologue bytes are a weaker crib than assumed — don't rely on `NN ff bd 27`.)

## Scripts (under scripts/, run with `python3 -I`; module `enum.py` renamed → `qenum.py`
## because it shadowed stdlib `enum` and broke capstone)
  decrypt.py      reproduce dec.bin from firmware_1.bin
  zt.py           block/table parser (hdr, tables, blocks, node, P)  [path → dec.bin]
  elfhdr.py       segment ELF program-header parser
  qesc.py qenum.py canon2.py litscan.py scan.py inf.py inf2.py   convention experiments
  (capstone 5.0.7 works for MIPS32 LE disassembly of the uncompressed .text)

## Community tools checked (2026-10-17) — NOT applicable to P520
Two camerahacking.com tools were evaluated and ruled out; both target CVS / Pure Digital
"one-time-use" camcorders, which share only the Zoran-COACH SoC lineage with the P520:
- OneTimeBrute 1.4: an RC2-40 CBC brute-forcer for the camcorder challenge/response UNLOCK
  key (OpenSSL + a bundled RC2). Nothing to do with firmware compression.
- CamcorderKit19.exe: UPX-packed Inno-Setup installer. Unpacked (upx -d) and carved the
  Inno overlay (deflate streams @ overlay 0x21000+). The real GUI tool (376 KB PE) does
  serial/USB flash download-upload + camcorder unlock (RC2 keys); it links STANDARD zlib
  1.1.3 (Mark Adler) — "invalid block type / incorrect header check" etc. The only
  "decompress/block" symbols in the other PE are Inno's own TLZMADecompressor runtime.
Conclusion: these handle SOFTWARE zlib CVS firmware. The P520 uses the SoC's HARDWARE
inflate engine with the custom A/B VLC + type-2 bitstream — a different format. No reuse.

## Zburn recovery / firmware-update verification (2026-10-17) — KEY for patch deployment
A full embedded nm-style symbol map (72,760 syms) was extracted → scripts/p520_symbols.map.
It gives addresses for every function. Resident .text range is [0xc00100e8,0xc00b5f24]
(file = va - 0xc00100e8 + 0x1848); addresses above that are in the compressed main app.

Firmware-update / Zburn burn path verifies images with **MD5 only — no crypto signature**:
- md5_init c005a500, md5_append c005a53c, md5_finish c005a680, DebugMD5sum c00624b4
  (all RESIDENT, confirmed readable MIPS). DebugMD5ResultCompare, BurnHash{Init,Update,
  Fanalize,Manager,Exit} (= md5 wrappers, in main app). Strings: "Verify binary data...",
  "ERROR: Hash verify failed on binary data!", "Hash verify success!", "Received hash" vs
  "Calculated hash", "Validate total size %d".
- Crypto-signature search over the whole image: no RSA/ECDSA/HMAC/cert in the burn/download
  path (only incidental 'RSA'/'AES' strings buried in unrelated compressed data). So image
  verification is INTEGRITY (hash compare), not signed → a patched image passes if its MD5
  is recomputed and written back. No signing key to forge.
- Zburn subsystem: Zburn/Download/{Usb,Uart,Card}, DownloadUsb/Uart/Card, Burn{Nand,Nor,
  OneNand,SdMmc,LbaNand,Ide}; a "Recover Area" partition with its own boot header
  (PrintRecoverHeaderProperties, InitRecoverSection, "Sart Burn from Recover Area").
  Download sources: USB, UART, SD/MMC/MS/CF — USB and SD are no-solder.

IMPLICATION for the stuck RAW patch: memory notes a GF(2)/Gaussian-elimination attack on a
"CRC discrepancy". MD5 is NON-LINEAR — GF(2) cannot forge it. If the rejecting check is this
MD5 (or includes it), the fix is to RECOMPUTE the MD5 after patching and write it into the
image's hash field, not solve a linear system. Possible there are two layers (a container
CRC that GF(2) handles + a payload MD5 that must be recomputed). Next step: reverse the image
-header struct (qafCreateImageHeader c0723040; Recover header) + DebugMD5ResultCompare to pin
exactly which bytes hold the expected MD5 and what byte-range it covers.

Note: resident inflate exists too (CreateInflater c00a044c, InflateElfStream c00a0590 are in
.text), while IP_LoadVlcTable/InflateProcessorCreate are main-app (compressed) — so the boot
decompressor may be readable after all; revisit the "pure silicon" conclusion via InflateElfStream.
