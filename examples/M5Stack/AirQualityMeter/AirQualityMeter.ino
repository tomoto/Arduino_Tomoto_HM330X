// Note
// - Please choose your target platform (M5STICKC or M5ATOM) in Platform.h.
// - Please choose your I2C pins in Platform.h if you don't use Grove connector

#include <Tomoto_HM330X.h>
#include <Wire.h>

#include "AirQualityIndex.h"
#include "Battery.h"
#include "Display.h"
#include "Platform.h"
#include "System.h"
#include "Utility.h"

Tomoto_HM330X sensor;

int displayBrightness = 0;

uint64_t failureStartTime = 0;
const uint64_t RESTART_TIMEOUT = 20000;

void setup() {
  initSystem();
  initDisplay();
  delay(100);

  if (!sensor.begin()) {
    Serial.println("Failed to initialize the sensor.");
    displayInitializationError();
    haltSystem();
  }

  Serial.println("Sensor initialized.");
  Serial.println();
}

void loop() {
  State sensorState;
  if (!sensor.readSensor()) {
    Serial.println("Failed to read the sensor.");
    sensorState = State::BAD;

    if (failureStartTime) {
      // Restart the system if the sensor has not responded for a while
      if (millis() - failureStartTime > RESTART_TIMEOUT) {
        Serial.println("Sensor is not responding. Restarting.");
        restartSystem();
      }
    } else {
      failureStartTime = millis();  // Failure started
    }
  } else {
    sensorState = millis() < 30 * 1000
                      ? State::WARN  // Sensor needs 30 seconds to become stable
                      : State::GOOD;

    failureStartTime = 0;  // Clear failure
  }

  int batteryPercentage = getBatteryPercent();
  State batteryState =
      isInputPowerConnected()
          ? State::COOL
          : batteryPercentage >= 60
                ? State::GOOD
                : batteryPercentage >= 30 ? State::WARN : State::BAD;

  const int pm2_5 = sensor.std.getPM2_5();
  const int pm10 = sensor.std.getPM10();
  const float aqi = AirQualityIndex::fromPM25(pm2_5);

  if (!failureStartTime) {
    Serial.print("Senror response: PM2.5=");
    Serial.print(pm2_5);
    Serial.print(", PM10=");
    Serial.print(pm10);
    Serial.print(", AQI=");
    Serial.println(aqi);
  }

  displayStatus(aqi, pm2_5, pm10, sensorState, batteryPercentage, batteryState);

  for (int i = 0; i < 30; i++) {
    animateDisplay();
    switch (getPressedButton()) {
      case ButtonCode::BTN1:
        displayBrightness = 1 - displayBrightness;
        setDisplayBrightness(displayBrightness);
        break;
      case ButtonCode::RESET:
        shutdownSystem();
        break;
      default:
        // nothing to do
        break;
    }
    delay(100);
  }
}
