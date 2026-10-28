# HELIOS_HUNTER

![HeliosPool logo](assets/heliospool-main.png)

HELIOS_HUNTER is multi-coin mining firmware for the ESP32 Cheap Yellow Display. It combines an independent SHA-256d Stratum miner with balance pages for Cheetahcoin, WojakCoin, FixedCoin, DigiByte, eCash, Zcash, Bitcoin Cash, and Bitcoin.

## Independent Project

HELIOS_HUNTER was not created by HeliosPool and is not an official HeliosPool product. I am simply a big fan of HeliosPool and wanted to create something for its community.

For support, I can be reached in the `SOLO_HUNTER` channel of the HeliosPool Discord server.

Two display versions are available:

- `HELIOS_HUNTER_ILI9341_v1.0.11_merged.bin` for ILI9341 panels
- `HELIOS_HUNTER_ST7789_v1.0.11_merged.bin` for ST7789 panels

Use only the firmware version that matches the display controller in your CYD.

## What's New in v1.0.11

- Added Garlicoin (GRLC) balances, logo, fiat prices, matching LED color and
  balance-increase alerts between WJK and FIX.
- Added a saved "Show on device" checkbox on every coin's web tab. Hidden
  pages are skipped in both directions; Home always remains available.
- Improved balance scheduling, price caching/fallbacks, TLS timeouts and memory
  use, including native flash-pixel logo drawing.
- Improved software-helper scheduling and buffered pool communication, with
  additional mining and reset diagnostics.
- Updated the pipelined hardware SHA engine with validated fallbacks.
- Added EUR alongside USD/CAD/GBP and kept all hashrates in kH/s.

GRLC and ZEC are balance tracking only. The miner uses SHA-256d. Actual
hashrate depends on hardware and runtime conditions; this release does not
promise a particular speed or zero watchdog resets.

## Features

- Hardware-accelerated ESP32 SHA-256d mining
- User-configurable pool, port, username, worker, and password
- Mining remains independent from the selected balance screen
- Screen order: HeliosPool, CHTA, WJK, GRLC, FIX, DGB, XEC, ZEC, BCH, BTC
- Looping swipe navigation and matching rear-LED colors
- Per-coin "Show on device" checkboxes in the web UI, saved across restarts
- Live hashrate, accepted/rejected shares, and best difficulty on every page
- Address balances and a selectable USD, CAD, GBP, or EUR value
- Block-found style notification when a tracked balance increases
- Browser-based setup and management UI
- Persistent Wi-Fi, mining, wallet, currency, brightness, and rotation settings

Third-party performance code and its required license notices are documented
in [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

## Hardware

HELIOS_HUNTER is designed for the:

- ESP32-2432S028R Cheap Yellow Display
- ILI9341 or ST7789 320x240 display
- XPT2046 touch controller

## New Installation

Use this method for a new device, recovery, switching from unrelated firmware,
or whenever the existing partition layout is unknown. Arduino IDE is **not
required**.

Download the merged `.bin` file that matches your display from the [latest GitHub release](https://github.com/XTVDDICT/HELIOS_HUNTER/releases/latest):

- `HELIOS_HUNTER_ILI9341_v1.0.11_merged.bin`
- `HELIOS_HUNTER_ST7789_v1.0.11_merged.bin`

Use an ESP32-compatible flashing tool and connect the CYD to your computer with USB.

1. Select the merged file matching the CYD's ILI9341 or ST7789 screen.
2. Flash it at `0x0000`.
3. Restart the device and complete `HELIOS_HUNTER_SETUP` when prompted.

The merged image contains the bootloader, partition table, boot application
data, and HELIOS_HUNTER application. Do not assign additional offsets or flash
the app-only update file during a new installation.

## Update Without Losing Settings

Use this method only on a device already running HELIOS_HUNTER with the Huge APP
partition layout.

Download the update file matching the device's screen:

- `HELIOS_HUNTER_ILI9341_v1.0.11_update.bin`
- `HELIOS_HUNTER_ST7789_v1.0.11_update.bin`

1. In the ESP32 flasher, select only the matching `_update.bin` file.
2. Set its flash address to `0x10000` (four zeros after the 1, **not `0x1000`**).
3. Set **Erase Flash** to **No Erase**, or disable full-chip erase.
4. Flash the update and restart the device.

The update image replaces only the application. It leaves the NVS settings at
`0x9000` untouched, preserving Wi-Fi, mining, wallet, currency, brightness,
LED, sleep, rotation, and coin-page visibility settings. Do not use this method if the flasher cannot
select `0x10000` and disable erase.

### Important

Always use the firmware matching the display controller:

- ILI9341 display → use the ILI9341 firmware
- ST7789 display → use the ST7789 firmware

Flashing the wrong display version may result in a blank, distorted, incorrectly colored, or unusable screen.

## First Boot

1. Flash the merged firmware file that matches your screen at `0x0000`.
2. Restart the ESP32 Cheap Yellow Display.
3. Connect a phone or computer to the `HELIOS_HUNTER_SETUP` Wi-Fi network.
4. Select the Wi-Fi network the miner should use.
5. Once connected, open the IP address shown on the Helios screen.
6. Use the **Mining** tab to configure any compatible SHA-256d Stratum pool.
7. Use each coin tab to save its balance address and choose **Show on device**.

Changing or viewing a coin does not change the active mining pool.

Mining is global and continues while the balance pages and web interface are in use.

## Source Code

The HELIOS_HUNTER source code is included in this repository for both supported display versions.

Separate source folders are provided for:

- ILI9341
- ST7789

Most users do not need to compile the source code. The precompiled merged `.bin` files can be flashed directly using an ESP32 flashing tool.

The source is provided for development, modification, troubleshooting, and community contributions.

## Balance Data

Balance pages display public network data for the configured addresses. They do not represent unpaid pool earnings.

The firmware checks:

- Cheetahcoin (CHTA)
- WojakCoin (WJK)
- DigiByte (DGB)
- eCash (XEC)
- Zcash (ZEC, transparent `t1`/`t3` addresses)
- Bitcoin Cash (BCH)
- Bitcoin (BTC)
- FixedCoin (FIX)

through multiple independent explorer or Electrum sources. The firmware refreshes periodically and automatically tries a fallback when a provider is unavailable. DigiByte public data may occasionally be delayed by its provider.

The ZEC page tracks public transparent-address balances and fiat values. Zcash
uses Equihash, so ZEC cannot be mined by HELIOS_HUNTER's SHA-256d engine.

The balance-increase notification assumes an increase may represent a mined reward. Incoming transfers can also trigger the notification.

## Release Privacy

Wi-Fi credentials, wallet addresses, and mining settings are entered after flashing and stored in the ESP32's local nonvolatile storage.

They are not embedded in this repository or in the released firmware files. Release binaries are built from the public source and checked for personal wallet addresses, Wi-Fi credentials, pool usernames, access tokens, email addresses, and local computer paths before publication.

The release copies also remove personal compiler/debug paths from read-only strings and update the ESP32 image checksums. Executable instructions are unchanged. Settings, filesystem, and core-dump partitions are checked to be blank; release images are not dumps from configured devices.

HELIOS_HUNTER does not require your wallet seed phrase or private keys.

**Never enter a wallet seed phrase or private key into HELIOS_HUNTER. Only public wallet addresses are required for balance tracking.**

## Building From Source

Most users should install the release `.bin` with an ESP32 flasher. Arduino instructions are provided separately in [BUILDING.md](BUILDING.md) only for developers who want to compile the public source themselves.

## License

HELIOS_HUNTER is distributed under GPL-3.0-or-later.

See [LICENSE](LICENSE).
