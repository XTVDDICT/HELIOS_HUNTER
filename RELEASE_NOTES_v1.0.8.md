# HELIOS_HUNTER v1.0.8

This release adds FixedCoin support and makes the balance, touchscreen, web,
and Wi-Fi paths substantially lighter while preserving the validated FAST SHA
engine. Tested CYDs have shown peaks around **920 kH/s**; results vary by chip,
pool, and network activity.

## Enhancements

- Added a complete FixedCoin (`FIX`) page, web tab, fiat value, logo, LED color,
  and balance-increase notification to both display versions.
- Updated the screen order to CHTA, WJK, FIX, DGB, BCH, BTC.
- Buffered web status responses and blocked overlapping polls to keep touch and
  screen navigation responsive.
- Reduced HTTPS balance memory pressure and retained the last confirmed balance
  when a provider is temporarily unavailable.
- Kept the persistent balance-increase popup with touch-to-clear behavior.
- Restricted setup AP mode to devices that truly have no saved Wi-Fi network.
- Added parity, generated-asset, stack, and FAST SHA layout checks.

## Install With an ESP32 Flasher

1. Download the merged `.bin` matching your CYD display: **ILI9341** or
   **ST7789**. Do not interchange the two.
2. Connect the CYD using a USB data cable and select its serial port in an
   ESP32-compatible flasher.
3. Flash the selected merged image at **0x0000** and restart the device.
4. When setup is needed, join **HELIOS_HUNTER_SETUP**, enter your Wi-Fi details,
   and open the IP shown on the miner to configure mining and balance addresses.

**Arduino IDE is not required.** Save existing settings before updating;
merged-image flashing can clear them. `SHA256SUMS.txt` is included for checking
the downloads.

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

See [CHANGELOG.md](https://github.com/XTVDDICT/HELIOS_HUNTER/blob/v1.0.8/CHANGELOG.md)
for the complete changes.
