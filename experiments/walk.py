import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
def bitstream(data, order):
    if order=='lsb':   return ''.join(format(c,'08b')[::-1] for c in data)
    if order=='msb':   return ''.join(format(c,'08b') for c in data)
    if order=='w32':   return ''.join(format(int.from_bytes(data[i:i+4],'little'),'032b') for i in range(0,len(data)&~3,4))
    if order=='w32lsb':return ''.join(format(int.from_bytes(data[i:i+4],'little'),'032b')[::-1] for i in range(0,len(data)&~3,4))
    if order=='w16':   return ''.join(format(int.from_bytes(data[i:i+2],'little'),'016b') for i in range(0,len(data)&~1,2))
    if order=='w16be': return ''.join(format(int.from_bytes(data[i:i+2],'big'),'016b') for i in range(0,len(data)&~1,2))
class W:
    def __init__(s,bits,dref,qinv): s.b=bits;s.p=0;s.n=len(bits);s.dref=dref;s.qinv=qinv
    def sym(s,T,dist):
        x=T[0]
        while True:
            k=node(x); ch=T[k[3]:k[3]+k[4]]
            if k[0]=='D':
                a,b=k[1],k[2]; save=s.p; raw=s.b[s.p:s.p+b].ljust(b,'0')
                if s.dref: raw=raw[::-1]
                i=int(raw,2) if b else 0; e=ch[i]
                if e&3==3:
                    v=(e&0x7fff)>>2; xb=(v>>10)&7 if (not dist and v&0x200) else 0
                    s.p=save+a+(e>>15)-xb; return v,xb
                s.p=save+b; x=e
            else:
                m=k[4]; i=0; one='0' if s.qinv else '1'
                while i<m-1 and s.p<s.n and s.b[s.p]==one: i+=1;s.p+=1
                if i<m-1: s.p+=1
                e=ch[i]
                if e&3==3:
                    v=(e&0x7fff)>>2; xb=(v>>10)&7 if (not dist and v&0x200) else 0
                    return v,xb
                x=e
    def rd(s,k,ref):
        raw=s.b[s.p:s.p+k].ljust(k,'0'); s.p+=k
        if ref: raw=raw[::-1]
        return int(raw,2) if k else 0
def dec(o,order,dref,qinv,xref,lim):
    A,B,C,_=tables(o); w=W(bitstream(C,order),dref,qinv); out=bytearray(); bad=0;nm=0
    while w.p<w.n-1 and len(out)<lim:
        v,xb=w.sym(A,False)
        if v<0x100: out.append(v); continue
        if v==0x100: return bytes(out),bad,nm
        L=(v&0x1ff)+w.rd(xb,xref); dv,_=w.sym(B,True); dex=dv>>2
        dist=((dv&3)<<dex)+1+w.rd(dex,xref); nm+=1
        if not(0<dist<=len(out)): bad+=1; dist=1
        for _ in range(L): out.append(out[-dist] if out else 0)
    return bytes(out),bad,nm
