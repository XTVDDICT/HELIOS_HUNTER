#include "HeliosDiagnostics.h"

#include <esp_attr.h>
#include <esp_system.h>

namespace {
constexpr uint32_t TRACE_MAGIC = 0x58545731U;
constexpr uint32_t TRACE_VERSION = 1;
struct RtcTrace {
  uint32_t magic;
  uint32_t version;
  HeliosWatchdogTrace trace;
};
// No initializer: startup must not overwrite the previous boot's checkpoints.
RTC_NOINIT_ATTR volatile RtcTrace retained;
HeliosWatchdogTrace previous = {};
bool previousAvailable = false;
}

HeliosWatchdogTrace heliosCurrentTrace() {
  // Each task owns its own fields; aligned 32-bit reads/writes are atomic.
  // A live read may straddle a checkpoint, so timestamps are approximate.
  return {retained.trace.uiStage, retained.trace.uiAtMs,
          retained.trace.balanceStage, retained.trace.balanceAtMs,
          retained.trace.balanceCoin};
}

void heliosDiagnosticsBegin() {
  const esp_reset_reason_t reason = esp_reset_reason();
  const bool abnormalReset = reason == ESP_RST_TASK_WDT || reason == ESP_RST_INT_WDT ||
                             reason == ESP_RST_WDT || reason == ESP_RST_PANIC;
  previousAvailable = abnormalReset && retained.magic == TRACE_MAGIC &&
      retained.version == TRACE_VERSION &&
      retained.trace.uiStage <= static_cast<uint32_t>(HeliosUiStage::Waiting) &&
      retained.trace.balanceStage <= static_cast<uint32_t>(HeliosBalanceStage::Storage);
  if (previousAvailable) previous = heliosCurrentTrace();
  retained.magic = 0;
  retained.version = TRACE_VERSION;
  retained.trace.uiStage = static_cast<uint32_t>(HeliosUiStage::Starting);
  retained.trace.uiAtMs = millis();
  retained.trace.balanceStage = static_cast<uint32_t>(HeliosBalanceStage::Starting);
  retained.trace.balanceAtMs = millis();
  retained.trace.balanceCoin = UINT32_MAX;
  retained.magic = TRACE_MAGIC;
}

void heliosMarkUi(HeliosUiStage stage) {
  retained.trace.uiStage = static_cast<uint32_t>(stage);
  retained.trace.uiAtMs = millis();
}

void heliosMarkBalance(HeliosBalanceStage stage) {
  retained.trace.balanceStage = static_cast<uint32_t>(stage);
  retained.trace.balanceAtMs = millis();
}

void heliosMarkBalanceCoin(uint32_t coin) {
  retained.trace.balanceCoin = coin;
  heliosMarkBalance(HeliosBalanceStage::Coin);
}

bool heliosPreviousWatchdogTrace(HeliosWatchdogTrace& trace) {
  if (!previousAvailable) return false;
  trace = previous;
  return true;
}

const char* heliosUiStageText(uint32_t stage) {
  switch (static_cast<HeliosUiStage>(stage)) {
    case HeliosUiStage::Starting: return "STARTING";
    case HeliosUiStage::Wifi: return "WIFI";
    case HeliosUiStage::Web: return "WEB";
    case HeliosUiStage::Display: return "DISPLAY";
    case HeliosUiStage::Waiting: return "WAITING";
  }
  return "UNKNOWN";
}

const char* heliosBalanceStageText(uint32_t stage) {
  switch (static_cast<HeliosBalanceStage>(stage)) {
    case HeliosBalanceStage::Starting: return "STARTING";
    case HeliosBalanceStage::Waiting: return "WAITING";
    case HeliosBalanceStage::Coin: return "COIN";
    case HeliosBalanceStage::HttpRequest: return "HTTP REQUEST";
    case HeliosBalanceStage::HttpBody: return "HTTP BODY";
    case HeliosBalanceStage::Electrum: return "ELECTRUM";
    case HeliosBalanceStage::Parsing: return "PARSING";
    case HeliosBalanceStage::Storage: return "STORAGE";
  }
  return "UNKNOWN";
}
