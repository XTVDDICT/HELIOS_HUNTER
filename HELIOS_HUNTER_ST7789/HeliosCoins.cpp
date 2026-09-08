#include "HeliosCoins.h"

namespace {

// RGB565 colors are intentionally distinct so every coin page is recognizable.
constexpr HeliosCoinProfile PROFILES[HELIOS_COIN_COUNT] = {
    {HeliosCoin::CHTA, "CHTA", "Cheetahcoin", "chta.heliospool.com", 3336,
     0xFBE0},
    {HeliosCoin::WJK, "WJK", "WojakCoin", "wjk.heliospool.com", 3337,
     0xF9A6},
    {HeliosCoin::DGB, "DGB", "DigiByte", "dgb.heliospool.com", 3335,
     0x2D7F},
    {HeliosCoin::BCH, "BCH", "Bitcoin Cash", "bch.heliospool.com", 3334,
     0x45E8},
    {HeliosCoin::BTC, "BTC", "Bitcoin", "btc.heliospool.com", 3333,
     0xFD20},
};

}  // namespace

const HeliosCoinProfile& heliosCoinProfile(HeliosCoin coin) {
  return PROFILES[heliosCoinIndex(coin)];
}

const HeliosCoinProfile& heliosCoinProfileAt(size_t index) {
  if (index >= HELIOS_COIN_COUNT) index = HELIOS_COIN_COUNT - 1;
  return PROFILES[index];
}

bool heliosCoinFromSymbol(String symbol, HeliosCoin& coin) {
  symbol.trim();
  symbol.toUpperCase();
  for (size_t i = 0; i < HELIOS_COIN_COUNT; ++i) {
    if (symbol == PROFILES[i].symbol) {
      coin = PROFILES[i].id;
      return true;
    }
  }
  return false;
}

size_t heliosCoinIndex(HeliosCoin coin) {
  size_t index = static_cast<size_t>(coin);
  return index < HELIOS_COIN_COUNT ? index : 0;
}
