import sys,itertools; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
from enum import enumerate_codes

def build_map(T,qpol,qrev,dmsb=1):
    cd=enumerate_codes(T,qpol=qpol,qrev=qrev,dmsb=dmsb)
    m={}
    for sv,c,l in cd: m[(l,c)]=sv
    return m, max(l for _,_,l in cd)

class BR:
    def __init__(s,data,order):
        s.d=data; s.order=order; s.pos=0
        if order.startswith('le'):
            w=int(order[2:]); s.wb=w
        else: s.wb=0
    def bit(s):
        i=s.pos; s.pos+=1
        if s.wb:
            # swap within wb-byte words
            wbytes=s.wb//8
            byte=i>>3; off=i&7
            base=(byte//wbytes)*wbytes; inw=byte%wbytes
            sb=base+(wbytes-1-inw)
            if sb>=len(s.d): raise IndexError
            return (s.d[sb]>>(7-off))&1
        byte=i>>3; off=i&7
        if byte>=len(s.d): raise IndexError
        b=s.d[byte]
        return (b>>(7-off))&1 if s.order=='msb' else (b>>off)&1
    def rev(s): # bits consumed
        return s.pos

def readbits(br,n,msb):
    v=0
    if msb:
        for _ in range(n): v=(v<<1)|br.bit()
    else:
        for i in range(n): v|=br.bit()<<i
    return v

def decsym(br,m,maxl):
    c=0
    for l in range(1,maxl+1):
        c=(c<<1)|br.bit()
        if (l,c) in m: return m[(l,c)]
    raise ValueError('nocode')

def inflate(A,B,Cc,osz,qpol,qrev,order,emsb):
    mA,mlA=build_map(A,qpol,qrev); mB,mlB=build_map(B,qpol,qrev)
    br=BR(Cc,order); out=bytearray()
    baddist=0
    try:
        while len(out)<osz:
            v=decsym(br,mA,mlA)
            if v<0x100: out.append(v)
            elif v==0x100: 
                return bytes(out),'eob',br.pos,baddist
            else:
                base=v&0x1ff; eb=(v>>10)&7
                ln=base+readbits(br,eb,emsb)
                dv=decsym(br,mB,mlB)
                dex=dv>>2; dist=(((dv&3)<<dex)+1)+readbits(br,dex,emsb)
                if dist>len(out): baddist+=1
                for _ in range(ln):
                    out.append(out[-dist] if dist<=len(out) else 0)
    except Exception as e:
        return bytes(out),'err:'+str(e)[:20],br.pos,baddist
    return bytes(out),'full',br.pos,baddist

if __name__=='__main__':
    nm,o=list(blocks())[0]
    A,B,Cc,osz=tables(o)
    print('osz',osz,'clen bits',len(Cc)*8)
    best=[]
    for qpol,qrev,order,emsb in itertools.product([0,1],[0,1],['msb','lsb','le16','le32'],[0,1]):
        out,st,pos,bd=inflate(A,B,Cc,osz,qpol,qrev,order,emsb)
        best.append((len(out),st,bd,qpol,qrev,order,emsb,pos))
    for n,st,bd,qp,qr,od,em,pos in sorted(best,reverse=True)[:20]:
        print('len%6d %-10s baddist%4d qp%d qr%d %s emsb%d bits%d/%d'%(n,st,bd,qp,qr,od,em,pos,len(Cc)*8))
