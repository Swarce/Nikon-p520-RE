import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths; from zt import blocks,tables
BL=list(blocks()); nm,o=BL[0]; A,B,C,osz=tables(o); clen=len(C)
def mkbits(C,pol):
    bits=[]
    if pol[0]=='b':
        src=[(by,8) for by in C]
    else:
        wsz=int(pol[1]); be=pol[2]=='B'
        src=[(int.from_bytes(C[i:i+wsz],'big' if be else 'little'),wsz*8) for i in range(0,len(C)-wsz+1,wsz)]
    msb=pol[-1]=='M'
    for v,w in src:
        rng=range(w-1,-1,-1) if msb else range(w)
        for k in rng: bits.append((v>>k)&1)
    return bits
def revb(v,b):
    r=0
    for i in range(b): r=(r<<1)|((v>>i)&1)
    return r
def lf(x):
    v=(x>>2)&0x7fff; return v&0x1ff,(v>>10)&7,(v>>9)&1,(v>>13)&1,(v>>14)&1
def decode(bits,drev,qdir,distf,skip,limit=osz):
    p=skip; out=bytearray(); N=len(bits)-1
    def rd(k):
        nonlocal p
        v=0
        for _ in range(k): v=(v<<1)|bits[p]; p+=1
        return v
    def walk(T):
        nonlocal p
        i=0
        while True:
            x=T[i]; t=x&3
            if t==3: return x
            a=x>>15; b=(x>>11)&15; w=(x>>2)&0x1ff
            if t==0:
                v=rd(b); i=w+(revb(v,b) if drev else v)
            else:
                nch=a+b; xd=(1<<(b+1))-nch; v=rd(b)
                if drev: v=revb(v,b)
                if qdir:
                    if v<xd: i=w+1+v
                    else: e=v-xd; i=w+1+xd+2*e+bits[p]; p+=1
                else:
                    esc=(1<<b)-xd
                    if v<esc: i=w+1+2*v+bits[p]; p+=1
                    else: i=w+1+esc*2+(v-esc)
    try:
        while len(out)<limit and p<N:
            base,eb,c9,c13,c14=lf(walk(A)); cls=(c14,c13,c9)
            if cls==(0,0,0):
                if base==256: return len(out),p,'EOB'
                out.append(base); continue
            # length (either (0,0,1) deflate-len or treat 0x2000 per distf)
            length=base+(rd(eb) if eb else 0)
            bb,beb,bc9,bc13,bc14=lf(walk(B))
            if distf==0: dist=bb+1
            elif distf==1: dist=bb+(256 if bc13 else 0)+1
            elif distf==2: dist=bb+(rd(beb) if beb else 0)+1
            else: dist=(bb<<( beb)) +1 if beb else bb+1
            if dist>len(out) or dist<1: return len(out),p,'bd@%d d=%d'%(len(out),dist)
            for _ in range(length):
                if len(out)>=limit: break
                out.append(out[-dist])
    except IndexError: return len(out),p,'eof'
    return len(out),p,'end'
best=[]
for pol in ['bM','bL','w2M','w2L','w4M','w4L','w2BM','w4BM']:
    bits=mkbits(C,pol)
    for drev in (0,1):
        for qdir in (0,1):
            for distf in (0,1,2,3):
                for skip in range(0,17):
                    nb,p,st=decode(bits,drev,qdir,distf,skip)
                    best.append((nb,pol,drev,qdir,distf,skip,st))
best.sort(reverse=True)
print('osz=%d clen=%d'%(osz,clen))
for b in best[:15]: print('bytes=%6d pol=%-4s drev=%d qdir=%d distf=%d skip=%2d %s'%(b[0],b[1],b[2],b[3],b[4],b[5],b[6]))
