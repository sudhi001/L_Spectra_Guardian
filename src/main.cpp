#include <Arduino.h>
#include "TemperatureSensor.h"
#include "UltrasonicSensor.h"
#include "Buzzer.h"
#include "ServoMotor.h"
#include "LED.h"

TemperatureSensor tempSensor(2); // DHT11 on GPIO2
UltrasonicSensor ultrasonicSensor(5, 4, 15.0); // HC-SR04: Trig=GPIO5(D1), Echo=GPIO4(D2), Alert=15cm
Buzzer buzzer(14); // TMB12A12 on GPIO14(D5)
ServoMotor servo(12); // SG90 on GPIO12(D6)
LED led(13); // Single color LED on GPIO13(D7)

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    tempSensor.begin();
    ultrasonicSensor.begin();
    buzzer.begin();
    servo.begin();
    led.begin();
    
    // Test components on startup
    buzzer.test();
    ultrasonicSensor.test();
    servo.test();
    led.test();
    
    // Start continuous rotation
    servo.setRotationSpeed(2); // Medium speed
    servo.startContinuousRotation();
}

void loop() {
    tempSensor.readTemperature(); // Read and display temperature
    
    // Update continuous servo rotation
    servo.updateRotation();
    
    // Update LED blinking
    led.update();
    
    // Read distance less frequently for better reliability
    static unsigned long lastDistanceRead = 0;
    static bool alarmActive = false;
    
    if (millis() - lastDistanceRead > 500) { // Check every 500ms
        ultrasonicSensor.readDistance();
        lastDistanceRead = millis();
        
        // Check if object is too close and play alert sound
        if (ultrasonicSensor.isObjectTooClose()) {
            if (!alarmActive) {
                led.on(); // Turn on LED first
                buzzer.playAlert(); // Then play buzzer
                alarmActive = true;
            }
        } else {
            if (alarmActive) {
                led.off(); // Turn off LED when alarm clears
                alarmActive = false;
            }
        }
    }
    
    delay(100); // Standard update interval
}