import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
from kraft import ncode
def walk(T,x,dist,prefix,out):
    k=node(x); ch=T[k[3]:k[3]+k[4]]
    if k[0]=='D':
        a,b=k[1],k[2]
        for i,e in enumerate(ch):
            bits=format(i,'0%db'%b)
            if e&3==3:
                nc=ncode(e,dist); c=prefix+bits[:a+nc]
                if not out or out[-1][0]!=c: out.append((c,e))
            else: walk(T,e,dist,prefix+bits,out)
    else:
        m=k[4]
        for i,e in enumerate(ch):
            p=prefix+'1'*i+('0' if i<m-1 else '')
            if e&3==3: out.append((p+'?'*ncode(e,dist),e))
            else: walk(T,e,dist,p,out)
    return out
if __name__=='__main__':
    o=int(sys.argv[1],0); d=int(sys.argv[2])
    A,B,C,osz=tables(o); T=(A,B)[d]
    for c,e in walk(T,T[0],d,'',[]):
        if len(sys.argv)<4 or sys.argv[3] in P(e) or '?' in c: print('%-22s %s'%(c,P(e)))
