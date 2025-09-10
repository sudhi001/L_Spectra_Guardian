#ifndef LED_H
#define LED_H

class LED {
private:
    int ledPin;
    bool isBlinking;
    unsigned long lastBlinkTime;
    bool ledState;
    int blinkInterval;
    
public:
    LED(int pin);
    void begin();
    void on();
    void off();
    void blink(int interval = 200);
    void stopBlinking();
    void update();
    void test();
};

#endif
