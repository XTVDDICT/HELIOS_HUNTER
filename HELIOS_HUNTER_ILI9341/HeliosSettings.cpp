#include "HeliosSettings.h"

#include <Preferences.h>

namespace {

constexpr const char* NAMESPACE_NAME = "helioshunter";
constexpr const char* WALLET_KEYS[HELIOS_COIN_COUNT] = {
    "w_chta", "w_wjk", "w_dgb", "w_bch", "w_btc"};
constexpr const char* LEGACY_POOL_HOST_KEYS[HELIOS_COIN_COUNT] = {
    "ph_chta", "ph_wjk", "ph_dgb", "ph_bch", "ph_btc"};
constexpr const char* LEGACY_POOL_PORT_KEYS[HELIOS_COIN_COUNT] = {
    "pp_chta", "pp_wjk", "pp_dgb", "pp_bch", "pp_btc"};

String cleaned(String value, size_t maximumLength) {
  value.trim();
  if (value.length() > maximumLength) value.remove(maximumLength);
  return value;
}

}  // namespace

void HeliosSettings::begin() {
  Preferences prefs;
  prefs.begin(NAMESPACE_NAME, true);
  for (size_t i = 0; i < HELIOS_COIN_COUNT; ++i) {
    data_.wallets[i] = prefs.getString(WALLET_KEYS[i], "");
  }
  uint8_t legacyCoin = prefs.getUChar("coin", 0);
  if (legacyCoin >= HELIOS_COIN_COUNT) legacyCoin = 0;
  const HeliosCoinProfile& legacyProfile = heliosCoinProfileAt(legacyCoin);
  String legacyHost = prefs.getString(LEGACY_POOL_HOST_KEYS[legacyCoin],
                                      legacyProfile.poolHost);
  uint16_t legacyPort = prefs.getUShort(LEGACY_POOL_PORT_KEYS[legacyCoin],
                                        legacyProfile.poolPort);
  data_.miningPoolHost = prefs.getString("minehost", legacyHost);
  data_.miningPoolPort = prefs.getUShort("mineport", legacyPort);
  data_.miningUsername = prefs.getString(
      "mineuser", prefs.getString(WALLET_KEYS[legacyCoin], ""));
  if (data_.miningPoolHost.isEmpty()) data_.miningPoolHost = legacyProfile.poolHost;
  if (data_.miningPoolPort == 0) data_.miningPoolPort = legacyProfile.poolPort;
  data_.worker = prefs.getString("worker", "helios-hunter");
  data_.stratumPassword = prefs.getString("pass", "x");
  data_.miningEnabled = prefs.getBool("enabled", false);
  data_.flipped = prefs.getBool("flipped", false);
  data_.brightness = prefs.getUChar("bright", 220);
  data_.fiatCurrency = prefs.getUChar("currency", 0);
  if (data_.fiatCurrency > 2) data_.fiatCurrency = 0;
  prefs.end();
}

const HeliosSettingsData& HeliosSettings::data() const { return data_; }

String HeliosSettings::wallet(HeliosCoin coin) const {
  return data_.wallets[heliosCoinIndex(coin)];
}

void HeliosSettings::saveText(const char* key, const String& value) {
  Preferences prefs;
  prefs.begin(NAMESPACE_NAME, false);
  prefs.putString(key, value);
  prefs.end();
}

void HeliosSettings::setWallet(HeliosCoin coin, String value) {
  size_t index = heliosCoinIndex(coin);
  data_.wallets[index] = cleaned(value, 128);
  saveText(WALLET_KEYS[index], data_.wallets[index]);
}

void HeliosSettings::setMining(String host, uint16_t port, String username,
                               String worker, String password, bool enabled) {
  host = cleaned(host, 96);
  username = cleaned(username, 128);
  worker = cleaned(worker, 32);
  password = cleaned(password, 64);
  if (worker.isEmpty()) worker = "helios-hunter";
  if (password.isEmpty()) password = "x";
  data_.miningPoolHost = host;
  data_.miningPoolPort = port == 0 ? 3333 : port;
  data_.miningUsername = username;
  data_.worker = worker;
  data_.stratumPassword = password;
  data_.miningEnabled = enabled;
  Preferences prefs;
  prefs.begin(NAMESPACE_NAME, false);
  prefs.putString("minehost", data_.miningPoolHost);
  prefs.putUShort("mineport", data_.miningPoolPort);
  prefs.putString("mineuser", data_.miningUsername);
  prefs.putString("worker", data_.worker);
  prefs.putString("pass", data_.stratumPassword);
  prefs.putBool("enabled", data_.miningEnabled);
  prefs.end();
}

void HeliosSettings::setMiningEnabled(bool enabled) {
  data_.miningEnabled = enabled;
  Preferences prefs;
  prefs.begin(NAMESPACE_NAME, false);
  prefs.putBool("enabled", enabled);
  prefs.end();
}

void HeliosSettings::setFlipped(bool flipped) {
  data_.flipped = flipped;
  Preferences prefs;
  prefs.begin(NAMESPACE_NAME, false);
  prefs.putBool("flipped", flipped);
  prefs.end();
}

void HeliosSettings::setBrightness(uint8_t brightness) {
  data_.brightness = brightness;
  Preferences prefs;
  prefs.begin(NAMESPACE_NAME, false);
  prefs.putUChar("bright", brightness);
  prefs.end();
}

void HeliosSettings::setFiatCurrency(uint8_t currency) {
  data_.fiatCurrency = currency <= 2 ? currency : 0;
  Preferences prefs;
  prefs.begin(NAMESPACE_NAME, false);
  prefs.putUChar("currency", data_.fiatCurrency);
  prefs.end();
}
