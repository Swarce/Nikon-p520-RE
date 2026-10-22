import sys,itertools,math; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
def tree_lengths(T,qmodel):
    res={}
    def rec(idx,depth):
        x=T[idx]; k=node(x)
        if k[0]=='L':
            s=(x>>2)&0x7fff
            if s not in res: res[s]=depth
            return
        if k[0]=='D':
            b,w,sz=k[2],k[3],k[4]; ch=T[w:w+sz]; i=0
            while i<len(ch):
                j=i
                while j<len(ch) and ch[j]==ch[i]: j+=1
                rec(w+i,depth+b-int(round(math.log2(j-i)))); i=j
        else:
            a,b,w=k[1],k[2],k[3]; n=a+b; kids=list(range(w+1,w+1+n))
            if qmodel=='skew':
                for jj,ci in enumerate(kids): rec(ci,depth+(jj+1 if jj<n-1 else n-1))
            elif qmodel=='esc':
                rb=b
                while (1<<rb)>n and rb>0: rb-=1
                direct=min((1<<rb)-1,n)
                for t in range(direct): rec(kids[t],depth+rb)
                tail=kids[direct:]; m=len(tail)
                for t,ci in enumerate(tail): rec(ci,depth+rb+(t+1 if t<m-1 else m-1))
            else: # balanced
                L=int(math.floor(math.log2(n)))
                ds=[L]*n if 2**L==n else ([L]*(2**(L+1)-n)+[L+1]*(2*n-2**(L+1)))
                for jj,ci in enumerate(kids): rec(ci,depth+ds[jj])
    rec(0,0); return res
def order_key(v,mode):
    if mode=='val': return v
    # deflate-ish: literals first, EOB, then rest by value
    if v<0x100: return v
    if v==0x100: return 256
    return 257+v
def canon(lens,omode):
    syms=sorted(lens, key=lambda v:(lens[v],order_key(v,omode)))
    maxl=max(lens.values()); bl=[0]*(maxl+1)
    for l in lens.values(): bl[l]+=1
    nextc=[0]*(maxl+2); code=0
    for b in range(1,maxl+1): code=(code+bl[b-1])<<1; nextc[b]=code
    m={}
    for v in sorted(lens,key=lambda v:(lens[v],order_key(v,omode))):
        l=lens[v]; m[(l,nextc[l])]=v; nextc[l]+=1
    return m,maxl
class BR:
    def __init__(s,d,order): s.d=d; s.order=order; s.pos=0
    def bit(s):
        i=s.pos; s.pos+=1; byte=i>>3; off=i&7
        return (s.d[byte]>>(7-off))&1 if s.order=='msb' else (s.d[byte]>>off)&1
def dec(br,m,ml):
    c=0
    for l in range(1,ml+1):
        c=(c<<1)|br.bit()
        if (l,c) in m: return m[(l,c)]
    return None
nm,o=list(blocks())[0]; A,B,Cc,osz=tables(o)
res=[]
for qmodel,omode,order in itertools.product(['skew','esc','bal'],['val','deflate'],['msb','lsb']):
    lens=tree_lengths(A,qmodel); mA,mlA=canon(lens,omode)
    br=BR(Cc,order); lits=bytearray()
    try:
        while True:
            v=dec(br,mA,mlA)
            if v is None or v>=0x100: break
            lits.append(v)
            if len(lits)>400: break
    except: pass
    res.append((len(lits),qmodel,omode,order,bytes(lits[:28]).hex()))
res.sort(key=lambda t:-t[0])
for r in res[:12]: print(r)
