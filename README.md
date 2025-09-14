# L Spectra Guardian 🌟

A comprehensive IoT sensor monitoring system built with ESP8266 NodeMCU that monitors temperature, humidity, proximity, air quality, and provides audio/visual alerts. This project was lovingly created by a father and son team for a school exhibition at Loyola School, Trivandrum.

## 📸 System Overview

![L Spectra Guardian IoT Monitoring System](./screenshots/L%20Spectra%20Guardian.png)

*The L Spectra Guardian in action - monitoring environmental conditions with real-time web interface*

## 👨‍👦 Project Story

This project was developed as a collaborative effort between a father and his UKG (Upper Kindergarten) son for the "L Spectra" school exhibition at Loyola School, Trivandrum. The young student actively participated in development, testing, and will be presenting this innovative project at the school exhibition, showcasing the wonderful world of IoT and sensor technology.

**Project Goals:**
- 🎓 **Educational**: Learning about sensors, programming, and electronics
- 🏆 **Exhibition**: Presenting at "L Spectra" school exhibition
- 🤝 **Collaboration**: Father-son bonding through technology
- 🌍 **Sharing**: Open source for other students and families to learn

## 🚀 Features

- **📺 Visual Display** - 0.91" I2C OLED display (128x64) with real-time sensor data visualization
- **🌡️ Temperature & Humidity Monitoring** - DHT11 sensor keeps track of environmental conditions
- **📏 Proximity Detection** - HC-SR04 ultrasonic sensor with 15cm alert threshold for safety
- **🌬️ Air Quality Monitoring** - MH MQ sensor detects air quality levels with status indicators
- **🔊 Audio Alerts** - TMB12A12 buzzer with proximity warnings (great for demonstrations!)
- **💡 Visual Indicators** - Single color LED for clear alarm status indication
- **⚡ Real-time Monitoring** - Optimized for responsive performance and smooth demonstrations
- **🎯 Educational Value** - Perfect for learning about sensors, programming, and IoT concepts
- **🎨 Interactive Interface** - Clean display with sensor readings and status indicators
- **🌐 Web Interface** - WiFi access point with responsive web dashboard
- **📱 Mobile-Friendly** - Optimized for smartphones and tablets
- **🔗 Easy Access** - Connect via domain name (app.local) or IP address

## 🌐 Web Interface Access

The L Spectra Guardian creates its own WiFi network for remote monitoring:

### WiFi Connection
- **Network Name**: `L_Spectra_Guardian`
- **Password**: None (open network)
- **Access Methods**:
  - **Domain Name**: `http://app.local` (recommended)
  - **IP Address**: `http://192.168.4.1`

### Web Dashboard Features
- **📊 Real-time Sensor Data** - Live updates every 2 seconds
- **📱 Responsive Design** - Works on phones, tablets, and computers
- **🎨 Modern Interface** - Clean, colorful display with sensor cards
- **📈 Visual Indicators** - Color-coded status for each sensor
- **🚨 Alert System** - Clear indication when proximity alarm is active

### How to Use
1. **Connect to WiFi**: Join "L_Spectra_Guardian" network on your device
2. **Open Browser**: Go to `http://app.local` or `http://192.168.4.1`
3. **Monitor Sensors**: View real-time temperature, humidity, air quality, and distance
4. **Check Alerts**: See if proximity alarm is active
5. **Mobile Access**: Perfect for demonstrations and remote monitoring

### Technical Details
- **mDNS Support**: Uses multicast DNS for `app.local` domain resolution
- **SPIFFS Storage**: Web files stored in ESP8266 flash memory
- **REST API**: JSON endpoint at `/api/sensors` for data access
- **Auto-refresh**: Page updates automatically every 2 seconds

## 📋 Hardware Requirements

### Components
- **ESP8266 NodeMCU v3** (Main controller)
- **0.91" OLED Display** (I2C SSD1306 - 128x64)
- **DHT11** (Temperature & Humidity sensor)
- **HC-SR04** (Ultrasonic distance sensor)
- **MH MQ Sensor** (Air quality sensor)
- **TMB12A12** (Active buzzer)
- **Single Color LED** (Status indicator)
- **Jumper wires** and **breadboard**
- **220Ω resistor** (for LED protection)

### Power Requirements
- **5V** for HC-SR04 and MH MQ sensor
- **3.3V** for DHT11, LED, and OLED Display
- **USB power** from computer or external adapter

## 🔌 Complete Sensor Setup

### 1. 0.91" OLED Display (I2C SSD1306)
```
OLED Display           NodeMCU
┌─────────┐          ┌─────────┐
│   VCC   │ ──────── │   3.3V   │
│   GND   │ ──────── │   GND    │
│   SCL   │ ──────── │   GPIO1  │ (TX) - I2C Clock
│   SDA   │ ──────── │   GPIO3  │ (RX) - I2C Data
└─────────┘          └─────────┘

Note: OLED Display uses I2C communication
- SCL (Serial Clock): GPIO1 (TX)
- SDA (Serial Data): GPIO3 (RX)
- I2C Address: 0x3C (default)
```

### 2. DHT11 (Temperature & Humidity)
```
DHT11 Sensor          NodeMCU
┌─────────┐          ┌─────────┐
│   VCC   │ ──────── │   3.3V  │
│   DATA  │ ──────── │   GPIO2 │ (D4)
│   NC    │ ── X ──── │   (NC) │
│   GND   │ ──────── │   GND   │
└─────────┘          └─────────┘
```

### 3. HC-SR04 (Distance Measurement)
```
HC-SR04 Sensor        NodeMCU
┌─────────┐          ┌─────────┐
│   VCC   │ ──────── │   5V    │
│   GND   │ ──────── │   GND   │
│   Trig  │ ──────── │   GPIO5 │ (D1)
│   Echo  │ ──────── │   GPIO4 │ (D2)
└─────────┘          └─────────┘
```

### 4. MH MQ Sensor (Air Quality)
```
MH MQ Sensor          NodeMCU
┌─────────┐          ┌─────────┐
│   VCC   │ ──────── │   5V    │
│   GND   │ ──────── │   GND   │
│   A0    │ ──────── │   GPIO0 │ (A0)
└─────────┘          └─────────┘
```

### 5. TMB12A12 (Buzzer)
```
TMB12A12 Buzzer       NodeMCU
┌─────────┐          ┌─────────┐
│   (+)   │ ──────── │   GPIO14│ (D5)
│   (-)   │ ──────── │   GND   │
└─────────┘          └─────────┘
```

### 6. Single Color LED
```
LED                   NodeMCU
┌─────────┐          ┌─────────┐
│   (+)   │ ──────── │   GPIO13│ (D7)
│   (-)   │ ──────── │   GND   │
└─────────┘          └─────────┘
```

**Optional:** Add 220Ω resistor between GPIO13 and LED positive for protection.

## 📊 Pin Assignment Summary

| Component | Pin | GPIO | Function |
|-----------|-----|------|----------|
| **OLED Display** | | | |
| OLED VCC | 3.3V | - | Power |
| OLED GND | GND | - | Ground |
| OLED SCL | TX | GPIO1 | I2C Clock |
| OLED SDA | RX | GPIO3 | I2C Data |
| **DHT11** | | | |
| DHT11 VCC | 3.3V | - | Power |
| DHT11 DATA | D4 | GPIO2 | Data |
| DHT11 GND | GND | - | Ground |
| **HC-SR04** | | | |
| HC-SR04 VCC | 5V | - | Power |
| HC-SR04 Trig | D1 | GPIO5 | Trigger |
| HC-SR04 Echo | D2 | GPIO4 | Echo |
| HC-SR04 GND | GND | - | Ground |
| **MH MQ Sensor** | | | |
| MQ VCC | 5V | - | Power |
| MQ GND | GND | - | Ground |
| MQ A0 | A0 | GPIO0 | Analog Input |
| **TMB12A12** | | | |
| Buzzer (+) | D5 | GPIO14 | Signal |
| Buzzer (-) | GND | - | Ground |
| **LED** | | | |
| LED (+) | D7 | GPIO13 | Signal |
| LED (-) | GND | - | Ground |

## 🛠️ Software Setup

### Prerequisites
- **PlatformIO** installed
- **ESP8266 board** drivers installed
- **USB cable** for programming

### Installation
1. Clone this repository
2. Open project in PlatformIO
3. Install dependencies:
   ```bash
   pio lib install
   ```
4. Upload to NodeMCU:
   ```bash
   pio run --target upload
   ```

### Dependencies
- **DHT sensor library** (Adafruit)
- **Adafruit Unified Sensor** (Required by DHT)
- **Adafruit SSD1306** (Adafruit) - For I2C OLED display support
- **Adafruit GFX Library** (Adafruit) - Graphics library for display
- **Servo** (Arduino built-in)

## 🎯 System Behavior

### Normal Operation
- **Temperature monitoring** every loop cycle
- **Distance monitoring** every 500ms
- **Servo rotation** continuous (0° to 180°)
- **Display** shows real-time sensor data with colorful interface
- **LED** OFF
- **Buzzer** silent

### Alarm Operation (Object < 15cm)
- **LED** turns ON (solid)
- **Buzzer** plays 3-beep alert pattern
- **Display** shows red alarm indicator and "OBJECT TOO CLOSE!" message
- **Servo** continues rotating
- **Distance monitoring** continues

### Alarm Clear (Object > 15cm)
- **LED** turns OFF immediately
- **Buzzer** stops
- **Display** shows green "SAFE" status
- **System** returns to normal operation

## 📈 Performance Specifications

- **Temperature Range**: -40°C to 80°C
- **Humidity Range**: 0% to 100%
- **Distance Range**: 2cm to 400cm
- **Alert Threshold**: 15cm
- **Update Frequency**: 100ms main loop
- **Distance Check**: 500ms intervals
- **Servo Speed**: 2 degrees per update

## 🔧 Troubleshooting

### Common Issues

#### HC-SR04 Not Working
- Check 5V power supply
- Verify Trig/Echo connections (GPIO5/GPIO4)
- Ensure object is 2-400cm away
- Try different jumper wires

#### DHT11 Not Reading
- Check 3.3V power supply
- Verify DATA connection (GPIO2)
- Wait 2 seconds after power-on
- Check for loose connections

#### Buzzer Not Sounding
- Verify GPIO14 connection
- Check buzzer polarity (+ to GPIO14, - to GND)
- Try different GPIO pin if needed

#### MQ Sensor Not Reading
- Check 5V power supply
- Verify A0 connection (GPIO0)
- Ensure sensor is properly heated (wait 2-3 minutes)
- Check for loose connections

#### LED Not Working
- Verify GPIO13 connection
- Check LED polarity
- Add 220Ω resistor if needed
- Try different GPIO pin

#### Web Interface Not Accessible
- **WiFi Connection**: Ensure device is connected to "L_Spectra_Guardian" network
- **Domain Name**: Try `http://app.local` first, then `http://192.168.4.1`
- **Browser Issues**: Clear browser cache or try incognito/private mode
- **mDNS Problems**: If `app.local` doesn't work, use IP address `192.168.4.1`
- **Network Settings**: Check if device has internet connection disabled (should be off)
- **ESP8266 Reset**: Power cycle the ESP8266 if web server doesn't start
- **Serial Monitor**: Check for "Web server started" message in serial output

### Serial Monitor Output
```
=== Temperature Reading ===
Temperature: 25.6 °C
Humidity: 45.2 %
===========================

=== Distance Reading ===
Distance: 12.5 cm
WARNING: Object closer than 15.0 cm!
=======================

=== Air Quality Reading ===
MQ Sensor - Raw: 512, Rs: 15.2 kOhms, Rs/Ro: 1.8, Air Quality: 50%
Air Quality: 50% (Fair)
=========================

WiFi Access Point Started
SSID: L_Spectra_Guardian
IP Address: 192.168.4.1
mDNS responder started
Access via: http://app.local
Web server started
Access URLs:
  http://192.168.4.1
  http://app.local

ALERT: Object too close!
```

## 📁 Project Structure

```
hello/
├── include/
│   ├── TemperatureSensor.h
│   ├── UltrasonicSensor.h
│   ├── Buzzer.h
│   ├── LED.h
│   ├── Display.h
│   └── MQSensor.h
├── src/
│   ├── main.cpp
│   ├── TemperatureSensor.cpp
│   ├── UltrasonicSensor.cpp
│   ├── Buzzer.cpp
│   ├── LED.cpp
│   ├── Display.cpp
│   └── MQSensor.cpp
├── data/
│   └── index.html          # Web interface files
├── platformio.ini
└── README.md
```

## 🚀 Getting Started

### For Students & Families
1. **Connect all sensors** according to pin assignments (adult supervision recommended)
2. **Upload the code** to your NodeMCU using PlatformIO
3. **Open serial monitor** (115200 baud) to see the magic happen!
4. **Test the system** by moving objects near the ultrasonic sensor
5. **Observe the behavior**:
   - Temperature and humidity readings
   - Distance measurements
   - LED and buzzer alerts
   - Servo rotation

### For Exhibition Presentation
- **Prepare a demo area** with objects to test proximity detection
- **Explain each sensor** and what it does
- **Show the serial monitor** output to demonstrate real-time data
- **Let visitors interact** by moving objects near the sensors
- **Highlight the educational value** of learning about IoT and sensors

## 🎓 Educational Benefits

This project teaches:
- **Basic Electronics** - Understanding sensors and connections
- **Programming Concepts** - Loops, conditions, and functions
- **IoT Fundamentals** - How devices communicate and work together
- **Problem Solving** - Debugging and troubleshooting
- **Presentation Skills** - Explaining technology to others
- **Collaboration** - Working together on a shared goal

## 🏆 Exhibition Success Tips

### For "L Spectra" Presentation:
1. **Practice the demo** - Make sure everything works smoothly
2. **Prepare simple explanations** - Use age-appropriate language
3. **Show the code** - Let visitors see how programming works
4. **Interactive demo** - Let people try moving objects near sensors
5. **Explain the learning** - Share what you learned while building this
6. **Be proud** - This is an amazing achievement for a UKG student!

### What to Highlight:
- **Father-son collaboration** - Show how families can learn together
- **Real-world applications** - How this technology is used in daily life
- **Future possibilities** - What other sensors could be added
- **Open source sharing** - How others can learn from this project

## 📝 License

This project is open source and available under the MIT License. Feel free to use, modify, and share for educational purposes!

## 🤝 Contributing

Contributions are welcome! This project is especially open to:
- **Students** wanting to learn and improve
- **Parents** looking to build similar projects with their children
- **Teachers** seeking educational IoT projects
- **Anyone** interested in making technology more accessible to young learners

## 📞 Support

For questions about this project or help with your own "L Spectra" exhibition, please open an issue in the repository. We're here to help young minds explore the wonderful world of technology!

---

**🌟 Good luck to my son with the "L Spectra" exhibition presentation! 🌟**

*Remember: The best part of this project isn't just the technology - it's the learning, collaboration, and joy of discovery you've shared together!*
