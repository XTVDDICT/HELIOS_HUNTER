#include "HeliosNetworkMemory.h"
#include <atomic>

namespace {
StaticSemaphore_t networkMemoryStorage;
SemaphoreHandle_t networkMemoryMutex = nullptr;
std::atomic<bool> balanceNetworkBusy(false);
std::atomic<unsigned> displayWorkDepth{0};
}

void heliosNetworkMemoryBegin() {
  if (!networkMemoryMutex) {
    networkMemoryMutex = xSemaphoreCreateMutexStatic(&networkMemoryStorage);
  }
}

bool heliosBalanceNetworkBusy() {
  return balanceNetworkBusy.load(std::memory_order_acquire);
}

HeliosNetworkMemoryGuard::HeliosNetworkMemoryGuard(TickType_t waitTicks,
                                                 bool balanceWork)
    : locked_(networkMemoryMutex &&
              xSemaphoreTake(networkMemoryMutex, waitTicks) == pdTRUE),
      balanceWork_(locked_ && balanceWork) {
  if (balanceWork_) balanceNetworkBusy.store(true, std::memory_order_release);
}

HeliosNetworkMemoryGuard::~HeliosNetworkMemoryGuard() {
  if (balanceWork_) balanceNetworkBusy.store(false, std::memory_order_release);
  if (locked_) xSemaphoreGive(networkMemoryMutex);
}

bool heliosDisplayBusy() {
  return displayWorkDepth.load(std::memory_order_acquire) != 0;
}
HeliosDisplayWorkGuard::HeliosDisplayWorkGuard() {
  displayWorkDepth.fetch_add(1, std::memory_order_acq_rel);
}
HeliosDisplayWorkGuard::~HeliosDisplayWorkGuard() {
  displayWorkDepth.fetch_sub(1, std::memory_order_release);
}
