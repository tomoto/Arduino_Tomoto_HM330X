#include "Tomoto_HM330X.h"

#ifdef SPARK_PLATFORM
typedef char WireByte;
#else
typedef uint8_t WireByte;
#endif

Tomoto_HM330X::Tomoto_HM330X(uint8_t addr) : Tomoto_HM330X(Wire, addr) {}

Tomoto_HM330X::Tomoto_HM330X(TwoWire& wire, uint8_t addr)
    : m_wire(Wire), m_addr(addr), std(m_data), atm(m_data), count(m_data) {}

bool Tomoto_HM330X::begin(bool retry) {
  m_wire.begin();

  int maxRetryCount = retry ? 10 : 0;
  int retryCount = 0;

  // Send select command
  while (!sendCommand(0x88)) {
    if (retryCount++ >= maxRetryCount) {
      return false;
    } else {
      delay(100);
    }
  }
  return true;
}

bool Tomoto_HM330X::sendCommand(uint8_t cmd) {
  m_wire.beginTransmission(m_addr);
  m_wire.write(cmd);
  uint8_t i2cError = m_wire.endTransmission();
  return !i2cError;
}

bool Tomoto_HM330X::readSensor() {
  // Read data
  uint8_t buf[DATA_SIZE];
  m_wire.requestFrom(m_addr, DATA_SIZE);
  if (m_wire.readBytes((WireByte*)(&buf), DATA_SIZE) < DATA_SIZE) {
    return false;
  }

  // Checksum
  uint8_t sum = 0;
  for (int i = 0; i < DATA_SIZE - 1; i++) {
    sum += buf[i];
  }
  if (sum != buf[DATA_SIZE - 1]) {
    return false;
  }

  // Everything is fine, commit the data
  memcpy(m_data, buf, DATA_SIZE);
  return true;
}
