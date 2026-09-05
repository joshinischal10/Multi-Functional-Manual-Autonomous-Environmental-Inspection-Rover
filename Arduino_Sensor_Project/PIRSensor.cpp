// PIRSensor.cpp
#include "PIRSensor.h"

PIRSensor::PIRSensor(uint8_t pin) : pin(pin) {}

void PIRSensor::begin() {
    pinMode(pin, INPUT);
}

bool PIRSensor::isMotionDetected() {
    return digitalRead(pin) == HIGH;
}