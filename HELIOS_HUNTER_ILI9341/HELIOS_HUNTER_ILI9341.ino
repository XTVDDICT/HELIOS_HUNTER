#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <esp_pm.h>
#include <esp_system.h>
#include <esp_wifi.h>

#include "HeliosCoins.h"
#include "HeliosDisplay.h"
#include "HeliosMiner.h"
#include "HeliosSettings.h"
#include "HeliosWeb.h"

namespace {

HeliosSettings settings;
HeliosBalances balances;
HeliosDisplay screen;
HeliosWeb web;
WiFiManager wifiManager;
esp_pm_lock_handle_t miningPowerLock = nullptr;
esp_pm_lock_handle_t cpuFrequencyLock = nullptr;
TaskHandle_t foregroundTaskHandle = nullptr;
bool networkServicesStarted = false;
uint32_t lastWifiRetryAt = 0;
uint8_t wifiRetryCount = 0;

enum class WifiCredentialState : uint8_t {
  Present,
  Missing,
  Unavailable,
};

WifiCredentialState readWifiCredentialState();
void startSetupPortal();
void retrySavedWifi();

WifiCredentialState wifiCredentialState = WifiCredentialState::Unavailable;

constexpr uint32_t WIFI_RETRY_INTERVAL_MS = 10000U;
constexpr uint8_t WIFI_RECONNECTS_BEFORE_RESTART = 6;
constexpr char BUILD_ID[] = "R20_XEC_20261008";

HeliosMiningConfig miningConfig() {
  const HeliosSettingsData& saved = settings.data();

  HeliosMiningConfig config;
  config.enabled = saved.miningEnabled && !saved.miningUsername.isEmpty();
  config.poolHost = saved.miningPoolHost;
  config.poolPort = saved.miningPoolPort;
  config.username = saved.miningUsername;
  config.worker = saved.worker;
  config.password = saved.stratumPassword;
  return config;
}

void applySettings() {
  heliosMinerConfigure(miningConfig());
  screen.requestRedraw();
}

WifiCredentialState readWifiCredentialState() {
  wifi_config_t config = {};
  esp_err_t result = esp_wifi_get_config(WIFI_IF_STA, &config);
  if (result != ESP_OK) {
    Serial.printf("Saved WiFi check unavailable: %d\n",
                  static_cast<int>(result));
    return WifiCredentialState::Unavailable;
  }
  return config.sta.ssid[0] == '\0' ? WifiCredentialState::Missing
                                    : WifiCredentialState::Present;
}

void startSetupPortal() {
  if (wifiManager.getConfigPortalActive()) return;
  Serial.println("No saved WiFi credentials; starting setup portal");
  wifiManager.startConfigPortal("HELIOS_HUNTER_SETUP");
}

void retrySavedWifi() {
  WifiCredentialState current = readWifiCredentialState();
  if (current == WifiCredentialState::Missing) {
    wifiCredentialState = current;
    startSetupPortal();
    return;
  }
  if (current == WifiCredentialState::Present) {
    wifiCredentialState = current;
  }

  ++wifiRetryCount;
  if (wifiRetryCount >= WIFI_RECONNECTS_BEFORE_RESTART) {
    wifiRetryCount = 0;
    Serial.println("WiFi retry: restarting station connection");
    WiFi.begin();
  } else {
    Serial.println("WiFi retry: reconnecting with saved credentials");
    WiFi.reconnect();
  }
}

void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);
  WiFi.setHostname("helios-hunter");
  wifiManager.setConfigPortalBlocking(false);
  wifiManager.setConnectTimeout(10);
  // Never let a connection timeout automatically create an AP. The portal is
  // started explicitly only when ESP32 confirms that no station SSID is saved.
  wifiManager.setEnableConfigPortal(false);
  wifiManager.setTitle("HELIOS_HUNTER Setup");
  wifiManager.setAPCallback([](WiFiManager*) {
    Serial.print("Setup AP ready: http://");
    Serial.println(WiFi.softAPIP());
    screen.showHome();
  });
  wifiCredentialState = readWifiCredentialState();
  if (wifiCredentialState == WifiCredentialState::Missing) {
    startSetupPortal();
    return;
  }

  WiFi.begin();
  uint32_t startedAt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startedAt < 15000U) {
    delay(100);
  }
  if (WiFi.status() == WL_CONNECTED) {
    wifiCredentialState = WifiCredentialState::Present;
    Serial.print("WiFi connected: ");
    Serial.println(WiFi.localIP());
    return;
  }
  Serial.println("Saved WiFi unavailable; remaining in station retry mode");
  lastWifiRetryAt = millis();
}

void startNetworkServices() {
  if (networkServicesStarted || WiFi.status() != WL_CONNECTED ||
      wifiManager.getConfigPortalActive()) {
    return;
  }
  web.begin(&settings, &balances, applySettings);
  networkServicesStarted = true;
  balances.requestRefresh();
  screen.requestRedraw();
  Serial.print("Web UI: http://");
  Serial.println(WiFi.localIP());
}

void serviceForeground() {
  wifiManager.process();
  bool wifiConnected = WiFi.status() == WL_CONNECTED;
  if (wifiConnected) {
    wifiCredentialState = WifiCredentialState::Present;
    wifiRetryCount = 0;
  } else if (!wifiManager.getConfigPortalActive() &&
             millis() - lastWifiRetryAt >= WIFI_RETRY_INTERVAL_MS) {
    lastWifiRetryAt = millis();
    retrySavedWifi();
  }
  startNetworkServices();
  if (networkServicesStarted) web.loop();
  screen.loop();
}

void foregroundTask(void*) {
  for (;;) {
    serviceForeground();
    vTaskDelay(pdMS_TO_TICKS(2));
  }
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.printf("Build: %s\n", BUILD_ID);
  Serial.printf("Last reset reason: %d\n", static_cast<int>(esp_reset_reason()));
  settings.begin();
  screen.begin(&settings, &balances);
  if (esp_pm_lock_create(ESP_PM_NO_LIGHT_SLEEP, 0, "helios-miner",
                         &miningPowerLock) == ESP_OK) {
    esp_pm_lock_acquire(miningPowerLock);
  }
  if (esp_pm_lock_create(ESP_PM_CPU_FREQ_MAX, 0, "helios-cpu",
                         &cpuFrequencyLock) == ESP_OK) {
    esp_pm_lock_acquire(cpuFrequencyLock);
  }
  heliosMinerBegin(miningConfig());
  balances.begin(&settings);
  connectWifi();
  startNetworkServices();
  screen.showHome();

  BaseType_t created = xTaskCreatePinnedToCore(
      foregroundTask, "helios-ui", 12288, nullptr, 2,
      &foregroundTaskHandle, 0);
  if (created != pdPASS) foregroundTaskHandle = nullptr;
  Serial.println();
  Serial.printf("CPU: %u MHz\n", getCpuFrequencyMhz());
  Serial.println("HELIOS_HUNTER ready");
}

void loop() {
  if (foregroundTaskHandle == nullptr) {
    serviceForeground();
    delay(2);
  } else {
    delay(1000);
  }
}
