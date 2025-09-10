#include "WiFiScanner.h"

WiFiScanner::WiFiScanner(const char* ssid, const char* pwd) {
    targetSSID = ssid;
    password = pwd;
    isConnected = false;
}

void WiFiScanner::begin() {
    Serial.println("WiFi Scanner Starting...");
    Serial.print("Looking for network: ");
    Serial.println(targetSSID);
    
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(1000);
    
    Serial.println("WiFi initialized in station mode");
}

void WiFiScanner::checkConnectionStatus() {
    if (WiFi.status() == WL_CONNECTED) {
        if (!isConnected) {
            isConnected = true;
            Serial.println("Connected to WiFi!");
            Serial.print("SSID: ");
            Serial.println(WiFi.SSID());
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());
        }
    } else {
        if (isConnected) {
            isConnected = false;
            Serial.println("WiFi disconnected. Starting scan...");
            scan();
        }
    }
}

bool WiFiScanner::isConnectedToTarget() {
    return (WiFi.status() == WL_CONNECTED && WiFi.SSID() == targetSSID);
}

bool WiFiScanner::connect() {
    Serial.print("Attempting to connect to ");
    Serial.println(targetSSID);
    
    WiFi.begin(targetSSID, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.println("Connected successfully!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        isConnected = true;
        return true;
    } else {
        Serial.println();
        Serial.println("Connection failed!");
        isConnected = false;
        return false;
    }
}

void WiFiScanner::scan() {
    if (isConnectedToTarget()) {
        Serial.println("Already connected to target network. No need to scan.");
        return;
    }
    
    Serial.println("Scanning for WiFi networks...");
    Serial.println("Please wait...");
    
    WiFi.scanDelete();
    delay(100);
    
    int n = WiFi.scanNetworks();
    
    if (n == WIFI_SCAN_FAILED) {
        Serial.println("WiFi scan failed!");
        return;
    }
    
    if (n == 0) {
        Serial.println("No networks found");
        Serial.println("Possible reasons:");
        Serial.println("- No WiFi networks in range");
        Serial.println("- WiFi not properly initialized");
        Serial.println("- Hardware issue");
        return;
    }
    
    Serial.print(n);
    Serial.println(" networks found:");
    Serial.println("-------------------");
    
    bool foundTarget = false;
    
    for (int i = 0; i < n; ++i) {
        String ssid = WiFi.SSID(i);
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(ssid);
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(" dBm) ");
        Serial.print("Channel: ");
        Serial.print(WiFi.channel(i));
        Serial.print(" ");
        Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "Open" : "Encrypted");
        
        if (ssid == targetSSID) {
            foundTarget = true;
            Serial.println("*** TARGET NETWORK FOUND! ***");
            Serial.print("Network: ");
            Serial.println(targetSSID);
            Serial.print("Password: ");
            Serial.println(password);
            Serial.println("Attempting to connect...");
            
            if (connect()) {
                Serial.println("Successfully connected to target network!");
            }
            break;
        }
        
        delay(10);
    }
    
    if (!foundTarget) {
        Serial.println("Target network 'ACHU-APPU-2G' not found in this scan.");
    }
    
    Serial.println("-------------------");
    Serial.println("Scan complete.");
}
