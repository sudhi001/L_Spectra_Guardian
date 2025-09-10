#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include <Servo.h>

class ServoMotor {
private:
    Servo servo;
    int servoPin;
    int currentAngle;
    int minAngle;
    int maxAngle;
    bool isRotating;
    int rotationDirection;
    int rotationSpeed;
    
public:
    ServoMotor(int pin, int min = 0, int max = 180);
    void begin();
    void moveTo(int angle);
    void sweep();
    void center();
    void test();
    void startContinuousRotation();
    void stopContinuousRotation();
    void updateRotation();
    int getCurrentAngle();
    void setLimits(int min, int max);
    void setRotationSpeed(int speed);
};

#endif
