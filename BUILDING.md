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

## Export a Shareable Binary

Use **Sketch > Export Compiled Binary**. Arduino places the exported files in
the selected sketch folder. Share the file ending in `.merged.bin` and label
it clearly as either ILI9341 or ST7789.

The merged image is flashed at address `0x0`. Do not flash an ILI9341 image to
an ST7789 unit or an ST7789 image to an ILI9341 unit.

## Command-Line Build

The equivalent fully qualified board name is:

```text
esp32:esp32:jczn_2432s028r:UploadSpeed=921600,CPUFreq=240,FlashFreq=80,FlashMode=qio,FlashSize=4M,PartitionScheme=huge_app,DebugLevel=none,EraseFlash=none,LoopCore=1,EventsCore=1
```

Use Arduino IDE's **Verify/Compile** command to validate the selected sketch
before exporting or uploading it.
