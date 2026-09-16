#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include "HeliosCoins.h"
#include "HeliosSettings.h"

struct HeliosBalanceSnapshot {
  bool available = false;
  bool pricesAvailable = false;
  bool refreshing = false;
  bool baselineReady = false;
  double balance = 0.0;
  double priceUsd = 0.0;
  double priceCad = 0.0;
  double priceGbp = 0.0;
  uint32_t pricesUpdatedAt = 0;
  double increaseAmount = 0.0;
  uint32_t increaseSequence = 0;
  uint32_t updatedAt = 0;
  String status = "WAITING";
};

struct HeliosBalanceFetchState {
  bool enabled = true;
  bool active = false;
  uint32_t sinceMs = 0;
};

class HeliosBalances {
 public:
  void begin(HeliosSettings* settings);
  bool setFetchEnabled(bool enabled);
  HeliosBalanceFetchState fetchState() const;
  void requestRefresh();
  void requestRefresh(HeliosCoin coin);
  void acknowledgeIncrease(HeliosCoin coin, uint32_t sequence);
  HeliosBalanceSnapshot get(HeliosCoin coin) const;

 private:
  static void taskEntry(void* argument);
  void taskLoop();
  void syncWallets();
  void refreshCoin(HeliosCoin coin, const String& wallet);
  bool fetchBalance(HeliosCoin coin, const String& wallet, double& balance,
                    String& error);
  bool fetchPrices(HeliosCoin coin, double& usd, double& cad, double& gbp,
                   String& error);

  HeliosSettings* settings_ = nullptr;
  mutable SemaphoreHandle_t mutex_ = nullptr;
  TaskHandle_t task_ = nullptr;
  String wallets_[HELIOS_COIN_COUNT];
  HeliosBalanceSnapshot snapshots_[HELIOS_COIN_COUNT];
  uint8_t pendingMask_ = 0;
  uint32_t lastSweepAt_ = 0;
  uint32_t increaseSequence_ = 0;
  bool fetchEnabled_ = true;
  bool fetchActive_ = false;
  uint32_t fetchStateSinceMs_ = 0;
};
