#!/usr/bin/env python3
"""Download + unpack the Nikon firmware images this project works on, then decrypt them.

  P520 v1.1  F-P520-V11W.exe  -> firmware/firmware.bin       -> work/dec.bin
  P530 v1.1  F-P530-V11W.exe  -> firmware/p530/firmware.bin  -> work/p530.bin
  P600 v1.2  F-P600-V12W.exe  -> firmware/p600/firmware.bin  -> work/p600.bin

usage:
  fetch_firmware.py                 download all three from Nikon, extract, decrypt
  fetch_firmware.py --only p520     just one model
  fetch_firmware.py --local DIR     no network: use .exe / .zip / firmware.bin files already in DIR
                                    (a raw .bin is recognised by its plaintext 'P520'/'P530'/'P600' header)
  fetch_firmware.py --no-decrypt    stop after placing firmware.bin

The URLs are Nikon Download Center's Windows links (the "Agree - Download" target on
downloadcenter.nikonimglib.com/en/download/fw/{41,42,43}.html). By using them you accept
Nikon's firmware licence shown on those pages.
"""
import argparse, os, shutil, subprocess, sys, tempfile, urllib.request, zipfile
sys.path.insert(0,os.path.join(os.path.dirname(os.path.abspath(__file__)),'..','parsers')); import p520paths

MODELS={
 'p520':dict(tag=b'P520',exe='F-P520-V11W.exe',size=81264928,dst='',dec='dec.bin',page=41,
   url='https://crossgate.nikonimglib.com/dsd_redirect/redirect.do?P=oe77D29&R=2LbNX05&L=ptqmS01&O=WSYvv00'),
 'p530':dict(tag=b'P530',exe='F-P530-V11W.exe',size=None,dst='p530',dec='p530.bin',page=42,
   url='https://crossgate.nikonimglib.com/dsd_redirect/redirect.do?P=UTvzw80&R=nBoFD07&L=oJrea01&O=V9q8600'),
 'p600':dict(tag=b'P600',exe='F-P600-V12W.exe',size=None,dst='p600',dec='p600.bin',page=43,
   url='https://crossgate.nikonimglib.com/dsd_redirect/redirect.do?P=bvQvg58&R=UPYlL14&L=d0scQ01&O=Umask00'),
}

def download(url,out):
    print('  GET',url)
    with urllib.request.urlopen(urllib.request.Request(url,headers={'User-Agent':'Mozilla/5.0'})) as r, open(out,'wb') as f:
        shutil.copyfileobj(r,f,1<<20)
    print('  ->',out,os.path.getsize(out),'B')

def extract(archive,outdir):
    """Nikon's Windows updaters are self-extracting archives; zipfile tolerates the SFX stub."""
    try:
        with zipfile.ZipFile(archive) as z: z.extractall(outdir); return True
    except zipfile.BadZipFile: pass
    for cmd in (['7z','x','-y','-o'+outdir,archive],['7zz','x','-y','-o'+outdir,archive],
                ['bsdtar','-xf',archive,'-C',outdir],['unzip','-o',archive,'-d',outdir]):
        if shutil.which(cmd[0]) and subprocess.run(cmd,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL).returncode==0:
            return True
    return False

def find_bin(root,tag):
    for dp,_,fs in os.walk(root):
        for f in fs:
            p=os.path.join(dp,f)
            if f.lower().endswith('.bin') and open(p,'rb').read(4)==tag: return p
    return None

def place(m,binpath):
    dstdir=os.path.join(p520paths.FW_DIR,m['dst']); os.makedirs(dstdir,exist_ok=True)
    dst=os.path.join(dstdir,'firmware.bin'); shutil.copyfile(binpath,dst)
    sz=os.path.getsize(dst)
    if m['size'] and sz!=m['size']: print('  WARNING size %d != expected %d'%(sz,m['size']))
    print('  firmware.bin ->',dst,sz,'B'); return dst

def decrypt(name,m,fw):
    os.makedirs(p520paths.WORK,exist_ok=True); out=os.path.join(p520paths.WORK,m['dec'])
    here=os.path.dirname(os.path.abspath(__file__))
    script=[os.path.join(here,'..','parsers','decrypt.py'),fw,out] if name=='p520' else [os.path.join(here,'dec_sib.py'),fw,out]
    subprocess.run([sys.executable,'-I']+script,check=True)

def main():
    ap=argparse.ArgumentParser(description=__doc__,formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--only',choices=sorted(MODELS),action='append')
    ap.add_argument('--local',metavar='DIR')
    ap.add_argument('--no-decrypt',action='store_true')
    a=ap.parse_args()
    dl=os.path.join(p520paths.FW_DIR,'downloads'); os.makedirs(dl,exist_ok=True)
    for name in a.only or sorted(MODELS):
        m=MODELS[name]; print('==',name)
        with tempfile.TemporaryDirectory() as tmp:
            if a.local:
                binpath=find_bin(a.local,m['tag'])
                if not binpath:
                    for f in sorted(os.listdir(a.local)):
                        if name.upper() in f.upper() and f.lower().endswith(('.exe','.zip')):
                            extract(os.path.join(a.local,f),tmp); binpath=find_bin(tmp,m['tag'])
                            if binpath: break
            else:
                exe=os.path.join(dl,m['exe'])
                if not os.path.exists(exe): download(m['url'],exe)
                if not extract(exe,tmp): sys.exit('  could not unpack %s (install p7zip and retry)'%exe)
                binpath=find_bin(tmp,m['tag'])
            if not binpath:
                print('  no %s firmware.bin found -- skipped'%m['tag'].decode()); continue
            fw=place(m,binpath)
        if not a.no_decrypt: decrypt(name,m,fw)

if __name__=='__main__': main()
