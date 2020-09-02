#pragma once

#ifndef M5STICKC
#error These functions are only for M5StickC
#endif

bool isInputPowerConnected();
int getBatteryPercent();