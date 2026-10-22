import sys,itertools; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
import capstone; from decode3 import inflate
md=capstone.Cs(capstone.CS_ARCH_MIPS,capstone.CS_MODE_MIPS32+capstone.CS_MODE_LITTLE_ENDIAN)
COMMON=set('addiu lw sw lui jr jal move nop beq bne andi ori sll srl addu subu sltu or and slti sltiu bnez beqz b lbu sb jalr lhu negu xori nor lb sh mult'.split())
def score(buf):
    return sum(1 for i in md.disasm(buf,0) if i.mnemonic in COMMON)
o=0x360364; best=[]
for order in ['lsb','msb','w32','w32lsb','w16']:
 for dref,dinv,qinv,xref in itertools.product((0,1),repeat=4):
    cfg=dict(order=order,dref=dref,dinv=dinv,qinv=qinv,xref=xref)
    try: out,p,st,bad=inflate(o,cfg,limit=512)
    except Exception as ex: continue
    if len(out)<64: continue
    best.append(((score(out[:512]),-bad,len(out)),out[:12].hex(),cfg))
best.sort(key=lambda t:t[0],reverse=True)
for key,hx,cfg in best[:10]:
    print('score%3d bad%3d len%4d %s | %s %s'%(key[0],-key[1],key[2],hx,cfg['order'],{k:v for k,v in cfg.items() if k!='order'}))
