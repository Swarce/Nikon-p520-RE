import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
from codes import build, gen
from decode3 import bitstream
def mkdec(d):
    by=sorted(set(len(c) for c in d)); return d,by
def readsym(bits,p,dec):
    d,lens=dec
    for L in lens:
        c=bits[p:p+L]
        if len(c)<L: return None
        if c in d: return d[c]+(L,)
    return None
def inflate(o,order,xref,dref=0,qinv=0,lim=None):
    A,B,C,osz=tables(o); bits=bitstream(C,order)
    dA=mkdec(build(A,False,dref,qinv)); dB=mkdec(build(B,True,dref,qinv))
    out=bytearray(); p=0; bad=0; nm=0; lim=lim or osz; n=len(bits)
    def rd(k):
        nonlocal p
        raw=bits[p:p+k]; p+=k
        if xref: raw=raw[::-1]
        return int(raw,2) if raw else 0
    while p<n-1 and len(out)<lim:
        r=readsym(bits,p,dA)
        if r is None: return bytes(out),bad,nm,'NOSYM',p
        v,xb,L=r; p+=L
        if v<0x100: out.append(v); continue
        if v==0x100: return bytes(out),bad,nm,'EOB',p
        ln=(v&0x1ff)+rd(xb)
        r2=readsym(bits,p,dB)
        if r2 is None: return bytes(out),bad,nm,'NODIST',p
        dv,_,L2=r2; p+=L2; dex=dv>>2; dist=((dv&3)<<dex)+1+rd(dex); nm+=1
        if dist>len(out) or dist<1: bad+=1; dist=1
        for _ in range(ln): out.append(out[-dist] if out else 0)
    return bytes(out),bad,nm,'END',p
