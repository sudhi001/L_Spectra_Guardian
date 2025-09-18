#ifndef MQSENSOR_H
#define MQSENSOR_H

#include <Arduino.h>

class MQSensor {
private:
    int analogPin;
    float airQuality;
    unsigned long lastRead;
    float r0;  // Baseline resistance in clean air
    float rs;  // Current sensor resistance
    float voltage;
    int rawValue;
    bool calibrationComplete;
    float coPPM;    // CO concentration in PPM
    float co2PPM;   // CO2 concentration in PPM
    static const unsigned long READ_INTERVAL = 1000; // Read every 1 second
    static const unsigned long CALIBRATION_TIME = 30000; // 30 seconds calibration
    
    // Gas detection functions
    float calculateCO(float ratio);
    float calculateCO2(float ratio);
    
public:
    MQSensor(int pin);
    void begin();
    void readAirQuality();
    void calibrate();
    float getAirQuality();
    float getRawValue();
    float getVoltage();
    float getResistance();
    float getResistanceRatio();
    float getCOPPM();
    float getCO2PPM();
    String getAirQualityStatus();
    String getCOStatus();
    String getCO2Status();
    bool isCalibrated();
    void test();
};

#endif
