# Device Diagnostics

Current release: v1.0.7, build `R12_SHA_REFERENCE_NATIVE_20260916`, driver
`REFERENCE_NATIVE_11`. The recovered native loop is gated by full startup tests;
`shaTiming: REFERENCE_NATIVE_DPORT` and `shaReferenceCheck: PASSED` identify it.
If it fails validation, the existing IRAM FAST loop is tested next. Invalid
hardware is never forced to remain active. The creator has confirmed speed
recovery on hardware; long-term restart stability is still under observation.

The Device tab shows last reset, uptime, pool reconnects, and minimum free memory.
A pool reconnect is not a device reboot: use uptime and reset reason to tell
them apart. Keep watchdog and candidate verification enabled.

Release binaries are installed with an ESP32 flasher at 0x0. Arduino is only
needed when building modified source. The sections below preserve earlier
investigation history and describe the retained diagnostic endpoints; their
older build identifiers do not describe the current release.

## First-Failure Capture

Driver `FAILURE_CAPTURE_7` keeps the first varied-header digest or filter failure
in RAM until reboot. It captures synthetic test data only, not mining credentials
or wallets. A single writer publishes the completed record once; later failures
cannot overwrite it. The per-nonce routines and original pass/fail checks are
unchanged from REFERENCE_BUFFER_6.

After the user's ST upload, read `GET /api/diagnostics/sha-failure`.
`captured: false` means the relevant failure has not been recorded yet.
Keep the ST powered on after it falls back so the record survives.
The response includes both hardware and reference headers, nonce accounting,
the hardware digest, automatic reference digest, a peripheral reread, and a
second reference explicitly forced through mbedTLS's software implementation.
That second computation happens only while capturing the first failure and does
not replace or bypass the original validation decision.

The reread is taken after the original automatic reference ran. A changed reread
does not distinguish a concurrent peripheral writer from automatic-reference
hardware use or an unstable initial read on its own.

Save only this dedicated response, not the credential-bearing status response.
Run `tools/Analyze-ShaFailure.ps1 -Path <capture.json>` on the PC to recompute
SHA-256d using .NET and distinguish mismatched inputs, hardware/readback errors,
and automatic-reference errors. This does not compile firmware.
`tools/Test-ShaFailureAnalyzer.ps1` covers a known genesis hash, corrupted
hardware/reference results, different headers, nonce wrap, and malformed data.

The capture diagnoses self-test failures, not every live candidate error. No
record is produced if all self-tests pass, even if a live candidate had failed.
No flash writes, serial logging, forced engine modes, or restart changes are
added. Both source versions are synchronized; test the ST and leave ILI running
its existing firmware.

## September 15 Speed Recovery

IRAM_ORDERED_5 continued producing digest mismatches and was observed running
guarded FAST around 750-780 kH/s. A short ST-only isolation test recorded:

- Paused start: uptime 1394 seconds, 14 recoveries, guarded FAST, 771.9 kH/s.
- After 90 seconds fully paused: uptime 1484, still 14 recoveries, guarded FAST,
  777.3 kH/s.
- Thirty seconds after restoring fetching: uptime 1514, 15 recoveries, polling
  SAFE, 522.0 kH/s. Fetching was confirmed enabled again.

This is a correlation from one short comparison, not proof of a TLS root cause.
No mining settings were changed and neither device was flashed by the agent.

REFERENCE_BUFFER_6 removes the three CONTINUE/LOAD barriers added in revision 5,
restoring the reference timed loop's command spacing and retaining its START
barriers and delays. Those added barriers had not prevented digest failures.
The full digest now uses ESP-IDF's `esp_dport_access_read_buffer`, also used by
its SHA-256 HAL, followed by byte conversion outside the protected read. This
keeps the interrupt-masked sequence inside the SDK's IRAM implementation instead
of our flash-resident caller. An aligned local buffer avoids casting the output
byte array to a word pointer.

Full-digest and candidate validation, bounded polling, protected reset, and
noinline/IRAM placement remain. Native FAST is still conditional on passing its
tests; no mode is forced. Hardware speed and fault behavior need testing after
the user's compile/upload. No new binary has been generated.

## September 14 Follow-Up

The ST running `RCC_LOCKED_4` still reported recovery failures, including a
varied-header self-test failure in SAFE. The previous correction did not resolve
the reported fault. The attached older reference binary exactly matches the
saved September 10 ST export (SHA-256
`ea3f0fa9a83d1e4a3dc4211846f4c627f70541f594773a3ec65e50d30042322f`).
Its matching ELF confirms that it used only the genesis hardware self-test.
FAST in that build is not evidence that it passed the newer varied-header test.

Inspection of existing compiled code showed both SHA routines inlined into
`mineLocked` in `.flash.text`, despite their IRAM attributes. The current source:

- Prevents inlining of both SHA routines so their IRAM placement is retained.
- Orders every timed SHA command with MEMW before starting its delay.
- Waits for the first compression to finish before overwriting TEXT in SAFE.
- Bounds each SAFE busy-wait at 4096 polls and returns a recorded timeout instead
  of spinning indefinitely.
- Distinguishes pipeline, nonce-accounting, reference-software, digest, and
  filter errors in the varied-header test.

The existing timed delays are not shortened, and validation remains enabled.
This may change measured hashrate; the effect must be measured on hardware.
The source checks do not establish the runtime root cause. After the user's
build, its ELF can be inspected without compiling again to confirm the routines
are actually in IRAM. The new driver marker is `IRAM_ORDERED_5`.

## SHA SAFE Investigation

The ST reported repeated candidate hash mismatches, reached polling/SAFE mode,
and continued reporting mismatches there. The previous single last-fault field
overwrote the specific downgrade reason when a later candidate failed.

Earlier RCC_LOCKED_4 corrections, retained in both screen versions:

- Stop rewriting shared crypto clock/reset registers every mining batch.
  ESP-IDF's engine reservations already keep SHA enabled.
- Use ESP-IDF's shared RCC lock and SHA-only reset operation on recovery.
  Do not assert SECUREBOOT reset, which also affects other crypto.
- Wait for SHA to finish before taking a full digest snapshot for verification.
  This adds no instructions inside either per-nonce hashing loop.
- Reset peripheral state before recovery self-tests. Keep genuine self-test
  failures, software verification, and watchdog protection enabled.
- Retain `shaLastFallback` and `shaLastSelfTestFailure` separately from
  `shaLastFault`, so later candidate failures cannot erase the downgrade reason.

These correct unsafe register access and digest-read behavior. They do not yet
establish which defect caused the observed faults or prove the restarts fixed.
Test the ST first and leave the older ILI firmware as a comparison.
`/api/status` identifies the current driver as `shaDriver: FAILURE_CAPTURE_7`.
Record SHA timing, recovery count, fallback reason, self-test failure, uptime,
and reset reason with normal balance updates running. Do not disable validation
or force FAST when a self-test fails.

## Controlled Comparison

1. Capture a serial watchdog report from the running firmware first, if possible.
   Keep the complete watchdog task list, backtrace, and ELF SHA. Decode using the
   ELF from that exact build, not a newly compiled replacement.
2. Test the ST by itself with one diagnostic binary for both phases. Keep the ILI
   unchanged as a reference; it currently runs a different firmware revision.
3. Record normal operation with balance fetching enabled. Record uptime, reset
   reason, SHA timing, recovery count, hashrate, and minimum free heap.
4. POST `enabled=0` (form encoded) to `/api/diagnostics/balance-fetch`.
5. Read `/api/status` until `balanceFetchState` is `PAUSED`. `PAUSING` means the
   selected coin refresh is still finishing; do not start the off-phase timer yet.
6. Observe for at least the same duration as the on-phase and preferably longer
   than the previously observed interval between resets. Record any SHA timing
   change separately, because it changes the comparison.
7. POST `enabled=1` to the same endpoint to resume and repeat the comparison.

The diagnostic switch is RAM-only and resets to enabled after a reboot. If uptime
drops, record the reset and end that phase; do not treat the resumed fetching as
part of the paused phase. Neither POST changes mining configuration or wallets.

Existing balances remain displayed while fetching is paused. Balance-based
increase notifications and prices cannot update until fetching resumes. A
currently selected coin refresh, including retries and storage, finishes normally
before the worker reports `PAUSED`; no task is forcibly suspended.

## Interpretation

- A watchdog backtrace and named starved task are needed to localize the restart.
- A restart after `PAUSED` weakens the hypothesis that active balance fetching is
  required for the failure. It does not rule out effects of earlier requests.
- No restart during one paused interval is evidence to investigate, not proof of
  a fix. Repeat the enabled/paused comparison.
- Hardware/software hash mismatches are a separate symptom. Keep validation and
  watchdogs enabled throughout the investigation.

Do not publish complete `/api/status` responses: they also contain wallet and
pool credentials. Save only the diagnostic fields listed above and the
`balanceFetchEnabled`, `balanceFetchActive`, `balanceFetchState`, and
`balanceFetchStateSinceMs` fields.
