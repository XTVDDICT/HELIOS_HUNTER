#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <esp_pm.h>
#include <esp_system.h>

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

void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.setHostname("helios-hunter");
  wifiManager.setConfigPortalBlocking(false);
  wifiManager.setConnectTimeout(10);
  wifiManager.setTitle("HELIOS_HUNTER Setup");
  if (wifiManager.getWiFiIsSaved()) {
    WiFi.begin();
    uint32_t startedAt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startedAt < 15000U) {
      delay(100);
    }
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Saved WiFi unavailable; retrying without setup mode");
    }
    return;
  }
  wifiManager.autoConnect("HELIOS_HUNTER_SETUP");
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
  if (WiFi.status() != WL_CONNECTED &&
      !wifiManager.getConfigPortalActive() &&
      millis() - lastWifiRetryAt >= 10000U) {
    lastWifiRetryAt = millis();
    WiFi.reconnect();
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
