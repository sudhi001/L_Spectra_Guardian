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
        return;
    }
    
    initialized = true;
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 20);
    display.println("READY");
    display.display();
    delay(1000);
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
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
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

void Display::updateDisplay(float temperature, float humidity, float distance, bool alarm, int servoAngle) {
    if (!initialized) return;
    
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    
    // Temperature
    display.setCursor(0, 0);
    display.print("T:");
    display.print(temperature);
    display.println("C");
    
    // Humidity
    display.setCursor(0, 20);
    display.print("H:");
    display.print(humidity);
    display.println("%");
    
    // Distance
    display.setCursor(0, 40);
    display.print("D:");
    display.print(distance);
    display.println("cm");
    
    display.display();
}

void Display::clear() {
    if (!initialized) return;
    display.clearDisplay();
    display.display();
}
