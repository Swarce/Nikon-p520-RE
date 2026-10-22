import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
from decode3 import bitstream,Dec
o=0x360364; A,B,C,osz=tables(o)
def run(cfg,ns=24):
    d=Dec(bitstream(C,cfg['order']),cfg); out=bytearray()
    print('=== cfg',cfg)
    for _ in range(ns):
        p0=d.p; v,xb=d.sym(A,False)
        if v<0x100:
            out.append(v); print('  @%-5d +%-2d lit %02x'%(p0,d.p-p0,v))
        elif v==0x100: print('  @%-5d EOB'%p0); break
        else:
            ext=d.rd(xb,cfg['xref']); L=(v&0x1ff)+ext; p1=d.p
            dv,_=d.sym(B,True); dex=(dv>>2) if dv>=4 else 0
            dbase=(((dv&3)<<dex)+1) if dv>=4 else dv+1; dist=dbase+d.rd(dex,cfg['xref'])
            print('  @%-5d len sym=%03x base=%d xb=%d L=%d | distsym=%x dist=%d  (outlen %d)'%(p0,v,v&0x1ff,xb,L,dv,dist,len(out)))
            for _ in range(L): out.append(out[-dist] if 0<dist<=len(out) else 0)
    print('  out:',bytes(out[:32]).hex())
for order in ['lsb','msb']:
  for qinv in (0,1):
    run(dict(order=order,dref=0,dinv=0,qinv=qinv,xref=1))
