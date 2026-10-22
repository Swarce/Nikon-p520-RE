#!/usr/bin/env python3
"""P520 firmware reseal: decrypt -> (patch) -> recompute byte-sum -> re-encrypt.

Integrity field (recovered 2026-10-19): a 32-bit little-endian additive byte-sum
  stored at decrypted offset CKSUM_OFF, covering dec[CKSUM_START:CKSUM_OFF].
  No CRC32, no crypto signature, no stored MD5 of the payload. Just recompute.

Cipher:  plain[i] = cipher[i] ^ K0[i & 0xffff] ^ C[(i>>16) & 0xff]
  (header bytes [0:0x20] are stored as plaintext verbatim; restored on both ways)

Usage:
  reseal.py decrypt  firmware.bin dec.bin
  reseal.py checksum dec.bin                      # print stored vs computed
  reseal.py patch    dec.bin out.bin  OFF=HEXBYTES [OFF=HEXBYTES ...]
  reseal.py encrypt  dec.bin out_firmware.bin     # recompute cksum + re-encrypt
"""
import sys, numpy as np, os

HERE=os.path.dirname(os.path.abspath(__file__))
DATA=os.path.join(os.path.dirname(HERE),'data')
CKSUM_START=0x50
CKSUM_OFF=0x4d80110
HDR_PLAIN=0x20

def _k0c():
    K0=np.fromfile(os.path.join(DATA,'K0.bin'),np.uint8)
    C =np.load(os.path.join(DATA,'C.npy'))
    return K0,C

def keystream(n):
    K0,C=_k0c()
    nch=(n+0xffff)//0x10000
    ks=np.tile(K0,nch)[:n].copy()
    ks^=np.repeat(C[np.arange(nch)&255],0x10000)[:n]
    return ks

def decrypt(enc):
    enc=np.asarray(enc,np.uint8); n=len(enc)
    d=enc^keystream(n); d[:HDR_PLAIN]=enc[:HDR_PLAIN]
    return d

def encrypt(dec):
    dec=np.asarray(dec,np.uint8); n=len(dec)
    e=dec^keystream(n)
    # header [0:0x20] is stored plaintext in the file and copied verbatim into dec
    e[:HDR_PLAIN]=dec[:HDR_PLAIN]
    return e

def compute_cksum(dec):
    return int(np.add.reduce(dec[CKSUM_START:CKSUM_OFF].astype(np.uint64))) & 0xffffffff

def stored_cksum(dec):
    return int(dec[CKSUM_OFF:CKSUM_OFF+4].view('<u4')[0])

def set_cksum(dec):
    v=compute_cksum(dec)
    dec[CKSUM_OFF:CKSUM_OFF+4]=np.frombuffer(v.to_bytes(4,'little'),np.uint8)
    return v

def main():
    cmd=sys.argv[1]
    if cmd=='decrypt':
        e=np.fromfile(sys.argv[2],np.uint8); decrypt(e).tofile(sys.argv[3])
    elif cmd=='checksum':
        d=np.fromfile(sys.argv[2],np.uint8)
        print('stored   %#010x'%stored_cksum(d)); print('computed %#010x'%compute_cksum(d))
        print('MATCH' if stored_cksum(d)==compute_cksum(d) else 'MISMATCH')
    elif cmd=='patch':
        d=np.fromfile(sys.argv[2],np.uint8)
        for a in sys.argv[4:]:
            off,hx=a.split('='); off=int(off,0); b=bytes.fromhex(hx)
            d[off:off+len(b)]=np.frombuffer(b,np.uint8)
            print('patched %#x <- %s'%(off,hx))
        v=set_cksum(d); print('new checksum %#010x'%v); d.tofile(sys.argv[3])
    elif cmd=='encrypt':
        d=np.fromfile(sys.argv[2],np.uint8); v=set_cksum(d)
        encrypt(d).tofile(sys.argv[3]); print('checksum %#010x -> %s'%(v,sys.argv[3]))
    else: print(__doc__)

if __name__=='__main__': main()
