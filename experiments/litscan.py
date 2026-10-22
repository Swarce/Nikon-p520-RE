import sys,itertools,math; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
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
            a,b,w=k[1],k[2],k[3]; n=a+b; kids=list(range(w+1,w+1+n))
            if qrev: kids=kids[::-1]
            if model=='skew':
                for jj,ci in enumerate(kids):
                    d=jj+1 if jj<n-1 else n-1
                    bits=((1<<d)-2) if jj<n-1 else ((1<<d)-1)
                    rec(ci,(code<<d)|bits,depth+d)
            elif model=='skew0':  # continue-on-0
                for jj,ci in enumerate(kids):
                    d=jj+1 if jj<n-1 else n-1
                    bits=1 if jj<n-1 else 0
                    rec(ci,(code<<d)|bits,depth+d)
            else:
                L=int(math.floor(math.log2(n)))
                if 2**L==n: ds=[L]*n
                else: ns=2**(L+1)-n; ds=([L]*ns+[L+1]*(n-ns))
                if model=='bal_df': ds=ds[::-1]
                order=sorted(range(n),key=lambda j:ds[j]); val=0; cur=ds[order[0]]; cc={}
                for j in order:
                    if ds[j]>cur: val<<=(ds[j]-cur); cur=ds[j]
                    cc[j]=(val,ds[j]); val+=1
                for j in range(n):
                    cv,cl=cc[j]; rec(kids[j],(code<<cl)|cv,depth+cl)
    rec(0,0,0); return out
class BR:
    def __init__(s,d,order): s.d=d; s.order=order; s.pos=0
    def bit(s):
        i=s.pos; s.pos+=1; byte=i>>3; off=i&7
        if s.order in('le16','le32'):
            wb=2 if s.order=='le16' else 4; base=(byte//wb)*wb; byte=base+(wb-1-(byte%wb)); off=i&7
            return (s.d[byte]>>(7-off))&1
        return (s.d[byte]>>(7-off))&1 if s.order=='msb' else (s.d[byte]>>off)&1
def dec(br,m,ml):
    c=0
    for l in range(1,ml+1):
        c=(c<<1)|br.bit()
        if (l,c) in m: return m[(l,c)]
    return None
nm,o=list(blocks())[0]; A,B,Cc,osz=tables(o)
res=[]
for model,qrev,order,dmsb in itertools.product(['skew','skew0','bal_sf','bal_df'],[0,1],['msb','lsb','le16','le32'],[1,0]):
    cdA=enum_codes(A,model,qrev,dmsb)
    mA={(l,c):sv for sv,c,l in cdA}; mlA=max(l for _,_,l in cdA)
    br=BR(Cc,order); lits=bytearray()
    try:
        while True:
            v=dec(br,mA,mlA)
            if v is None or v>=0x100: break
            lits.append(v)
            if len(lits)>64: break
    except: pass
    res.append((len(lits),model,qrev,order,dmsb,bytes(lits[:20]).hex()))
res.sort(reverse=True)
for n,model,qrev,order,dmsb,hx in res[:24]:
    print('lits%3d %-6s qr%d %-4s dm%d %s'%(n,model,qrev,order,dmsb,hx))
