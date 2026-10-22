import sys,math; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *

def qdepths(a,b,model):
    n=a+b
    if model=='skew': return [min(j+1,n-1) for j in range(n)]
    if model=='revskew': return [min(n-j,n-1) for j in range(n)]
    if model in ('bal_sf','bal_df'):
        L=int(math.floor(math.log2(n)))
        if 2**L==n: ds=[L]*n
        else:
            nshallow=2**(L+1)-n; ndeep=n-nshallow
            ds=[L]*nshallow+[L+1]*ndeep
        if model=='bal_df': ds=ds[::-1]
        return ds
    raise ValueError

def enumerate_codes(T,model='bal_sf',qrev=0,dmsb=1):
    out=[]
    def rec(idx,code,depth):
        x=T[idx]; k=node(x)
        if k[0]=='L': out.append(((x>>2)&0x7fff,code,depth)); return
        if k[0]=='D':
            a,b,w,sz=k[1],k[2],k[3],k[4]; ch=T[w:w+sz]; i=0
            while i<len(ch):
                j=i
                while j<len(ch) and ch[j]==ch[i]: j+=1
                r=j-i; L=b-int(round(math.log2(r)))
                bits=i>>(b-L) if dmsb else i&((1<<L)-1)
                rec(w+i,(code<<L)|bits,depth+L); i=j
        else:
            a,b,w=k[1],k[2],k[3]; n=a+b
            kids=list(range(w+1,w+1+n))
            ds=qdepths(a,b,model)
            if qrev: kids=kids[::-1]
            # assign prefix-free codes by sorted depth (canonical): sort children by depth
            order=sorted(range(n),key=lambda j:ds[j])
            code2=0; prev=0
            cur=0; lastd=0
            # canonical assignment in listed order of increasing depth
            assigned={}
            c=0; pd=ds[order[0]]
            cc=0
            cd=ds[order[0]]
            codev=0
            # standard canonical: iterate children in (depth) order
            cl=0; val=0
            codes={}
            val=0; curlen=ds[order[0]]
            for oi,j in enumerate(order):
                if ds[j]>curlen:
                    val<<=(ds[j]-curlen); curlen=ds[j]
                codes[j]=(val,curlen); val+=1
            for j in range(n):
                cv,cl=codes[j]
                rec(kids[j],(code<<cl)|cv,depth+cl)
    rec(0,0,0)
    return out
