#pragma once
#include <stdint.h>

// Backlight policy only. Never suspends mining, networking, or the CPU.
class HunterDisplayPolicy {
 public:
  bool sleeping = false;
  bool night = false;
  bool alert = false;

  void begin(uint32_t now) { awakeAt_ = now; }
  void blockFound(uint32_t now) {
    alert = true;
    alertAt_ = now;
    awakeAt_ = now;
    wakeWindow_ = 15000;
    sleeping = false;
  }
  void blockCleared() { alert = false; }
  bool touch(uint32_t now) {
    awakeAt_ = now;
    wakeWindow_ = 30000;
    if (sleeping) {
      sleeping = false;
      return false;
    }
    return alert;
  }
  bool update(uint32_t now, bool scheduledNight, uint16_t idleSeconds,
              bool autoClear) {
    if (scheduledNight != night) {
      night = scheduledNight;
      if (!night) {
        sleeping = false;
        awakeAt_ = now;
      } else if (!alert) {
        sleeping = true;
      }
    }
    const bool expired = alert && autoClear &&
                         uint32_t(now - alertAt_) >= 30000;
    const uint32_t limit = alert ? wakeWindow_
                                : (night ? 30000UL : uint32_t(idleSeconds) * 1000UL);
    if (!night && idleSeconds == 0) sleeping = false;
    else if (limit && uint32_t(now - awakeAt_) >= limit) sleeping = true;
    return expired;
  }

 private:
  uint32_t awakeAt_ = 0;
  uint32_t alertAt_ = 0;
  uint32_t wakeWindow_ = 15000;
};
