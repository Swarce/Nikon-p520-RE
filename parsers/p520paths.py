"""Shared paths for the P520 RE scripts (replaces the old hardcoded /home/claude/p520 layout).

Override with env vars: P520_FIRMWARE, P520_DEC, P520_FW_DIR, P520_WORK.
Default layout (both dirs gitignored):
  firmware/firmware.bin        P520 v1.1 SD-update image (encrypted)
  firmware/p530/firmware.bin   P530 v1.1
  firmware/p600/firmware.bin   P600 v1.2
  work/dec.bin                 decrypted P520 image (tools/fetch_firmware.py builds it)
Importing this module also puts parsers/, experiments/ and codec/ on sys.path
(appended, so local modules never shadow the stdlib).
"""
import os, sys, bisect

REPO=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DATA=os.path.join(REPO,'data')
REVERSE=os.path.join(REPO,'reverse')
FW_DIR=os.environ.get('P520_FW_DIR',os.path.join(REPO,'firmware'))
WORK=os.environ.get('P520_WORK',os.path.join(REPO,'work'))
FIRMWARE=os.environ.get('P520_FIRMWARE',os.path.join(FW_DIR,'firmware.bin'))
DEC=os.environ.get('P520_DEC',os.path.join(WORK,'dec.bin'))
SYMBOLS=os.path.join(REVERSE,'p520_symbols.map')

for _d in ('parsers','experiments','codec'):
    _p=os.path.join(REPO,_d)
    if _p not in sys.path: sys.path.append(_p)

def load_mainsyms():
    """(sorted addrs, names) from p520_symbols.map -- stands in for the old mainsyms.pkl."""
    rows=[]
    for line in open(SYMBOLS):
        p=line.split()
        if len(p)>=3:
            try: rows.append((int(p[0],16),p[2]))
            except ValueError: pass
    rows.sort()
    return [a for a,_ in rows],[n for _,n in rows]
