#pragma once

#include "Utility.h"

void initDisplay();
void setDisplayBrightness(int brightness);
void displayStatus(float aqi, int pm2_5, int pm10, State sensorState,
                   int batteryPercentage, State batteryState);
void animateDisplay();
void displayInitializationError();