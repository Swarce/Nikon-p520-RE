import sys,itertools; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from decode3 import bitstream,Dec
from zt import tables,node
def run(o,cfg,lim):
    A,B,C,osz=tables(o); d=Dec(bitstream(C,cfg['order']),cfg); out=bytearray(); bad=0; nmatch=0
    while d.p<d.n-1 and len(out)<lim:
        v,xb=d.sym(A,False)
        if v<0x100: out.append(v); continue
        if v==0x100: return len(out),bad,nmatch,'EOB',d.p
        L=(v&0x1ff)+d.rd(xb,cfg['xref'])
        dv,_=d.sym(B,True); dex=dv>>2; dist=((dv&3)<<dex)+1+d.rd(dex,cfg['xref'])
        nmatch+=1
        if dist>len(out) or dist<1: bad+=1; dist=1
        for _ in range(L): out.append(out[-dist] if out else 0)
    return len(out),bad,nmatch,'END',d.p
o=0x360364; res=[]
for order in ['lsb','msb','w32','w32lsb','w16']:
 for dref,dinv,qinv,xref in itertools.product((0,1),repeat=4):
    cfg=dict(order=order,dref=dref,dinv=dinv,qinv=qinv,xref=xref)
    try: ln,bad,nm,st,p=run(o,cfg,4000)
    except Exception: continue
    frac=bad/nm if nm else 1
    res.append((frac,bad,nm,ln,st,cfg))
res.sort(key=lambda t:(t[0],-t[3]))
for frac,bad,nm,ln,st,cfg in res[:12]:
    print('baddist %5.3f (%d/%d) len%5d %s | %s %s'%(frac,bad,nm,ln,st,cfg['order'],{k:v for k,v in cfg.items() if k!='order'}))
