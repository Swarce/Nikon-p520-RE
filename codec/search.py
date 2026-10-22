import sys,itertools; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths; from zt import blocks,tables
import capstone
md=capstone.Cs(capstone.CS_ARCH_MIPS,capstone.CS_MODE_MIPS32|capstone.CS_MODE_LITTLE_ENDIAN)
BL=list(blocks()); nm,o=BL[0]; A,B,C,osz=tables(o); clen=len(C)

def mkbits(C,pol):
    out=bytearray()
    if pol[0]=='b':
        it=((by,8) for by in C)
    else:
        wsz=int(pol[1]); it=((int.from_bytes(C[i:i+wsz],'little'),wsz*8) for i in range(0,len(C)-wsz+1,wsz))
    msb = pol.endswith('M')
    bits=[]
    for v,w in it:
        if msb:
            for k in range(w-1,-1,-1): bits.append((v>>k)&1)
        else:
            for k in range(w): bits.append((v>>k)&1)
    return bits

def revbits(v,b): 
    r=0
    for i in range(b): r=(r<<1)|((v>>i)&1)
    return r

def leaf(x):
    v=(x>>2)&0x7fff; return v&0x1ff,(v>>10)&7,(v>>9)&1,(v>>13)&1,(v>>14)&1

def decode(bits,drev,qdirect,qesc_rev,limit):
    # returns (nbytes,bits_used,status,out)
    p=0; out=bytearray(); N=len(bits)
    def rd(k):
        nonlocal p
        v=0
        for _ in range(k):
            v=(v<<1)|bits[p]; p+=1
        return v
    def walk(T):
        nonlocal p
        i=0
        while True:
            x=T[i]; tag=x&3
            if tag==3: return x
            a=x>>15; b=(x>>11)&15; w=(x>>2)&0x1ff
            if tag==0:
                v=rd(b);  i=w+(revbits(v,b) if drev else v)
            else:
                nch=a+b; xd=(1<<(b+1))-nch; esc=(1<<b)-xd
                v=rd(b); 
                if drev: v=revbits(v,b)
                if qdirect:
                    if v<xd: i=w+1+v
                    else:
                        e=v-xd; i=w+1+xd+2*e+(bits[p]); p+=1
                else:
                    if v<esc:
                        e=v; i=w+1+2*e+(bits[p]); p+=1
                    else:
                        i=w+1+esc*2+(v-esc)
    try:
        while len(out)<osz and p<N-1:
            x=walk(A); base,eb,c9,c13,c14=leaf(x); cls=(c14,c13,c9)
            # class semantics (fixed best-guess; swept outside)
            if cls==(0,0,0) and base==256: 
                return len(out),p,'EOB',out
            if cls==(0,0,0) and base<256:
                out.append(base)
            elif cls==(0,1,0) and eb==0:
                out.append(base&0xff)   # mystery-as-literal
            else:
                length=base+(rd(eb) if eb else 0)
                dx=walk(B); db,deb,_,_,_=leaf(dx); dist=db+(rd(deb) if deb else 0)
                if dist<=0 or dist>len(out): return len(out),p,'baddist',out
                for _ in range(length):
                    if len(out)>=osz: break
                    out.append(out[-dist])
    except IndexError: return len(out),p,'eof',out
    return len(out),p,'end',out

pols=['bM','bL','w2M','w2L','w4M','w4L']
best=[]
for pol in pols:
    bits=mkbits(C,pol)
    for drev in (0,1):
        for qdirect in (0,1):
            for qesc_rev in (0,):
                nb,used,st,out=decode(bits,drev,qdirect,qesc_rev,osz)
                score=nb + (100000 if st=='EOB' and abs(nb-osz)<4 else 0)
                best.append((nb,used,st,pol,drev,qdirect,bytes(out[:12])))
best.sort(reverse=True)
print('osz=%d clen=%d (bits=%d)'%(osz,clen,clen*8))
for b in best[:12]:
    nb,used,st,pol,drev,qd,hd=b
    print('bytes=%6d used=%6d/%6d st=%-8s pol=%-3s drev=%d qdir=%d head=%s'%(nb,used,clen*8,st,pol,drev,qd,hd.hex(' ')))
