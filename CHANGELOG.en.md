# Changelog - ESP32 Diagnostic System

All notable changes to this project are documented in this file.

---

## [3.1.0] - 2025-10-16

### 🎉 Major Release - Complete 8-Page Interface

**This version integrates ALL v2.5 features into the modern v3.0.1 architecture**

### ✨ Major New Features

#### Complete Web Interface
- **8 functional pages** instead of 3
- Enhanced tab navigation
- Glassmorphism design maintained
- Responsive mobile/tablet/desktop

#### Page 2: LEDs (NEW)
- **Built-in LED (GPIO 97)**
  - On/Off control
  - Blink (5x)
  - Quick test
  - PWM fade effect
- **NeoPixel (GPIO 48)**
  - Patterns: Rainbow, Pulse, Strobe
  - Custom RGB color
  - Real-time control

#### Page 3: Screens (NEW)
- **TFT 320x240 SPI Screen**
  - Configuration CS:14, DC:47, RST:21
  - Buttons: Test, Colors, Pattern, Clear
  - Ready for hardware connection
- **OLED 0.96" I2C Screen**
  - Auto-detection 0x3C/0x3D
  - Configurable SDA/SCL pins
  - Complete test + custom message

#### Page 4: Advanced Tests (NEW)
- **ADC Test**
  - 10 channels (GPIO 1-10)
  - Display: Raw, Voltage, Percentage
- **Touch Pads Test**
  - 14 touch pads on ESP32-S3
  - Capacitance values
- **PWM Test**
  - 4 channels tested
  - 5kHz, 8-bit resolution
- **SPI Bus**
  - SPI2, SPI3 scan
- **Flash Partitions**
  - Complete list with addresses
- **Memory Stress Test**
  - Intensive RAM test

#### Page 5: GPIO (NEW)
- Complete test of **26 GPIO**
- Input/Output verification
- HIGH/LOW testing
- Detailed status table

#### Page 6: WiFi Scanner (NEW)
- Scan available networks
- SSID, Signal, Channel, Quality, Security
- Sort by signal strength
- Clear and readable interface

#### Page 7: Performance (NEW)
- **CPU Benchmark**
  - 1M operations
  - Performance in MFLOPS
- **Memory Benchmark**
  - 10KB allocation test
  - Speed in MB/s
- **Flash Benchmark**
  - Read/write test

#### Page 8: Export (ENHANCED)
- **TXT Format** (NEW)
  - Structured text report
- **JSON Format** (existing)
- **CSV Format** (existing)
- **Printable PDF Version** (NEW)
  - Browser print() function

### 🔧 New API Endpoints

#### LEDs
- `POST /api/led/builtin` - Built-in LED control
  ```json
  {"action": "on|off|blink|test|fade"}
  ```

#### Advanced Tests
- `GET /api/test/gpio` - Complete GPIO test
- `GET /api/test/adc` - ADC all channels
- `GET /api/test/touch` - Touch Pads test
- `GET /api/test/pwm` - PWM test

#### Scanner & Benchmark
- `GET /api/scan/wifi` - Network scanner
- `GET /api/benchmark` - Performance benchmarks

#### Export
- `GET /api/export/txt` - Text export (NEW)

### 📊 Interface Improvements

- **Optimized navigation**
  - 8 tabs instead of 3
  - Horizontal scroll on mobile
  - Enhanced active tab indicator

- **Improved buttons**
  - Category colors (red, green, blue, purple, cyan)
  - Intuitive emoji icons
  - Click visual feedback

- **Responsive grids**
  - Export in 4 columns
  - Adaptive info grids
  - Scrollable tables on mobile

- **Smooth animations**
  - Loading spinners
  - Smooth transitions
  - Hover effects

### 🔐 Security & Configuration

- Multi-WiFi system maintained from v3.0.1
- Protected config.h file (.gitignore)
- exemple-config.h as safe template

### 📝 Documentation

- **README.md v3.1.0**
  - Complete 8-page documentation
  - Detailed installation guide
  - Complete troubleshooting
  - Version comparison

- **CHANGELOG.md**
  - Detailed history
  - Migration guide

### ⚡ Performance

- JSON buffer memory optimization
- Async API request handling
- Optimized browser cache

### 🐛 Bug Fixes

- Fix OLED status display based on I2C scan
- Fix responsive tabs overflow on mobile
- Fix countdown progress bar
- Fix NeoPixel availability detection
- Fix API handlers definition order
- Fix optimal JSON buffer sizes

### 📦 Modified Files

- `ESP32-Diagnostic.ino` → v3.1.0
- `api_handlers.h` → v3.1.0 (all endpoints)
- `web_interface.h` → v3.1.0 (8 complete pages)
- `test_functions.h` → v3.0.1 (unchanged)
- `translations.h` → v3.0.1 (unchanged)
- `README.md` → v3.1.0 (complete documentation)
- `CHANGELOG.md` → v3.1.0

### 🎯 v3.1.0 Highlights

✅ **100% of v2.5 features** integrated  
✅ **Modern v3.0.1 architecture** maintained  
✅ **Multi-WiFi support** kept  
✅ **Arduino Core 3.3.2** optimizations  
✅ **8 complete pages** functional  
✅ **Glassmorphism design** beautiful  
✅ **Complete FR/EN translations**  
✅ **4 export formats** (TXT, JSON, CSV, PDF)  

---

## [3.0.1] - 2025-10-16

### ⚡ Major Improvements

#### Arduino Core Upgrade
- **Arduino Core ESP32 3.1.3 → 3.3.2**
- Support for latest Espressif optimizations
- Improved WiFi stability
- Enhanced heap capabilities API

#### Multi-WiFi Support ✨ NEW
- **WiFiMulti integrated**: Support for multiple WiFi networks
- **Automatic fallback**: ESP32 tries each network in order
- **Flexible**: Works at home, office, on the go
- **Simple configuration**: Add as many networks as needed
- **Fallback AP**: Access Point mode if no network available

Multi-WiFi configuration example:
```cpp
#define MULTI_WIFI_ENABLED
#define NUM_SSIDS 2

const char* ssid_list[] = {"Home", "Office"};
const char* password_list[] = {"pass1", "pass2"};
```

#### Secure Configuration System
- **exemple-config.h**: Public template (safe for Git)
- **config.h**: Your private configuration (ignored by Git)
- **.gitignore**: Automatic credential protection
- **Complete documentation**: Security guide included

#### Memory Optimizations (Core 3.3.2)
- Use of `heap_caps_get_free_size()` for better accuracy
- Added `heap_caps_get_minimum_free_size()` for minimum memory tracking
- Added `heap_caps_get_largest_free_block()` for fragmentation detection
- Memory fragmentation analysis in percentage
- Improved OPI PSRAM support for ESP32-S3

#### WiFi Improvements
- Optimized WiFi configuration with `WiFi.persistent(false)`
- Auto-reconnect enabled by default
- Extended network information (Gateway, DNS, Subnet Mask)
- WiFi channel information
- Better signal quality detection

#### Enriched System Tests
- Last reset reason (Power-on, Software, Watchdog, etc.)
- IDF version (ESP-IDF framework)
- Chip revision
- Flash memory speed
- Global system status (HEALTHY/WARNING/CRITICAL)

### 🔧 Technical Modifications

#### Modified Files
- `ESP32-Diagnostic.ino`: Version 3.0.1, Core 3.3.2 optimizations
- `api_handlers.h`: Optimized JSON buffers, new memory APIs
- `test_functions.h`: Advanced memory tests, complete WiFi analysis
- `translations.h`: No functional changes
- `web_interface.h`: Display of new metrics
- `INSTALLATION.md`: Updated Core 3.3.2 requirements

#### Compatibility
- **Minimum required**: Arduino Core ESP32 3.3.2+
- **Tested on**: ESP32-S3 with 8MB Flash, OPI PSRAM
- **Compatible**: ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6, ESP32-H2

### 📊 New Displayed Metrics

#### Memory
- Minimum free heap (since startup)
- Largest free block
- Fragmentation percentage
- Minimum free PSRAM (if available)

#### WiFi
- WiFi channel used
- Gateway address
- DNS server
- Subnet mask
- Signal quality (Excellent/Good/Fair/Weak)

#### System
- Last reset reason
- ESP-IDF version
- Chip revision
- Flash frequency
- Global system status

### 🛠 Bug Fixes

- Improved memory management in API handlers
- Better stability with dynamic JSON buffers
- Optimized HTTP responses for NeoPixel

### 📚 Documentation

- Installation guide updated with Core 3.3.2
- OPI PSRAM configuration instructions for ESP32-S3
- Completed installation checklist
- Enhanced troubleshooting section

---

## [3.0.0] - 2025-10-10

### 🎉 Initial Release

#### Main Features
- Dynamic web interface with real-time updates
- Modern glassmorphism design
- Tab navigation
- Integrated FR/EN translation system
- JSON and CSV export
- Smooth animations and transitions

#### Architecture
- Modular 5-file structure
- Complete REST API
- Integrated translation system
- Optimized WebServer

#### Hardware Support
- ESP32-S2, ESP32-S3, ESP32-C3
- Arduino Core ESP32 3.1.3+
- Optional PSRAM

---

## 🔄 Migration Between Versions

### From v3.0.1 to v3.1.0

**✅ Compatible** - No breaking changes

**Steps:**
1. Replace all files with v3.1.0 ones
2. Keep your existing `config.h`
3. Compile and upload
4. Enjoy the 8 pages! 🎉

**Visible improvements:**
- 5 new tabs in interface
- Operational LED controls
- Functional advanced tests
- Active WiFi scanner
- Available benchmarks
- Added TXT export

### From v2.5 to v3.1.0

**⚠️ Important Changes**

**Steps:**
1. Update Arduino Core → 3.3.2+
2. Create `config.h` from `exemple-config.h`
3. Configure Multi-WiFi (recommended)
4. Replace all files
5. Compile and upload

**Improvements:**
- Modular architecture
- Multi-WiFi support
- Modern design
- FR/EN translations
- Advanced metrics
- Configuration security

### From v3.0.0 to v3.1.0

**✅ Compatible** - Simple migration

**Steps:**
1. Update Core → 3.3.2
2. Create `config.h` (new)
3. Replace files
4. Compile

---

## 📈 Version Statistics

| Version | Lines of Code | Files | UI Pages | API Endpoints |
|---------|---------------|-------|----------|---------------|
| v2.5    | ~3000         | 5     | 8        | 15            |
| v3.0.0  | ~2500         | 5     | 3        | 8             |
| v3.0.1  | ~2800         | 6     | 3        | 10            |
| **v3.1.0** | **~4500**  | **6** | **8**    | **18**        |

---

## 🗺️ Future Roadmap

### v3.2.0 (Planned - Q1 2026)
- [ ] Real-time charts with Chart.js
- [ ] Metrics history (SPIFFS storage)
- [ ] Configurable alerts (memory thresholds, etc.)
- [ ] I2C sensor support (BME280, DHT22)
- [ ] Dark/light mode toggle
- [ ] WebSocket API for reduced latency
- [ ] WiFi configuration via web interface

### v3.3.0 (Planned - Q2 2026)
- [ ] Customizable dashboard (drag & drop)
- [ ] Third-party sensor plugins
- [ ] Scheduled automatic export (CRON-like)
- [ ] Push notifications
- [ ] System logs with rotation
- [ ] Power saving mode

### v4.0.0 (Future - Q3 2026)
- [ ] Complete React/Vue.js rewrite
- [ ] Dedicated mobile app (Flutter)
- [ ] Optional cloud sync (MQTT/HTTP)
- [ ] Multi-ESP32 network monitoring
- [ ] OTA Update via interface
- [ ] SQLite database for history
- [ ] Documented REST API (OpenAPI/Swagger)

---

## 🏆 Contributors

### v3.1.0
- **Complete architecture**: 8-page integration
- **Advanced tests**: ADC, Touch, PWM, GPIO
- **WiFi Scanner**: Complete network scan
- **Benchmarks**: CPU/Memory/Flash performance

### v3.0.1
- **Multi-WiFi**: Multiple network support
- **Core 3.3.2**: Migration and optimizations
- **Security**: Protected configuration system

### v3.0.0
- **Modern design**: Glassmorphism UI
- **Architecture**: Modular structure
- **Translations**: FR/EN support

---

## 📝 Important Notes

### Hardware Compatibility

**✅ Tested and validated:**
- ESP32-S3 (8MB Flash, OPI PSRAM) - Excellent
- ESP32-S3 (4MB Flash, QSPI PSRAM) - Good
- ESP32-S2 (4MB Flash) - Good

**⚠️ Compatible but untested:**
- ESP32-C3
- ESP32-C6
- ESP32-H2

**❌ Not compatible:**
- Classic ESP32 (missing Core 3.3.2 features)

### Recommended Memory Configuration

**Minimum:**
- Flash: 4MB
- PSRAM: Optional
- Free Heap: 100KB+

**Recommended:**
- Flash: 8MB
- PSRAM: 8MB (OPI)
- Free Heap: 200KB+

**Optimal:**
- Flash: 16MB
- PSRAM: 8MB (OPI)
- Free Heap: 300KB+

---

## 🔗 Useful Links

- **Arduino Core 3.3.2 Documentation:** https://docs.espressif.com/
- **Adafruit NeoPixel Library:** https://github.com/adafruit/Adafruit_NeoPixel
- **ArduinoJson v6:** https://arduinojson.org/v6/

---

## 📄 License

MIT License - Free use, modification and distribution

---

## ⭐ Acknowledgments

- **Espressif Systems** for the ESP32 platform
- **Adafruit** for the NeoPixel library
- **Arduino Community** for support
- **All testers** of v3.1.0

---

**Last update:** October 16, 2025  
**Current version:** 3.1.0  
**Next version:** 3.2.0 (Q1 2026)

---

*To report a bug or suggest a feature, create an issue on the GitHub repository.*

**[🇫🇷 Version Française](CHANGELOG.md)**