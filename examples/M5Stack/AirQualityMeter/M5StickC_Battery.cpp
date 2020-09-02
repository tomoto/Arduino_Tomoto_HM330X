#include "Platform.h"
#ifdef M5STICKC

#include <M5StickC.h>

#include "Battery.h"

// https://github.com/m5stack/M5-Schematic/blob/master/Core/AXP192%20Datasheet_v1.1_en_draft_2211.pdf

bool isInputPowerConnected() { return !!(M5.Axp.GetInputPowerStatus() & 0x20); }

int getBatteryPercent() {
  return int((M5.Axp.GetBatVoltage() - 3.0) / 1.1 * 100);
}

#endif