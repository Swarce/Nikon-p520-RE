import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
import struct, collections
D=open(p520paths.DEC,'rb').read()
SEGS=[('s1',0x360364,0x584f94),('s2',0x584fe8,0x75557c),('s3',0x7555d0,0x806300),
      ('s4',0x806354,0x80aa00),('s5a',0x80aa74,0xa7ced0),('s5b',0xa7ced0,0xae6b54)]
def hdr(o):
    t,osz,clen=struct.unpack('<III',D[o:o+12]); a,b=struct.unpack('<HH',D[o+12:o+16])
    return t,osz,clen,a,b
def tables(o):
    t,osz,clen,a,b=hdr(o)
    A=struct.unpack('<%dI'%(a//4),D[o+16:o+16+a]); B=struct.unpack('<%dI'%(b//4),D[o+16+a:o+16+a+b])
    return A,B,D[o+16+a+b:o+16+a+b+clen],osz
def blocks():
    for nm,s,e in SEGS:
        o=s
        while o<e:
            t,osz,clen,a,b=hdr(o)
            yield nm,o
            o+=16+a+b+clen
def node(x):
    """('L',val,n) | ('Q'|'D',a,b,start,size)"""
    if x&3==3: return ('L',(x&0x7fff)>>2,x>>15)
    a=x>>15; b=(x>>11)&15; w=(x>>2)&0x1ff
    if x&1: return ('Q',a,b,w+1,a+b)
    return ('D',a,b,w,1<<b)
def P(x):
    k=node(x)
    if k[0]=='L': return 'L%x/%d'%k[1:]
    return '%s(a%d,b%d,@%d)'%(k[0],k[1],k[2],k[3])
