# HELIOS_HUNTER v1.0.12

Wi-Fi management, scheduled night mode, a compact Device page, and additional dashboard statistics for both ILI9341 and ST7789 CYDs.

## Enhancements

- Added Wi-Fi network name/password controls to the Device page, with an explicit open-network option.
- Try new Wi-Fi credentials before saving them. Failed connection attempts restore the previous network after 30 seconds.
- Open HELIOS_HUNTER_SETUP after 10 failed saved-network attempts. Power cycling starts a new set of attempts.
- Compacted Device settings into display/lighting and Wi-Fi cards, arranged side by side on desktop and stacked on phones.
- Added optional night schedules with start/end times and named timezones. Scheduled sleep turns off the backlight and rear LEDs while mining continues.
- Improved touch wake and block-alert handling. Optional 30-second alert auto-clear defaults to off.
- Added session uptime, average/peak hashrate in kH/s, and rejected-share percentage to the web dashboard.

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


## Integrity and Validation

- Both screen variants were compiled and exported by the project owner.
- Source parity and embedded web assets checked. Responsive layout and Wi-Fi form checked with mocked browser responses.
- Exported application matches its merged image. ESP32 image checksums, SHA-256 footers, partition layout, and blank user-data partitions checked.
- Release copies remove personal debug-path prefixes while preserving executable instructions.
- SHA loop layout checked against both exported ELF files.
- Hardware Wi-Fi recovery, night schedules, and long-running device stability require device validation.
- SHA256SUMS.txt contains checksums for all four firmware downloads.

HELIOS_HUNTER is an independent fan project created by XTVDDICT. Support is available through the SOLO_HUNTER channel in the HeliosPool Discord.
