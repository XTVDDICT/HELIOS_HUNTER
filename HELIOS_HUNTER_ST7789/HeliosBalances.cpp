#include "HeliosBalances.h"
#include "HeliosDiagnostics.h"
#include "HeliosNetworkMemory.h"
#include "HeliosCooperativeTls.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <StreamString.h>
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
constexpr uint32_t PRICE_RETRY_MS = 60000;
constexpr uint32_t PRICE_CACHE_MS = 10UL * 60UL * 1000UL;
constexpr uint32_t REQUEST_TIMEOUT_MS = 12000;
constexpr uint8_t REQUEST_ATTEMPTS = 2;
constexpr uint32_t ELECTRUM_TIMEOUT_MS = 6000;
constexpr size_t MAX_ELECTRUM_LINE_BYTES = 4096;
constexpr uint32_t BALANCE_NETWORK_SETTLE_MS = 20000;
constexpr uint32_t BETWEEN_COIN_REQUESTS_MS = 5000;
constexpr size_t MAX_HTTP_PAYLOAD_BYTES = 32768;

// Bound incoming data before StreamString can allocate it. Content-Length can
// be absent (or chunked), so checking the size only after reading is too late.
class BoundedPayloadWriter : public Stream {
 public:
  explicit BoundedPayloadWriter(StreamString& payload)
      : payload_(payload), startedAt_(millis()), yieldedAt_(startedAt_) {}
  size_t write(uint8_t value) override { return write(&value, 1); }
  size_t write(const uint8_t* data, size_t size) override {
    if (error_) return 0;
    if (payload_.length() > MAX_HTTP_PAYLOAD_BYTES ||
        size > MAX_HTTP_PAYLOAD_BYTES - payload_.length()) {
      error_ = "RESPONSE TOO LARGE";
      return 0;
    }
    const uint32_t now = millis();
    if (now - startedAt_ >= REQUEST_TIMEOUT_MS) {
      error_ = "BODY TIMEOUT";
      return 0;
    }
    if (now - yieldedAt_ >= 20U) {
      vTaskDelay(1);
      yieldedAt_ = millis();
    }
    const size_t written = payload_.write(data, size);
    if (written != size) error_ = "LOW MEMORY";
    return written;
  }
  int available() override { return 0; }
  int read() override { return -1; }
  int peek() override { return -1; }
  void flush() override {}
  const char* error() const { return error_; }
 private:
  StreamString& payload_;
  uint32_t startedAt_;
  uint32_t yieldedAt_;
  const char* error_ = nullptr;
};
// Only suppress a request when the heap is critically low. TLS can operate
// with fragmented heap on these CYDs, and each response allocation is checked
// separately once its actual Content-Length is known.
constexpr size_t MIN_REQUEST_FREE_HEAP = 24000;
constexpr size_t MIN_REQUEST_LARGEST_BLOCK = 8192;
static_assert(HELIOS_COIN_COUNT <= 16, "Coin refresh mask capacity exceeded");
constexpr uint16_t ALL_COINS_MASK = (1U << HELIOS_COIN_COUNT) - 1U;
constexpr const char* BALANCE_STATE_NAMESPACE = "heliosbal";
constexpr const char* BALANCE_WALLET_KEYS[HELIOS_COIN_COUNT] = {
    "wallet0", "wallet1", "wallet2", "wallet3", "wallet4", "wallet5", "wallet6", "wallet7", "wallet8"};
constexpr const char* BALANCE_VALUE_KEYS[HELIOS_COIN_COUNT] = {
    "balance0", "balance1", "balance2", "balance3", "balance4", "balance5", "balance6", "balance7", "balance8"};
constexpr const char* PENDING_VALUE_KEYS[HELIOS_COIN_COUNT] = {
    "pending0", "pending1", "pending2", "pending3", "pending4", "pending5", "pending6", "pending7", "pending8"};
constexpr const char* PRICE_IDS[HELIOS_COIN_COUNT] = {
    "chta-cheetahcoin", "wjk-wojakcoin", "dgb-digibyte",
    "bch-bitcoin-cash", "btc-bitcoin", "fix-fixedcoin",
    "xec-ecash", "zec-zcash", "grlc-garlicoin"};

struct ElectrumServer {
  const char* host;
  uint16_t port;
};

// Current community servers, checked against the configured GRLC balance.
constexpr ElectrumServer GRLC_ELECTRUM_SERVERS[] = {
    {"electrum.maxpuig.com", 50001},
    {"electrum.niftybakes.com", 50001}};

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
  heliosMarkBalance(HeliosBalanceStage::Storage);
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
  heliosMarkBalance(HeliosBalanceStage::Storage);
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

// Mainnet servers published by Electrum ABC. Prefer the live-tested server.
constexpr ElectrumServer XEC_ELECTRUM_SERVERS[] = {
    {"fulcrum.pierre.cash", 50001},
    {"electrum.bitcoinabc.org", 50001},
    {"electrum.fabien.cash", 50001}};

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

bool getPayloadOnce(const String& url, bool secure, StreamString& payload,
                     String& error) {
  heliosMarkBalance(HeliosBalanceStage::HttpRequest);
  if (!requestMemoryAvailable(error)) return false;

  // HTTPClient borrows its transport and may still call stop() in its
  // destructor after end() on a failed/disconnected request. Local objects
  // are destroyed in reverse declaration order: transports must outlive http.
  WiFiClient client;
  HeliosCooperativeTls tls(REQUEST_TIMEOUT_MS);
  HTTPClient http;
  http.setConnectTimeout(8000);
  http.setTimeout(REQUEST_TIMEOUT_MS);
  http.setUserAgent("HELIOS_HUNTER/0.1");
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  http.setReuse(false);
  http.useHTTP10(true);

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
    http.end();
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
  heliosMarkBalance(HeliosBalanceStage::HttpBody);
  BoundedPayloadWriter writer(payload);
  int bytesRead = http.writeToStream(&writer);
  if (writer.error()) {
    error = writer.error();
    http.end();
    return false;
  }
  if (bytesRead < 0) {
    error = String("NET ") + bytesRead;
    http.end();
    return false;
  }
  if (responseSize >= 0 && bytesRead != responseSize) {
    error = "INCOMPLETE RESPONSE";
    http.end();
    return false;
  }
  if (payload.length() > MAX_HTTP_PAYLOAD_BYTES) {
    payload.clear();
    error = "RESPONSE TOO LARGE";
    http.end();
    return false;
  }
  http.end();
  return true;
}

bool getPayload(const String& url, bool secure, StreamString& payload,
                String& error) {
  for (uint8_t attempt = 0; attempt < REQUEST_ATTEMPTS; ++attempt) {
    payload.clear();
    if (getPayloadOnce(url, secure, payload, error)) {
      heliosMarkBalance(HeliosBalanceStage::Parsing);
      return true;
    }
    if (attempt + 1 < REQUEST_ATTEMPTS) vTaskDelay(pdMS_TO_TICKS(350));
  }
  return false;
}

bool parseGarlicoin(const String& payload, double& balance) {
  JsonDocument document;
  JsonDocument filter;
  filter["balance"] = true;
  filter["error"] = true;
  if (deserializeJson(document, payload, DeserializationOption::Filter(filter)) ||
      !document["error"].isNull() || !document["balance"].is<double>()) {
    return false;
  }
  // addr.php?api=1&op=balance reports native GRLC, unlike the explorer's
  // separate lite_api_balance endpoint, which reports atomic units.
  const double amount = document["balance"].as<double>();
  if (!isfinite(amount) || amount < 0 || amount > 69000000.0) return false;
  balance = amount;
  return true;
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

// ZecBlock balances are atomic-unit counts.
bool parseAtomicAmount(JsonVariantConst value, double units, double& amount) {
  uint64_t atomic = 0;
  if (value.is<const char*>()) {
    const char* text = value.as<const char*>();
    if (!text || !*text) return false;
    for (; *text; ++text) {
      if (*text < '0' || *text > '9') return false;
      uint8_t digit = static_cast<uint8_t>(*text - '0');
      if (atomic > (UINT64_MAX - digit) / 10U) return false;
      atomic = atomic * 10U + digit;
    }
  } else if (value.is<uint64_t>()) {
    atomic = value.as<uint64_t>();
  } else {
    return false;
  }
  amount = static_cast<double>(atomic) / units;
  return isfinite(amount) && amount >= 0;
}

bool parseZecBlock(const String& payload, const String& wallet, double& balance) {
  JsonDocument document;
  JsonDocument filter;
  filter["data"]["address"] = true;
  filter["data"]["balance"] = true;
  filter["data"]["balanceZat"] = true;
  filter["meta"]["network"] = true;
  filter["meta"]["units"]["zatoshiPerZec"] = true;
  if (deserializeJson(document, payload, DeserializationOption::Filter(filter)) ||
      document["data"]["address"] != wallet || document["meta"]["network"] != "mainnet" ||
      document["meta"]["units"]["zatoshiPerZec"] != "100000000") return false;
  // Empty addresses omit the optional balanceZat alias; balance is still an
  // exact zatoshi string, as established by the checked unit metadata above.
  JsonVariantConst value = document["data"]["balanceZat"];
  if (value.isNull()) value = document["data"]["balance"];
  return parseAtomicAmount(value, 100000000.0, balance);
}

String addressPath(const String& address) {
  static const char hex[] = "0123456789ABCDEF";
  String encoded;
  encoded.reserve(address.length() * 3);
  for (size_t i = 0; i < address.length(); ++i) {
    uint8_t c = static_cast<uint8_t>(address[i]);
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9')) encoded += static_cast<char>(c);
    else { encoded += '%'; encoded += hex[c >> 4]; encoded += hex[c & 15]; }
  }
  return encoded;
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
                       uint8_t hash[20], const char* expectedPrefix = "bitcoincash") {
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
                                 : String(expectedPrefix);
  String payload = separator >= 0 ? normalized.substring(separator + 1)
                                  : normalized;
  if (prefix != expectedPrefix || payload.length() <= 8 ||
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
                                 String& scriptHashHex, const char* cashPrefix = "bitcoincash") {
  uint8_t hash[20];
  bool isScriptHash = false;
  if (allowCashAddress &&
      (address.indexOf(':') >= 0 || address.startsWith("q") ||
       address.startsWith("p") || address.startsWith("Q") ||
       address.startsWith("P"))) {
    if (!decodeCashAddress(address, isScriptHash, hash, cashPrefix)) return false;
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

bool garlicoinElectrumScriptHash(const String& address, String& scriptHashHex) {
  String normalized = address;
  normalized.toLowerCase();
  if (!normalized.startsWith("grlc1")) {
    return addressToElectrumScriptHash(address, 38, 50, false, scriptHashHex);
  }
  // Garlicoin supports Bech32 witness v0 (20- or 32-byte programs), not
  // Bech32m/Taproot. Verify the checksum and padding before querying a server.
  if (address.length() < 14 || address.length() > 90) return false;
  bool lower = false, upper = false;
  for (size_t i = 0; i < address.length(); ++i) {
    lower |= address[i] >= 'a' && address[i] <= 'z';
    upper |= address[i] >= 'A' && address[i] <= 'Z';
  }
  if (lower && upper) return false;
  static constexpr char CHARSET[] = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";
  static constexpr uint32_t GENERATORS[] = {
      0x3b6a57b2, 0x26508e6d, 0x1ea119fa, 0x3d4233dd, 0x2a1462b3};
  uint32_t checksum = 1;
  auto addChecksum = [&](uint8_t value) {
    uint8_t top = checksum >> 25;
    checksum = ((checksum & 0x1ffffffU) << 5) ^ value;
    for (uint8_t i = 0; i < 5; ++i) {
      if ((top >> i) & 1U) checksum ^= GENERATORS[i];
    }
  };
  for (size_t i = 0; i < 4; ++i) addChecksum(normalized[i] >> 5);
  addChecksum(0);
  for (size_t i = 0; i < 4; ++i) addChecksum(normalized[i] & 31);
  const size_t dataLength = normalized.length() - 5;
  uint8_t data[85];
  for (size_t i = 0; i < dataLength; ++i) {
    const char* found = strchr(CHARSET, normalized[5 + i]);
    if (!found) return false;
    data[i] = static_cast<uint8_t>(found - CHARSET);
    addChecksum(data[i]);
  }
  if (checksum != 1 || data[0] != 0) return false;
  uint8_t script[34] = {};
  size_t programLength = 0;
  uint32_t accumulator = 0;
  uint8_t bits = 0;
  for (size_t i = 1; i < dataLength - 6; ++i) {
    accumulator = ((accumulator << 5) | data[i]) & 0xfffU;
    bits += 5;
    if (bits >= 8) {
      bits -= 8;
      if (programLength >= 32) return false;
      script[2 + programLength++] = (accumulator >> bits) & 255U;
    }
  }
  if (bits >= 5 || ((accumulator << (8 - bits)) & 255U) != 0 ||
      (programLength != 20 && programLength != 32)) return false;
  script[1] = programLength;
  uint8_t digest[32];
  if (!sha256Bytes(script, programLength + 2, digest)) return false;
  static constexpr char HEX_DIGITS[] = "0123456789abcdef";
  scriptHashHex = "";
  if (!scriptHashHex.reserve(64)) return false;
  for (int i = 31; i >= 0; --i) {
    scriptHashHex += HEX_DIGITS[digest[i] >> 4];
    scriptHashHex += HEX_DIGITS[digest[i] & 15];
  }
  return scriptHashHex.length() == 64;
}

bool queryElectrum(const ElectrumServer& server, const String& scriptHash,
                   double& balance, String& error, double units = 100000000.0) {
  heliosMarkBalance(HeliosBalanceStage::Electrum);
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
  String line;
  if (!line.reserve(256)) {
    error = "LOW MEMORY";
    client.stop();
    return false;
  }
  while (millis() - startedAt < ELECTRUM_TIMEOUT_MS) {
    if (!client.available()) {
      if (!client.connected()) break;
      vTaskDelay(pdMS_TO_TICKS(20));
      continue;
    }
    // Read incrementally so a missing newline cannot extend the overall
    // timeout or grow a String without limit.
    const int ch = client.read();
    if (ch < 0 || ch == '\r') continue;
    if (ch != '\n') {
      if (line.length() >= MAX_ELECTRUM_LINE_BYTES) {
        error = "RESPONSE TOO LARGE";
        client.stop();
        return false;
      }
      if (!line.concat(static_cast<char>(ch))) {
        error = "LOW MEMORY";
        client.stop();
        return false;
      }
      continue;
    }
    JsonDocument document;
    const bool wanted = !deserializeJson(document, line) && document["id"] == 2;
    line = "";
    if (!wanted) continue;
    JsonObject result = document["result"];
    if (result.isNull() || !result["confirmed"].is<int64_t>() ||
        !result["unconfirmed"].is<int64_t>()) {
      error = "BAD RESPONSE";
      client.stop();
      return false;
    }
    int64_t satoshis = result["confirmed"].as<int64_t>() +
                       result["unconfirmed"].as<int64_t>();
    balance = static_cast<double>(satoshis) / units;
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
                        String& error, double units = 100000000.0,
                        const char* cashPrefix = "bitcoincash") {
  String scriptHash;
  if (!addressToElectrumScriptHash(wallet, pubkeyVersion, scriptVersion,
                                   allowCashAddress, scriptHash, cashPrefix)) {
    error = "INVALID ADDRESS";
    return false;
  }
  for (size_t i = 0; i < N; ++i) {
    if (queryElectrum(servers[i], scriptHash, balance, error, units)) return true;
    if (i + 1 < N) vTaskDelay(pdMS_TO_TICKS(100));
  }
  return false;
}

}  // namespace

void HeliosBalances::begin(HeliosSettings* settings) {
  heliosNetworkMemoryBegin();
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

HeliosBalanceFetchState HeliosBalances::fetchState(bool includeStack) const {
  HeliosBalanceFetchState state;
  if (!mutex_) return state;
  xSemaphoreTake(mutex_, portMAX_DELAY);
  state.enabled = fetchEnabled_;
  state.active = fetchActive_;
  state.sinceMs = fetchStateSinceMs_;
  if (includeStack && task_) {
    state.stackFreeBytes =
        static_cast<uint32_t>(uxTaskGetStackHighWaterMark(task_));
  }
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
      snapshots_[i].priceStatus = previous.priceStatus;
      snapshots_[i].priceUsd = previous.priceUsd;
      snapshots_[i].priceCad = previous.priceCad;
      snapshots_[i].priceGbp = previous.priceGbp;
      snapshots_[i].priceEur = previous.priceEur;
      snapshots_[i].pricesUpdatedAt = previous.pricesUpdatedAt;
      double storedBalance = 0.0;
      double pendingIncrease = 0.0;
      if (loadStoredBalanceState(i, wallet, storedBalance, pendingIncrease)) {
        snapshots_[i].baselineReady = true;
        snapshots_[i].available = true;
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
  priorityMask_ |= 1U << index;
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
  bool queued = false;
  xSemaphoreTake(mutex_, portMAX_DELAY);
  if (snapshots_[index].increaseSequence == sequence) {
    snapshots_[index].increaseAmount = 0.0;
    snapshots_[index].increaseSequence = 0;
    acknowledgedWallets_[index] = wallets_[index];
    pendingAcknowledgementMask_ |= 1U << index;
    queued = true;
  }
  xSemaphoreGive(mutex_);
  if (queued && task_) xTaskNotifyGive(task_);
}

HeliosBalanceSnapshot HeliosBalances::get(HeliosCoin coin) const {
  HeliosBalanceSnapshot result;
  if (!mutex_) return result;
  xSemaphoreTake(mutex_, portMAX_DELAY);
  result = snapshots_[heliosCoinIndex(coin)];
  xSemaphoreGive(mutex_);
  return result;
}

double HeliosBalances::fiatTotal(uint8_t currency, bool& available) const {
  available = false;
  if (!mutex_) return 0.0;
  double total = 0.0;
  xSemaphoreTake(mutex_, portMAX_DELAY);
  for (size_t i = 0; i < HELIOS_COIN_COUNT; ++i) {
    const HeliosBalanceSnapshot& snapshot = snapshots_[i];
    if (!snapshot.available || !snapshot.pricesAvailable) continue;
    double price = currency == 1 ? snapshot.priceCad
                                 : currency == 3 ? snapshot.priceEur
                                 : currency == 2 ? snapshot.priceGbp
                                                 : snapshot.priceUsd;
    if (!isfinite(snapshot.balance) || !isfinite(price)) continue;
    total += snapshot.balance * price;
    available = true;
  }
  xSemaphoreGive(mutex_);
  return total;
}

void HeliosBalances::taskEntry(void* argument) {
  static_cast<HeliosBalances*>(argument)->taskLoop();
}

void HeliosBalances::processAcknowledgements() {
  for (;;) {
    size_t index = HELIOS_COIN_COUNT;
    String wallet;
    bool clearStored = false;
    xSemaphoreTake(mutex_, portMAX_DELAY);
    for (size_t i = 0; i < HELIOS_COIN_COUNT; ++i) {
      if ((pendingAcknowledgementMask_ & (1U << i)) == 0) continue;
      pendingAcknowledgementMask_ &= ~(1U << i);
      index = i;
      wallet = acknowledgedWallets_[i];
      acknowledgedWallets_[i] = "";
      clearStored = snapshots_[i].increaseSequence == 0;
      break;
    }
    xSemaphoreGive(mutex_);
    if (index == HELIOS_COIN_COUNT) return;
    if (clearStored) clearStoredPendingIncrease(index, wallet);
  }
}

void HeliosBalances::taskLoop() {
  uint32_t wifiReadyAt = 0;
  size_t nextRefreshPosition = 0;
  for (;;) {
    heliosMarkBalance(HeliosBalanceStage::Waiting);
    processAcknowledgements();
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
    bool pricesOnly = false;
    String wallet;
    xSemaphoreTake(mutex_, portMAX_DELAY);
    // Prioritize an explicitly saved wallet, then rotate in display order.
    // Refilling the five-minute mask must not restart selection at index 0
    // and starve coins near the end when a sweep takes longer than five minutes.
    for (uint8_t pass = 0; fetchEnabled_ && pass < 2 && next < 0; ++pass) {
      const uint16_t candidates = pass == 0
          ? pendingMask_ & priorityMask_ : pendingMask_;
      for (size_t offset = 0; offset < HELIOS_COIN_COUNT; ++offset) {
        const size_t position = (nextRefreshPosition + offset) % HELIOS_COIN_COUNT;
        const size_t index = heliosCoinIndex(heliosDisplayCoinProfileAt(position).id);
        if ((candidates & (1U << index)) == 0) continue;
        pendingMask_ &= ~(1U << index);
        priorityMask_ &= ~(1U << index);
        next = static_cast<int>(index);
        wallet = wallets_[index];
        nextRefreshPosition = (position + 1) % HELIOS_COIN_COUNT;
        break;
      }
    }
    // Recover missing/expired prices between normal wallet sweeps. Keep the
    // same fair display-order cursor, one coin at a time, with a minute of
    // backoff per coin. Do not fetch wallet balances again just to retry prices.
    if (fetchEnabled_ && next < 0) {
      for (size_t offset = 0; offset < HELIOS_COIN_COUNT; ++offset) {
        const size_t position = (nextRefreshPosition + offset) % HELIOS_COIN_COUNT;
        const size_t index = heliosCoinIndex(heliosDisplayCoinProfileAt(position).id);
        const auto& snapshot = snapshots_[index];
        if (wallets_[index].isEmpty() ||
            (snapshot.pricesAvailable &&
             millis() - snapshot.pricesUpdatedAt < PRICE_CACHE_MS) ||
            millis() - lastPriceAttemptAt_[index] < PRICE_RETRY_MS) continue;
        next = static_cast<int>(index);
        wallet = wallets_[index];
        pricesOnly = true;
        nextRefreshPosition = (position + 1) % HELIOS_COIN_COUNT;
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

    {
      // Do not hold the snapshot mutex while waiting: web/display readers
      // need it. Release the memory gate before the between-coin delay.
      HeliosNetworkMemoryGuard memory(portMAX_DELAY, true);
      if (memory) refreshCoin(static_cast<HeliosCoin>(next), wallet, pricesOnly);
    }
    xSemaphoreTake(mutex_, portMAX_DELAY);
    fetchActive_ = false;
    fetchStateSinceMs_ = millis();
    xSemaphoreGive(mutex_);
    ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(BETWEEN_COIN_REQUESTS_MS));
  }
}

void HeliosBalances::refreshCoin(HeliosCoin coin, const String& wallet,
                                  bool pricesOnly) {
  size_t index = heliosCoinIndex(coin);
  heliosMarkBalanceCoin(static_cast<uint32_t>(index));
  xSemaphoreTake(mutex_, portMAX_DELAY);
  snapshots_[index].refreshing = true;
  if (!pricesOnly)
    snapshots_[index].status = wallet.isEmpty() ? "NOT SET" : "UPDATING";
  xSemaphoreGive(mutex_);

  double balance = 0;
  double usd = 0;
  double cad = 0;
  double gbp = 0;
  double eur = 0;
  String balanceError;
  String priceError;
  bool balanceOk = !pricesOnly && !wallet.isEmpty() &&
                   fetchBalance(coin, wallet, balance, balanceError);
  bool persistBalance = false;
  double pendingIncrease = 0.0;
  xSemaphoreTake(mutex_, portMAX_DELAY);
  bool priceFetched = false;
  bool pricesOk = snapshots_[index].pricesAvailable &&
                  millis() - snapshots_[index].pricesUpdatedAt <
                      PRICE_CACHE_MS;
  if (pricesOk) {
    usd = snapshots_[index].priceUsd;
    cad = snapshots_[index].priceCad;
    gbp = snapshots_[index].priceGbp;
    eur = snapshots_[index].priceEur;
  }
  xSemaphoreGive(mutex_);
  if (!wallet.isEmpty() && !pricesOk) {
    lastPriceAttemptAt_[index] = millis();
    pricesOk = fetchPrices(coin, usd, cad, gbp, eur, priceError);
    priceFetched = pricesOk;
  }
  xSemaphoreTake(mutex_, portMAX_DELAY);
  if (wallets_[index] != wallet) {
    xSemaphoreGive(mutex_);
    return;
  }
  snapshots_[index].refreshing = false;
  if (balanceOk) {
    snapshots_[index].available = true;
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
  } else if (!pricesOnly && wallet.isEmpty()) {
    snapshots_[index].available = false;
    snapshots_[index].balance = 0;
    snapshots_[index].status = "NOT SET";
  } else if (!pricesOnly) {
    snapshots_[index].available = snapshots_[index].baselineReady;
    String reason = balanceError.isEmpty() ? "UNAVAILABLE" : balanceError;
    snapshots_[index].status = snapshots_[index].available
        ? String("CACHED (") + reason + ")"
        : reason;
  }
  if (pricesOk) {
    snapshots_[index].pricesAvailable = true;
    snapshots_[index].priceUsd = usd;
    snapshots_[index].priceCad = cad;
    snapshots_[index].priceGbp = gbp;
    snapshots_[index].priceEur = eur;
    if (priceFetched) snapshots_[index].pricesUpdatedAt = millis();
    snapshots_[index].priceStatus = "UPDATED";
  } else if (wallet.isEmpty()) {
    snapshots_[index].priceStatus = "NOT SET";
  } else {
    const String reason = priceError.isEmpty() ? "UNAVAILABLE" : priceError;
    snapshots_[index].priceStatus = snapshots_[index].pricesAvailable
        ? String("CACHED (") + reason + ")" : reason;
  }
  xSemaphoreGive(mutex_);
  if (balanceOk && persistBalance) {
    saveStoredBalanceState(index, wallet, balance, pendingIncrease);
  }
}

bool HeliosBalances::fetchBalance(HeliosCoin coin, const String& wallet,
                                  double& balance, String& error) {
  if (coin == HeliosCoin::GRLC) {
    String explorerError;
    {
      StreamString payload;
      const String url = "https://explorer.grlc.eu/addr.php?a=" +
                         addressPath(wallet) + "&api=1&op=balance";
      if (getPayload(url, true, payload, explorerError) &&
          parseGarlicoin(payload, balance)) return true;
      if (explorerError.isEmpty()) explorerError = "GRLC RESPONSE";
    }
    // Keep the working explorer first. If HTTPS fails, use the lightweight
    // community servers; release its payload before opening another socket.
    String scriptHash;
    if (!garlicoinElectrumScriptHash(wallet, scriptHash)) {
      error = "INVALID ADDRESS";
      return false;
    }
    for (const ElectrumServer& server : GRLC_ELECTRUM_SERVERS) {
      if (queryElectrum(server, scriptHash, balance, error)) return true;
      vTaskDelay(1);
    }
    error = explorerError;
    return false;
  }
  if (coin == HeliosCoin::XEC) {
    return getElectrumBalance(wallet, 0, 5, true, XEC_ELECTRUM_SERVERS,
                              balance, error, 100.0, "ecash");
  }
  if (coin == HeliosCoin::ZEC) {
    if (!(wallet.startsWith("t1") || wallet.startsWith("t3"))) {
      error = "USE TRANSPARENT ADDRESS";
      return false;
    }
    StreamString payload;
    String url = "https://api.zecblock.com/v1/addresses/" + addressPath(wallet) + "?limit=1";
    if (!getPayload(url, true, payload, error)) return false;
    if (parseZecBlock(payload, wallet, balance)) return true;
    error = "ZEC RESPONSE";
    return false;
  }
  if (coin == HeliosCoin::WJK) {
    StreamString payload;
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
  StreamString payload;
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

    case HeliosCoin::FIX:
      if (getPayload("https://explorer.fixedcoin.org/ext/getbalance/" +
                         wallet,
                     true, payload, primaryError) &&
          parseNumber(payload, balance)) {
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
                                 double& gbp, double& eur, String& error) {
  // Parse only the four prices. Full ticker metadata is unnecessary on a CYD.
  // Assign outputs only after the complete quote set passes validation.
  auto fetchQuotes = [&](const String& url, const char* geckoId,
                         String& reason) -> bool {
    StreamString payload;
    if (!getPayload(url, true, payload, reason)) return false;
    JsonDocument filter;
    if (geckoId) {
      for (const char* currency : {"usd", "cad", "gbp", "eur"})
        filter[geckoId][currency] = true;
    } else {
      for (const char* currency : {"USD", "CAD", "GBP", "EUR"})
        filter["quotes"][currency]["price"] = true;
    }
    JsonDocument document;
    if (deserializeJson(document, static_cast<const String&>(payload),
                        DeserializationOption::Filter(filter))) {
      reason = "PRICE RESPONSE";
      return false;
    }
    // ArduinoJson member proxies at different nesting depths have distinct
    // types. Assign to JsonVariant explicitly instead of mixing them in ?:.
    JsonVariant values[4];
    if (geckoId) {
      JsonObject quotes = document[geckoId];
      values[0] = quotes["usd"].as<JsonVariant>();
      values[1] = quotes["cad"].as<JsonVariant>();
      values[2] = quotes["gbp"].as<JsonVariant>();
      values[3] = quotes["eur"].as<JsonVariant>();
    } else {
      JsonObject quotes = document["quotes"];
      values[0] = quotes["USD"]["price"].as<JsonVariant>();
      values[1] = quotes["CAD"]["price"].as<JsonVariant>();
      values[2] = quotes["GBP"]["price"].as<JsonVariant>();
      values[3] = quotes["EUR"]["price"].as<JsonVariant>();
    }
    double prices[4];
    for (size_t i = 0; i < 4; ++i) {
      if (!values[i].is<double>()) {
        reason = "PRICE MISSING";
        return false;
      }
      prices[i] = values[i].as<double>();
      if (!isfinite(prices[i]) || prices[i] <= 0) {
        reason = "PRICE INVALID";
        return false;
      }
    }
    usd = prices[0]; cad = prices[1]; gbp = prices[2]; eur = prices[3];
    return true;
  };

  const char* geckoId = nullptr;
  switch (coin) {
    case HeliosCoin::GRLC: geckoId = "garlicoin"; break;
    case HeliosCoin::DGB: geckoId = "digibyte"; break;
    case HeliosCoin::BCH: geckoId = "bitcoin-cash"; break;
    case HeliosCoin::ZEC: geckoId = "zcash"; break;
    default: break;
  }
  String primaryError;
  // GRLC is inactive on Coinpaprika; use its verified CoinGecko ID directly.
  if (coin != HeliosCoin::GRLC &&
      fetchQuotes("https://api.coinpaprika.com/v1/tickers/" +
                      String(PRICE_IDS[heliosCoinIndex(coin)]) +
                      "?quotes=USD,CAD,GBP,EUR",
                  nullptr, primaryError)) return true;
  if (geckoId) {
    if (fetchQuotes("https://api.coingecko.com/api/v3/simple/price?ids=" +
                        String(geckoId) + "&vs_currencies=usd,cad,gbp,eur",
                    geckoId, error)) return true;
    if (!primaryError.isEmpty()) error = primaryError + "; " + error;
    return false;
  }
  if (coin == HeliosCoin::WJK) {
    // WJK is a native chain, not CoinGecko's unrelated WOJAK token. Its
    // compact converter supplies USD; reuse recent fiat ratios from any
    // successfully fetched quote set instead of issuing three extra calls.
    double cadRatio = 0, gbpRatio = 0, eurRatio = 0;
    xSemaphoreTake(mutex_, portMAX_DELAY);
    for (const auto& snapshot : snapshots_) {
      if (!snapshot.pricesAvailable || snapshot.priceUsd <= 0 ||
          millis() - snapshot.pricesUpdatedAt >= PRICE_CACHE_MS) continue;
      cadRatio = snapshot.priceCad / snapshot.priceUsd;
      gbpRatio = snapshot.priceGbp / snapshot.priceUsd;
      eurRatio = snapshot.priceEur / snapshot.priceUsd;
      break;
    }
    xSemaphoreGive(mutex_);
    if (isfinite(cadRatio) && isfinite(gbpRatio) && isfinite(eurRatio) &&
        cadRatio > 0 && gbpRatio > 0 && eurRatio > 0) {
      StreamString payload;
      if (getPayload("https://api.coinpaprika.com/v1/price-converter?base_currency_id=wjk-wojakcoin&quote_currency_id=usd-us-dollars&amount=1",
                     true, payload, error)) {
        JsonDocument filter;
        filter["price"] = true;
        filter["base_currency_id"] = true;
        filter["quote_currency_id"] = true;
        JsonDocument document;
        if (!deserializeJson(document, static_cast<const String&>(payload),
                             DeserializationOption::Filter(filter)) &&
            document["base_currency_id"] == "wjk-wojakcoin" &&
            document["quote_currency_id"] == "usd-us-dollars" &&
            document["price"].is<double>()) {
          const double nextUsd = document["price"].as<double>();
          const double nextCad = nextUsd * cadRatio;
          const double nextGbp = nextUsd * gbpRatio;
          const double nextEur = nextUsd * eurRatio;
          if (isfinite(nextUsd) && isfinite(nextCad) && isfinite(nextGbp) &&
              isfinite(nextEur) && nextUsd > 0 && nextCad > 0 &&
              nextGbp > 0 && nextEur > 0) {
            usd = nextUsd; cad = nextCad; gbp = nextGbp; eur = nextEur;
            return true;
          }
        }
        error = "WJK PRICE RESPONSE";
      }
    } else {
      error = "WAITING FOR FX";
    }
    error = primaryError + "; " + error;
    return false;
  }
  error = primaryError;
  return false;
}
