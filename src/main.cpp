#include <Arduino.h>
#include "WiFiScanner.h"
#include "TemperatureSensor.h"
#include "UltrasonicSensor.h"
#include "Buzzer.h"

WiFiScanner wifiScanner("ACHU-APPU-2G", "R#I#T#W#I#K");
TemperatureSensor tempSensor(2); // DHT11 on GPIO2
UltrasonicSensor ultrasonicSensor(5, 4, 15.0); // HC-SR04: Trig=GPIO5(D1), Echo=GPIO4(D2), Alert=15cm
Buzzer buzzer(14); // TMB12A12 on GPIO14(D5)

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    // wifiScanner.begin();
    tempSensor.begin();
    ultrasonicSensor.begin();
    buzzer.begin();
    
    // Test buzzer on startup
    buzzer.test();
    
    // wifiScanner.scan(); // Initial WiFi scan
}

void loop() {
    // wifiScanner.checkConnectionStatus(); // Check WiFi connection
    tempSensor.readTemperature(); // Read and display temperature
    ultrasonicSensor.readDistance(); // Read and display distance
    
    // Check if object is too close and play alert sound
    if (ultrasonicSensor.isObjectTooClose()) {
        buzzer.playAlert();
    }
    
    delay(1000); // Check every 1 second for real-time response
}