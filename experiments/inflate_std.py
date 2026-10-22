import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
from lengths import leaf_len_values
from canon import LEN_ORDER, canonical
# distance: value -> distcode number. value encodes extra=v>>2, mant=v&3.
def dist_order():
    vals=[0,1,2,3]
    for ex in range(1,14):
        vals+=[ (2<<ex)|0 if False else ((2<<ex)) ]  # placeholder
    return None
# Standard deflate tables
LBASE=[3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258]
LEXT =[0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0]
DBASE=[1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577]
DEXT =[0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13]
def B_lens(T):
    vals=leaf_len_values(T,True)
    # value v -> distcode: find index in DBASE by (extra=v>>2, mant=v&3): base=((v&3)<<(v>>2))+1
    lens=[0]*30
    for v,L in vals.items():
        ex=v>>2; base=((v&3)<<ex)+1
        if base in DBASE and DEXT[DBASE.index(base)]==ex:
            lens[DBASE.index(base)]=L
    return lens
def A_lens(T):
    vals=leaf_len_values(T,False)
    out=[vals.get(i,0) for i in range(256)]+[vals.get(0x100,0)]+[vals.get(v,0) for v in LEN_ORDER]
    return out
class BR:
    def __init__(s,data,order): 
        from walk import bitstream; s.b=bitstream(data,order); s.p=0;s.n=len(s.b)
    def code(s,table):  # table: dict code_int->(sym,len) ; read MSB-first
        c=0
        for L in range(1,17):
            c=(c<<1)|(1 if (s.p<s.n and s.b[s.p]=='1') else 0); s.p+=1
            if (c,L) in table: return table[(c,L)]
        return None
    def bitsLSB(s,k):
        v=0
        for i in range(k):
            if s.p<s.n and s.b[s.p]=='1': v|=(1<<i)
            s.p+=1
        return v
def mktab(codes):  # sym->(code,len)  ==> (code,len)->sym
    return {(c,l):sym for sym,(c,l) in codes.items()}
def inflate(o,order,lim=None):
    A,B,C,osz=tables(o); al=A_lens(A); bl=B_lens(B)
    ca=mktab(canonical(al)); cb=mktab(canonical(bl))
    br=BR(C,order); out=bytearray(); bad=0;nm=0; lim=lim or osz
    while br.p<br.n-1 and len(out)<lim:
        r=br.code(ca)
        if r is None: return bytes(out),bad,nm,'NOSYM'
        if r<256: out.append(r); continue
        if r==256: return bytes(out),bad,nm,'EOB'
        li=r-257; L=LBASE[li]+br.bitsLSB(LEXT[li])
        d=br.code(cb)
        if d is None: return bytes(out),bad,nm,'NODIST'
        dist=DBASE[d]+br.bitsLSB(DEXT[d]); nm+=1
        if not(0<dist<=len(out)): bad+=1; dist=1
        for _ in range(L): out.append(out[-dist] if out else 0)
    return bytes(out),bad,nm,'END'
if __name__=='__main__':
    o=0x360364
    for order in ['lsb','msb','w32','w32lsb','w16','w16be']:
        try: out,bad,nm,st=inflate(o,order,lim=8000)
        except Exception as e: print(order,'ERR',repr(e)); continue
        print('%-6s len%5d bad%4d/%4d %-6s %s'%(order,len(out),bad,nm,st,out[:12].hex()))
