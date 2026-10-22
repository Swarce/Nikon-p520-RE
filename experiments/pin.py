import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
from codes import build
from decode4 import mkdec,readsym
from decode3 import bitstream
o=0x360364; A,B,C,osz=tables(o)
order='lsb'; bits=bitstream(C,order)
dA=mkdec(build(A,False,0,0)); dB=mkdec(build(B,True,0,0))
p=0; out=bytearray(); nsym=0
while len(out)<osz and p<len(bits)-1:
    r=readsym(bits,p,dA)
    if r is None: print('NOSYM@',p); break
    v,xb,L=r; cp=p; p+=L
    if v<0x100: out.append(v); nsym+=1; continue
    if v==0x100: print('EOB@',p,'out',len(out)); break
    raw=bits[p:p+xb]; ext=int(raw[::-1],2) if raw else 0; p+=xb; ln=(v&0x1ff)+ext
    r2=readsym(bits,p,dB); dv,_,L2=r2; dp=p; p+=L2
    dex=dv>>2; draw=bits[p:p+dex]; dext=int(draw[::-1],2) if draw else 0; p+=dex
    dist=((dv&3)<<dex)+1+dext
    tag='' 
    if dist>len(out): tag=' <<< BADDIST'
    if nsym<60 or tag:
        print('sym#%d outlen%d  lenval=%03x L=%d(xb%d ext%d)  distsym=%02x dex=%d dist=%d%s'%(nsym,len(out),v,ln,xb,ext,dv,dex,dist,tag))
    nsym+=1
    for _ in range(ln): out.append(out[-dist] if 0<dist<=len(out) else 0x90)
    if tag and nsym>0 and len([1 for _ in range(1) ])>=0 and dist>len(out)+0: 
        if nsym> 0 and sum(1 for c in '')==0 and False: break
    if tag: 
        if nsym> 40: break
print('total out',len(out))
