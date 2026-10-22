import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
res=collections.Counter(); bad=[]
for nm,o in blocks():
    A,B,C,osz=tables(o)
    for ti,T in enumerate((A,B)):
        st=[T[0]]
        while st:
            k=node(st.pop())
            ch=list(T[k[3]:k[3]+k[4]])
            for e in ch:
                if e&3!=3: st.append(e)
            if k[0]!='D': continue
            a,b=k[1],k[2]; i=0
            while i<len(ch):
                e=ch[i]
                if e&3!=3: res['D-ptr-child']+=1; i+=1; continue
                v,n=node(e)[1:]
                islen = ti==0 and v>=0x200 and v!=0x100
                ln = n
                r=1
                while i+r<len(ch) and ch[i+r]==e: r+=1
                # expected replication
                aligned = (i % r == 0)
                key=('len' if islen else ('dist' if ti else 'lit'))
                exp = 2**(b-a-n) if b-a-n>=0 else None
                res[(key,'ok' if exp==r else 'mismatch', 'aligned' if aligned else 'unaligned')]+=1
                if exp!=r and len(bad)<15: bad.append((nm,hex(o),'AB'[ti],P(T[0]),'D a%d b%d'%(a,b),i,P(e),'rep',r,'exp',exp))
                i+=r
for k,v in sorted(res.items(),key=str): print(k,v)
for x in bad: print(x)
