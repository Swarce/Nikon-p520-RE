import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
res=collections.Counter(); nb=0; types=collections.Counter()
for nm,o in blocks():
    nb+=1; types[hdr(o)[0]]+=1
    A,B,C,osz=tables(o)
    for T in (A,B):
        cov=[0]*len(T); cov[0]=1; st=[T[0]]
        while st:
            k=node(st.pop())
            for i in range(k[3],k[3]+k[4]):
                if i>=len(T): res['overflow']+=1; continue
                cov[i]+=1
                if T[i]&3!=3: st.append(T[i])
        res['unc']+=cov.count(0); res['multi']+=sum(c>1 for c in cov)
print(nb,types,res)
