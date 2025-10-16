**📚 Related Documents:**
- [README.md](README.md) - Main documentation (French)
- [README.en.md](README.en.md) - Main documentation (English)
- [CHANGELOG.md](CHANGELOG.md) - Version history (French)
- [CHANGELOG.en.md](CHANGELOG.en.md) - Version history (English)
- [INSTALLATION.md](INSTALLATION.md) - Installation guide (French)
- [INSTALLATION.en.md](INSTALLATION.en.md) - Installation guide (English)
- [PROJECT_STRUCTURE.# 📁 PROJECT STRUCTURE v3.1.0 - Complete Overview

## 🗂️ Project Tree

```
ESP32-Diagnostic/
│
├── 📄 ESP32-Diagnostic.ino          # Main Arduino file
├── 📄 exemple-config.h               # Configuration template (GIT)
├── 📄 config.h                       # Your configuration (GITIGNORE)
├── 📄 api_handlers.h                 # REST API handlers
├── 📄 web_interface.h                # HTML/CSS/JS web interface
├── 📄 test_functions.h               # Hardware test functions
├── 📄 translations.h                 # Translation system
│
├── 📄 .gitignore                     # Git exclusions
├── 📄 README.md                      # Main documentation (French)
├── 📄 README.en.md                   # Main documentation (English)
├── 📄 CHANGELOG.md                   # Version history (French)
├── 📄 CHANGELOG.en.md                # Version history (English)
├── 📄 INSTALLATION.md                # Installation guide (French)
├── 📄 INSTALLATION.en.md             # Installation guide (English)
├── 📄 PROJECT_STRUCTURE.md           # This file (French)
└── 📄 PROJECT_STRUCTURE.en.md        # This file (English)
```

---

## 📋 File Descriptions

### 1. ESP32-Diagnostic.ino (Main)

**Version:** 3.1.0  
**Size:** ~300 lines  
**Role:** Program entry point

**Content:**
```cpp
✅ Library includes (WiFi, WebServer, Wire, etc.)
✅ Constant definitions (VERSION, GPIO pins)
✅ Include config.h (WiFi credentials)
✅ Global objects (server, wifiMulti, strip)
✅ System functions (getChipModel, getMemoryInfo, etc.)
✅ Module includes (translations, test_functions, api_handlers, web_interface)
✅ Hardware initialization (LED, NeoPixel)
✅ WiFi connection (Multi-WiFi support)
✅ Web server configuration (setupRoutes)
✅ setup() - Complete initialization
✅ loop() - Main loop
```

**Dependencies:**
- Arduino Core ESP32 3.3.2+
- WiFi.h
- WiFiMulti.h
- WebServer.h
- ESPmDNS.h
- Wire.h (I2C)
- Adafruit_NeoPixel.h
- esp_heap_caps.h
- esp_system.h
- esp_chip_info.h
- driver/adc.h

**Includes:**
- config.h
- translations.h
- test_functions.h
- api_handlers.h
- web_interface.h

---

### 2. exemple-config.h (Template)

**Version:** 3.1.0  
**Size:** ~200 lines  
**Role:** WiFi configuration template

**Content:**
```cpp
✅ Complete documentation
✅ Installation instructions
✅ Multi-WiFi mode (recommended)
✅ Single WiFi mode (simple)
✅ Static IP configuration (optional)
✅ Advanced WiFi parameters
✅ AP mode configuration (fallback)
✅ Commented examples
```

**Characteristics:**
- ✅ No real credentials (safe for Git)
- ✅ Comments in English
- ✅ Multiple network examples
- ✅ Step-by-step guide

**Usage:**
```bash
# Copy and rename
cp exemple-config.h config.h

# Edit config.h with your credentials
nano config.h  # or your editor
```

---

### 3. config.h (Your Configuration)

**Version:** Created by user  
**Size:** ~200 lines  
**Role:** Personal WiFi configuration

**Content:**
```cpp
✅ Your WiFi credentials
✅ Multi-WiFi or Single configuration
✅ Custom parameters
✅ Static IP (optional)
```

**⚠️ IMPORTANT:**
- ❌ NEVER commit to Git
- ❌ NEVER share
- ✅ Always in .gitignore
- ✅ Keep secure backup

**Multi-WiFi Format:**
```cpp
#define MULTI_WIFI_ENABLED
#define NUM_SSIDS 2

const char* ssid_list[] = {"WiFi1", "WiFi2"};
const char* password_list[] = {"Pass1", "Pass2"};
```

---

### 4. api_handlers.h (REST API)

**Version:** 3.1.0  
**Size:** ~800 lines  
**Role:** All API endpoints

**Implemented Endpoints:**

#### System (v3.0.1)
- `handleSystemInfo()` → GET /api/system
- `handleTests()` → GET /api/tests
- `handleGetLanguage()` → GET /api/language
- `handleSetLanguage()` → POST /api/language

#### LEDs (v3.1.0 NEW)
- `handleBuiltinLED()` → POST /api/led/builtin
  - Actions: on, off, blink, test, fade
- `handleNeoPixelPattern()` → POST /api/neopixel/pattern
  - Patterns: rainbow, pulse, strobe, off
- `handleNeoPixelColor()` → POST /api/neopixel/color
  - Custom RGB: {r, g, b}

#### Advanced Tests (v3.1.0 NEW)
- `handleTestGPIO()` → GET /api/test/gpio
  - Complete 26 GPIO test
- `handleTestADC()` → GET /api/test/adc
  - 10 ADC channels reading
- `handleTestTouch()` → GET /api/test/touch
  - 14 touch pads test
- `handleTestPWM()` → GET /api/test/pwm
  - 4 PWM channels test

#### Scanner & Benchmark (v3.1.0 NEW)
- `handleWiFiScan()` → GET /api/scan/wifi
  - Scan available networks
- `handleBenchmark()` → GET /api/benchmark
  - CPU/Memory/Flash benchmarks

#### Export
- `handleExportJSON()` → GET /api/export/json
- `handleExportCSV()` → GET /api/export/csv
- `handleExportTXT()` → GET /api/export/txt (v3.1.0 NEW)

**Dependencies:**
- ArduinoJson (v6.x)
- Functions from ESP32-Diagnostic.ino
- Global objects (server, strip, etc.)

**Response Format:**
```json
// Structured JSON
{
  "status": "success",
  "data": {...}
}
```

---

### 5. web_interface.h (Web Interface)

**Version:** 3.1.0  
**Size:** ~1200 lines  
**Role:** Complete HTML/CSS/JavaScript interface

**Content:**

#### HTML Structure
```html
✅ HTML5 DOCTYPE
✅ Responsive viewport meta
✅ Title with version
✅ Inline CSS styles
✅ Glassmorphism container
✅ Header with language
✅ 8 tab navigation
✅ 8 content sections
✅ Complete JavaScript
```

#### CSS Styles
```css
✅ CSS reset
✅ Animated gradient background
✅ Glassmorphism cards
✅ Responsive design
✅ Smooth animations
✅ Hover effects
✅ Loading spinners
✅ Progress bars
✅ Colored buttons
✅ Styled tables
✅ Mobile media queries
```

#### JavaScript Functions
```javascript
✅ setLanguage(lang) - Language switching
✅ showTab(tabName) - Tab navigation
✅ loadData() - Load system data
✅ controlBuiltinLED(action) - LED control
✅ neoPattern(pattern) - NeoPixel patterns
✅ neoColor() - Custom NeoPixel color
✅ testADC() - ADC test
✅ testTouch() - Touch test
✅ testPWM() - PWM test
✅ testAllGPIO() - Complete GPIO test
✅ scanWiFi() - WiFi scanner
✅ runBenchmark() - Benchmarks
✅ exportTXT() - Text export
✅ exportJSON() - JSON export
✅ exportCSV() - CSV export
✅ startCountdown() - Auto-update countdown
```

#### The 8 Implemented Pages

1. **Overview** - System overview
2. **LEDs** - LED + NeoPixel control
3. **Screens** - TFT/OLED screen configuration
4. **Advanced** - ADC/Touch/PWM/SPI/Flash/Stress tests
5. **GPIO** - Complete 26 GPIO test
6. **WiFi** - Network scanner
7. **Performance** - Benchmarks
8. **Export** - 4 export formats

**Characteristics:**
- 📱 Responsive (mobile/tablet/desktop)
- 🌐 Multilingual (FR/EN)
- 🎨 Modern glassmorphism design
- ⚡ Real-time updates
- 🔄 Smooth animations

---

### 6. test_functions.h (Hardware Tests)

**Version:** 3.0.1  
**Size:** ~400 lines  
**Role:** Hardware test functions

**Implemented Functions:**

```cpp
✅ testGPIO(JsonArray& gpioArray)
   - I/O test on pins
   - HIGH/LOW verification
   
✅ testI2C(JsonArray& i2cArray)
   - Scan addresses 0x00-0x7F
   - Peripheral identification
   
✅ testSPI(JsonArray& spiArray)
   - SPI bus detection
   - Pin configuration
   
✅ testMemory(JsonObject& memTest)
   - Heap free/size/min
   - Fragmentation
   - PSRAM (if available)
   
✅ testWiFi(JsonObject& wifiTest)
   - Connection status
   - SSID, IP, RSSI
   - Signal quality
   - Gateway, DNS, Subnet
   
✅ testSystem(JsonObject& sysTest)
   - Chip model/revision
   - CPU cores/frequency
   - Flash size/speed
   - Uptime
   - Reset reason
   - IDF version
   - Overall status
```

**Dependencies:**
- ArduinoJson
- Wire.h (I2C)
- esp_chip_info.h
- esp_system.h
- heap_caps

**Core 3.3.2 Optimizations:**
- heap_caps_get_free_size()
- heap_caps_get_minimum_free_size()
- heap_caps_get_largest_free_block()

---

### 7. translations.h (Translations)

**Version:** 3.0.1  
**Size:** ~300 lines  
**Role:** FR/EN translation system

**Content:**

```cpp
✅ French constants (TEXT_FR_*)
✅ English constants (TEXT_EN_*)
✅ t() function - Translation helper
✅ T_* macros - Shortcuts

// Translated categories:
- Tab titles
- System information
- Memory labels
- WiFi status
- Test names
- Export buttons
- Status messages
```

**Supported Languages:**
- 🇫🇷 French (default)
- 🇬🇧 English

**Usage:**
```cpp
// In code
String title = t(TEXT_FR_TITLE, TEXT_EN_TITLE);

// Or with macro
String title = T_TITLE;
```

**Extensibility:**
Easy to add other languages:
```cpp
const char* TEXT_ES_TITLE = "Diagnóstico ESP32";  // Spanish
const char* TEXT_DE_TITLE = "ESP32 Diagnose";     // German
```

---

## 🔗 File Dependencies

### Dependency Graph

```
ESP32-Diagnostic.ino (MAIN)
    │
    ├──> config.h (WiFi Configuration)
    │
    ├──> translations.h (Translations)
    │
    ├──> test_functions.h (Tests)
    │    └──> ArduinoJson
    │    └──> Wire.h
    │    └──> esp_*.h
    │
    ├──> api_handlers.h (API)
    │    └──> ArduinoJson
    │    └──> test_functions.h
    │    └──> translations.h
    │
    └──> web_interface.h (Interface)
         └──> Embedded HTML/CSS/JS
         └──> api_handlers.h (endpoints)
```

### IMPORTANT Include Order

⚠️ **Include order is critical:**

```cpp
// 1. System libraries
#include <WiFi.h>
#include <WebServer.h>
...

// 2. Configuration
#include "config.h"

// 3. Modules (precise order)
#include "translations.h"      // Must be before api_handlers
#include "test_functions.h"    // Must be before api_handlers
#include "api_handlers.h"      // Uses translations and test_functions
#include "web_interface.h"     // Must be last
```

**Why this order?**
- translations.h defines `currentLanguage` and `t()` functions
- test_functions.h defines test functions
- api_handlers.h uses translations and test_functions
- web_interface.h uses everything above

---

## 📦 Required External Libraries

### Mandatory Libraries

| Library | Version | Usage |
|---------|---------|-------|
| **Arduino Core ESP32** | 3.3.2+ | Main core |
| **Adafruit NeoPixel** | Latest | NeoPixel control |
| **ArduinoJson** | 6.x | JSON Parse/Serialize |

### Included Libraries

| Library | From | Usage |
|---------|------|-------|
| WiFi.h | ESP32 Core | WiFi connection |
| WiFiMulti.h | ESP32 Core | Multi-WiFi |
| WebServer.h | ESP32 Core | HTTP server |
| ESPmDNS.h | ESP32 Core | mDNS resolution |
| Wire.h | ESP32 Core | I2C communication |
| esp_heap_caps.h | ESP32 Core | Memory management |
| esp_system.h | ESP32 Core | System info |
| esp_chip_info.h | ESP32 Core | Chip info |
| driver/adc.h | ESP32 Core | ADC hardware |

---

## ⚙️ GPIO Configuration

### Default Used GPIO

| GPIO | Usage | Direction | Description |
|------|-------|-----------|-------------|
| 97 | Built-in LED | Output | ESP32-S3 board LED |
| 48 | NeoPixel | Output | WS2812B data pin |
| 14 | TFT CS | Output | TFT Chip Select |
| 47 | TFT DC | Output | TFT Data/Command |
| 21 | TFT RST / OLED SDA | Output/I2C | TFT Reset / I2C Data |
| 20 | OLED SCL | I2C | I2C Clock |
| 1-10 | ADC | Input | ADC channels |
| 1-14 | Touch | Input | Capacitive touch |
| 2-5 | PWM | Output | PWM test |

### Tested GPIO (GPIO Test)

```
1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 
21, 38, 39, 40, 41, 42, 47, 48
```

**Total:** 26 GPIO

**⚠️ Reserved GPIO (do not use):**
- GPIO 19, 20: USB (ESP32-S3)
- GPIO 26-32: Flash SPI (ESP32-S3)
- GPIO 33-37: OPI PSRAM (ESP32-S3)

---

## 🔧 Global Variables

### In ESP32-Diagnostic.ino

```cpp
// Main objects
WebServer server(80);              // HTTP server port 80
WiFiMulti wifiMulti;               // Multi-WiFi manager
Adafruit_NeoPixel* strip;          // NeoPixel pointer
bool neopixelAvailable = false;    // NeoPixel status
bool builtinLedAvailable = true;   // Built-in LED status

// System state
String currentLanguage = "fr";     // Current language
unsigned long lastUpdateTime = 0;  // Last update
const unsigned long UPDATE_INTERVAL = 5000;  // 5 seconds
```

### Defined Constants

```cpp
#define DIAGNOSTIC_VERSION "3.1.0"
#define ARDUINO_CORE_VERSION "3.3.2"

#define BUILTIN_LED_PIN 97
#define NEOPIXEL_PIN 48
#define NEOPIXEL_COUNT 1

#define TFT_CS 14
#define TFT_DC 47
#define TFT_RST 21
#define OLED_SDA 21
#define OLED_SCL 20
```

---

## 📊 Buffers and Memory

### JSON Buffer Sizes

```cpp
// api_handlers.h

StaticJsonDocument<2048> doc;      // handleSystemInfo
DynamicJsonDocument doc(8192);     // handleTests
StaticJsonDocument<128> doc;       // handleGetLanguage
StaticJsonDocument<256> doc;       // handleNeoPixelPattern
DynamicJsonDocument doc(4096);     // handleTestGPIO
StaticJsonDocument<512> doc;       // handleTestADC/Touch/PWM
DynamicJsonDocument doc(4096);     // handleWiFiScan
StaticJsonDocument<512> doc;       // handleBenchmark
DynamicJsonDocument doc(8192);     // handleExportJSON
```

**Memory Optimization:**
- Static for fixed sizes < 1KB
- Dynamic for variable sizes or > 1KB
- Buffers adjusted to actual content

---

## 🌐 Complete API Endpoints

### Summary Table

| Method | Endpoint | Function | Added |
|--------|----------|----------|-------|
| GET | `/` | handleRoot | v3.0.0 |
| GET | `/api/system` | handleSystemInfo | v3.0.0 |
| GET | `/api/tests` | handleTests | v3.0.0 |
| GET | `/api/language` | handleGetLanguage | v3.0.0 |
| POST | `/api/language` | handleSetLanguage | v3.0.0 |
| POST | `/api/led/builtin` | handleBuiltinLED | **v3.1.0** |
| POST | `/api/neopixel/pattern` | handleNeoPixelPattern | v3.0.1 |
| POST | `/api/neopixel/color` | handleNeoPixelColor | v3.0.1 |
| GET | `/api/test/gpio` | handleTestGPIO | **v3.1.0** |
| GET | `/api/test/adc` | handleTestADC | **v3.1.0** |
| GET | `/api/test/touch` | handleTestTouch | **v3.1.0** |
| GET | `/api/test/pwm` | handleTestPWM | **v3.1.0** |
| GET | `/api/scan/wifi` | handleWiFiScan | **v3.1.0** |
| GET | `/api/benchmark` | handleBenchmark | **v3.1.0** |
| GET | `/api/export/json` | handleExportJSON | v3.0.0 |
| GET | `/api/export/csv` | handleExportCSV | v3.0.0 |
| GET | `/api/export/txt` | handleExportTXT | **v3.1.0** |

**Total:** 17 endpoints

---

## 📏 Code Statistics

### Lines of Code per File

| File | Lines | Comments | Effective Code |
|------|-------|----------|----------------|
| ESP32-Diagnostic.ino | ~300 | ~80 | ~220 |
| exemple-config.h | ~200 | ~120 | ~80 |
| api_handlers.h | ~800 | ~150 | ~650 |
| web_interface.h | ~1200 | ~100 | ~1100 |
| test_functions.h | ~400 | ~80 | ~320 |
| translations.h | ~300 | ~40 | ~260 |
| **TOTAL** | **~3200** | **~570** | **~2630** |

### Code Distribution

```
📊 Distribution:
- Web Interface: 37% (web_interface.h)
- API Handlers: 25% (api_handlers.h)
- Hardware Tests: 12% (test_functions.h)
- Translations: 9% (translations.h)
- Configuration: 9% (Main + config)
- Documentation: 8% (Comments)
```

---

## 🔐 Security Files

### .gitignore (IMPORTANT)

```gitignore
# Private configuration
config.h

# Arduino build
build/
*.hex
*.elf
*.bin

# IDE
.vscode/
.idea/
*.swp
*~

# OS
.DS_Store
Thumbs.db

# Logs
*.log

# Backup
*.backup
*.bak
```

**⚠️ Verifications:**
```bash
# Check that config.h is ignored
git status

# config.h must NOT appear in tracked files
```

---

## 📦 Distribution Package

### Files to Distribute

**Mandatory:**
```
✅ ESP32-Diagnostic.ino
✅ exemple-config.h
✅ api_handlers.h
✅ web_interface.h
✅ test_functions.h
✅ translations.h
✅ .gitignore
```

**Documentation:**
```
✅ README.md (French)
✅ README.en.md (English)
✅ CHANGELOG.md (French)
✅ CHANGELOG.en.md (English)
✅ INSTALLATION.md (French)
✅ INSTALLATION.en.md (English)
✅ PROJECT_STRUCTURE.md (French)
✅ PROJECT_STRUCTURE.en.md (English)
✅ LICENSE (MIT)
```

**⚠️ NEVER DISTRIBUTE:**
```
❌ config.h (personal credentials)
❌ build/ (compiled files)
❌ .vscode/ (IDE configuration)
```

---

## 💾 Memory Usage

### Flash Memory (Program)

**Estimated Sizes:**
```
Compiled code: ~450 KB
SPIFFS: ~1.5 MB (reserved)
OTA: 0 KB (not used in v3.1.0)
Free: ~2 MB (on 4MB partition)
```

### RAM Memory (Runtime)

**Typical Usage:**
```
Heap used: ~50-80 KB
Heap free: ~250-300 KB (ESP32-S3)
PSRAM used: ~0 KB (not used in v3.1.0)
PSRAM free: ~8 MB
Stack: ~8-12 KB per task
```

**Memory Peaks:**
- WiFi Scanner: +20 KB temporary
- Benchmarks: +10 KB temporary
- JSON Export: +8 KB temporary

---

<div align="center">

## 🎉 End of Structure Documentation v3.1.0

**Project:** ESP32 Diagnostic System  
**Version:** 3.1.0  
**Date:** October 16, 2025  
**Author:** ESP32 Diagnostic Team  

---

**📚 Related Documents:**
- [README.md](README.md) - Main documentation (French)
- [README.en.md](README.en.md) - Main documentation (English)
- [CHANGELOG.md](CHANGELOG.md) - Version history (French)
- [CHANGELOG.en.md](CHANGELOG.en.md) - Version history (English)

---

*Thank you for using ESP32 Diagnostic System v3.1.0!* ⭐

**[🇫🇷 Version Française](PROJECT_STRUCTURE.md)**

</div>