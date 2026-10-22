import sys,struct; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import D
def parse(o):
    assert D[o:o+4]==b'\x7fELF', D[o:o+4]
    e_type,e_mach=struct.unpack('<HH',D[o+16:o+20])
    e_entry,e_phoff,e_shoff=struct.unpack('<III',D[o+24:o+36])
    e_phentsz,e_phnum=struct.unpack('<HH',D[o+42:o+46])
    print('ELF @%#x type%d mach%d entry %#x phoff %#x phnum %d'%(o,e_type,e_mach,e_entry,e_phoff,e_phnum))
    phs=[]
    for i in range(e_phnum):
        b=o+e_phoff+i*e_phentsz
        p_type,p_off,p_va,p_pa,p_fsz,p_msz,p_flg,p_al=struct.unpack('<IIIIIIII',D[b:b+32])
        print('  ph%d type%d off %#x va %#x filesz %#x memsz %#x flg%d'%(i,p_type,p_off,p_va,p_fsz,p_msz,p_flg))
        phs.append((p_type,p_off,p_va,p_fsz,p_msz))
    return e_entry,phs
if __name__=='__main__':
    for a in sys.argv[1:]: parse(int(a,0))
