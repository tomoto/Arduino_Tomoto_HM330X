#include "Platform.h"
#ifdef M5STICKC

#include <M5StickC.h>

#include "Display.h"

static M5Display& lcd = M5.Lcd;  // Short alias

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
  lcd.fillScreen(TFT_BLACK);
  lcd.setRotation(1);
  lcd.setTextSize(1);

  setDisplayBrightness(0);
}

void setDisplayBrightness(int brightness) {
  static uint8_t screenBreathes[] = {9, 12};
  M5.Axp.ScreenBreath(screenBreathes[brightness]);
}

// Basic coordinates
static const int x[] = {10, 72, 52, 124};
static const int y[] = {42, 62, 78};

static void updateAndDisplaySensorState(bool update,
                                        State sensorState = State::GOOD) {
  static State currentSensorState;
  if (update) {
    currentSensorState = sensorState;
  }

  lcd.setTextFont(1);
  bool on = currentSensorState == State::GOOD
                ? (millis() % 2000) < 1600  // Fast blink
                : (millis() % 500) < 300;   // Slow blink
  lcd.setTextColor(on ? statusColors[currentSensorState] : TFT_BLACK,
                   TFT_BLACK);
  lcd.drawString("SENS *", x[3], y[1]);
}

void displayStatus(float aqi, int pm2_5, int pm10, State sensorState,
                   int batteryPercentage, State batteryState) {
  lcd.fillScreen(TFT_BLACK);
  lcd.setTextDatum(L_BASELINE);

  // AQI
  uint16_t c = signalColors[min(int(aqi / 50), 4)];

  lcd.fillRect(0, 0, 160, 4, c);  // Top bar

  lcd.setTextColor(c);
  lcd.setFreeFont(&FreeSans18pt7b);
  lcd.drawString("AQI", x[0], y[0]);
  lcd.setFreeFont(&FreeSansBold24pt7b);
  lcd.drawNumber(int(aqi), x[1] + (aqi < 100) * 8, y[0]);

  // PM um/m3
  lcd.setTextColor(TFT_LIGHTGREY);
  lcd.setTextFont(2);
  lcd.drawString("PM2.5", x[0], y[1]);
  lcd.drawString("PM10", x[0], y[2]);

  lcd.setTextColor(TFT_WHITE);
  lcd.setFreeFont(&FreeSans9pt7b);
  lcd.drawNumber(pm2_5, x[2], y[1]);
  lcd.drawNumber(pm10, x[2], y[2]);

  // Sensor status
  updateAndDisplaySensorState(true, sensorState);

  // Battery status
  lcd.setTextFont(1);
  lcd.setTextColor(statusColors[batteryState]);
  char buf[8];
  sprintf(buf, "%3d%% *", batteryPercentage);
  lcd.drawString(buf, x[3], y[2]);
}

void animateDisplay() {
  // Animate the sensor status to say "I'm breathing"
  updateAndDisplaySensorState(false);
}

void displayInitializationError() {
  lcd.setTextFont(2);
  lcd.setTextColor(TFT_RED);
  lcd.setTextWrap(true);
  lcd.println(
      "Failed to initialize the sensor. Please check the cable and press "
      "button to restart.");
}

#endif
