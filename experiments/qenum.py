import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
import math

def leafval(x):
    v=(x>>2)&0x7fff
    return v

def enumerate_codes(T, qmode='skew', qpol=0, qrev=0, dmsb=1):
    """Return list of (symval, code_int, length). code MSB-first bit order."""
    out=[]
    def rec(idx, code, depth):
        x=T[idx]; k=node(x)
        if k[0]=='L':
            out.append((leafval(x), code, depth)); return
        if k[0]=='D':
            a,b,w,sz=k[1],k[2],k[3],k[4]
            ch=T[w:w+sz]; i=0
            while i<len(ch):
                j=i
                while j<len(ch) and ch[j]==ch[i]: j+=1
                r=j-i; L=b-int(math.log2(r))
                off=i  # start offset
                bits = off>>(b-L) if dmsb else off & ((1<<L)-1)
                rec(w+i, (code<<L)|bits, depth+L)
                i=j
        else: # Q
            a,b,w=k[1],k[2],k[3]; n=a+b
            kids=list(range(w+1, w+1+n))
            if qrev: kids=kids[::-1]
            # fully-skewed unary: child j (j<n-1): j*(cont) + 1 stop ; last: (n-1) cont
            cont = qpol        # bit meaning "go deeper"
            stop = 1-qpol
            for j,ci in enumerate(kids):
                if j<n-1:
                    c=(code<<(j+1)); 
                    for t in range(j): c=(c<<0); # build below
                    # bits: j conts then a stop
                    bits=0
                    for t in range(j): bits=(bits<<1)|cont
                    bits=(bits<<1)|stop
                    rec(ci, (code<<(j+1))|bits, depth+j+1)
                else:
                    bits=0
                    for t in range(n-1): bits=(bits<<1)|cont
                    rec(ci, (code<<(n-1))|bits, depth+(n-1))
    rec(0,0,0)
    return out

def check(codes, nsym):
    # prefix-free + kraft
    seen={}
    kraft=0
    for sv,c,l in codes:
        kraft+=2.0**-l
        key=(c,l); 
    # prefix-free: build set of (len,code); check no code is prefix of another
    items=sorted(codes,key=lambda t:t[2])
    pref=set()
    pf=True
    for sv,c,l in items:
        # check all proper prefixes
        for pl in range(1,l):
            if (c>>(l-pl), pl) in pref_codes: pass
    return kraft

if __name__=='__main__':
    nm,o=list(blocks())[0]
    A,B,Cc,osz=tables(o)
    import itertools
    for qpol,qrev,dmsb in itertools.product([0,1],[0,1],[1,0]):
        cd=enumerate_codes(A,qpol=qpol,qrev=qrev,dmsb=dmsb)
        kraft=sum(2.0**-l for _,_,l in cd)
        # prefix-free check
        codeset={}
        pf=True
        for sv,c,l in cd:
            codeset.setdefault(l,{})[c]=sv
        allc=sorted(((l,c,sv) for sv,c,l in cd))
        pref=set(); 
        for l,c,sv in allc:
            ok=True
            for pl in range(1,l):
                if (pl, c>>(l-pl)) in pref: ok=False;break
            if not ok: pf=False
            pref.add((l,c))
        nsym=len(set(sv for sv,_,_ in cd))
        print('qpol%d qrev%d dmsb%d: nsym%d kraft%.4f pf%s'%(qpol,qrev,dmsb,nsym,kraft,pf))
