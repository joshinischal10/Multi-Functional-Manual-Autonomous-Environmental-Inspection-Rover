// DHTSensor.cpp
#include "DHTSensor.h"

DHTSensor::DHTSensor(uint8_t pin, uint8_t type) : dht(pin, type), temperature(0.0), humidity(0.0) {}

void DHTSensor::begin() {
    dht.begin();
}

bool DHTSensor::read() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
        return false;
    }

    humidity = h;
    temperature = t;
    return true;
}

float DHTSensor::getTemperature() const {
    return temperature;
}

float DHTSensor::getHumidity() const {
    return humidity;
}