import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths; from zt import blocks,tables,node
import capstone
md=capstone.Cs(capstone.CS_ARCH_MIPS,capstone.CS_MODE_MIPS32|capstone.CS_MODE_LITTLE_ENDIAN)
nm,o=list(blocks())[0]; A,B,C,osz=tables(o)

class Bits:
    def __init__(s,d,order): s.d=d; s.order=order; s.pos=0; s.n=len(d)*8
    def b1(s):
        by=s.d[s.pos>>3]; bit=s.pos&7; s.pos+=1
        return (by>>(7-bit))&1 if s.order=='msb' else (by>>bit)&1
    def bn(s,k):
        v=0
        if s.order=='msb':
            for _ in range(k): v=(v<<1)|s.b1()
        else:
            for i in range(k): v|=s.b1()<<i
        return v

def leafval(x):
    v=(x>>2)&0x7fff; return v&0x1ff,(v>>10)&7,(v>>9)&1,(v>>13)&1,(v>>14)&1

def qwalk(bits,a,b,w,escape_first):
    nch=a+b; x=(1<<(b+1))-nch   # #short(direct) at b bits
    nesc=(1<<b)-x               # escape slots
    s=bits.bn(b)
    if escape_first:
        if s<nesc:
            extra=bits.b1(); return w+1+x+2*s+extra
        return w+1+ (s-nesc) +  (nch-x)  # map remaining to short block... (variant)
    else:
        if s<x: return w+1+s
        e=s-x                    # escape slot index 0..nesc-1
        extra=bits.b1(); return w+1+x+2*e+extra

def walk(bits,T,escape_first):
    i=0
    while True:
        x=T[i]; tag=x&3
        if tag==3: return leafval(x)
        a=x>>15; b=(x>>11)&15; w=(x>>2)&0x1ff
        if tag==0: i=w+bits.bn(b)
        else: i=qwalk(bits,a,b,w,escape_first)

def decode(order,escape_first,lenclass='001',mystery='literal',limit=2048):
    bits=Bits(C,order); out=bytearray(); nm2=0
    try:
        while len(out)<min(osz,limit):
            base,eb,c9,c13,c14=walk(bits,A,escape_first); cls=(c14,c13,c9)
            if cls==(0,0,0):
                if base==256: break
                out.append(base&0xff)
            elif cls==(0,1,0):
                if mystery=='literal': out.append(base&0xff)
                else:
                    length=base; db,deb,_,_,_=walk(bits,B,escape_first); dist=db+(bits.bn(deb) if deb else 0)
                    nm2+=1
                    if dist==0 or dist>len(out): return out,nm2,'baddist@%d d=%d'%(len(out),dist)
                    for _ in range(length): out.append(out[-dist])
            else:
                length=base+(bits.bn(eb) if eb else 0)
                db,deb,_,_,_=walk(bits,B,escape_first); dist=db+(bits.bn(deb) if deb else 0)
                nm2+=1
                if dist==0 or dist>len(out): return out,nm2,'baddist@%d d=%d len=%d'%(len(out),dist,length)
                for _ in range(length): out.append(out[-dist])
    except EOFError: pass
    except IndexError as e: return out,nm2,'idx@%d'%len(out)
    return out,nm2,'ok'

def mips(out):
    n=(len(out)//4)*4
    if n<4: return 0,0
    addrs=set(a for a,_,_,_ in md.disasm_lite(bytes(out[:n]),0x8014e900))
    good=sum(1 for k in range(n//4) if (0x8014e900+4*k) in addrs)
    return good,n//4

res=[]
for order in('msb','lsb'):
  for ef in(False,True):
    for myst in('literal','match'):
      out,nm2,st=decode(order,ef,mystery=myst,limit=512)
      g,t=mips(out); res.append((g,len(out),order,ef,myst,nm2,st))
      print('%s esc_first=%-5s myst=%-7s bytes=%4d matches=%3d mips=%d/%d %s'%(order,ef,myst,len(out),nm2,g,t,st))
res.sort(reverse=True); g,ln,order,ef,myst,nm2,st=res[0]
print('\nBEST order=%s esc_first=%s myst=%s mips=%d'%(order,ef,myst,g))
out,_,_=decode(order,ef,mystery=myst,limit=256)
print('head bytes:',bytes(out[:32]).hex(' '))
for a,sz,mn,op in list(md.disasm_lite(bytes(out[:96]),0x8014e900))[:20]: print('  %08x %s %s'%(a,mn,op))
