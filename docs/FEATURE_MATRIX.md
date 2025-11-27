# ESP32 Diagnostic Suite – Feature Matrix (v3.15.0)

This matrix summarises the diagnostic coverage for the ESP32 boards supported by version 3.15.0 with three distinct build environments.
Use it to plan validation campaigns and to verify whether optional peripherals require additional wiring.

> **Important:** Version 3.15.0 (PlatformIO) **does not support Bluetooth/BLE** functionality. For BLE diagnostics, use the archived [ESP32-Diagnostic-Arduino-IDE](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE) Arduino IDE version.

**New in v3.15.0:** Multi-environment build system with hardware-specific pin mappings. Select your target board in `platformio.ini` to automatically configure pins and peripherals.

## Legend
- ✅ – Supported out of the box by the firmware.
- ⚙️ – Supported with optional configuration or hardware tweaks (see notes column).
- ⛔ – Not available for the target board or requires custom firmware extensions.

## Core board capabilities
| Board | Environment | Wi-Fi scan | BLE scan | PSRAM probe | Flash integrity | NeoPixel test | OLED test | TFT test | Notes |
|-------|-------------|-----------|----------|-------------|-----------------|---------------|-----------|----------|-------|
| ESP32-S3 N16R8 | `esp32s3_n16r8` | ✅ | ⛔ | ✅ | ✅ | ✅ | ✅ | ✅ | **Primary target.** 16MB Flash, 8MB PSRAM OPI. GPIO 35-48 reserved. Dedicated pin mapping. |
| ESP32-S3 N8R8 | `esp32s3_n8r8` | ✅ | ⛔ | ✅ | ✅ | ✅ | ✅ | ✅ | 8MB Flash, 8MB PSRAM. Shares pin mapping with N16R8. GPIO 35-48 reserved. |
| ESP32 Classic | `esp32devkitc` | ✅ | ⛔ | ⛔ | ✅ | ✅ | ✅ | ✅ | 4MB Flash, **no PSRAM**. Different pin mapping (see PIN_MAPPING.md). No GPIO 35-48 restrictions. |

## Peripheral bus coverage
| Bus / Peripheral | Default pins | Supported boards | Notes |
|------------------|--------------|------------------|-------|
| I2C primary bus | **ESP32-S3:** SDA 21, SCL 20<br>**ESP32 Classic:** SDA 21, SCL 22 | All supported boards | Used for OLED, sensor packs, EEPROM. Pins vary by target. |
| I2C secondary bus | Disabled by default | ESP32, ESP32-S3 | Enable via `ENABLE_SECONDARY_I2C` flag in `config.h`. |
| SPI test bus | **Varies by target** (see PIN_MAPPING.md) | ESP32, ESP32-S3 | Validates external flash/SD adaptors. |
| UART loopback | TX0/RX0 & optional UART1/UART2 | All | Requires jumper wire TX↔RX on chosen UART. |
| TFT ST7789 display | **ESP32-S3:** GPIO 7-15<br>**ESP32 Classic:** GPIO 2-32 | All supported boards | 240x240 display. Pins hardware-specific. |
| RGB LED (separate) | **ESP32-S3:** R=14, G=13, B=18<br>**ESP32 Classic:** R=25, G=26, B=27 | All | Individual RGB control pins. |

## Optional module checklist
| Module | Firmware flag | Default state | Description |
|--------|---------------|---------------|-------------|
| OLED diagnostics | `ENABLE_OLED_TESTS` | Enabled when OLED wiring detected | Runs pixel sweep, contrast, I2C recovery. |
| NeoPixel animator | `ENABLE_NEOPIXEL_TESTS` | Enabled | Exercises colour wheel and stress test at 800 kHz. |
| MQTT reporter | `ENABLE_MQTT_BRIDGE` | Disabled | Publishes summary payloads; requires broker credentials in `config.h`. |
| REST automation | `ENABLE_REST_API` | Enabled | Provides JSON endpoints for CI/CD or fleet tools. |
| Power monitor | `ENABLE_POWER_MONITOR` | Disabled | Reads INA219/INA3221 sensors over I2C. |

## Environmental assumptions
- ESP32 Arduino Core **3.3.3** (PlatformIO `espressif32@6.6.x` or Arduino Boards Manager 3.1.x+).
- USB power supply delivering at least 500 mA during OLED/NeoPixel stress tests.
- Stable Wi-Fi network (2.4 GHz) for throughput benchmarks; captive portals are not supported.

## Related documents
- [Configuration guide](CONFIG.md) – enabling or disabling optional modules.
- [Diagnostic modules deep dive](DIAGNOSTIC_MODULES.md) – execution order and metrics captured for each check.
- [Troubleshooting](TROUBLESHOOTING.md) – steps to follow when a module is reported as unavailable.
