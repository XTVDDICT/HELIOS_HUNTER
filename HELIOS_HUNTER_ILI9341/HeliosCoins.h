#pragma once

#include <Arduino.h>

enum class HeliosCoin : uint8_t {
  CHTA = 0,
  WJK,
  DGB,
  BCH,
  BTC,
  Count
};

struct HeliosCoinProfile {
  HeliosCoin id;
  const char* symbol;
  const char* name;
  const char* poolHost;
  uint16_t poolPort;
  uint16_t color;
};

constexpr size_t HELIOS_COIN_COUNT = static_cast<size_t>(HeliosCoin::Count);

const HeliosCoinProfile& heliosCoinProfile(HeliosCoin coin);
const HeliosCoinProfile& heliosCoinProfileAt(size_t index);
bool heliosCoinFromSymbol(String symbol, HeliosCoin& coin);
size_t heliosCoinIndex(HeliosCoin coin);

