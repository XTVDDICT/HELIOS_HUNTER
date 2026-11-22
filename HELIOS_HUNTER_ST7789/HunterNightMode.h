#pragma once
#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <time.h>
#include "HunterTimezones.h"

struct HunterDisplayOptions {
  bool autoClear = false;
  bool nightEnabled = false;
  uint16_t nightStart = 22 * 60;
  uint16_t nightEnd = 7 * 60;
  String timezone = "Etc/UTC";

  void load(Preferences& prefs) {
    autoClear = prefs.getBool("alertclear", false);
    nightEnabled = prefs.getBool("nighton", false);
    nightStart = prefs.getUShort("nightstart", 22 * 60) % 1440;
    nightEnd = prefs.getUShort("nightend", 7 * 60) % 1440;
    timezone = prefs.getString("nightzone", "Etc/UTC");
    if (hunterTimezoneIndex(timezone.c_str()) < 0) timezone = "Etc/UTC";
  }
  void save(Preferences& prefs) const {
    prefs.putBool("alertclear", autoClear);
    prefs.putBool("nighton", nightEnabled);
    prefs.putUShort("nightstart", nightStart);
    prefs.putUShort("nightend", nightEnd);
    prefs.putString("nightzone", timezone);
  }
};

inline String hunterTimeText(uint16_t minutes) {
  char text[6];
  snprintf(text, sizeof(text), "%02u:%02u", minutes / 60, minutes % 60);
  return String(text);
}

inline bool hunterParseTime(const String& text, uint16_t& minutes) {
  if (text.length() != 5 || text[2] != ':' || text[0] < '0' || text[0] > '2' ||
      text[1] < '0' || text[1] > '9' || text[3] < '0' || text[3] > '5' ||
      text[4] < '0' || text[4] > '9') return false;
  unsigned hour = (text[0] - '0') * 10 + text[1] - '0';
  if (hour > 23) return false;
  minutes = hour * 60 + (text[3] - '0') * 10 + text[4] - '0';
  return true;
}

inline bool hunterReadNightOptions(const String& start, const String& end,
                                   const String& zone, HunterDisplayOptions& out) {
  uint16_t first, last;
  if (!hunterParseTime(start, first) || !hunterParseTime(end, last) ||
      first == last || hunterTimezoneIndex(zone.c_str()) < 0) return false;
  out.nightStart = first;
  out.nightEnd = last;
  out.timezone = zone;
  return true;
}

inline void hunterEnsureClockStarted(bool enabled) {
  static bool started = false;
  if (enabled && !started && WiFi.status() == WL_CONNECTED) {
    configTime(0, 0, "pool.ntp.org", "time.cloudflare.com", "time.google.com");
    started = true;
  }
}

class HunterNightClock {
 public:
  bool valid = false;
  bool active(const HunterDisplayOptions& options) {
    // NTP runs asynchronously; never wait for time on the foreground task.
    hunterEnsureClockStarted(options.nightEnabled);
    time_t utc = time(nullptr);
    valid = utc >= 1767225600LL && utc < 4133980800LL;
    if (!valid || !options.nightEnabled) return false;
    if (utc == cachedUtc_ && options.timezone == cachedZone_ &&
        options.nightStart == cachedStart_ && options.nightEnd == cachedEnd_) return cachedActive_;
    cachedUtc_ = utc;
    cachedZone_ = options.timezone;
    cachedStart_ = options.nightStart;
    cachedEnd_ = options.nightEnd;
    int zone = hunterTimezoneIndex(options.timezone.c_str());
    if (zone < 0) return false;
    time_t local = utc + hunterTimezoneOffset(zone, utc);
    struct tm clock;
    gmtime_r(&local, &clock);
    uint16_t minute = clock.tm_hour * 60 + clock.tm_min;
    cachedActive_ = options.nightStart < options.nightEnd
        ? minute >= options.nightStart && minute < options.nightEnd
        : minute >= options.nightStart || minute < options.nightEnd;
    return cachedActive_;
  }
 private:
  time_t cachedUtc_ = 0;
  String cachedZone_;
  uint16_t cachedStart_ = 0;
  uint16_t cachedEnd_ = 0;
  bool cachedActive_ = false;
};
