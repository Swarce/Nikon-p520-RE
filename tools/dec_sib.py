import numpy as np, collections, sys
def decrypt(path,k0off=0x150000):
    d=np.fromfile(path,np.uint8); n=len(d)
    K0=d[k0off:k0off+0x10000].copy()
    nch=(n+0xffff)//0x10000
    pad=np.zeros(nch*0x10000,np.uint8); pad[:n]=d
    X=pad.reshape(nch,0x10000)^K0
    votes=collections.defaultdict(collections.Counter)
    for i,r in enumerate(X[:-1]):
        h=np.bincount(r,minlength=256); votes[i&255][h.argmax()]+=1+h.max()/65536
    C=np.array([votes[r].most_common(1)[0][0] for r in range(256)],np.uint8)
    P=(X^C[np.arange(nch)&255][:,None]).reshape(-1)[:n]
    P[:0x20]=d[:0x20]
    return P,K0,C
def score(P):
    b=P.tobytes()
    return b.count(b'\x7fELF\x01\x01\x01'), b.count(b'hcemain.elf'), b.count(b'.text')
if __name__=='__main__':
    path=sys.argv[1]; out=sys.argv[2]
    best=None
    # try P520 offset first, else scan chunk offsets with all-constant plaintext signature
    for k0off in [0x150000]+[c*0x10000 for c in range(0x10,0x40)]:
        if k0off+0x10000>len(np.fromfile(path,np.uint8)): continue
        P,K0,C=decrypt(path,k0off)
        e,h,t=score(P)
        if best is None or e>best[0]: best=(e,h,t,k0off,P,K0,C)
        if e>5 and h>0: break
    e,h,t,k0off,P,K0,C=best
    print('%s: k0off=%#x ELFmagics=%d hcemain=%d .text=%d'%(path,k0off,e,h,t))
    P.tofile(out); K0.tofile(out+'.k0'); np.save(out+'.C.npy',C)
    print('head:',P[:0x20].tobytes()); print('wrote',out,len(P))
