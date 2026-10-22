# No-UART RAM-dump lead from the "ops" tool (2026-10-19)

## Are the two uploads useful?
- **OneTimeBrute 1.4 — NO.** It brute-forces the RC2-40 challenge/response that unlocks a
  CVS/Pure-Digital *one-time-use* camcorder. The Nikon P520 has no such lock. Irrelevant
  (same conclusion as the earlier evaluation).
- **ops-for-linux — not runnable on the P520, but it hands us the key TECHNIQUE.** It's a
  tool for the CVS "Saturn" camcorder (Zoran COACH SoC, USB VID 0x167b). The P520 is a Nikon
  PTP device (VID 0x04B0) with a different USB stack, so ops won't talk to it directly. BUT
  its source documents something we can try to reuse.

## The technique ops reveals (download_memory.c + send_monitor_command.c)
COACH silicon exposes its **debug MONITOR over USB**, and the monitor can dump memory:
- `Monitor(cmd)` = a USB vendor CONTROL transfer: `bRequest=0x01, wValue=0xEF00, wIndex=0x0101`,
  data = an ASCII monitor command string.
- Memory dump = monitor command **`dumpf <start> <len> <tmpfile>`**, then the temp file is
  read back over USB (SetFileName 0xB101 -> RequestRead 0x9301 -> bulk-read).
- Also raw reads via `0xF100|partition` (set transfer filter/size) + `0xF300` (initiate).
=> On a COACH camcorder you can RAM/flash-dump over USB with no UART.

## Why this matters for the P520 (the pieces line up)
The P520 firmware contains the SAME architecture:
- Full monitor command parser: `CMonitorCmdParser` with `HandleCommandStackDump`,
  `HandleCommandGetMemUsage`, `EvalMathExpr`, `HandleCommandScript`, `ListMonCmd`,
  `GetCliVersion`, plus `MonitorParseAndExec`, `MonitorMain`, `DebugConsoleMain`, `MonitorDumpAll`.
- USB vendor-command infrastructure: `UsbVendorCommandThread`, `UsbCmdDispatcher`,
  `HcePtpVendor`, `PtpVendorEPControlListener`, `HceUsbMSCustomAction`.
- Vendor PTP opcodes seen on the live cam earlier: 0x90A1 0x90A2 0x90A3, 0x9805.
So a USB->monitor bridge analogous to ops' 0xEF00 is plausible on the P520.

## Honest status of the lead
- CONFIRMED: P520 has a monitor parser AND USB vendor channels (symbols).
- CONFIRMED: P520 resident `DebugConsoleMain` reads input from **UART only** (not the win).
- UNCONFIRMED: that any USB vendor opcode feeds strings to `MonitorParseAndExec`. That bridge
  code sits in the COMPRESSED main app, so it can't be read statically (needs the codec - the
  very thing we're trying to dump our way into). It must be tested LIVE on the camera.
- RISK: blindly sending vendor opcodes with data phases can write/erase/brick. Probe
  read-biased, benign commands first, on a camera you can afford to lose.

## Procedure (needs the camera + USB, NO soldering)
Tool: `p520_usb_monitor.py` (read-biased; nothing sent without --go).
1. `python3 p520_usb_monitor.py info`  -> list vendor opcodes the cam advertises.
2. `python3 p520_usb_monitor.py probe-ctrl --cmd ver --go`
     try the ops-style control transfer (wValue 0xEF00 + neighbors). Watch for a TEXT reply.
3. `python3 p520_usb_monitor.py probe-ptp 0x90A1 0x90A2 0x90A3 0x9805 --cmd ver --go`
     send "ver" as the data phase of each vendor opcode; a channel that echoes ASCII
     (version/help/prompt text) IS the monitor bridge.
4. Once a channel replies to text, find the memory-read command syntax by trying benign
     monitor words: `ver`, `help`, `?`, `lm`/`listcmd` (list commands), `stackdump`,
     `memusage`. The command list it prints reveals the memory-dump verb.
5. Dump segment s1 block0's output region (va 0x8014e900, 64 KiB is plenty):
     `python3 p520_usb_monitor.py dump --chan ptp:0xXXXX --addr 0x8014e900 --len 65536 \
        --memcmd "<the real dump cmd> 0x8014e900 65536" --out s1_blk0.bin --go --danger`
   (edit --memcmd to the syntax discovered in step 4.)

## What a single 64 KiB dump buys us
That region is the decompressed output of P520 segment-1 block 0 (whose compressed bytes +
A/B tables we already have). Feeding it to the search harness turns the last unknowns
(the 0x2000 opcode-class, exact bit walk, distance scheme) into a solved constraint -> the
whole HW-inflate codec falls, which (with integrity already solved) completes the RAW patch
pipeline. One successful dump = done.
