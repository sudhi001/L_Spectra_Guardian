#include <Arduino.h>
#include "Buzzer.h"

Buzzer::Buzzer(int pin) {
    buzzerPin = pin;
}

void Buzzer::begin() {
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);
    Serial.println("TMB12A12 buzzer initialized");
}

void Buzzer::playTone(int frequency, int duration) {
    // Simple tone generation for active buzzer
    // For TMB12A12 (active buzzer), just turn on/off
    digitalWrite(buzzerPin, HIGH);
    delay(duration);
    digitalWrite(buzzerPin, LOW);
}

void Buzzer::playAlert() {
    // Play alert sound pattern
    Serial.println("ALERT: Object too close!");
    
    // Play 3 short beeps
    for (int i = 0; i < 3; i++) {
        playTone(1000, 200); // 200ms beep
        delay(100); // 100ms pause between beeps
    }
}

void Buzzer::test() {
    Serial.println("Testing buzzer...");
    
    // Test 1: Simple on/off
    Serial.println("Test 1: Simple beep");
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
    delay(500);
    
    // Test 2: Multiple beeps
    Serial.println("Test 2: Multiple beeps");
    for (int i = 0; i < 5; i++) {
        digitalWrite(buzzerPin, HIGH);
        delay(100);
        digitalWrite(buzzerPin, LOW);
        delay(100);
    }
    
    Serial.println("Buzzer test complete");
}

void Buzzer::stop() {
    digitalWrite(buzzerPin, LOW);
}
