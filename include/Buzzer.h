#ifndef BUZZER_H
#define BUZZER_H

class Buzzer {
private:
    int buzzerPin;
    
public:
    Buzzer(int pin);
    void begin();
    void playTone(int frequency, int duration);
    void playAlert();
    void test();
    void stop();
};

#endif
