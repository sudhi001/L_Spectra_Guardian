#ifndef WIFI_SCANNER_H
#define WIFI_SCANNER_H

#include <ESP8266WiFi.h>

class WiFiScanner {
private:
    const char* targetSSID;
    const char* password;
    bool isConnected;
    
public:
    WiFiScanner(const char* ssid, const char* pwd);
    void begin();
    void scan();
    bool connect();
    bool isConnectedToTarget();
    void checkConnectionStatus();
};

#endif
