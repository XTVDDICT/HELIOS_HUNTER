# HELIOS_HUNTER v1.0.11

Garlicoin balance support, selectable device pages, and balance/mining
improvements for both ILI9341 and ST7789 CYDs.

## Enhancements

- Added GRLC balance page, official coin logo, fiat value, matching rear LED,
  cached balance and touch-dismissed balance-increase alerts.
- Screen order: Home, CHTA, WJK, GRLC, FIX, DGB, XEC, ZEC, BCH, BTC.
- Added an auto-saving **Show on device** checkbox to every coin's web tab.
  Hidden pages and dots are skipped; Home is always available. Hiding pages
  leaves wallet settings, balance tracking and global mining intact.
- Improved fair balance refreshes, saved-address priority, cached prices,
  price-provider fallbacks, XEC units and ZEC transparent-address lookups.
- Added EUR alongside USD/CAD/GBP and kept all displayed hashrates in kH/s.
- Improved TLS scheduling, request deadlines, network memory use and dashboard
  recovery. Logos draw directly from flash without a runtime PNG decoder.
- Improved pipelined hardware SHA selection, software-helper scheduling and
  buffered Stratum traffic, with additional mining/reset diagnostics.

GRLC and ZEC are balance tracking only; mining supports SHA-256d Stratum pools.
Actual speed depends on the device and runtime conditions. Occasional resets
or pool reconnects may still occur. A balance-increase alert does not prove
that the device mined a block.

## Fresh Installation or Recovery

Use this for a new device, unrelated firmware or an unknown partition layout.
Arduino IDE is not required.

1. Download the **ILI9341** or **ST7789** file ending in **_merged.bin** that
   matches your CYD display.
2. Connect with a USB data cable and choose the correct serial port in an
   ESP32-compatible flasher.
3. Flash the merged image at **0x0** and restart the device. It already contains
   the bootloader, partition table and application; add no other files.
4. When prompted, join **HELIOS_HUNTER_SETUP**, enter Wi-Fi details, then open
   the screen's IP address to configure mining and coin balance addresses.

If the flasher asks: ESP32, 4 MB flash, QIO, 80 MHz. Use 921600 baud, or 115200
if communication fails. A merged installation can clear saved settings.

## Update Without Losing Settings

Use this only when already running HELIOS_HUNTER with the Huge APP partition
layout. Choose the update matching the physical display.

1. Download the matching file ending in **_update.bin**.
2. Select **only** that update file and flash it at **0x10000**.
   This is four zeros after the 1: **do not use 0x1000**.
3. Choose **No Erase**, or disable full-chip/Erase All Flash.
4. Flash and restart. Existing Wi-Fi, mining, wallet, currency, brightness,
   LED, sleep, rotation and page-visibility settings remain saved. Existing
   versions without page-visibility settings initially show every coin.

The update replaces only the application and leaves NVS at 0x9000 untouched.
Do not use an update with an unknown/different partition layout, or a flasher
that cannot select the address and disable full-chip erase.
Use the merged image at 0x0 for recovery.

## Release Files

- HELIOS_HUNTER_ILI9341_v1.0.11_merged.bin
- HELIOS_HUNTER_ILI9341_v1.0.11_update.bin
- HELIOS_HUNTER_ST7789_v1.0.11_merged.bin
- HELIOS_HUNTER_ST7789_v1.0.11_update.bin
- SHA256SUMS.txt

Do not interchange screen versions. A wrong screen build can give a black or
distorted display.

These images were exported and tested by the project owner. Release copies
have blank user-data partitions and personal compiler-path prefixes removed;
image checksums/hashes were checked and executable instructions are unchanged.
No configured-device dumps, ELF files or private-edition files are published.

**Independent fan project:** HELIOS_HUNTER was not created by HeliosPool and is
not an official HeliosPool product. I am a fan who wanted to create something
for the community. For support, reach me in the **SOLO_HUNTER** channel of the
HeliosPool Discord.

Performance code credits and license notices:
[THIRD_PARTY_NOTICES.md](https://github.com/XTVDDICT/HELIOS_HUNTER/blob/v1.0.11/THIRD_PARTY_NOTICES.md).
See [CHANGELOG.md](https://github.com/XTVDDICT/HELIOS_HUNTER/blob/v1.0.11/CHANGELOG.md)
for the source changes.
