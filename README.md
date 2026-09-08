# HELIOS_HUNTER

![HeliosPool logo](assets/heliospool-main.png)

HELIOS_HUNTER is multi-coin mining firmware for the ESP32 Cheap Yellow
Display, built as a collaboration with HeliosPool. It combines an independent
SHA-256d Stratum miner with balance pages for Cheetahcoin, WojakCoin,
DigiByte, Bitcoin Cash, and Bitcoin.

Two display versions are included:

- `HELIOS_HUNTER_ILI9341` for ILI9341 panels
- `HELIOS_HUNTER_ST7789` for ST7789 panels

Use only the folder that matches the display controller in your CYD.

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

- ESP32-2432S028R Cheap Yellow Display
- ILI9341 or ST7789 320x240 display
- XPT2046 touch controller

## Arduino Requirements

- Arduino IDE 2.x
- Espressif ESP32 board package `3.3.11`
- ArduinoJson
- LovyanGFX
- WiFiManager

See [BUILDING.md](BUILDING.md) for the exact board settings and build steps.

## First Boot

1. Flash the firmware version matching the screen.
2. Connect a phone or computer to `HELIOS_HUNTER_SETUP`.
3. Select the Wi-Fi network the miner should use.
4. Open the IP address shown on the Helios screen.
5. Use the **Mining** tab to configure any compatible SHA-256d Stratum pool.
6. Use each coin tab to save the address whose balance should be displayed.

Changing or viewing a coin does not change the active mining pool. Mining is
global and continues while the balance and web interfaces are in use.

## Balance Data

Balance pages display public explorer data for the configured addresses. They
do not represent unpaid pool earnings. The firmware checks CHTA, WJK, DGB,
BCH, and BTC through their public explorer APIs and refreshes periodically.
WJK uses a secondary compatible explorer when its primary service is
unavailable. DigiByte's public data may be delayed by its provider.

The balance-increase notification assumes an increase may represent a mined
reward. Incoming transfers can also trigger it.

## Privacy

Wi-Fi credentials, wallet addresses, and mining settings are entered after
flashing and stored in the ESP32's local nonvolatile storage. They are not
embedded in this repository or in a freshly compiled firmware image.

## License

HELIOS_HUNTER is distributed under GPL-3.0-or-later. See [LICENSE](LICENSE).

