#pragma once

#include <Arduino.h>
#include <LovyanGFX.hpp>

#include "HeliosBalances.h"
#include "HeliosCoins.h"
#include "HeliosMiner.h"
#include "HeliosSettings.h"

class HeliosDisplay {
 public:
  void begin(HeliosSettings* settings, HeliosBalances* balances);
  void loop();
  void showHome();
  void requestRedraw();
  uint8_t page() const;

 private:
  void draw(bool force = false);
  void drawHome(const HeliosMiningStats& stats);
  void drawHomeValues(const HeliosMiningStats& stats);
  void drawCoin(const HeliosCoinProfile& profile,
                const HeliosMiningStats& stats);
  void drawCoinValues(const HeliosCoinProfile& profile,
                      const HeliosMiningStats& stats);
  void checkBalanceIncreases();
  void drawBlockPopup();
  void clearBlockPopup();
  void setRearLed(uint16_t color);
  void updateRearLed();
  void drawFooter(uint16_t accent);
  void handleTouch();
  String rateText(float kh) const;
  String balanceText(const HeliosCoinProfile& profile) const;
  String fiatText(double value) const;

  HeliosSettings* settings_ = nullptr;
  HeliosBalances* balances_ = nullptr;
  uint8_t page_ = 0;
  uint32_t lastDrawAt_ = 0;
  uint32_t lastTouchPollAt_ = 0;
  uint8_t appliedBrightness_ = 0;
  bool appliedFlipped_ = false;
  bool redrawRequested_ = false;
  bool touching_ = false;
  bool blockPopupActive_ = false;
  HeliosCoin blockPopupCoin_ = HeliosCoin::CHTA;
  double blockPopupAmount_ = 0.0;
  uint32_t seenIncreaseSequence_[HELIOS_COIN_COUNT] = {0};
  int32_t touchStartX_ = 0;
  int32_t touchStartY_ = 0;
  int32_t touchLastX_ = 0;
  int32_t touchLastY_ = 0;
};
