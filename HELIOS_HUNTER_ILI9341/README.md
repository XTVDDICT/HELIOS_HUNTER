# HELIOS HUNTER v1.0.12

Current source sketch for ILI9341, including the 10-attempt Wi-Fi setup fallback.
Release firmware is compiled and exported by the project owner. Installation and settings-preserving update instructions are in the repository README.

## Display and Night Mode

Keep the existing screen-sleep choices on the Device tab.
Touch wakes a sleeping screen. A continuous held touch counts only once.
Mining and pool communications continue; this is backlight sleep, not ESP32 CPU sleep.

Night mode is optional and saved across restarts. Choose different start and end
times and a named timezone from the dropdown. Overnight schedules are supported.
During the schedule, the backlight and rear RGB LEDs are off. A touch wakes the
screen for 30 seconds, but the LEDs remain off until the schedule ends.

Network time is obtained asynchronously via NTP. Until valid network time is
available, scheduled night mode stays inactive; regular screen sleep still works.
The dropdown includes 597 timezone names and aliases from IANA tzdb 2026f via
Moment Timezone. The bundled transition data covers 2026 through 2100, including
daylight-saving and non-hour offsets. Future government rule changes require a
timezone data update. See TIMEZONE-LICENSE.txt.

## Block Notifications

On Mining Settings, "Auto-clear BLOCK FOUND after 30 seconds" is beside Enable Mining.
Auto-clear defaults to off. A new alert always wakes the screen for a fixed,
non-editable 15 seconds. If sleep or night mode is active, the backlight then
turns off, but the alert stays pending. With auto-clear enabled, the alert is
dismissed 30 seconds after it starts, independently of the backlight timer.

The first touch on a sleeping screen only wakes it. A second distinct touch
clears the pending alert. An already-awake alert can be cleared by one touch.
No Web UI clear-block button is added.

## Web Dashboard

The existing dashboard keeps its layout with neutral black surfaces.
Mining stats are Web UI-only: session uptime, average and peak kH/s, accepted
shares, and rejection percentage. No new stats are added to the device screen.
Average is total session hashes divided by elapsed session time, including
connection/idle time. Peak is the highest engine-reported smoothed sample.
Rejection percentage excludes pending submissions. Session stats reset when the
mining engine starts a new configuration/session, or after a reboot.
All hashrates remain in kH/s, including above 1,000.

## Open and Validate

Open HELIOS_HUNTER_ILI9341.ino from this folder in Arduino. Keep every .cpp, .h and .S file
beside it. Use the same ESP32 CYD board, Huge APP partition, and clock settings
as the previous working version; Arduino and event execution should use Core 0.
Existing libraries remain required, including WiFiManager and LovyanGFX.
The ILI9341 build retains its LovyanGFX touchscreen driver.
Existing NVS namespaces and old setting keys are preserved. New settings default
to off. Compiling/exporting and on-device validation are left to the owner.

Before release, test sleep at 30 seconds, touch wake/release, block wake at
15 seconds, optional auto-clear at 30 seconds, night start/end and LED behavior,
and settings persistence after reboot on both panels. Check hashing remains
normal with the browser closed and open. Source and mocked-browser checks do not
replace these hardware checks. No new .bin files are included here.
