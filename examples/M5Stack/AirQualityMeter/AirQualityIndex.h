#pragma once

class AirQualityIndex {
 public:
  // calculate AQI from PM2.5 concentration (ug/m3)
  static float fromPM25(float value);
};
