import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
def ncode(e,dist):   # huffman bits WITHIN final table step for a leaf (excludes extra)
    v=(e&0x7fff)>>2; n=e>>15
    if not dist and (v&0x200): n-=(v>>10)&7
    return n,v
def gen(T,dist,dref,qinv):
    """yield (codestring, value, extra_bits). codestring = huffman code MSB-first as labeled."""
    out=[]
    def walk(x,pre):
        k=node(x); ch=T[k[3]:k[3]+k[4]]
        if k[0]=='D':
            a,b=k[1],k[2]
            for i in range(1<<b):
                bits=format(i,'0%db'%b)
                if dref: bits=bits[::-1]
                e=ch[i]
                if e&3==3:
                    nc,v=ncode(e,dist); code=pre+bits[:a+nc]
                    xb=0 if dist else ((v>>10)&7 if (v&0x200) else 0)
                    if not out or out[-1][0]!=code: out.append((code,v,xb))
                else:
                    walk(e,pre+bits)  # pointer consumes all b
        else:
            m=k[4]; one='0' if qinv else '1'
            for i in range(m):
                p=pre+one*i+(('1' if qinv else '0') if i<m-1 else '')
                e=ch[i]
                if e&3==3:
                    nc,v=ncode(e,dist); xb=0 if dist else ((v>>10)&7 if (v&0x200) else 0)
                    out.append((p+'?'*nc, v, xb) if nc else (p,v,xb))
                else: walk(e,p)
    walk(T[0],'')
    return out
def build(T,dist,dref,qinv):
    """return dict: code(str, may contain '?' wildcards expanded) -> (value,xb). Expand wildcards."""
    d={}
    for code,v,xb in gen(T,dist,dref,qinv):
        if '?' in code:
            base=code.replace('?',''); nq=code.count('?')
            # wildcard bits are index bits already consumed as part of huffman length -> they ARE part of code
            # but we don't know their value here; they were the low index bits. Instead treat code length only.
            for j in range(1<<nq):
                d[base+format(j,'0%db'%nq)]=(v,xb)
        else:
            d[code]=(v,xb)
    return d
if __name__=='__main__':
    o=0x360364; A,B,C,osz=tables(o)
    for dref in (0,1):
        dA=build(A,False,dref,0)
        lens=[len(c) for c in dA]
        from fractions import Fraction as F
        kr=sum(F(1,2**len(c)) for c in dA)
        # prefix-free check
        s=sorted(dA); pf=all(not s[i+1].startswith(s[i]) for i in range(len(s)-1))
        print('dref%d A: codes=%d Kraft=%s prefixfree=%s minlen=%d maxlen=%d'%(dref,len(dA),kr,pf,min(lens),max(lens)))
