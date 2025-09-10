#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <DHT.h>

class TemperatureSensor {
private:
    DHT* dht;
    int dhtPin;
    
public:
    TemperatureSensor(int pin = 2);
    void begin();
    void readTemperature();
    float getTemperature();
    float getHumidity();
    bool isSensorWorking();
};

#endif
