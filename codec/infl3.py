import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths; from zt import blocks,tables
import capstone,struct
md=capstone.Cs(capstone.CS_ARCH_MIPS,capstone.CS_MODE_MIPS32|capstone.CS_MODE_LITTLE_ENDIAN)
nm,o=list(blocks())[0]; A,B,C,osz=tables(o)

def bitseq(C,policy):
    bits=[]
    if policy in('byte_msb','byte_lsb'):
        for by in C:
            rng=range(7,-1,-1) if policy=='byte_msb' else range(8)
            for k in rng: bits.append((by>>k)&1)
    else:
        wsz=4 if policy.startswith('w32') else 2
        msb=policy.endswith('msb')
        for i in range(0,len(C)-wsz+1,wsz):
            v=int.from_bytes(C[i:i+wsz],'little')
            rng=range(wsz*8-1,-1,-1) if msb else range(wsz*8)
            for k in rng: bits.append((v>>k)&1)
    return bits

class Bits:
    def __init__(s,seq): s.s=seq; s.p=0
    def bn(s,k):
        v=0
        for _ in range(k): v=(v<<1)|s.s[s.p]; s.p+=1
        return v
    def b1(s): x=s.s[s.p]; s.p+=1; return x

def leafval(x):
    v=(x>>2)&0x7fff; return v&0x1ff,(v>>10)&7,(v>>9)&1,(v>>13)&1,(v>>14)&1
def qwalk(bits,a,b,w):
    nch=a+b; x=(1<<(b+1))-nch; s=bits.bn(b)
    if s<x: return w+1+s
    e=s-x; return w+1+x+2*e+bits.b1()
def walk(bits,T):
    i=0
    while True:
        x=T[i]; tag=x&3
        if tag==3: return leafval(x)
        a=x>>15; b=(x>>11)&15; w=(x>>2)&0x1ff
        i = w+bits.bn(b) if tag==0 else qwalk(bits,a,b,w)
def decode(seq,myst,limit=600):
    bits=Bits(seq); out=bytearray(); nmat=0
    try:
        while len(out)<min(osz,limit):
            base,eb,c9,c13,c14=walk(bits,A); cls=(c14,c13,c9)
            if cls==(0,0,0):
                if base==256: break
                out.append(base&0xff)
            elif cls==(0,1,0) and myst=='literal': out.append(base&0xff)
            else:
                length=base+(bits.bn(eb) if eb else 0)
                db,deb,_,_,_=walk(bits,B); dist=db+(bits.bn(deb) if deb else 0)
                nmat+=1
                if dist==0 or dist>len(out): return out,nmat,'baddist@%d'%len(out)
                for _ in range(length): out.append(out[-dist])
    except (IndexError,): return out,nmat,'eof@%d'%len(out)
    return out,nmat,'ok'
def mips(out):
    n=(len(out)//4)*4
    if n<4: return 0,0
    ad=set(a for a,_,_,_ in md.disasm_lite(bytes(out[:n]),0x8014e900))
    return sum(1 for k in range(n//4) if 0x8014e900+4*k in ad), n//4
res=[]
for pol in('byte_msb','byte_lsb','w32_msb','w32_lsb','w16_msb','w16_lsb'):
    seq=bitseq(C,pol)
    for myst in('literal','match'):
        out,nmat,st=decode(seq,myst); g,t=mips(out)
        res.append((g,len(out),pol,myst,nmat,st,bytes(out[:16])))
        print('%-9s %-7s bytes=%4d mips=%3d/%3d match=%d %s'%(pol,myst,len(out),g,t,nmat,st))
res.sort(reverse=True); print()
for r in res[:3]:
    g,ln,pol,myst,nmat,st,hd=r
    print('TOP',pol,myst,'mips=%d/%d'%(g,ln//4),'head',hd.hex(' '))
g,ln,pol,myst,nmat,st,hd=res[0]
seq=bitseq(C,pol); out,_,_=decode(seq,myst,limit=256)
print('\ndisasm best:')
for a,sz,mn,op in list(md.disasm_lite(bytes(out[:120]),0x8014e900))[:24]: print('  %08x %s %s'%(a,mn,op))
