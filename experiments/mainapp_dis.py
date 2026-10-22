import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
import numpy as np, pickle, bisect, re, sys, capstone
D=np.fromfile(p520paths.DEC,dtype=np.uint8).tobytes()
addrs,names=p520paths.load_mainsyms()
SEGS=[(0xc0000800,0x205eb4,0x14e058),(0xc0000180,0x205b14,0x1a0),(0xc0000600,0x205cb4,0x200)]
def v2f(a):
    a=(a&0x1fffffff)|0xc0000000
    for va,fo,sz in SEGS:
        if va<=a<va+sz: return fo+a-va
    return None
def sym(a):
    a=(a&0x1fffffff)|0xc0000000
    i=bisect.bisect_right(addrs,a)-1
    if i<0: return '?'
    off=a-addrs[i]
    return names[i]+('+%#x'%off if off else '')
def fsize(name):
    i=names.index(name); a=addrs[i]
    j=i+1
    while addrs[j]==a: j+=1
    return a,addrs[j]-a
md=capstone.Cs(capstone.CS_ARCH_MIPS,capstone.CS_MODE_MIPS32+capstone.CS_MODE_LITTLE_ENDIAN)
def cstr(a):
    o=v2f(a)
    if o is None: return None
    s=D[o:o+80].split(b'\0')[0]
    return s.decode('latin1') if len(s)>=3 and all(32<=c<127 or c in(9,10) for c in s) else None
def dis(name=None,a=None,sz=None,out=print):
    if name: a,sz=fsize(name)
    o=v2f(a); lui={}
    out('=== %s @%08x size %#x'%(sym(a),a,sz))
    for i in md.disasm(D[o:o+sz],a):
        ops=i.op_str.split(', '); cm=''
        if i.mnemonic=='lui': lui[ops[0]]=int(ops[1],16)<<16
        elif len(ops)>=2:
            m=re.match(r'(-?0x[0-9a-f]+|\d+)?\((\$\w+)\)',ops[-1])
            if m and m.group(2) in lui and i.mnemonic in('lw','sw','lbu','lhu','lb','lh','sb','sh'):
                off=int(m.group(1),0) if m.group(1) else 0
                cm=' ; [%s]'%sym(lui[m.group(2)]+off)
            elif i.mnemonic in('addiu','ori') and len(ops)==3 and ops[1] in lui:
                imm=int(ops[2],16)
                v=(lui[ops[1]]+(imm if i.mnemonic=='ori' else ((imm+0x8000)&0xffff)-0x8000))&0xffffffff
                s=cstr(v); cm=' ; %08x %s'%(v,repr(s) if s else sym(v))
        if i.mnemonic in('jal','j'): cm=' ; '+sym(int(i.op_str,16))
        out('%08x: %-8s %s%s'%(i.address,i.mnemonic,i.op_str,cm))
if __name__=='__main__':
    for n in sys.argv[1:]: dis(n)
