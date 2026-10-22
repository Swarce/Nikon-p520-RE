import sys,itertools; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
from walk import bitstream
from bigsearch import W
def dec(o,cfg,skip):
    A,B,C,osz=tables(o); w=W(bitstream(C,cfg['order']),cfg); w.p=skip; out=bytearray(); bad=0;nm=0
    while w.p<w.n-1 and len(out)<osz:
        v,xb=w.sym(A,False)
        if v<0x100: out.append(v); continue
        if v==0x100: return len(out),bad,nm,'EOB'
        L=(v&0x1ff)+w.rd(xb,cfg['xref']); dv,_=w.sym(B,True); dex=dv>>2
        dist=((dv&3)<<dex)+1+w.rd(dex,cfg['xref']); nm+=1
        if not(0<dist<=len(out)): 
            bad+=1
            if bad>12: return len(out),bad,nm,'ABORT'
            dist=1
        for _ in range(L): out.append(out[-dist] if out else 0)
    return len(out),bad,nm,'END'
o=0x360364; A,B,C,osz=tables(o); res=[]
for order in ['lsb','msb','w32','w32lsb','w16','w16be']:
 for dref,qinv,qrev,add_a,xref in itertools.product((0,1),repeat=5):
  for skip in range(0,8):
    cfg=dict(order=order,dref=dref,qinv=qinv,qrev=qrev,add_a=add_a,xref=xref)
    try: ln,bad,nm,st=dec(o,cfg,skip)
    except Exception: continue
    res.append((abs(ln-osz) if st!='ABORT' else 10**9, bad, ln, st, skip, cfg))
res.sort(key=lambda t:(t[1],t[0]))
print('osz=%#x'%osz)
for d,bad,ln,st,skip,cfg in res[:14]:
    print('bad%3d len%5d/%5x %-6s skip%d %s %s'%(bad,ln,osz,st,skip,cfg['order'],{k:v for k,v in cfg.items() if k!='order'}))
