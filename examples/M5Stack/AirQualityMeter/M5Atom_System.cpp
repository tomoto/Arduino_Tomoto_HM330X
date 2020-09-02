#include "Platform.h"
#ifdef M5ATOM

#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>

#include "System.h"

const int BUTTON_PIN = 39;

void initSystem() {
  // We don't use M5Atom library because its LED implementation is really bad
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  pinMode(BUTTON_PIN, INPUT);
}

void shutdownSystem() {
  // Not supported
}

void restartSystem() {
  FastLED.clear(true);
  ESP.restart();
}

void haltSystem() {
  esp_deep_sleep_start();  // Leave the LED on and deep sleep
}

ButtonCode getPressedButton() {
  static int state = HIGH;
  if (state != digitalRead(BUTTON_PIN)) {
    state = !state;
    return state ? ButtonCode::NONE : ButtonCode::BTN1;
  } else {
    return ButtonCode::NONE;
  }
}

#endif