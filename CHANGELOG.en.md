# 📋 CHANGELOG - ESP32 Diagnostic

[🇫🇷 Lire en Français](CHANGELOG.md) | [📖 Documentation](README.en.md)

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [3.2.0] - 2025-10-17

### ✨ New Features

#### Multi-SSID WiFi Configuration
- **WiFiMulti** : Multiple WiFi network support
  - Configure multiple SSID/password pairs
  - Automatic connection to strongest available network
  - Configurable priority order
  - Automatic reconnection on signal loss
- **wifi-config.h file** : Separate and secure WiFi configuration
  - Template `exemple-wifi-config.h` provided
  - Supports unlimited number of networks
  - Automatic Git exclusion (.gitignore)
  - Fallback access point configuration

#### Dynamic Hardware Configuration
- **config.h file** : Centralized pin configuration
  - Built-in LED configuration
  - NeoPixel configuration (optional)
  - OLED configuration (optional)
  - Temperature sensor configuration (optional)
  - Buzzer and buttons configuration (optional)
  - Configurable arrays for GPIO/PWM/ADC tests

#### Enhanced Web Interface
- **Pin configuration** : Edit via web interface (coming soon)
  - Modify pins without recompiling
  - Persistent configuration saving
  - Real-time pin validation

### 🔒 Security

- **Protected sensitive files** :
  - `wifi-config.h` excluded from Git
  - `config.h` excluded from Git
  - Complete `.gitignore` provided
- **Access Point Mode** : Automatic failover if no WiFi available
- **Missing file detection** : Warnings if configuration absent

### 🔧 Improvements

#### Architecture
- **Configuration separation** : Code/Configuration decoupled
- **Templates provided** : Detailed examples for all configurations
- **Automatic detection** : Uses `__has_include` for optional files
- **Compatibility** : Works without configuration files (default mode)

#### Performance
- **Smart reconnection** : Attempts reconnection every 30s if disconnected
- **Multi-network** : No interruption when switching networks
- **Configurable timeout** : Adjustable per network

### 📝 Documentation

- **INSTALLATION.md** : Section added for multi-WiFi configuration
- **INSTALLATION.en.md** : English version updated
- **README.md** : Documentation of new architecture
- **README.en.md** : English version updated
- **Detailed examples** : In wifi-config.h and config.h

### ⚙️ Configuration Required

**New files to create** :
1. Copy `exemple-wifi-config.h` → `wifi-config.h`
2. Copy `exemple-config.h` → `config.h`  
3. Configure your WiFi networks in `wifi-config.h`
4. Adjust hardware pins in `config.h`

---

## [3.1.1] - 2025-10-17

### 🐛 Bug Fixes

#### Compilation fixes
- **PWM API** : Migration to new `ledcAttach()` API for ESP32 Core 3.x
  - Replaced `ledcSetup()` + `ledcAttachPin()` with `ledcAttach()`
  - Compatible with all recent ESP32 core versions
- **Function typing** : Fixed return types
  - `getFlashSize()` now returns `size_t` instead of `String`
  - `getPSRAMSize()` now returns `size_t` instead of `String`
  - Fixed `handleExportCSV()` to use numeric values

#### Missing functions
- **NeoPixel** : Added functions in main file
  - `setNeoPixelColor(r, g, b)` : Set RGB color
  - `neoPixelRainbow(cycles)` : Rainbow animation
  - Empty versions if `USE_NEOPIXEL` not defined
- **Built-in LED** : Added `initBuiltinLED()`

#### Syntax corrections
- **Missing quotes** : Fixed line 820 in `api_handlers.h`
  - Added closing quote in `server.send()`
- **Braces** : Verified all function closures

#### API Routes
- **Simplification** : Removed unimplemented routes
  - Removed `/api/led/builtin` (not implemented)
  - Removed `/api/test/adc` (not implemented)
  - Removed `/api/test/touch` (not implemented)
  - Removed `/api/export/txt` (not implemented)

### 📝 Documentation

- **INSTALLATION.md** : Version update
- **INSTALLATION.en.md** : Version update
- Fixed version references in all examples

---

## [3.1.0] - 2025-10-16

### ✨ New Features

#### Web Interface
- Modern and responsive interface with professional design
- 8 thematic pages: Dashboard, Tests, Memory, WiFi, GPIO, Benchmarks, Export, About
- FR/EN language selector with persistence
- Smooth animations and transitions
- Elegant dark mode with gradients

#### REST API
- 18 complete REST endpoints
- CORS support for cross-origin development
- JSON and CSV export
- Multilingual management via API

#### NeoPixel System
- WS2812B NeoPixel LED support
- RGB color control via API
- Predefined patterns (blink, fade, rainbow, test)
- System status visual indicators

#### Hardware Tests
- Complete GPIO tests
- PWM tests with new API
- WiFi scan with network details
- Performance benchmarks (CPU, memory)
- Internal sensor tests

### 🔧 Improvements

#### Performance
- Memory optimization for ESP32-S3
- Efficient JSON buffer management
- Reduced web server latency

#### Compatibility
- ESP32 Core 3.x support
- Compatible ESP32, ESP32-S2, ESP32-S3, ESP32-C3
- Automatic PSRAM detection

#### Documentation
- Complete README in FR and EN
- Detailed installation guide
- Documented project structure
- REST API examples

---

## [3.0.0] - 2025-10-15

### ✨ Initial Release

#### Basic Features
- ESP32 diagnostic system
- Basic web interface
- System information
- Simple hardware tests
- Data export

#### Compatibility
- Classic ESP32
- Basic WiFi support
- Serial monitor

---

## 🔗 Links

- **Documentation** : [README.en.md](README.en.md)
- **Installation** : [INSTALLATION.en.md](INSTALLATION.en.md)
- **Structure** : [PROJECT_STRUCTURE.en.md](PROJECT_STRUCTURE.en.md)
- **Repository** : [GitHub](https://github.com/your-username/ESP32-Diagnostic)

---

## 📌 Versioning Convention

This project follows [Semantic Versioning](https://semver.org/spec/v2.0.0.html) :

- **MAJOR** (X.0.0) : Breaking changes incompatible with previous versions
- **MINOR** (0.X.0) : New features compatible with previous versions
- **PATCH** (0.0.X) : Bug fixes compatible with previous versions

### Change Types

- ✨ **New Features** : Adding new capabilities
- 🔧 **Improvements** : Optimizations and enhancements
- 🐛 **Bug Fixes** : Bug corrections
- 🔒 **Security** : Vulnerability fixes
- 📝 **Documentation** : Documentation updates
- ⚠️ **Deprecated** : Features to be removed soon
- 🗑️ **Removed** : Removed features

---

**Last update** : October 17, 2025  
**Current version** : 3.1.1