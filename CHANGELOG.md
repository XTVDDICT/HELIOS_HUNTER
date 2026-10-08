# Changelog

## v1.0.9 - 2026-10-08

- Add eCash (`XEC`) to both display versions with its official logo, HeliosPool
  `xec.heliospool.com:3339` profile, wallet balance, fiat value, web tab,
  rear-LED color, and balance-increase notification support.
- Place XEC between DGB and BCH while preserving all existing persistent coin
  indexes and saved settings.
- Show the selected-currency total wallet value on the main screen with brighter,
  bolder HeliosPool-themed styling.
- Keep the pool host and port together as a single `host:port` endpoint on the
  main screen.

## v1.0.8 - 2026-10-02

- Add a FixedCoin (`FIX`) balance page, web tab, official logo, fiat values,
  matching rear-LED color, and balance-increase notifications to both display
  versions, ordered CHTA -> WJK -> FIX -> DGB -> BCH -> BTC.
- Keep FIX balance requests in the existing staggered background worker so
  mining continues while wallet data refreshes.
- Lock the recovered native SHA loop to the instruction-cache alignment used
  by the measured 920 kH/s build, preventing unrelated UI, logo, or coin-page
  additions from silently reducing mining speed.
- Start `HELIOS_HUNTER_SETUP` only when ESP32 confirms that no station SSID is
  stored. Failed connections, slow routers, and temporary Wi-Fi outages now
  remain in retry mode and never trigger a false setup portal.
- Strengthen station recovery with repeated saved-credential reconnects and a
  fresh station connection attempt every sixth retry without erasing settings.
- Buffer status JSON into 512-byte network writes and prevent overlapping web
  polls so dashboard traffic no longer blocks touchscreen handling one byte at
  a time.
- Use one bounded response buffer for HTTPS balance providers, preserve the
  last confirmed balance during temporary failures, and move popup
  acknowledgement storage outside the touch-critical path.
- Add compact runtime status and stack telemetry for diagnosing long-running
  devices without burdening normal display refreshes.
- Add generated web-asset checks, sketch-parity checks, and FAST SHA layout
  checks so both display versions retain the same shared behavior and mining
  engine.

## v1.0.7 - 2026-09-16

### Mining

- Recover the saved fast native SHA assembly and its flash-resident execution
  on eligible ESP32 revision-3 devices at 240 MHz CPU / 80 MHz APB.
- Observed peaks around 920 kH/s on tested CYDs; this is not a guaranteed rate
  for every chip or network condition.
- Retain full genesis and varied-header digest tests, nonce-wrap checks, and
  independent software verification before submitting a candidate.
- Retry hardware recovery with protected SHA-only resets. If the recovered
  loop fails validation, try the existing FAST loop before releasing hardware
  for software mining and a later retry. Do not permanently demote FAST after
  one transient failure or disable validation.
- Give the scheduler a real one-tick delay after 16 hardware batches while
  retaining watchdog protection and the auxiliary CPU miner.

### Display and Controls

- Add an amber BLOCKS counter beside accepted/rejected shares on the main page.
- Expand the 320x240 page layout and retain large, unabridged CHTA balances
  followed by CHTA.
- Add a rear-LED switch and screen-sleep timer to the Device tab. Turning the
  screen off does not stop mining; touch wakes it and a balance alert can wake it.
- Retain selectable USD/CAD/GBP values, coin logos, global mining statistics,
  and HeliosPool -> CHTA -> WJK -> DGB -> BCH -> BTC navigation.
- Reduce routine screen refresh work; keep swipe and alert handling responsive.

### Balances and Connectivity

- Persist balance baselines and pending balance-increase alerts for each saved
  address, and acknowledge alerts when dismissed. The first balance for a new
  address establishes a baseline, not a reward notification.
- Refresh balances every five minutes, stagger requests, bound HTTP response
  sizes, and check available memory before starting requests.
- Retain multiple explorer/Electrum providers. Provider failures can still make
  a balance temporarily unavailable; DigiByte data may be delayed.
- Enable Wi-Fi auto-reconnect and open the setup portal after saved credentials
  cannot connect at startup. Distinguish an active setup AP from WIFI RETRY.
- Add reset, uptime, pool-reconnect, memory, SHA timing, reference-validation,
  and recovery diagnostics.

### Release Safety

- Provide separate ILI9341 and ST7789 4 MB merged images for flashing at 0x0.
- Match both images to the R12 public sketch sources without recompiling them.
- Remove personal debug-path prefixes only from read-only strings and regenerate
  image checksums/hashes. No executable instructions or mining timing are changed.
- Check blank NVS, filesystem, and core-dump partitions and include SHA256SUMS.txt.

### Known Limitations

- Pool reconnects may still occur. Long-running watchdog/restart stability
  remains under observation; zero resets are not guaranteed.
- A balance increase is not proof of a mined block: incoming transfers can also
  trigger the notification. Address balances are not unpaid pool earnings.
- Merged-image flashing can clear local settings. Back them up before updating.

HELIOS_HUNTER is an independent fan project, not an official HeliosPool product.
Support is available from its creator in the SOLO_HUNTER channel of the
HeliosPool Discord.
