#include "HeliosWeb.h"

#include <ArduinoJson.h>
#include <WiFi.h>
#include <esp_system.h>
#include <freertos/task.h>
#include <string.h>

#include "HeliosMiner.h"
#include "HeliosWebPageGzip.h"
#include "SoloHunterSha256.h"

namespace {

constexpr char FIRMWARE_BUILD[] = "R20_XEC_20261008";

class BufferedNetworkWriter : public Print {
 public:
  explicit BufferedNetworkWriter(NetworkClient& client) : client_(client) {}

  size_t write(uint8_t value) override {
    if (used_ == sizeof(buffer_) && !flushBuffer()) return 0;
    buffer_[used_++] = value;
    return 1;
  }

  size_t write(const uint8_t* data, size_t size) override {
    size_t accepted = 0;
    while (accepted < size) {
      if (used_ == sizeof(buffer_) && !flushBuffer()) break;
      size_t available = sizeof(buffer_) - used_;
      size_t count = min(available, size - accepted);
      memcpy(buffer_ + used_, data + accepted, count);
      used_ += count;
      accepted += count;
    }
    return accepted;
  }

  bool flushBuffer() {
    size_t offset = 0;
    while (offset < used_) {
      size_t written = client_.write(buffer_ + offset, used_ - offset);
      if (written == 0) {
        used_ = 0;
        return false;
      }
      offset += written;
    }
    used_ = 0;
    return true;
  }

 private:
  NetworkClient& client_;
  uint8_t buffer_[512];
  size_t used_ = 0;
};

String diagnosticHex(const uint8_t* bytes, size_t length) {
  if (length > 80) return String();
  static const char digits[] = "0123456789abcdef";
  char buffer[161];
  for (size_t i = 0; i < length; ++i) {
    buffer[2 * i] = digits[bytes[i] >> 4];
    buffer[2 * i + 1] = digits[bytes[i] & 15];
  }
  buffer[2 * length] = 0;
  return String(buffer);
}

const char* resetReasonText() {
  switch (esp_reset_reason()) {
    case ESP_RST_POWERON: return "POWER ON";
    case ESP_RST_EXT: return "EXTERNAL RESET";
    case ESP_RST_SW: return "SOFTWARE RESET";
    case ESP_RST_PANIC: return "CRASH / PANIC";
    case ESP_RST_INT_WDT: return "INTERRUPT WATCHDOG";
    case ESP_RST_TASK_WDT: return "TASK WATCHDOG";
    case ESP_RST_WDT: return "WATCHDOG";
    case ESP_RST_DEEPSLEEP: return "DEEP SLEEP";
    case ESP_RST_BROWNOUT: return "LOW VOLTAGE";
    case ESP_RST_SDIO: return "SDIO";
    default: return "UNKNOWN";
  }
}


bool validCredential(const String& value) {
  if (value.length() > 128) return false;
  for (size_t i = 0; i < value.length(); ++i) {
    unsigned char c = static_cast<unsigned char>(value[i]);
    if (c < 33 || c > 126) return false;
  }
  return true;
}

void writeResponseHeader(NetworkClient& client, int status,
                         const __FlashStringHelper* contentType,
                         size_t contentLength, bool gzip = false) {
  switch (status) {
    case 200:
      client.print(F("HTTP/1.1 200 OK\r\n"));
      break;
    case 400:
      client.print(F("HTTP/1.1 400 Bad Request\r\n"));
      break;
    case 404:
      client.print(F("HTTP/1.1 404 Not Found\r\n"));
      break;
    case 503:
      client.print(F("HTTP/1.1 503 Service Unavailable\r\n"));
      break;
    default:
      client.print(F("HTTP/1.1 500 Internal Server Error\r\n"));
      break;
  }
  client.print(F("Content-Type: "));
  client.print(contentType);
  client.print(F("\r\nContent-Length: "));
  client.print(contentLength);
  if (gzip) client.print(F("\r\nContent-Encoding: gzip"));
  client.print(F("\r\nCache-Control: no-store\r\nConnection: close\r\n\r\n"));
}

void sendJsonDocument(WebServer& server, JsonDocument& document) {
  NetworkClient& client = server.client();
  writeResponseHeader(client, 200, F("application/json"),
                      measureJson(document));
  BufferedNetworkWriter writer(client);
  serializeJson(document, writer);
  writer.flushBuffer();
}

}  // namespace

void HeliosWeb::begin(HeliosSettings* settings, HeliosBalances* balances,
                      HeliosSettingsCallback settingsCallback) {
  settings_ = settings;
  balances_ = balances;
  settingsCallback_ = settingsCallback;
  server_.on("/", HTTP_GET, [this]() {
    NetworkClient& client = server_.client();
    writeResponseHeader(client, 200, F("text/html; charset=utf-8"),
                        HELIOS_WEB_PAGE_GZIP_SIZE, true);
    client.write(HELIOS_WEB_PAGE_GZIP, HELIOS_WEB_PAGE_GZIP_SIZE);
  });
  server_.on("/api/status", HTTP_GET, [this]() { handleStatus(); });
  server_.on("/api/diagnostics/sha-failure", HTTP_GET, [this]() {
    SoloHunterSha256Failure failure;
    JsonDocument document;
    const bool captured = soloHunterSha256GetFailure(failure);
    document["schema"] = 1;
    document["driver"] = "REFERENCE_NATIVE_11";
    document["captured"] = captured;
    if (captured) {
      document["reason"] = failure.reason;
      document["timing"] = failure.timing;
      document["uptimeMs"] = failure.uptimeMs;
      document["seed"] = failure.seed;
      document["startNonceSwapped"] = failure.startNonceSwapped;
      document["nextNonceSwapped"] = failure.nextNonceSwapped;
      document["nonce"] = failure.nonce;
      document["hashes"] = failure.hashes;
      document["candidate"] = failure.candidate;
      document["softwareReferenceOk"] = failure.softwareReferenceOk;
      document["rereadValid"] = failure.rereadValid;
      document["hardwareHeader"] = diagnosticHex(failure.hardwareHeader, 80);
      document["referenceHeader"] = diagnosticHex(failure.referenceHeader, 80);
      document["hardwareHash"] = diagnosticHex(failure.hardwareHash, 32);
      document["referenceHash"] = diagnosticHex(failure.referenceHash, 32);
      document["softwareHash"] = diagnosticHex(failure.softwareHash, 32);
      document["rereadHash"] = diagnosticHex(failure.rereadHash, 32);
    }
    sendJsonDocument(server_, document);
  });
  server_.on("/api/diagnostics/balance-fetch", HTTP_POST, [this]() {
    String enabled = server_.arg("enabled");
    if (enabled != "0" && enabled != "1") {
      sendError(400, "enabled must be 0 or 1");
      return;
    }
    if (!balances_->setFetchEnabled(enabled == "1")) {
      sendError(503, "Balance worker unavailable");
      return;
    }
    sendOk();
  });
  server_.on("/api/wallet", HTTP_POST, [this]() { handleWallet(); });
  server_.on("/api/mining", HTTP_POST, [this]() { handleMining(); });
  server_.on("/api/mining-settings", HTTP_POST, [this]() { handleMiningSettings(); });
  server_.on("/api/device", HTTP_POST, [this]() { handleDevice(); });
  server_.onNotFound([this]() { sendError(404, "Not found"); });
  server_.begin();
}

void HeliosWeb::loop() { server_.handleClient(); }

void HeliosWeb::handleStatus() {
  const bool compact =
      server_.hasArg("compact") && server_.arg("compact") == "1";
  const HeliosSettingsData& cfg = settings_->data();
  HeliosMiningStats stats = heliosMinerGetStats(true);
  const HeliosBalanceFetchState fetch = balances_->fetchState(true);
  JsonDocument document;
  document["wifi"] = WiFi.status() == WL_CONNECTED;
  document["build"] = FIRMWARE_BUILD;
  document["ip"] = WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() : String("--");
  document["status"] = stats.status;
  document["lastReset"] = resetReasonText();
  document["deviceUptimeSeconds"] = millis() / 1000U;
  document["minFreeHeap"] = ESP.getMinFreeHeap();
  document["miningEnabled"] = cfg.miningEnabled;
  document["poolHost"] = cfg.miningPoolHost;
  document["poolPort"] = cfg.miningPoolPort;
  document["pool"] = cfg.miningPoolHost + ":" + cfg.miningPoolPort;
  document["username"] = cfg.miningUsername;
  document["hashrateKh"] = stats.hashrateKh;
  document["primaryHashrateKh"] = stats.primaryHashrateKh;
  document["auxiliaryHashrateKh"] = stats.auxiliaryHashrateKh;
  document["accepted"] = stats.acceptedShares;
  document["rejected"] = stats.rejectedShares;
  document["blocks"] = stats.blocksFound;
  document["poolReconnects"] = stats.poolReconnects;
  document["bestDifficulty"] = stats.bestDifficulty;
  document["hardwareSha"] = stats.hardwareSha;
  document["fastSha"] = soloHunterSha256FastPathActive();
  document["uiStackFreeBytes"] =
      static_cast<uint32_t>(uxTaskGetStackHighWaterMark(nullptr));
  document["primaryStackFreeBytes"] = stats.primaryStackFreeBytes;
  document["helperStackFreeBytes"] = stats.auxiliaryStackFreeBytes;
  document["balanceStackFreeBytes"] = fetch.stackFreeBytes;
  document["worker"] = cfg.worker;
  document["password"] = cfg.stratumPassword;
  document["brightness"] = cfg.brightness;
  document["rearLedEnabled"] = cfg.rearLedEnabled;
  document["screenSleepSeconds"] = cfg.screenSleepSeconds;
  document["flipped"] = cfg.flipped;
  document["currency"] = cfg.fiatCurrency == 1 ? "CAD" : cfg.fiatCurrency == 2 ? "GBP" : "USD";
  if (!compact) {
    document["freeHeap"] = ESP.getFreeHeap();
    document["cpuMhz"] = getCpuFrequencyMhz();
    document["totalHashes"] = stats.totalHashes;
    document["submitted"] = stats.submittedShares;
    document["poolDifficulty"] = stats.poolDifficulty;
    document["shaDriver"] = "REFERENCE_NATIVE_11";
    document["shaChipRevision"] = soloHunterSha256ChipRevision();
    document["shaTiming"] = soloHunterSha256Timing();
    document["shaReferenceCheck"] = soloHunterSha256ReferenceValidation();
    document["balanceScheduling"] = "IDLE_PRIORITY_1";
    document["balanceFetchEnabled"] = fetch.enabled;
    document["balanceFetchActive"] = fetch.active;
    document["balanceFetchStateSinceMs"] = fetch.sinceMs;
    document["balanceFetchState"] = fetch.enabled
        ? (fetch.active ? "FETCHING" : "IDLE")
        : (fetch.active ? "PAUSING" : "PAUSED");
    document["shaLastFault"] = soloHunterSha256LastFault();
    document["shaLastFallback"] = soloHunterSha256LastFallback();
    document["shaLastSelfTestFailure"] = soloHunterSha256LastSelfTestFailure();
    document["shaRecoveries"] = soloHunterSha256RecoveryCount();
  }
  JsonArray coins = document["coins"].to<JsonArray>();
  for (size_t i = 0; i < HELIOS_COIN_COUNT; ++i) {
    const HeliosCoinProfile& profile = heliosCoinProfileAt(i);
    HeliosBalanceSnapshot balance = balances_->get(profile.id);
    JsonObject coin = coins.add<JsonObject>();
    coin["symbol"] = profile.symbol;
    coin["name"] = profile.name;
    coin["wallet"] = cfg.wallets[i];
    coin["balanceAvailable"] = balance.available;
    coin["balance"] = balance.balance;
    coin["balanceStatus"] = balance.status;
    coin["pricesAvailable"] = balance.pricesAvailable;
    coin["value"] = balance.balance *
        (cfg.fiatCurrency == 1 ? balance.priceCad
         : cfg.fiatCurrency == 2 ? balance.priceGbp
                                 : balance.priceUsd);
    if (!compact) {
      coin["configured"] = !cfg.wallets[i].isEmpty();
      coin["priceUsd"] = balance.priceUsd;
      coin["priceCad"] = balance.priceCad;
      coin["priceGbp"] = balance.priceGbp;
      coin["valueUsd"] = balance.balance * balance.priceUsd;
      coin["valueCad"] = balance.balance * balance.priceCad;
      coin["valueGbp"] = balance.balance * balance.priceGbp;
    }
  }
  sendJsonDocument(server_, document);
}

void HeliosWeb::handleWallet() {
  HeliosCoin coin;
  if (!server_.hasArg("coin") || !heliosCoinFromSymbol(server_.arg("coin"), coin)) { sendError(400, "Unknown coin"); return; }
  String wallet = server_.arg("wallet");
  wallet.trim();
  if (!wallet.isEmpty() && !validCredential(wallet)) { sendError(400, "Wallet contains invalid characters or is too long"); return; }
  settings_->setWallet(coin, wallet);
  balances_->requestRefresh(coin);
  sendOk();
}

void HeliosWeb::handleMining() {
  bool enabled = server_.arg("enabled") == "1";
  if (enabled && settings_->data().miningUsername.isEmpty()) { sendError(400, "Enter a wallet or pool username first"); return; }
  settings_->setMiningEnabled(enabled);
  if (settingsCallback_) settingsCallback_();
  sendOk();
}

void HeliosWeb::handleMiningSettings() {
  bool enabled = server_.arg("enabled") == "1";
  String host = server_.arg("host");
  String username = server_.arg("username");
  host.trim();
  username.trim();
  long portValue = server_.arg("port").toInt();
  if (host.isEmpty() || host.length() > 96 || host.indexOf(' ') >= 0) { sendError(400, "Enter a valid pool host"); return; }
  if (portValue < 1 || portValue > 65535) { sendError(400, "Pool port must be from 1 to 65535"); return; }
  if (!username.isEmpty() && !validCredential(username)) { sendError(400, "Pool username contains invalid characters or is too long"); return; }
  if (enabled && username.isEmpty()) { sendError(400, "Enter a wallet or pool username before enabling mining"); return; }
  settings_->setMining(host, static_cast<uint16_t>(portValue), username,
                       server_.arg("worker"), server_.arg("password"), enabled);
  if (settingsCallback_) settingsCallback_();
  sendOk();
}

void HeliosWeb::handleDevice() {
  if (server_.hasArg("brightness")) settings_->setBrightness(static_cast<uint8_t>(constrain(server_.arg("brightness").toInt(), 20, 255)));
  if (server_.hasArg("rearLed")) settings_->setRearLedEnabled(server_.arg("rearLed") == "1");
  if (server_.hasArg("screenSleep")) settings_->setScreenSleepSeconds(static_cast<uint16_t>(constrain(server_.arg("screenSleep").toInt(), 0, 1800)));
  if (server_.hasArg("flipped")) settings_->setFlipped(server_.arg("flipped") == "1");
  if (server_.hasArg("currency")) {
    String currency = server_.arg("currency");
    settings_->setFiatCurrency(currency == "CAD" ? 1 : currency == "GBP" ? 2 : 0);
  }
  sendOk();
}

void HeliosWeb::sendOk() {
  static constexpr char BODY[] = "{\"ok\":true}";
  NetworkClient& client = server_.client();
  writeResponseHeader(client, 200, F("application/json"), sizeof(BODY) - 1);
  client.write(reinterpret_cast<const uint8_t*>(BODY), sizeof(BODY) - 1);
}

void HeliosWeb::sendError(int status, const String& message) {
  NetworkClient& client = server_.client();
  writeResponseHeader(client, status, F("text/plain; charset=utf-8"),
                      message.length());
  client.print(message);
}
