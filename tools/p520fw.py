#!/usr/bin/env python3
"""Nikon COOLPIX P520 firmware tool: decrypt, decompress, patch, recompress, reseal.

    p520fw.py unpack  firmware.bin OUTDIR          # dec.bin, seg_*.bin, ram_c0000000.bin
    p520fw.py patch   firmware.bin out.bin ADDR=HEX [ADDR=HEX ...]
    p520fw.py verify   firmware.bin                # byte-sum + decode every block
    p520fw.py selftest firmware.bin                # decode, bit-exact re-encode + table rebuild

ADDR is a main-app address as in p520_symbols.map (0xc0xxxxxx; 0x80xxxxxx is accepted too).
`patch` edits the decompressed image, re-encodes only the affected compressed blocks by
token surgery (every block keeps its Huffman tables and its clen, so the file layout does
not move), re-decodes the whole image to prove the result, fixes the byte-sum and re-encrypts.

Needs data/K0.bin and data/C.npy (cipher) from this repo. Pure Python + numpy.
"""
import os, sys, struct
import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
DATA = os.path.join(os.path.dirname(HERE), 'data')

# ---------------------------------------------------------------- cipher / checksum
CKSUM_START, CKSUM_OFF, HDR_PLAIN = 0x50, 0x4d80110, 0x20

def keystream(n):
    K0 = np.fromfile(os.path.join(DATA, 'K0.bin'), np.uint8)
    C = np.load(os.path.join(DATA, 'C.npy'))
    nch = (n + 0xffff) // 0x10000
    ks = np.tile(K0, nch)[:n].copy()
    ks ^= np.repeat(C[np.arange(nch) & 255], 0x10000)[:n]
    return ks

def decrypt(enc):
    enc = np.frombuffer(enc, np.uint8)
    d = enc ^ keystream(len(enc))
    d[:HDR_PLAIN] = enc[:HDR_PLAIN]
    return bytearray(d.tobytes())

def encrypt(dec, header):
    e = np.frombuffer(bytes(dec), np.uint8) ^ keystream(len(dec))
    e[:HDR_PLAIN] = np.frombuffer(header[:HDR_PLAIN], np.uint8)
    return e.tobytes()

def byte_sum(dec):
    return int(np.frombuffer(bytes(dec[CKSUM_START:CKSUM_OFF]), np.uint8).sum(dtype=np.uint64)) & 0xffffffff

def reseal(dec):
    v = byte_sum(dec)
    dec[CKSUM_OFF:CKSUM_OFF + 4] = v.to_bytes(4, 'little')
    return v

# ---------------------------------------------------------------- container
def segments(D):
    """Main-app ELF segments (entry 0x80000600): (file_start, file_end, pa, memsz, compressed)."""
    segs, o = [], 0
    while True:
        o = D.find(b'\x7fELF\x01\x01\x01', o + 1)
        if o < 0 or o > 0x1000000:
            break
        entry, phoff = struct.unpack('<II', D[o + 24:o + 32])
        phnum = struct.unpack('<H', D[o + 44:o + 46])[0]
        if entry != 0x80000600:
            continue
        for i in range(phnum):
            p = struct.unpack('<8I', D[o + phoff + i * 32:o + phoff + i * 32 + 32])
            if p[0] == 1 and p[4] > 0x100:
                packed = p[4] != p[5] and D[o + p[1]:o + p[1] + 4] == b'\x02\0\0\0'
                segs.append((o + p[1], o + p[1] + p[4], p[3], p[5], packed))
    return segs

# ---------------------------------------------------------------- codec
# Zoran COACH "type-2" HW-inflate format (cracked 2026-10-22):
#  block = u32 type(2), u32 osz, u32 clen, u16 aBytes, u16 bBytes, A[], B[], C[clen]
#  A (lit/len) and B (dist) are HW lookup trees; they only carry the CODE LENGTHS:
#    leaf tag3: value=(x>>2)&0x1fff, x>>15 = (code bits past node.a) + extra bits
#    D    tag0: a=x>>15 (min len), b=(x>>11)&15, w=(x>>2)&0x1ff; children T[w..w+2^b-1];
#               a run of r equal leaves = one code of length b-log2(r)
#    Q    tag1: children T[w+1..w+a+b]; first b form a unary chain under one bit, the
#               next a a unary chain under the other (chain of n: lengths 1..n-1, n-1)
#  Codes are DEFLATE-canonical over DEFLATE alphabets (lit 0-255, EOB, 29 length codes;
#  B value v=(extra<<2)|top2 = the 30 DEFLATE distance codes). Bitstream, codes and
#  extra bits are all MSB-first. The 32 KiB window runs across all blocks of a segment.
#  The stream is zero-padded to clen (always a multiple of 128 bytes).
LB = [3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258]
LE = [0] * 8 + [1] * 4 + [2] * 4 + [3] * 4 + [4] * 4 + [5] * 4 + [0]
DB = [1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537,
      2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577]
DE = [0, 0, 0, 0] + [i // 2 - 1 for i in range(4, 30)]

def tree_lengths(T):
    out = {}
    def chain(n):
        return list(range(1, n)) + [n - 1] if n > 1 else [0]
    def rec(i, d):
        x = T[i]
        if x & 3 == 3:
            out[(x >> 2) & 0x1fff] = d
            return
        a, b, w = x >> 15, (x >> 11) & 15, (x >> 2) & 0x1ff
        if x & 3 == 0:
            j = 0
            while j < (1 << b):
                k = j
                while k + 1 < (1 << b) and T[w + k + 1] == T[w + j] and T[w + j] & 3 == 3:
                    k += 1
                rec(w + j, d + b - (k - j + 1).bit_length() + 1)
                j = k + 1
        else:
            for j, l in enumerate([1 + l for l in chain(b)] + [1 + l for l in chain(a)]):
                rec(w + 1 + j, d + l)
    rec(0, 0)
    return out

def canon(lmap):
    items = sorted((l, s) for s, l in lmap.items())
    out, code, pl = {}, -1, items[0][0]
    for l, s in items:
        code = (code + 1) << (l - pl)
        pl = l
        out[s] = (l, code)
    return out

# --- table builder: canonical codes -> HW tree words, bit-exact with Nikon's encoder
#     (reproduces all 1292 tables of the P520 v1.1 image).
#  Non-root subtree: Q-node if both of its spines carry only leaves, else a dense D-node
#  (a = shallowest leaf, b = deepest leaf). Root: Q over the two spines, or a dense D-node of
#  any width b with sub-tables hung below it, whichever gives the smaller table (Q wins ties,
#  smallest b wins among D). Leaf x>>15 = (code bits past the parent's a) + extra bits.
def _trie(codes):
    root = [None, None]
    for s, (l, c) in codes.items():
        n = root
        for i in range(l - 1, -1, -1):
            bit = (c >> i) & 1
            if i == 0:
                n[bit] = ('L', s)
            else:
                if n[bit] is None:
                    n[bit] = [None, None]
                n = n[bit]
    return root

def _isleaf(n):
    return isinstance(n, tuple)

def _spine(n, side):
    kids, cur = [], n[side]
    while not _isleaf(cur):
        kids.append(cur[1 - side])
        cur = cur[side]
    return kids + [cur]

def _depths(n, d=0):
    return [d] if _isleaf(n) else _depths(n[0], d + 1) + _depths(n[1], d + 1)

def _dense(n, b):
    out = []
    def rec(m, d):
        if _isleaf(m) or d == b:
            out.extend([(m, d)] * (1 << (b - d)))
        else:
            rec(m[0], d + 1)
            rec(m[1], d + 1)
    rec(n, 0)
    return out

def _leafvalue(sym, alphabet):
    if alphabet == 'A':
        if sym < 257:
            return sym, 0
        return 0x200 | (LB[sym - 257] & 0x1ff) | (LE[sym - 257] << 10), LE[sym - 257]
    return (sym if sym < 4 else (DE[sym] << 2) | 2 | (sym & 1)), 0

def build_table(codes, alphabet):
    """codes: sym -> (len, code) canonical; alphabet 'A' (lit/len) or 'B' (dist). -> list of words."""
    t = _trie(codes)
    def leaf(m, top):
        v, eb = _leafvalue(m[1], alphabet)
        return 3 | (v << 2) | ((top + eb) << 15)
    def subtable(T, m):
        s0, s1 = _spine(m, 0), _spine(m, 1)
        if all(_isleaf(k) for k in s0 + s1):
            w = len(T) - 1
            T.extend(leaf(k, 0) for k in s0 + s1)
            return 1 | (w << 2) | (len(s0) << 11) | (len(s1) << 15)
        ds = _depths(m)
        a, b = min(ds), max(ds)
        w = len(T)
        T.extend(leaf(k, L - a) for k, L in _dense(m, b))
        return (w << 2) | (b << 11) | (a << 15)
    def root_q():
        s0, s1 = _spine(t, 0), _spine(t, 1)
        ks = s0 + s1
        T = [1 | (len(s0) << 11) | (len(s1) << 15)] + [None] * len(ks)
        for j, k in enumerate(ks):
            if _isleaf(k):
                T[1 + j] = leaf(k, 0)
        order = list(range(len(s0) - 1, -1, -1)) + list(range(len(ks) - 1, len(s0) - 1, -1))
        for j in order:
            if not _isleaf(ks[j]):
                T[1 + j] = subtable(T, ks[j])
        return T
    def root_d(b):
        slots = _dense(t, b)
        leaves = [L for k, L in slots if _isleaf(k)]
        a = min(leaves) if leaves else b
        T = [(1 << 2) | (b << 11) | (a << 15)] + [None] * len(slots)
        j = 0
        while j < len(slots):
            k, L = slots[j]
            r = 1 << (b - L)
            if _isleaf(k):
                T[1 + j:1 + j + r] = [leaf(k, L - a)] * r
            else:
                T[1 + j] = subtable(T, k)
            j += r
        return T
    best = root_q()
    maxlen = max(l for l, c in codes.values())
    for b in range(1, maxlen + 1):
        T = root_d(b)
        if len(T) < len(best):
            best = T
    return best

class Block:
    def __init__(self, D, off):
        t, self.osz, self.clen, a, b = struct.unpack('<IIIHH', D[off:off + 16])
        assert t == 2, 'not a type-2 block at %#x' % off
        self.off = off
        self.A = struct.unpack('<%dI' % (a // 4), D[off + 16:off + 16 + a])
        self.B = struct.unpack('<%dI' % (b // 4), D[off + 16 + a:off + 16 + a + b])
        self.cstart = off + 16 + a + b
        self.C = bytes(D[self.cstart:self.cstart + self.clen])
        la = {(v if not v & 0x200 else 257 + LB.index(v & 0x1ff)): l for v, l in tree_lengths(self.A).items()}
        lb = {(v if v < 4 else 2 * (v >> 2) + 2 + (v & 1)): l for v, l in tree_lengths(self.B).items()}
        self.ca, self.cb = canon(la), canon(lb)
        self.end = self.cstart + self.clen

    def tokens(self, hist_len):
        da = {v: s for s, v in self.ca.items()}
        db = {v: s for s, v in self.cb.items()}
        n = len(self.C) * 8
        S = int.from_bytes(self.C, 'big')
        p = 0
        toks, produced = [], 0
        def get(k):
            nonlocal p
            v = (S >> (n - p - k)) & ((1 << k) - 1)
            p += k
            return v
        def sym(d):
            code = 0
            for L in range(1, 16):
                code = (code << 1) | get(1)
                s = d.get((L, code))
                if s is not None:
                    return s
            raise ValueError('bad code in block @%#x' % self.off)
        while True:
            s = sym(da)
            if s < 256:
                toks.append(s)
                produced += 1
                continue
            if s == 256:
                break
            ln = LB[s - 257] + (get(LE[s - 257]) if LE[s - 257] else 0)
            d = sym(db)
            dist = DB[d] + (get(DE[d]) if DE[d] else 0)
            if dist > produced + hist_len:
                raise ValueError('distance out of window in block @%#x' % self.off)
            toks.append((ln, dist))
            produced += ln
        if produced != self.osz:
            raise ValueError('size mismatch in block @%#x' % self.off)
        if any(self.C[(p + 7) // 8:]):
            raise ValueError('non-zero padding in block @%#x' % self.off)
        return toks

    def encode(self, toks):
        acc, nb = 0, 0
        def put(v, k):
            nonlocal acc, nb
            acc = (acc << k) | v
            nb += k
        for t in toks:
            if isinstance(t, int):
                if t not in self.ca:
                    raise KeyError('byte %#04x has no code in block @%#x' % (t, self.off))
                put(self.ca[t][1], self.ca[t][0])
                continue
            ln, dist = t
            li = 28 if ln == 258 else max(i for i in range(28) if LB[i] <= ln)
            put(self.ca[257 + li][1], self.ca[257 + li][0])
            if LE[li]:
                put(ln - LB[li], LE[li])
            di = max(i for i in range(30) if DB[i] <= dist)
            if di not in self.cb:
                raise KeyError('distance code %d has no code in block @%#x' % (di, self.off))
            put(self.cb[di][1], self.cb[di][0])
            if DE[di]:
                put(dist - DB[di], DE[di])
        put(self.ca[256][1], self.ca[256][0])
        pad = -nb % 8
        put(0, pad)
        out = acc.to_bytes((nb + pad) // 8, 'big')
        if len(out) > self.clen:
            raise OverflowError('block @%#x: %d bytes > clen %d' % (self.off, len(out), self.clen))
        return out + bytes(self.clen - len(out))

def expand(toks, out):
    """Append the expansion of toks to bytearray out (which holds the history)."""
    for t in toks:
        if isinstance(t, int):
            out.append(t)
        else:
            ln, dist = t
            s = len(out) - dist
            if dist >= ln:
                out += out[s:s + ln]
            else:
                for i in range(ln):
                    out.append(out[s + i])

def segment_blocks(D, seg):
    s, e, pa, msz, packed = seg
    blocks, o = [], s
    while o < e:
        b = Block(D, o)
        blocks.append(b)
        o = b.end
    return blocks

def decode_segment(D, seg):
    if not seg[4]:
        return bytes(D[seg[0]:seg[1]]), []
    out = bytearray()
    toks = []
    for b in segment_blocks(D, seg):
        t = b.tokens(min(len(out), 32768))
        toks.append(t)
        expand(t, out)
    return bytes(out), toks

# ---------------------------------------------------------------- patching
def fix_tokens(toks, start, target):
    """Rewrite toks (producing target[start:...]) so they reproduce target. Matches that would
    copy wrong bytes are split; wrong stretches become literals. Returns (new_toks, changed)."""
    new, pos, changed = [], start, False
    for t in toks:
        if isinstance(t, int):
            if target[pos] != t:
                new.append(target[pos])
                changed = True
            else:
                new.append(t)
            pos += 1
            continue
        ln, dist = t
        ok = [target[pos + i] == target[pos + i - dist] for i in range(ln)]
        if all(ok):
            new.append(t)
        else:
            changed = True
            i = 0
            while i < ln:
                j = i
                while j < ln and ok[j] == ok[i]:
                    j += 1
                if ok[i] and j - i >= 3:
                    new.append((j - i, dist))
                else:
                    new.extend(target[pos + i:pos + j])
                i = j
        pos += ln
    return new, changed

def patch_segment(D, seg, edits):
    """edits: {offset_in_segment: byte}. Re-encodes affected blocks in place in D."""
    blocks = segment_blocks(D, seg)
    orig = bytearray()
    toks = []
    for b in blocks:
        t = b.tokens(min(len(orig), 32768))
        toks.append(t)
        expand(t, orig)
    target = bytearray(orig)
    for o, v in edits.items():
        target[o] = v
    pos, touched = 0, []
    for b, t in zip(blocks, toks):
        nt, changed = fix_tokens(t, pos, target)
        if changed:
            D[b.cstart:b.end] = b.encode(nt)
            touched.append(b.off)
        pos += b.osz
    return touched, bytes(target)

def parse_edits(args):
    edits = {}
    for a in args:
        addr, hx = a.split('=')
        addr = int(addr, 0)
        if addr & 0xe0000000 == 0x80000000:
            addr |= 0x40000000
        for i, v in enumerate(bytes.fromhex(hx)):
            edits[addr + i] = v
    return edits

# ---------------------------------------------------------------- CLI
def cmd_unpack(fw, outdir):
    os.makedirs(outdir, exist_ok=True)
    D = decrypt(open(fw, 'rb').read())
    open(os.path.join(outdir, 'dec.bin'), 'wb').write(D)
    print('byte-sum stored %#010x computed %#010x' %
          (struct.unpack('<I', D[CKSUM_OFF:CKSUM_OFF + 4])[0], byte_sum(D)))
    ram = bytearray(0x1300000)
    for seg in segments(D):
        if not seg[2] & 0x40000000 or seg[2] >= 0xc1300000:
            continue
        data, _ = decode_segment(D, seg)
        assert len(data) == seg[3]
        open(os.path.join(outdir, 'seg_%08x.bin' % seg[2]), 'wb').write(data)
        ram[seg[2] - 0xc0000000:seg[2] - 0xc0000000 + len(data)] = data
        print('segment pa %#010x  %8d bytes  %s (file %#x..%#x)' %
              (seg[2], len(data), 'compressed' if seg[4] else 'plain     ', seg[0], seg[1]))
    open(os.path.join(outdir, 'ram_c0000000.bin'), 'wb').write(ram)
    print('wrote', outdir)

def cmd_patch(fw, out, args):
    enc = open(fw, 'rb').read()
    D = decrypt(enc)
    edits = parse_edits(args)
    segs = segments(D)
    expected = {}
    for seg in segs:
        mine = {a - seg[2]: v for a, v in edits.items() if seg[2] <= a < seg[2] + seg[3]}
        if not mine:
            continue
        if not seg[4]:
            for o, v in mine.items():
                D[seg[0] + o] = v
            print('segment %#010x: %d byte(s) patched (uncompressed)' % (seg[2], len(mine)))
            continue
        touched, target = patch_segment(D, seg, mine)
        expected[seg] = target
        print('segment %#010x: %d byte(s) patched, re-encoded blocks %s' %
              (seg[2], len(mine), ', '.join('%#x' % t for t in touched)))
    missing = [a for a in edits if not any(s[2] <= a < s[2] + s[3] for s in segs)]
    if missing:
        sys.exit('addresses outside the compressed segments: %s' % ', '.join('%#x' % a for a in missing[:8]))
    for seg, target in expected.items():
        data, _ = decode_segment(D, seg)
        assert data == target, 'verification failed for segment %#x' % seg[2]
    print('verified: patched segments re-decode to the expected bytes')
    v = reseal(D)
    open(out, 'wb').write(encrypt(D, enc))
    print('byte-sum %#010x, wrote %s' % (v, out))

def cmd_selftest(fw):
    """Prove the codec against a firmware image: every block must decode to osz with zero
    padding, re-encode bit-for-bit, and have its A/B tables rebuilt bit-for-bit from lengths."""
    D = decrypt(open(fw, 'rb').read())
    nb = bad = 0
    for seg in segments(D):
        if not seg[4]:
            continue
        out = bytearray()
        for b in segment_blocks(D, seg):
            t = b.tokens(min(len(out), 32768))
            expand(t, out)
            ok = (b.encode(t) == b.C and tuple(build_table(b.ca, 'A')) == b.A
                  and tuple(build_table(b.cb, 'B')) == b.B)
            nb += 1
            bad += not ok
            if not ok:
                print('MISMATCH block @%#x' % b.off)
    print('%d blocks: %d decode+re-encode+table-rebuild exact, %d mismatches' % (nb, nb - bad, bad))

def cmd_verify(fw):
    D = decrypt(open(fw, 'rb').read())
    ok = struct.unpack('<I', D[CKSUM_OFF:CKSUM_OFF + 4])[0] == byte_sum(D)
    print('byte-sum', 'OK' if ok else 'MISMATCH')
    n = 0
    for seg in segments(D):
        data, toks = decode_segment(D, seg)
        n += len(toks)
        if seg[4]:
            print('segment %#010x: %d blocks, %d bytes OK' % (seg[2], len(toks), len(data)))
    print('%d blocks decoded' % n)

if __name__ == '__main__':
    if len(sys.argv) >= 4 and sys.argv[1] == 'unpack':
        cmd_unpack(sys.argv[2], sys.argv[3])
    elif len(sys.argv) >= 5 and sys.argv[1] == 'patch':
        cmd_patch(sys.argv[2], sys.argv[3], sys.argv[4:])
    elif len(sys.argv) >= 3 and sys.argv[1] == 'verify':
        cmd_verify(sys.argv[2])
    elif len(sys.argv) >= 3 and sys.argv[1] == 'selftest':
        cmd_selftest(sys.argv[2])
    else:
        print(__doc__)
