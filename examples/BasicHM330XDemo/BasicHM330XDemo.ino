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

    Serial.println("Concentration based on CF=1 standard particlate matter (ug/m^3) --");
    printValue("PM1.0", sensor.std.getPM1());
    printValue("PM2.5", sensor.std.getPM2_5());
    printValue("PM10", sensor.std.getPM10());

    Serial.println("Concentration based on atmospheric environment (ug/m^3) --");
    printValue("PM1.0", sensor.atm.getPM1());
    printValue("PM2.5", sensor.atm.getPM2_5());
    printValue("PM10", sensor.atm.getPM10());

    // Maybe supported or not, depending on the sensor model
    Serial.println("Number of particles with diameter of (/0.1L) --");
    printValue(">=0.3um", sensor.count.get0_3());
    printValue(">=0.5um", sensor.count.get0_5());
    printValue(">=1.0um", sensor.count.get1());
    printValue(">=2.5um", sensor.count.get2_5());
    printValue(">=5.0um", sensor.count.get5());
    printValue(">=10um", sensor.count.get10());

    Serial.println();
  }

  delay(5000);
}
