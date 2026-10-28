#pragma once

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <freertos/task.h>

// HTTPClient uses connect(host, port, timeout). Install cooperative BIO
// callbacks before the TLS handshake, and retain them for headers/body reads.
// A body writer alone cannot yield while the TLS library is receiving data.
class HeliosCooperativeTls : public WiFiClientSecure {
 public:
  explicit HeliosCooperativeTls(uint32_t requestTimeoutMs)
      : requestTimeoutMs_(requestTimeoutMs) {}
  using WiFiClientSecure::connect;

  int connect(const char* host, uint16_t port, int32_t timeout) override {
    requestStartedAt_ = millis();
    // The base client prepares its TLS context but defers the handshake.
    // Reset this on every connection, including redirects/reconnects.
    setPlainStart();
    if (!WiFiClientSecure::connect(host, port, timeout)) {
      _stillinPlainStart = false;
      return 0;
    }
    return finishConnect();
  }

  int connect(IPAddress ip, uint16_t port, int32_t timeout) override {
    requestStartedAt_ = millis();
    setPlainStart();
    if (!WiFiClientSecure::connect(ip, port, timeout)) {
      _stillinPlainStart = false;
      return 0;
    }
    return finishConnect();
  }

 private:
  int finishConnect() {
    mbedtls_ssl_set_bio(&sslclient->ssl_ctx, this, send, receive, nullptr);
    const int connected = WiFiClientSecure::startTLS();
    // Failed startTLS() stops the socket but leaves its plain-start flag set.
    // Restore secure-mode cleanup so available() cannot poll socket -1.
    _stillinPlainStart = false;
    return connected;
  }

  bool allowIo() const {
    // Block for an actual RTOS tick, even when the socket is continuously
    // readable. taskYIELD() or a body deadline does not ensure this pause.
    vTaskDelay(1);
    return static_cast<uint32_t>(millis() - requestStartedAt_) < requestTimeoutMs_;
  }

  static int receive(void* context, unsigned char* data, size_t size) {
    auto& client = *static_cast<HeliosCooperativeTls*>(context);
    if (!client.allowIo()) return MBEDTLS_ERR_NET_RECV_FAILED;
    // Arduino's default BIO likewise supplies the socket integer's address.
    return mbedtls_net_recv(&client.sslclient->socket, data, size);
  }

  static int send(void* context, const unsigned char* data, size_t size) {
    auto& client = *static_cast<HeliosCooperativeTls*>(context);
    if (!client.allowIo()) return MBEDTLS_ERR_NET_SEND_FAILED;
    return mbedtls_net_send(&client.sslclient->socket, data, size);
  }

  const uint32_t requestTimeoutMs_;
  uint32_t requestStartedAt_ = 0;
};
