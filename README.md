<!-- 
Available in multiple languages:
- English: README.md
- Français: README.fr.md
-->
# ESP32 Complete Diagnostic v3.0.0

[🇫🇷 Version française](README.fr.md) | 🇬🇧 English Version

> **Note:** This is the English version. Pour la version française, [cliquez ici](README.fr.md).

---

## 📋 Description

Comprehensive **real-time dynamic** diagnostic tool for ESP32 microcontrollers, accessible via web interface. Automatically tests all hardware components, analyzes memory, scans peripherals and generates detailed reports **without page reloads**.

**What's new in v3.0.0**: **Fully dynamic interface** with **real-time updates** every 5 seconds and **instant language switching** (FR/EN) without page reload.

## ✨ Features

### 🔄 Dynamic Real-Time Interface (NEW v3.0.0)
- **No page reloads** - Everything updates dynamically via REST API
- **Auto-refresh every 5 seconds** - Live monitoring of uptime, temperature, memory
- **Instant language switching** - FR/EN toggle without reload
- **Connection status indicator** - Visual online/offline feedback (pulsing dot)
- **Update notifications** - Floating badge shows when data refreshes
- **Smooth animations** - FadeIn effects, hover interactions, transitions
- **Modern UI/UX** - Card-based design with depth and shadows

### 🌐 Multilingual Interface (v2.4+)
- **French** (default language)
- **English**
- **Dynamic switching**: FR/EN buttons in interface
- **Real-time update**: No reload required
- **Complete translations**: All texts, labels, messages and status

### Hardware Tests
- **Built-in LED** - Configuration and testing with patterns (blink, fade, on/off)
- **NeoPixel/WS2812B** - Multi-LED support with RGB effects (rainbow, fade, custom colors)
- **TFT 320x240 Screen** - Color, shape, text testing
- **OLED 0.96" I2C Screen** - 10 display tests including animations
- **GPIO** - Automatic testing of all available GPIO
- **ADC** - Reading all analog channels
- **Touch Pads** - Capacitive touch sensor testing
- **PWM/LEDC** - Pulse width modulation testing

### System Analysis
- **Detailed Memory** - Flash, PSRAM (OPI/QSPI), SRAM with integrity tests
- **WiFi Scanner** - Network scan with RSSI, channel, encryption
- **I2C Scan** - Automatic peripheral detection (0x01-0x7F)
- **SPI Scan** - Available SPI bus information
- **Flash Partitions** - Complete partition list
- **Benchmarks** - CPU and memory performance
- **Stress Test** - Memory allocation limit testing

### Web Interface
- **8 tabs** - Intuitive navigation (Overview, LEDs, Screens, Advanced Tests, GPIO, WiFi, Performance, Export)
- **Language selector** - FR/EN buttons in top right
- **Dynamic configuration** - OLED I2C pins modifiable via web
- **Real-time** - Data refresh without reload (every 5s)
- **Responsive** - Mobile/tablet/desktop compatible
- **Complete exports** - TXT, JSON, CSV, printable PDF version

### Network Access
- **mDNS** - Access via http://ESP32-Diagnostic.local
- **Multi-WiFi** - Automatic connection with failover
- **REST API** - JSON endpoints for integration and real-time updates

## 🎯 Compatible Boards

Tested and optimized for:
- ✅ **ESP32** (original)
- ✅ **ESP32-S2**
- ✅ **ESP32-S3** (recommended - with OPI PSRAM)
- ✅ **ESP32-C3**
- ⚠️ **ESP32-C6, ESP32-H2** (not tested but should work)

## 📦 Requirements

### Hardware
- ESP32 board (any model)
- USB cable
- Optional: NeoPixel LED, TFT screen, OLED screen

### Software
- **Arduino IDE** 2.x or higher
- **ESP32 Arduino Core** 3.1.3 or higher

### Required Arduino Libraries

**Mandatory (via Library Manager):**
```
1. Adafruit BusIO
2. Adafruit GFX Library
3. Adafruit SSD1306
4. Adafruit NeoPixel
5. TFT_eSPI
```

**Included with ESP32 Core:**
```
WiFi, WebServer, ESPmDNS, Wire, SPI
esp_chip_info, esp_mac, esp_flash, esp_heap_caps, esp_partition
```

## 🚀 Installation

### 1. Download

Download or clone this project to your Arduino folder.

### 2. Required Files

**Project structure:**
```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino              (main file)
├── api_handlers.h                    (REST API handlers)
├── web_interface.h                   (dynamic web interface)
├── languages.h                       (FR/EN translations)
├── config.h.example                  (configuration template)
├── config.h                          (your credentials - to create)
└── README.md
```

### 3. WiFi Configuration

**IMPORTANT:** Create a `config.h` file in the same folder as the `.ino`

**Option A - Rename the example file:**
Rename the file `example-config.h` to `config.h`
Edit the file and add your WiFi network(s) by filling in the values

```cpp
const char* WIFI_SSID_1 = "SSID1";
const char* WIFI_PASS_1 = "Password1";

const char* WIFI_SSID_2 = "SSID2";
const char* WIFI_PASS_2 = "Password2";

// Add as many networks as needed
```

**Option B - Create `config.h` manually:**
```cpp
#ifndef CONFIG_H
#define CONFIG_H

// ========== WIFI CONFIGURATION ==========
const char* WIFI_SSID_1 = "YourSSID";
const char* WIFI_PASS_1 = "YourPassword";

// Add as many networks as needed
// const char* WIFI_SSID_2 = "SSID2";
// const char* WIFI_PASS_2 = "Password2";

#endif
```

### 4. Copy Required Files

Copy the provided files to the same folder as the `.ino`:
- `languages.h` (translations)
- `api_handlers.h` (API endpoints)
- `web_interface.h` (dynamic interface)

### 5. Hardware Configuration

**PSRAM (if available):**
- ESP32-S3: `Tools → PSRAM → OPI PSRAM`
- ESP32: `Tools → PSRAM → Enabled`

**Flash Size:**
```
Tools → Flash Size → [Your board's actual size]
```

### 6. Compile and Upload

```
Sketch → Upload
```

### 7. Accessing the Interface

- **http://ESP32-Diagnostic.local** (mDNS)
- **http://[DISPLAYED_IP]** (direct IP)

## 🌐 Dynamic Features Usage

### Real-Time Updates

The interface automatically refreshes every 5 seconds:
- ✅ **Uptime** - Updated continuously
- ✅ **Temperature** - Live CPU temperature (if supported)
- ✅ **Memory** - SRAM/PSRAM usage with animated progress bars
- ✅ **Fragmentation** - Memory fragmentation percentage

**Connection Status:**
- 🟢 **Green pulsing dot** - Connected and updating
- 🔴 **Red pulsing dot** - Connection lost

### Language Switching

**In the web interface:**
1. Click the **FR** or **EN** button in the top right
2. The interface updates **instantly** (< 100ms)
3. No reload required
4. All tabs and content refresh in the selected language

**Default:** French interface

**Via API:**
```bash
# Switch to English
curl http://ESP32-Diagnostic.local/api/set-language?lang=en

# Switch to French
curl http://ESP32-Diagnostic.local/api/set-language?lang=fr

# Get current translations (with cache-busting)
curl http://ESP32-Diagnostic.local/api/get-translations?lang=en&t=1234567890
```

### Available Tabs

1. **Overview** - Complete system information with real-time updates
2. **LEDs** - Built-in LED and NeoPixel tests with instant control
3. **Screens** - TFT and OLED tests
4. **Advanced Tests** - ADC, Touch, PWM, SPI, Partitions, Stress Test
5. **GPIO** - Automatic all GPIO testing
6. **WiFi** - WiFi network scanner
7. **Performance** - CPU/Memory benchmarks
8. **Export** - TXT, JSON, CSV, Printable version

## 📊 REST API

**New endpoints in v3.0.0:**
```
GET /api/status                      → Real-time system status (auto-refresh)
GET /api/overview                    → Complete system overview
GET /api/leds-info                   → LED and NeoPixel status
GET /api/screens-info                → TFT and OLED information
GET /api/set-language?lang=fr        → Change language (fr/en)
GET /api/get-translations?lang=XX    → Get translations JSON (cache-busted)
```

**Existing routes:**
```
GET /                                → Dynamic web interface
GET /api/test-gpio                   → GPIO test
GET /api/wifi-scan                   → WiFi scanner
GET /api/builtin-led-test            → Test built-in LED
GET /api/builtin-led-control?action  → Control LED (on/off/blink/fade)
GET /api/neopixel-test               → Test NeoPixel
GET /api/neopixel-pattern?pattern    → NeoPixel patterns
GET /api/neopixel-color?r&g&b        → Custom NeoPixel color
GET /api/benchmark                   → Performance benchmarks
GET /export/txt                      → TXT export
GET /export/json                     → JSON export
GET /export/csv                      → CSV export
```

## 🛠️ Customization

### Change Default Language

In `languages.h`:
```cpp
Language currentLanguage = LANG_EN; // For English by default
```

### Add a New Language

1. Define the enum in `languages.h`:
```cpp
enum Language {
  LANG_FR = 0,
  LANG_EN = 1,
  LANG_ES = 2  // Spanish
};
```

2. Create the translation structure:
```cpp
const Translations LANG_ES_TEXTS = {
  "Diagnóstico ESP32 Completo",
  "v",
  // ... all translations
};
```

3. Modify the `T()` function to include the new language.

## 🔧 Pin Configuration

### Default Pins

**I2C (OLED) - Modifiable via web:**
| Model    | SDA | SCL |
|----------|-----|-----|
| ESP32    | 21  | 22  |
| ESP32-S2 | 8   | 9   |
| ESP32-S3 | 8   | 9   |
| ESP32-C3 | 5   | 6   |

**TFT SPI (fixed):**
```
MOSI: 45, SCLK: 3, CS: 14, DC: 47, RST: 21, MISO: 46
```

**Built-in LED (auto-detected):**
- ESP32-S3: GPIO 2
- ESP32-C3: GPIO 8
- ESP32: GPIO 2

**NeoPixel (auto-detected):**
- ESP32-S3: GPIO 48
- ESP32-C3: GPIO 8

## 🛠️ Troubleshooting

### Dynamic Interface Issues

**Interface not updating:**
1. Check browser console for errors (F12)
2. Verify ESP32 is connected to WiFi
3. Check connection indicator (should be green and pulsing)
4. Try hard refresh (Ctrl+F5)

**Language switching doesn't work:**
1. Verify `languages.h` is in the folder
2. Clear browser cache completely
3. Check browser console for API errors
4. Ensure JavaScript is enabled

**API calls failing:**
1. Check Serial Monitor for ESP32 status
2. Verify IP address is correct
3. Test direct API endpoint: `http://[IP]/api/status`
4. Check WiFi connection strength

### Memory Issues

**Low memory warnings:**
- Normal with many features active
- PSRAM helps significantly if available
- Reduce LED count in NeoPixel configuration
- Disable unused features

## 📈 Performance

- **Startup time**: ~5s
- **Initial page load**: ~200ms (60% faster than v2.x)
- **Language switching**: <100ms (no reload)
- **Auto-refresh interval**: 5000ms (configurable)
- **API response time**: <50ms per endpoint
- **Memory heap used**: ~250KB
- **Multilingual overhead**: ~15KB Flash
- **JavaScript footprint**: ~30KB

## 🔒 Security

⚠️ **Local/development use only.**

**Security considerations:**
- No authentication implemented
- No HTTPS support
- Plain text WiFi credentials in config.h
- All API endpoints are open

**Never expose this interface to the public internet.**

**Never share `config.h` with your WiFi credentials.**

## 📁 Project Structure v3.0.0

```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino              (main code + setup)
├── api_handlers.h                    (REST API endpoints - NEW)
├── web_interface.h                   (dynamic interface - NEW)
├── languages.h                       (translation system FR/EN)
├── config.h.example                  (template)
├── config.h                          (your credentials - gitignore)
├── README.md                         (this file - English)
├── README.fr.md                      (French version)
├── CHANGELOG.md                      (version history)
└── .gitignore                        (excludes config.h)
```

## 📄 Changelog

### v3.0.0 (2025-01-16) - DYNAMIC INTERFACE
- ✨ **Fully dynamic interface** without page reloads
- ✨ **Real-time updates** every 5 seconds
- ✨ **Instant language switching** (FR/EN)
- ✨ **REST API architecture** complete
- ✨ **Connection status indicator** with visual feedback
- ✨ **Smooth animations** and modern UI/UX
- ✨ All v2.5 features preserved

### v2.5 (2025-01-10) - MULTILINGUAL EXPORTS
- ✨ **Exported files translated** (FR/EN)

### v2.4 (2025-01-09) - MULTILINGUAL
- ✨ **Complete multilingual system** (FR/EN)
- ✨ New `languages.h` file with translations
- ✨ FR/EN buttons in interface
- ✨ REST API for language switching

### v2.3 (2025-01-08)
- OLED 0.96" test with 10 animations
- Dynamic I2C pin configuration

### v2.2 (2025-01-07)
- TFT SPI screen support

### v2.1 (2025-01-06)
- NeoPixel/WS2812B support

### v2.0 (2025-01-05)
- Complete diagnostic system rewrite

## 📝 License

This project is free to use, modify and distribute under MIT License.

## 🤝 Contributing

**To add a language:**
1. Fork the project
2. Add your language in `languages.h`
3. Test all screens and features
4. Pull Request with screenshots

**To report bugs or suggest features:**
- Open an issue on GitHub
- Include ESP32 model and Arduino Core version
- Provide Serial Monitor output if applicable

## 🎓 Credits

Developed for the ESP32 community.

**v3.0.0 - Dynamic real-time interface**
**v2.5 - Multilingual exports**
**v2.4 - Multilingual system**
**v2.3 - Comprehensive base**

---

## 🚀 Quick Start Guide

1. **Install Arduino IDE 2.x** and ESP32 Core 3.1.3+
2. **Install required libraries** via Library Manager
3. **Download project** and create `config.h` with your WiFi credentials
4. **Copy all .h files** to the sketch folder
5. **Configure PSRAM** if your board supports it
6. **Upload to ESP32**
7. **Open Serial Monitor** to get IP address
8. **Access** http://ESP32-Diagnostic.local or http://[IP]
9. **Enjoy real-time monitoring!**

## 📞 Support

- **Documentation**: See code comments and this README
- **Issues**: GitHub Issues
- **Email**: Check repository for contact information

---

**Current version**: 3.0.0 Dynamic Real-Time
**Last update**: January 16, 2025
**Available languages**: French (default), English  
**Support**: ESP32 Arduino Core 3.1.3+
**Browser requirements**: ES6+ (Chrome 51+, Firefox 54+, Safari 10+)

🌐 **Access**: http://ESP32-Diagnostic.local  
🇫🇷🇬🇧 **Language switching**: FR/EN buttons in interface (instant, no reload)
⚡ **Real-time**: Auto-refresh every 5 seconds
🔄 **Dynamic**: No page reloads required

---

## 🌟 Key Features Summary

| Feature | v2.x | v3.0.0 |
|---------|------|--------|
| Page reloads | Required | ❌ None |
| Language switch | Reload needed | ✅ Instant |
| Real-time updates | Manual refresh | ✅ Auto (5s) |
| Connection status | None | ✅ Visual indicator |
| API architecture | Partial | ✅ Complete REST |
| Performance | Baseline | ✅ 60% faster |
| Memory usage | Baseline | ✅ Optimized |
| Animation/UX | Basic | ✅ Modern |

**Upgrade to v3.0.0 for the best experience!**
