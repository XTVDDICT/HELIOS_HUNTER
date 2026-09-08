#pragma once

#include "SoloHunterMiner.h"

using HeliosMiningConfig = SoloHunterMiningConfig;
using HeliosMiningStats = SoloHunterMiningStats;

inline void heliosMinerBegin(const HeliosMiningConfig& config) {
  soloHunterMinerBegin(config);
}

inline void heliosMinerConfigure(const HeliosMiningConfig& config) {
  soloHunterMinerConfigure(config);
}

inline HeliosMiningStats heliosMinerGetStats() {
  return soloHunterMinerGetStats();
}

