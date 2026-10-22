import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
from fractions import Fraction as F
def ncode(e,dist):
    k=node(e); v,n=k[1],k[2]
    if not dist and v&0x200: n-= (v>>10)&7
    return n
def K(T,x,dist,variant):
    k=node(x)
    ch=T[k[3]:k[3]+k[4]]
    if k[0]=='D':
        return sum(F(1,2**k[2])*(1 if e&3==3 else K(T,e,dist,variant)) for e in ch)
    a,b,m=k[1],k[2],k[4]
    tot=F(0)
    for i,e in enumerate(ch):
        if variant=='unary':      # 1^i 0 ; last = 1^(m-1)
            L = i+1 if i<m-1 else m-1
        sub = F(1,2**ncode(e,dist)) if e&3==3 else K(T,e,dist,variant)
        if variant=='unary' and e&3==3 and 'leafn' not in variant: pass
        tot+=F(1,2**L)*(sub)
    return tot
if __name__=='__main__':
    for variant in ['unary']:
        c=collections.Counter(); ex=[]
        for nm,o in blocks():
            A,B,C,osz=tables(o)
            for d,T in ((0,A),(1,B)):
                k=K(T,T[0],d,variant); c[(d,k==1)]+=1
                if k!=1 and len(ex)<10: ex.append((hex(o),d,str(k),float(k)))
        print(variant,c); [print(e) for e in ex]
