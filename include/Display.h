#ifndef DISPLAY_H
#define DISPLAY_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Display {
private:
    Adafruit_SSD1306 display;
    int sclPin;
    int sdaPin;
    bool initialized;
    
public:
    Display(int scl, int sda);
    void begin();
    void test();
    void showMessage(const char* message);
    void showHello();
    void updateDisplay(float temperature, float humidity, float distance, float airQuality, bool alarm);
    void clear();
};

#endif
