# Changelog

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
