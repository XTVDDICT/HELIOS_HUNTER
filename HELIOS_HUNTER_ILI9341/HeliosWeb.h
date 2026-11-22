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
  bool takeWifiChange(String& ssid, String& password);
  void finishWifiChange(const char* message);
  void suspend() { server_.stop(); }
  void resume() { server_.begin(); }

 private:
  void handleStatus();
  void handleWallet();
  void handleMining();
  void handleMiningSettings();
  void handleDevice();
  void handleWifi();
  void sendOk();
  void sendError(int status, const String& message);

  String wifiSsid_;
  String wifiPassword_;
  String wifiMessage_;
  uint32_t wifiQueuedAt_ = 0;
  bool wifiQueued_ = false;
  bool wifiBusy_ = false;
  WebServer server_{80};
  HeliosSettings* settings_ = nullptr;
  HeliosBalances* balances_ = nullptr;
  HeliosSettingsCallback settingsCallback_ = nullptr;
};
