#include "MQSensor.h"

MQSensor::MQSensor(int pin) : analogPin(pin), airQuality(0.0), lastRead(0), 
    r0(0.0), rs(0.0), voltage(0.0), rawValue(0), calibrationComplete(false),
    coPPM(0.0), co2PPM(0.0) {
}

void MQSensor::begin() {
    pinMode(analogPin, INPUT);
    Serial.println("MQ Sensor initialized");
    Serial.println("Starting calibration process...");
    Serial.println("Please ensure sensor is in clean air environment");
    calibrate();
}

void MQSensor::calibrate() {
    Serial.println("Calibrating MQ Sensor...");
    Serial.println("This will take 30 seconds...");
    
    float sum = 0;
    int samples = 0;
    unsigned long startTime = millis();
    
    while (millis() - startTime < CALIBRATION_TIME) {
        rawValue = analogRead(analogPin);
        voltage = rawValue * (3.3 / 1023.0);
        
        // Calculate resistance (assuming 5V supply and 10kΩ load resistor)
        rs = (3.3 - voltage) / voltage * 10000.0;
        sum += rs;
        samples++;
        
        Serial.print("Calibrating... ");
        Serial.print((millis() - startTime) / 1000);
        Serial.println("s");
        delay(1000);
    }
    
    r0 = sum / samples;
    calibrationComplete = true;
    
    Serial.println("Calibration complete!");
    Serial.print("Baseline resistance (R0): ");
    Serial.print(r0);
    Serial.println(" ohms");
}

void MQSensor::readAirQuality() {
    unsigned long currentTime = millis();
    if (currentTime - lastRead >= READ_INTERVAL) {
        rawValue = analogRead(analogPin);
        
        // Convert analog reading (0-1023) to voltage (0-3.3V)
        voltage = rawValue * (3.3 / 1023.0);
        
        // Calculate sensor resistance
        rs = (3.3 - voltage) / voltage * 10000.0;
        
        if (calibrationComplete && r0 > 0) {
            // Calculate air quality based on resistance ratio
            float ratio = rs / r0;
            
            // Calculate specific gas concentrations
            coPPM = calculateCO(ratio);
            co2PPM = calculateCO2(ratio);
            
            // Convert ratio to air quality percentage
            // Lower ratio = better air quality
            if (ratio < 0.5) {
                airQuality = 100 - (ratio * 100);
            } else if (ratio < 1.0) {
                airQuality = 80 - ((ratio - 0.5) * 60);
            } else if (ratio < 2.0) {
                airQuality = 50 - ((ratio - 1.0) * 30);
            } else {
                airQuality = 20 - ((ratio - 2.0) * 20);
            }
            
            airQuality = constrain(airQuality, 0, 100);
        } else {
            // Fallback to simple voltage-based calculation
            airQuality = map(voltage * 100, 0, 330, 100, 0);
            airQuality = constrain(airQuality, 0, 100);
            coPPM = 0;
            co2PPM = 0;
        }
        
        lastRead = currentTime;
        
        Serial.print("MQ Sensor - Raw: ");
        Serial.print(rawValue);
        Serial.print(", Voltage: ");
        Serial.print(voltage, 3);
        Serial.print("V, Rs: ");
        Serial.print(rs, 0);
        Serial.print("Ω, Rs/R0: ");
        Serial.print(rs/r0, 2);
        Serial.print(", CO: ");
        Serial.print(coPPM, 1);
        Serial.print("ppm, CO2: ");
        Serial.print(co2PPM, 1);
        Serial.print("ppm, Air Quality: ");
        Serial.print(airQuality, 1);
        Serial.println("%");
    }
}

float MQSensor::getAirQuality() {
    return airQuality;
}

float MQSensor::getRawValue() {
    return rawValue;
}

float MQSensor::getVoltage() {
    return voltage;
}

float MQSensor::getResistance() {
    return rs;
}

float MQSensor::getResistanceRatio() {
    return (r0 > 0) ? rs / r0 : 0;
}

bool MQSensor::isCalibrated() {
    return calibrationComplete;
}

float MQSensor::getCOPPM() {
    return coPPM;
}

float MQSensor::getCO2PPM() {
    return co2PPM;
}

float MQSensor::calculateCO(float ratio) {
    // CO calculation based on MQ sensor characteristics
    // This is an approximation - actual calibration would require known CO concentrations
    if (ratio < 0.5) return 0;
    else if (ratio < 1.0) return (ratio - 0.5) * 20;  // 0-10 ppm
    else if (ratio < 2.0) return 10 + (ratio - 1.0) * 40;  // 10-50 ppm
    else if (ratio < 3.0) return 50 + (ratio - 2.0) * 50;  // 50-100 ppm
    else return 100 + (ratio - 3.0) * 100;  // 100+ ppm
}

float MQSensor::calculateCO2(float ratio) {
    // CO2 calculation based on MQ sensor characteristics
    // This is an approximation - actual calibration would require known CO2 concentrations
    if (ratio < 0.3) return 400;  // Normal atmospheric CO2
    else if (ratio < 0.8) return 400 + (ratio - 0.3) * 200;  // 400-500 ppm
    else if (ratio < 1.5) return 500 + (ratio - 0.8) * 100;  // 500-570 ppm
    else if (ratio < 2.5) return 570 + (ratio - 1.5) * 50;   // 570-620 ppm
    else return 620 + (ratio - 2.5) * 100;  // 620+ ppm
}

String MQSensor::getCOStatus() {
    if (coPPM < 9) return "Safe";
    else if (coPPM < 35) return "Moderate";
    else if (coPPM < 50) return "High";
    else return "Dangerous";
}

String MQSensor::getCO2Status() {
    if (co2PPM < 400) return "Excellent";
    else if (co2PPM < 600) return "Good";
    else if (co2PPM < 800) return "Moderate";
    else if (co2PPM < 1000) return "Poor";
    else return "Dangerous";
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
