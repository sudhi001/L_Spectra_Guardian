#include <Arduino.h>
#include "LED.h"

LED::LED(int pin) {
    ledPin = pin;
    isBlinking = false;
    lastBlinkTime = 0;
    ledState = false;
    blinkInterval = 200;
}

void LED::begin() {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    Serial.println("Single color LED initialized");
}

void LED::on() {
    digitalWrite(ledPin, HIGH);
    ledState = true;
    isBlinking = false;
}

void LED::off() {
    digitalWrite(ledPin, LOW);
    ledState = false;
    isBlinking = false;
}

void LED::blink(int interval) {
    isBlinking = true;
    blinkInterval = interval;
    lastBlinkTime = millis();
}

void LED::stopBlinking() {
    isBlinking = false;
    digitalWrite(ledPin, LOW);
    ledState = false;
}

void LED::update() {
    if (isBlinking) {
        if (millis() - lastBlinkTime >= (unsigned long)blinkInterval) {
            ledState = !ledState;
            digitalWrite(ledPin, ledState ? HIGH : LOW);
            lastBlinkTime = millis();
        }
    }
}

void LED::test() {
    Serial.println("Testing single color LED...");
    
    // Test 1: Turn on
    Serial.println("LED ON");
    on();
    delay(1000);
    
    // Test 2: Turn off
    Serial.println("LED OFF");
    off();
    delay(1000);
    
    // Test 3: Blink test
    Serial.println("LED BLINKING");
    blink(500); // Blink every 500ms
    delay(3000); // Blink for 3 seconds
    stopBlinking();
    
    Serial.println("LED test complete");
}
