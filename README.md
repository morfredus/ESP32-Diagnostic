# ESP32 Diagnostic Suite (v3.20.3)

Comprehensive diagnostic firmware for ESP32 microcontrollers featuring an interactive web dashboard, automated hardware tests, and bilingual content (FR/EN). The firmware targets PlatformIO with ESP32 Arduino Core 3.3.3 and supports ESP32-S3 and ESP32 Classic targets.

## New in version 3.20.3

**Code Optimization Release** - Systematic memory efficiency improvements:
- Applied 9 optimizations ([OPT-001] through [OPT-009]) eliminating 90+ string allocations
- Buffer-based formatting for version strings, uptime, GPIO lists, and test results
- Created reusable constants (DEFAULT_TEST_RESULT_STR, OK_STR, FAIL_STR) used in 30+ locations
- Converted 13 debug/status messages to snprintf approach (2-9 allocations → 1)
- **No functionality changes** - all pins, tests, and features remain identical
- Successfully compiled and validated on ESP32-S3 and ESP32 CLASSIC targets

## Previous: version 3.20.2
1. **Dynamic Web UI Pin References**: Web interface now displays correct GPIO pins based on compiled target
   - All hardcoded pin values replaced with dynamic references from `board_config.h`
   - RGB LED, DHT sensor, light sensor, distance sensor, motion sensor, and PWM pins now read from configuration
   - Pin values injected as JavaScript constants, ensuring UI always shows actual hardware pins
   - Serial startup banner displays TARGET identification and actual pin values for verification

## New in version 3.20.1
1. **USB/OTG stability (ESP32-S3)**: Freed USB D-/D+ by moving defaults off GPIO19/20.
   - I2C defaults: SDA=15, SCL=16
   - RGB LED: Red=21, Green=45, Blue=47 (Red leaves GPIO19)
   - Keeps OTG stable; note GPIO45 is a strapping pin—LED remains off at boot.

## New in version 3.20.0
1. **Advanced Button Management**: Enhanced button functionality with visual feedback and interactive controls
   - **BOOT Button (GPIO 0)**: Long press (2 seconds) triggers system reboot with progress bar on TFT display; release before 2s clears the screen
   - **Button 1 (GPIO 38)**: Short press cycles through RGB LED colors (Off → Red → Green → Blue → White)
   - **Button 2 (GPIO 39)**: Short press triggers confirmation beep
   - Real-time visual feedback during long-press operations on TFT
   - Debouncing and long-press detection for reliable operation

## New in version 3.19.0
1. **Pin Mapping Isolation**: Board GPIO definitions extracted to `include/board_config.h`; `config.h` now contains only common configuration settings.
   - All pin definitions for ESP32-S3 (N16R8 / N8R8) and ESP32 Classic consolidated in one place
   - Buttons remain on GPIO 38/39 (ESP32-S3) to avoid USB upload/reset conflicts
   - No other pin values changed
2. **Secrets Management**: Wi-Fi configuration file renamed from `wifi-config.h` to `secrets.h`
   - Legacy `wifi-config.h` headers now emit compile-time error to force migration
   - `secrets.h` is protected in `.gitignore` to prevent credential leaks
   - Use `include/secrets-example.h` as template
3. **Documentation Complete**: All user-facing docs updated to reflect architecture changes and v3.19.0 features
4. **Version Consistency**: `PROJECT_VERSION` updated to 3.19.0 in `platformio.ini`

## New in version 3.18.2
1. **Translation Keys**: Added missing translation keys for complete GPS and environmental sensors UI support.
   - `gps_status`: GPS connection status indicator
   - `temperature_avg`: Average temperature label for multi-sensor displays
   - `pressure_hpa`: Barometric pressure with unit display
   - `altitude_calculated`: Calculated altitude from pressure

## New in version 3.18.1
1. **AHT20 Sensor Fix**: Corrected bit extraction algorithm for AHT20 humidity and temperature values (proper 20-bit value extraction from 6-byte response).
2. **Environmental Sensors API**: Fixed JSON structure for web interface compatibility.

## New in version 3.18.0
1. **GPS Module Support**: Full NEO-6M/NEO-8M/NEO-M GPS receiver integration.
   - NMEA sentence parsing (RMC, GGA, GSA, GSV)
   - Latitude, longitude, altitude, speed, course tracking
   - Satellite count and signal quality (HDOP, VDOP, PDOP)
   - Web interface integration with GPS status display
2. **Environmental Sensors**: AHT20 (temperature/humidity) + BMP280 (pressure/altitude) on I2C.
   - Real-time sensor data streaming to web interface
   - Altitude calculation from barometric pressure
   - Sensor status indicators (OK/Error/Not detected)
3. **Web UI Enhancements**: GPS module section in Wireless page, environmental sensors in Sensors page.

## New in version 3.17.1
1. **Refreshed pin mapping (ESP32-S3 & Classic):** Updated defaults in `include/config.h` to align GPS, TFT, RGB, and sensor pins with DevKitC-1 layout and to keep buttons intact. ESP32-S3 defaults now use GPS RX=18/TX=17/PPS=8; TFT MOSI=11, SCLK=12, CS=10, DC=9, RST=13, BL=7; RGB R=47/G=48/B=45; BTN1=1, BTN2=2; PWM/Buzzer=14; DHT=5; Motion=4; Light=19; HC-SR04 TRIG=3/ECHO=6. ESP32 Classic defaults now use GPS RX=16/TX=17/PPS=4; TFT MOSI=23, SCLK=18, CS=19, DC=27, RST=26, BL=13; RGB R=12/G=14/B=15; BTN1=34, BTN2=35; PWM/Buzzer=5; DHT=25; Motion=36; Light=2; HC-SR04 TRIG=32/ECHO=33.
2. **Docs + version bump:** Pin mapping guides (EN/FR), README, feature matrix, usage, and build notes updated to reflect the new defaults; `PROJECT_VERSION` set to `3.17.1`.

## New in version 3.17.0
1. **Hardware Buttons (Optional)**: Simple actions bound to BTN1/BTN2 without changing pin mapping.
  - BTN1: short press plays a brief buzzer tone for user feedback.
  - BTN2: short press cycles RGB LED colors (red → green → blue → white).
  - Enable via `ENABLE_BUTTONS` in `include/config.h`.

## New in version 3.16.0
1. **Network Monitoring**: Automatic logging of connected client IP addresses in the Serial Monitor for enhanced diagnostics.
2. **WiFi Status Display**: Real-time network connection status in WiFi tab showing current IP address, SSID, gateway, DNS, and signal strength.
3. **OLED Resolution Configuration**: Dynamic configuration of OLED screen resolution (width × height) through the web interface.
4. **TFT Configuration UI**: Complete TFT display configuration including pin mapping (MOSI, SCLK, CS, DC, RST, BL), resolution, and rotation settings.
5. **New API Endpoint**: `/api/tft-config` for comprehensive TFT configuration with real-time validation.
6. **Enhanced Display Info**: Updated `/api/screens-info` to include full resolution and pin details for both OLED and TFT.

## New in version 3.15.1
**Critical Bug Fix**: Resolved web interface loading failure on ESP32 Classic (`esp32devkitc`) caused by memory exhaustion. JavaScript content is now streamed in small chunks from PROGMEM, eliminating heap crashes on boards without PSRAM. All environments benefit from improved memory efficiency.

## New in version 3.15.0
1. **Multi-Environment Support**: Three distinct build environments for different ESP32 boards:
   - `esp32s3_n16r8` (default): ESP32-S3 with 16MB Flash + 8MB PSRAM
   - `esp32s3_n8r8`: ESP32-S3 with 8MB Flash + 8MB PSRAM
   - `esp32devkitc`: ESP32 Classic with 4MB Flash (no PSRAM)
2. **Hardware-Specific Pin Mapping**: Dedicated pin configurations in `board_config.h` (automatically included by `config.h`) for each target.
3. **ESP32-S3 Configuration (updated in 3.19.0)**: I2C (SDA=21, SCL=20), RGB LED (R=19, G=47, B=45), NeoPixel=48, buttons BTN1=38 / BTN2=39, GPS RX=18/TX=17/PPS=8, TFT MOSI=11/SCLK=12/CS=10/DC=9/RST=13/BL=7, sensors (PWM/Buzzer=14, DHT=5, Motion=46, Light=4, HC-SR04 TRIG=3/ECHO=6).
4. **ESP32 Classic Configuration (updated in 3.17.1)**: I2C (SDA=21, SCL=22), RGB LED (R=12, G=14, B=15), buttons BTN1=34 / BTN2=35, GPS RX=16/TX=17/PPS=4, TFT MOSI=23/SCLK=18/CS=19/DC=27/RST=26/BL=13, sensors (PWM/Buzzer=5, DHT=25, Motion=36, Light=2, HC-SR04 TRIG=32/ECHO=33).
5. **Shared Mappings**: Common pin assignments between ESP32-S3 N8R8 and ESP32 Classic where hardware permits.

## Highlights of version 3.14.0
1. **TFT Web Interface**: Complete cartouche for testing TFT ST7789 display (240x240) via web interface.
2. **Individual TFT Tests**: 8 tests available (boot splash, colors, shapes, text, lines, animation, progress, final message).
3. **Boot Screen Buttons**: Quick restore startup display for both OLED and TFT.
4. **TFT REST API**: `/api/tft-test`, `/api/tft-step`, `/api/tft-boot` for programmatic control.
5. **OLED REST API**: `/api/oled-boot` to restore OLED boot screen.

## Project structure
- `src/main.cpp` – firmware entry point, main loop, task scheduler, and HTTP handlers.
- `include/languages.h` – translation catalog and runtime language switcher.
- `include/web_interface.h` – HTML/CSS/JS templates served by the firmware.
- `include/board_config.h` – board-specific pin mapping (selected via TARGET_ESP32_*).
- `include/secrets-example.h` – template for Wi-Fi credential storage (copy to `include/secrets.h`).
- `platformio.ini` – PlatformIO configuration for all supported ESP32 targets.
- `docs/` – comprehensive wiki including installation, usage, API reference, and deployment guides (EN/FR).

## Documentation
| Topic | English | Français |
|-------|---------|----------|
| Wiki home | [home.md](docs/home.md) | [home_FR.md](docs/home_FR.md) |
| Overview | [OVERVIEW.md](docs/OVERVIEW.md) | [OVERVIEW_FR.md](docs/OVERVIEW_FR.md) |
| Feature matrix | [FEATURE_MATRIX.md](docs/FEATURE_MATRIX.md) | [FEATURE_MATRIX_FR.md](docs/FEATURE_MATRIX_FR.md) |
| Diagnostic modules | [DIAGNOSTIC_MODULES.md](docs/DIAGNOSTIC_MODULES.md) | [DIAGNOSTIC_MODULES_FR.md](docs/DIAGNOSTIC_MODULES_FR.md) |
| Web interface | [WEB_INTERFACE.md](docs/WEB_INTERFACE.md) | [WEB_INTERFACE_FR.md](docs/WEB_INTERFACE_FR.md) |
| REST API | [API_REFERENCE.md](docs/API_REFERENCE.md) | [API_REFERENCE_FR.md](docs/API_REFERENCE_FR.md) |
| Build & deploy | [BUILD_AND_DEPLOY.md](docs/BUILD_AND_DEPLOY.md) | [BUILD_AND_DEPLOY_FR.md](docs/BUILD_AND_DEPLOY_FR.md) |
| Security | [SECURITY.md](docs/SECURITY.md) | [SECURITY_FR.md](docs/SECURITY_FR.md) |
| FAQ | [FAQ.md](docs/FAQ.md) | [FAQ_FR.md](docs/FAQ_FR.md) |
| Installation | [INSTALL.md](docs/INSTALL.md) | [INSTALL_FR.md](docs/INSTALL_FR.md) |
| Configuration | [CONFIG.md](docs/CONFIG.md) | [CONFIG_FR.md](docs/CONFIG_FR.md) |
| Usage guide | [USAGE.md](docs/USAGE.md) | [USAGE_FR.md](docs/USAGE_FR.md) |
| Firmware architecture | [ARCHITECTURE.md](docs/ARCHITECTURE.md) | [ARCHITECTURE_FR.md](docs/ARCHITECTURE_FR.md) |
| Troubleshooting | [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md) | [TROUBLESHOOTING_FR.md](docs/TROUBLESHOOTING_FR.md) |
| Contribution rules | [CONTRIBUTING.md](CONTRIBUTING.md) | [CONTRIBUTING_FR.md](CONTRIBUTING_FR.md) |
| Changelog | [CHANGELOG.md](CHANGELOG.md) | [CHANGELOG_FR.md](CHANGELOG_FR.md) |

## Core capabilities
- Multilingual interface with real-time FR/EN switching without page reload.
- Automated hardware diagnostics covering GPIO, ADC, touch pads, PWM, I2C, SPI, PSRAM, flash, and Wi-Fi scanning.
- **TFT ST7789 display support** with boot splash screen and real-time WiFi/IP status (240x240 resolution).
- OLED 0.96" I2C screen test suite with per-step triggers and animation previews.
- **Dual display support**: simultaneous OLED and TFT operation for enhanced diagnostics.
- REST API endpoints for triggering diagnostics and retrieving reports (TXT/JSON/CSV/printable view).
- Wi-Fi multi-AP support, mDNS discovery via `http://ESP32-Diagnostic.local`, and exportable reports.
- Optional NeoPixel/WS2812B LED test patterns and benchmarking tools for CPU/memory.

## Compatibility & requirements
- **Boards:** 
  - ESP32-S3-DevKitC-1 N16R8 (16MB Flash, 8MB PSRAM OPI/QSPI) — Primary target
  - ESP32-S3-DevKitC-1 N8R8 (8MB Flash, 8MB PSRAM)
  - ESP32-DevKitC Classic (4MB Flash, no PSRAM)
- **Platform:** PlatformIO with ESP32 Arduino Core 3.3.3+
- **Build Environments:** 
  - `esp32s3_n16r8` (default): ESP32-S3 with huge_app partition, PSRAM enabled, OPI support
  - `esp32s3_n8r8`: ESP32-S3 with huge_app partition, 8MB Flash
  - `esp32devkitc`: ESP32 Classic with default partition, 4MB Flash
- **Environment Selection:** Change `default_envs` in `platformio.ini` or use `pio run -e <environment>`
- **Libraries:** Adafruit BusIO ^1.17, Adafruit GFX ^1.12, Adafruit ST7735/ST7789 ^1.11, Adafruit NeoPixel ^1.12, U8g2 ^2.36 (auto-installed via platformio.ini).

## Support
- License: [MIT](LICENSE)
- Issues & feature requests: [Open a GitHub issue](https://github.com/morfredus/ESP32-Diagnostic/issues/new/choose) with reproduction steps and board details.
- Contributions: [Fork the repository](https://github.com/morfredus/ESP32-Diagnostic/fork) and see [CONTRIBUTING.md](CONTRIBUTING.md).
