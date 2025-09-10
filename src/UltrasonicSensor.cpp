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
        Serial.println("Check connections:");
        Serial.println("- VCC to 5V");
        Serial.println("- GND to GND");
        Serial.println("- Trig to GPIO5 (D1)");
        Serial.println("- Echo to GPIO4 (D2)");
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
    // Try multiple times for better reliability
    for (int attempt = 0; attempt < 3; attempt++) {
        // Clear the trigPin
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        
        // Set the trigPin HIGH for 10 microseconds
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        
        // Read the echoPin, returns the sound wave travel time in microseconds
        // Add timeout to prevent hanging
        long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
        
        // Check if we got a valid reading
        if (duration == 0) {
            delay(10); // Small delay before retry
            continue; // Try again
        }
        
        // Calculate the distance
        float distance = duration * 0.034 / 2;
        
        // Check if distance is valid (between 2cm and 400cm)
        if (distance >= 2 && distance <= 400) {
            return distance; // Valid reading found
        }
        
        delay(10); // Small delay before retry
    }
    
    return -1; // All attempts failed
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

void UltrasonicSensor::test() {
    Serial.println("Testing HC-SR04 ultrasonic sensor...");
    
    // Test 1: Check pin states
    Serial.println("Test 1: Checking pin states");
    Serial.print("Trig pin (GPIO5): ");
    Serial.println(trigPin);
    Serial.print("Echo pin (GPIO4): ");
    Serial.println(echoPin);
    
    // Test 2: Try multiple readings
    Serial.println("Test 2: Taking 5 readings");
    int successCount = 0;
    for (int i = 0; i < 5; i++) {
        float distance = getDistance();
        if (distance > 0) {
            Serial.print("Reading ");
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(distance);
            Serial.println(" cm - SUCCESS");
            successCount++;
        } else {
            Serial.print("Reading ");
            Serial.print(i + 1);
            Serial.println(": FAILED");
        }
        delay(100);
    }
    
    Serial.print("Success rate: ");
    Serial.print(successCount);
    Serial.println("/5");
    
    if (successCount == 0) {
        Serial.println("HC-SR04 test FAILED - Check connections!");
    } else {
        Serial.println("HC-SR04 test PASSED");
    }
}
