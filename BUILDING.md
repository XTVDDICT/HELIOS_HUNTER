# Building From Source

This document is optional. Normal installation uses a merged `.bin` from the
[GitHub Releases page](https://github.com/XTVDDICT/HELIOS_HUNTER/releases) and
an ESP32 flashing program with flash address `0x0`.

## Select the Sketch

Open exactly one of these files in Arduino IDE:

- `HELIOS_HUNTER_ILI9341/HELIOS_HUNTER_ILI9341.ino`
- `HELIOS_HUNTER_ST7789/HELIOS_HUNTER_ST7789.ino`

The folder and `.ino` filename must remain identical.

## Install Support

In Arduino IDE, install Espressif's ESP32 board package version `3.3.11` and
these libraries through Library Manager:

- ArduinoJson
- LovyanGFX
- WiFiManager

## Board Settings

Use these settings under **Tools**:

| Setting | Value |
| --- | --- |
| Board | ESP32-2432S028R |
| Upload Speed | 921600 |
| CPU Frequency | 240 MHz |
| Flash Frequency | 80 MHz |
| Flash Mode | QIO |
| Flash Size | 4 MB |
| Partition Scheme | Huge APP (3MB No OTA/1MB SPIFFS) |
| Core Debug Level | None |
| Erase All Flash Before Sketch Upload | Disabled |
| Arduino Runs On | Core 1 |
| Events Run On | Core 1 |
| Zigbee Mode | Disabled |

Choose the correct COM port, then use **Sketch > Verify/Compile** followed by
**Sketch > Upload**.

## Hashrate Release Gate

Hashrate is a release requirement. The native SHA loop has a fixed 32-byte
function boundary and internal padding that preserve the cache position of the
measured 920 kH/s build. Do not remove or alter that alignment or padding.

Before publishing either display version, test it on the same CYD and pool as
the current reference image. A new build must retain `HARDWARE SHA FAST`, must
not introduce watchdog resets or SAFE-mode fallback, and must not show a
repeatable hashrate regression. A feature is not release-ready when it passes
functional checks but reduces mining speed.

After compiling, verify the private ELF before testing the device:

```text
powershell -ExecutionPolicy Bypass -File tools/Check-FastShaLayout.ps1 <path-to-sketch.elf>
```

The check must report `PASS`. Keep the ELF private; do not include it in a
release.

## Export a Shareable Binary

Use **Sketch > Export Compiled Binary**. Arduino places the exported files in
the selected sketch folder. Share the file ending in `.merged.bin` and label
it clearly as either ILI9341 or ST7789.

The merged image is flashed at address `0x0`. Do not flash an ILI9341 image to
an ST7789 unit or an ST7789 image to an ILI9341 unit.

## Edit the Web Interface

The editable web page lives once at `web/HeliosWebPage.html`. After changing it,
regenerate the compressed sketch headers and check that they are current:

```text
node tools/Generate-WebAsset.cjs .
node tools/Generate-WebAsset.cjs --check .
```

Both display sketches serve the same gzip asset. Do not edit the generated
`HeliosWebPageGzip.h` files directly.

## Regenerate Display Assets

The original PNG artwork lives in `assets/`. After changing a logo, regenerate
the shared display header and copy the result to the other screen folder:

```text
py -3 tools/generate_logo.py assets HELIOS_HUNTER_ILI9341/HeliosAssets.h
```

Both screen versions use identical artwork, so their generated
`HeliosAssets.h` files must remain byte-for-byte identical.

## Prepare a Public Release Copy

Compiled libraries may embed your local computer paths in assertion/debug
strings even when no wallet or Wi-Fi settings are compiled in. The v1.0.8
release copies use the included Node.js tool to remove those personal prefixes:

```text
node tools/Prepare-ReleaseImage.cjs input.merged.bin output.merged.bin
node tools/Prepare-ReleaseImage.cjs --test
```

This tool is pinned to the R17/v1.0.8 build. It accepts only a 4 MB merged image
with one application partition and blank user-data partitions. It modifies
read-only debug-path prefixes only, preserves string lengths and executable
instructions, and regenerates and checks the ESP32 checksum/SHA-256 footers.
Signed images, configured-device dumps, and unexpected formats are rejected.
It does not compile, export through Arduino, connect to hardware, or flash.

Check the prepared image with Espressif's image tools, scan for personal data,
and publish its SHA-256 checksum. Never publish an ELF, build cache, configured
flash dump, or complete `/api/status` response: those may contain local paths
or private settings. Use the original ELF privately for decoding backtraces;
only debug-path strings and integrity footers differ in the public release copy.

## Command-Line Build

The equivalent fully qualified board name is:

```text
esp32:esp32:jczn_2432s028r:UploadSpeed=921600,CPUFreq=240,FlashFreq=80,FlashMode=qio,FlashSize=4M,PartitionScheme=huge_app,DebugLevel=none,EraseFlash=none,LoopCore=1,EventsCore=1
```

Use Arduino IDE's **Verify/Compile** command to validate the selected sketch
before exporting or uploading it.
