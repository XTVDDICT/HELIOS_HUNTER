#pragma once

#include <Arduino.h>

// Checkpoints survive watchdog/panic resets in RTC memory. They identify the last
// operation entered, not a substitute for a serial panic backtrace.
enum class HeliosUiStage : uint32_t { Starting, Wifi, Web, Display, Waiting };
enum class HeliosBalanceStage : uint32_t {
  Starting, Waiting, Coin, HttpRequest, HttpBody, Electrum, Parsing, Storage
};

struct HeliosWatchdogTrace {
  uint32_t uiStage;
  uint32_t uiAtMs;
  uint32_t balanceStage;
  uint32_t balanceAtMs;
  uint32_t balanceCoin;
};

void heliosDiagnosticsBegin();
void heliosMarkUi(HeliosUiStage stage);
void heliosMarkBalance(HeliosBalanceStage stage);
void heliosMarkBalanceCoin(uint32_t coin);
HeliosWatchdogTrace heliosCurrentTrace();
bool heliosPreviousWatchdogTrace(HeliosWatchdogTrace& trace);
const char* heliosUiStageText(uint32_t stage);
const char* heliosBalanceStageText(uint32_t stage);
