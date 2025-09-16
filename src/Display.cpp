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
    display.println("L SECTRA GUARDIAN");
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
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    
    // Temperature
    display.setCursor(0, 1);
    display.print("T:");
    display.print(temperature);
    display.println("C");
    
    // Humidity
    display.setCursor(0, 21);
    display.print("H:");
    display.print(humidity);
    display.println("%");
    
    // Distance
    // display.setCursor(0, 24);
    // display.print("D:");
    // display.print(distance);
    // display.println("cm");
    
    // Air Quality
    display.setCursor(0, 41);
    display.print("AQ:");
    display.print(airQuality);
    display.println("%");
    
    
    display.display();
}

void Display::clear() {
    if (!initialized) return;
    display.clearDisplay();
    display.display();
}
