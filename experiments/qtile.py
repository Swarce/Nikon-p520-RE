import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
def cover(T,qsize,qoff):
    cov=[0]*len(T); st=[0]; seen=set()
    while st:
        idx=st.pop()
        if idx in seen or idx>=len(T): continue
        seen.add(idx); x=T[idx]
        if x&3==3: continue
        a=x>>15;b=(x>>11)&15;w=(x>>2)&0x1ff
        if x&1: start=w+qoff; size=(1<<b) if qsize=='2b' else a+b
        else: start=w; size=1<<b
        if start+size>len(T): return ('OVF',start+size,len(T))
        for i in range(start,start+size):
            cov[i]+=1
            if T[i]&3!=3: st.append(i)
    return (cov.count(0), sum(c>1 for c in cov), len(T))
A,B,C,osz=tables(0x360364)
for T,nm in ((A,'A'),(B,'B')):
    for qsize in ['ab','2b']:
        for qoff in (0,1):
            print('%s qsize=%-2s qoff=%d -> %s'%(nm,qsize,qoff,cover(T,qsize,qoff)))
