#include "Platform.h"
#ifdef M5STICKC

#include <M5StickC.h>

#include "System.h"

void initSystem() {
  M5.begin();
  Wire.begin(SDA_PIN, SCL_PIN);
}

void shutdownSystem() { M5.Axp.PowerOff(); }

void restartSystem() {
  M5.Axp.DeepSleep(1);  // Go to deep sleep and wake up immediately
}

void haltSystem() {
  // Wait for 30 seconds then shut down for power save
  for (int i = 0; i < 300; i++) {
    if (getPressedButton()) {
      restartSystem();
    }
    delay(100);
  }

  shutdownSystem();
}

ButtonCode getPressedButton() {
  M5.update();
  if (M5.BtnA.wasPressed()) {
    return ButtonCode::BTN1;
  }
  if (M5.BtnB.wasPressed()) {
    return ButtonCode::BTN2;
  }
  if (M5.Axp.GetBtnPress()) {
    return ButtonCode::RESET;
  }
  return ButtonCode::NONE;
}

#endif