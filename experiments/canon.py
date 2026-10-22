import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
from lengths import leaf_len_values
# symbol index mapping: literals 0..255 ->0..255, EOB(0x100)->256, length codes 0x200..->257..285
LEN_ORDER=[0x203,0x204,0x205,0x206,0x207,0x208,0x209,0x20a,0x60b,0x60d,0x60f,0x611,
           0xa13,0xa17,0xa1b,0xa1f,0xe23,0xe2b,0xe33,0xe3b,0x1243,0x1253,0x1263,0x1273,
           0x1683,0x16a3,0x16c3,0x16e3,0x302]  # 29 length symbols, in symbol-number order 257..285
LEN_EXTRA={0x203:0,0x204:0,0x205:0,0x206:0,0x207:0,0x208:0,0x209:0,0x20a:0,0x302:0,
           0x60b:1,0x60d:1,0x60f:1,0x611:1,0xa13:2,0xa17:2,0xa1b:2,0xa1f:2,
           0xe23:3,0xe2b:3,0xe33:3,0xe3b:3,0x1243:4,0x1253:4,0x1263:4,0x1273:4,
           0x1683:5,0x16a3:5,0x16c3:5,0x16e3:5}
LEN_BASE ={v:(v&0x1ff) for v in LEN_ORDER}
def A_symlens(T):
    vals=leaf_len_values(T,False)
    lit=[vals.get(i,0) for i in range(256)]
    eob=[vals.get(0x100,0)]
    lens=[vals.get(v,0) for v in LEN_ORDER]
    return lit+eob+lens   # 286 entries
def B_symlens(T):
    vals=leaf_len_values(T,True)
    order=sorted(vals.keys(), key=lambda v:((v>>2),(v&3)))  # distance symbol number order
    # distance symbol n: extra=n//... actually map value->distcode number
    # value encodes extra=v>>2, mant=v&3; distcode number = 2*extra + mant_index? Build standard 30.
    return vals
def canonical(lengths):
    maxb=max(lengths) if lengths else 0
    blc=[0]*(maxb+1)
    for l in lengths:
        if l: blc[l]+=1
    code=0; nxt=[0]*(maxb+2); 
    for b in range(1,maxb+1): code=(code+blc[b-1])<<1; nxt[b]=code
    codes={}
    for sym,l in enumerate(lengths):
        if l: codes[sym]=(nxt[l],l); nxt[l]+=1
    return codes  # sym->(code,len), MSB-first
if __name__=='__main__':
    A,B,C,osz=tables(0x360364)
    al=A_symlens(A)
    print('A symlens: lit nonzero=%d eob=%d lens=%s'%(sum(1 for x in al[:256] if x),al[256],al[257:]))
    print('maxlen',max(al))
    cc=canonical(al); 
    print('sample codes: sym0 len',al[0],'sym0x24',al[0x24],'EOB',al[256])
