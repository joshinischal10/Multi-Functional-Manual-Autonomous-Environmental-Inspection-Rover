// PIRSensor.h
#ifndef PIR_SENSOR_H
#define PIR_SENSOR_H

#include <Arduino.h>

class PIRSensor {
private:
    uint8_t pin;

public:
    PIRSensor(uint8_t pin);
    void begin();
    bool isMotionDetected();
};

#endif