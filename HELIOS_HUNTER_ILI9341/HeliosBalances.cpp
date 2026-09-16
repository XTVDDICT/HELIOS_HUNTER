#include "HeliosBalances.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <esp_heap_caps.h>
#include <math.h>
#include <mbedtls/sha256.h>
#include <stdlib.h>
#include <string.h>

namespace {

constexpr uint32_t REFRESH_INTERVAL_MS = 5UL * 60UL * 1000UL;
constexpr uint32_t PRICE_CACHE_MS = 10UL * 60UL * 1000UL;
constexpr uint32_t REQUEST_TIMEOUT_MS = 12000;
constexpr uint8_t REQUEST_ATTEMPTS = 2;
constexpr uint32_t ELECTRUM_TIMEOUT_MS = 6000;
constexpr uint32_t BALANCE_NETWORK_SETTLE_MS = 20000;
constexpr uint32_t BETWEEN_COIN_REQUESTS_MS = 5000;
constexpr size_t MAX_HTTP_PAYLOAD_BYTES = 32768;
// Only suppress a request when the heap is critically low. TLS can operate
// with fragmented heap on these CYDs, and each response allocation is checked
// separately once its actual Content-Length is known.
constexpr size_t MIN_REQUEST_FREE_HEAP = 24000;
constexpr size_t MIN_REQUEST_LARGEST_BLOCK = 8192;
constexpr uint8_t ALL_COINS_MASK = (1U << HELIOS_COIN_COUNT) - 1U;
constexpr const char* BALANCE_STATE_NAMESPACE = "heliosbal";
constexpr const char* BALANCE_WALLET_KEYS[HELIOS_COIN_COUNT] = {
    "wallet0", "wallet1", "wallet2", "wallet3", "wallet4"};
constexpr const char* BALANCE_VALUE_KEYS[HELIOS_COIN_COUNT] = {
    "balance0", "balance1", "balance2", "balance3", "balance4"};
constexpr const char* PENDING_VALUE_KEYS[HELIOS_COIN_COUNT] = {
    "pending0", "pending1", "pending2", "pending3", "pending4"};
constexpr const char* PRICE_IDS[HELIOS_COIN_COUNT] = {
    "chta-cheetahcoin", "wjk-wojakcoin", "dgb-digibyte",
    "bch-bitcoin-cash", "btc-bitcoin"};

struct ElectrumServer {
  const char* host;
  uint16_t port;
};

bool loadStoredBalanceState(size_t index, const String& wallet,
                            double& balance, double& pendingIncrease) {
  balance = 0.0;
  pendingIncrease = 0.0;
  if (wallet.isEmpty()) return false;

  Preferences prefs;
  if (!prefs.begin(BALANCE_STATE_NAMESPACE, true)) return false;
  bool walletMatches =
      prefs.getString(BALANCE_WALLET_KEYS[index], "") == wallet;
  double storedBalance = prefs.getDouble(BALANCE_VALUE_KEYS[index], NAN);
  double storedPending = prefs.getDouble(PENDING_VALUE_KEYS[index], 0.0);
  prefs.end();

  if (!walletMatches || !isfinite(storedBalance) || storedBalance < 0.0) {
    return false;
  }
  balance = storedBalance;
  if (isfinite(storedPending) && storedPending > 0.0) {
    pendingIncrease = storedPending;
  }
  return true;
}

void saveStoredBalanceState(size_t index, const String& wallet,
                            double balance, double pendingIncrease) {
  if (wallet.isEmpty() || !isfinite(balance) || balance < 0.0) return;
  Preferences prefs;
  if (!prefs.begin(BALANCE_STATE_NAMESPACE, false)) return;
  prefs.putString(BALANCE_WALLET_KEYS[index], wallet);
  prefs.putDouble(BALANCE_VALUE_KEYS[index], balance);
  if (isfinite(pendingIncrease) && pendingIncrease > 0.0) {
    prefs.putDouble(PENDING_VALUE_KEYS[index], pendingIncrease);
  } else {
    prefs.remove(PENDING_VALUE_KEYS[index]);
  }
  prefs.end();
}

void clearStoredPendingIncrease(size_t index, const String& wallet) {
  if (wallet.isEmpty()) return;
  Preferences prefs;
  if (!prefs.begin(BALANCE_STATE_NAMESPACE, false)) return;
  if (prefs.getString(BALANCE_WALLET_KEYS[index], "") == wallet) {
    prefs.remove(PENDING_VALUE_KEYS[index]);
  }
  prefs.end();
}

constexpr ElectrumServer CHTA_ELECTRUM_SERVERS[] = {
    {"electrum.shorelinecrypto.com", 10007},
    {"electrum.blastinvest.com", 10007},
    {"electrum2.mooo.com", 10007}};

constexpr ElectrumServer BCH_ELECTRUM_SERVERS[] = {
    {"bch.electrum1.cipig.net", 10055},
    {"bch.electrum2.cipig.net", 10055},
    {"bch.electrum3.cipig.net", 10055}};

bool parseNumber(String payload, double& value) {
  payload.trim();
  if (payload.isEmpty() || payload.startsWith("ERROR")) return false;
  char* end = nullptr;
  value = strtod(payload.c_str(), &end);
  return end != payload.c_str() && *end == '\0' && isfinite(value) && value >= 0;
}

bool requestMemoryAvailable(String& error) {
  const size_t freeHeap = heap_caps_get_free_size(MALLOC_CAP_8BIT);
  const size_t largestBlock =
      heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
  if (freeHeap >= MIN_REQUEST_FREE_HEAP &&
      largestBlock >= MIN_REQUEST_LARGEST_BLOCK) {
    return true;
  }
  error = "LOW MEMORY";
  return false;
}

bool getPayloadOnce(const String& url, bool secure, String& payload,
                     String& error) {
  if (!requestMemoryAvailable(error)) return false;

  HTTPClient http;
  http.setConnectTimeout(8000);
  http.setTimeout(REQUEST_TIMEOUT_MS);
  http.setUserAgent("HELIOS_HUNTER/0.1");
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  http.setReuse(false);
  http.useHTTP10(true);

  WiFiClient client;
  WiFiClientSecure tls;
  bool started = false;
  if (secure) {
    tls.setInsecure();
    tls.setHandshakeTimeout(15);
    started = http.begin(tls, url);
  } else {
    started = http.begin(client, url);
  }
  if (!started) {
    error = "START FAILED";
    return false;
  }

  int code = http.GET();
  if (code != HTTP_CODE_OK) {
    error = code > 0 ? String("HTTP ") + code : String("NET ") + code;
    if (code < 0) {
      Serial.printf("Balance HTTP error %d: %s\n", code,
                    HTTPClient::errorToString(code).c_str());
    }
    http.end();
    return false;
  }
  const int responseSize = http.getSize();
  if (responseSize > static_cast<int>(MAX_HTTP_PAYLOAD_BYTES)) {
    error = "RESPONSE TOO LARGE";
    http.end();
    return false;
  }
  if (responseSize > 0 &&
      !payload.reserve(static_cast<size_t>(responseSize) + 1U)) {
    error = "LOW MEMORY";
    http.end();
    return false;
  }
  payload = http.getString();
  if (payload.length() > MAX_HTTP_PAYLOAD_BYTES) {
    payload = "";
    error = "RESPONSE TOO LARGE";
    http.end();
    return false;
  }
  http.end();
  return true;
}

bool getPayload(const String& url, bool secure, String& payload,
                String& error) {
  for (uint8_t attempt = 0; attempt < REQUEST_ATTEMPTS; ++attempt) {
    payload = "";
    if (getPayloadOnce(url, secure, payload, error)) return true;
    if (attempt + 1 < REQUEST_ATTEMPTS) vTaskDelay(pdMS_TO_TICKS(350));
  }
  return false;
}

bool parseElectrs(const String& payload, double& balance) {
  JsonDocument document;
  if (deserializeJson(document, payload)) return false;
  JsonObject chain = document["chain_stats"];
  JsonObject mempool = document["mempool_stats"];
  if (chain.isNull() || mempool.isNull()) return false;
  for (const char* key : {"funded_txo_sum", "spent_txo_sum"}) {
    if (!chain[key].is<int64_t>() || !mempool[key].is<int64_t>()) return false;
  }
  int64_t satoshis = chain["funded_txo_sum"].as<int64_t>() -
                     chain["spent_txo_sum"].as<int64_t>() +
                     mempool["funded_txo_sum"].as<int64_t>() -
                     mempool["spent_txo_sum"].as<int64_t>();
  balance = static_cast<double>(satoshis) / 100000000.0;
  return true;
}

bool parseHaskoin(const String& payload, double& balance) {
  JsonDocument document;
  if (deserializeJson(document, payload)) return false;
  if (!document["confirmed"].is<int64_t>() ||
      !document["unconfirmed"].is<int64_t>()) {
    return false;
  }
  int64_t satoshis = document["confirmed"].as<int64_t>() +
                     document["unconfirmed"].as<int64_t>();
  balance = static_cast<double>(satoshis) / 100000000.0;
  return true;
}

bool sha256Bytes(const uint8_t* data, size_t length, uint8_t output[32]) {
  mbedtls_sha256_context context;
  mbedtls_sha256_init(&context);
  bool ok = mbedtls_sha256_starts(&context, 0) == 0 &&
            mbedtls_sha256_update(&context, data, length) == 0 &&
            mbedtls_sha256_finish(&context, output) == 0;
  mbedtls_sha256_free(&context);
  return ok;
}

bool decodeBase58Address(const String& address, uint8_t& version,
                         uint8_t hash[20]) {
  static constexpr char ALPHABET[] =
      "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
  if (address.isEmpty() || address.length() > 64) return false;

  size_t zeroCount = 0;
  while (zeroCount < address.length() && address[zeroCount] == '1') {
    ++zeroCount;
  }

  uint8_t littleEndian[40] = {};
  size_t littleLength = 0;
  for (size_t i = zeroCount; i < address.length(); ++i) {
    const char* digit = strchr(ALPHABET, address[i]);
    if (!digit) return false;
    uint32_t carry = static_cast<uint32_t>(digit - ALPHABET);
    for (size_t j = 0; j < littleLength; ++j) {
      carry += static_cast<uint32_t>(littleEndian[j]) * 58U;
      littleEndian[j] = static_cast<uint8_t>(carry & 0xffU);
      carry >>= 8;
    }
    while (carry != 0) {
      if (littleLength >= sizeof(littleEndian)) return false;
      littleEndian[littleLength++] = static_cast<uint8_t>(carry & 0xffU);
      carry >>= 8;
    }
  }

  if (zeroCount + littleLength != 25) return false;
  uint8_t decoded[25] = {};
  for (size_t i = 0; i < littleLength; ++i) {
    decoded[zeroCount + i] = littleEndian[littleLength - 1U - i];
  }

  uint8_t firstHash[32];
  uint8_t checksum[32];
  if (!sha256Bytes(decoded, 21, firstHash) ||
      !sha256Bytes(firstHash, sizeof(firstHash), checksum) ||
      memcmp(checksum, decoded + 21, 4) != 0) {
    return false;
  }
  version = decoded[0];
  memcpy(hash, decoded + 1, 20);
  return true;
}

uint64_t cashaddrPolymodStep(uint64_t checksum, uint8_t value) {
  static constexpr uint64_t GENERATORS[5] = {
      0x98f2bc8e61ULL, 0x79b76d99e2ULL, 0xf33e5fb3c4ULL,
      0xae2eabe2a8ULL, 0x1e4f43e470ULL};
  uint8_t top = static_cast<uint8_t>(checksum >> 35);
  checksum = ((checksum & 0x07ffffffffULL) << 5) ^ value;
  for (uint8_t i = 0; i < 5; ++i) {
    if (top & (1U << i)) checksum ^= GENERATORS[i];
  }
  return checksum;
}

bool decodeCashAddress(const String& address, bool& scriptHash,
                       uint8_t hash[20]) {
  static constexpr char CHARSET[] =
      "qpzry9x8gf2tvdw0s3jn54khce6mua7l";
  if (address.isEmpty() || address.length() > 130) return false;

  bool hasLower = false;
  bool hasUpper = false;
  for (size_t i = 0; i < address.length(); ++i) {
    char ch = address[i];
    hasLower |= ch >= 'a' && ch <= 'z';
    hasUpper |= ch >= 'A' && ch <= 'Z';
  }
  if (hasLower && hasUpper) return false;

  String normalized = address;
  normalized.toLowerCase();
  int separator = normalized.lastIndexOf(':');
  String prefix = separator >= 0 ? normalized.substring(0, separator)
                                 : String("bitcoincash");
  String payload = separator >= 0 ? normalized.substring(separator + 1)
                                  : normalized;
  if (prefix != "bitcoincash" || payload.length() <= 8 ||
      payload.length() > 120) {
    return false;
  }

  uint8_t values[120];
  for (size_t i = 0; i < payload.length(); ++i) {
    const char* value = strchr(CHARSET, payload[i]);
    if (!value) return false;
    values[i] = static_cast<uint8_t>(value - CHARSET);
  }

  uint64_t checksum = 1;
  for (size_t i = 0; i < prefix.length(); ++i) {
    checksum = cashaddrPolymodStep(checksum, prefix[i] & 0x1f);
  }
  checksum = cashaddrPolymodStep(checksum, 0);
  for (size_t i = 0; i < payload.length(); ++i) {
    checksum = cashaddrPolymodStep(checksum, values[i]);
  }
  if (checksum != 1) return false;

  uint8_t decoded[65];
  size_t decodedLength = 0;
  uint32_t accumulator = 0;
  uint8_t bits = 0;
  size_t dataLength = payload.length() - 8;
  for (size_t i = 0; i < dataLength; ++i) {
    accumulator = ((accumulator << 5) | values[i]) & 0x0fffU;
    bits += 5;
    while (bits >= 8) {
      bits -= 8;
      if (decodedLength >= sizeof(decoded)) return false;
      decoded[decodedLength++] =
          static_cast<uint8_t>((accumulator >> bits) & 0xffU);
    }
  }
  if (bits >= 5 || ((accumulator << (8 - bits)) & 0xffU) != 0 ||
      decodedLength != 21 || (decoded[0] & 0x80U) != 0) {
    return false;
  }

  uint8_t type = decoded[0] >> 3;
  uint8_t sizeCode = decoded[0] & 0x07U;
  if (sizeCode != 0 || (type != 0 && type != 1 && type != 2 && type != 3)) {
    return false;
  }
  scriptHash = type == 1 || type == 3;
  memcpy(hash, decoded + 1, 20);
  return true;
}

bool addressToElectrumScriptHash(const String& address, uint8_t pubkeyVersion,
                                 uint8_t scriptVersion, bool allowCashAddress,
                                 String& scriptHashHex) {
  uint8_t hash[20];
  bool isScriptHash = false;
  if (allowCashAddress &&
      (address.indexOf(':') >= 0 || address.startsWith("q") ||
       address.startsWith("p") || address.startsWith("Q") ||
       address.startsWith("P"))) {
    if (!decodeCashAddress(address, isScriptHash, hash)) return false;
  } else {
    uint8_t version = 0;
    if (!decodeBase58Address(address, version, hash)) return false;
    if (version == pubkeyVersion) {
      isScriptHash = false;
    } else if (version == scriptVersion) {
      isScriptHash = true;
    } else {
      return false;
    }
  }

  uint8_t script[25];
  size_t scriptLength = 0;
  if (isScriptHash) {
    script[0] = 0xa9;
    script[1] = 0x14;
    memcpy(script + 2, hash, sizeof(hash));
    script[22] = 0x87;
    scriptLength = 23;
  } else {
    script[0] = 0x76;
    script[1] = 0xa9;
    script[2] = 0x14;
    memcpy(script + 3, hash, sizeof(hash));
    script[23] = 0x88;
    script[24] = 0xac;
    scriptLength = 25;
  }

  uint8_t digest[32];
  if (!sha256Bytes(script, scriptLength, digest)) return false;
  static constexpr char HEX_DIGITS[] = "0123456789abcdef";
  scriptHashHex = "";
  scriptHashHex.reserve(64);
  for (int i = 31; i >= 0; --i) {
    scriptHashHex += HEX_DIGITS[digest[i] >> 4];
    scriptHashHex += HEX_DIGITS[digest[i] & 0x0f];
  }
  return true;
}

bool queryElectrum(const ElectrumServer& server, const String& scriptHash,
                   double& balance, String& error) {
  WiFiClient client;
  client.setTimeout(ELECTRUM_TIMEOUT_MS);
  if (!client.connect(server.host, server.port, ELECTRUM_TIMEOUT_MS)) {
    error = "SOURCE OFFLINE";
    return false;
  }
  client.setNoDelay(true);
  client.print(
      "{\"id\":1,\"method\":\"server.version\",\"params\":[\"HELIOS_HUNTER\",\"1.4\"]}\n");
  client.print(
      String("{\"id\":2,\"method\":\"blockchain.scripthash.get_balance\",\"params\":[\"") +
      scriptHash + "\"]}\n");

  uint32_t startedAt = millis();
  while (millis() - startedAt < ELECTRUM_TIMEOUT_MS) {
    if (!client.available()) {
      if (!client.connected()) break;
      vTaskDelay(pdMS_TO_TICKS(20));
      continue;
    }
    String line = client.readStringUntil('\n');
    JsonDocument document;
    if (deserializeJson(document, line) || document["id"] != 2) continue;
    JsonObject result = document["result"];
    if (result.isNull() || !result["confirmed"].is<int64_t>() ||
        !result["unconfirmed"].is<int64_t>()) {
      error = "BAD RESPONSE";
      client.stop();
      return false;
    }
    int64_t satoshis = result["confirmed"].as<int64_t>() +
                       result["unconfirmed"].as<int64_t>();
    balance = static_cast<double>(satoshis) / 100000000.0;
    client.stop();
    return isfinite(balance) && balance >= 0;
  }
  client.stop();
  error = "SOURCE TIMEOUT";
  return false;
}

template <size_t N>
bool getElectrumBalance(const String& wallet, uint8_t pubkeyVersion,
                        uint8_t scriptVersion, bool allowCashAddress,
                        const ElectrumServer (&servers)[N], double& balance,
                        String& error) {
  String scriptHash;
  if (!addressToElectrumScriptHash(wallet, pubkeyVersion, scriptVersion,
                                   allowCashAddress, scriptHash)) {
    error = "INVALID ADDRESS";
    return false;
  }
  for (size_t i = 0; i < N; ++i) {
    if (queryElectrum(servers[i], scriptHash, balance, error)) return true;
    if (i + 1 < N) vTaskDelay(pdMS_TO_TICKS(100));
  }
  return false;
}

}  // namespace

void HeliosBalances::begin(HeliosSettings* settings) {
  settings_ = settings;
  mutex_ = xSemaphoreCreateMutex();
  if (!mutex_) return;
  syncWallets();
  pendingMask_ = ALL_COINS_MASK;
  // TLS calculations can run longer than the core-0 idle watchdog deadline.
  // Share priority with idle and the helper miner so tick preemption still
  // services both, even while a library call is CPU-bound.
  BaseType_t created =
      xTaskCreatePinnedToCore(taskEntry, "heliosBalance", 14336, this, tskIDLE_PRIORITY,
                              &task_, 0);
  if (created != pdPASS) {
    task_ = nullptr;
    xSemaphoreTake(mutex_, portMAX_DELAY);
    for (size_t i = 0; i < HELIOS_COIN_COUNT; ++i) {
      snapshots_[i].status = "TASK ERROR";
    }
    xSemaphoreGive(mutex_);
  }
}

bool HeliosBalances::setFetchEnabled(bool enabled) {
  if (!mutex_ || !task_) return false;
  xSemaphoreTake(mutex_, portMAX_DELAY);
  if (fetchEnabled_ != enabled) {
    fetchEnabled_ = enabled;
    fetchStateSinceMs_ = millis();
    if (enabled) pendingMask_ |= ALL_COINS_MASK;
  }
  xSemaphoreGive(mutex_);
  xTaskNotifyGive(task_);
  return true;
}

HeliosBalanceFetchState HeliosBalances::fetchState() const {
  HeliosBalanceFetchState state;
  if (!mutex_) return state;
  xSemaphoreTake(mutex_, portMAX_DELAY);
  state.enabled = fetchEnabled_;
  state.active = fetchActive_;
  state.sinceMs = fetchStateSinceMs_;
  xSemaphoreGive(mutex_);
  return state;
}

void HeliosBalances::syncWallets() {
  if (!settings_ || !mutex_) return;
  xSemaphoreTake(mutex_, portMAX_DELAY);
  for (size_t i = 0; i < HELIOS_COIN_COUNT; ++i) {
    String wallet = settings_->wallet(static_cast<HeliosCoin>(i));
    if (wallet != wallets_[i]) {
      HeliosBalanceSnapshot previous = snapshots_[i];
      wallets_[i] = wallet;
      snapshots_[i] = HeliosBalanceSnapshot();
      snapshots_[i].pricesAvailable = previous.pricesAvailable;
      snapshots_[i].priceUsd = previous.priceUsd;
      snapshots_[i].priceCad = previous.priceCad;
      snapshots_[i].priceGbp = previous.priceGbp;
      snapshots_[i].pricesUpdatedAt = previous.pricesUpdatedAt;
      double storedBalance = 0.0;
      double pendingIncrease = 0.0;
      if (loadStoredBalanceState(i, wallet, storedBalance, pendingIncrease)) {
        snapshots_[i].baselineReady = true;
        snapshots_[i].balance = storedBalance;
        if (pendingIncrease > 0.0) {
          snapshots_[i].increaseAmount = pendingIncrease;
          snapshots_[i].increaseSequence = ++increaseSequence_;
          if (increaseSequence_ == 0) {
            increaseSequence_ = 1;
            snapshots_[i].increaseSequence = increaseSequence_;
          }
        }
      }
      snapshots_[i].status = wallet.isEmpty() ? "NOT SET" : "QUEUED";
    }
  }
  xSemaphoreGive(mutex_);
}

void HeliosBalances::requestRefresh() {
  if (!mutex_) return;
  syncWallets();
  xSemaphoreTake(mutex_, portMAX_DELAY);
  pendingMask_ |= ALL_COINS_MASK;
  for (size_t i = 0; i < HELIOS_COIN_COUNT; ++i) {
    if (!wallets_[i].isEmpty() && !snapshots_[i].refreshing) {
      snapshots_[i].status = "QUEUED";
    }
  }
  xSemaphoreGive(mutex_);
  if (task_) xTaskNotifyGive(task_);
}

void HeliosBalances::requestRefresh(HeliosCoin coin) {
  if (!mutex_) return;
  syncWallets();
  xSemaphoreTake(mutex_, portMAX_DELAY);
  size_t index = heliosCoinIndex(coin);
  pendingMask_ |= 1U << index;
  if (!wallets_[index].isEmpty() && !snapshots_[index].refreshing) {
    snapshots_[index].status = "QUEUED";
  }
  xSemaphoreGive(mutex_);
  if (task_) xTaskNotifyGive(task_);
}

void HeliosBalances::acknowledgeIncrease(HeliosCoin coin,
                                         uint32_t sequence) {
  if (!mutex_ || sequence == 0) return;
  size_t index = heliosCoinIndex(coin);
  xSemaphoreTake(mutex_, portMAX_DELAY);
  if (snapshots_[index].increaseSequence == sequence) {
    snapshots_[index].increaseAmount = 0.0;
    snapshots_[index].increaseSequence = 0;
    clearStoredPendingIncrease(index, wallets_[index]);
  }
  xSemaphoreGive(mutex_);
}

HeliosBalanceSnapshot HeliosBalances::get(HeliosCoin coin) const {
  HeliosBalanceSnapshot result;
  if (!mutex_) return result;
  xSemaphoreTake(mutex_, portMAX_DELAY);
  result = snapshots_[heliosCoinIndex(coin)];
  xSemaphoreGive(mutex_);
  return result;
}

void HeliosBalances::taskEntry(void* argument) {
  static_cast<HeliosBalances*>(argument)->taskLoop();
}

void HeliosBalances::taskLoop() {
  uint32_t wifiReadyAt = 0;
  for (;;) {
    if (!fetchState().enabled) {
      ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(1000));
      continue;
    }
    if (WiFi.status() != WL_CONNECTED) {
      wifiReadyAt = 0;
      ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(5000));
      continue;
    }
    if (wifiReadyAt == 0) wifiReadyAt = millis();
    if ((uint32_t)(millis() - wifiReadyAt) < BALANCE_NETWORK_SETTLE_MS) {
      vTaskDelay(pdMS_TO_TICKS(1000));
      continue;
    }

    if (millis() - lastSweepAt_ >= REFRESH_INTERVAL_MS) {
      xSemaphoreTake(mutex_, portMAX_DELAY);
      pendingMask_ |= ALL_COINS_MASK;
      xSemaphoreGive(mutex_);
      lastSweepAt_ = millis();
    }

    int next = -1;
    String wallet;
    xSemaphoreTake(mutex_, portMAX_DELAY);
    for (size_t i = 0; fetchEnabled_ && i < HELIOS_COIN_COUNT; ++i) {
      if (pendingMask_ & (1U << i)) {
        pendingMask_ &= ~(1U << i);
        next = static_cast<int>(i);
        wallet = wallets_[i];
        break;
      }
    }
    // Finish a selected coin refresh before acknowledging a diagnostic pause;
    // suspending inside HTTP/TLS or storage code could leave a lock held.
    fetchActive_ = next >= 0;
    if (fetchActive_) fetchStateSinceMs_ = millis();
    xSemaphoreGive(mutex_);

    if (next < 0) {
      ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(30000));
      continue;
    }

    refreshCoin(static_cast<HeliosCoin>(next), wallet);
    xSemaphoreTake(mutex_, portMAX_DELAY);
    fetchActive_ = false;
    fetchStateSinceMs_ = millis();
    xSemaphoreGive(mutex_);
    vTaskDelay(pdMS_TO_TICKS(BETWEEN_COIN_REQUESTS_MS));
  }
}

void HeliosBalances::refreshCoin(HeliosCoin coin, const String& wallet) {
  size_t index = heliosCoinIndex(coin);
  xSemaphoreTake(mutex_, portMAX_DELAY);
  snapshots_[index].refreshing = true;
  snapshots_[index].status = wallet.isEmpty() ? "NOT SET" : "UPDATING";
  xSemaphoreGive(mutex_);

  double balance = 0;
  double usd = 0;
  double cad = 0;
  double gbp = 0;
  String balanceError;
  String priceError;
  bool balanceOk = !wallet.isEmpty() &&
                   fetchBalance(coin, wallet, balance, balanceError);
  bool persistBalance = false;
  double pendingIncrease = 0.0;
  xSemaphoreTake(mutex_, portMAX_DELAY);
  bool pricesOk = snapshots_[index].pricesAvailable &&
                  millis() - snapshots_[index].pricesUpdatedAt <
                      PRICE_CACHE_MS;
  if (pricesOk) {
    usd = snapshots_[index].priceUsd;
    cad = snapshots_[index].priceCad;
    gbp = snapshots_[index].priceGbp;
  }
  xSemaphoreGive(mutex_);
  if (!wallet.isEmpty() && !pricesOk) {
    pricesOk = fetchPrices(coin, usd, cad, gbp, priceError);
  }
  xSemaphoreTake(mutex_, portMAX_DELAY);
  if (wallets_[index] != wallet) {
    xSemaphoreGive(mutex_);
    return;
  }
  snapshots_[index].refreshing = false;
  snapshots_[index].available = balanceOk;
  snapshots_[index].pricesAvailable = pricesOk;
  if (balanceOk) {
    double previousBalance = snapshots_[index].balance;
    bool hadBaseline = snapshots_[index].baselineReady;
    bool increased = hadBaseline &&
                     balance > previousBalance + 0.000000001;
    if (increased) {
      snapshots_[index].increaseAmount = balance - previousBalance;
      snapshots_[index].increaseSequence = ++increaseSequence_;
      if (increaseSequence_ == 0) {
        increaseSequence_ = 1;
        snapshots_[index].increaseSequence = increaseSequence_;
      }
    }
    persistBalance = !hadBaseline ||
                     fabs(balance - previousBalance) > 0.000000001;
    snapshots_[index].baselineReady = true;
    snapshots_[index].balance = balance;
    snapshots_[index].updatedAt = millis();
    snapshots_[index].status = coin == HeliosCoin::DGB ? "CACHED (UP TO 6H)" : "UPDATED";
    if (snapshots_[index].increaseSequence != 0) {
      pendingIncrease = snapshots_[index].increaseAmount;
    }
  } else if (wallet.isEmpty()) {
    snapshots_[index].balance = 0;
    snapshots_[index].status = "NOT SET";
  } else {
    snapshots_[index].status =
        balanceError.isEmpty() ? "UNAVAILABLE" : balanceError;
  }
  if (pricesOk) {
    snapshots_[index].priceUsd = usd;
    snapshots_[index].priceCad = cad;
    snapshots_[index].priceGbp = gbp;
    snapshots_[index].pricesUpdatedAt = millis();
  }
  xSemaphoreGive(mutex_);
  if (balanceOk && persistBalance) {
    saveStoredBalanceState(index, wallet, balance, pendingIncrease);
  }
}

bool HeliosBalances::fetchBalance(HeliosCoin coin, const String& wallet,
                                  double& balance, String& error) {
  if (coin == HeliosCoin::WJK) {
    String payload;
    String primaryError;
    String primaryUrl =
        "https://explorer.wojakcoin.cash/api/address/" + wallet;
    if (getPayload(primaryUrl, true, payload, primaryError) &&
        parseElectrs(payload, balance)) {
      return true;
    }

    String fallbackError;
    String fallbackUrl =
        "https://wojak-explorer.dedoo.xyz/ext/getbalance/" + wallet;
    if (getPayload(fallbackUrl, true, payload, fallbackError) &&
        parseNumber(payload, balance)) {
      return true;
    }
    error = fallbackError.isEmpty() ? primaryError : fallbackError;
    if (error.isEmpty()) error = "BAD RESPONSE";
    return false;
  }
  String payload;
  String primaryError;
  String fallbackError;
  switch (coin) {
    case HeliosCoin::CHTA:
      return getElectrumBalance(wallet, 28, 5, false,
                                CHTA_ELECTRUM_SERVERS, balance, error);

    case HeliosCoin::WJK:
      return false;

    case HeliosCoin::DGB:
      if (getPayload(
              "https://chainz.cryptoid.info/dgb/api.dws?q=getbalance&a=" +
                  wallet,
              true, payload, primaryError) &&
          parseNumber(payload, balance)) {
        return true;
      }
      if (getPayload("https://digiexplorer.info/api/address/" + wallet, true,
                     payload, fallbackError) &&
          parseElectrs(payload, balance)) {
        return true;
      }
      break;

    case HeliosCoin::BCH:
      if (getPayload(
              "https://api.blockchain.info/haskoin-store/bch/address/" +
                  wallet + "/balance",
              true, payload, primaryError) &&
          parseHaskoin(payload, balance)) {
        return true;
      }
      if (getElectrumBalance(wallet, 0, 5, true, BCH_ELECTRUM_SERVERS,
                             balance, fallbackError)) {
        return true;
      }
      break;

    case HeliosCoin::BTC:
      if (getPayload("https://blockstream.info/api/address/" + wallet, true,
                     payload, primaryError) &&
          parseElectrs(payload, balance)) {
        return true;
      }
      if (getPayload("https://mempool.space/api/address/" + wallet, true,
                     payload, fallbackError) &&
          parseElectrs(payload, balance)) {
        return true;
      }
      break;

    default:
      error = "UNKNOWN COIN";
      return false;
  }

  error = fallbackError.isEmpty() ? primaryError : fallbackError;
  if (error.isEmpty()) error = "BAD RESPONSE";
  return false;
}

bool HeliosBalances::fetchPrices(HeliosCoin coin, double& usd, double& cad,
                                 double& gbp, String& error) {
  String url = "https://api.coinpaprika.com/v1/tickers/" +
               String(PRICE_IDS[heliosCoinIndex(coin)]) +
               "?quotes=USD,CAD,GBP";
  String payload;
  if (!getPayload(url, true, payload, error)) return false;
  JsonDocument document;
  if (deserializeJson(document, payload)) {
    error = "PRICE RESPONSE";
    return false;
  }
  JsonObject quotes = document["quotes"];
  if (!quotes["USD"]["price"].is<double>() ||
      !quotes["CAD"]["price"].is<double>() ||
      !quotes["GBP"]["price"].is<double>()) {
    error = "PRICE MISSING";
    return false;
  }
  usd = quotes["USD"]["price"].as<double>();
  cad = quotes["CAD"]["price"].as<double>();
  gbp = quotes["GBP"]["price"].as<double>();
  return isfinite(usd) && isfinite(cad) && isfinite(gbp) && usd >= 0 &&
         cad >= 0 && gbp >= 0;
}
