# P520 RAW (NRW): where it is gated and a candidate patch (2026-10-22)

With the main app decompressed (see `CODEC_SOLVED.md`), the RAW gate turns out to be a **UI menu
link**. Capture, settings and storage all handle NRW. The P520 build just never offers it.

## What is already in the firmware

* Quality setting **0xc29** (settings DB `fINF_*ModeValue*`). Values from
  `pstSTO_QUALITY_MAKERNOTE`/`pstSTO_QUALITY_EXIF`:
  `1 FINE, 2 NORMAL, 3 BASIC, 0 EXTRA, 5 NRW+FINE, 6 NRW+NORMAL, 7 NRW`.
* `fINF_CompressRatioMultiSet(q)` (c0577d3c) accepts 5/6/7. It only converts RAW→JPEG via
  `fINF_ReplaceRawRatioToJpegRatio` (5→1, 6→2, 7→1) in certain shooting modes
  (mode 0xee in bitmask 0x83000040, or drive 0xc43 in 0x608). For q=7 it forces image size
  0x54 ('T') and skips `I43_ConfigCapture(0x3c, …)`.
* `fINF_ChangeActiveValueByFn1Down/Up`: P7700-style "Fn1 + shutter = RAW" (setting 0xbff == 1)
  switches to quality 7 / size 0x48 for the shot.
* Storage: `%s\DSCN%04d.NRW`, `.NRW` extension tables, RAW+JPEG DCF pairing, `fSTO_*Raw*`,
  in-camera RAW development (`fPLY_RAWDEV_*`), PTP NRW object format.

## The gate

The Fn-button menu on the P520 is `gUI_PopupItem_FN_SETTING_Q11095_Main_0` / `..._Q11105_..`
(and their `_OFFSET_SCROLL` variants). Item 0 ("Image quality") is a sub-page link
(`type 0x1c`, page id at item+0x10):

| page id | popup | items (value of setting 0xc29) |
|---|---|---|
| 0x32 | `FN_IMAGE_COMPRESSRATIO_RAW` | Fine 1, Normal 2, NRW+Fine 5, NRW+Normal 6, NRW 7 |
| 0x33 | `FN_IMAGE_COMPRESSRATIO_WITHOUT_RAW` | Fine 1, Normal 2 |

All four P520 Fn menus link to **0x33**. The complete RAW popup (0x32) ships but is never
referenced. Same pattern for image size: `FN_IMAGE_MODE_12M` (0x34, P7700-class 12 MP) vs
`FN_IMAGE_MODE_18M` (0x35, used by P520). The main Setup-menu `IMAGE_COMPRESSRATIO` page
defines only Fine/Normal.

No code special-cases page 0x32/0x33. The popup engine is table-driven (`fUIFrame_DisplayFnPopupMenu`,
`fUI_Popup_GetPopupPage`, page table `gUI_PopupPage`, entries `{title, main, id, 1, 0}`).

## Candidate patch (UNTESTED ON HARDWARE)

Point the four Fn-menu quality items at the RAW popup:

    python3 tools/p520fw.py patch firmware.bin firmware_raw.bin \
        0xc0dadebc=32 0xc0dae734=32 0xc0daefac=32 0xc0daf8e0=32

Result on v1.1 (sha256 of input `631a4c6a…979c`): one block re-encoded in place
(file 0xa22ad8, same tables, same clen), 4 RAM bytes changed, byte-sum 0x0390dae0, output
sha256 `a76d98a6eff887c97e3e5968993c186b81c62623c8a6f3a86089a3143da45b39`.
Verified by re-decoding the whole image.

### Risks / what is not known

* Whether the P520's sensor/IPU path produces a valid NRW (the code is shared with RAW models;
  P520-specific RAW parameters may be missing). Possible outcomes range from working NRW files
  to errors, a hang on capture, or corrupt files.
* The quality setting is persistent. If selecting NRW breaks something at power-on, you need
  a working firmware-update menu (or "Reset all") to recover. Test with a spare card, shoot in
  P/S/A/M (scene/auto modes convert RAW back to JPEG by design), and try NRW+Fine first.
* Flashing modified firmware can brick the camera. The update path itself only checks the byte-sum.

## Other leads (not patched)

* `fINF_ChangeActiveValueByFn1*` (Fn1+shutter RAW) needs setting 0xbff = 1; there is no
  P520 menu for it.
* Hidden factory OPT menu: `OPT_SET_STILL_RAW_REPLACE`, `OPT_SET_WRITE_RAW_PWB`,
  `OPT_SET_RVS_MOVIE_RAW` (debug RAW dumps).
