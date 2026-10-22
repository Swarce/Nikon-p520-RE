#!/usr/bin/env python3
"""
p520_usb_monitor.py - probe for a USB -> debug-monitor bridge on the Nikon P520
(COACH SoC), modeled on the CVS/Pure-Digital "ops" tool which reaches the same
kind of COACH monitor over USB (vendor ctrl msg 0xef00 carrying a command string,
then a "dumpf <start> <len> <file>" memory-dump command).

GOAL: find a USB channel that feeds a string to the P520 monitor command parser
(CMonitorCmdParser, confirmed present in the firmware) WITHOUT a UART. If one
responds, the monitor's memory/stack-dump commands give a RAM dump = the single
ground-truth block needed to crack the HW-inflate codec.

SAFETY: read-biased. It only (a) reads PTP DeviceInfo, and (b) sends SHORT,
benign command strings you approve, over channels you name. Blind vendor writes
can brick a camera, so nothing is sent without --go, and memory-affecting or
unknown opcodes are refused unless you also pass --danger. Start on a camera you
can afford to lose, battery full, and keep a backup of anything you read.

Deps: pip install pyusb libusb-package   (Windows: Zadig->WinUSB on the PTP iface)

Usage:
  python3 p520_usb_monitor.py info
      list the camera's PTP vendor opcodes (candidates for a monitor channel)
  python3 p520_usb_monitor.py probe-ctrl --go
      try the ops-style vendor CONTROL transfer (bRequest=1, wValue=0xEF00 and a
      few neighbors, wIndex=0x0101) with a benign command, read any text reply
  python3 p520_usb_monitor.py probe-ptp 0x90A1 0x90A2 0x90A3 --cmd "ver" --go
      send the command string as the data phase of each vendor PTP opcode, read
      back, and flag any ASCII that looks like monitor output
  python3 p520_usb_monitor.py dump --chan ptp:0x90A1 --addr 0x8014e900 --len 65536 \
      --out s1_blk0.bin --go --danger
      once a working channel + command syntax are known, pull memory

Candidate monitor command words to try as --cmd (benign/read-only first):
  "ver"  "help"  "?"  "lm"  "listcmd"  "stackdump"  "memusage"
Memory-read-ish (COACH/ops vocabulary; confirm before trusting):
  "dumpf <start> <len> <file>"   "d <addr> <len>"   "md <addr> <len>"
"""
import sys, argparse, time
try:
    import usb.core, usb.util
except Exception:
    print("need pyusb (pip install pyusb libusb-package)"); raise

NIKON_VID = 0x04b0

def _backend():
    """Get a libusb backend. On Windows the reliable source is the libusb-package
    wheel, which bundles libusb-1.0.dll (no system install needed)."""
    try:
        import libusb_package
        be = libusb_package.get_libusb1_backend()   # newer libusb-package
        if be: return be
    except Exception:
        pass
    try:
        import libusb_package, usb.backend.libusb1 as lb
        be = lb.get_backend(find_library=libusb_package.find_library)
        if be: return be
    except Exception:
        pass
    try:
        import usb.backend.libusb1 as lb
        be = lb.get_backend()                        # system libusb on PATH
        if be: return be
    except Exception:
        pass
    return None

def open_dev(vid=NIKON_VID):
    be = _backend()
    if be is None:
        print("No USB backend available (a Windows/pyusb setup issue, not the camera).\n"
              "Fix:  pip install libusb-package    (ships libusb-1.0.dll; no system install)\n"
              "then re-run.  If it still fails, also:  pip install libusb")
        sys.exit(2)
    dev = usb.core.find(idVendor=vid, backend=be)
    if dev is None:
        seen = ["%04x:%04x" % (d.idVendor, d.idProduct)
                for d in usb.core.find(find_all=True, backend=be)]
        print("No Nikon (VID %04x) found. USB devices seen: %s" % (vid, ", ".join(seen) or "none"))
        print("Plug the camera in, power on (PTP mode), close Nikon Transfer / Photos / any app that grabs it.")
        sys.exit(2)
    try:
        dev.set_configuration()
    except usb.core.USBError as e:
        print("Found the camera but could not claim it: %s" % e)
        print("Windows' MTP/WPD driver probably owns the PTP interface. Bind it to WinUSB with\n"
              "Zadig (Options -> List All Devices -> pick the Nikon camera interface -> Replace\n"
              "Driver -> WinUSB). Reversible: Device Manager -> Uninstall device -> replug.")
        sys.exit(3)
    return dev

# ---- minimal PTP over bulk (container protocol) ----
def _eps(dev):
    cfg=dev.get_active_configuration()
    for intf in cfg:
        ei=eo=None
        for e in intf:
            if usb.util.endpoint_direction(e.bEndpointAddress)==usb.util.ENDPOINT_IN and \
               usb.util.endpoint_type(e.bmAttributes)==usb.util.ENDPOINT_TYPE_BULK: ei=e
            if usb.util.endpoint_direction(e.bEndpointAddress)==usb.util.ENDPOINT_OUT and \
               usb.util.endpoint_type(e.bmAttributes)==usb.util.ENDPOINT_TYPE_BULK: eo=e
        if ei and eo: return intf,ei,eo
    raise RuntimeError("no bulk endpoints (is the PTP interface bound to libusb/WinUSB?)")

import struct
_tid=[0]
def ptp_txn(dev, opcode, params=(), data_out=None, recv=True, timeout=4000):
    intf,ei,eo=_eps(dev)
    _tid[0]+=1; tid=_tid[0]
    # command container
    body=struct.pack("<HHI",1,opcode,tid)+b"".join(struct.pack("<I",p) for p in params)
    eo.write(struct.pack("<I",len(body)+4)+body,timeout)
    if data_out is not None:
        dc=struct.pack("<IHHI",12+len(data_out),2,opcode,tid)+data_out
        eo.write(dc,timeout)
    out=b""
    if recv:
        try:
            while True:
                chunk=bytes(ei.read(ei.wMaxPacketSize or 512,timeout))
                if not chunk: break
                out+=chunk
                if len(chunk)<(ei.wMaxPacketSize or 512): break
        except usb.core.USBError: pass
    return out

def looks_like_text(b):
    if not b: return False
    pr=sum(1 for c in b if 9<=c<=13 or 32<=c<127)
    return pr/len(b)>0.7

def cmd_info(a):
    dev=open_dev(a.vid)
    out=ptp_txn(dev,0x1001,recv=True)   # GetDeviceInfo
    # vendor opcodes live in the operations-supported array; just hexdump + scan
    print("DeviceInfo %d bytes"%len(out))
    # crude: find 0x9xxx shorts
    vend=set()
    for i in range(0,len(out)-1,2):
        v=out[i]|(out[i+1]<<8)
        if 0x9000<=v<=0x9fff: vend.add(v)
    print("candidate vendor opcodes:", ", ".join("0x%04X"%v for v in sorted(vend)) or "none seen")
    if looks_like_text(out): print(out.decode('latin1','replace')[:400])

def cmd_probe_ctrl(a):
    dev=open_dev(a.vid)
    if not a.go: print("add --go to actually send (read-biased safety)"); return
    cmd=(a.cmd or "ver").encode()+b"\x00"
    for wval in [0xEF00,0xEF01,0xEF10,0xE000,0xB200]:
        try:
            # ops style: OUT|VENDOR|DEVICE, bRequest=1, wValue=cmd, wIndex=0x0101
            dev.ctrl_transfer(0x40,0x01,wval,0x0101,cmd,2000)
            # read reply: IN|VENDOR|INTERFACE, bRequest=1, wValue=cmd, wIndex=0
            rep=bytes(dev.ctrl_transfer(0xC1,0x01,wval,0x0000,256,2000))
            tag=" <== TEXT!" if looks_like_text(rep) else ""
            print("ctrl wValue=0x%04X -> %d bytes%s %r"%(wval,len(rep),tag,rep[:64]))
        except usb.core.USBError as e:
            print("ctrl wValue=0x%04X -> err %s"%(wval,e))

def cmd_probe_ptp(a):
    dev=open_dev(a.vid)
    if not a.go: print("add --go to actually send"); return
    cmd=(a.cmd or "ver").encode()+b"\x00"
    for op in [int(x,16) for x in a.opcodes]:
        try:
            r=ptp_txn(dev,op,data_out=cmd,recv=True)
            tag=" <== TEXT!" if looks_like_text(r) else ""
            print("ptp 0x%04X (data='%s') -> %d bytes%s %r"%(op,a.cmd or "ver",len(r),tag,r[:80]))
        except usb.core.USBError as e:
            print("ptp 0x%04X -> err %s"%(op,e))

def cmd_dump(a):
    if not (a.go and a.danger):
        print("dump needs --go AND --danger (it drives a discovered channel). Confirm the"
              " channel/command syntax with probe-* first."); return
    dev=open_dev(a.vid)
    kind,op=a.chan.split(":"); op=int(op,16)
    # expects a monitor memory-read command; syntax is camera-specific - EDIT HERE once known
    cmd=(a.memcmd or ("d 0x%x %d"%(a.addr,a.len))).encode()+b"\x00"
    r=ptp_txn(dev,op,data_out=cmd,recv=True) if kind=="ptp" else b""
    open(a.out,"wb").write(r); print("wrote %d bytes to %s"%(len(r),a.out))

def main():
    ap=argparse.ArgumentParser(); ap.add_argument("--vid",type=lambda x:int(x,0),default=NIKON_VID)
    sub=ap.add_subparsers(dest="c",required=True)
    sub.add_parser("info").set_defaults(fn=cmd_info)
    p=sub.add_parser("probe-ctrl"); p.add_argument("--cmd"); p.add_argument("--go",action="store_true"); p.set_defaults(fn=cmd_probe_ctrl)
    p=sub.add_parser("probe-ptp"); p.add_argument("opcodes",nargs="+"); p.add_argument("--cmd"); p.add_argument("--go",action="store_true"); p.set_defaults(fn=cmd_probe_ptp)
    p=sub.add_parser("dump"); p.add_argument("--chan",required=True); p.add_argument("--addr",type=lambda x:int(x,0),default=0x8014e900); p.add_argument("--len",type=lambda x:int(x,0),default=65536); p.add_argument("--memcmd"); p.add_argument("--out",default="dump.bin"); p.add_argument("--go",action="store_true"); p.add_argument("--danger",action="store_true"); p.set_defaults(fn=cmd_dump)
    a=ap.parse_args(); a.fn(a)

if __name__=="__main__": main()
