import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
import struct,re,sys
D=open(p520paths.DEC,'rb').read()
offs=[m.start() for m in re.finditer(rb'\x7fELF\x01\x01\x01',D)]
def info(o):
    e_type,e_mach=struct.unpack('<HH',D[o+16:o+20])
    e_entry,e_phoff,e_shoff=struct.unpack('<III',D[o+24:o+36])
    e_flags,e_ehsz,e_phentsz,e_phnum,e_shentsz,e_shnum,e_shstrndx=struct.unpack('<IHHHHHH',D[o+36:o+52])
    phs=[struct.unpack('<8I',D[o+e_phoff+i*32:o+e_phoff+i*32+32]) for i in range(e_phnum)]
    end=max(p[1]+p[4] for p in phs) if phs else 0
    secs=[]
    if e_shnum and e_shoff< 0x8000000 and o+e_shoff+e_shnum*40<=len(D):
        shs=[struct.unpack('<10I',D[o+e_shoff+i*40:o+e_shoff+i*40+40]) for i in range(e_shnum)]
        try:
            stro=o+shs[e_shstrndx][4]
            for sh in shs:
                nm=D[stro+sh[0]:D.index(b'\0',stro+sh[0])].decode('latin1')
                secs.append((nm,sh[3],sh[4],sh[5],sh[1]))
            end=max(end,e_shoff+e_shnum*40)
        except Exception as ex: secs=['err']
    return e_type,e_mach,e_entry,phs,secs,end
if __name__=='__main__':
  for o in offs:
    t,m,en,phs,secs,end=info(o)
    print('ELF@%#x type%d mach%d entry %#x  phnum %d shnum %d  span->%#x'%(o,t,m,en,len(phs),len(secs),o+end))
    for p in phs: print('   PH off %#x va %#x pa %#x fsz %#x msz %#x fl%d'%(p[1],p[2],p[3],p[4],p[5],p[6]))
    for s in secs[:12]: print('   SH',s)
