#!/usr/bin/env python3
"""
p520tool - USB/PTP/SCSI prober + UART terminal for Nikon COOLPIX P520 RE.

Subcommands:
  usb-probe   list USB devices + descriptors (which mode/class the cam is in)
  ptp-info    PTP GetDeviceInfo -> supported operation codes (incl vendor 0x9xxx)
  ptp-send    send one PTP operation (read-biased; writes need --force)
  scsi-inq    standard SCSI INQUIRY over mass-storage (safe, confirms BOT works)
  scsi        send one raw SCSI CDB over mass-storage (read-biased; --dir out needs --force)
  scsi-scan   probe vendor SCSI read-opcodes 0xC0..0xFF (IN only; needs --confirm)
  serial      UART terminal (auto-baud + logging) for a USB-UART adapter

Deps:  pip install pyusb pyserial   (+ libusb backend; Windows: Zadig for usb-probe/ptp/scsi)

SAFETY
  * PTP/SCSI probes default to READ direction and standard, documented commands.
  * The tool never issues an OUT/WRITE transfer or an undocumented opcode unless
    you pass --force (and --confirm for the bulk scan). Blind vendor WRITEs can
    brick a camera - research each opcode (libgphoto2 ptp.h, Nikon notes, the
    72k-symbol map) before using --force.
  * Pull & keep a copy of anything you read before you ever write.
"""
import sys, struct, argparse

NIKON_VID = 0x04b0

# --- libusb backend resolution (fixes Windows "No backend available") ---
def _backend():
    # Prefer the pip-installable bundled libusb so no manual DLL hunting is needed.
    try:
        import libusb_package
        return libusb_package.get_libusb1_backend()
    except Exception:
        return None  # fall back to pyusb's own search (system libusb-1.0)

def ufind(**kw):
    import usb.core
    bk=_backend()
    try:
        return usb.core.find(backend=bk, **kw)
    except Exception as e:
        from usb.core import NoBackendError
        if isinstance(e, NoBackendError):
            print("No libusb backend. Fix on Windows:\n"
                  "   pip install libusb-package\n"
                  " (that bundles libusb-1.0.dll). Then re-run. For the commands that\n"
                  " talk to the camera (ptp-*/scsi-*), you also need a WinUSB/libusb\n"
                  " driver bound to the camera interface via Zadig - see README.")
            raise SystemExit(2)
        raise

PTP_OPS = {
 0x1001:"GetDeviceInfo",0x1002:"OpenSession",0x1003:"CloseSession",0x1004:"GetStorageIDs",
 0x1005:"GetStorageInfo",0x1006:"GetNumObjects",0x1007:"GetObjectHandles",0x1008:"GetObjectInfo",
 0x1009:"GetObject",0x100A:"GetThumb",0x100B:"DeleteObject",0x100C:"SendObjectInfo",
 0x100D:"SendObject",0x100E:"InitiateCapture",0x100F:"FormatStore",0x1010:"ResetDevice",
 0x1011:"SelfTest",0x1012:"SetObjectProtection",0x1014:"GetDevicePropDesc",0x1015:"GetDevicePropValue",
 0x1016:"SetDevicePropValue",0x1017:"ResetDevicePropValue",0x1019:"MoveObject",0x101A:"CopyObject",
 0x101B:"GetPartialObject",0x101C:"InitiateOpenCapture",
 0x90C0:"Nikon_Capture",0x9801:"Nikon_GetObjectPropsSupported",0x9802:"Nikon_GetObjectPropDesc",
 0x9803:"Nikon_GetObjectPropValue",0x9810:"Nikon_GetObjectPropList",
}
# PTP ops that only READ (safe to send without --force)
PTP_READONLY = {0x1001,0x1002,0x1003,0x1004,0x1005,0x1006,0x1007,0x1008,0x1009,0x100A,
                0x1014,0x1015,0x101B,0x9801,0x9802,0x9803,0x9810}
def opname(c):
    n=PTP_OPS.get(c)
    if n: return n
    if 0x9000<=c<=0x9fff: return "VENDOR_0x%04X"%c
    return "0x%04X"%c

def _find_iface(d, cls_tuple):
    for cfg in d:
        for intf in cfg:
            if (intf.bInterfaceClass,intf.bInterfaceSubClass,intf.bInterfaceProtocol)==cls_tuple:
                epin=epout=None
                for ep in intf:
                    if ep.bmAttributes&3==2:
                        if ep.bEndpointAddress&0x80: epin=ep
                        else: epout=ep
                return intf,epin,epout
    return None

def _open(args, cls_tuple, kindname):
    import usb.core
    vid=int(args.vid,0) if args.vid else NIKON_VID
    d=ufind(idVendor=vid)
    if d is None: print("Camera not found (VID 0x%04x). Pass --vid 0xXXXX."%vid); return None
    try: d.set_configuration()
    except Exception: pass
    found=_find_iface(d,cls_tuple)
    if not found:
        print("No %s interface (class %d/%d/%d). Current mode doesn't expose it - check `usb-probe`."%((kindname,)+cls_tuple)); return None
    intf,epin,epout=found
    if not (epin and epout): print("%s iface missing bulk endpoints."%kindname); return None
    return d,intf,epin,epout

# ============================== PTP transport ==============================
class PTP:
    def __init__(s,epin,epout): s.epin=epin; s.epout=epout; s.tx=0
    def cmd(s,op,params=()):
        s.tx+=1
        b=struct.pack("<IHHI",12+4*len(params),1,op,s.tx)+b"".join(struct.pack("<I",p&0xffffffff) for p in params)
        s.epout.write(b,3000)
    def _read(s,timeout=4000):
        buf=bytes(s.epin.read(s.epin.wMaxPacketSize,timeout))
        if len(buf)<12: return None
        ln,typ,code,tx=struct.unpack("<IHHI",buf[:12])
        while len(buf)<ln: buf+=bytes(s.epin.read(s.epin.wMaxPacketSize,timeout))
        return typ,code,tx,buf[12:ln]
    def transact(s,op,params=(),data_out=None):
        s.cmd(op,params)
        if data_out is not None:
            s.tx_data(op,data_out)
        data=b''; resp=None
        while True:
            r=s._read()
            if r is None: break
            typ,code,tx,payload=r
            if typ==2: data=payload            # data-in
            elif typ==3: resp=code; break       # response
            else: break
        return data,resp
    def tx_data(s,op,payload):
        hdr=struct.pack("<IHHI",12+len(payload),2,op,s.tx)
        s.epout.write(hdr+payload,5000)

def _ptp(args):
    o=_open(args,(6,1,1),"PTP"); 
    if not o: return None
    d,intf,epin,epout=o
    p=PTP(epin,epout)
    try: p.transact(0x1002,(1,))   # OpenSession
    except Exception: pass
    return p

def ptp_info(args):
    try: import usb.core
    except ImportError: print("pip install pyusb"); return 2
    p=_ptp(args)
    if not p: return 1
    data,resp=p.transact(0x1001)
    if not data: print("No DeviceInfo data (resp 0x%s)"%(resp and format(resp,'04x'))); return 1
    o=0
    std,=struct.unpack_from("<H",data,o);o+=2
    vext,=struct.unpack_from("<I",data,o);o+=4
    vver,=struct.unpack_from("<H",data,o);o+=2
    n=data[o];o+=1; vdesc=data[o:o+2*n].decode("utf-16-le","replace").rstrip("\x00");o+=2*n
    fmode,=struct.unpack_from("<H",data,o);o+=2
    def au16(o):
        k,=struct.unpack_from("<I",data,o);o+=4
        return list(struct.unpack_from("<%dH"%k,data,o)),o+2*k
    ops,o=au16(o); events,o=au16(o); props,o=au16(o)
    print("StandardVersion %d  VendorExtID 0x%08x v%d '%s'  FuncMode %d"%(std,vext,vver,vdesc,fmode))
    print("\nOperationsSupported (%d):"%len(ops))
    for c in ops: print("  0x%04X  %s%s"%(c,opname(c),"   <-- VENDOR" if 0x9000<=c<=0x9fff else ""))
    print("\nDeviceProperties (%d): %s"%(len(props),", ".join("0x%04X"%x for x in props)))
    vend=[c for c in ops if 0x9000<=c<=0x9fff]
    print("\n%d vendor opcodes -> candidates. Try reads with: ptp-send 0xNNNN --recv"%len(vend))
    return 0

def ptp_send(args):
    try: import usb.core
    except ImportError: print("pip install pyusb"); return 2
    op=int(args.opcode,0); params=[int(x,0) for x in args.params]
    data_out=None
    if args.send_file:
        data_out=open(args.send_file,"rb").read()
    if (op not in PTP_READONLY or data_out is not None) and not args.force:
        print("Op 0x%04X is not in the known read-only set%s."%(op," and you passed data" if data_out else ""))
        print("Refusing without --force (a vendor/write op can brick the camera)."); return 3
    p=_ptp(args)
    if not p: return 1
    data,resp=p.transact(op,params,data_out)
    print("Response: 0x%04X  (%s)"%(resp or 0, {0x2001:"OK",0x2002:"GeneralError",0x2005:"OperationNotSupported",0x201e:"InvalidTransactionID"}.get(resp,"?")))
    print("Data in: %d bytes"%len(data))
    if data:
        if args.out: open(args.out,"wb").write(data); print("  written to %s"%args.out)
        else:
            for i in range(0,min(len(data),args.show),16):
                row=data[i:i+16]; print("  %04x  %-47s  %s"%(i," ".join("%02x"%b for b in row),"".join(chr(b) if 32<=b<127 else "." for b in row)))
    return 0

# ============================ Mass-storage (BOT) ===========================
def bot(epin,epout,cdb,data_in_len=0,data_out=None,lun=0,tag=1):
    flags=0x80 if data_in_len>0 else 0x00
    dlen=data_in_len if data_in_len>0 else (len(data_out) if data_out else 0)
    cdb=bytes(cdb)
    cbw=struct.pack("<IIIBBB",0x43425355,tag,dlen,flags,lun,len(cdb))+cdb+b"\x00"*(16-len(cdb))
    epout.write(cbw,3000)
    data=b""
    try:
        if data_in_len>0: data=bytes(epin.read(data_in_len,4000))
        elif data_out: epout.write(data_out,4000)
    except Exception as e: data=b""; print("  (data phase: %s)"%e)
    csw=bytes(epin.read(13,4000))
    sig,ctag,residue,status=struct.unpack("<IIIB",csw[:13])
    return data,status,residue

def _msc(args):
    o=_open(args,(8,6,80),"Mass-Storage")
    if not o: return None
    return o[2],o[3]  # epin, epout

def scsi_inq(args):
    try: import usb.core
    except ImportError: print("pip install pyusb"); return 2
    m=_msc(args)
    if not m: return 1
    epin,epout=m
    data,st,res=bot(epin,epout,[0x12,0,0,0,36,0],data_in_len=36)
    print("INQUIRY status=%d"%st)
    if len(data)>=36:
        vendor=data[8:16].decode("latin1").strip(); product=data[16:32].decode("latin1").strip(); rev=data[32:36].decode("latin1").strip()
        print("  Vendor=%r Product=%r Rev=%r PDT=0x%02x"%(vendor,product,rev,data[0]&0x1f))
    cap,st2,_=bot(epin,epout,[0x25,0,0,0,0,0,0,0,0,0],data_in_len=8,tag=2)
    if len(cap)>=8:
        lba,bl=struct.unpack(">II",cap[:8]); print("  READ CAPACITY: last LBA %d, block %d"%(lba,bl))
    return 0

def scsi(args):
    try: import usb.core
    except ImportError: print("pip install pyusb"); return 2
    cdb=[int(x,16) for x in args.cdb.replace(","," ").split()]
    if args.dir=="out" and not args.force:
        print("OUT/write CDB refused without --force."); return 3
    m=_msc(args)
    if not m: return 1
    epin,epout=m
    data_out=bytes.fromhex(args.send.replace(" ","")) if args.send else None
    data,st,res=bot(epin,epout,cdb,data_in_len=(args.datalen if args.dir=="in" else 0),data_out=data_out)
    print("CDB %s -> status=%d residue=%d, %d bytes in"%(" ".join("%02x"%b for b in cdb),st,res,len(data)))
    if data:
        if args.out: open(args.out,"wb").write(data); print("  written %s"%args.out)
        else:
            for i in range(0,min(len(data),args.show),16):
                row=data[i:i+16]; print("  %04x  %-47s  %s"%(i," ".join("%02x"%b for b in row),"".join(chr(b) if 32<=b<127 else "." for b in row)))
    return 0

def scsi_scan(args):
    try: import usb.core
    except ImportError: print("pip install pyusb"); return 2
    if not args.confirm:
        print("scsi-scan sends vendor opcodes 0xC0..0xFF (IN direction, %d-byte reads)."%args.datalen)
        print("Even reads *can* have side effects on exotic firmware. Re-run with --confirm to proceed."); return 3
    m=_msc(args)
    if not m: return 1
    epin,epout=m
    hits=[]
    for op in range(0xC0,0x100):
        cdb=[op,0,0,0,0,0,0,0,(args.datalen>>8)&0xff,args.datalen&0xff] + [0]*6
        try:
            data,st,res=bot(epin,epout,cdb[:12],data_in_len=args.datalen,tag=op)
        except Exception as e:
            print("  0x%02x: usb err %s"%(op,str(e)[:30])); continue
        if st==0 and data and any(data):
            hits.append(op); print("  0x%02x: OK, %d bytes  %s"%(op,len(data)," ".join("%02x"%b for b in data[:16])))
        elif st==0:
            print("  0x%02x: OK but empty/zero"%op)
    print("\nvendor read-opcodes returning data: %s"%(", ".join("0x%02x"%h for h in hits) or "none"))
    return 0

# ============================== USB probe ==================================
def usb_probe(args):
    try: import usb.core, usb.util
    except ImportError: print("pip install pyusb"); return 2
    vid=int(args.vid,0) if args.vid else None
    devs=list(ufind(find_all=True, idVendor=vid) if vid else ufind(find_all=True))
    if not devs: print("No USB devices (try sudo / Zadig)."); return 1
    for d in devs:
        try: import usb.util; man=usb.util.get_string(d,d.iManufacturer); prod=usb.util.get_string(d,d.iProduct)
        except Exception: man=prod=None
        print("USB %04x:%04x  %s %s%s"%(d.idVendor,d.idProduct,man or "?",prod or "","   <-- NIKON" if d.idVendor==NIKON_VID else ""))
        for cfg in d:
            for intf in cfg:
                cls=(intf.bInterfaceClass,intf.bInterfaceSubClass,intf.bInterfaceProtocol)
                kind={(6,1,1):"PTP/StillImage -> use ptp-info",(8,6,80):"MassStorage -> use scsi-inq",
                      (2,2,1):"CDC-ACM (virtual serial!) -> use serial",(10,0,0):"CDC-Data",(255,255,255):"Vendor-specific!"}.get(cls,"")
                print("   iface %d  class %d/%d/%d  %s"%(intf.bInterfaceNumber,*cls,kind))
    return 0

# ================================ SERIAL ==================================
BAUDS=[115200,57600,38400,19200,9600,230400,460800,921600,74880]
def serial_term(args):
    try: import serial
    except ImportError: print("pip install pyserial"); return 2
    if args.autobaud:
        print("Auto-baud on %s (power-cycle camera now)..."%args.port)
        for b in BAUDS:
            try: s=serial.Serial(args.port,b,timeout=1.2)
            except Exception as e: print("  %d: %s"%(b,e)); continue
            data=s.read(400); s.close()
            pr=sum(1 for c in data if 9<=c<=13 or 32<=c<=126)/max(len(data),1)
            print("  %6d: %4d bytes %3.0f%% printable %s"%(b,len(data),pr*100,repr(data[:40]) if data else ""))
        return 0
    import threading,serial
    s=serial.Serial(args.port,args.baud or 115200,timeout=0.1)
    log=open(args.log,"ab") if args.log else None
    print("Connected %s @ %d. Ctrl-] quits."%(args.port,args.baud or 115200))
    stop=[False]
    def rd():
        while not stop[0]:
            dt=s.read(256)
            if dt:
                sys.stdout.buffer.write(dt); sys.stdout.buffer.flush()
                if log: log.write(dt); log.flush()
    threading.Thread(target=rd,daemon=True).start()
    try:
        import termios,tty
        fd=sys.stdin.fileno(); old=termios.tcgetattr(fd); tty.setraw(fd)
        try:
            while True:
                ch=sys.stdin.buffer.read(1)
                if ch==b'\x1d': break
                s.write(b'\r' if ch==b'\n' else ch)
        finally: termios.tcsetattr(fd,termios.TCSADRAIN,old)
    except ImportError:
        while True:
            ln=input()
            if ln=="~quit": break
            s.write(ln.encode()+b"\r")
    stop[0]=True; s.close()
    if log: log.close()
    return 0

# ================================= DFU =====================================
# DFU 1.1 requests on the DFU interface:
#   0=DETACH 1=DNLOAD 2=UPLOAD 3=GETSTATUS 4=CLRSTATUS 5=GETSTATE 6=ABORT
# bmRequestType: 0x21 host->dev (class,iface) ; 0xA1 dev->host
DFU_OUT=0x21; DFU_IN=0xA1
DFU_STATE={0:"appIDLE",1:"appDETACH",2:"dfuIDLE",3:"dfuDNLOAD-SYNC",4:"dfuDNBUSY",
 5:"dfuDNLOAD-IDLE",6:"dfuMANIFEST-SYNC",7:"dfuMANIFEST",8:"dfuMANIFEST-WAIT-RESET",
 9:"dfuUPLOAD-IDLE",10:"dfuERROR"}

def _dfu_iface(d, want=None):
    import usb.util
    for cfg in d:
        for intf in cfg:
            cls=(intf.bInterfaceClass,intf.bInterfaceSubClass)
            if want is not None and intf.bInterfaceNumber!=want: continue
            is_dfu = cls==(0xFE,0x01)
            if is_dfu or want is not None:
                # parse DFU functional descriptor from .extra if present
                func=None; extra=bytes(getattr(intf,"extra",b"") or b"")
                i=0
                while i+2<=len(extra):
                    ln=extra[i]; dt=extra[i+1]
                    if dt==0x21 and ln>=7: func=extra[i:i+ln]; break
                    i+=max(ln,1)
                return intf.bInterfaceNumber, func
    return None

def _parse_func(func):
    if not func or len(func)<7: return None
    attrs=func[2]
    wDetach=struct.unpack_from("<H",func,3)[0]
    wXfer=struct.unpack_from("<H",func,5)[0]
    bcd=struct.unpack_from("<H",func,7)[0] if len(func)>=9 else 0
    return {"CanDnload":bool(attrs&1),"CanUpload":bool(attrs&2),
            "ManifestTolerant":bool(attrs&4),"WillDetach":bool(attrs&8),
            "DetachTimeout":wDetach,"TransferSize":wXfer,"bcdDFU":bcd}

def _dfu_open(args):
    import usb.core
    vid=int(args.vid,0) if args.vid else NIKON_VID
    d=ufind(idVendor=vid)
    if d is None: print("Camera not found (VID 0x%04x)."%vid); return None
    want=int(args.intf) if getattr(args,"intf",None) is not None else None
    r=_dfu_iface(d,want)
    if not r:
        print("No DFU interface found. In Zadig it showed as 'Camera DFU Device (Interface 2)',")
        print("so try: --intf 2   (and make sure WinUSB is bound to THAT interface)."); return None
    intf,func=r
    return d,intf,_parse_func(func)

def dfu_status(d,intf):
    s=bytes(d.ctrl_transfer(DFU_IN,3,0,intf,6,3000))  # GETSTATUS
    status=s[0]; poll=s[1]|(s[2]<<8)|(s[3]<<16); state=s[4]
    return status,state,poll

def dfu_info(args):
    try: import usb.core
    except ImportError: print("pip install pyusb"); return 2
    o=_dfu_open(args)
    if not o: return 1
    d,intf,f=o
    print("DFU interface #%d"%intf)
    if f: print("  CanUpload=%s CanDnload=%s TransferSize=%d bcdDFU=0x%04x WillDetach=%s ManifestTolerant=%s"%(
        f["CanUpload"],f["CanDnload"],f["TransferSize"],f["bcdDFU"],f["WillDetach"],f["ManifestTolerant"]))
    else: print("  (no DFU functional descriptor exposed; may be vendor-DFU - upload still worth trying)")
    try:
        st,state,poll=dfu_status(d,intf)
        print("  status=%d state=%d (%s) poll=%dms"%(st,state,DFU_STATE.get(state,"?"),poll))
    except Exception as e: print("  GETSTATUS failed: %s"%e)
    print("\nNext: dfu-upload --out p520_dfu.bin   (reads firmware/memory OUT - safe, read-only)")
    return 0

def dfu_upload(args):
    try: import usb.core
    except ImportError: print("pip install pyusb"); return 2
    o=_dfu_open(args)
    if not o: return 1
    d,intf,f=o
    xfer=args.xfer or (f["TransferSize"] if f and f["TransferSize"] else 1024)
    if f and not f["CanUpload"] and not args.force:
        print("DFU descriptor says CanUpload=0. Re-run with --force to try anyway (some vendor")
        print("DFU still answer UPLOAD), or we use dfu-dnload for patch-flashing instead."); return 3
    # reset to idle
    try: d.ctrl_transfer(DFU_OUT,6,0,intf,None,2000)  # ABORT
    except Exception: pass
    out=open(args.out,"wb")
    total=0; blk=0
    print("Uploading (xfer=%d) to %s ... Ctrl-C to stop."%(xfer,args.out))
    try:
        while True:
            chunk=bytes(d.ctrl_transfer(DFU_IN,2,blk,intf,xfer,8000))  # UPLOAD
            out.write(chunk); total+=len(chunk); blk+=1
            if blk%64==0: print("  %d bytes (block %d)"%(total,blk))
            if len(chunk)<xfer:
                print("  short block (%d<%d) -> end of upload"%(len(chunk),xfer)); break
            if args.max and total>=args.max:
                print("  hit --max %d"%args.max); break
    except KeyboardInterrupt:
        print("  stopped by user")
    except Exception as e:
        print("  upload ended: %s"%e)
    out.close()
    print("Saved %d bytes to %s"%(total,args.out))
    if total: print("Send me the first bytes / the file and I'll ID it against the 72k-symbol map.")
    return 0

def dfu_dnload(args):
    try: import usb.core
    except ImportError: print("pip install pyusb"); return 2
    if not args.force:
        print("dfu-dnload WRITES firmware to the camera - real brick risk.")
        print("This is the patch-deployment path, but only use it once we understand the image")
        print("format + MD5. Re-run with --force when you're deliberately flashing."); return 3
    o=_dfu_open(args)
    if not o: return 1
    d,intf,f=o
    data=open(args.file,"rb").read()
    xfer=args.xfer or (f["TransferSize"] if f and f["TransferSize"] else 1024)
    try: d.ctrl_transfer(DFU_OUT,6,0,intf,None,2000)
    except Exception: pass
    blk=0; off=0
    while off<len(data):
        piece=data[off:off+xfer]
        d.ctrl_transfer(DFU_OUT,1,blk,intf,piece,8000)  # DNLOAD
        # wait for dnbusy->idle
        while True:
            st,state,poll=dfu_status(d,intf)
            import time; time.sleep(poll/1000.0)
            if state in (5,2): break
            if state==10: print("DFU ERROR at block %d"%blk); return 1
        off+=len(piece); blk+=1
        if blk%32==0: print("  %d/%d bytes"%(off,len(data)))
    d.ctrl_transfer(DFU_OUT,1,blk,intf,b"",8000)  # zero-length = manifest
    print("Download complete (%d bytes). GETSTATUS to confirm manifestation."%len(data))
    return 0

def main():
    ap=argparse.ArgumentParser(description=__doc__,formatter_class=argparse.RawDescriptionHelpFormatter)
    sub=ap.add_subparsers(dest="cmd",required=True)
    def add(name,fn,**kw):
        p=sub.add_parser(name,**kw); p.set_defaults(fn=fn); p.add_argument("--vid"); return p
    add("usb-probe",usb_probe)
    add("ptp-info",ptp_info)
    p=add("ptp-send",ptp_send); p.add_argument("opcode"); p.add_argument("params",nargs="*"); p.add_argument("--recv",action="store_true"); p.add_argument("--send-file"); p.add_argument("--out"); p.add_argument("--show",type=int,default=256); p.add_argument("--force",action="store_true")
    add("scsi-inq",scsi_inq)
    p=add("scsi",scsi); p.add_argument("--cdb",required=True,help='hex bytes, e.g. "C0 00 00 00 00 00 00 00 00 40"'); p.add_argument("--dir",choices=["in","out"],default="in"); p.add_argument("--datalen",type=int,default=64); p.add_argument("--send"); p.add_argument("--out"); p.add_argument("--show",type=int,default=256); p.add_argument("--force",action="store_true"); p.set_defaults(scan=False)
    p=add("scsi-scan",scsi_scan); p.add_argument("--datalen",type=int,default=64); p.add_argument("--confirm",action="store_true")
    p=add("dfu-info",dfu_info); p.add_argument("--intf")
    p=add("dfu-upload",dfu_upload); p.add_argument("--intf"); p.add_argument("--out",default="p520_dfu.bin"); p.add_argument("--xfer",type=int); p.add_argument("--max",type=int); p.add_argument("--force",action="store_true")
    p=add("dfu-dnload",dfu_dnload); p.add_argument("file"); p.add_argument("--intf"); p.add_argument("--xfer",type=int); p.add_argument("--force",action="store_true")
    p=add("serial",serial_term); p.add_argument("port"); p.add_argument("--baud",type=int); p.add_argument("--autobaud",action="store_true"); p.add_argument("--log")
    a=ap.parse_args()
    try:
        sys.exit(a.fn(a))
    except Exception as e:
        # friendly hint for the common Windows "can't claim" case
        msg=str(e)
        if "Access denied" in msg or "Errno 13" in msg or "not found" in msg.lower() and "backend" not in msg.lower():
            print("\n[!] Access denied claiming the interface.")
            print("    Windows' built-in driver owns it; libusb can't claim it yet.")
            print("    Fix: run Zadig (https://zadig.akeo.ie), Options->List All Devices,")
            print("    pick the Nikon camera interface this command uses (PTP=6/1/1,")
            print("    MassStorage=8/6/80), set driver to WinUSB, Replace Driver, then re-run.")
            print("    Reversible: Device Manager -> device -> Uninstall -> replug.")
            raise SystemExit(13)
        raise

if __name__=="__main__": main()
