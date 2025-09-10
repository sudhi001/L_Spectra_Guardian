#include <Arduino.h>
#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(int trig, int echo, float alertDist) {
    trigPin = trig;
    echoPin = echo;
    alertDistance = alertDist;
}

void UltrasonicSensor::begin() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.println("HC-SR04 ultrasonic sensor initialized");
}

void UltrasonicSensor::readDistance() {
    float distance = getDistance();
    
    if (distance < 0) {
        Serial.println("Failed to read from HC-SR04 sensor!");
        return;
    }
    
    Serial.println("=== Distance Reading ===");
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    if (isObjectTooClose()) {
        Serial.print("WARNING: Object closer than ");
        Serial.print(alertDistance);
        Serial.println(" cm!");
    }
    
    Serial.println("=======================");
}

float UltrasonicSensor::getDistance() {
    // Clear the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Set the trigPin HIGH for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Read the echoPin, returns the sound wave travel time in microseconds
    long duration = pulseIn(echoPin, HIGH);
    
    // Calculate the distance
    float distance = duration * 0.034 / 2;
    
    // Check if distance is valid (between 2cm and 400cm)
    if (distance < 2 || distance > 400) {
        return -1; // Invalid reading
    }
    
    return distance;
}

bool UltrasonicSensor::isSensorWorking() {
    float distance = getDistance();
    return (distance >= 2 && distance <= 400);
}

bool UltrasonicSensor::isObjectTooClose() {
    float distance = getDistance();
    return (distance > 0 && distance < alertDistance);
}

void UltrasonicSensor::setAlertDistance(float distance) {
    alertDistance = distance;
}
