#pragma once

#include <Arduino.h>

struct SoloHunterMiningConfig {
  bool enabled = false;
  String poolHost;
  uint16_t poolPort = 3333;
  String username;
  String worker;
  String password = "x";
};

struct SoloHunterMiningStats {
  float hashrateKh = 0.0f;
  float primaryHashrateKh = 0.0f;
  float auxiliaryHashrateKh = 0.0f;
  const char* auxiliaryMode = "WAITING";
  uint32_t auxiliarySliceYields = 0;
  uint32_t auxiliaryBalanceWaits = 0;
  uint32_t auxiliaryPartialBatches = 0;
  uint32_t auxiliaryRunBudgetUs = 0;
  uint32_t auxiliaryIdleTicks = 0;
  uint32_t auxiliaryTaskPriority = 0;
  uint64_t totalHashes = 0;
  uint32_t submittedShares = 0;
  uint32_t pendingShares = 0;
  uint32_t acceptedShares = 0;
  uint32_t rejectedShares = 0;
  uint32_t blocksFound = 0;
  uint32_t poolReconnects = 0;
  uint32_t uptimeSeconds = 0;
  uint32_t primaryStackFreeBytes = 0;
  uint32_t auxiliaryStackFreeBytes = 0;
  double bestDifficulty = 0.0;
  double poolDifficulty = 0.0;
  bool hardwareSha = false;
  String status = "OFF";
};

void soloHunterMinerBegin(const SoloHunterMiningConfig& config);
void soloHunterMinerConfigure(const SoloHunterMiningConfig& config);
SoloHunterMiningStats soloHunterMinerGetStats(bool includeStack = false);

// Cumulative wall-clock profiling for one helper-control epoch. No saved settings.
struct SoloHunterMiningProfile {
  uint32_t epoch = 0;
  uint32_t controlEpoch = 0;
  bool helperEnabled = true;
  uint64_t elapsedUs = 0;
  uint64_t hardwareCallUs = 0;
  uint64_t poolReadUs = 0;
  uint64_t poolReadCalls = 0;
  uint64_t poolReadBytes = 0;
  uint64_t poolMessages = 0;
  uint32_t helperRunBudgetUs = 20000;
  uint64_t candidateUs = 0;
  // Subtimers overlap candidateUs; do not add them to total elapsed time.
  uint64_t candidateVerifyUs = 0;
  uint64_t hardwareCandidates = 0;
  uint64_t confirmedCandidates = 0;
  uint64_t shareSubmitUs = 0;
  uint64_t shareSubmitCalls = 0;
  uint64_t shareSubmitFailures = 0;
  uint64_t shareWriteCalls = 0;
  uint64_t shareBytes = 0;
  uint64_t hardwareCalls = 0;
  uint64_t hardwareHashes = 0;
  uint64_t softwareHashes = 0;
  uint64_t helperHashes = 0;
};

bool soloHunterMinerSetHelperEnabled(bool enabled);
bool soloHunterMinerSetHelperRunBudgetUs(uint32_t budgetUs);
SoloHunterMiningProfile soloHunterMinerGetProfile();
