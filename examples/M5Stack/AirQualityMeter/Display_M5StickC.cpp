#include "Platform.h"
#ifdef M5STICKC

#include <M5StickC.h>

#include "Display.h"

static const uint16_t signalColors[] = {
    TFT_GREEN,   // Good
    TFT_YELLOW,  // Moderate
    TFT_ORANGE,  // Unhealthy for sensitive groups
    TFT_RED,     // Unhealthy
    TFT_PURPLE   // Hazardous
};

static const uint16_t statusColors[] = {
    TFT_CYAN,    // Good
    TFT_YELLOW,  // Warn
    TFT_RED,     // Bad
    TFT_BLUE     // Cool
};

void initDisplay() {
  static M5Display& d = M5.Lcd;

  d.fillScreen(TFT_BLACK);
  d.setRotation(1);
  d.setTextSize(1);

  setDisplayBrightness(0);
}

void setDisplayBrightness(int brightness) {
  static uint8_t screenBreathes[] = {9, 12};
  M5.Axp.ScreenBreath(screenBreathes[brightness]);
}

// Basic coordinates
static const int x[] = {10, 72, 52, 120};
static const int y[] = {42, 62, 78};

static void updateAndDisplaySensorState(TFT_eSPI& d, bool update,
                                        State sensorState = State::GOOD) {
  static State currentSensorState;
  if (update) {
    currentSensorState = sensorState;
  }

  d.setTextFont(1);
  bool on = currentSensorState == State::GOOD
                ? (millis() % 2000) < 1600  // Fast blink
                : (millis() % 500) < 300;   // Slow blink
  d.setTextColor(on ? statusColors[currentSensorState] : TFT_BLACK, TFT_BLACK);
  d.drawString("SENS *", x[3], y[1]);
}

void displayStatus(float aqi, int pm2_5, int pm10, State sensorState,
                   int batteryPercentage, State batteryState) {
  TFT_eSprite d(&M5.Lcd);
  d.createSprite(160, 80);

  d.fillScreen(TFT_BLACK);
  d.setTextDatum(L_BASELINE);

  // AQI
  uint16_t c = signalColors[min(int(aqi / 50), 4)];

  d.fillRect(0, 0, 160, 4, c);  // Top bar

  d.setTextColor(c);
  d.setFreeFont(&FreeSans18pt7b);
  d.drawString("AQI", x[0], y[0]);
  d.setFreeFont(&FreeSansBold24pt7b);
  d.drawNumber(int(aqi), x[1] + (aqi < 100) * 8, y[0]);

  // PM um/m3
  d.setTextColor(TFT_LIGHTGREY);
  d.setTextFont(2);
  d.drawString("PM2.5", x[0], y[1]);
  d.drawString("PM10", x[0], y[2]);

  d.setTextColor(TFT_WHITE);
  d.setFreeFont(&FreeSans9pt7b);
  d.drawNumber(pm2_5, x[2], y[1]);
  d.drawNumber(pm10, x[2], y[2]);

  // Sensor status
  updateAndDisplaySensorState(d, true, sensorState);

  // Battery status
  d.setTextFont(1);
  d.setTextColor(statusColors[batteryState]);
  char buf[8];
  sprintf(buf, "%3d%% *", batteryPercentage);
  d.drawString(buf, x[3], y[2]);

  d.pushSprite(0, 0);
  d.deleteSprite();
}

void animateDisplay() {
  // Animate the sensor status to say "I'm breathing"
  updateAndDisplaySensorState(M5.Lcd, false);
}

void displayInitializationError() {
  const int MARGIN = 4;
  TFT_eSprite d(&M5.Lcd);
  d.createSprite(160 - MARGIN * 2, 80);

  d.setTextFont(2);
  d.setTextColor(TFT_RED);
  d.setTextWrap(true);
  d.println(
      "Failed to initialize the sensor. Please check the cable and press "
      "button to restart.");

  d.pushSprite(MARGIN, 0);
  d.deleteSprite();
}

#endif
