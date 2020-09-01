#include <Tomoto_HM330X.h>

Tomoto_HM330X sensor;
// Tomoto_HM330X sensor(Wire1); // to use the alternative wire

void printValue(const char* label, int value) {
  Serial.print(label);
  Serial.print(": ");
  Serial.println(value);
}

void setup() {
  Serial.begin(115200);
  delay(100);

  // Wire.begin(25, 21); // to specify the I2C pins

  if (!sensor.begin()) {
    Serial.println("Failed to initialize HM330X");
    while (1)
      ;
  }

  Serial.println("HM330X initialized");
  Serial.println();
}

void loop() {
  if (!sensor.readSensor()) {
    Serial.println("Failed to read HM330X");
  } else {
    printValue("Sensor number", sensor.getSensorNumber());

    Serial.println("Concentration in CF=1 standard atmosphere (ug/m^3) --");
    printValue("PM1.0", sensor.std.getPM1());
    printValue("PM2.5", sensor.std.getPM2_5());
    printValue("PM10", sensor.std.getPM10());

    Serial.println("Concentration in ambient atmosphere (ug/m^3) --");
    printValue("PM1.0", sensor.amb.getPM1());
    printValue("PM2.5", sensor.amb.getPM2_5());
    printValue("PM10", sensor.amb.getPM10());

    Serial.println("Number of particles with diameter of (/0.1L) --");
    printValue(">=0.3um", sensor.raw.get0_3());
    printValue(">=0.5um", sensor.raw.get0_5());
    printValue(">=1.0um", sensor.raw.get1());
    printValue(">=2.5um", sensor.raw.get2_5());
    printValue(">=5.0um", sensor.raw.get5());
    printValue(">=10um", sensor.raw.get10());

    Serial.println();
  }

  delay(5000);
}
