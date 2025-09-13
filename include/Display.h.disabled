#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Display {
private:
    Adafruit_SSD1306 display;
    int sclPin;
    int sdaPin;
    bool isInitialized;
    
    // Display layout constants
    static const int SCREEN_WIDTH = 128;
    static const int SCREEN_HEIGHT = 64;
    static const int OLED_RESET = -1; // No reset pin needed for I2C
    static const int LINE_HEIGHT = 10;
    static const int MARGIN = 2;
    
    // Display update tracking
    unsigned long lastUpdate;
    static const unsigned long UPDATE_INTERVAL = 1000; // Update every 1 second
    
    // Previous values for comparison
    float lastTemperature;
    float lastHumidity;
    float lastDistance;
    bool lastAlarmState;
    
public:
    Display(int scl = 5, int sda = 4);
    
    void begin();
    void test();
    void clear();
    void showHeader();
    void showTemperature(float temp, float humidity);
    void showDistance(float distance);
    void showAlarmStatus(bool alarmActive);
    void showServoStatus(float angle);
    void updateDisplay(float temp, float humidity, float distance, bool alarmActive, float servoAngle);
    
    // Utility methods
    void showMessage(const char* message);
    void drawProgressBar(int x, int y, int width, int height, int progress);
    
private:
    void drawAlarmIndicator(bool active);
    void formatFloat(float value, char* buffer, int decimals = 1);
};

#endif
