import sys,itertools; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
import capstone; from decode2 import inflate
md=capstone.Cs(capstone.CS_ARCH_MIPS,capstone.CS_MODE_MIPS32+capstone.CS_MODE_LITTLE_ENDIAN)
COMMON=set('addiu lw sw lui jr jal move nop beq bne andi ori sll srl addu subu sltu or and slti sltiu bnez beqz b lbu sb jalr lhu negu xori nor lhu lb sh'.split())
def mscore(buf):
    sc=0; tot=0
    for i in md.disasm(buf,0):
        tot+=1
        if i.mnemonic in COMMON: sc+=1
    return sc
o=0x360364
best=[]
orders=['lsb','msb','w32','w32lsb','w16','w16lsb']
for order in orders:
 for dref,dinv,qinv,xref,lsep,dsep,dadv in itertools.product((0,1),(0,1),(0,1),(0,1),(0,1),(0,1),('codelen','full')):
    cfg=dict(order=order,dref=dref,dinv=dinv,qinv=qinv,xref=xref,lenx_sep=lsep,distx_sep=dsep,dadv=dadv)
    try: out,p,st,bad=inflate(o,cfg,limit=256)
    except Exception: continue
    if len(out)<64: continue
    sc=mscore(out[:256])
    key=(sc,-bad,len(out))
    best.append((key, out[:8].hex(), cfg))
best.sort(key=lambda t:t[0], reverse=True)
for key,hx,cfg in best[:14]:
    print('score%3d bad%3d len%4d %s | %s %s'%(key[0],-key[1],key[2],hx,cfg['order'],{k:v for k,v in cfg.items() if k!='order'}))
