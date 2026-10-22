import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
from walk import bitstream
from bigsearch import W
def full(o,cfg):
    A,B,C,osz=tables(o); w=W(bitstream(C,cfg['order']),cfg); out=bytearray();bad=0;nm=0;firstbad=None
    while w.p<w.n-1 and len(out)<osz:
        v,xb=w.sym(A,False)
        if v<0x100: out.append(v); continue
        if v==0x100: return out,bad,nm,len(out),'EOB',firstbad
        L=(v&0x1ff)+w.rd(xb,cfg['xref']); dv,_=w.sym(B,True); dex=dv>>2
        dist=((dv&3)<<dex)+1+w.rd(dex,cfg['xref']); nm+=1
        if not(0<dist<=len(out)):
            bad+=1
            if firstbad is None: firstbad=(len(out),v&0x1ff,dv,dist)
            dist=1
        for _ in range(L): out.append(out[-dist] if out else 0)
    return out,bad,nm,len(out),'END',firstbad
o=0x360364
for order in ['lsb','msb']:
  for dref in (0,1):
    cfg=dict(order=order,dref=dref,qinv=0,qrev=0,add_a=1,xref=1)
    out,bad,nm,ln,st,fb=full(o,cfg)
    print('%s dref%d: %s len%d bad%d/%d firstbad=%s'%(order,dref,st,ln,bad,nm,fb))
    print('   ',bytes(out[:48]).hex())
