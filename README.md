# HELIOS_HUNTER

![HeliosPool logo](assets/heliospool-main.png)

HELIOS_HUNTER is multi-coin mining firmware for the ESP32 Cheap Yellow Display. It combines an independent SHA-256d Stratum miner with balance pages for Cheetahcoin, WojakCoin, DigiByte, Bitcoin Cash, and Bitcoin.

## Independent Project

HELIOS_HUNTER was not created by HeliosPool and is not an official HeliosPool product. I am simply a big fan of HeliosPool and wanted to create something for its community.

For support, I can be reached in the `SOLO_HUNTER` channel of the HeliosPool Discord server.

Two display versions are available:

- `HELIOS_HUNTER_ILI9341_v1.0.6_merged.bin` for ILI9341 panels
- `HELIOS_HUNTER_ST7789_v1.0.6_merged.bin` for ST7789 panels

Use only the firmware version that matches the display controller in your CYD.

## Features

- Hardware-accelerated ESP32 SHA-256d mining
- User-configurable pool, port, username, worker, and password
- Mining remains independent from the selected balance screen
- Screen order: HeliosPool, CHTA, WJK, DGB, BCH, BTC
- Swipe navigation and matching rear-LED colors
- Live hashrate, accepted/rejected shares, and best difficulty on every page
- Address balances and a selectable USD, CAD, or GBP value
- Block-found style notification when a tracked balance increases
- Browser-based setup and management UI
- Persistent Wi-Fi, mining, wallet, currency, brightness, and rotation settings

## Hardware

HELIOS_HUNTER is designed for the:

- ESP32-2432S028R Cheap Yellow Display
- ILI9341 or ST7789 320x240 display
- XPT2046 touch controller

## Firmware Installation

HELIOS_HUNTER is provided as a ready-to-flash merged ESP32 firmware image. Arduino IDE is **not required** to install the firmware.

Download the merged `.bin` file that matches your display from the [latest GitHub release](https://github.com/XTVDDICT/HELIOS_HUNTER/releases/latest):

- `HELIOS_HUNTER_ILI9341_v1.0.6_merged.bin`
- `HELIOS_HUNTER_ST7789_v1.0.6_merged.bin`

### Flashing

Use an ESP32-compatible flashing tool and connect the CYD to your computer with USB.

Flash the selected merged firmware file at:

`0x0000`

Because the provided file is a **merged firmware image**, the bootloader, partition table, boot application data, and HELIOS_HUNTER firmware are already contained in a single file.

You do not need to manually flash separate files at `0x1000`, `0x8000`, `0xE000`, or `0x10000`.

### Important

Make sure you use the correct firmware for your display controller.

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
7. Use each coin tab to save the address whose balance should be displayed.

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
- Bitcoin Cash (BCH)
- Bitcoin (BTC)

through multiple independent explorer or Electrum sources. The firmware refreshes periodically and automatically tries a fallback when a provider is unavailable. DigiByte public data may occasionally be delayed by its provider.

The balance-increase notification assumes an increase may represent a mined reward. Incoming transfers can also trigger the notification.

## Release Privacy

Wi-Fi credentials, wallet addresses, and mining settings are entered after flashing and stored in the ESP32's local nonvolatile storage.

They are not embedded in this repository or in the released firmware files. Release binaries are built from the public source and checked for personal wallet addresses, Wi-Fi credentials, pool usernames, access tokens, email addresses, and local computer paths before publication.

HELIOS_HUNTER does not require your wallet seed phrase or private keys.

**Never enter a wallet seed phrase or private key into HELIOS_HUNTER. Only public wallet addresses are required for balance tracking.**

## Building From Source

Most users should install the release `.bin` with an ESP32 flasher. Arduino instructions are provided separately in [BUILDING.md](BUILDING.md) only for developers who want to compile the public source themselves.

## License

HELIOS_HUNTER is distributed under GPL-3.0-or-later.

See [LICENSE](LICENSE).
