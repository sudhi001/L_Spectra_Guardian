#include <Arduino.h>
#include "ServoMotor.h"

ServoMotor::ServoMotor(int pin, int min, int max) {
    servoPin = pin;
    minAngle = min;
    maxAngle = max;
    currentAngle = 90; // Start at center
    isRotating = false;
    rotationDirection = 1; // 1 for forward, -1 for backward
    rotationSpeed = 1; // Degrees per update
}

void ServoMotor::begin() {
    servo.attach(servoPin);
    servo.write(currentAngle);
    Serial.println("SG90 servo motor initialized");
    delay(1000); // Give servo time to reach position
}

void ServoMotor::moveTo(int angle) {
    // Constrain angle within limits
    angle = constrain(angle, minAngle, maxAngle);
    
    Serial.print("Moving servo to: ");
    Serial.print(angle);
    Serial.println(" degrees");
    
    servo.write(angle);
    currentAngle = angle;
    delay(500); // Give servo time to move
}

void ServoMotor::sweep() {
    Serial.println("Servo sweep test...");
    
    // Sweep from min to max
    for (int angle = minAngle; angle <= maxAngle; angle += 5) {
        servo.write(angle);
        currentAngle = angle;
        delay(50);
    }
    
    // Sweep back from max to min
    for (int angle = maxAngle; angle >= minAngle; angle -= 5) {
        servo.write(angle);
        currentAngle = angle;
        delay(50);
    }
    
    // Return to center
    center();
}

void ServoMotor::center() {
    int centerAngle = (minAngle + maxAngle) / 2;
    moveTo(centerAngle);
    Serial.println("Servo centered");
}

void ServoMotor::test() {
    Serial.println("Testing SG90 servo...");
    
    // Test 1: Move to center
    Serial.println("Test 1: Moving to center");
    center();
    delay(1000);
    
    // Test 2: Move to extremes
    Serial.println("Test 2: Moving to min position");
    moveTo(minAngle);
    delay(1000);
    
    Serial.println("Test 3: Moving to max position");
    moveTo(maxAngle);
    delay(1000);
    
    // Test 3: Return to center
    Serial.println("Test 4: Returning to center");
    center();
    
    Serial.println("Servo test complete");
}

int ServoMotor::getCurrentAngle() {
    return currentAngle;
}

void ServoMotor::setLimits(int min, int max) {
    minAngle = min;
    maxAngle = max;
    Serial.print("Servo limits set: ");
    Serial.print(minAngle);
    Serial.print(" to ");
    Serial.println(maxAngle);
}

void ServoMotor::startContinuousRotation() {
    isRotating = true;
    Serial.println("Starting continuous rotation");
}

void ServoMotor::stopContinuousRotation() {
    isRotating = false;
    servo.write(90); // Stop at center
    currentAngle = 90;
    Serial.println("Stopping continuous rotation");
}

void ServoMotor::updateRotation() {
    if (!isRotating) return;
    
    // Update angle
    currentAngle += rotationDirection * rotationSpeed;
    
    // Reverse direction at limits
    if (currentAngle >= maxAngle) {
        currentAngle = maxAngle;
        rotationDirection = -1;
    } else if (currentAngle <= minAngle) {
        currentAngle = minAngle;
        rotationDirection = 1;
    }
    
    // Move servo
    servo.write(currentAngle);
}

void ServoMotor::setRotationSpeed(int speed) {
    rotationSpeed = constrain(speed, 1, 10);
    Serial.print("Rotation speed set to: ");
    Serial.println(rotationSpeed);
}
