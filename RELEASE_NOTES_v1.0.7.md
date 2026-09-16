# HELIOS_HUNTER v1.0.7

The fast engine is back. This release includes the recovered native SHA loop
with observed peaks around **920 kH/s** on tested CYDs, while retaining full
hash validation and watchdog protection. Rates vary with the chip and activity.

## Enhancements

- Recovered FAST engine with validation-gated startup and recovery retries.
- More visible amber **BLOCKS** counter beside accepted/rejected shares.
- Expanded screen layout and large, unabridged CHTA balances with the CHTA label.
- Rear-LED on/off and screen-sleep options. Mining continues with the screen asleep.
- Persistent balance baselines and pending balance-increase alerts across reboots.
- Staggered balance updates, memory checks, and clearer Wi-Fi retry/setup behavior.
- Device uptime, last-reset, reconnect, and SHA-engine diagnostics.

## Install With an ESP32 Flasher

1. Download the merged `.bin` matching your CYD display:
   **ILI9341** or **ST7789**. Do not interchange the two.
2. Connect the CYD using a USB data cable and select its serial port in an
   ESP32-compatible flasher.
3. Flash the selected merged image at **0x0000** and restart the device.
4. When needed, join **HELIOS_HUNTER_SETUP**, enter your Wi-Fi details, and open
   the IP displayed on the miner to configure Mining and balance addresses.

**Arduino IDE is not required.** Save your existing settings before updating;
merged-image flashing can clear them. SHA256SUMS.txt is included for checking
the downloaded binaries.

## Notes

Pool reconnects can still occur. Long-term restart stability remains under
observation; this release is not a promise of zero watchdog resets. Balance
alerts indicate an address-balance increase, which can also be an incoming
transfer, not necessarily a mined reward.

Released images contain no configured wallets or mining credentials. Settings,
filesystem, and core-dump partitions are blank. Personal debug paths have been
removed, image checksums validated, and executable instructions left unchanged.

**Independent fan project:** HELIOS_HUNTER was not created by HeliosPool and is
not an official HeliosPool product. I am a fan who wanted to make something for
the community. For support, reach me in the **SOLO_HUNTER** channel of the
HeliosPool Discord.

See [CHANGELOG.md](https://github.com/XTVDDICT/HELIOS_HUNTER/blob/v1.0.7/CHANGELOG.md)
for the full changes.
