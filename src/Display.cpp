#include "Display.h"

Display::Display(int scl, int sda) : 
    display(128, 64, &Wire, -1), // 128x64 OLED, no reset pin
    sclPin(scl), 
    sdaPin(sda),
    initialized(false) {
}

void Display::begin() {
    Wire.begin(sdaPin, sclPin); // Initialize I2C with custom pins
    
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 allocation failed");
        Serial.println("Check I2C connections: SCL=GPIO1(TX), SDA=GPIO3(RX)");
        Serial.println("Try I2C address 0x3D if 0x3C doesn't work");
        return;
    }
    
    initialized = true;
    Serial.println("OLED Display initialized successfully");
    
    display.clearDisplay();
    display.setTextSize(1); // Smaller text for better fit
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10);
    display.println("L SPECTRA GUARDIAN");
    display.setCursor(0, 25);
    display.println("IoT Monitoring");
    display.setCursor(0, 40);
    display.println("System Ready!");
    display.display();
    delay(2000);
}

void Display::test() {
    if (!initialized) return;
    
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(25, 20);
    display.println("TEST");
    display.display();
    delay(1000);
}

void Display::showMessage(const char* message) {
    if (!initialized) return;
    
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 2);
    display.println(message);
    display.display();
}

void Display::showHello() {
    if (!initialized) return;
    
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(15, 10);
    display.println("I am");
    display.setCursor(10, 30);
    display.println("Ritwik");
    display.display();
}

void Display::updateDisplay(float temperature, float humidity, float distance, float airQuality, bool alarm) {
    if (!initialized) return;
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // Temperature
    display.setCursor(0, 0);
    display.print("Temp: ");
    display.print(temperature, 1);
    display.println(" C");
    
    // Humidity
    display.setCursor(0, 12);
    display.print("Hum:  ");
    display.print(humidity, 1);
    display.println(" %");
    
    // Distance
    display.setCursor(0, 24);
    display.print("Dist: ");
    display.print(distance, 1);
    display.println(" cm");
    
    // Air Quality with descriptive text
    display.setCursor(0, 36);
    display.print("Air: ");
    if (airQuality >= 80) {
        display.println("Excellent");
    } else if (airQuality >= 60) {
        display.println("Good");
    } else if (airQuality >= 40) {
        display.println("Fair");
    } else if (airQuality >= 20) {
        display.println("Poor");
    } else {
        display.println("Very Poor");
    }
    
    // Alarm Status
    display.setCursor(0, 48);
    display.print("Status: ");
    display.println(alarm ? "ALERT!" : "SAFE");
    
    display.display();
}

void Display::clear() {
    if (!initialized) return;
    display.clearDisplay();
    display.display();
}
