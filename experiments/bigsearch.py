import sys,itertools; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
from walk import bitstream
import capstone
md=capstone.Cs(capstone.CS_ARCH_MIPS,capstone.CS_MODE_MIPS32+capstone.CS_MODE_LITTLE_ENDIAN)
COMMON=set('addiu lw sw lui jr jal move nop beq bne andi ori sll srl addu subu sltu or and slti sltiu bnez beqz b lbu sb jalr lhu negu xori nor lb sh mult multu div bgez blez bgtz mflo mfhi'.split())
class W:
    def __init__(s,bits,cfg): s.b=bits;s.p=0;s.n=len(bits);s.cfg=cfg
    def sym(s,T,dist):
        x=T[0]; cfg=s.cfg
        while True:
            k=node(x); ch=T[k[3]:k[3]+k[4]]
            if k[0]=='D':
                a,b=k[1],k[2]; save=s.p; raw=s.b[s.p:s.p+b].ljust(b,'0')
                if cfg['dref']: raw=raw[::-1]
                i=int(raw,2) if b else 0; e=ch[i]
                if e&3==3:
                    v=(e&0x7fff)>>2; xb=(v>>10)&7 if (not dist and v&0x200) else 0
                    nc=(e>>15)-xb; s.p=save+(a if cfg['add_a'] else 0)+nc; return v,xb
                s.p=save+b; x=e
            else:
                m=k[4]; j=0; one='0' if cfg['qinv'] else '1'
                while j<m-1 and s.p<s.n and s.b[s.p]==one: j+=1;s.p+=1
                if j<m-1: s.p+=1
                idx=(m-1-j) if cfg['qrev'] else j
                e=ch[idx]
                if e&3==3:
                    v=(e&0x7fff)>>2; xb=(v>>10)&7 if (not dist and v&0x200) else 0
                    return v,xb
                x=e
    def rd(s,k,ref):
        raw=s.b[s.p:s.p+k].ljust(k,'0'); s.p+=k
        if ref: raw=raw[::-1]
        return int(raw,2) if k else 0
def dec(o,cfg,lim):
    A,B,C,osz=tables(o); w=W(bitstream(C,cfg['order']),cfg); out=bytearray(); bad=0;nm=0
    eob=False
    while w.p<w.n-1 and len(out)<lim:
        v,xb=w.sym(A,False)
        if v<0x100: out.append(v); continue
        if v==0x100: eob=True; break
        L=(v&0x1ff)+w.rd(xb,cfg['xref']); dv,_=w.sym(B,True); dex=dv>>2
        dist=((dv&3)<<dex)+1+w.rd(dex,cfg['xref']); nm+=1
        if not(0<dist<=len(out)): bad+=1; dist=1
        for _ in range(L): out.append(out[-dist] if out else 0)
    return bytes(out),bad,nm,eob,osz
def mips(buf):
    return sum(1 for i in md.disasm(buf,0) if i.mnemonic in COMMON)
o=0x360364; res=[]
for order in ['lsb','msb','w32','w32lsb','w16','w16be']:
 for dref,qinv,qrev,add_a,xref in itertools.product((0,1),repeat=5):
    cfg=dict(order=order,dref=dref,qinv=qinv,qrev=qrev,add_a=add_a,xref=xref)
    try: out,bad,nm,eob,osz=dec(o,cfg,2000)
    except Exception: continue
    if len(out)<100: continue
    sc=mips(out[:2000])
    res.append(((-bad if nm else -9999, sc), bad,nm,len(out),eob,out[:8].hex(),cfg))
res.sort(key=lambda t:t[0],reverse=True)
for key,bad,nm,ln,eob,hx,cfg in res[:12]:
    print('bad%4d/%4d mips%4d len%4d eob%d %s | %s %s'%(bad,nm,key[1],ln,eob,hx,cfg['order'],{k:v for k,v in cfg.items() if k!='order'}))
