#include <Arduino.h>
#include "UltrasonicSensor.h"
#include "Buzzer.h"
#include "LED.h"

// Sensor objects
UltrasonicSensor ultrasonicSensor(5, 4, 15.0); // HC-SR04: Trig=GPIO5(D1), Echo=GPIO4(D2), Alert=15cm
Buzzer buzzer(14); // TMB12A12 on GPIO14(D5)
LED led(13); // Single color LED on GPIO13(D7)

// Global alarm state
bool alarmActive = false;

// Timing variables
unsigned long lastDistanceRead = 0;
const unsigned long DISTANCE_READ_INTERVAL = 500;     // Distance checks every 500ms
const unsigned long SERIAL_UPDATE_INTERVAL = 1000;    // Serial output every 1 second
unsigned long lastSerialUpdate = 0;

void setup() {
    Serial.begin(115200);
    delay(2000);  // Wait for ESP8266 boot messages to finish
    
    Serial.println("\n\n\n\n\n\n\n\n\n\n");
    Serial.println("========================================");
    Serial.println("=== L Spectra Guardian Starting ===");
    Serial.println("ESP8266 NodeMCU Initialized");
    Serial.println("Minimal Mode - Ultrasonic Only");
    Serial.println("========================================");
    
    // Initialize components
    ultrasonicSensor.begin();
    buzzer.begin();
    led.begin();
    
    // Test components on startup
    buzzer.test();
    ultrasonicSensor.test();
    led.test();
    
    Serial.println("System Ready!");
    delay(2000);
}

void loop() {
    unsigned long currentTime = millis();
    
    // Update LED blinking (non-blocking)
    led.update();
    
    // Read distance at optimized intervals
    if (currentTime - lastDistanceRead > DISTANCE_READ_INTERVAL) {
        ultrasonicSensor.readDistance();
        lastDistanceRead = currentTime;
        
        // Check if object is too close and play alert sound
        if (ultrasonicSensor.isObjectTooClose()) {
            if (!alarmActive) {
                led.on();
                buzzer.playAlert();
                alarmActive = true;
            }
        } else {
            if (alarmActive) {
                led.off();
                alarmActive = false;
            }
        }
    }
    
    // Print sensor values to serial monitor at optimized intervals
    if (currentTime - lastSerialUpdate > SERIAL_UPDATE_INTERVAL) {
        Serial.println("========================================");
        Serial.println("=== SENSOR READINGS ===");
        Serial.print("Distance: ");
        Serial.print(ultrasonicSensor.getDistance());
        Serial.println(" cm");
        Serial.print("Alarm Status: ");
        Serial.println(alarmActive ? "ACTIVE" : "SAFE");
        Serial.println("========================================");
        
        lastSerialUpdate = currentTime;
    }
    
    // Small delay to prevent overwhelming the system
    yield();
}
