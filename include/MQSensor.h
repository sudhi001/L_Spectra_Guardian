#ifndef MQSENSOR_H
#define MQSENSOR_H

#include <Arduino.h>

class MQSensor {
private:
    int analogPin;
    float airQuality;
    unsigned long lastRead;
    static const unsigned long READ_INTERVAL = 1000; // Read every 1 second
    
public:
    MQSensor(int pin);
    void begin();
    void readAirQuality();
    float getAirQuality();
    String getAirQualityStatus();
    void test();
};

#endif
