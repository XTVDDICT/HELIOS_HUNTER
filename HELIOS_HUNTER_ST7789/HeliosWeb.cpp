#include "HeliosWeb.h"

#include <ArduinoJson.h>
#include <WiFi.h>
#include <esp_system.h>
#include <esp_app_desc.h>
#include <esp_core_dump.h>
#include <esp_idf_version.h>
#include <freertos/task.h>
#include <string.h>

#include "HeliosMiner.h"
#include "HeliosDiagnostics.h"
#include "HeliosNetworkMemory.h"
#include "HeliosWebPageGzip.h"
#include "SoloHunterSha256.h"

namespace {

constexpr char FIRMWARE_BUILD[] = "R23_GRLC_BALANCE_MINING_20261028";

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
      // A slow browser must leave CPU time for idle and the balance worker.
      vTaskDelay(1);
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
  if (document.overflowed()) {
    // Never publish a valid-looking partial list of coins after allocation
    // failure; the browser can keep its last successful snapshot and retry.
    // Release the failed document first. WebServer::send() allocates header
    // nodes with throwing new, so using it here caused the captured panic.
    document.clear();
    static constexpr char BODY[] =
        "{\"error\":\"Status memory unavailable; retry\"}";
    NetworkClient& client = server.client();
    writeResponseHeader(client, 503, F("application/json"), sizeof(BODY) - 1);
    client.write(reinterpret_cast<const uint8_t*>(BODY), sizeof(BODY) - 1);
    return;
  }
  NetworkClient& client = server.client();
  writeResponseHeader(client, 200, F("application/json"),
                      measureJson(document));
  BufferedNetworkWriter writer(client);
  serializeJson(document, writer);
  writer.flushBuffer();
}

void addWatchdogTrace(JsonObject object, const HeliosWatchdogTrace& trace) {
  object["uiStage"] = heliosUiStageText(trace.uiStage);
  object["uiAtMs"] = trace.uiAtMs;
  object["balanceStage"] = heliosBalanceStageText(trace.balanceStage);
  object["balanceAtMs"] = trace.balanceAtMs;
  if (trace.balanceCoin < HELIOS_COIN_COUNT) {
    object["balanceCoin"] = heliosCoinProfileAt(trace.balanceCoin).symbol;
  }
}

String addressHex(uint32_t address) {
  return String("0x") + String(address, HEX);
}

void addSavedCrash(JsonDocument& document) {
  document["schema"] = 1;
  document["build"] = FIRMWARE_BUILD;
  document["lastReset"] = resetReasonText();
  document["runningElfSha256"] =
      diagnosticHex(esp_app_get_description()->app_elf_sha256, 32);
  document["available"] = false;
#if CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH && CONFIG_ESP_COREDUMP_DATA_FORMAT_ELF && defined(CONFIG_IDF_TARGET_ESP32)
  size_t address = 0;
  size_t size = 0;
  esp_err_t result = esp_core_dump_image_get(&address, &size);
  document["imageStatus"] = esp_err_to_name(result);
  if (result != ESP_OK) return;
  document["imageBytes"] = size;
  // Never decode an incomplete/corrupt dump as a valid backtrace.
  result = esp_core_dump_image_check();
  document["integrityStatus"] = esp_err_to_name(result);
  if (result != ESP_OK) return;
  esp_core_dump_summary_t summary = {};
  result = esp_core_dump_get_summary(&summary);
  document["summaryStatus"] = esp_err_to_name(result);
  if (result != ESP_OK) return;
  summary.exc_task[sizeof(summary.exc_task) - 1] = '\0';
  summary.app_elf_sha256[sizeof(summary.app_elf_sha256) - 1] = '\0';
  document["available"] = true;
  // These buffers live only until this helper returns. Own the JSON strings
  // before sendJsonDocument serializes them in the caller.
  document["crashedTask"] = String(summary.exc_task);
  document["exceptionPC"] = addressHex(summary.exc_pc);
  document["exceptionCause"] = summary.ex_info.exc_cause;
  document["exceptionAddress"] = addressHex(summary.ex_info.exc_vaddr);
  document["crashedElfSha256"] =
      String(reinterpret_cast<const char*>(summary.app_elf_sha256));
  document["backtraceCorrupted"] = summary.exc_bt_info.corrupted;
  JsonArray backtrace = document["backtrace"].to<JsonArray>();
  const size_t capacity = sizeof(summary.exc_bt_info.bt) / sizeof(summary.exc_bt_info.bt[0]);
  const size_t depth = summary.exc_bt_info.depth < capacity
                          ? summary.exc_bt_info.depth : capacity;
  for (size_t i = 0; i < depth; ++i) {
    backtrace.add(addressHex(summary.exc_bt_info.bt[i]));
  }
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 5, 5)
  char reason[256] = {};
  result = esp_core_dump_get_panic_reason(reason, sizeof(reason));
  reason[sizeof(reason) - 1] = '\0';
  document["reasonStatus"] = esp_err_to_name(result);
  if (result == ESP_OK) document["panicReason"] = String(reason);
#endif
#else
  document["imageStatus"] = "FLASH_ELF_CORE_DUMP_UNSUPPORTED";
#endif
  // Read-only: keep the saved dump intact for a matching-ELF investigation.
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
    for (size_t offset = 0; offset < HELIOS_WEB_PAGE_GZIP_SIZE;) {
      const size_t remaining = HELIOS_WEB_PAGE_GZIP_SIZE - offset;
      const size_t count = remaining > 1024U ? 1024U : remaining;
      const size_t written = client.write(HELIOS_WEB_PAGE_GZIP + offset, count);
      if (written == 0) break;
      offset += written;
      vTaskDelay(1);
    }
  });
  server_.on("/api/status", HTTP_GET, [this]() { handleStatus(); });
  server_.on("/api/diagnostics/crash", HTTP_GET, [this]() {
    JsonDocument document;
    addSavedCrash(document);
    sendJsonDocument(server_, document);
  });
  server_.on("/api/diagnostics/watchdog", HTTP_GET, [this]() {
    JsonDocument document;
    document["build"] = FIRMWARE_BUILD;
    document["sdk"] = ESP.getSdkVersion();
    document["lastReset"] = resetReasonText();
    document["deviceUptimeSeconds"] = millis() / 1000U;
    document["freeHeap"] = ESP.getFreeHeap();
    document["minFreeHeap"] = ESP.getMinFreeHeap();
    document["largestFreeBlock"] = ESP.getMaxAllocHeap();
    document["uiPriority"] = uxTaskPriorityGet(nullptr);
    document["scheduling"] = "IDLE_PRIORITY_UI_1";
    HeliosWatchdogTrace previous;
    const bool previousAvailable = heliosPreviousWatchdogTrace(previous);
    document["previousAvailable"] = previousAvailable;
    if (previousAvailable) {
      addWatchdogTrace(document["previous"].to<JsonObject>(), previous);
    }
    addWatchdogTrace(document["current"].to<JsonObject>(), heliosCurrentTrace());
    sendJsonDocument(server_, document);
  });
  server_.on("/api/diagnostics/sha-failure", HTTP_GET, [this]() {
    SoloHunterSha256Failure failure;
    JsonDocument document;
    const bool captured = soloHunterSha256GetFailure(failure);
    document["schema"] = 1;
    document["driver"] = "PIPELINED_V2_R22";
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
  server_.on("/api/diagnostics/mining", HTTP_GET, [this]() {
    const SoloHunterMiningProfile p = soloHunterMinerGetProfile();
    JsonDocument document;
    document["build"] = FIRMWARE_BUILD;
    document["uptimeSeconds"] = millis() / 1000U;
    document["helperEnabled"] = p.helperEnabled;
    document["epoch"] = p.epoch;
    document["controlEpoch"] = p.controlEpoch;
    document["ready"] = p.epoch == p.controlEpoch && p.elapsedUs > 0;
    document["elapsedUs"] = p.elapsedUs;
    document["hardwareCallUs"] = p.hardwareCallUs;
    document["poolReadUs"] = p.poolReadUs;
    document["poolReadMode"] = "CHUNKED_256";
    document["poolReadCalls"] = p.poolReadCalls;
    document["poolReadBytes"] = p.poolReadBytes;
    document["poolMessages"] = p.poolMessages;
    document["helperRunBudgetUs"] = p.helperRunBudgetUs;
    document["candidateUs"] = p.candidateUs;
    document["candidateVerifyUs"] = p.candidateVerifyUs;
    document["hardwareCandidates"] = p.hardwareCandidates;
    document["confirmedCandidates"] = p.confirmedCandidates;
    document["stratumWriteMode"] = "BUFFERED_256";
    document["shareSubmitUs"] = p.shareSubmitUs;
    document["shareSubmitCalls"] = p.shareSubmitCalls;
    document["shareSubmitFailures"] = p.shareSubmitFailures;
    document["shareWriteCalls"] = p.shareWriteCalls;
    document["shareBytes"] = p.shareBytes;
    const uint64_t accounted = p.hardwareCallUs + p.poolReadUs + p.candidateUs;
    document["otherUs"] = p.elapsedUs >= accounted ? p.elapsedUs - accounted : 0;
    document["hardwareCalls"] = p.hardwareCalls;
    document["hardwareHashes"] = p.hardwareHashes;
    document["softwareHashes"] = p.softwareHashes;
    document["helperHashes"] = p.helperHashes;
    document["shaTiming"] = soloHunterSha256Timing();
    sendJsonDocument(server_, document);
  });
  server_.on("/api/diagnostics/helper", HTTP_POST, [this]() {
    const bool changeEnabled = server_.hasArg("enabled");
    const bool changeBudget = server_.hasArg("runBudgetUs");
    const String enabled = server_.arg("enabled");
    const String budget = server_.arg("runBudgetUs");
    if ((!changeEnabled && !changeBudget) ||
        (changeEnabled && enabled != "0" && enabled != "1") ||
        (changeBudget && budget != "20000" && budget != "40000")) {
      sendError(400, "Use enabled=0/1 or runBudgetUs=20000/40000");
      return;
    }
    if ((changeBudget && !soloHunterMinerSetHelperRunBudgetUs(budget.toInt())) ||
        (changeEnabled && !soloHunterMinerSetHelperEnabled(enabled == "1"))) {
      sendError(503, "Helper task unavailable");
      return;
    }
    sendOk();
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
  server_.on("/api/coin-visibility", HTTP_POST, [this]() {
    HeliosCoin coin;
    const String visible = server_.arg("visible");
    if (!server_.hasArg("coin") ||
        !heliosCoinFromSymbol(server_.arg("coin"), coin) ||
        (visible != "0" && visible != "1")) {
      sendError(400, "Use a known coin and visible=0/1");
      return;
    }
    if (!settings_->setCoinVisible(coin, visible == "1")) {
      sendError(503, "Could not save device page setting");
      return;
    }
    if (settingsCallback_) settingsCallback_();
    sendOk();
  });
  server_.on("/api/wallet", HTTP_POST, [this]() { handleWallet(); });
  server_.on("/api/mining", HTTP_POST, [this]() { handleMining(); });
  server_.on("/api/mining-settings", HTTP_POST, [this]() { handleMiningSettings(); });
  server_.on("/api/device", HTTP_POST, [this]() { handleDevice(); });
  server_.onNotFound([this]() { sendError(404, "Not found"); });
  server_.begin();
}

void HeliosWeb::loop() {
  HeliosNetworkMemoryGuard memory(0);
  if (!memory) return;
  // WebServer also uses throwing new while parsing a request. Defer before
  // entering it when memory is scarce, rather than relying on a JSON check.
  if (ESP.getFreeHeap() < 16384U || ESP.getMaxAllocHeap() < 4096U) return;
  server_.handleClient();
}

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
  document["currency"] = cfg.fiatCurrency == 1 ? "CAD" : cfg.fiatCurrency == 2 ? "GBP" : cfg.fiatCurrency == 3 ? "EUR" : "USD";
  if (!compact) {
    document["helperMode"] = stats.auxiliaryMode;
    document["helperSliceYields"] = stats.auxiliarySliceYields;
    document["helperBalanceWaits"] = stats.auxiliaryBalanceWaits;
    document["helperPartialBatches"] = stats.auxiliaryPartialBatches;
    document["helperRunBudgetUs"] = stats.auxiliaryRunBudgetUs;
    document["helperIdleTicks"] = stats.auxiliaryIdleTicks;
    document["helperTaskPriority"] = stats.auxiliaryTaskPriority;
    document["freeHeap"] = ESP.getFreeHeap();
    document["largestFreeBlock"] = ESP.getMaxAllocHeap();
    document["uiScheduling"] = "IDLE_PRIORITY_UI_1";
    document["cpuMhz"] = getCpuFrequencyMhz();
    document["totalHashes"] = stats.totalHashes;
    document["submitted"] = stats.submittedShares;
    document["poolDifficulty"] = stats.poolDifficulty;
    document["shaDriver"] = "PIPELINED_V2_R22";
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
    coin["showOnDevice"] = settings_->coinVisible(profile.id);
    coin["balanceAvailable"] = balance.available;
    coin["balance"] = balance.balance;
    coin["balanceStatus"] = balance.status;
    coin["pricesAvailable"] = balance.pricesAvailable;
    coin["priceStatus"] = balance.priceStatus;
    coin["value"] = balance.balance *
        (cfg.fiatCurrency == 1 ? balance.priceCad
         : cfg.fiatCurrency == 3 ? balance.priceEur
         : cfg.fiatCurrency == 2 ? balance.priceGbp
                                 : balance.priceUsd);
    if (!compact) {
      coin["configured"] = !cfg.wallets[i].isEmpty();
      coin["priceUsd"] = balance.priceUsd;
      coin["priceCad"] = balance.priceCad;
      coin["priceGbp"] = balance.priceGbp;
      coin["priceEur"] = balance.priceEur;
      coin["valueUsd"] = balance.balance * balance.priceUsd;
      coin["valueCad"] = balance.balance * balance.priceCad;
      coin["valueGbp"] = balance.balance * balance.priceGbp;
      coin["valueEur"] = balance.balance * balance.priceEur;
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
    settings_->setFiatCurrency(currency == "CAD" ? 1 : currency == "GBP" ? 2 : currency == "EUR" ? 3 : 0);
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
