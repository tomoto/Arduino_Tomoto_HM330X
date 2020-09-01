#include "AirQualityIndex.h"

// https://en.wikipedia.org/wiki/NowCast_(air_quality_index)
struct Breakpoint {
  float c;  // PM2.5 concentration
  float i;  // AQI
} breakpoints[] = {
    {0.0f, 0},      // bottom line
    {12.0f, 50},    // good
    {35.4f, 100},   // moderate
    {55.4f, 150},   // unhealthy for sensitive groups
    {150.4f, 200},  // unhealthy
    {250.4f, 300},  // very unhealthy
    {320.4f, 400},  // hazardous
    {500.4f, 500}   // hazardous
};

float AirQualityIndex::fromPM25(float value) {
  // capping
  if (value >= 500) {
    value = 500;
  }

  for (int i = 1; i <= 7; i++) {
    float ch = breakpoints[i].c;
    if (value < ch) {
      float cl = breakpoints[i - 1].c;
      float ih = breakpoints[i].i;
      float il = breakpoints[i - 1].i;
      return (value - cl) / (ch - cl) * (ih - il) + il;
    }
  }

  return 500;
}
