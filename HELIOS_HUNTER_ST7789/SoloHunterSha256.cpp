#include "SoloHunterSha256.h"

#if defined(CONFIG_IDF_TARGET_ESP32)

#include <sha/sha_parallel_engine.h>
#include <soc/dport_access.h>
#include <soc/dport_reg.h>
#include <soc/hwcrypto_reg.h>

namespace {

bool hardwareLocked = false;

struct PipelineArgs {
  volatile uint32_t* shaBase;
  const uint32_t* header;
  uint32_t nonce;
  uint32_t limit;
  uint32_t filterMask;
  uint32_t hashes;
  uint32_t found;
};

static_assert(sizeof(PipelineArgs) == 28, "ESP32 pipeline arguments changed");

void enableShaPeripheral() {
  DPORT_REG_SET_BIT(DPORT_PERI_CLK_EN_REG, DPORT_PERI_EN_SHA);
  DPORT_REG_CLR_BIT(DPORT_PERI_RST_EN_REG,
                    DPORT_PERI_EN_SHA | DPORT_PERI_EN_SECUREBOOT);
}

bool tryLockShaHardware() {
  if (!esp_sha_try_lock_engine(SHA1)) return false;
  if (!esp_sha_try_lock_engine(SHA2_256)) {
    esp_sha_unlock_engine(SHA1);
    return false;
  }
  // SHA-384 and SHA-512 share the third physical engine on classic ESP32.
  if (!esp_sha_try_lock_engine(SHA2_384)) {
    esp_sha_unlock_engine(SHA2_256);
    esp_sha_unlock_engine(SHA1);
    return false;
  }
  return true;
}

void unlockShaHardware() {
  esp_sha_unlock_engine(SHA2_384);
  esp_sha_unlock_engine(SHA2_256);
  esp_sha_unlock_engine(SHA1);
}

void readDigest(uint8_t output[32]) {
  uint32_t* words = reinterpret_cast<uint32_t*>(output);
  DPORT_INTERRUPT_DISABLE();
  words[7] =
      __builtin_bswap32(DPORT_SEQUENCE_REG_READ(SHA_TEXT_BASE + 7 * 4));
  words[6] =
      __builtin_bswap32(DPORT_SEQUENCE_REG_READ(SHA_TEXT_BASE + 6 * 4));
  words[5] =
      __builtin_bswap32(DPORT_SEQUENCE_REG_READ(SHA_TEXT_BASE + 5 * 4));
  words[4] =
      __builtin_bswap32(DPORT_SEQUENCE_REG_READ(SHA_TEXT_BASE + 4 * 4));
  words[3] =
      __builtin_bswap32(DPORT_SEQUENCE_REG_READ(SHA_TEXT_BASE + 3 * 4));
  words[2] =
      __builtin_bswap32(DPORT_SEQUENCE_REG_READ(SHA_TEXT_BASE + 2 * 4));
  words[1] =
      __builtin_bswap32(DPORT_SEQUENCE_REG_READ(SHA_TEXT_BASE + 1 * 4));
  words[0] =
      __builtin_bswap32(DPORT_SEQUENCE_REG_READ(SHA_TEXT_BASE + 0 * 4));
  DPORT_INTERRUPT_RESTORE();
}

IRAM_ATTR void runPipeline(PipelineArgs& args) {
  // Register scheduling follows the pipelined hardware-SHA technique used by
  // BitsyMiner (Justin Williams, GPL-3.0-or-later). Prepare SHA_TEXT while the
  // engine is busy, then synchronize before issuing the next command.
  __asm__ __volatile__(
      "l32i     a3,  %[args], 0\n"
      "l32i     a4,  %[args], 4\n"
      "l32i     a2,  %[args], 8\n"
      "l32i     a7,  %[args], 12\n"
      "l32i     a9,  %[args], 16\n"
      "movi     a5,  144\n"
      "add.n    a5,  a5, a3\n"
      "movi.n   a6,  0\n"
      "movi.n   a10, 1\n"

      "10:\n"
      // Load block one and start the first compression.
      "l32i.n   a8,  a4, 0\n"
      "s32i.n   a8,  a3, 0\n"
      "l32i.n   a8,  a4, 4\n"
      "s32i.n   a8,  a3, 4\n"
      "l32i.n   a8,  a4, 8\n"
      "s32i.n   a8,  a3, 8\n"
      "l32i.n   a8,  a4, 12\n"
      "s32i.n   a8,  a3, 12\n"
      "l32i.n   a8,  a4, 16\n"
      "s32i.n   a8,  a3, 16\n"
      "l32i.n   a8,  a4, 20\n"
      "s32i.n   a8,  a3, 20\n"
      "l32i.n   a8,  a4, 24\n"
      "s32i.n   a8,  a3, 24\n"
      "l32i.n   a8,  a4, 28\n"
      "s32i.n   a8,  a3, 28\n"
      "l32i.n   a8,  a4, 32\n"
      "s32i.n   a8,  a3, 32\n"
      "l32i.n   a8,  a4, 36\n"
      "s32i.n   a8,  a3, 36\n"
      "l32i.n   a8,  a4, 40\n"
      "s32i.n   a8,  a3, 40\n"
      "l32i.n   a8,  a4, 44\n"
      "s32i.n   a8,  a3, 44\n"
      "l32i.n   a8,  a4, 48\n"
      "s32i.n   a8,  a3, 48\n"
      "l32i.n   a8,  a4, 52\n"
      "s32i.n   a8,  a3, 52\n"
      "l32i.n   a8,  a4, 56\n"
      "s32i.n   a8,  a3, 56\n"
      "l32i.n   a8,  a4, 60\n"
      "s32i.n   a8,  a3, 60\n"
      "s32i.n   a10, a5, 0\n"
      "memw\n"

      // Fill the nonce-bearing tail while the first compression is running.
      "l32i     a8,  a4, 64\n"
      "s32i.n   a8,  a3, 0\n"
      "l32i     a8,  a4, 68\n"
      "s32i.n   a8,  a3, 4\n"
      "l32i     a8,  a4, 72\n"
      "s32i.n   a8,  a3, 8\n"
      "s32i.n   a2,  a3, 12\n"
      "movi.n   a8,  1\n"
      "slli     a8,  a8, 31\n"
      "s32i.n   a8,  a3, 16\n"
      "movi.n   a8,  0\n"
      "s32i.n   a8,  a3, 20\n"
      "s32i.n   a8,  a3, 24\n"
      "s32i.n   a8,  a3, 28\n"
      "s32i.n   a8,  a3, 32\n"
      "s32i.n   a8,  a3, 36\n"
      "s32i.n   a8,  a3, 40\n"
      "s32i.n   a8,  a3, 44\n"
      "s32i.n   a8,  a3, 48\n"
      "s32i.n   a8,  a3, 52\n"
      "s32i.n   a8,  a3, 56\n"
      "movi     a8,  640\n"
      "s32i.n   a8,  a3, 60\n"

      "11:\n"
      "l32i.n   a8,  a5, 12\n"
      "bnez.n   a8,  11b\n"
      "s32i.n   a10, a5, 4\n"
      "memw\n"

      "12:\n"
      "l32i.n   a8,  a5, 12\n"
      "bnez.n   a8,  12b\n"
      "s32i.n   a10, a5, 8\n"
      "memw\n"
      "13:\n"
      "l32i.n   a8,  a5, 12\n"
      "bnez.n   a8,  13b\n"

      // SHA_LOAD replaced words 0-7 with the first digest. Words 9-14
      // are already zero from the tail block, so only padding changes.
      "movi.n   a8,  1\n"
      "slli     a8,  a8, 31\n"
      "s32i.n   a8,  a3, 32\n"
      "movi     a8,  256\n"
      "s32i.n   a8,  a3, 60\n"
      "s32i.n   a10, a5, 0\n"
      "memw\n"

      // Account for this nonce while the second SHA is running.
      "addi.n   a6,  a6, 1\n"
      "addi.n   a2,  a2, 1\n"
      "addi.n   a7,  a7, -1\n"

      "14:\n"
      "l32i.n   a8,  a5, 12\n"
      "bnez.n   a8,  14b\n"
      "s32i.n   a10, a5, 8\n"
      "memw\n"
      "15:\n"
      "l32i.n   a8,  a5, 12\n"
      "bnez.n   a8,  15b\n"

      // Only return hashes that can still meet the current pool target.
      "l16ui    a8,  a3, 28\n"
      "and      a8,  a8, a9\n"
      "beqz.n   a8,  17f\n"
      "beqz.n   a7,  18f\n"
      "j        10b\n"

      "17:\n"
      "movi.n   a8,  1\n"
      "s32i.n   a8,  %[args], 24\n"
      "j        19f\n"
      "18:\n"
      "movi.n   a8,  0\n"
      "s32i.n   a8,  %[args], 24\n"
      "19:\n"
      "s32i.n   a2,  %[args], 8\n"
      "s32i.n   a6,  %[args], 20\n"
      "memw\n"
      :
      : [args] "r"(&args)
      : "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "a10",
        "memory");
}

bool mineLocked(const uint32_t headerSwapped[20],
                uint32_t& nextNonceSwapped, uint16_t leadingZeroMask,
                uint32_t maxHashes, SoloHunterSha256Result& result) {
  if (maxHashes == 0) return false;

  PipelineArgs args;
  args.shaBase = reinterpret_cast<volatile uint32_t*>(SHA_TEXT_BASE);
  args.header = headerSwapped;
  args.nonce = nextNonceSwapped;
  args.limit = maxHashes;
  args.filterMask = leadingZeroMask;
  args.hashes = 0;
  args.found = 0;

  enableShaPeripheral();
  runPipeline(args);
  nextNonceSwapped = args.nonce;
  result.hashes = args.hashes;
  result.candidate = args.found != 0;
  if (result.candidate) {
    result.nonce = __builtin_bswap32(nextNonceSwapped - 1);
    readDigest(result.hash);
  }
  return true;
}

bool hardwareSelfTest() {
  static const uint8_t genesisHeader[80] = {
      0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3b, 0xa3, 0xed, 0xfd,
      0x7a, 0x7b, 0x12, 0xb2, 0x7a, 0xc7, 0x2c, 0x3e, 0x67, 0x76,
      0x8f, 0x61, 0x7f, 0xc8, 0x1b, 0xc3, 0x88, 0x8a, 0x51, 0x32,
      0x3a, 0x9f, 0xb8, 0xaa, 0x4b, 0x1e, 0x5e, 0x4a, 0x29, 0xab,
      0x5f, 0x49, 0xff, 0xff, 0x00, 0x1d, 0x1d, 0xac, 0x2b, 0x7c};
  static const uint8_t expectedHash[32] = {
      0x6f, 0xe2, 0x8c, 0x0a, 0xb6, 0xf1, 0xb3, 0x72, 0xc1, 0xa6,
      0xa2, 0x46, 0xae, 0x63, 0xf7, 0x4f, 0x93, 0x1e, 0x83, 0x65,
      0xe1, 0x5a, 0x08, 0x9c, 0x68, 0xd6, 0x19, 0x00, 0x00, 0x00,
      0x00, 0x00};

  uint32_t swappedHeader[20];
  for (size_t i = 0; i < 20; ++i) {
    uint32_t word;
    memcpy(&word, genesisHeader + i * 4, sizeof(word));
    swappedHeader[i] = __builtin_bswap32(word);
  }

  uint32_t nonce = __builtin_bswap32(0x7c2bac1d);
  SoloHunterSha256Result result;
  if (!mineLocked(swappedHeader, nonce, 0xFFFF, 1, result)) return false;
  return result.candidate && result.hashes == 1 &&
         result.nonce == 0x7c2bac1d &&
         memcmp(result.hash, expectedHash, sizeof(expectedHash)) == 0;
}

}  // namespace

bool soloHunterSha256Begin() {
  if (hardwareLocked) return true;
  if (!tryLockShaHardware()) return false;
  bool ok = hardwareSelfTest();
  if (ok) {
    // Keep SHA reserved for mining so concurrent TLS uses software SHA.
    hardwareLocked = true;
  } else {
    unlockShaHardware();
  }
  return ok;
}

bool soloHunterSha256Mine(const uint32_t headerSwapped[20],
                           uint32_t& nextNonceSwapped,
                           uint16_t leadingZeroMask, uint32_t maxHashes,
                           SoloHunterSha256Result& result) {
  if (!hardwareLocked) return false;
  return mineLocked(headerSwapped, nextNonceSwapped, leadingZeroMask,
                    maxHashes, result);
}

#else

bool soloHunterSha256Begin() {
  return false;
}

bool soloHunterSha256Mine(const uint32_t[20], uint32_t&, uint16_t, uint32_t,
                          SoloHunterSha256Result&) {
  return false;
}

#endif
