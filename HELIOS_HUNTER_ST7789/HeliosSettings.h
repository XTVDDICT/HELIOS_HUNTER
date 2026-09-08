#pragma once

#include <Arduino.h>

#include "HeliosCoins.h"

struct HeliosSettingsData {
  String wallets[HELIOS_COIN_COUNT];
  String miningPoolHost = "chta.heliospool.com";
  uint16_t miningPoolPort = 3336;
  String miningUsername;
  String worker = "helios-hunter";
  String stratumPassword = "x";
  bool miningEnabled = false;
  bool flipped = false;
  uint8_t brightness = 220;
  uint8_t fiatCurrency = 0;
};

class HeliosSettings {
 public:
  void begin();
  const HeliosSettingsData& data() const;
  String wallet(HeliosCoin coin) const;
  void setWallet(HeliosCoin coin, String value);
  void setMining(String host, uint16_t port, String username, String worker,
                 String password, bool enabled);
  void setMiningEnabled(bool enabled);
  void setFlipped(bool flipped);
  void setBrightness(uint8_t brightness);
  void setFiatCurrency(uint8_t currency);

 private:
  void saveText(const char* key, const String& value);
  HeliosSettingsData data_;
};
