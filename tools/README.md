# p520tool — USB/PTP/SCSI prober + UART terminal (Nikon COOLPIX P520)

Runs on the computer the camera is plugged into.

## Windows setup (fixes "No backend available")
    pip install pyusb pyserial libusb-package
`libusb-package` bundles libusb-1.0.dll, so `usb-probe` works right away:
    python3 p520tool.py usb-probe
That only *lists* devices — enough to see the camera's VID:PID and interface class.

To actually TALK to the camera (ptp-*/scsi-*), Windows must let a libusb driver
claim the interface. Use **Zadig** (https://zadig.akeo.ie):
  1. Zadig → Options → "List All Devices".
  2. Pick the Nikon camera (or its interface) in the dropdown.
  3. Driver target: **WinUSB** (or libusb-win32). Click "Replace Driver".
  - This detaches the normal MTP/WPD driver while bound. It's reversible:
    Device Manager → the device → Uninstall device → unplug/replug (or
    "Scan for hardware changes") restores Windows' default driver.
  - Do Zadig on the specific interface `usb-probe` flagged (PTP 6/1/1 or
    Mass-Storage 8/6/80), not a random one.

Linux/macOS: `pip install pyusb pyserial libusb-package`; run with sudo or add a
udev rule. macOS may also need `brew install libusb`.

## Step 1 — which mode is the camera in
    python3 p520tool.py usb-probe
  6/1/1  PTP/StillImage      -> PTP section
  8/6/80 Mass-Storage        -> SCSI section
  2/2/1  CDC-ACM             -> UART-over-USB! use `serial` on the new COMx
  255/255/255 Vendor-specific-> tell me, that's a custom channel

## PTP path (class 6/1/1)   [needs Zadig→WinUSB on that interface]
    python3 p520tool.py ptp-info
    python3 p520tool.py ptp-send 0x90XX --recv --out dump.bin
Standard reads send freely; vendor ops / data-out need --force.

## SCSI path (class 8/6/80)  [needs Zadig→WinUSB on that interface]
    python3 p520tool.py scsi-inq
    python3 p520tool.py scsi-scan --confirm
    python3 p520tool.py scsi --cdb "C0 00 00 00 00 00 00 00 00 40" --datalen 64 --out out.bin
`scsi` defaults to a read; writes/--dir out need --force.

## Serial path (USB-UART adapter, 3.3V)
    python3 p520tool.py serial COM5 --autobaud
    python3 p520tool.py serial COM5 --baud 115200 --log boot.txt

## Safety
Read-biased. No OUT transfer or undocumented opcode without --force (and --confirm
for scsi-scan). Keep a copy of anything you read before writing.
