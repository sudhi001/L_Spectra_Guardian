#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include <FS.h>
#include <WiFiClient.h>
#include "TemperatureSensor.h"
#include "UltrasonicSensor.h"
#include "Buzzer.h"
#include "LED.h"
#include "Display.h"
#include "MQSensor.h"

TemperatureSensor tempSensor(2); // DHT11 on GPIO2
UltrasonicSensor ultrasonicSensor(5, 4, 15.0); // HC-SR04: Trig=GPIO5(D1), Echo=GPIO4(D2), Alert=15cm
Buzzer buzzer(14); // TMB12A12 on GPIO14(D5)
LED led(13); // Single color LED on GPIO13(D7)
MQSensor mqSensor(0); // MH MQ sensor on A0 (GPIO0)
Display display(1, 3); // I2C Display: SCL=GPIO1(TX), SDA=GPIO3(RX)

// WiFi Access Point Configuration
const char* ssid = "L_Spectra_Guardian";
const char* password = ""; // Open access point
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Global alarm state for consistent hardware and web interface
bool alarmActive = false;

// Power optimization variables
unsigned long lastSensorRead = 0;
unsigned long lastWebSocketUpdate = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastDistanceRead = 0;
const unsigned long SENSOR_READ_INTERVAL = 2000;    // Read sensors every 2 seconds
const unsigned long WEBSOCKET_UPDATE_INTERVAL = 1000; // WebSocket updates every 1 second
const unsigned long DISPLAY_UPDATE_INTERVAL = 1000;   // Display updates every 1 second
const unsigned long DISTANCE_READ_INTERVAL = 500;     // Distance checks every 500ms

// Web server functions
void handleRoot() {
    File file = SPIFFS.open("/index.html", "r");
    if (file) {
        server.streamFile(file, "text/html");
        file.close();
    } else {
        // Fallback HTML page if SPIFFS file not found
        String html = "<!DOCTYPE html><html><head><title>L Spectra Guardian</title>";
        html += "<meta name='viewport' content='width=device-width, initial-scale=1.0, user-scalable=no'>";
        html += "<style>body{font-family:Arial;background:#667eea;color:white;text-align:center;padding:10px;}";
        html += ".container{background:rgba(255,255,255,0.1);padding:20px;border-radius:15px;max-width:100%;margin:0 auto;}";
        html += ".sensor{background:rgba(255,255,255,0.2);margin:10px;padding:15px;border-radius:10px;}";
        html += ".value{font-size:1.8em;font-weight:bold;margin:10px 0;}";
        html += ".alert{background:#ff6b6b;padding:10px;border-radius:10px;margin:10px 0;}";
        html += "@media (max-width: 600px) {.container{padding:15px;} .value{font-size:1.5em;}}</style></head>";
        html += "<body><div class='container'><h1>L Spectra Guardian</h1>";
        html += "<div class='sensor'><h3>TEMP Temperature</h3><div class='value' id='temp'>--°C</div></div>";
        html += "<div class='sensor'><h3>HUM Humidity</h3><div class='value' id='hum'>--%</div></div>";
        html += "<div class='sensor'><h3>AIR Air Quality</h3><div class='value' id='aq'>--%</div></div>";
        html += "<div class='sensor'><h3>DIST Distance</h3><div class='value' id='dist'>--cm</div></div>";
        html += "<div class='alert' id='alert'>Status: SAFE</div>";
        html += "<script>setInterval(()=>{fetch('/api/sensors').then(r=>r.json()).then(d=>{";
        html += "document.getElementById('temp').textContent=d.temperature.toFixed(1)+'C';";
        html += "document.getElementById('hum').textContent=d.humidity.toFixed(1)+'%';";
        html += "document.getElementById('aq').textContent=d.airQuality.toFixed(1)+'%';";
        html += "document.getElementById('dist').textContent=d.distance.toFixed(1)+'cm';";
        html += "document.getElementById('alert').textContent='Status: '+(d.alarm?'ALERT':'SAFE');";
        html += "document.getElementById('alert').style.background=d.alarm?'#ff6b6b':'#2ecc71';";
        html += "}).catch(e=>console.error(e));},2000);</script></div></body></html>";
        server.send(200, "text/html", html);
    }
}

void handleAPI() {
    String json = "{";
    json += "\"temperature\":" + String(tempSensor.getTemperature()) + ",";
    json += "\"humidity\":" + String(tempSensor.getHumidity()) + ",";
    json += "\"distance\":" + String(ultrasonicSensor.getDistance()) + ",";
    json += "\"airQuality\":" + String(mqSensor.getAirQuality()) + ",";
    json += "\"alarm\":" + String(ultrasonicSensor.isObjectTooClose() ? "true" : "false");
    json += "}";
    
    server.send(200, "application/json", json);
}

// Function to broadcast sensor data to all connected WebSocket clients
void sendSensorData(bool alarmStatus) {
    String json = "{";
    json += "\"temperature\":" + String(tempSensor.getTemperature()) + ",";
    json += "\"humidity\":" + String(tempSensor.getHumidity()) + ",";
    json += "\"distance\":" + String(ultrasonicSensor.getDistance()) + ",";
    json += "\"airQuality\":" + String(mqSensor.getAirQuality()) + ",";
    json += "\"alarm\":" + String(alarmStatus ? "true" : "false");
    json += "}";
    
    Serial.print("Broadcasting WebSocket data: ");
    Serial.println(json);
    Serial.print("Alarm status: ");
    Serial.println(alarmStatus ? "true" : "false");
    
    webSocket.broadcastTXT(json);
}

// WebSocket event handlers
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            // Send initial data to newly connected client
            sendSensorData(alarmActive);
            break;
        }
        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);
            break;
        default:
            break;
    }
}

void setupWiFi() {
    // Optimize WiFi power settings
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    
    // Set WiFi power to maximum for stable connection
    WiFi.setOutputPower(20.5); // Maximum power (20.5 dBm)
    
    Serial.println("WiFi Access Point Started");
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());
    
    // Setup mDNS for local domain name
    if (MDNS.begin("app")) {
        Serial.println("mDNS responder started");
        Serial.println("Access via: http://app.local");
        MDNS.addService("http", "tcp", 80);
    } else {
        Serial.println("mDNS responder failed to start");
    }
    
    // Setup web server routes
    server.on("/", handleRoot);
    server.on("/api/sensors", handleAPI);
    server.begin();
    
    // Setup WebSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    
    Serial.println("Web server started");
    Serial.println("WebSocket server started on port 81");
    Serial.println("Access URLs:");
    Serial.println("  http://192.168.4.1");
    Serial.println("  http://app.local");
}

void setup() {
    Serial.begin(115200);
    delay(5000);  // Wait longer for ESP8266 boot messages to finish
    
    // Clear any remaining boot messages
    Serial.flush();
    delay(1000);
    
    // Clear the screen and start fresh
    Serial.println("\n\n\n\n\n\n\n\n\n\n");
    Serial.println("========================================");
    Serial.println("=== L Spectra Guardian Starting ===");
    Serial.println("ESP8266 NodeMCU Initialized");
    Serial.println("========================================");
    
    // Initialize SPIFFS for web files
    if (!SPIFFS.begin()) {
        Serial.println("SPIFFS initialization failed!");
        return;
    }
    Serial.println("SPIFFS initialized successfully");
    
    // Initialize display first for visual feedback
    display.begin();
    display.showHello();
    delay(3000);
    
    tempSensor.begin();
    ultrasonicSensor.begin();
    buzzer.begin();
    led.begin();
    mqSensor.begin();
    
    // Test components on startup
    buzzer.test();
    ultrasonicSensor.test();
    led.test();
    mqSensor.test();
    
    // Show initial status on display
    display.showMessage("System Ready!");
    delay(2000);
    
    // Setup WiFi Access Point
    setupWiFi();
}

void loop() {
    unsigned long currentTime = millis();
    
    // Read sensors at optimized intervals
    if (currentTime - lastSensorRead > SENSOR_READ_INTERVAL) {
        tempSensor.readTemperature();
        mqSensor.readAirQuality();
        lastSensorRead = currentTime;
    }
    
    // Update LED blinking (non-blocking)
    led.update();
    
    // Read distance at optimized intervals
    if (currentTime - lastDistanceRead > DISTANCE_READ_INTERVAL) {
        ultrasonicSensor.readDistance();
        lastDistanceRead = currentTime;
        
        // Check if object is too close and play alert sound
        if (ultrasonicSensor.isObjectTooClose()) {
            if (!alarmActive) {
                led.on();
                buzzer.playAlert();
                alarmActive = true;
                sendSensorData(alarmActive);
            }
        } else {
            if (alarmActive) {
                led.off();
                alarmActive = false;
                sendSensorData(alarmActive);
            }
        }
    }
    
    // Update display at optimized intervals
    if (currentTime - lastDisplayUpdate > DISPLAY_UPDATE_INTERVAL) {
        display.updateDisplay(
            tempSensor.getTemperature(),
            tempSensor.getHumidity(),
            ultrasonicSensor.getDistance(),
            mqSensor.getAirQuality(),
            alarmActive
        );
        
        // Print sensor values to serial monitor
        Serial.println("========================================");
        Serial.println("=== SENSOR READINGS ===");
        Serial.print("Temperature: ");
        Serial.print(tempSensor.getTemperature());
        Serial.println(" °C");
        Serial.print("Humidity: ");
        Serial.print(tempSensor.getHumidity());
        Serial.println(" %");
        Serial.print("Air Quality: ");
        Serial.print(mqSensor.getAirQuality());
        Serial.println(" %");
        Serial.print("Distance: ");
        Serial.print(ultrasonicSensor.getDistance());
        Serial.println(" cm");
        Serial.print("Alarm Status: ");
        Serial.println(alarmActive ? "ACTIVE" : "SAFE");
        Serial.println("========================================");
        
        lastDisplayUpdate = currentTime;
    }
    
    // Handle web server requests (non-blocking)
    server.handleClient();
    
    // Handle WebSocket events (non-blocking)
    webSocket.loop();
    
    // Handle mDNS requests (non-blocking)
    MDNS.update();
    
    // Broadcast sensor data via WebSocket at optimized intervals
    if (currentTime - lastWebSocketUpdate > WEBSOCKET_UPDATE_INTERVAL) {
        sendSensorData(alarmActive);
        lastWebSocketUpdate = currentTime;
    }
    
    // Non-blocking delay - yield to other tasks
    yield();
}