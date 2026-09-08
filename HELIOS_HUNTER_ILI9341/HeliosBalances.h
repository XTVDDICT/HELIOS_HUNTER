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

class HeliosBalances {
 public:
  void begin(HeliosSettings* settings);
  void requestRefresh();
  void requestRefresh(HeliosCoin coin);
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
};
