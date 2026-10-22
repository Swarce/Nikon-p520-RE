# Zoran COACH "type-2" HW-inflate codec — SOLVED (2026-10-22)

The compressed main application of the P520 v1.1 image (646 type-2 blocks in 6 segments,
18.4 MB decompressed) now decodes completely in software, re-encodes bit-for-bit, and every
hardware Huffman table is rebuilt bit-for-bit from code lengths. Reference implementation:
`tools/p520fw.py` (`selftest` proves all three claims against `firmware.bin`).

## It is DEFLATE

Semantically the format is raw DEFLATE (dynamic Huffman, 32 KiB window) with:

* **MSB-first** bit packing (DEFLATE is LSB-first): codes and extra bits are read high bit first.
* **Canonical** Huffman codes (exactly DEFLATE's assignment: sort by length, then symbol).
* The code lengths are not sent in the stream; they are implied by the A/B lookup tables
  that the driver loads into the engine (`IP_LoadVlcTable` copies the words verbatim to
  `0xb9800024`).
* One block header per block; the window runs continuously across all blocks of a segment.
* The bitstream is zero-padded to `clen`, and `clen` is always a multiple of 128 bytes.
  The driver halts with "Input buffer size doesn't meet alignment" otherwise (`IP_Execute`).

## Block layout

    u32 type (=2) | u32 osz | u32 clen | u16 aBytes | u16 bBytes | A[aBytes] | B[bBytes] | C[clen]

## Symbol alphabets

* **A (literal/length):** exactly DEFLATE's 286. A leaf value `v` (13 bits) is
  `v < 0x100` literal, `0x100` end-of-block, else `0x200 | base | extra<<10` for the 29 DEFLATE length codes.
* **B (distance):** the 30 DEFLATE distance codes, stored as `v = (extra_bits << 2) | top2`.
  So `dist = (top2 << extra) + extra_value + 1` (and `dist = v + 1` for `v < 4`).

The "0x2000 opcode class" from earlier sessions does not exist. It came from reading the leaf
value as `(x>>2)&0x7fff`, which overlaps the leaf's separate `x>>15` field.

## Table (tree) format

Each table is an array of 32-bit LE words: `tag = x&3`.

| tag | kind | fields |
|---|---|---|
| 3 | leaf | `value=(x>>2)&0x1fff`, `x>>15 = (code bits past parent's a) + extra bits` |
| 0 | D (dense) | `a=x>>15` (shortest code in node), `b=(x>>11)&15`, `w=(x>>2)&0x1ff`; children `T[w .. w+2^b-1]`, index = next b bits MSB-first; a run of r identical leaves is one code of length `b - log2 r` |
| 1 | Q (two spines) | `a=x>>15`, `b=(x>>11)&15`, `w`; children `T[w+1 .. w+a+b]` |

A **Q-node** is the node's left and right spines. Bit 0 enters the left spine, where every further 0
continues and a 1 exits to the next child. Bit 1 enters the right spine, where every further 1
continues and a 0 exits:

    left  (b children): 0 1, 0 01, 0 001, ..., 0 0^(b-1)        (b=1: just "0")
    right (a children): 1 0, 1 10, 1 110, ..., 1 1^(a-1)        (a=1: just "1")

Walking the trees with these rules reproduces the canonical codes for all 1292 tables.

### Encoder's construction rules (bit-exact on all 1292 tables)

* Non-root subtree: a Q-node if both of its spines carry only leaves; otherwise a dense D-node
  covering the whole subtree (`a` = shallowest leaf, `b` = deepest leaf).
* Root: a Q-node over the two spines, or a dense D-node of some width `b` with sub-tables
  hung under it, whichever table is **smaller** (Q wins ties; the smallest such `b` wins).
* Layout: a running array. A Q-node's children go at `w+1..` (so `w` overlaps the previous
  entry); a D-node's at `w..`. Root sub-tables are allocated from the end of each spine
  back toward the root (left spine first), or in slot order under a D root.
* Leaf top field: under a D-node, `L - a + extra_bits`; under a Q-node, `extra_bits`.
* Code lengths are ≤ 13 (A) and ≤ 12 (B) in the shipped image.

## Memory map (decompressed), from the main-app ELF program headers

| pa | size | kind |
|---|---|---|
| c0000180 | 0x1a0 | plain (exception vectors) |
| c0000600 | 0x200 | plain |
| c0000800 | 0x14e058 | plain (resident; = reverse/resident.bin) |
| c014e900 | 0x3b5300 | type-2 (s1) |
| c0503c80 | 0x367800 | type-2 (s2) |
| c086b500 | 0x12a880 | type-2 (s3) |
| c0995e00 | 0x13680 | type-2 (s4) |
| c09a9500 | 0x88b180 | type-2 (s5a) |
| c1234680 | 0xa67ac | type-2 (s5b) |

Virtual address = pa − 0x40000000 (s1 loads at 0x8014e900 = `HceNikonCustomVega10ChInit`).

## Patching

`p520fw.py patch firmware.bin out.bin 0xc0XXXXXX=HEX ...` edits the decompressed image and
re-encodes only the affected blocks by token surgery. Each block keeps its tables and its
`clen`, so nothing else in the file moves. It re-decodes everything to verify, fixes the byte-sum,
and re-encrypts. A no-op patch reproduces `firmware.bin` byte-for-byte.

Limitation: a block can only emit byte values that its table A has a code for (190/646 blocks
have all 256). If a patch needs a missing byte, the block's tables must be rebuilt
(`build_table`), and the new `aBytes+bBytes` must keep the block footprint and `clen % 128 == 0`.
That fallback is not wired into `patch` yet.
