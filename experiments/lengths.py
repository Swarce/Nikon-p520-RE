import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
def leaf_len_values(T,dist):
    """walk tree, return dict symbol_value -> huffman_code_length (excluding extra bits)."""
    res={}
    def ncode(e):
        v=(e&0x7fff)>>2; n=e>>15
        if not dist and (v&0x200): n-=(v>>10)&7
        return n,v
    def walk(x,depth):
        k=node(x); ch=T[k[3]:k[3]+k[4]]
        if k[0]=='D':
            a,b=k[1],k[2]
            seen=set()
            for i in range(1<<b):
                e=ch[i]
                if e&3==3:
                    nc,v=ncode(e); L=depth+a+nc
                    if v not in res: res[v]=L
                else:
                    if id(e) not in seen: pass
                    walk(e,depth+b)
        else:
            m=k[4]
            for i in range(m):
                e=ch[i]; d2=depth+(i if i<m-1 else m-1)+(1 if i<m-1 else 0)
                if e&3==3:
                    nc,v=ncode(e); res[v]=d2+nc
                else: walk(e,d2)
    walk(T[0],0)
    return res
