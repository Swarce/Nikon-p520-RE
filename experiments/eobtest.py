import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from zt import *
import zinf
from search import sym2
def run(o,order,iq,idd,xl,skip=0):
    A,B,C,osz=tables(o); bs=zinf.Bits(C,order); bs.p=skip; n=0; bad=0; nsym=0
    while bs.p<bs.n and n<osz*2:
        e,cons=sym2(bs,A,iq,idd); v=(e&0x7fff)>>2; nsym+=1
        if v<0x100: bs.p+=cons; n+=1
        elif v==0x100: bs.p+=cons; return n,bs.p,'EOB',bad
        else:
            ex=(v>>10)&7; bs.p+=cons-ex; L=(v&0x1ff)+bs.get(ex,xl)
            e2,c2=sym2(bs,B,iq,idd); dv=(e2&0x7fff)>>2; dex=dv>>2 if dv>=4 else 0
            bs.p+=c2; dist=(((dv&3)<<dex)+1 if dv>=4 else dv+1)+bs.get(dex,xl)
            if dist>n: bad+=1
            n+=L
    return n,bs.p,'END',bad
if __name__=='__main__':
    o=int(sys.argv[1],0)
    A,B,C,osz=tables(o); real=len(C.rstrip(b'\0'))
    print('osz %#x clen %#x nonzero-end %#x bits %d'%(osz,len(C),real,real*8))
    for order in ['lsb','msb','w32','w16']:
      for iq in (0,1):
        for idd in (0,1):
          for xl in (0,1):
            print(order,iq,idd,xl,run(o,order,iq,idd,xl))
