#pragma once

#include <Arduino.h>

struct SoloHunterSha256Result {
  uint32_t hashes = 0;
  bool candidate = false;
  uint32_t nonce = 0;
  uint8_t hash[32] = {0};
};

bool soloHunterSha256Begin();
bool soloHunterSha256Mine(const uint32_t headerSwapped[20],
                          uint32_t& nextNonceSwapped,
                          uint16_t leadingZeroMask,
                          uint32_t maxHashes,
                          SoloHunterSha256Result& result);
