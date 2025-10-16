# ESP32 Complete Diagnostic v3.1.0 - Full Interface

<div align="center">

[![fr](https://img.shields.io/badge/lang-fr-blue.svg)](README.md)
[![en](https://img.shields.io/badge/lang-en-red.svg)](README.en.md)

![ESP32](https://img.shields.io/badge/ESP32-Compatible-green)
![Version](https://img.shields.io/badge/version-3.1.0-blue)
![Core](https://img.shields.io/badge/Arduino%20Core-3.3.2+-orange)
![License](https://img.shields.io/badge/license-MIT-orange)

**[🇫🇷 Lire en Français](README.md)**

</div>

---

## 📋 Description

Complete **multilingual** diagnostic tool for ESP32 microcontrollers with modern web interface. **8 fully functional pages** including LED control, screen testing, advanced hardware tests, WiFi scanner, performance benchmarks, and multi-format exports.

**New in v3.1.0**: Complete integration of **ALL v2.5 features** into the modern v3.0.1 architecture with **8 fully functional web pages**.

---

## ✨ Features

### 🌐 Complete Interface - 8 Pages

#### 1️⃣ **Overview** 📊
- Real-time system information
- Heap/PSRAM memory status
- WiFi connection details
- Auto-update every 5 seconds

#### 2️⃣ **LEDs** 💡 ✨ NEW
**Built-in LED (GPIO 97)**
- On/Off control
- Blink pattern (5x)
- Quick test
- PWM fade effect

**NeoPixel (GPIO 48)**
- Patterns: Rainbow, Pulse, Strobe
- Custom RGB color selector
- Real-time control

#### 3️⃣ **Screens** 🖥️ ✨ NEW
**TFT 320x240 SPI**
- Configuration: CS:14, DC:47, RST:21
- Test buttons: Complete, Colors, Pattern, Clear
- Ready for hardware connection

**OLED 0.96" I2C**
- Auto-detection 0x3C/0x3D
- Configurable SDA/SCL pins
- Complete test + custom message

#### 4️⃣ **Advanced Tests** 🧪 ✨ NEW
- **ADC Test** - 10 channels (GPIO 1-10)
- **Touch Pads Test** - 14 pads (ESP32-S3)
- **PWM Test** - 4 channels, 5kHz
- **SPI Bus** - SPI2, SPI3 detection
- **Flash Partitions** - Complete list
- **Memory Stress Test** - Intensive RAM test

#### 5️⃣ **GPIO** 📌 ✨ NEW
- Complete test of **26 GPIO pins**
- Input/Output verification
- HIGH/LOW testing
- Detailed status table

#### 6️⃣ **WiFi Scanner** 📶 ✨ NEW
- Scan available networks
- Display: SSID, Signal, Channel, Quality, Security
- Sort by signal strength
- Signal quality assessment

#### 7️⃣ **Performance** ⚡ ✨ NEW
**Benchmarks:**
- CPU Benchmark (1M operations, MFLOPS)
- Memory Benchmark (10KB, MB/s)
- Flash Benchmark (read/write speed)

#### 8️⃣ **Export** 💾 (Enhanced)
- **TXT Format** ✨ NEW - Structured text report
- **JSON Format** - Structured data
- **CSV Format** - Excel compatible
- **Printable PDF** ✨ NEW - Browser print function

### 🌐 Multilingual Interface
- **French** (default)
- **English**
- Dynamic FR/EN button switching
- Complete UI element translations

### 📡 Multi-Network Connectivity
- **Multi-WiFi Support** - Multiple WiFi networks
- **Automatic Fallback** - Tries each network
- **Flexible** - Home, office, on the go
- **AP Mode Fallback** - Access Point on failure

### 🎨 Modern Design
- **Glassmorphism** with transparency and blur
- **Animated gradients** purple/blue
- **Smooth animations** on all elements
- **Responsive interface** mobile/tablet/desktop

---

## 🚀 Quick Start

### Prerequisites
- **ESP32** (ESP32-S2, ESP32-S3, or ESP32-C3)
- **Arduino IDE 2.x** or PlatformIO
- **Arduino Core ESP32 v3.3.2+** ⚡ (minimum required)
- Modern web browser

### Installation

1. **Install ESP32 Board Manager v3.3.2+**
2. **Install libraries** (Adafruit NeoPixel, ArduinoJson 6.x)
3. **Create config.h** from exemple-config.h
4. **Configure WiFi** (Multi-WiFi recommended)
5. **Compile and upload**
6. **Access**: http://esp32-diagnostic.local

📖 **[Complete Installation Guide](INSTALLATION.en.md)**

---

## 📊 Version Comparison

| Feature | v2.5 | v3.0.1 | v3.1.0 |
|---------|------|--------|--------|
| Interface pages | 8 | 3 | **8** ✅ |
| Built-in LED | ✅ | ❌ | **✅** |
| NeoPixel | ✅ | ✅ | **✅** |
| TFT/OLED screens | ✅ | ❌ | **✅** |
| ADC/Touch/PWM tests | ✅ | ❌ | **✅** |
| WiFi Scanner | ✅ | ❌ | **✅** |
| Benchmarks | ✅ | ❌ | **✅** |
| TXT Export | ✅ | ❌ | **✅** |
| Multi-WiFi | ❌ | ✅ | **✅** |
| Arduino Core | 3.1.3 | 3.3.2 | **3.3.2** |
| Modern design | ⚠️ | ✅ | **✅** |
| FR/EN translations | ⚠️ | ✅ | **✅** |

**Result:** v3.1.0 = **ALL** v2.5 features + v3.0.1 improvements 🎉

---

## 🆕 What's New in v3.1.0

### Complete Interface - 8 Pages
- ✨ **5 new pages** added
- ✨ Built-in LED control (GPIO 97)
- ✨ NeoPixel RGB control (GPIO 48)
- ✨ TFT/OLED screen testing
- ✨ ADC test (10 channels)
- ✨ Touch Pads test (14 pads)
- ✨ PWM test (4 channels)
- ✨ WiFi network scanner
- ✨ Performance benchmarks
- ✨ TXT export format added

---

## 🌐 API Endpoints

### System
- `GET /api/system` - System information
- `GET /api/tests` - All hardware tests
- `GET /api/language` - Current language
- `POST /api/language` - Change language

### LEDs (NEW v3.1.0)
- `POST /api/led/builtin` - Control built-in LED
- `POST /api/neopixel/pattern` - NeoPixel pattern
- `POST /api/neopixel/color` - Custom RGB color

### Advanced Tests (NEW v3.1.0)
- `GET /api/test/gpio` - Complete GPIO test
- `GET /api/test/adc` - ADC 10 channels
- `GET /api/test/touch` - Touch Pads test
- `GET /api/test/pwm` - PWM test

### Scanner & Benchmark (NEW v3.1.0)
- `GET /api/scan/wifi` - WiFi scanner
- `GET /api/benchmark` - Performance benchmarks

### Export
- `GET /api/export/txt` - Text export (NEW)
- `GET /api/export/json` - JSON export
- `GET /api/export/csv` - CSV export

---

## 🔧 Configuration

### GPIO Pins
```cpp
#define BUILTIN_LED_PIN 97    // Built-in LED
#define NEOPIXEL_PIN 48       // NeoPixel
#define TFT_CS 14             // TFT
#define OLED_SDA 21           // OLED
```

### Update Interval
```cpp
const unsigned long UPDATE_INTERVAL = 5000; // 5 seconds
```

---

## 🛠️ Hardware Support

### Tested & Validated
- ✅ ESP32-S3 (8MB Flash, OPI PSRAM) - Excellent
- ✅ ESP32-S2 (4MB Flash) - Good

### Compatible Untested
- ⚠️ ESP32-C3, C6, H2

### Recommended Memory Configuration
- **Minimum**: Flash 4MB, Free Heap 100KB+
- **Recommended**: Flash 8MB, PSRAM 8MB (OPI)

---

## 📖 Documentation

- **[Installation Guide](INSTALLATION.en.md)** - Complete step-by-step
- **[Changelog](CHANGELOG.en.md)** - Version history
- **[Project Structure](PROJECT_STRUCTURE.en.md)** - Architecture view
- **[🇫🇷 Guide d'Installation](INSTALLATION.md)**
- **[🇫🇷 Historique Versions](CHANGELOG.md)**
- **[🇫🇷 Structure Projet](PROJECT_STRUCTURE.md)**

---

## 🔍 Troubleshooting

### ESP32 won't connect to WiFi
✅ Check credentials in `config.h`  
✅ WiFi 2.4GHz only (not 5GHz)  
✅ AP mode enabled automatically on failure  

### Interface doesn't load
✅ Clear browser cache (Ctrl+Shift+R)  
✅ Check IP address in Serial Monitor  
✅ Try http://192.168.x.x instead of .local  

### LEDs don't work
✅ Check GPIO configuration  
✅ NeoPixel needs sufficient power  
✅ Built-in LED on GPIO 97 (ESP32-S3)  

---

## 🤝 Contributing

Contributions are welcome! Feel free to submit a Pull Request.

---

## 📄 License

MIT License - Free use

---

## 🗺️ Roadmap

### v3.2.0 (Planned - Q1 2026)
- Real-time charts Chart.js
- Metrics history
- Configurable alerts
- I2C sensor support

### v4.0.0 (Future - Q3 2026)
- React rewrite
- Mobile app
- Optional cloud sync

---

## 📊 Project Statistics

**Version:** 3.1.0  
**Date:** October 16, 2025  
**Lines of Code:** ~4500  
**Files:** 6  
**Interface Pages:** 8  
**API Endpoints:** 18  
**Languages:** 2 (FR/EN)  

---

## ✅ Installation Checklist

- [ ] Arduino Core ESP32 3.3.2+ installed
- [ ] Libraries installed
- [ ] config.h created and configured
- [ ] Compilation without errors
- [ ] Interface accessible
- [ ] 8 tabs functional

---

<div align="center">

**Current version:** 3.1.0  
**Arduino Core:** 3.3.2+  
**Last update:** October 16, 2025  

🌐 **Access:** http://esp32-diagnostic.local  
🇫🇷🇬🇧 **Languages:** FR/EN buttons in interface

---

Made with ❤️ for the ESP32 community

**[🇫🇷 Lire en Français](README.md)**

</div>