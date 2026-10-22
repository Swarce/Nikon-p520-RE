import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from decode4 import inflate
o=0x360364
for order in ['lsb','msb','w32','w32lsb','w16']:
  for xref in (0,1):
    try: out,bad,nm,st,p=inflate(o,order,xref,lim=6000)
    except Exception as e: print(order,xref,'ERR',e); continue
    print('%-6s xref%d len%5d bad%4d/%4d %-6s %s'%(order,xref,len(out),bad,nm,st,out[:12].hex()))
