#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include <FS.h>
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
        html += "document.getElementById('temp').textContent=d.temperature.toFixed(1)+'°C';";
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
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    
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
    delay(1000);
    
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
    tempSensor.readTemperature(); // Read and display temperature
    mqSensor.readAirQuality(); // Read air quality
    
    // Update LED blinking
    led.update();
    
    // Read distance less frequently for better reliability
    static unsigned long lastDistanceRead = 0;
    
    if (millis() - lastDistanceRead > 500) { // Check every 500ms
        ultrasonicSensor.readDistance();
        lastDistanceRead = millis();
        
        // Check if object is too close and play alert sound
        if (ultrasonicSensor.isObjectTooClose()) {
            if (!alarmActive) {
                led.on(); // Turn on LED first
                buzzer.playAlert(); // Then play buzzer
                alarmActive = true;
                // Immediately broadcast alarm state change
                sendSensorData(alarmActive);
            }
        } else {
            if (alarmActive) {
                led.off(); // Turn off LED when alarm clears
                alarmActive = false;
                // Immediately broadcast alarm state change
                sendSensorData(alarmActive);
            }
        }
    }
    
    // Update display with current sensor data
    static unsigned long lastDisplayUpdate = 0;
    if (millis() - lastDisplayUpdate > 1000) { // Update display every second
        display.updateDisplay(
            tempSensor.getTemperature(),
            tempSensor.getHumidity(),
            ultrasonicSensor.getDistance(),
            mqSensor.getAirQuality(),
            alarmActive
        );
        lastDisplayUpdate = millis();
    }
    
    // Handle web server requests
    server.handleClient();
    
    // Handle WebSocket events
    webSocket.loop();
    
    // Handle mDNS requests
    MDNS.update();
    
    // Broadcast sensor data via WebSocket every 500ms for real-time updates
    static unsigned long lastWebSocketUpdate = 0;
    if (millis() - lastWebSocketUpdate > 500) {
        sendSensorData(alarmActive);
        lastWebSocketUpdate = millis();
    }
    
    delay(100); // Standard update interval
}