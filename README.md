# Nikon COOLPIX P520 firmware RE

Target: P520 v1.1 SD-update image `firmware.bin` (81,264,928 B, sha256 `631a4c6a…979c`).
Goal: enable RAW (NRW) stills.

## Status

| piece | state |
|---|---|
| Decryption (position XOR stream) | solved |
| Integrity (32-bit byte-sum @ 0x4d80110) | solved |
| HW-inflate codec (Zoran COACH type-2) | **solved**: it's DEFLATE with MSB-first bits and canonical codes implied by HW lookup tables. Decode, bit-exact re-encode and bit-exact table rebuild for all 646 blocks. See [docs/CODEC_SOLVED.md](docs/CODEC_SOLVED.md) |
| Patch pipeline | **working**: patch any main-app address, recompress in place, reseal, re-encrypt (`tools/p520fw.py`) |
| RAW gate | **SOLVED + confirmed on hardware**: two edits enable NRW (menu stores a RAW quality + neuter the RAW→JPEG conversion). The camera shoots valid `.NRW`. See [docs/RAW_ENABLE.md](docs/RAW_ENABLE.md). `RAW_GATE.md` is the earlier (wrong) menu-link guess. |
| NRW desktop use | `.NRW` needs a one-time endianness/bit-depth fix for standard converters — [tools/nrwfix.py](tools/nrwfix.py). |

## Quick start

    pip install numpy
    python3 tools/p520fw.py selftest firmware.bin            # prove codec on your image
    python3 tools/p520fw.py unpack   firmware.bin out/       # dec.bin + decompressed RAM image
    python3 tools/p520fw.py patch    firmware.bin new.bin 0xc0XXXXXX=HEX ...

`out/ram_c0000000.bin` is the whole main app at 0xc0000000 (vaddr = pa − 0x40000000). Load it
into Ghidra as raw MIPS:LE:32 at base 0xc0000000 and run `reverse/ApplyAll.java
p520_symbols.map` (72,760 symbols) to get named functions; `reverse/DecompNames.java OUT name…`
decompiles by name.

`p520_re_complete.zip` holds the earlier sessions' material (decompiles, symbol map,
experiments, notes). Its notes on an "unsolvable codec" and a "0x2000 opcode class" are
superseded by `docs/CODEC_SOLVED.md`.
