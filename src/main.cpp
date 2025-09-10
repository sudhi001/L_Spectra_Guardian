#include <Arduino.h>
#include "WiFiScanner.h"
#include "TemperatureSensor.h"
#include "UltrasonicSensor.h"
#include "Buzzer.h"
#include "ServoMotor.h"

WiFiScanner wifiScanner("ACHU-APPU-2G", "R#I#T#W#I#K");
TemperatureSensor tempSensor(2); // DHT11 on GPIO2
UltrasonicSensor ultrasonicSensor(5, 4, 15.0); // HC-SR04: Trig=GPIO5(D1), Echo=GPIO4(D2), Alert=15cm
Buzzer buzzer(14); // TMB12A12 on GPIO14(D5)
ServoMotor servo(12); // SG90 on GPIO12(D6)

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    // wifiScanner.begin();
    tempSensor.begin();
    ultrasonicSensor.begin();
    buzzer.begin();
    servo.begin();
    
    // Test components on startup
    buzzer.test();
    ultrasonicSensor.test();
    servo.test();
    
    // Start continuous rotation
    servo.setRotationSpeed(2); // Medium speed
    servo.startContinuousRotation();
    
    // wifiScanner.scan(); // Initial WiFi scan
}

void loop() {
    // wifiScanner.checkConnectionStatus(); // Check WiFi connection
    tempSensor.readTemperature(); // Read and display temperature
    
    // Update continuous servo rotation
    servo.updateRotation();
    
    // Read distance less frequently for better reliability
    static unsigned long lastDistanceRead = 0;
    if (millis() - lastDistanceRead > 500) { // Read every 500ms
        ultrasonicSensor.readDistance();
        lastDistanceRead = millis();
        
        // Check if object is too close and play alert sound
        if (ultrasonicSensor.isObjectTooClose()) {
            buzzer.playAlert();
            // Servo continues rotating even when object is close
        }
    }
    
    delay(100); // Faster updates for smooth rotation
}