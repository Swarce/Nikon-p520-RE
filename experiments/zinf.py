import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
class Bits:
    def __init__(s,data,order):
        s.p=0; s.order=order
        if order=='lsb':   s.bits=''.join(format(c,'08b')[::-1] for c in data)
        elif order=='msb': s.bits=''.join(format(c,'08b') for c in data)
        elif order=='w32': s.bits=''.join(format(int.from_bytes(data[i:i+4],'little'),'032b') for i in range(0,len(data)-3,4))
        elif order=='w16': s.bits=''.join(format(int.from_bytes(data[i:i+2],'little'),'016b') for i in range(0,len(data)-1,2))
        elif order=='w32lsb': s.bits=''.join(format(int.from_bytes(data[i:i+4],'little'),'032b')[::-1] for i in range(0,len(data)-3,4))
        s.n=len(s.bits)
    def peek(s,k,inv=False):
        t=s.bits[s.p:s.p+k].ljust(k,'0')
        if inv: t=t.translate(str.maketrans('01','10'))
        return t
    def get(s,k,lsbfirst):
        t=s.bits[s.p:s.p+k].ljust(k,'0'); s.p+=k
        if k==0: return 0
        return int(t[::-1],2) if lsbfirst else int(t,2)
def sym(bs,T,inv):
    x=T[0]
    while True:
        k=node(x); ch=T[k[3]:k[3]+k[4]]
        if k[0]=='D':
            a,b=k[1],k[2]; idx=int(bs.peek(b,inv),2); e=ch[idx]
            if e&3==3:
                v,n=node(e)[1:]; return e,a+n   # caller handles consumption
            bs.p+=b; x=e
        else:
            m=k[4]; i=0; one='0' if inv else '1'
            while i<m-1 and bs.peek(1)==one: i+=1; bs.p+=1
            if i<m-1: bs.p+=1
            e=ch[i]
            if e&3==3:
                v,n=node(e)[1:]; return e,n
            x=e
def inflate(o,order,inv=False,xlsb=False,maxout=None,window=b''):
    A,B,C,osz=tables(o); bs=Bits(C,order); out=bytearray(window); base0=len(window)
    while True:
        e,cons=sym(bs,A,inv); v=(e&0x7fff)>>2
        if v<0x100:
            bs.p+=cons; out.append(v)
        elif v==0x100:
            bs.p+=cons; return out[base0:],bs.p,'EOB'
        else:
            ex=(v>>10)&7; bs.p+=cons-ex; L=(v&0x1ff)+bs.get(ex,xlsb)
            e2,c2=sym(bs,B,inv); dv=(e2&0x7fff)>>2; dex=dv>>2
            bs.p+=c2; dist=((dv&3)<<dex)+1 if dv>=4 else dv+1
            dist+=bs.get(dex if dv>=4 else 0,xlsb)
            if dist>len(out): return out[base0:],bs.p,('BADDIST',dist,len(out)-base0)
            for _ in range(L): out.append(out[-dist])
        if len(out)-base0>osz+300: return out[base0:],bs.p,'OVER'
        if bs.p>bs.n+64: return out[base0:],bs.p,'EOD'
if __name__=='__main__':
    o=int(sys.argv[1],0) if len(sys.argv)>1 else 0x360364
    A,B,C,osz=tables(o)
    for order in ['lsb','msb','w32','w16','w32lsb']:
        for inv in (0,1):
            for xl in (0,1):
                out,p,st=inflate(o,order,inv,xl)
                print('%-6s inv%d xlsb%d out %#x/%#x bits %d/%d %s'%(order,inv,xl,len(out),osz,p,len(C)*8,st))
