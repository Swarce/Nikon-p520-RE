#!/usr/bin/env python3
"""P520 soft-brick recovery helper — UART boot-log monitor + Zburn burn-protocol client.

Context: a bad main-app firmware makes the camera bootloop. The bootloader/boot-ROM are
intact, but all reflash routes (card / USB / UART) go through the SoC's *burn mode*, which
is selected by a hardware boot-strap/test-point at power-on (NOT a button or software flag).
Once in burn mode the camera exposes:
  * USB: Zoran device  VID 0x0595  PID 0x6161 , two bulk endpoints EP2(OUT)/EP3(IN), 64-byte packets
  * UART: 115200 8N1 on the board's debug pads
  * Card: a *.ELF burn file (0x55aa header + trailing MD5) in the card root

This script has two jobs:
  monitor   — passively read the UART boot log at 115200 so you can SEE the crash / any prompt
              (works as soon as you solder RX/TX/GND, even without burn mode)
  usb-info  — detect whether the camera is in USB burn mode (0595:6161)
  uart-burn / usb-burn — EXPERIMENTAL Zburn client (framing reversed from zburn.elf; NOT
              hardware-validated — see WARNING). Use only once you are in burn mode.

Requires: pyserial (monitor/uart), pyusb (usb).  python3 -m pip install pyserial pyusb
"""
import sys, time, struct

BAUD = 115200
USB_VID, USB_PID = 0x0595, 0x6161   # Zoran COACH burn mode (from DownloadUsb in zburn.elf)

# ---------------------------------------------------------------- UART boot-log monitor
def monitor(port):
    import serial
    s = serial.Serial(port, BAUD, timeout=0.2)
    print("[monitor] %s @ %d 8N1 — power the camera on now; Ctrl-C to stop" % (port, BAUD))
    try:
        while True:
            data = s.read(4096)
            if data:
                sys.stdout.buffer.write(data); sys.stdout.flush()
    except KeyboardInterrupt:
        s.close()

# ---------------------------------------------------------------- USB burn-mode detect
def usb_info():
    import usb.core
    dev = usb.core.find(idVendor=USB_VID, idProduct=USB_PID)
    if dev is None:
        print("No %04x:%04x device. The camera is NOT in USB burn mode." % (USB_VID, USB_PID))
        print("(In burn mode it enumerates as Zoran 0595:6161. If you see nothing, burn mode")
        print(" is not entered — that needs the board boot-strap/test-point at power-on.)")
        return
    print("FOUND camera in USB burn mode: %04x:%04x" % (USB_VID, USB_PID))
    for cfg in dev:
        for intf in cfg:
            print("  interface", intf.bInterfaceNumber, "endpoints:",
                  [hex(e.bEndpointAddress) for e in intf])

# ---------------------------------------------------------------- Zburn client framing
# Reversed from zburn.elf UartClientCommand/Read/Write + DownloadUart/DownloadUsb.
# The camera is the "client": the PC drives it. Command packets are 4 bytes; the camera
# acks with 'T' (0x54). 0xF0 = write-flash-burst (then a 0x40-byte burn header whose off
# 0x3e must be 0x55aa, then section data in bursts). THIS FRAMING IS BEST-EFFORT FROM
# STATIC RE AND IS NOT VERIFIED ON HARDWARE — expect to adjust against a real boot log.
CMD_WRITE_FLASH_BURST = 0xF0
ACK = b'T'

def _uart_cmd(s, cmd, arg=0):
    s.write(bytes([0x01, 0x00, cmd & 0xff, arg & 0xff]))
    r = s.read(2)
    return r[:1] == ACK

def uart_burn(port, burn_file):
    """EXPERIMENTAL. burn_file must be a *burn-format* image (0x55aa header), NOT the SD
    firmware.bin update package. Producing that image is an open item (see docs/RECOVERY.md)."""
    import serial
    print("WARNING: experimental, unverified. Confirm the boot log shows a burn prompt first.")
    data = open(burn_file, 'rb').read()
    if data[0x3e:0x40] != b'\xaa\x55':
        print("note: byte 0x3e..0x3f is not 0x55aa — this may not be a valid burn image.")
    s = serial.Serial(port, BAUD, timeout=1)
    if not _uart_cmd(s, CMD_WRITE_FLASH_BURST):
        print("no ACK to write-flash-burst; camera not in UART burn mode / framing differs.")
        return
    # 1KB bursts, each chunk acked 'T' (see UartClientWrite). Tune on hardware.
    off = 0
    while off < len(data):
        chunk = data[off:off+1024]
        s.write(bytes([0x02, (len(chunk)+1)//2])); s.write(chunk)
        s.read(2)
        off += len(chunk)
    s.write(bytes([0x04, 0])); s.read(2)
    print("sent %d bytes (unverified)." % len(data))

def usb_burn(burn_file):
    print("EXPERIMENTAL. See docs/RECOVERY.md. Implement EP2/EP3 bulk framing mirroring uart_burn.")
    print("Open item: the exact burn-image format + whether the SD firmware.bin can be used directly.")

if __name__ == '__main__':
    a = sys.argv
    if len(a) >= 3 and a[1] == 'monitor':
        monitor(a[2])
    elif len(a) == 2 and a[1] == 'usb-info':
        usb_info()
    elif len(a) >= 4 and a[1] == 'uart-burn':
        uart_burn(a[2], a[3])
    elif len(a) >= 3 and a[1] == 'usb-burn':
        usb_burn(a[2])
    else:
        print(__doc__)
