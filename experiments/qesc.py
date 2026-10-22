import sys,itertools,math; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
def enum_codes(T,qmode,escpos,tailpol,dmsb):
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
            # determine read width rb: largest rb<=b with 2^rb<=n ... use b if 2^b<=n else floor(log2(n+1))? use min
            rb=b
            while (1<<rb)>n and rb>0: rb-=1
            direct=(1<<rb)-1
            if direct>=n: direct=n  # all direct, no escape
            # escape slot index:
            esc = (1<<rb)-1 if escpos=='last' else 0
            # assign direct children to the non-escape index slots
            slots=[s for s in range(1<<rb) if s!=esc] if direct<n else list(range(1<<rb))
            di=0
            for s in range(1<<rb):
                if s==esc and direct<n:
                    continue
                if di<direct:
                    rec(kids[di], (code<<rb)|s, depth+rb); di+=1
            # tail: remaining kids di..n-1 as skewed unary under escape
            tail=kids[di:]
            m=len(tail)
            if m>0:
                base=(code<<rb)|esc
                for t,ci in enumerate(tail):
                    if t<m-1:
                        # t ones then stop (tailpol=1) or t zeros then one
                        if tailpol==1: bits=((1<<(t+1))-2)
                        else: bits=1
                        rec(ci,(base<<(t+1))|bits, depth+rb+t+1)
                    else:
                        if tailpol==1: bits=(1<<t)-1
                        else: bits=0
                        rec(ci,(base<<t)|bits if t>0 else base, depth+rb+(t if t>0 else 0))
    rec(0,0,0); return out
class BR:
    def __init__(s,d,order): s.d=d; s.order=order; s.pos=0
    def bit(s):
        i=s.pos; s.pos+=1; byte=i>>3; off=i&7
        if s.order in('le16','le32'):
            wb=2 if s.order=='le16' else 4; base=(byte//wb)*wb; byte=base+(wb-1-(byte%wb))
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
for escpos,tailpol,order,dmsb in itertools.product(['last','first'],[0,1],['msb','lsb','le16','le32'],[1,0]):
    cdA=enum_codes(A,'',escpos,tailpol,dmsb)
    # verify prefix-free + kraft
    mA={(l,c):sv for sv,c,l in cdA}
    if len(mA)!=len(cdA): res.append((-1,escpos,tailpol,order,dmsb,'COLLIDE')); continue
    kr=sum(2.0**-l for _,_,l in cdA); mlA=max(l for _,_,l in cdA)
    br=BR(Cc,order); lits=bytearray()
    try:
        while True:
            v=dec(br,mA,mlA)
            if v is None or v>=0x100: break
            lits.append(v)
            if len(lits)>200: break
    except: pass
    res.append((len(lits),escpos,tailpol,order,dmsb,'kr%.3f'%kr,bytes(lits[:24]).hex()))
res.sort(key=lambda t:-t[0])
for r in res[:16]: print(r)
