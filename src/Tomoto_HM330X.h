/**
 * Library for Laser PM2.5 sensor HM3301/HM3302
 * https://wiki.seeedstudio.com/Grove-Laser_PM2.5_Sensor-HM3301/
 */

#pragma once

#include <Arduino.h>
#include <Wire.h>

#define DATA_PROVIDER_PRIVATES(T) \
 private:                         \
  friend class Tomoto_HM330X;     \
  const uint8_t* m_data;          \
  T(const uint8_t* data) : m_data(data) {}

class Tomoto_HM330X {
 private:
  TwoWire& m_wire;
  uint8_t m_addr;

  static const uint8_t DATA_SIZE = 29;
  uint8_t m_data[DATA_SIZE];

 public:
  static const uint8_t DEFAULT_I2C_ADDR = 0x40;
  Tomoto_HM330X(uint8_t addr = DEFAULT_I2C_ADDR);
  Tomoto_HM330X(TwoWire& wire, uint8_t addr = DEFAULT_I2C_ADDR);

  bool begin(bool retry = true);
  bool readSensor();

  uint16_t getSensorNumber() const { return decodeUint16(m_data, 1); }

  // The data sheet says "Standard particulate" and "Atmospheric environment",
  // but what do they mean?
  // https://files.seeedstudio.com/wiki/Grove-Laser_PM2.5_Sensor-HM3301/res/HM-3300%263600_V2.1.pdf
  //
  // I use my own interpretation based on the discussion below:
  // https://publiclab.org/questions/samr/04-07-2019/how-to-interpret-pms5003-sensor-values

  // Concentration in the CF=1 standard atmosphere (ug/m3)
  class StandardAtmosphere {
    DATA_PROVIDER_PRIVATES(StandardAtmosphere)
   public:
    uint16_t getPM1() const { return decodeUint16(m_data, 2); }
    uint16_t getPM2_5() const { return decodeUint16(m_data, 3); };
    uint16_t getPM10() const { return decodeUint16(m_data, 4); };
  } std;

  // Concentration in the ambient atmosphere as it is (ug/m3)
  class AmbientAtmosphere {
    DATA_PROVIDER_PRIVATES(AmbientAtmosphere)
   public:
    uint16_t getPM1() const { return decodeUint16(m_data, 5); }
    uint16_t getPM2_5() const { return decodeUint16(m_data, 6); };
    uint16_t getPM10() const { return decodeUint16(m_data, 7); };
  } amb;

  // Number of particles with the specified diameter or above in 0.1L of air
  class RawNumberOfParticles {
    DATA_PROVIDER_PRIVATES(RawNumberOfParticles)
   public:
    uint16_t get0_3() const { return decodeUint16(m_data, 8); }
    uint16_t get0_5() const { return decodeUint16(m_data, 9); }
    uint16_t get1() const { return decodeUint16(m_data, 10); }
    uint16_t get2_5() const { return decodeUint16(m_data, 11); }
    uint16_t get5() const { return decodeUint16(m_data, 12); }
    uint16_t get10() const { return decodeUint16(m_data, 13); }
  } raw;

 private:
  bool sendCommand(uint8_t cmd);

  static uint16_t decodeUint16(const uint8_t* data, int i) {
    return data[i * 2] * 0x100 + data[i * 2 + 1];
  }
};

#undef DATA_PROVIDER_PRIVATES
