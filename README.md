# ESP32 Complete Diagnostic v3.0.1 - Multilingual

<div align="center">

[![en](https://img.shields.io/badge/lang-en-red.svg)](README.md)
[![fr](https://img.shields.io/badge/lang-fr-blue.svg)](README.fr.md)

![ESP32](https://img.shields.io/badge/ESP32-Compatible-green)
![Version](https://img.shields.io/badge/version-3.0.1-blue)
![Core](https://img.shields.io/badge/Arduino%20Core-3.3.2+-orange)
![License](https://img.shields.io/badge/license-MIT-orange)

**[🇫🇷 Lire en Français](README.fr.md)**

</div>

---

## 📋 Description

Comprehensive **multilingual** diagnostic tool for ESP32 microcontrollers, accessible via modern web interface. Automatically tests all hardware components, analyzes memory, scans peripherals, and generates detailed reports.

**New in v3.0.1**: Upgraded to **Arduino Core ESP32 3.3.2** with optimized memory management, improved WiFi stability, and 12 new system metrics.

---

## ✨ Features

### 🌍 Multilingual Interface
- **French** (default language)
- **English**
- **Dynamic switching**: FR/EN buttons in interface
- **Real-time update**: No page reload required
- **Complete translations**: All texts, labels, messages and status

### 🎨 Modern Design
- **Glassmorphism** with transparency and blur effects
- **Animated gradients** purple/blue
- **Smooth animations** on all elements
- **Responsive interface** for mobile/tablet/desktop

### 🔄 Real-time Updates
- Automatic refresh every **5 seconds**
- Visual pulsing indicator
- Progress bar for next update
- Live system monitoring

### 🧪 Complete Hardware Testing

#### GPIO Test
- All available pins tested
- Input/Output functionality
- Status: OK/FAIL with details

#### I2C Test
- Automatic device scan (0x00-0x7F)
- Common device identification
- Address and device name display

#### SPI Test
- Bus detection and configuration
- Pin mapping (MOSI, MISO, SCLK)
- Compatibility check

#### Memory Test (Enhanced with Core 3.3.2) ⚡
- Free/total heap
- Free/total PSRAM (if available)
- **NEW**: Minimum free memory since boot
- **NEW**: Largest free block
- **NEW**: Memory fragmentation percentage
- Intelligent status (OK/MEDIUM/LOW)

#### WiFi Test (Enhanced with Core 3.3.2) ⚡
- Connection status
- Signal strength (RSSI)
- SSID and IP address
- MAC address
- **NEW**: WiFi channel
- **NEW**: Gateway address
- **NEW**: DNS server
- **NEW**: Subnet mask
- **NEW**: Signal quality (Excellent/Good/Fair/Weak)

#### System Test (Enhanced with Core 3.3.2) ⚡
- Chip model and revision
- CPU cores and frequency
- Flash size and speed
- Uptime
- **NEW**: Last reset reason
- **NEW**: ESP-IDF version
- **NEW**: Overall system status (HEALTHY/WARNING/CRITICAL)

### 💾 Data Export
- **JSON format**: Complete structured data
- **CSV format**: Excel compatible
- One-click download
- All diagnostic data included

### 🎛️ NeoPixel Control (Optional)
- Predefined patterns (Rainbow, Pulse, Strobe)
- Custom RGB color
- Real-time control

---

## 🚀 Quick Start

### Prerequisites
- **ESP32** (ESP32-S2, ESP32-S3, or ESP32-C3)
- **Arduino IDE 2.x** or PlatformIO
- **Arduino Core ESP32 v3.3.2+** ⚡ (minimum required)
- Modern web browser

### Installation

1. **Install ESP32 Board Manager v3.3.2+**
   ```
   File > Preferences > Additional Board Manager URLs:
   https://espressif.github.io/arduino-esp32/package_esp32_index.json

   Tools > Board Manager > Search "ESP32" > Install 3.3.2+
   ```

2. **Install Required Libraries**
   ```
   Sketch > Include Library > Manage Libraries
   - Adafruit NeoPixel (latest)
   - ArduinoJson 6.x (NOT v7)
   ```

3. **Download Project Files**
   - ESP32-Diagnostic.ino
   - api_handlers.h
   - test_functions.h
   - translations.h
   - web_interface.h

4. **Configure WiFi**
   ```cpp
   const char* ssid = "YourSSID";
   const char* password = "YourPassword";
   ```

5. **Compile and Upload**
   - Select your board (ESP32S3 Dev Module)
   - Configure settings (Flash: 8MB, PSRAM: OPI if available)
   - Upload ✓

6. **Access Interface**
   - Open browser: `http://esp32-diag.local`
   - Or use IP address shown in Serial Monitor

---

## 📊 Interface Screenshots

### Overview Tab
- System information (Chip, Cores, Frequency, Flash, PSRAM)
- Memory status with usage percentage
- WiFi status and signal quality
- Automatic 5-second updates

### Tests Tab
- Complete GPIO test table
- I2C device scan results
- SPI bus configuration
- Detailed memory analysis with fragmentation
- Advanced WiFi diagnostics
- System information with reset reason

### Exports Tab
- One-click JSON export
- One-click CSV export
- All diagnostic data included

---

## 🔧 Configuration

### NeoPixel (Optional)
```cpp
#define NEOPIXEL_PIN 48        // Data pin
#define NEOPIXEL_COUNT 1       // Number of LEDs
```

### mDNS Name
```cpp
const char* mdnsName = "esp32-diag";  // Access via http://esp32-diag.local
```

### Update Interval
```cpp
const unsigned long UPDATE_INTERVAL = 5000; // 5 seconds (in web_interface.h)
```

---

## 🌐 API Endpoints

### GET Endpoints
- `/` - Main web interface
- `/api/system` - System information (JSON)
- `/api/tests` - All hardware tests (JSON)
- `/api/language` - Current language
- `/api/export/json` - Download JSON export
- `/api/export/csv` - Download CSV export

### POST Endpoints
- `/api/language` - Change language (body: `{"language": "fr"}`)
- `/api/neopixel/pattern` - Set pattern (body: `{"pattern": "rainbow"}`)
- `/api/neopixel/color` - Set color (body: `{"r": 255, "g": 0, "b": 0}`)

---

## 🆕 What's New in v3.0.1

### ⚡ Arduino Core Upgrade
- **Arduino Core ESP32 3.1.3 → 3.3.2**
- Improved memory management with heap_caps API
- Enhanced WiFi stability with auto-reconnect
- OPI PSRAM support for ESP32-S3

### 📊 New Metrics (12 total)
**Memory:**
- Minimum free heap since boot
- Largest free memory block
- Memory fragmentation percentage

**WiFi:**
- WiFi channel used
- Gateway IP address
- DNS server
- Subnet mask
- Signal quality assessment

**System:**
- Last reset reason (Power-on, Watchdog, etc.)
- ESP-IDF version
- Flash chip speed
- Overall system health status

### 🔧 Optimizations
- WiFi auto-reconnect enabled by default
- No flash writes for WiFi credentials (persistent=false)
- Improved JSON buffer management
- Better error handling

---

## 📖 Documentation

- [Installation Guide](INSTALLATION.md) - Complete step-by-step guide
- [Changelog](CHANGELOG.md) - Version history
- [🇫🇷 Guide d'installation français](INSTALLATION.md)
- [🇫🇷 Changelog français](CHANGELOG.md)

---

## 🛠️ Hardware Support

### Tested On
- ✅ ESP32-S3 (8MB Flash, OPI PSRAM) - Fully tested
- ✅ ESP32-S2 - Compatible
- ✅ ESP32-C3 - Compatible
- ✅ ESP32-C6 - Compatible (Core 3.3.2+)
- ✅ ESP32-H2 - Compatible (Core 3.3.2+)

### Requirements
- **Minimum**: ESP32 with 4MB Flash
- **Recommended**: ESP32-S3 with 8MB Flash + PSRAM
- **Arduino Core**: 3.3.2 or higher
- **ArduinoJson**: 6.x (not 7.x)

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

### Development Setup
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## 📝 License

This project is licensed under the MIT License - see the LICENSE file for details.

---

## 🙏 Acknowledgments

- Espressif Systems for ESP32 platform
- Adafruit for NeoPixel library
- Arduino community

---

## 📞 Support

- **Issues**: Report bugs or request features via GitHub Issues
- **Discussions**: Join discussions for questions and ideas
- **Documentation**: Check INSTALLATION.md for detailed help

---

## 🗺️ Roadmap

### v3.1.0 (Planned)
- Real-time memory charts
- Metrics history
- Configurable alerts
- Sensor support (temperature, humidity)
- Dark/Light mode toggle

### v3.2.0 (Planned)
- Customizable dashboard
- Third-party sensor plugins
- Scheduled automatic exports
- Push notifications

### v4.0.0 (Future)
- Complete React rewrite
- Dedicated mobile app
- Optional cloud sync
- Multi-ESP32 monitoring

---

**Current version**: 3.0.1
**Arduino Core**: 3.3.2+
**Last update**: October 2025
**Available languages**: French (default), English

🌐 **Access**: http://esp32-diag.local
🇫🇷🇬🇧 **Language switching**: FR/EN buttons in interface

---

<div align="center">

Made with ❤️ for the ESP32 community

[⬆ Back to top](#esp32-complete-diagnostic-v301---multilingual)

</div>
