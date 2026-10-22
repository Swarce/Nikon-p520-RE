import sys; sys.path.insert(0,__import__('os').path.join(__import__('os').path.dirname(__import__('os').path.abspath(__file__)),'..','parsers')); import p520paths
import pickle,bisect,sys
addrs,names=p520paths.load_mainsyms()
def around(a,n=12):
    i=bisect.bisect_left(addrs,a)
    for j in range(max(0,i-1),min(len(addrs),i+n)):
        print('%08x %+8d %s'%(addrs[j],addrs[j]-a,names[j]))
if __name__=='__main__':
    for a in sys.argv[1:]: 
        print('--- around %s'%a); around(int(a,0))
