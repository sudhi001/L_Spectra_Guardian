#include "MQSensor.h"

MQSensor::MQSensor(int pin) : analogPin(pin), airQuality(0.0), lastRead(0) {
}

void MQSensor::begin() {
    pinMode(analogPin, INPUT);
    Serial.println("MQ Sensor initialized");
}

void MQSensor::readAirQuality() {
    unsigned long currentTime = millis();
    if (currentTime - lastRead >= READ_INTERVAL) {
        int rawValue = analogRead(analogPin);
        
        // Convert analog reading (0-1023) to voltage (0-3.3V)
        float voltage = rawValue * (3.3 / 1023.0);
        
        // Convert voltage to air quality percentage
        // MQ sensors typically read higher values for better air quality
        // Lower voltage = better air quality
        airQuality = map(voltage * 100, 0, 330, 100, 0);
        airQuality = constrain(airQuality, 0, 100);
        
        lastRead = currentTime;
        
        Serial.print("MQ Sensor - Raw: ");
        Serial.print(rawValue);
        Serial.print(", Voltage: ");
        Serial.print(voltage);
        Serial.print("V, Air Quality: ");
        Serial.print(airQuality);
        Serial.println("%");
    }
}

float MQSensor::getAirQuality() {
    return airQuality;
}

String MQSensor::getAirQualityStatus() {
    if (airQuality >= 80) return "Excellent";
    else if (airQuality >= 60) return "Good";
    else if (airQuality >= 40) return "Fair";
    else if (airQuality >= 20) return "Poor";
    else return "Very Poor";
}

void MQSensor::test() {
    Serial.println("Testing MQ Sensor...");
    readAirQuality();
    Serial.print("Air Quality: ");
    Serial.print(airQuality);
    Serial.print("% (");
    Serial.print(getAirQualityStatus());
    Serial.println(")");
}
