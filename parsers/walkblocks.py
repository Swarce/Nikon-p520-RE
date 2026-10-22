import struct,re
def segments(D):
    segs=[]
    for m in re.finditer(rb'\x7fELF\x01\x01\x01',D):
        o=m.start()
        try:
            e_entry,e_phoff=struct.unpack('<II',D[o+24:o+32]); e_phnum=struct.unpack('<H',D[o+44:o+46])[0]
        except: continue
        if e_entry!=0x80000600: continue
        for i in range(e_phnum):
            p=struct.unpack('<8I',D[o+e_phoff+i*32:o+e_phoff+i*32+32])
            if p[0]==1 and p[4]>0x100:
                segs.append((o+p[1], o+p[1]+p[4], p[2]))  # start, end, va
    return segs
def walk(D):
    out=[]
    for s,e,va in segments(D):
        o=s
        while o+16<=e:
            t,osz,clen=struct.unpack('<III',D[o:o+12]); a,b=struct.unpack('<HH',D[o+12:o+16])
            if t!=2 or clen==0 or clen>e-o or a>0x10000 or b>0x10000: break
            out.append((o,osz,clen,a,b,va)); 
            o+=16+a+b+clen
    return out
if __name__=='__main__':
    import sys
    for name,path in [('P520','dec.bin'),('P530','p530.bin'),('P600','p600.bin')]:
        D=open(path,'rb').read(); bl=walk(D)
        if not bl: print(name,'no blocks found'); continue
        bl2=sorted(bl,key=lambda x:-(x[1]/max(x[2],1)))
        print('\n%s: %d type-2 blocks. top ratio:'%(name,len(bl)))
        for o,osz,clen,a,b,va in bl2[:5]:
            print('  @%#x osz=%d clen=%d ratio=%.1f a=%d b=%d'%(o,osz,clen,osz/max(clen,1),a,b))
