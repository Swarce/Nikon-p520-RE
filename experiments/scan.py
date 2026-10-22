import sys,itertools,math; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
def qdepths(a,b,model):
    n=a+b
    if model=='skew': return None  # handled separately
    if model in('bal_sf','bal_df'):
        L=int(math.floor(math.log2(n)))
        if 2**L==n: ds=[L]*n
        else:
            ns=2**(L+1)-n; ds=[L]*ns+[L+1]*(n-ns)
        return ds[::-1] if model=='bal_df' else ds
def enum_codes(T,model,qrev,dmsb):
    out=[]
    def rec(idx,code,depth):
        x=T[idx]; k=node(x)
        if k[0]=='L': out.append(((x>>2)&0x7fff,code,depth)); return
        if k[0]=='D':
            b,w,sz=k[2],k[3],k[4]; ch=T[w:w+sz]; i=0
            while i<len(ch):
                j=i
                while j<len(ch) and ch[j]==ch[i]: j+=1
                L=b-int(round(math.log2(j-i)))
                bits=i>>(b-L) if dmsb else i&((1<<L)-1)
                rec(w+i,(code<<L)|bits,depth+L); i=j
        else:
            a,b,w=k[1],k[2],k[3]; n=a+b
            kids=list(range(w+1,w+1+n))
            if qrev: kids=kids[::-1]
            if model=='skew':
                for j,ci in enumerate(kids):
                    if j<n-1: rec(ci,(code<<(j+1))|((1<<j)-1<<1 if False else ((1<<(j+1))-2)),depth+j+1)
                    else: rec(ci,(code<<(n-1))|((1<<(n-1))-1),depth+n-1)
            else:
                ds=qdepths(a,b,model); order=sorted(range(n),key=lambda j:ds[j])
                codes={}; val=0; curlen=ds[order[0]]
                for j in order:
                    if ds[j]>curlen: val<<=(ds[j]-curlen); curlen=ds[j]
                    codes[j]=(val,curlen); val+=1
                for j in range(n):
                    cv,cl=codes[j]; rec(kids[j],(code<<cl)|cv,depth+cl)
    rec(0,0,0); return out
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
    raise ValueError
nm,o=list(blocks())[0]; A,B,Cc,osz=tables(o)
best=[]
for model,qrev,order,dmsb,emsb in itertools.product(['skew','bal_sf','bal_df'],[0,1],['msb','lsb'],[1,0],[0,1]):
    try:
        cdA=enum_codes(A,model,qrev,dmsb); cdB=enum_codes(B,model,qrev,dmsb)
        mA={(l,c):sv for sv,c,l in cdA}; mB={(l,c):sv for sv,c,l in cdB}
        mlA=max(l for _,_,l in cdA); mlB=max(l for _,_,l in cdB)
    except: continue
    br=BR(Cc,order); out=bytearray(); nlit=0; firstbr=None; ok=True
    try:
        while len(out)<300:
            v=dec(br,mA,mlA)
            if v<0x100:
                out.append(v)
                if firstbr is None: nlit+=1
            elif v==0x100: break
            else:
                base=v&0x1ff; eb=(v>>10)&7; ln=base+rb(br,eb,emsb)
                dv=dec(br,mB,mlB); dex=dv>>2; dist=(((dv&3)<<dex)+1)+rb(br,dex,emsb)
                if firstbr is None: firstbr=(len(out),dist,dist<=len(out))
                if dist>len(out): ok=False
                for _ in range(ln): out.append(out[-dist] if 0<dist<=len(out) else 0)
    except Exception as e: pass
    best.append((nlit, firstbr[2] if firstbr else False, model,qrev,order,dmsb,emsb, bytes(out[:12]).hex(), firstbr))
best.sort(key=lambda t:(-t[0],))
for nlit,okd,model,qrev,order,dmsb,emsb,hx,fbr in best[:20]:
    print('nlit%3d okdist%d %-6s qr%d %s dm%d em%d %s fbr%s'%(nlit,okd,model,qrev,order,dmsb,emsb,hx,fbr))
