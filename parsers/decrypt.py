# Nikon P520 firmware decrypt: ks[i] = K0[i & 0xffff] ^ C[(i>>16) & 0xff]
# K0 = ciphertext of an all-constant plaintext 64K chunk (0x150000), C = per-64K-chunk constant (period 256)
# usage: decrypt.py [firmware.bin] [dec.bin]   (defaults from p520paths)
import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
import os, numpy as np, collections
src=sys.argv[1] if len(sys.argv)>1 else p520paths.FIRMWARE
dst=sys.argv[2] if len(sys.argv)>2 else p520paths.DEC
d=np.fromfile(src,dtype=np.uint8)
n=len(d); K0=d[0x150000:0x160000].copy()
nch=(n+0xffff)//0x10000
pad=np.zeros(nch*0x10000,np.uint8); pad[:n]=d
X=pad.reshape(nch,0x10000)^K0
votes=collections.defaultdict(collections.Counter)
for i,r in enumerate(X[:-1]):
    h=np.bincount(r,minlength=256); votes[i&255][h.argmax()]+=1+h.max()/65536
C=np.array([votes[r].most_common(1)[0][0] for r in range(256)],np.uint8)
P=(X^C[np.arange(nch)&255][:,None]).reshape(-1)[:n]
P[:0x20]=d[:0x20]
os.makedirs(os.path.dirname(os.path.abspath(dst)),exist_ok=True)
P.tofile(dst); np.save(dst+'.C.npy',C); K0.tofile(dst+'.k0')
same=lambda a,b: os.path.exists(b) and np.array_equal(a,np.load(b) if b.endswith('.npy') else np.fromfile(b,np.uint8))
print('wrote %s (%d B); K0 matches data/K0.bin: %s; C matches data/C.npy: %s'%(
    dst,n,same(K0,os.path.join(p520paths.DATA,'K0.bin')),same(C,os.path.join(p520paths.DATA,'C.npy'))))
