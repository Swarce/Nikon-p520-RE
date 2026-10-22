"""Offline decoder-convention search for the Zoran COACH type-2 block.
Oracle: output must disassemble as valid MIPS32-LE (seg s1 blk0 = code @0x8014e900).
"""
import sys, math, struct
sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import blocks, hdr, tables, node
import capstone
md=capstone.Cs(capstone.CS_ARCH_MIPS, capstone.CS_MODE_MIPS32|capstone.CS_MODE_LITTLE_ENDIAN)

nm,o=list(blocks())[0]; A,B,C,osz=tables(o)

class Bits:
    def __init__(self,data,order):
        self.d=data; self.order=order; self.pos=0; self.n=len(data)*8
    def get1(self):
        if self.pos>=self.n: raise EOFError
        byte=self.d[self.pos>>3]; bit=self.pos&7; self.pos+=1
        if self.order=='msb': return (byte>>(7-bit))&1
        else: return (byte>>bit)&1
    def get(self,k):
        v=0
        for _ in range(k):
            v=(v<<1)|self.get1()   # MSB-accumulate
        return v
    def getlsb(self,k):
        v=0
        for i in range(k):
            v|=self.get1()<<i
        return v

def leafval(x):
    v=(x>>2)&0x7fff; base=v&0x1ff; eb=(v>>10)&7
    c9=(v>>9)&1; c13=(v>>13)&1; c14=(v>>14)&1
    return base,eb,c9,c13,c14

# --- tree walk: D reads b bits (MSB dense). Q: pluggable model. ---
def walk(bits, T, qmodel):
    i=0
    while True:
        x=T[i]; tag=x&3
        if tag==3:   # leaf
            return leafval(x)
        a=x>>15; b=(x>>11)&15; w=(x>>2)&0x1ff
        if tag==0:   # D dense
            idx=bits.get(b); i=w+idx
        else:        # Q
            i=qmodel(bits,a,b,w,T,i)

def q_dense_escape(bits,a,b,w,T,i):
    # read b bits; if < 2^b-1 -> child; else escape, read 1 bit per extra child
    idx=bits.get(b)
    nch=a+b
    if idx < (1<<b)-1 or nch<=(1<<b):
        return w+1+min(idx,nch-1)
    # escape tail: remaining children reached by unary
    j=(1<<b)-1
    while j<nch-1:
        if bits.get1()==0: break
        j+=1
    return w+1+j

def q_unary_then_dense(bits,a,b,w,T,i):
    # 'a' unary prefixes then b-bit dense
    j=0
    while j<a:
        if bits.get1()==0: break
        j+=1
    if j<a:
        return w+1+j
    return w+1+a+bits.get(b)

def q_flat(bits,a,b,w,T,i):
    # read ceil(log2(a+b)) bits flat
    nch=a+b; k=max(1,(nch-1).bit_length())
    idx=bits.get(k)
    return w+1+min(idx,nch-1)

QMODELS={'dense_escape':q_dense_escape,'unary_dense':q_unary_then_dense,'flat':q_flat}

def decode(order,qname,limit=4096):
    bits=Bits(C,order); out=bytearray(); qm=QMODELS[qname]
    distB=B
    nmatch=0
    try:
        while len(out)<min(osz,limit):
            base,eb,c9,c13,c14=walk(bits,A,qm)
            cls=(c14,c13,c9)
            if cls==(0,0,0):
                if base<256: out.append(base)
                elif base==256: break          # EOB
                else: out.append(base&0xff)
            elif cls==(0,1,0):
                # mystery class: TRY as literal byte (base is 1..255)
                out.append(base&0xff)
            else:
                # length code: length = base + extra, then distance from B
                length=base+(bits.get(eb) if eb else 0)
                # distance
                db,deb,_,_,_=walk(bits,distB,qm)
                dist=db+(bits.get(deb) if deb else 0)
                nmatch+=1
                if dist==0 or dist>len(out):
                    return out,nmatch,'baddist@%d dist=%d len=%d'%(len(out),dist,length)
                for _ in range(length):
                    out.append(out[-dist])
    except EOFError:
        pass
    return out,nmatch,'ok'

def mips_score(out):
    n=(len(out)//4)*4
    if n<4: return 0,0
    good=0; total=n//4; code=bytes(out[:n])
    addrs=set(a for a,sz,mn,op in md.disasm_lite(code,0x8014e900))
    # count 4-byte slots that decoded to an instruction at their aligned addr
    for k in range(total):
        if (0x8014e900+4*k) in addrs: good+=1
    return good,total

best=[]
for order in ('msb','lsb'):
    for qn in QMODELS:
        try:
            out,nm2,status=decode(order,qn,limit=512)
        except Exception as e:
            out,nm2,status=bytearray(),0,'exc:%r'%e
        g,t=mips_score(out)
        best.append((g,len(out),order,qn,nm2,status))
        print('%-4s %-12s bytes=%4d matches=%3d mips_ok=%d/%d  %s'%(order,qn,len(out),nm2,g,t,status))
best.sort(reverse=True)
print('\nBEST:',best[0])
# dump head of best as MIPS
g,ln,order,qn,nm2,st=best[0]
out,_,_=decode(order,qn,limit=256)
print('\nhead disasm (%s/%s):'%(order,qn))
for a,sz,mn,op in list(md.disasm_lite(bytes(out[:128]),0x8014e900))[:24]:
    print('  %08x %s %s'%(a,mn,op))
