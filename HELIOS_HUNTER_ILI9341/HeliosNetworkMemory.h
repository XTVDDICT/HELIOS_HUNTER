#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// Initialize before starting the balance worker or foreground task.
void heliosNetworkMemoryBegin();
bool heliosBalanceNetworkBusy();

// Keep TLS/payload/JSON refresh allocations separate from WebServer parsing
// and responses. The UI uses a zero timeout so display/touch keep running.
class HeliosNetworkMemoryGuard {
 public:
  explicit HeliosNetworkMemoryGuard(TickType_t waitTicks, bool balanceWork = false);
  ~HeliosNetworkMemoryGuard();
  explicit operator bool() const { return locked_; }
  HeliosNetworkMemoryGuard(const HeliosNetworkMemoryGuard&) = delete;
  HeliosNetworkMemoryGuard& operator=(const HeliosNetworkMemoryGuard&) = delete;

 private:
  bool locked_;
  bool balanceWork_;
};

// Cooperatively give foreground rendering the helper's CPU time. A counter
// keeps nested drawing scopes paused until the outermost draw completes.
bool heliosDisplayBusy();
class HeliosDisplayWorkGuard {
 public:
  HeliosDisplayWorkGuard();
  ~HeliosDisplayWorkGuard();
  HeliosDisplayWorkGuard(const HeliosDisplayWorkGuard&) = delete;
  HeliosDisplayWorkGuard& operator=(const HeliosDisplayWorkGuard&) = delete;
};
