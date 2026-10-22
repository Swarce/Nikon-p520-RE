#!/usr/bin/env python3
"""Fix a Nikon COOLPIX P520 (RAW-mod) .NRW so standard raw converters open it.

The COACH raw path (never finished on the P520) writes the CFA strip as BIG-endian
16-bit samples under a little-endian ('II') TIFF header, and tags BitsPerSample=12
although the data is full 16-bit. Standard tools (LibRaw/dcraw/RawTherapee/darktable)
honor the container's byte order and the 12-bit tag, so they mis-read the pixels.

This rewrites the file in place-corrected form:
  * byte-swaps every 16-bit sample in each CFA strip (PhotometricInterpretation==32803)
  * sets that SubIFD's BitsPerSample to 16
After this the file opens correctly as a 16-bit little-endian Bayer raw. (Color is not
camera-calibrated — there is no Nikon colour matrix for 'Zoran COACH' — so set white
balance / profile in your converter.)

Usage:  python3 -I nrwfix.py IN.NRW OUT.NRW
"""
import sys, struct
TSZ={1:1,2:1,3:2,4:4,5:8,6:1,7:1,8:2,9:4,10:8,11:4,12:8}
def fix(src,dst):
    d=bytearray(open(src,'rb').read())
    if d[:2]!=b'II': sys.exit('not a little-endian TIFF/NRW')
    bo='<'
    def ifd(off):
        n=struct.unpack(bo+'H',d[off:off+2])[0]; e={}
        for i in range(n):
            o=off+2+i*12; tag,typ,cnt=struct.unpack(bo+'HHI',d[o:o+8]); e[tag]=(typ,cnt,o+8)
        return e
    def gv(e,tag):
        typ,cnt,vo=e[tag]; sz=TSZ[typ]*cnt
        raw=d[struct.unpack(bo+'I',d[vo:vo+4])[0]:][:sz] if sz>4 else d[vo:vo+sz]
        return list(struct.unpack(bo+{1:'B',3:'H',4:'I'}[typ]*cnt, bytes(raw[:TSZ[typ]*cnt])))
    e0=ifd(struct.unpack(bo+'I',d[4:8])[0])
    subs=gv(e0,330) if 330 in e0 else []
    fixed=0
    for sub in subs:
        es=ifd(sub)
        if es.get(262) and gv(es,262)[0]==32803:
            off=gv(es,273)[0]; cnt=gv(es,279)[0]
            s=bytearray(d[off:off+cnt]); s[0::2],s[1::2]=s[1::2],s[0::2]; d[off:off+cnt]=s
            if 258 in es: struct.pack_into(bo+'H',d,es[258][2],16)
            fixed+=cnt//2
    if not fixed: sys.exit('no CFA (PhotometricInterpretation 32803) strip found')
    open(dst,'wb').write(d)
    print('fixed %d samples (byteswap + BitsPerSample=16) -> %s'%(fixed,dst))
if __name__=='__main__':
    if len(sys.argv)!=3: sys.exit(__doc__)
    fix(sys.argv[1],sys.argv[2])
