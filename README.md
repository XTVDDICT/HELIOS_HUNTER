# HELIOS_HUNTER

![HeliosPool logo](assets/heliospool-main.png)

HELIOS_HUNTER is multi-coin mining firmware for the ESP32 Cheap Yellow Display. It combines an independent SHA-256d Stratum miner with balance pages for Cheetahcoin, WojakCoin, Garlicoin, FixedCoin, DigiByte, eCash, Zcash, Bitcoin Cash, and Bitcoin.

## Independent Project

HELIOS_HUNTER was not created by HeliosPool and is not an official HeliosPool product. I am simply a big fan of HeliosPool and wanted to create something for its community.

For support, I can be reached in the `SOLO_HUNTER` channel of the HeliosPool Discord server.

Two display versions are available:

- `HELIOS_HUNTER_ILI9341_v1.0.12_merged.bin` for ILI9341 panels
- `HELIOS_HUNTER_ST7789_v1.0.12_merged.bin` for ST7789 panels

Use only the firmware version that matches the display controller in your CYD.

## What's New in v1.0.12

- Added Wi-Fi network name/password controls to the Device page, with an explicit open-network option.
- Try new Wi-Fi credentials before saving them. Failed connection attempts restore the previous network after 30 seconds.
- Open HELIOS_HUNTER_SETUP after 10 failed saved-network attempts. Power cycling starts a new set of attempts.
- Compacted Device settings into display/lighting and Wi-Fi cards, arranged side by side on desktop and stacked on phones.
- Added optional night schedules with start/end times and named timezones. Scheduled sleep turns off the backlight and rear LEDs while mining continues.
- Improved touch wake and block-alert handling. Optional 30-second alert auto-clear defaults to off.
- Added session uptime, average/peak hashrate in kH/s, and rejected-share percentage to the web dashboard.

GRLC and ZEC provide balance tracking. Mining uses SHA-256d, with hashrates displayed in kH/s.

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

Download the matching firmware from the [latest GitHub release](https://github.com/XTVDDICT/HELIOS_HUNTER/releases/latest). Use an ESP32 flasher that lets you select a file and flash address.

| Screen controller | Fresh-install file | Flash address |
| --- | --- | --- |
| ILI9341 | `HELIOS_HUNTER_ILI9341_v1.0.12_merged.bin` | `0x0` |
| ST7789 | `HELIOS_HUNTER_ST7789_v1.0.12_merged.bin` | `0x0` |

1. Connect the CYD using a USB data cable and select its COM/serial port in the flasher.
2. Choose the matching `_merged.bin` and enter address **`0x0`**.
3. If the flasher asks, use **ESP32**, **4 MB flash**, **QIO**, **80 MHz**, and **921600 baud**. Try **115200 baud** if transfer fails.
4. Flash the image, then restart the device. If it will not enter download mode, hold BOOT, tap RESET, and release BOOT when the flasher connects.
5. Connect your phone or computer to **HELIOS_HUNTER_SETUP**. Open **http://192.168.4.1** if the setup page does not appear automatically, then enter your 2.4 GHz Wi-Fi credentials.
6. Open the IP address shown on the device. Configure the Mining tab and each coin's balance address.

Use this fresh-install method for a new device, recovery, switching from other firmware, or an unknown partition layout. The merged image includes the bootloader, partition table, and application. A fresh install resets saved settings. Arduino IDE is unnecessary for installing these release files.

## Update Without Losing Settings

Use the update image on a device already running HELIOS_HUNTER with the **Huge APP (3MB No OTA/1MB SPIFFS)** partition layout.

| Screen controller | Settings-preserving update file | Flash address |
| --- | --- | --- |
| ILI9341 | `HELIOS_HUNTER_ILI9341_v1.0.12_update.bin` | `0x10000` |
| ST7789 | `HELIOS_HUNTER_ST7789_v1.0.12_update.bin` | `0x10000` |

1. Connect the device, select its COM/serial port, and choose **only** the matching `_update.bin`.
2. Set the flash address to **`0x10000`**, with four zeros after the 1.
3. Disable **Erase All Flash**, **Full-chip erase**, or the equivalent option. Select **No Erase** if available. The flasher still erases the application sectors it writes.
4. Flash the update and restart the device.

This replaces the application and preserves the NVS settings at `0x9000`, including saved Wi-Fi, mining configuration, wallets, currency, brightness, rotation, LED/sleep settings, and coin-page visibility. New night-mode and alert auto-clear settings default to off.

**`0x1000` is the wrong address for an application update.** Use `0x10000` for `_update.bin`. A merged image belongs at `0x0` and can clear your configuration. If the flasher cannot select an address and disable full-chip erase, choose a flasher that supports both options.

Always match the firmware to the screen controller. A mismatched display version can show a black or distorted screen.

## Changing Wi-Fi or Moving the Miner

While connected, open **Device > Wi-Fi**, enter the new network name and password, and select **Connect and save Wi-Fi**. Choose **Open network** only for a network without a password. Changing networks temporarily interrupts the pool connection and the dashboard. After connecting, join the new network on your phone and open the IP shown on the device.

The new credentials are saved after a successful connection. A failed attempt restores the previous network after 30 seconds.

If you move the device away from its saved network, power it on and allow **10 failed connection attempts**, approximately **1 minute 45 seconds** from power-up. Connect your phone to **HELIOS_HUNTER_SETUP** and open **http://192.168.4.1** to enter the new network. Power cycling resets the attempt counter. Mining and wallet settings are retained during Wi-Fi setup.

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
- Garlicoin (GRLC)
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
