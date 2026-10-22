# Zoran COACH type-2 inflate — codec RE status after routes C & D (2026-10-19)

## Route D (external prior art): EXHAUSTED — no public source
Searched: Zoran COACH datasheets, Zoran-assignee lossless/code-decompression patents,
the exact SDK source-path strings ("Os/Inflate/HwAcceleration", "Zcam/Zboot/Inflater"),
and the camera-hacking communities. Findings:
- No public Zoran COACH programming manual / SDK with a software reference decoder.
- Communities that touched COACH (Pure Digital/Flip, camerahacking.com) used SOFTWARE zlib —
  a DIFFERENT format. Nikon-hacking (nikonhacker, simeonpilgrim/nikon-firmware-tools, CHDK)
  targets Expeed/DIGIC DSLRs, never the COACH Coolpix HW-inflate code format.
- "Zoran + inflate" online = the unrelated ZR360xx PCI MJPEG capture driver.
- VLC/instruction-decompression patents found are Intel/other or Zoran's DCT/JPEG image line,
  not this LZ+Huffman program-code compressor.
=> The format is undocumented publicly. No shortcut via docs.

## Route C (crack from the file): MAJOR STRUCTURAL ADVANCE, final convention still blocked
### SOLVED this session: the Q-node traversal (was the stated wall)
Using Kraft-sum = 1 as a bit-order-independent oracle, the Q-node is a **balanced prefix
code over its nch=a+b children, minimum code length b**:
  #children at b bits   = 2^(b+1) - nch      (direct slots)
  #children at b+1 bits = 2*nch - 2^(b+1)     (reached via escape slots)
Verified exactly on every Q shape in block 0:
  Q(1,1)->2@1 ; Q(2,1)->1@1+2@2 ; Q(2,2)->4@2 ; Q(13,4)->15@4+2@5.  Whole-tree Kraft=1.000000.
Decode rule: read b bits; first (2^(b+1)-nch) slots are direct children; each remaining slot
is an escape -> read 1 more bit to pick between its 2 deep children.
(D-node already known: read b bits MSB dense; leaf run r => code length = depth + b - log2 r.)

### Also nailed down
- Effective alphabet ~ **285 distinct symbols, max code length 10** (DEFLATE-scale 286).
  Strongly implies a DEFLATE-like lit/len(+EOB 256) + distance scheme, richer by the
  0x2000 class (161 raw syms, base 1..255, no extra bits) whose MEANING is still open.
- Full decoder harness built (scripts/infl2.py, infl3.py) with a **MIPS-32 validity oracle**
  (seg s1 blk0 output must disassemble at load VA 0x8014e900). Sweeps bit order
  (byte/16/32-bit word x msb/lsb), Q escape ordering, and mystery-class semantics.

### Where it still stalls (the irreducible coupling)
Every combination decodes only 2-3 literals, then the first match symbol yields an
out-of-range distance -- identical failure mode to all prior sessions. Remaining coupled
unknowns, none separable without a ground-truth block:
  1. Which children in a Q node get the short vs long code (array order? reversed?) and the
     escape-slot index mapping.
  2. Semantics of the 0x2000 class (literal? extra length alphabet? something else).
  3. The exact length/distance base tables + extra-bit application + stream bit-FIFO order.
There is NO internal oracle until ~16+ decoded instructions exist to disassemble, and we
can't reach that depth while any one of the above is wrong -> chicken-and-egg.

## Why this is now a cheap finish with ONE ground-truth block
The harness already scores candidate conventions by MIPS validity. Give it ONE known
(compressed block -> decompressed bytes) pair from a RAM dump (route A) and the search
collapses: the output bytes become a full pass/fail oracle, so child-ordering, the 0x2000
class, and the base tables all resolve by direct constraint. Equivalently, a chosen-plaintext
run on the silicon (route B) via the now-fully-mapped engine (regs @0xb9800000, DCU DMA,
EOB/EOI/EOO IRQs) nails it and validates re-encoding.

## Bottom line
- Q-node structure: SOLVED (new).  Alphabet: ~286 DEFLATE-scale (new).  Engine I/O: fully mapped.
- Final per-bit convention: NOT recoverable by unguided file-only search (now shown rigorously
  from 3 principled angles), but the built harness turns a single RAM-dump block into an
  immediate, automatic crack. That one dump is the whole remaining dependency.

## Files
  scripts/infl2.py, scripts/infl3.py   decoder harness + MIPS oracle (edit block index to retarget)
  scripts/reseal.py                     decrypt/patch/recompute-byte-sum/re-encrypt (integrity solved)

## ADDENDUM — "try everything" file-only assault (2026-10-19, session 2b)
Pushed hard with new oracles. Real advances, but the wall is now pinned to ONE unknown.

### New confirmed facts
1. **Length alphabet = DEFLATE, exactly.** Tree A's length leaves reproduce DEFLATE's 29
   (base,extra-bit) pairs 29/29 under value layout base=val[0:9], eb=val[10:13]. So the
   lit/len side is DEFLATE-family: literals class(0,0,0) base 0-255, EOB base 256,
   lengths class(0,0,1) base 3-258 + extra bits. This validates the leaf bit-layout.
2. **Distance tree B is NOT DEFLATE.** B leaves are small (base 0-55, ZERO extra bits;
   plus a c13/0x2000 subset base 14-31). Only 2/30 match DEFLATE distances. Implies either
   a SMALL sliding window or a custom distance mapping. (A 3-bit eb field can't express
   DEFLATE's up-to-13 distance extra bits, corroborating a small/custom window.)
3. **Decoder MUST advance by true code length, not by b.** D-nodes have run leaves
   (e.g. 4 identical slots in a b=5 table = a 3-bit code). Correct decode: PEEK b bits,
   find leaf, advance only by L=b-log2(run), push back the rest. Consuming all b bits
   (previous approach) desyncs right after the first direct literals. Fixed in the harness.
4. **Oracle lesson:** "disassembles as MIPS" is fooled by repeat-pattern desyncs (a len-N
   dist-4 match yields one instruction x50, scoring 100%). Must score by DISTINCT valid
   instructions + byte entropy. With that gate, NO candidate decodes block 0 to real code.

### Search space swept (all combined, nothing passed the diversity+entropy oracle)
bit order {byte,16,32-bit}x{LE,BE}x{MSB,LSB}; dense index direct vs BIT-REVERSED;
Q escape-order direct-first vs escape-first; run-length-aware advance; distance = base+1 /
base+256*c13+1 / base+extrabits+1; bitstream skip 0..32; targets = block0 (code) AND the
most-compressible block (ratio 48:1, RLE-dominated). Full-block-consumption + MIPS-validity
+ distinct-instruction/entropy oracles.

### Where the wall now sits — a single model unknown, not a parameter
The ONLY large unexplained element is the **0x2000 class (c13=1): 168 symbols in tree A
(base 1-255, no extra bits) and a subset in tree B.** Everything desyncs the moment one
appears, because its ACTION is unknown (candidate meanings: a 2nd literal bank, a
"repeat-last-distance", a run/RLE op, a distance high-part, a near-pointer...). Its meaning
is not guessable from structure — it needs one ground-truth (compressed->decompressed) pair
to disambiguate. More blocks don't help (same model gap in all); the issue is semantic, not
statistical. This is the crisp residual dependency for routes A/B.

### Net after C&D
Format is DEFLATE-family (lit/len proven) with a custom small-distance scheme and one
unknown 0x2000 opcode-class. Q-node, length codes, leaf layout, and the advance rule are
SOLVED. The harness (scripts/search2.py + diversity oracle) will confirm the full convention
automatically the instant a single block's true output is available.

## ADDENDUM 2 — sibling firmwares P530 & P600 (2026-10-19)
Decrypted both with the same position-XOR cipher (auto-found K0): P530 v1.1 (39.6MB,
K0@0x130000), P600 v1.2 (48.5MB, K0@0x160000). Both validated (hcemain.elf, ELF magics).
- SAME codec: both carry type-2 HW-inflate segments (P600: 453 blocks; P530: type-2 too,
  interspersed with .mod relocation tables at different load VAs ~0x842xxxxx/0x846/0x84b).
- NEITHER ships the main app uncompressed -> no free known-plaintext.
- Cross-firmware byte-matching: P530 segment content not found in P520 (different builds).
- Net: siblings PROVE the codec is shared and add ~1000 blocks of constraint data, but do
  NOT provide a verified (compressed->decompressed) pair. The model gap (0x2000 class +
  exact bit convention) is unchanged.

### Verdict on file-only cracking (after P520+P530+P600, all oracles)
NOT crackable from compressed firmware alone. Confirmed structure: DEFLATE-exact length
alphabet, Kraft-solved Q-node, run-length-advance rule, small-value distance tree, ~286
symbols. Irreducible unknown: the 0x2000 opcode-class ACTION (swept lit/rle/direct-len/
repeat-dist — none give a clean high-entropy full-block decode). Needs ONE ground-truth
block, obtainable only by: (a) hardware RAM dump / chosen-plaintext on the engine; (b) an
external firmware/SDK that ships a SOFTWARE reference decoder for this exact Zoran COACH
format; or (c) any model that ships the shared code uncompressed (none of P520/P530/P600 do).
