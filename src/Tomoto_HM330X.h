/**
 * Library for Laser PM2.5 sensor HM3301/HM3302
 * https://wiki.seeedstudio.com/Grove-Laser_PM2.5_Sensor-HM3301/
 */

#pragma once

#include <Arduino.h>
#include <Wire.h>

#define TOMOTO_HM330X_DATA_PROVIDER_PRIVATES(T) \
 private:                                       \
  friend class Tomoto_HM330X;                   \
  const uint8_t* m_data;                        \
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

  // The data sheet says "Standard particulate" and "Atmospheric environment", but what they mean?
  //
  // https://wiki.seeedstudio.com/Grove-Laser_PM2.5_Sensor-HM3301/ says:
  //   The standard particulate matter mass concentration value refers to the mass concentration
  //   value obtained by density conversion of industrial metal particles as equivalent particles,
  //   and is suitable for use in industrial production workshops and the like.
  //   The concentration of particulate matter in the atmospheric environment is
  //   converted by the density of the main pollutants in the air as equivalent particles,
  //   and is suitable for ordinary indoor and outdoor atmospheric environments.

  // Concentration based on CF=1 standard particulate matter (ug/m3)
  class StandardParticulate {
    TOMOTO_HM330X_DATA_PROVIDER_PRIVATES(StandardParticulate)
   public:
    uint16_t getPM1() const { return decodeUint16(m_data, 2); }
    uint16_t getPM2_5() const { return decodeUint16(m_data, 3); };
    uint16_t getPM10() const { return decodeUint16(m_data, 4); };
  } std;

  // Concentration based on the pollutants in the air (ug/m3)
  class AtmosphericEnvironment {
    TOMOTO_HM330X_DATA_PROVIDER_PRIVATES(AtmosphericEnvironment)
   public:
    uint16_t getPM1() const { return decodeUint16(m_data, 5); }
    uint16_t getPM2_5() const { return decodeUint16(m_data, 6); };
    uint16_t getPM10() const { return decodeUint16(m_data, 7); };
  } atm;

  // Number of particles with the specified diameter or above in 0.1L of air
  class ParticleCount {
    TOMOTO_HM330X_DATA_PROVIDER_PRIVATES(ParticleCount)
   public:
    uint16_t get0_3() const { return decodeUint16(m_data, 8); }
    uint16_t get0_5() const { return decodeUint16(m_data, 9); }
    uint16_t get1() const { return decodeUint16(m_data, 10); }
    uint16_t get2_5() const { return decodeUint16(m_data, 11); }
    uint16_t get5() const { return decodeUint16(m_data, 12); }
    uint16_t get10() const { return decodeUint16(m_data, 13); }
  } count;

 private:
  bool sendCommand(uint8_t cmd);

  static uint16_t decodeUint16(const uint8_t* data, int i) {
    return data[i * 2] * 0x100 + data[i * 2 + 1];
  }
};

#undef TOMOTO_HM330X_DATA_PROVIDER_PRIVATES
