#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(int pin) {
    dhtPin = pin;
    dht = new DHT(dhtPin, DHT11);
}

void TemperatureSensor::begin() {
    dht->begin();
    Serial.println("DHT11 sensor initialized");
    delay(2000); // Wait for DHT sensor to stabilize
}

void TemperatureSensor::readTemperature() {
    float h = dht->readHumidity();
    float t = dht->readTemperature();
    
    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }
    
    Serial.println("=== Temperature Reading ===");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.println(" %");
    Serial.println("===========================");
}

float TemperatureSensor::getTemperature() {
    return dht->readTemperature();
}

float TemperatureSensor::getHumidity() {
    return dht->readHumidity();
}

bool TemperatureSensor::isSensorWorking() {
    float t = dht->readTemperature();
    float h = dht->readHumidity();
    return !(isnan(t) || isnan(h));
}
