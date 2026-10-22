import sys,itertools; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
import enum2
class BR:
    def __init__(s,d,order): s.d=d; s.order=order; s.pos=0
    def bit(s):
        i=s.pos; s.pos+=1; byte=i>>3; off=i&7
        if order_swap(s.order):
            wb=order_swap(s.order); base=(byte//wb)*wb; sb=base+(wb-1-(byte%wb))
            if sb>=len(s.d): raise IndexError
            return (s.d[sb]>>(7-off))&1
        if byte>=len(s.d): raise IndexError
        return (s.d[byte]>>(7-off))&1 if s.order=='msb' else (s.d[byte]>>off)&1
def order_swap(o): return {'le16':2,'le32':4}.get(o,0)
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
    raise ValueError
def inflate(A,B,Cc,osz,model,qrev,order,emsb):
    cdA=enum2.enumerate_codes(A,model,qrev); cdB=enum2.enumerate_codes(B,model,qrev)
    mA={(l,c):sv for sv,c,l in cdA}; mB={(l,c):sv for sv,c,l in cdB}
    mlA=max(l for _,_,l in cdA); mlB=max(l for _,_,l in cdB)
    # validate prefix-free
    br=BR(Cc,order); out=bytearray(); bd=0
    try:
        while len(out)<osz:
            v=decsym(br,mA,mlA)
            if v<0x100: out.append(v)
            elif v==0x100: return bytes(out),'eob',br.pos,bd
            else:
                base=v&0x1ff; eb=(v>>10)&7; ln=base+readbits(br,eb,emsb)
                dv=decsym(br,mB,mlB); dex=dv>>2; dist=(((dv&3)<<dex)+1)+readbits(br,dex,emsb)
                if dist>len(out): bd+=1
                for _ in range(ln): out.append(out[-dist] if dist<=len(out) and dist>0 else 0)
    except Exception as e:
        return bytes(out),'err',br.pos,bd
    return bytes(out),'full',br.pos,bd
if __name__=='__main__':
    nm,o=list(blocks())[0]; A,B,Cc,osz=tables(o)
    print('osz',osz,'bits',len(Cc)*8)
    res=[]
    for model,qrev,order,emsb in itertools.product(['skew','revskew','bal_sf','bal_df'],[0,1],['msb','lsb','le16','le32'],[0,1]):
        out,st,pos,bd=inflate(A,B,Cc,osz,model,qrev,order,emsb)
        res.append((len(out) if st!='full' else len(out)+100000 if bd==0 else len(out),len(out),st,bd,model,qrev,order,emsb,pos))
    for sc,n,st,bd,model,qrev,order,emsb,pos in sorted(res,reverse=True)[:18]:
        print('len%6d %-5s bd%4d %-7s qr%d %s em%d bits%d'%(n,st,bd,model,qrev,order,emsb,pos))
