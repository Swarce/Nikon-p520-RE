#!/usr/bin/env python3
"""COACH-emu lite — boot the P520 main-app RAM image under Unicorn (MIPS32LE) far enough
to trace execution, capture the firmware's own asserts, and localize faults to a symbol.

This is a *diagnostic* emulator, not a camera: it has no sensor/lens/display model. Its job
is to run the real firmware code path, show it to you symbolized, and let you test whether a
patch changes behaviour — without risking hardware.

Boot model (derived from _start @ 0xc0000600):
  _start programs the TLB itself with 256MB pages: virtual 0xc0000000 -> physical 0x80000000
  (contiguous), plus an uncached alias at 0xe0000000. It performs only CP0 ops (no memory
  access) until it `jr`s into the mapped region, so we start at _start's KSEG1 alias
  (0xa0000600, fixed-translate, needs no TLB), let it install the TLB, and execution lands in
  the mapped image. MMIO (KSEG1) is caught lazily and modeled as RW memory; registers whose
  readback the firmware verifies against a different address are seeded from MMIO_MODEL (the
  assert messages tell you the expected values — e.g. "CGU_UNIT_EN1 expected: 2000").

Inputs (regenerate if absent):
  ram image : tools/p520fw.py unpack firmware.bin  (-> unpacked/ram_c0000000.bin)
  symbols   : p520_symbols.map  (addr T name per line)

Usage:
  python3 -I tools/coachemu.py --image ram_c0000000.bin --syms p520_symbols.map
  python3 -I tools/coachemu.py ... --patch 0xc0ebf358=61000000   # test a patch's effect
  python3 -I tools/coachemu.py ... --trace OsSysHaltEx,GdiGetTextDataById --max-insn 20000000
"""
import sys, bisect, argparse
from unicorn import *
from unicorn.mips_const import *

P_LOW, SZ_LOW   = 0x00000000, 0x01000000      # KSEG1 alias of low image (bootstrap fetch)
P_DRAM, SZ_DRAM = 0x80000000, 0x20000000      # vaddr 0xc0000000..0xe0000000 -> here (512MB,
                                              # the TLB's 256MB even+odd pages, contiguous)
START           = 0xa0000600                  # KSEG1 alias of _start

# Seed specific MMIO readback registers: phys_addr -> value. Grow this as asserts reveal
# expected values. (phys = KSEG1 vaddr & 0x1fffffff.)
MMIO_MODEL = {
    0x1000800c: 0x2000,   # CGU_UNIT_EN1 status, read by CCguUnitMgmtImpl::VerifyUnits()
    0x18003c10: 7,        # DRAM geometry index (DcuGetDramAttributes); 7 = 512MB (matches map)
}

# CPU-feature shims: Unicorn's MIPS core omits some CP0 features the firmware probes for in
# tight spin loops (as it omits the boot-ROM TLB setup). Each entry patches a GPR right after
# the mfc0 that read the missing feature, so the probe sees a sane value. addr -> (reg, OR-mask).
CPU_SHIMS = {
    # _gpr_shadow_check: after `mfc0 $t3, SRSCtl`, force HSS (bits 29:26) nonzero so the
    # "wait for shadow register sets" loop (beqz $t4,self @0xc008b52c) exits.
    0xc008b528: ("T3", 0x0C000000),
}

def v2p(a):
    a &= 0xffffffff
    if 0xc0000000 <= a < 0xe0000000: return P_DRAM + (a - 0xc0000000)   # cached, 512MB
    if 0xe0000000 <= a < 0xf0000000: return P_DRAM + (a - 0xe0000000)   # uncached alias
    if 0x80000000 <= a < 0xc0000000: return a & 0x1fffffff
    return a

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--image", default="/home/user/fw/unpacked/ram_c0000000.bin")
    ap.add_argument("--syms",  default="/home/user/work/reverse/p520_symbols.map")
    ap.add_argument("--patch", action="append", default=[], help="VA=HEXBYTES, applied to the image")
    ap.add_argument("--trace", default="", help="comma-separated symbols to log on entry")
    ap.add_argument("--max-insn", type=int, default=8_000_000)
    ap.add_argument("--timeout", type=int, default=25, help="seconds")
    ap.add_argument("--mmio", action="store_true", help="log first touch of each MMIO page")
    ap.add_argument("--skip-halts", action="store_true",
                    help="log asserts/halts and return instead of stopping (explore boot flow)")
    ap.add_argument("--skip-match", default="", help="comma-separated substrings: skip only halts whose message matches")
    ap.add_argument("--skip-limit", type=int, default=400, help="stop after this many skipped halts")
    ap.add_argument("--peek", default="", help="comma-separated VAs (hex) to dump as words at each halt")
    ap.add_argument("--watch", default="", help="PHYS lo:hi (hex): log every read in range with PC")
    a = ap.parse_args()

    img = bytearray(open(a.image, "rb").read())
    for p in a.patch:                              # VA=hex -> write into DRAM-relative image
        va, hexb = p.split("="); off = (int(va, 16) & 0xffffffff) - 0xc0000000
        b = bytes.fromhex(hexb); img[off:off+len(b)] = b
        print("[patch] %s = %s (img off 0x%x)" % (va, hexb, off))

    syms, addrs = {}, []
    for ln in open(a.syms, errors="replace"):
        q = ln.split()
        if len(q) >= 3 and len(q[0]) == 8:
            try: ad = int(q[0], 16)
            except ValueError: continue
            syms[ad] = q[2]; addrs.append(ad)
    addrs.sort()
    def nearest(x):
        i = bisect.bisect_right(addrs, x) - 1
        if i < 0: return "?"
        b = addrs[i]
        return syms[b] if x == b else "%s+0x%x" % (syms[b], x - b)
    name2addr = {v: k for k, v in syms.items()}

    uc = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 + UC_MODE_LITTLE_ENDIAN)
    uc.mem_map(P_LOW, SZ_LOW); uc.mem_map(P_DRAM, SZ_DRAM)
    uc.mem_write(P_LOW, bytes(img[:SZ_LOW])); uc.mem_write(P_DRAM, bytes(img))
    try: uc.reg_write(UC_MIPS_REG_CP0_STATUS, 0)
    except Exception: pass

    def rd_cstr(va, n=240):
        out = b""
        try:
            while len(out) < n:
                c = uc.mem_read(v2p(va + len(out)), 1)
                if c == b"\x00": break
                out += c
        except UcError: pass
        return out.decode("latin1", "replace")

    seen = set()
    last_acc = []
    def h_unmapped(uc, access, address, size, value, user):
        pg = address & ~0xFFF
        last_acc.append((("W" if access in (UC_MEM_WRITE, UC_MEM_WRITE_UNMAPPED) else
                          "X" if access in (UC_MEM_FETCH, UC_MEM_FETCH_UNMAPPED) else "R"),
                         address, uc.reg_read(UC_MIPS_REG_PC)))
        if len(last_acc) > 6: last_acc.pop(0)
        if a.mmio and pg not in seen:
            seen.add(pg); pc = uc.reg_read(UC_MIPS_REG_PC)
            t = "W" if access in (UC_MEM_WRITE, UC_MEM_WRITE_UNMAPPED) else "R"
            print("[mmio %s] phys 0x%08x pc=0x%08x(%s)" % (t, address, pc, nearest(pc)))
        try:
            uc.mem_map(pg, 0x1000)
            for pa, val in MMIO_MODEL.items():     # seed modeled regs in this page
                if pg <= pa < pg + 0x1000:
                    uc.mem_write(pa, int(val).to_bytes(4, "little"))
        except UcError: pass
        return True
    uc.hook_add(UC_HOOK_MEM_READ_UNMAPPED | UC_HOOK_MEM_WRITE_UNMAPPED | UC_HOOK_MEM_FETCH_UNMAPPED, h_unmapped)

    def h_printf(uc, addr, size, user):
        s = rd_cstr(uc.reg_read(UC_MIPS_REG_A0))
        if s: sys.stdout.write("[LOG] %s%s" % (s, "" if s.endswith("\n") else "\n"))
    for nm in ("Printf_B", "Fprintf_B"):
        ad = name2addr.get(nm)
        if ad: uc.hook_add(UC_HOOK_CODE, h_printf, begin=ad, end=ad)

    halts = [0]
    def h_halt(uc, addr, size, user):
        args = [uc.reg_read(r) for r in (UC_MIPS_REG_A0, UC_MIPS_REG_A1, UC_MIPS_REG_A2, UC_MIPS_REG_A3)]
        msg = next((rd_cstr(x) for x in args if sum(c.isprintable() for c in rd_cstr(x)) > 3), "")
        ra = uc.reg_read(UC_MIPS_REG_RA)
        print("[HALT] %s  %r  (from %s)" % (nearest(addr), msg.replace("\x1b", " ").replace("\n", " / "), nearest(ra)))
        for va in [int(x, 16) for x in a.peek.split(",") if x]:
            try: print("   peek 0x%08x = 0x%08x" % (va, int.from_bytes(uc.mem_read(v2p(va), 4), "little")))
            except UcError: print("   peek 0x%08x = <unmapped>" % va)
        for x in recent[-14:]: print("     . 0x%08x  %s" % (x, nearest(x)))
        match = any(m and m in msg for m in a.skip_match.split(","))
        if a.skip_halts or match:
            halts[0] += 1
            if halts[0] >= a.skip_limit:
                print("[skip-limit reached]"); uc.emu_stop(); return
            uc.reg_write(UC_MIPS_REG_PC, ra)       # return to caller, skip the halt
        else:
            uc.emu_stop()
    for nm in ("OsSysHaltEx", "OsSysHalt", "OsAssert", "OsSysAssert"):
        ad = name2addr.get(nm)
        if ad: uc.hook_add(UC_HOOK_CODE, h_halt, begin=ad, end=ad)

    for nm in [s for s in a.trace.split(",") if s]:
        ad = name2addr.get(nm)
        if ad:
            def mk(n):
                return lambda uc, addr, size, user: print("[trace] %s  a0=0x%08x a1=0x%08x ra=0x%08x(%s)" %
                       (n, uc.reg_read(UC_MIPS_REG_A0), uc.reg_read(UC_MIPS_REG_A1),
                        uc.reg_read(UC_MIPS_REG_RA), nearest(uc.reg_read(UC_MIPS_REG_RA))))
            uc.hook_add(UC_HOOK_CODE, mk(nm), begin=ad, end=ad)
        else:
            print("[trace] unknown symbol: %s" % nm)

    if a.watch:
        wlo, whi = (int(x, 16) for x in a.watch.split(":"))
        def h_watch(uc, access, address, size, value, user):
            if wlo <= address < whi:
                pc = uc.reg_read(UC_MIPS_REG_PC)
                print("[watch R] phys 0x%08x sz%d pc=0x%08x(%s)" % (address, size, pc, nearest(pc)))
        uc.hook_add(UC_HOOK_MEM_READ, h_watch, begin=wlo, end=whi)

    import unicorn.mips_const as mc
    def mk_shim(regname, mask):
        reg = getattr(mc, "UC_MIPS_REG_" + regname)
        return lambda uc, address, size, user: uc.reg_write(reg, uc.reg_read(reg) | mask)
    for sa, (rn, mask) in CPU_SHIMS.items():
        uc.hook_add(UC_HOOK_CODE, mk_shim(rn, mask), begin=sa, end=sa)

    recent = []
    def h_blk(uc, address, size, user):
        recent.append(address)
        if len(recent) > 24: recent.pop(0)
    uc.hook_add(UC_HOOK_BLOCK, h_blk)

    print("[run] %s  start @0x%08x  cap %d insn / %ds" % (a.image, START, a.max_insn, a.timeout))
    uc.reg_write(UC_MIPS_REG_PC, START)
    try:
        uc.emu_start(START, 0, timeout=a.timeout * 1_000_000, count=a.max_insn)
        print("[stop] limit/return; PC=0x%08x (%s)" %
              (uc.reg_read(UC_MIPS_REG_PC), nearest(uc.reg_read(UC_MIPS_REG_PC))))
        from collections import Counter
        c = Counter(nearest(x).split("+")[0] for x in recent)
        print("  recent block funcs:", dict(c))
        for x in recent[-12:]: print("     . 0x%08x  %s" % (x, nearest(x)))
    except UcError as e:
        pc = uc.reg_read(UC_MIPS_REG_PC)
        print("\n[FAULT] %s  PC=0x%08x (%s)  RA=0x%08x (%s)" % (e, pc, nearest(pc),
              uc.reg_read(UC_MIPS_REG_RA), nearest(uc.reg_read(UC_MIPS_REG_RA))))
        print("  last accesses (type addr pc):")
        for t, ad, p in last_acc[-6:]: print("     %s 0x%08x  (pc 0x%08x %s)" % (t, ad, p, nearest(p)))
        for x in recent[-16:]: print("    0x%08x  %s" % (x, nearest(x)))

if __name__ == "__main__":
    main()
