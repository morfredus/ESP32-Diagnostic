# ESP32 Diagnostic Suite — Release Notes v3.15.0

**Release Date:** November 27, 2025  
**Repository:** https://github.com/morfredus/ESP32-Diagnostic  
**Branch:** dev

---

## Overview

Version 3.15.0 introduces **multi-environment support** for the ESP32 Diagnostic Suite, enabling developers to build and deploy firmware tailored to three distinct ESP32 hardware configurations. This release focuses on hardware flexibility through conditional compilation and dedicated pin mappings.

---

## What's New

### 1. Multi-Environment Build System

Three distinct PlatformIO environments are now available in `platformio.ini`:

#### **esp32s3_n16r8** (Default)
- ESP32-S3 DevKitC-1 N16R8
- 16MB Flash (QIO mode, 80MHz)
- 8MB PSRAM OPI/QSPI (80MHz)
- Partition: `huge_app.csv`
- Target define: `TARGET_ESP32_S3`

#### **esp32s3_n8r8**
- ESP32-S3 DevKitC-1 N8R8
- 8MB Flash
- 8MB PSRAM
- Partition: `huge_app.csv`
- Target define: `TARGET_ESP32_S3`

#### **esp32devkitc**
- ESP32 Classic DevKitC
- 4MB Flash
- No PSRAM
- Partition: `default.csv`
- Target define: `TARGET_ESP32_CLASSIC`

### 2. Hardware-Specific Pin Mappings

Pin configurations are now defined conditionally in `include/config.h`:

**ESP32-S3 Configuration (N16R8 / N8R8):**
- I2C: SDA=21, SCL=20
- RGB LED: R=14, G=13, B=18
- Sensors: DHT=19, Light=4, Distance TRIG=16/ECHO=17, Motion=39, Buzzer=3
- TFT ST7789: MOSI=11, SCLK=12, CS=10, DC=9, RST=7, BL=15
- GPS: RXD=8, TXD=5, PPS=38

**ESP32 Classic Configuration (DevKitC):**
- I2C: SDA=21, SCL=22
- RGB LED: R=25, G=26, B=27
- Sensors: DHT=4, Light=34, Distance TRIG=5/ECHO=18, Motion=36, Buzzer=13
- TFT ST7789: MOSI=23, SCLK=18, CS=15, DC=2, RST=4, BL=32
- GPS: RXD=16, TXD=17, PPS=39
- Buttons: BTN1=0 (BOOT), BTN2=35

### 3. Conditional Compilation

Firmware code now uses preprocessor directives to adapt to target hardware:

```cpp
#if defined(TARGET_ESP32_S3)
    // ESP32-S3 specific code and pin definitions
#elif defined(TARGET_ESP32_CLASSIC)
    // ESP32 Classic specific code and pin definitions
#else
    #error "No target defined"
#endif
```

### 4. Shared Pin Mappings

Where hardware constraints permit, ESP32-S3 N8R8 and ESP32 Classic share common pin assignments to simplify board transitions.

---

## Documentation Updates

All documentation has been updated to reflect multi-environment support:

- **README.md / README_FR.md:** Updated version banner and compatibility matrix
- **CHANGELOG.md / CHANGELOG_FR.md:** Added v3.15.0 release section
- **docs/PIN_MAPPING.md / PIN_MAPPING_FR.md:** Complete pin reference for all three environments
- **docs/BUILD_AND_DEPLOY.md / BUILD_AND_DEPLOY_FR.md:** Build instructions and environment selection guide
- **docs/CONFIG.md / CONFIG_FR.md:** Configuration details for each target

---

## Build & Deployment

### Selecting an Environment

**Method 1: Edit platformio.ini**
```ini
[platformio]
default_envs = esp32s3_n16r8  ; Change to esp32s3_n8r8 or esp32devkitc
```

**Method 2: Command Line**
```bash
# Build specific environment
pio run -e esp32s3_n16r8
pio run -e esp32s3_n8r8
pio run -e esp32devkitc

# Upload to specific board
pio run -e esp32devkitc --target upload
```

### Build Status

All three environments compile successfully:
- ✓ **esp32s3_n16r8:** Build OK, Upload OK, Hardware Tested
- ✓ **esp32s3_n8r8:** Build OK, Compilation Validated
- ✓ **esp32devkitc:** Build OK, Hardware Testing Pending

---

## Migration Guide

### From v3.14.x

No code changes required for existing ESP32-S3 N16R8 deployments. The default environment remains `esp32s3_n16r8`.

For other hardware:
1. Select appropriate environment in `platformio.ini`
2. Review pin mappings in `docs/PIN_MAPPING.md`
3. Rebuild firmware: `pio run -e <environment>`

### Pin Mapping Changes

Pin definitions have moved from runtime variables to compile-time constants based on target. If you have custom pin configurations:

1. Edit `include/config.h`
2. Modify pins in the appropriate `#if defined(TARGET_...)` block
3. Rebuild firmware

Runtime pin changes via web interface remain supported for sensors.

---

## Known Issues

1. **esp32devkitc:** Hardware testing pending due to unavailable reference board
2. **TFT/HC-SR04 conflict (ESP32 Classic):** SCLK (GPIO 18) shares with ECHO pin—use one or the other
3. **ADC2 limitation (ESP32 Classic):** ADC2 pins (GPIO 0, 2, 4, 12-15, 25-27) unusable when WiFi active

---

## Breaking Changes

**None.** This release maintains backward compatibility with v3.14.x firmware for ESP32-S3 N16R8 boards.

---

## Upgrade Path

1. Pull latest code from `dev` branch
2. Select target environment in `platformio.ini`
3. Build: `pio run -e <environment>`
4. Upload: `pio run -e <environment> --target upload`
5. Verify via serial monitor (115200 baud)

---

## Credits

- **Development:** morfredus
- **Testing:** ESP32-S3 N16R8 reference board validated
- **Documentation:** Complete bilingual update (EN/FR)

---

## Next Steps

- Hardware testing on ESP32-DevKitC Classic
- Additional environment support (ESP32-S2, ESP32-C3) if demand warrants
- Pin conflict resolution tools in web interface

---

## Support

- **Issues:** https://github.com/morfredus/ESP32-Diagnostic/issues
- **Documentation:** https://github.com/morfredus/ESP32-Diagnostic/tree/dev/docs
- **License:** MIT

---

**Full Changelog:** [CHANGELOG.md](../CHANGELOG.md) | [CHANGELOG_FR.md](../CHANGELOG_FR.md)
