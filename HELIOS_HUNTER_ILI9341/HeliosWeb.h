#pragma once

#include <Arduino.h>
#include <WebServer.h>

#include "HeliosCoins.h"
#include "HeliosBalances.h"
#include "HeliosSettings.h"

using HeliosSettingsCallback = void (*)();

class HeliosWeb {
 public:
  void begin(HeliosSettings* settings, HeliosBalances* balances,
             HeliosSettingsCallback settingsCallback);
  void loop();

 private:
  void handleStatus();
  void handleWallet();
  void handleMining();
  void handleMiningSettings();
  void handleDevice();
  void sendOk();
  void sendError(int status, const String& message);

  WebServer server_{80};
  HeliosSettings* settings_ = nullptr;
  HeliosBalances* balances_ = nullptr;
  HeliosSettingsCallback settingsCallback_ = nullptr;
};
