# HELIOS_HUNTER v1.0.9

This release adds complete eCash (`XEC`) support to both CYD display versions
while preserving the validated FAST SHA mining engine and existing saved-setting
indexes.

## Enhancements

- Added an XEC balance page, web tab, fiat value, official logo, matching rear
  LED color, and balance-increase notification.
- Added `ecash:` and prefixless cash-address support using the official Electrum
  ABC server list and the correct two-decimal XEC base unit.
- Added the HeliosPool `xec.heliospool.com:3339` profile.
- Updated navigation to CHTA, WJK, FIX, DGB, XEC, BCH, BTC.
- Added a brighter, bolder total wallet value to the main screen.
- Combined the active pool host and port into one `host:port` line.
- Kept XEC balance work in the staggered background worker so mining remains
  independent from balance and price updates.

## New Installation

Use this for a new device, recovery, unrelated firmware, or an unknown partition
layout.

1. Download the `_merged.bin` matching the CYD's **ILI9341** or **ST7789**
   display. Do not interchange the two.
2. Connect the CYD with a USB data cable and select its serial port in an
   ESP32-compatible flasher.
3. Flash the merged image at **0x0000** and restart the device.
4. Join **HELIOS_HUNTER_SETUP** when prompted, enter Wi-Fi details, then open
   the IP shown on the screen to configure mining and wallet addresses.

The merged image contains everything required. Do not add separate bootloader,
partition, or application files.

## Update Without Losing Settings

Use this only on a device already running HELIOS_HUNTER with the Huge APP
partition layout.

1. Download the matching file ending in `_update.bin`.
2. Select only that file in the ESP32 flasher and set its address to
   **0x10000**.
3. Set **Erase Flash** to **No Erase**, or disable full-chip erase.
4. Flash the update and restart the device.

The update image replaces only the application and leaves the NVS partition at
`0x9000` untouched. Wi-Fi, mining, wallet, currency, brightness, LED, sleep,
and rotation settings are preserved. A flasher that cannot select `0x10000` and
disable erasing must not be used for a settings-preserving update.

Use `_merged.bin` at `0x0000` for clean installation or recovery. A merged image
can overwrite saved settings.

## Notes

Pool reconnects and occasional resets may still occur. Long-running stability
remains under observation; this release does not promise zero watchdog resets.
A balance alert means the tracked address increased and is not proof that the
device mined a block.

Released images contain no configured wallets, Wi-Fi details, or mining
credentials. User-data partitions are blank, personal compiler paths are
removed, image checksums are rebuilt, and executable instructions are unchanged.

**Independent fan project:** HELIOS_HUNTER was not created by HeliosPool and is
not an official HeliosPool product. I am a fan who wanted to make something for
the community. For support, reach me in the **SOLO_HUNTER** channel of the
HeliosPool Discord.

See [CHANGELOG.md](https://github.com/XTVDDICT/HELIOS_HUNTER/blob/v1.0.9/CHANGELOG.md)
for the complete changes.
