import sys,itertools; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
from decode import inflate
from zt import tables
o=0x360364
res=[]
for order in ['lsb','msb','w32','w32lsb','w16']:
 for dref in (0,1):
  for dinv in (0,1):
   for qinv in (0,1):
    for xref in (0,1):
     for lsep in (0,1):
      for dsep in (0,1):
        cfg=dict(order=order,dref=dref,dinv=dinv,qinv=qinv,xref=xref,lenx_sep=lsep,distx_sep=dsep)
        try: out,p,st,bad=inflate(o,cfg,limit=4)
        except Exception as ex: continue
        if len(out)>=4 and out[2]==0xBD and out[3]==0x27:
            res.append((cfg,out[:4].hex()))
for r in res: print(r)
print('matches:',len(res))
