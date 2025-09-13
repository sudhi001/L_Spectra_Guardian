#include "Display.h"

Display::Display(int cs, int dc, int rst, int led) 
    : csPin(cs), dcPin(dc), rstPin(rst), ledPin(led), isInitialized(false),
      lastTemperature(-999), lastHumidity(-999), lastDistance(-999), lastAlarmState(false) {
    
    // Initialize colors (using 16-bit RGB565 format)
    backgroundColor = TFT_BLACK;
    textColor = TFT_WHITE;
    headerColor = TFT_BLUE;
    alarmColor = TFT_RED;
}

void Display::begin() {
    Serial.println("Initializing MB3 Display...");
    
    // Initialize TFT display
    tft.init();
    tft.setRotation(1); // Landscape orientation
    
    // Set initial colors
    tft.fillScreen(backgroundColor);
    
    // Initialize backlight control pin
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH); // Turn on backlight
    
    // Show startup screen
    showHeader();
    showMessage("L Spectra Guardian", TFT_GREEN);
    delay(2000);
    
    // Clear and show main interface
    clear();
    showHeader();
    
    isInitialized = true;
    lastUpdate = millis();
    
    Serial.println("MB3 Display initialized successfully!");
}

void Display::test() {
    if (!isInitialized) {
        Serial.println("Display not initialized!");
        return;
    }
    
    Serial.println("Testing MB3 Display...");
    
    // Test different colors
    tft.fillScreen(TFT_RED);
    delay(500);
    tft.fillScreen(TFT_GREEN);
    delay(500);
    tft.fillScreen(TFT_BLUE);
    delay(500);
    tft.fillScreen(TFT_YELLOW);
    delay(500);
    tft.fillScreen(TFT_MAGENTA);
    delay(500);
    tft.fillScreen(TFT_CYAN);
    delay(500);
    
    // Test text
    tft.fillScreen(backgroundColor);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.drawString("Display Test", 50, 50);
    tft.setTextSize(1);
    tft.drawString("All colors working!", 50, 100);
    
    delay(2000);
    
    // Return to normal display
    clear();
    showHeader();
    
    Serial.println("Display test completed!");
}

void Display::clear() {
    tft.fillScreen(backgroundColor);
}

void Display::showHeader() {
    // Draw header background
    tft.fillRect(0, 0, tft.width(), HEADER_HEIGHT, headerColor);
    
    // Draw title
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.drawString("L Spectra Guardian", MARGIN, 5);
    
    // Draw separator line
    tft.drawLine(0, HEADER_HEIGHT, tft.width(), HEADER_HEIGHT, TFT_WHITE);
}

void Display::showTemperature(float temp, float humidity) {
    if (temp == lastTemperature && humidity == lastHumidity) return;
    
    int y = HEADER_HEIGHT + MARGIN;
    
    // Clear previous temperature area
    tft.fillRect(0, y, tft.width(), LINE_HEIGHT * 2, backgroundColor);
    
    // Draw temperature icon
    drawSensorIcon(MARGIN, y, 0); // Temperature icon
    
    // Display temperature
    tft.setTextColor(TFT_CYAN);
    tft.setTextSize(2);
    char tempStr[20];
    formatFloat(temp, tempStr, 1);
    tft.drawString("Temp: ", MARGIN + 30, y);
    tft.drawString(tempStr, MARGIN + 100, y);
    tft.drawString("C", MARGIN + 180, y);
    
    // Display humidity
    y += LINE_HEIGHT;
    tft.setTextColor(TFT_BLUE);
    tft.setTextSize(2);
    char humStr[20];
    formatFloat(humidity, humStr, 1);
    tft.drawString("Humidity: ", MARGIN + 30, y);
    tft.drawString(humStr, MARGIN + 120, y);
    tft.drawString("%", MARGIN + 200, y);
    
    lastTemperature = temp;
    lastHumidity = humidity;
}

void Display::showDistance(float distance) {
    if (distance == lastDistance) return;
    
    int y = HEADER_HEIGHT + MARGIN + (LINE_HEIGHT * 2) + MARGIN;
    
    // Clear previous distance area
    tft.fillRect(0, y, tft.width(), LINE_HEIGHT, backgroundColor);
    
    // Draw distance icon
    drawSensorIcon(MARGIN, y, 1); // Distance icon
    
    // Display distance
    tft.setTextColor(TFT_YELLOW);
    tft.setTextSize(2);
    char distStr[20];
    formatFloat(distance, distStr, 1);
    tft.drawString("Distance: ", MARGIN + 30, y);
    tft.drawString(distStr, MARGIN + 120, y);
    tft.drawString("cm", MARGIN + 200, y);
    
    // Draw progress bar for distance visualization
    int barWidth = 200;
    int barHeight = 15;
    int barX = MARGIN + 30;
    int barY = y + 20;
    
    // Background bar
    tft.fillRect(barX, barY, barWidth, barHeight, TFT_DARKGREY);
    
    // Distance bar (green for safe, yellow for warning, red for danger)
    uint16_t barColor = TFT_GREEN;
    if (distance < 30) barColor = TFT_YELLOW;
    if (distance < 15) barColor = TFT_RED;
    
    int progress = map(constrain(distance, 0, 100), 0, 100, 0, barWidth);
    tft.fillRect(barX, barY, progress, barHeight, barColor);
    
    lastDistance = distance;
}

void Display::showAlarmStatus(bool alarmActive) {
    if (alarmActive == lastAlarmState) return;
    
    int y = HEADER_HEIGHT + MARGIN + (LINE_HEIGHT * 3) + MARGIN;
    
    // Clear previous alarm area
    tft.fillRect(0, y, tft.width(), LINE_HEIGHT, backgroundColor);
    
    // Draw alarm indicator
    drawAlarmIndicator(alarmActive);
    
    // Display alarm status
    tft.setTextSize(2);
    if (alarmActive) {
        tft.setTextColor(alarmColor);
        tft.drawString("ALARM: OBJECT TOO CLOSE!", MARGIN + 30, y);
    } else {
        tft.setTextColor(TFT_GREEN);
        tft.drawString("Status: SAFE", MARGIN + 30, y);
    }
    
    lastAlarmState = alarmActive;
}

void Display::showServoStatus(float angle) {
    int y = HEADER_HEIGHT + MARGIN + (LINE_HEIGHT * 4) + MARGIN;
    
    // Clear previous servo area
    tft.fillRect(0, y, tft.width(), LINE_HEIGHT, backgroundColor);
    
    // Draw servo icon
    drawSensorIcon(MARGIN, y, 2); // Servo icon
    
    // Display servo angle
    tft.setTextColor(TFT_MAGENTA);
    tft.setTextSize(2);
    char angleStr[20];
    formatFloat(angle, angleStr, 0);
    tft.drawString("Servo: ", MARGIN + 30, y);
    tft.drawString(angleStr, MARGIN + 100, y);
    tft.drawString("deg", MARGIN + 150, y);
    
    // Draw servo position indicator
    int indicatorWidth = 100;
    int indicatorHeight = 10;
    int indicatorX = MARGIN + 200;
    int indicatorY = y + 5;
    
    // Background
    tft.fillRect(indicatorX, indicatorY, indicatorWidth, indicatorHeight, TFT_DARKGREY);
    
    // Position indicator
    int position = map(constrain(angle, 0, 180), 0, 180, 0, indicatorWidth);
    tft.fillRect(indicatorX, indicatorY, position, indicatorHeight, TFT_MAGENTA);
}

void Display::updateDisplay(float temp, float humidity, float distance, bool alarmActive, float servoAngle) {
    if (!isInitialized) return;
    
    // Only update if enough time has passed
    if (millis() - lastUpdate < UPDATE_INTERVAL) return;
    
    showTemperature(temp, humidity);
    showDistance(distance);
    showAlarmStatus(alarmActive);
    showServoStatus(servoAngle);
    
    lastUpdate = millis();
}

void Display::setBrightness(int brightness) {
    // Control backlight brightness (0-255)
    brightness = constrain(brightness, 0, 255);
    analogWrite(ledPin, brightness);
}

void Display::showMessage(const char* message, int color) {
    tft.setTextColor(color);
    tft.setTextSize(2);
    tft.drawString(message, MARGIN, tft.height() / 2);
}

void Display::drawProgressBar(int x, int y, int width, int height, int progress, uint16_t color) {
    // Background
    tft.fillRect(x, y, width, height, TFT_DARKGREY);
    
    // Progress
    progress = constrain(progress, 0, 100);
    int progressWidth = (progress * width) / 100;
    tft.fillRect(x, y, progressWidth, height, color);
    
    // Border
    tft.drawRect(x, y, width, height, TFT_WHITE);
}

void Display::drawAlarmIndicator(bool active) {
    int x = MARGIN;
    int y = HEADER_HEIGHT + MARGIN + (LINE_HEIGHT * 3) + MARGIN;
    
    if (active) {
        // Draw blinking alarm icon
        tft.fillCircle(x + 10, y + 10, 8, alarmColor);
        tft.fillCircle(x + 10, y + 10, 5, TFT_WHITE);
    } else {
        // Draw safe icon
        tft.fillCircle(x + 10, y + 10, 8, TFT_GREEN);
        tft.fillCircle(x + 10, y + 10, 5, TFT_WHITE);
    }
}

void Display::drawSensorIcon(int x, int y, int iconType) {
    switch (iconType) {
        case 0: // Temperature icon
            tft.fillCircle(x + 10, y + 10, 8, TFT_CYAN);
            tft.drawString("T", x + 6, y + 3);
            break;
        case 1: // Distance icon
            tft.fillRect(x + 5, y + 5, 15, 10, TFT_YELLOW);
            tft.drawString("D", x + 8, y + 3);
            break;
        case 2: // Servo icon
            tft.fillRect(x + 5, y + 5, 15, 10, TFT_MAGENTA);
            tft.drawString("S", x + 8, y + 3);
            break;
    }
}

void Display::formatFloat(float value, char* buffer, int decimals) {
    dtostrf(value, 0, decimals, buffer);
}
