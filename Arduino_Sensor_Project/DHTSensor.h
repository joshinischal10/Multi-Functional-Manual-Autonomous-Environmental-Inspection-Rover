// DHTSensor.h
#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include <DHT.h>

class DHTSensor {
private:
    DHT dht;
    float temperature;
    float humidity;

public:
    DHTSensor(uint8_t pin, uint8_t type);
    void begin();
    bool read();
    float getTemperature() const;
    float getHumidity() const;
};

#endif