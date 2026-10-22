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
        s.d=data; s.order=order; s.pos=0; s.n=len(data)*8
    def bit(s):
        i=s.pos; s.pos+=1
        byte=i>>3; off=i&7
        if byte>=len(s.d): raise IndexError
        b=s.d[byte]
        if s.order=='msb': return (b>>(7-off))&1
        else: return (b>>off)&1

def decsym(br,m,maxl):
    c=0
    for l in range(1,maxl+1):
        c=(c<<1)|br.bit()
        if (l,c) in m: return m[(l,c)]
    raise ValueError('nocode')

def inflate(A,B,Cc,osz,qpol,qrev,order,maxsym=400):
    mA,mlA=build_map(A,qpol,qrev); mB,mlB=build_map(B,qpol,qrev)
    br=BR(Cc,order); out=bytearray()
    try:
        while len(out)<osz and len(out)<maxsym:
            v=decsym(br,mA,mlA)
            if v<0x100: out.append(v)
            elif v==0x100: break
            elif v&0x200:
                base=v&0x1ff; eb=(v>>10)&7
                ln=base
                for _ in range(eb): ln=(ln<<1)|br.bit()  # placeholder; extra order tested separately
                # proper: ln = base + read eb bits
                # redo correctly:
                dv=decsym(br,mB,mlB)
                dex=dv>>2; dbase=((dv&3)<<dex)+1
                dist=dbase
                for _ in range(dex): dist=dist  # placeholder
                # we'll handle extra properly in v2
                break
            else: out.append(v&0xff)
    except Exception as e:
        return bytes(out), str(e)
    return bytes(out), 'ok'

if __name__=='__main__':
    nm,o=list(blocks())[0]
    A,B,Cc,osz=tables(o)
    crib=[None,0xff,0xbd,0x27,None,0x00,0xbf,0xaf]
    for qpol,qrev,order in itertools.product([0,1],[0,1],['msb','lsb']):
        out,st=inflate(A,B,Cc,osz,qpol,qrev,order,maxsym=16)
        score=sum(1 for i,c in enumerate(crib) if c is not None and i<len(out) and out[i]==c)
        print('qpol%d qrev%d %s: score%d/6 out=%s %s'%(qpol,qrev,order,score,out[:8].hex(),st))
