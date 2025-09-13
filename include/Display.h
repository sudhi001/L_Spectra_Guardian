#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>

class Display {
private:
    TFT_eSPI tft;
    int csPin;
    int dcPin;
    int rstPin;
    int ledPin;
    bool isInitialized;
    
    // Display layout constants
    static const int HEADER_HEIGHT = 30;
    static const int LINE_HEIGHT = 25;
    static const int MARGIN = 10;
    
    // Color definitions
    uint16_t backgroundColor;
    uint16_t textColor;
    uint16_t headerColor;
    uint16_t alarmColor;
    
    // Display update tracking
    unsigned long lastUpdate;
    static const unsigned long UPDATE_INTERVAL = 1000; // Update every 1 second
    
    // Previous values for comparison
    float lastTemperature;
    float lastHumidity;
    float lastDistance;
    bool lastAlarmState;
    
public:
    Display(int cs = 15, int dc = 0, int rst = 16, int led = 1);
    
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
    void setBrightness(int brightness);
    void showMessage(const char* message, int color = TFT_WHITE);
    void drawProgressBar(int x, int y, int width, int height, int progress, uint16_t color);
    
private:
    void drawAlarmIndicator(bool active);
    void drawSensorIcon(int x, int y, int iconType);
    void formatFloat(float value, char* buffer, int decimals = 1);
};

#endif
