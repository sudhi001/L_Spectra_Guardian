#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

class UltrasonicSensor {
private:
    int trigPin;
    int echoPin;
    float alertDistance;
    
public:
    UltrasonicSensor(int trig, int echo, float alertDist = 15.0);
    void begin();
    void readDistance();
    float getDistance();
    bool isSensorWorking();
    bool isObjectTooClose();
    void setAlertDistance(float distance);
};

#endif
