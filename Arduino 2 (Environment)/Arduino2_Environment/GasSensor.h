#ifndef GAS_SENSOR_H
#define GAS_SENSOR_H

#include <Arduino.h>

class GasSensor {
private:
  uint8_t pin;

public:
  GasSensor(uint8_t sensorPin);
  void begin();
  int read();
};

#endif
