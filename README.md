# ESP32 Diagnostic Suite (v3.17.0)

Comprehensive diagnostic firmware for ESP32 microcontrollers featuring an interactive web dashboard, automated hardware tests, and bilingual content (FR/EN). The firmware targets PlatformIO with ESP32 Arduino Core 3.3.3 and supports ESP32-S3 and ESP32 Classic targets.

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
2. **Hardware-Specific Pin Mapping**: Dedicated pin configurations in `config.h` for each target.
3. **ESP32-S3 Configuration**: I2C (SDA=21, SCL=20), RGB LED (R=14, G=13, B=18), sensors optimized for S3 GPIO layout.
4. **ESP32 Classic Configuration**: I2C (SDA=21, SCL=22), RGB LED (R=25, G=26, B=27), sensors adapted for Classic GPIO.
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
- `include/wifi-config-example.h` – template for Wi-Fi credential storage.
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
