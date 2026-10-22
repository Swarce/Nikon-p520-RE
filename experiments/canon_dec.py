import sys,math; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
# deflate length table (index 257..285)
LB=[(3,0),(4,0),(5,0),(6,0),(7,0),(8,0),(9,0),(10,0),(11,1),(13,1),(15,1),(17,1),(19,2),(23,2),(27,2),(31,2),(35,3),(43,3),(51,3),(59,3),(67,4),(83,4),(99,4),(115,4),(131,5),(163,5),(195,5),(227,5),(258,0)]
lb2idx={}
for i,(b,e) in enumerate(LB): lb2idx[(b,e)]=257+i
def val2litidx(v):
    if v<0x100: return v
    if v==0x100: return 256
    base=v&0x1ff; eb=(v>>10)&7; return lb2idx[(base,eb)]
def val2distidx(v):
    ex=v>>2; base=((v&3)<<ex)+1
    # deflate dist bases
    DB=[1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577]
    return DB.index(base)
def depths(T,model,qrev=0):
    res={}
    def rec(idx,depth):
        x=T[idx]; k=node(x)
        if k[0]=='L': res[(x>>2)&0x7fff]=depth; return
        if k[0]=='D':
            b,w,sz=k[2],k[3],k[4]; ch=T[w:w+sz]; i=0
            while i<len(ch):
                j=i
                while j<len(ch) and ch[j]==ch[i]: j+=1
                rec(w+i,depth+b-int(round(math.log2(j-i)))); i=j
        else:
            a,b,w=k[1],k[2],k[3]; n=a+b; kids=list(range(w+1,w+1+n))
            if qrev: kids=kids[::-1]
            if model=='skew':
                for jj,ci in enumerate(kids): rec(ci,depth+(jj+1 if jj<n-1 else n-1))
            else:
                L=int(math.floor(math.log2(n)))
                if 2**L==n: ds=[L]*n
                else: ns=2**(L+1)-n; ds=[L]*ns+[L+1]*(n-ns)
                for jj,ci in enumerate(kids): rec(ci,depth+ds[jj])
    rec(0,0); return res
def canon(lens):  # lens: dict idx->len ; returns dict (len,code)->idx  MSB canonical
    maxl=max(lens.values()); bl_count=[0]*(maxl+1)
    for l in lens.values(): bl_count[l]+=1
    code=0; nextcode=[0]*(maxl+1)
    for b in range(1,maxl+1):
        code=(code+bl_count[b-1])<<1; nextcode[b]=code
    m={}
    for idx in sorted(lens):
        l=lens[idx]; m[(l,nextcode[l])]=idx; nextcode[l]+=1
    return m,maxl
class BR:
    def __init__(s,d,order): s.d=d; s.order=order; s.pos=0
    def bit(s):
        i=s.pos; s.pos+=1; byte=i>>3; off=i&7
        return (s.d[byte]>>(7-off))&1 if s.order=='msb' else (s.d[byte]>>off)&1
def rb(br,n,msb):
    v=0
    if msb:
        for _ in range(n): v=(v<<1)|br.bit()
    else:
        for i in range(n): v|=br.bit()<<i
    return v
def dec(br,m,ml):
    c=0
    for l in range(1,ml+1):
        c=(c<<1)|br.bit()
        if (l,c) in m: return m[(l,c)]
    raise ValueError('nocode@%d'%br.pos)
def run(model,order,emsb,qrev=0):
    nm,o=list(blocks())[0]; A,B,Cc,osz=tables(o)
    dA=depths(A,model,qrev); dB=depths(B,model,qrev)
    lensA={val2litidx(v):l for v,l in dA.items()}
    lensB={val2distidx(v):l for v,l in dB.items()}
    idx2val_len={lb2idx[(b,e)]:(0x200|((e&7)<<10)|b) for b,e in LB}
    mA,mlA=canon(lensA); mB,mlB=canon(lensB)
    br=BR(Cc,order); out=bytearray(); ev=[]
    DB=[1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577]
    DE=[0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13]
    try:
        while len(out)<osz:
            idx=dec(br,mA,mlA)
            if idx<256: out.append(idx)
            elif idx==256: ev.append(('eob',len(out))); break
            else:
                b,e=LB[idx-257]; ln=b+rb(br,e,emsb)
                di=dec(br,mB,mlB); dist=DB[di]+rb(br,DE[di],emsb)
                if dist>len(out): ev.append(('baddist',len(out),dist)); 
                for _ in range(ln): out.append(out[-dist] if 0<dist<=len(out) else 0)
    except Exception as ex: ev.append(('err',str(ex),len(out)))
    return bytes(out),ev
import itertools
for model,order,emsb,qrev in itertools.product(['skew','bal'],['msb','lsb'],[0,1],[0,1]):
    out,ev=run(model,order,emsb,qrev)
    # count literals before first backref/err
    print('%-4s %s em%d qr%d: len%5d ev0=%s head=%s'%(model,order,emsb,qrev,len(out),ev[0] if ev else '-',out[:24].hex()))
