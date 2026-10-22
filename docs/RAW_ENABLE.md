# Enabling NRW (RAW) on the P520 — WORKING (2026-10-22)

Confirmed on hardware: the patched firmware installs, the camera shoots NRW, and the
resulting `.NRW` is a valid 4912×3680 12/16-bit Bayer raw (sample `DSCN0119.NRW` decodes
in LibRaw after `tools/nrwfix.py`). This supersedes the menu-link guess in `RAW_GATE.md`.

## The two gates (both required — neither alone is enough)

The P520 ships the whole RAW/NRW stack but gates it in two places:

1. **The menu never stores a RAW quality value.** The shooting *Image quality* menu
   (`gUI_MenuMain_IMAGE_COMPRESSRATIO` @ `c0ebf1a0`) is a static 2-item list that writes
   quality (settings slot `0xc29`) = 1 (FINE) or 2 (NORMAL). The 5-option RAW list exists
   only in an unused Fn popup (`FN_IMAGE_COMPRESSRATIO_RAW`, page 0x32). The P520 has no Fn
   button, so repointing that popup does nothing — that was the first failed attempt.
   Quality values: `1 FINE, 2 NORMAL, 3 BASIC, 0 EXTRA, 5 NRW+FINE, 6 NRW+NORMAL, 7 NRW`.

2. **A conversion function strips RAW back to JPEG.** `fINF_ReplaceRawRatioToJpegRatio`
   @ `c057631c` maps 5→1, 6→2, 7→1. It is called from `fINF_PhotoModeMultiSet`,
   `fINF_CompressRatioMultiSet`, and (unconditionally) `fINF_CopyActiveValueFromBeforeRecMode`
   @ `c0582768`, which re-runs on every mode/scene change. So even a RAW value gets
   rewritten to JPEG before capture.

The capture pipeline itself is intact: quality 5/6/7 → `fSTL_Lib_SetRecodeType` @ `c0550b08`
→ `AppConfigCapture(1, 0x200)` → `CaptureOnImageCompress` RAW branch → file type `0x200` =
`.NRW` (`%s\DSCN%04d.NRW`). RAW engages for **single-shot ordinary still modes only**
(RecKind 1–5); not in continuous/bracket/movie/most scene modes, by design.

## The patch (2 edits), applied with `tools/p520fw.py`

```
python3 tools/p520fw.py patch firmware.bin firmware_nrw.bin \
    0xc0ebf2f8=05000000 \
    0xc057634c=21108000
```

| VA | old | new | segment | effect |
|---|---|---|---|---|
| `0xc0ebf2f8` | `02000000` | `05000000` | c09a9500 (compressed) | Image-quality menu row 2 now stores quality **5 = NRW+Fine** (was 2 = NORMAL). Use `07000000` for pure NRW (no JPEG). |
| `0xc057634c` | `21106000` | `21108000` | c0503c80 (compressed) | Neuters `fINF_ReplaceRawRatioToJpegRatio`: its `jr $ra` delay slot becomes `move $v0,$a0` (return input unchanged), so a RAW quality survives to capture. |

Both VAs are in compressed main-app segments, so `p520fw.py` recompresses the affected
blocks in place and recomputes the byte-sum. Verified: exactly 2 bytes differ in the
decompressed image; all 646 blocks re-decode; byte-sum valid.

Result of the v1.1 build: input sha256 `631a4c6a…979c` → `firmware_nrw.bin`
sha256 `a666efdd…3f16`, byte-sum `0x039126d4`.

## In-camera use

1. Mode dial to **P** or **Auto**, single-shot drive.
2. MENU → Shooting menu → **Image quality** → **second row**. It still reads/icons as
   "Normal" (cosmetic — see below) but stores NRW+Fine.
3. Shoot; read the card on a PC. You get `DSCN####.NRW` (value 5 also writes a `.JPG`).

## Cosmetic: the row still says "Normal"

Only the menu *value* was changed, not its label. Menu labels resolve at runtime through
the UI string table (`GdiGetTextDataById`/`_hAppZst`, handle @ runtime) by string id
(the row's label id `0x2f3` @ `c0ebf35c` = "NORMAL"). Repointing it to an "NRW" string id
is not safely verifiable statically (the loaded resource is not at a fixed VA; a wrong id
renders garbage), so it is left as-is. The live-view quality indicator already shows
"NRW+Fine" correctly.

## The `.NRW` needs a one-time fix for desktop converters — `tools/nrwfix.py`

The unfinished COACH raw path writes the CFA strip **big-endian** under a little-endian
(`II`) TIFF header, and tags `BitsPerSample=12` though the data is 16-bit. Standard tools
(LibRaw/dcraw/RawTherapee/darktable) honor the header and mis-read the pixels (garbled,
striped color). `tools/nrwfix.py` byte-swaps the CFA strip and sets `BitsPerSample=16`:

```
python3 -I tools/nrwfix.py DSCN0119.NRW DSCN0119_fixed.NRW
```

After this the file opens correctly. Color is **not** camera-calibrated (no Nikon colour
matrix exists for the raw's `Make/Model` = `Zoran Corporation / COACH`), so set white
balance / a profile in your converter. Container details: IFD0 = thumbnail + full-size
embedded JPEG preview (SubIFD @264); the CFA raw is SubIFD @382 (4912×3680, CFA, BGGR
`CFAPattern [2,1,1,0]`, uncompressed, strip at file offset 5808).

## Firmware-update acceptance (for reference)

The retail "Setup → Firmware version → Update" reads `I:\firmware\firmware.bin` (a
`firmware` folder at the SD-card root). Validity = model-name match (first 6 bytes "P520")
+ header fields (block count 1, field@0x24 = 0x50) + the additive byte-sum. There is **no
firmware-version comparison** (same-version re-flash is allowed) and **no MD5/signature** on
the retail image (the payload's trailing 16 bytes are zero, not a hash). So `p520fw.py`'s
byte-sum reseal is sufficient for the camera to accept a patched image.
