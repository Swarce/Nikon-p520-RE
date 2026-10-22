import sys,capstone,pickle,bisect; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import D
addrs,names=p520paths.load_mainsyms()
SEG=(0xc0000800,0x205eb4,0x14e058)
def sym(a):
    a=(a&0x1fffffff)|0xc0000000; i=bisect.bisect_right(addrs,a)-1
    if i<0:return '?'
    return names[i]+('+%#x'%(a-addrs[i]) if a-addrs[i] else '')
tgt=[int(x,0) for x in sys.argv[1:]]
md=capstone.Cs(capstone.CS_ARCH_MIPS,capstone.CS_MODE_MIPS32+capstone.CS_MODE_LITTLE_ENDIAN)
va,fo,sz=SEG
hits={t:[] for t in tgt}
for i in md.disasm(D[fo:fo+sz],va):
    if i.mnemonic in ('jal','j','bal'):
        try: t=int(i.op_str,16)
        except: continue
        tn=(t&0x1fffffff)|0xc0000000
        if tn in hits: hits[tn].append(i.address)
for t in tgt:
    print('xrefs to %08x (%s):'%(t,sym(t)))
    for a in hits[t]: print('   %08x  in %s'%(a,sym(a)))
