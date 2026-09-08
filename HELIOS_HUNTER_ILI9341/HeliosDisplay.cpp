#include "HeliosDisplay.h"
#include "HeliosPanel.h"

#include <WiFi.h>

#include "HeliosAssets.h"

namespace {

constexpr uint16_t BLACK = 0x0000;
constexpr uint16_t PANEL = 0x1082;
constexpr uint16_t PANEL_LIGHT = 0x18E3;
constexpr uint16_t WHITE = 0xFFFF;
constexpr uint16_t MUTED = 0xAD55;
constexpr uint16_t HELIOS_GREEN = 0x47E9;
constexpr uint16_t HELIOS_AMBER = 0xFCC0;
constexpr uint16_t SUCCESS = 0x4E89;
constexpr uint16_t DANGER = 0xF2A6;
constexpr int32_t SWIPE_DISTANCE = 45;
constexpr uint8_t LED_RED_PIN = 4;
constexpr uint8_t LED_GREEN_PIN = 16;
constexpr uint8_t LED_BLUE_PIN = 17;

class HeliosLgfx : public lgfx::LGFX_Device {
 public:
  HeliosLgfx() {
    {
      auto cfg = bus_.config();
      cfg.spi_host = HSPI_HOST;
      cfg.spi_mode = 0;
#if defined(HELIOS_PANEL_ST7789)
      cfg.freq_write = 40000000;
#else
      cfg.freq_write = 27000000;
#endif
      cfg.freq_read = 16000000;
      cfg.spi_3wire = false;
      cfg.use_lock = true;
      cfg.dma_channel = 1;
      cfg.pin_sclk = 14;
      cfg.pin_mosi = 13;
      cfg.pin_miso = 12;
      cfg.pin_dc = 2;
      bus_.config(cfg);
      panel_.setBus(&bus_);
    }
    {
      auto cfg = panel_.config();
      cfg.pin_cs = 15;
      cfg.pin_rst = -1;
      cfg.pin_busy = -1;
      cfg.memory_width = 240;
      cfg.memory_height = 320;
      cfg.panel_width = 240;
      cfg.panel_height = 320;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = true;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;
      panel_.config(cfg);
    }
    {
      auto cfg = light_.config();
      cfg.pin_bl = 21;
      cfg.invert = false;
      cfg.freq = 44100;
      cfg.pwm_channel = 7;
      light_.config(cfg);
      panel_.setLight(&light_);
    }
    {
      auto cfg = touch_.config();
      cfg.x_min = 300;
      cfg.x_max = 3900;
      cfg.y_min = 3700;
      cfg.y_max = 200;
      cfg.pin_int = -1;
      cfg.bus_shared = false;
      cfg.offset_rotation = 0;
      cfg.spi_host = -1;
      cfg.freq = 1000000;
      cfg.pin_sclk = 25;
      cfg.pin_mosi = 32;
      cfg.pin_miso = 39;
      cfg.pin_cs = 33;
      touch_.config(cfg);
      panel_.setTouch(&touch_);
    }
    setPanel(&panel_);
  }

 private:
#if defined(HELIOS_PANEL_ST7789)
  lgfx::Panel_ST7789 panel_;
#else
  lgfx::Panel_ILI9341 panel_;
#endif
  lgfx::Bus_SPI bus_;
  lgfx::Light_PWM light_;
  lgfx::Touch_XPT2046 touch_;
};

HeliosLgfx display;

void drawLabel(int16_t x, int16_t y, const char* label,
               uint16_t background = PANEL) {
  display.setTextDatum(lgfx::top_left);
  display.setTextSize(1);
  display.setTextColor(MUTED, background);
  display.drawString(label, x, y, 2);
}

void drawValue(int16_t x, int16_t y, int16_t width, const String& value,
               uint16_t color = WHITE, uint16_t background = PANEL) {
  display.fillRect(x, y, width, 18, background);
  display.setTextDatum(lgfx::top_left);
  display.setTextSize(1);
  display.setTextColor(color, background);
  display.drawString(value, x, y, 2);
}

struct PngAsset {
  const uint8_t* data;
  size_t size;
};

PngAsset coinLogo(HeliosCoin coin) {
  switch (coin) {
    case HeliosCoin::CHTA:
      return {CHTA_LOGO, CHTA_LOGO_SIZE};
    case HeliosCoin::WJK:
      return {WJK_LOGO, WJK_LOGO_SIZE};
    case HeliosCoin::DGB:
      return {DGB_LOGO, DGB_LOGO_SIZE};
    case HeliosCoin::BCH:
      return {BCH_LOGO, BCH_LOGO_SIZE};
    case HeliosCoin::BTC:
      return {BTC_LOGO, BTC_LOGO_SIZE};
  }
  return {BTC_LOGO, BTC_LOGO_SIZE};
}

void drawBorder(uint16_t accent) {
  display.drawRect(0, 0, 320, 240, accent);
  display.drawRect(1, 1, 318, 238, accent);
  display.drawRect(2, 2, 316, 236, PANEL_LIGHT);
}

}  // namespace

void HeliosDisplay::begin(HeliosSettings* settings, HeliosBalances* balances) {
  settings_ = settings;
  balances_ = balances;
  display.init();
  display.setColorDepth(16);
  display.setRotation(settings_->data().flipped ? 3 : 1);
  display.setBrightness(settings_->data().brightness);
  appliedFlipped_ = settings_->data().flipped;
  appliedBrightness_ = settings_->data().brightness;
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
  digitalWrite(LED_RED_PIN, HIGH);
  digitalWrite(LED_GREEN_PIN, HIGH);
  digitalWrite(LED_BLUE_PIN, HIGH);
  display.fillScreen(BLACK);
  page_ = 0;
  draw(true);
}

void HeliosDisplay::loop() {
  if (settings_->data().brightness != appliedBrightness_) {
    appliedBrightness_ = settings_->data().brightness;
    display.setBrightness(appliedBrightness_);
  }
  if (settings_->data().flipped != appliedFlipped_) {
    appliedFlipped_ = settings_->data().flipped;
    display.setRotation(appliedFlipped_ ? 3 : 1);
    redrawRequested_ = true;
  }
  checkBalanceIncreases();
  handleTouch();
  if (blockPopupActive_) return;
  if (redrawRequested_) {
    redrawRequested_ = false;
    draw(true);
  } else if (millis() - lastDrawAt_ >= 1000) {
    draw(false);
  }
}

void HeliosDisplay::showHome() {
  page_ = 0;
  draw(true);
}

uint8_t HeliosDisplay::page() const { return page_; }

void HeliosDisplay::requestRedraw() { redrawRequested_ = true; }

String HeliosDisplay::rateText(float kh) const {
  if (kh >= 1000.0f) return String(kh / 1000.0f, 2) + " MH/s";
  return String(kh, 0) + " kH/s";
}

void HeliosDisplay::drawHome(const HeliosMiningStats& stats) {
  const HeliosSettingsData& cfg = settings_->data();
  display.fillScreen(BLACK);
  drawBorder(HELIOS_GREEN);
  display.drawPng(HELIOS_MAIN, HELIOS_MAIN_SIZE, 177, 6);

  display.setTextDatum(lgfx::top_left);
  display.setTextColor(HELIOS_GREEN, BLACK);
  display.drawString("HELIOS_HUNTER", 11, 11, 2);
  display.setTextColor(MUTED, BLACK);
  display.drawString("MINING HASHRATE", 11, 34, 1);
  display.drawString("SHARES A / R", 11, 79, 1);

  display.drawFastHLine(8, 109, 304, HELIOS_GREEN);
  display.fillRoundRect(10, 116, 300, 48, 4, PANEL);
  display.setTextColor(MUTED, PANEL);
  display.drawString("MINING POOL", 19, 123, 1);
  display.setTextColor(HELIOS_GREEN, PANEL);
  display.drawString(cfg.miningPoolHost, 19, 140, 2);
  display.setTextDatum(lgfx::middle_right);
  display.setTextColor(WHITE, PANEL);
  display.drawString(String(cfg.miningPoolPort), 301, 146, 1);

  drawLabel(12, 174, "BEST DIFFICULTY", BLACK);
  drawLabel(169, 174, "WEB UI", BLACK);
  drawHomeValues(stats);
  drawFooter(HELIOS_GREEN);
}

void HeliosDisplay::drawHomeValues(const HeliosMiningStats& stats) {
  display.fillRect(10, 43, 162, 28, BLACK);
  display.setTextDatum(lgfx::top_left);
  display.setTextColor(WHITE, BLACK);
  display.drawString(rateText(stats.hashrateKh), 11, 43, 4);
  drawValue(11, 91, 160,
            String(stats.acceptedShares) + " / " +
                String(stats.rejectedShares),
            WHITE, BLACK);
  drawValue(12, 189, 145, String(stats.bestDifficulty, 4), WHITE, BLACK);
  display.fillRect(169, 189, 137, 18, BLACK);
  display.setTextDatum(lgfx::top_left);
  display.setTextColor(WiFi.status() == WL_CONNECTED ? SUCCESS : DANGER,
                       BLACK);
  String ip = WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString()
                                            : "HELIOS_HUNTER_SETUP";
  display.drawString(ip, 169, 189, 2);
}

void HeliosDisplay::drawCoin(const HeliosCoinProfile& profile,
                             const HeliosMiningStats& stats) {
  display.fillScreen(BLACK);
  drawBorder(profile.color);
  PngAsset logo = coinLogo(profile.id);
  display.drawPng(logo.data, logo.size, 238, 7);

  display.setTextDatum(lgfx::top_left);
  display.setTextColor(HELIOS_GREEN, BLACK);
  display.drawString("HELIOS_HUNTER", 11, 10, 2);
  display.setTextColor(MUTED, BLACK);
  display.drawString("MINING HASHRATE", 11, 34, 1);
  display.setTextColor(profile.color, BLACK);
  display.drawString(profile.symbol, 238, 84, 2);

  display.fillRoundRect(10, 106, 300, 72, 4, PANEL);
  display.drawRoundRect(10, 106, 300, 72, 4, profile.color);
  display.setTextColor(MUTED, PANEL);
  display.drawString("ADDRESS BALANCE", 19, 112, 1);

  display.fillRoundRect(10, 184, 145, 36, 4, PANEL);
  display.fillRoundRect(165, 184, 145, 36, 4, PANEL);
  drawLabel(18, 188, "SHARES A / R");
  drawLabel(173, 188, "BEST DIFF");
  drawCoinValues(profile, stats);
  drawFooter(profile.color);
}

void HeliosDisplay::drawCoinValues(const HeliosCoinProfile& profile,
                                   const HeliosMiningStats& stats) {
  bool mining = settings_->data().miningEnabled;
  display.fillRect(10, 43, 215, 28, BLACK);
  display.setTextDatum(lgfx::top_left);
  display.setTextColor(WHITE, BLACK);
  display.drawString(mining ? rateText(stats.hashrateKh) : "--", 11, 43, 4);
  display.fillRect(10, 76, 215, 18, BLACK);

  HeliosBalanceSnapshot snapshot = balances_->get(profile.id);
  display.fillRect(18, 124, 284, 29, PANEL);
  display.setTextColor(profile.color, PANEL);
  display.drawString(balanceText(profile) + " " + profile.symbol, 18, 124, 4);
  display.fillRect(18, 153, 284, 23, PANEL);
  display.setTextColor(WHITE, PANEL);
  uint8_t currency = settings_->data().fiatCurrency;
  const char* currencySymbol =
      currency == 1 ? "C$" : currency == 2 ? "\xC2\xA3" : "$";
  double price = currency == 1 ? snapshot.priceCad
                               : currency == 2 ? snapshot.priceGbp
                                               : snapshot.priceUsd;
  String fiat = snapshot.available && snapshot.pricesAvailable
                    ? fiatText(snapshot.balance * price)
                    : "--";
  display.drawString(String(currencySymbol) + fiat, 18, 156, 2);
  drawValue(18, 201, 128,
            mining ? String(stats.acceptedShares) + " / " +
                         String(stats.rejectedShares)
                   : "--",
            WHITE, PANEL);
  drawValue(173, 201, 128,
            mining ? String(stats.bestDifficulty, 4) : "--", WHITE, PANEL);
}

void HeliosDisplay::checkBalanceIncreases() {
  if (blockPopupActive_ || !balances_) return;
  for (size_t i = 0; i < HELIOS_COIN_COUNT; ++i) {
    HeliosCoin coin = static_cast<HeliosCoin>(i);
    HeliosBalanceSnapshot snapshot = balances_->get(coin);
    if (snapshot.increaseSequence == 0 ||
        snapshot.increaseSequence == seenIncreaseSequence_[i]) {
      continue;
    }
    seenIncreaseSequence_[i] = snapshot.increaseSequence;
    blockPopupCoin_ = coin;
    blockPopupAmount_ = snapshot.increaseAmount;
    blockPopupActive_ = true;
    drawBlockPopup();
    return;
  }
}

void HeliosDisplay::drawBlockPopup() {
  const HeliosCoinProfile& profile = heliosCoinProfile(blockPopupCoin_);
  setRearLed(profile.color);
  constexpr int16_t width = 240;
  constexpr int16_t height = 90;
  constexpr int16_t x = (320 - width) / 2;
  constexpr int16_t y = (240 - height) / 2;
  display.fillRoundRect(x, y, width, height, 8, PANEL);
  display.drawRoundRect(x, y, width, height, 8, profile.color);
  display.drawRoundRect(x + 1, y + 1, width - 2, height - 2, 8,
                        HELIOS_GREEN);
  display.setTextDatum(lgfx::top_center);
  display.setTextColor(WHITE, PANEL);
  display.drawString("BLOCK FOUND", 160, y + 14, 4);
  String amount = String(blockPopupAmount_, 8);
  while (amount.endsWith("0")) amount.remove(amount.length() - 1);
  if (amount.endsWith(".")) amount.remove(amount.length() - 1);
  String reward = "+" + amount + " " + profile.symbol;
  display.setTextColor(profile.color, PANEL);
  display.drawString(reward, 160, y + 52, reward.length() > 19 ? 1 : 2);
  display.setTextDatum(lgfx::top_left);
}

void HeliosDisplay::clearBlockPopup() {
  blockPopupActive_ = false;
  draw(true);
}

void HeliosDisplay::setRearLed(uint16_t color) {
  uint8_t red = static_cast<uint8_t>(((color >> 11) & 0x1F) * 255 / 31);
  uint8_t green = static_cast<uint8_t>(((color >> 5) & 0x3F) * 255 / 63);
  uint8_t blue = static_cast<uint8_t>((color & 0x1F) * 255 / 31);
  analogWrite(LED_RED_PIN, 255 - red);
  analogWrite(LED_GREEN_PIN, 255 - green);
  analogWrite(LED_BLUE_PIN, 255 - blue);
}

void HeliosDisplay::updateRearLed() {
  setRearLed(page_ == 0 ? HELIOS_AMBER
                        : heliosCoinProfileAt(page_ - 1).color);
}

void HeliosDisplay::drawFooter(uint16_t accent) {
  display.fillRect(118, 228, 84, 10, BLACK);
  for (uint8_t i = 0; i < 6; ++i) {
    int16_t x = 127 + i * 13;
    display.fillCircle(x, 233, i == page_ ? 3 : 2,
                       i == page_ ? accent : PANEL_LIGHT);
  }
}

String HeliosDisplay::balanceText(const HeliosCoinProfile& profile) const {
  auto snapshot = balances_->get(profile.id);
  if (!snapshot.available) return snapshot.status;
  if (snapshot.balance >= 1000000) return String(snapshot.balance / 1000000, 2) + "M";
  return String(snapshot.balance, snapshot.balance >= 1000 ? 2 : 8);
}

String HeliosDisplay::fiatText(double value) const {
  if (value >= 1000000.0) return String(value / 1000000.0, 2) + "M";
  if (value >= 1000.0) return String(value / 1000.0, 2) + "K";
  if (value >= 1.0) return String(value, 2);
  if (value >= 0.01) return String(value, 4);
  if (value > 0.0) return String(value, 6);
  return "0.00";
}

void HeliosDisplay::draw(bool force) {
  if (!settings_) return;
  if (!force && millis() - lastDrawAt_ < 900) return;
  if (force) updateRearLed();
  lastDrawAt_ = millis();
  HeliosMiningStats stats = heliosMinerGetStats();
  if (page_ == 0) {
    force ? drawHome(stats) : drawHomeValues(stats);
  } else {
    const HeliosCoinProfile& profile = heliosCoinProfileAt(page_ - 1);
    force ? drawCoin(profile, stats) : drawCoinValues(profile, stats);
  }
}

void HeliosDisplay::handleTouch() {
  int32_t x = 0;
  int32_t y = 0;
  bool pressed = display.getTouch(&x, &y);
  if (pressed) {
    if (!touching_) {
      touching_ = true;
      touchStartX_ = x;
      touchStartY_ = y;
    }
    touchLastX_ = x;
    touchLastY_ = y;
    return;
  }

  if (!touching_) return;
  touching_ = false;
  if (blockPopupActive_) {
    clearBlockPopup();
    return;
  }
  int32_t dx = touchLastX_ - touchStartX_;
  int32_t dy = touchLastY_ - touchStartY_;
  if (abs(dx) >= SWIPE_DISTANCE && abs(dx) > abs(dy)) {
    if (dx < 0 && page_ < 5) ++page_;
    if (dx > 0 && page_ > 0) --page_;
    draw(true);
    return;
  }

}
