#include "Platform.h"
#ifdef M5ATOM

#include "Battery.h"

bool isInputPowerConnected() { return false; }

int getBatteryPercent() { return 0; }

#endif