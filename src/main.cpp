#include <Arduino.h>
#include "TemperatureSensor.h"
#include "UltrasonicSensor.h"
#include "Buzzer.h"
#include "LED.h"
#include "Display.h"

TemperatureSensor tempSensor(2); // DHT11 on GPIO2
UltrasonicSensor ultrasonicSensor(5, 4, 15.0); // HC-SR04: Trig=GPIO5(D1), Echo=GPIO4(D2), Alert=15cm
Buzzer buzzer(14); // TMB12A12 on GPIO14(D5)
LED led(13); // Single color LED on GPIO13(D7)
Display display(1, 3); // I2C Display: SCL=GPIO1(TX), SDA=GPIO3(RX)

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    // Initialize display first for visual feedback
    display.begin();
    display.showHello();
    delay(3000);
    
    tempSensor.begin();
    ultrasonicSensor.begin();
    buzzer.begin();
    led.begin();
    
    // Test components on startup
    buzzer.test();
    ultrasonicSensor.test();
    led.test();
    
    // Show initial status on display
    display.showMessage("System Ready!");
    delay(2000);
}

void loop() {
    tempSensor.readTemperature(); // Read and display temperature
    
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
    
    // Update display with current sensor data
    static unsigned long lastDisplayUpdate = 0;
    if (millis() - lastDisplayUpdate > 1000) { // Update display every second
        display.updateDisplay(
            tempSensor.getTemperature(),
            tempSensor.getHumidity(),
            ultrasonicSensor.getDistance(),
            alarmActive,
            0
        );
        lastDisplayUpdate = millis();
    }
    
    delay(100); // Standard update interval
}