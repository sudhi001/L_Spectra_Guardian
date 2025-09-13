#include "Display.h"

Display::Display(int scl, int sda) : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
                                      sclPin(scl), sdaPin(sda), isInitialized(false), 
                                      lastTemperature(-999), lastHumidity(-999), 
                                      lastDistance(-999), lastAlarmState(false) {
}

void Display::begin() {
    Serial.println("Initializing I2C Display...");
    
    // Initialize I2C communication with custom pins
    Wire.begin(sdaPin, sclPin);
    
    // Initialize SSD1306 display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 allocation failed!");
        return;
    }
    
    // Clear the buffer
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // Show startup screen
    display.setCursor(0, 0);
    display.println("L Spectra Guardian");
    display.println("Initializing...");
    display.display();
    delay(2000);
    
    isInitialized = true;
    lastUpdate = millis();
    
    Serial.println("I2C Display initialized successfully!");
}

void Display::test() {
    if (!isInitialized) {
        Serial.println("Display not initialized!");
        return;
    }
    
    Serial.println("Testing I2C Display...");
    
    // Test different text sizes
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Display Test");
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.println("Size 2");
    display.setTextSize(1);
    display.setCursor(0, 45);
    display.println("All working!");
    display.display();
    
    delay(2000);
    
    // Return to normal display
    clear();
    showHeader();
    
    Serial.println("Display test completed!");
}

void Display::clear() {
    display.clearDisplay();
    display.display();
}

void Display::showHeader() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("L Spectra Guardian");
    display.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);
    display.display();
}

void Display::showTemperature(float temp, float humidity) {
    if (temp == lastTemperature && humidity == lastHumidity) return;
    
    display.clearDisplay();
    showHeader();
    
    // Display temperature prominently
    display.setTextSize(2);
    display.setCursor(0, 15);
    char tempStr[20];
    formatFloat(temp, tempStr, 1);
    display.print("T:");
    display.print(tempStr);
    display.println("C");
    
    // Display humidity prominently
    char humStr[20];
    formatFloat(humidity, humStr, 1);
    display.print("H:");
    display.print(humStr);
    display.println("%");
    
    display.display();
    
    lastTemperature = temp;
    lastHumidity = humidity;
}

void Display::showDistance(float distance) {
    if (distance == lastDistance) return;
    
    display.clearDisplay();
    showHeader();
    
    // Display temperature
    char tempStr[20];
    formatFloat(lastTemperature, tempStr, 1);
    display.setTextSize(2);
    display.setCursor(0, 15);
    display.print("T:");
    display.print(tempStr);
    display.println("C");
    
    // Display humidity prominently
    char humStr[20];
    formatFloat(lastHumidity, humStr, 1);
    display.print("H:");
    display.print(humStr);
    display.println("%");
    
    // Display distance
    char distStr[20];
    formatFloat(distance, distStr, 1);
    display.print("Distance: ");
    display.print(distStr);
    display.println("cm");
    
    // Draw simple progress bar for distance
    int barWidth = 100;
    int barHeight = 8;
    int barX = 0;
    int barY = 50;
    
    // Background bar
    display.drawRect(barX, barY, barWidth, barHeight, SSD1306_WHITE);
    
    // Distance bar (green for safe, red for danger)
    int progress = map(constrain(distance, 0, 100), 0, 100, 0, barWidth - 2);
    if (distance < 15) {
        // Red for danger
        display.fillRect(barX + 1, barY + 1, progress, barHeight - 2, SSD1306_WHITE);
    } else {
        // White for safe
        display.fillRect(barX + 1, barY + 1, progress, barHeight - 2, SSD1306_WHITE);
    }
    
    display.display();
    
    lastDistance = distance;
}

void Display::showAlarmStatus(bool alarmActive) {
    if (alarmActive == lastAlarmState) return;
    
    display.clearDisplay();
    showHeader();
    
    // Display temperature
    char tempStr[20];
    formatFloat(lastTemperature, tempStr, 1);
    display.setTextSize(2);
    display.setCursor(0, 15);
    display.print("T:");
    display.print(tempStr);
    display.println("C");
    
    // Display humidity prominently
    char humStr[20];
    formatFloat(lastHumidity, humStr, 1);
    display.print("H:");
    display.print(humStr);
    display.println("%");
    
    // Display distance
    char distStr[20];
    formatFloat(lastDistance, distStr, 1);
    display.print("Distance: ");
    display.print(distStr);
    display.println("cm");
    
    // Display alarm status
    if (alarmActive) {
        display.setTextSize(2);
        display.setCursor(0, 40);
        display.println("ALARM!");
        display.setTextSize(1);
        display.println("Too Close!");
    } else {
        display.setTextSize(1);
        display.setCursor(0, 40);
        display.println("Status: SAFE");
    }
    
    display.display();
    
    lastAlarmState = alarmActive;
}

void Display::showServoStatus(float angle) {
    display.clearDisplay();
    showHeader();
    
    // Display temperature
    char tempStr[20];
    formatFloat(lastTemperature, tempStr, 1);
    display.setTextSize(2);
    display.setCursor(0, 15);
    display.print("T:");
    display.print(tempStr);
    display.println("C");
    
    // Display humidity prominently
    char humStr[20];
    formatFloat(lastHumidity, humStr, 1);
    display.print("H:");
    display.print(humStr);
    display.println("%");
    
    // Display distance
    char distStr[20];
    formatFloat(lastDistance, distStr, 1);
    display.print("Distance: ");
    display.print(distStr);
    display.println("cm");
    
    // Display servo angle
    char angleStr[20];
    formatFloat(angle, angleStr, 0);
    display.print("Servo: ");
    display.print(angleStr);
    display.println("deg");
    
    display.display();
}

void Display::updateDisplay(float temp, float humidity, float distance, bool alarmActive, float servoAngle) {
    if (!isInitialized) return;
    
    // Only update if enough time has passed
    if (millis() - lastUpdate < UPDATE_INTERVAL) return;
    
    display.clearDisplay();
    showHeader();
    
    // Display all sensor data
    char tempStr[20], humStr[20], distStr[20], angleStr[20];
    formatFloat(temp, tempStr, 1);
    formatFloat(humidity, humStr, 1);
    formatFloat(distance, distStr, 1);
    formatFloat(servoAngle, angleStr, 0);
    
    // Display temperature and humidity prominently
    display.setTextSize(2);
    display.setCursor(0, 15);
    display.print("T:");
    display.print(tempStr);
    display.println("C");
    
    display.print("H:");
    display.print(humStr);
    display.println("%");
    
    // Display other data in smaller text
    display.setTextSize(1);
    display.print("D:");
    display.print(distStr);
    display.print("cm S:");
    display.print(angleStr);
    display.println("deg");
    
    // Show alarm status
    if (alarmActive) {
        display.setTextSize(2);
        display.setCursor(0, 40);
        display.println("ALARM!");
    } else {
        display.setTextSize(1);
        display.setCursor(0, 40);
        display.println("SAFE");
    }
    
    display.display();
    
    lastUpdate = millis();
}

void Display::showMessage(const char* message) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, SCREEN_HEIGHT / 2);
    display.println(message);
    display.display();
}

void Display::drawProgressBar(int x, int y, int width, int height, int progress) {
    // Background
    display.drawRect(x, y, width, height, SSD1306_WHITE);
    
    // Progress
    progress = constrain(progress, 0, 100);
    int progressWidth = (progress * (width - 2)) / 100;
    display.fillRect(x + 1, y + 1, progressWidth, height - 2, SSD1306_WHITE);
}

void Display::drawAlarmIndicator(bool active) {
    if (active) {
        display.setTextSize(2);
        display.setCursor(0, 40);
        display.println("ALARM!");
    } else {
        display.setTextSize(1);
        display.setCursor(0, 40);
        display.println("SAFE");
    }
}

void Display::formatFloat(float value, char* buffer, int decimals) {
    dtostrf(value, 0, decimals, buffer);
}