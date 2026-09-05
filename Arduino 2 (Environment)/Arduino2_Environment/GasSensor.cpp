#include "GasSensor.h"

GasSensor::GasSensor(uint8_t sensorPin) {
  pin = sensorPin;
}

void GasSensor::begin() {
  pinMode(pin, INPUT);
}

int GasSensor::read() {
  return analogRead(pin);
}
