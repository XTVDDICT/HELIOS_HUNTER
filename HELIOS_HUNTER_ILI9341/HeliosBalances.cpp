#include "HeliosBalances.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <math.h>
#include <stdlib.h>

namespace {

constexpr uint32_t REFRESH_INTERVAL_MS = 15UL * 60UL * 1000UL;
constexpr uint32_t PRICE_CACHE_MS = 10UL * 60UL * 1000UL;
constexpr uint32_t REQUEST_TIMEOUT_MS = 12000;
constexpr uint8_t REQUEST_ATTEMPTS = 2;
constexpr uint8_t ALL_COINS_MASK = (1U << HELIOS_COIN_COUNT) - 1U;
constexpr const char* PRICE_IDS[HELIOS_COIN_COUNT] = {
    "chta-cheetahcoin", "wjk-wojakcoin", "dgb-digibyte",
    "bch-bitcoin-cash", "btc-bitcoin"};

bool parseNumber(String payload, double& value) {
  payload.trim();
  if (payload.isEmpty() || payload.startsWith("ERROR")) return false;
  char* end = nullptr;
  value = strtod(payload.c_str(), &end);
  return end != payload.c_str() && *end == '\0' && isfinite(value) && value >= 0;
}

bool getPayloadOnce(const String& url, bool secure, String& payload,
                    String& error) {
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
  payload = http.getString();
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

}  // namespace

void HeliosBalances::begin(HeliosSettings* settings) {
  settings_ = settings;
  mutex_ = xSemaphoreCreateMutex();
  if (!mutex_) return;
  syncWallets();
  pendingMask_ = ALL_COINS_MASK;
  BaseType_t created =
      xTaskCreatePinnedToCore(taskEntry, "heliosBalance", 8192, this, 1,
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
  for (;;) {
    if (WiFi.status() != WL_CONNECTED) {
      ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(5000));
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
    for (size_t i = 0; i < HELIOS_COIN_COUNT; ++i) {
      if (pendingMask_ & (1U << i)) {
        pendingMask_ &= ~(1U << i);
        next = static_cast<int>(i);
        wallet = wallets_[i];
        break;
      }
    }
    xSemaphoreGive(mutex_);

    if (next < 0) {
      ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(30000));
      continue;
    }

    refreshCoin(static_cast<HeliosCoin>(next), wallet);
    vTaskDelay(pdMS_TO_TICKS(250));
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
  if (!pricesOk) pricesOk = fetchPrices(coin, usd, cad, gbp, priceError);
  xSemaphoreTake(mutex_, portMAX_DELAY);
  if (wallets_[index] != wallet) {
    xSemaphoreGive(mutex_);
    return;
  }
  snapshots_[index].refreshing = false;
  snapshots_[index].available = balanceOk;
  snapshots_[index].pricesAvailable = pricesOk;
  if (balanceOk) {
    bool increased = snapshots_[index].updatedAt != 0 &&
                     balance > snapshots_[index].balance + 0.000000001;
    if (increased) {
      snapshots_[index].increaseAmount = balance - snapshots_[index].balance;
      snapshots_[index].increaseSequence = ++increaseSequence_;
      if (increaseSequence_ == 0) {
        increaseSequence_ = 1;
        snapshots_[index].increaseSequence = increaseSequence_;
      }
    }
    snapshots_[index].balance = balance;
    snapshots_[index].updatedAt = millis();
    snapshots_[index].status = coin == HeliosCoin::DGB ? "CACHED (UP TO 6H)" : "UPDATED";
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

  String url;
  bool secure = true;
  switch (coin) {
    case HeliosCoin::CHTA:
      secure = false;
      url = "http://chtaexplorer.mooo.com:3002/ext/getbalance/" + wallet;
      break;
    case HeliosCoin::WJK:
      return false;
    case HeliosCoin::DGB:
      url = "https://chainz.cryptoid.info/dgb/api.dws?q=getbalance&a=" +
            wallet;
      break;
    case HeliosCoin::BCH:
      url = "https://api.blockchain.info/haskoin-store/bch/address/" +
            wallet + "/balance";
      break;
    case HeliosCoin::BTC:
      url = "https://blockstream.info/api/address/" + wallet;
      break;
    default:
      error = "UNKNOWN COIN";
      return false;
  }

  String payload;
  if (!getPayload(url, secure, payload, error)) return false;
  bool parsed = false;
  if (coin == HeliosCoin::BTC) {
    parsed = parseElectrs(payload, balance);
  } else if (coin == HeliosCoin::BCH) {
    parsed = parseHaskoin(payload, balance);
  } else {
    parsed = parseNumber(payload, balance);
  }
  if (!parsed) error = "BAD RESPONSE";
  return parsed;
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
