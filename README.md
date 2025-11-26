# ESP32 Diagnostic Suite (v3.12.2)

Comprehensive diagnostic firmware for ESP32 microcontrollers featuring an interactive web dashboard, automated hardware tests, and bilingual content (FR/EN). The firmware targets PlatformIO with ESP32 Arduino Core 3.3.3 and supports the ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6, and ESP32-H2 families.

## Highlights of version 3.12.2
- **BUGFIX**: Robust HC-SR04 distance test on ESP32/ESP32-S3 (pre-echo settle, `pulseInLong` with extended timeout, stable conversion). Prevents false "No echo" errors (v3.12.2).
  
## Highlights of version 3.12.1
- **BUGFIX**: PSRAM reliably enabled for ESP32-S3 DevKitC-1 N16R8 on PlatformIO (board_build.psram, BOARD_HAS_PSRAM). Ensures memory tests and allocations use external PSRAM where available (v3.12.1).
- **RELEASE**: Production-ready release with validated TFT configuration and cleaned repository structure (v3.12.0).
- **CONFIG**: Validated TFT backlight pin (GPIO 15) and resolved duplicate declarations (v3.12.0).
- **DOCUMENTATION**: Removed all development and debugging artifacts for cleaner project structure (v3.12.0).
- **PREVIOUS**: Code quality improvements and cleanup (v3.11.4).
- **BUILD FIX**: Resolved FPSTR() type casting error and web interface JavaScript loading issues (v3.11.2, v3.11.1).
- **NEW FEATURE**: TFT ST7789 display support (240x240) with boot splash screen and WiFi status (v3.11.0).
- **Memory optimization**: Implemented streaming JavaScript delivery to prevent memory overflow.

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
- **Boards:** ESP32, ESP32-S2, ESP32-S3 (recommended), ESP32-C3, ESP32-C6, ESP32-H2.
- **Platform:** PlatformIO with ESP32 Arduino Core 3.3.3.
- **Libraries:** Adafruit BusIO, Adafruit GFX Library, Adafruit ST7735 and ST7789 Library, Adafruit SSD1306, Adafruit NeoPixel, U8g2 (auto-installed via platformio.ini).

## Support
- License: [MIT](LICENSE)
- Issues & feature requests: [Open a GitHub issue](https://github.com/morfredus/ESP32-Diagnostic/issues/new/choose) with reproduction steps and board details.
- Contributions: [Fork the repository](https://github.com/morfredus/ESP32-Diagnostic/fork) and see [CONTRIBUTING.md](CONTRIBUTING.md).
