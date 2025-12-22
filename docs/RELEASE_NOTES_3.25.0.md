# ESP32 Diagnostic Suite - Release Notes v3.25.0

**Release Date:** December 22, 2025

## Overview

Version 3.25.0 represents a **major architectural improvement** that restores dynamic GPIO pin remapping functionality while solving preprocessor conflicts that occurred in earlier versions. This release introduces an elegant naming convention solution that maintains both compile-time efficiency and runtime flexibility.

## Key Feature: Dynamic GPIO Pin Remapping Restored

### Problem Solved

In version 3.24.0, an architectural simplification removed the `DEFAULT_` prefix from GPIO pin names in `board_config.h`, which eliminated runtime pin variables. This caused preprocessor conflicts and removed the ability to dynamically remap GPIO pins via the Web UI—a critical feature for hardware flexibility.

### Solution Implemented

**Two-Layer GPIO Architecture with Distinct Naming Conventions:**

1. **Compile-time defaults** (UPPERCASE in `board_config.h`):
   ```cpp
   #define I2C_SDA       15  // SDA pin
   #define I2C_SCL       16  // SCL pin
   #define RGB_LED_PIN_R 21  // RGB Red channel
   ```

2. **Runtime variables** (lowercase in `main.cpp`):
   ```cpp
   int i2c_sda = I2C_SDA;          // Modifiable at runtime
   int i2c_scl = I2C_SCL;          // Modifiable at runtime
   int rgb_led_pin_r = RGB_LED_PIN_R;  // Modifiable at runtime
   ```

**Key Insight:** Using different naming conventions (UPPERCASE vs lowercase) prevents the preprocessor from expanding lowercase variable names, eliminating conflicts while maintaining both layers.

## What's New

### Added

- **Runtime Pin Variables with Lowercase Names**
  - Declared in `src/main.cpp:201-216`
  - External declarations in `include/web_interface.h:24-35`
  - Variables: `i2c_sda`, `i2c_scl`, `rgb_led_pin_r`, `rgb_led_pin_g`, `rgb_led_pin_b`, `pwm_pin`, `buzzer_pin`, `dht_pin`, `light_sensor_pin`, `distance_trig_pin`, `distance_echo_pin`, `motion_sensor_pin`

### Restored

- **Dynamic Pin Remapping via Web UI** - All configuration handlers fully functional:
  - I2C pins (OLED and environmental sensors)
  - RGB LED pins (R, G, B channels)
  - Buzzer pin
  - DHT sensor pin
  - Light sensor pin
  - Distance sensor pins (Trigger and Echo)
  - Motion sensor pin

### Changed

- **All GPIO Pin References Updated** (~100+ occurrences):
  - `src/main.cpp`: Test functions, configuration handlers, JavaScript injection
  - `src/environmental_sensors.cpp`: I2C pin references
  - Maintains UPPERCASE defines in `board_config.h` as authoritative source

## Technical Architecture

### Two-Layer System Benefits

| Layer | Location | Format | Purpose | Modifiable |
|-------|----------|--------|---------|------------|
| Compile-time | `board_config.h` | `#define I2C_SDA 15` | Default values | No (requires recompilation) |
| Runtime | `main.cpp` | `int i2c_sda = I2C_SDA;` | Active pin mapping | Yes (via Web UI) |

### Why This Works

- **No preprocessor conflicts**: Lowercase variable names don't trigger macro expansion
- **Clear distinction**: UPPERCASE = compile-time, lowercase = runtime
- **Performance**: Compiler optimizes constant initialization
- **Flexibility**: Users can test different pin configurations without recompiling

## Migration Guide

### From v3.24.0

1. **No code changes required** - architecture is backward compatible
2. **Firmware update recommended** - flash new version to restore Web UI pin remapping
3. **No hardware changes** - pin mappings remain identical to v3.24.0 defaults

### From v3.23.x

- **Fully compatible** - all features from v3.23.x are preserved
- **Improved architecture** - cleaner naming convention prevents conflicts

## Files Modified

### Code Files
- `src/main.cpp` - Runtime variables, handlers, 100+ pin references
- `include/web_interface.h` - External declarations
- `src/environmental_sensors.cpp` - I2C pin references

### Configuration
- `include/board_config.h` - Maintains UPPERCASE defines (from user modifications)
- `platformio.ini` - Version bump to 3.25.0

### Documentation
- `CHANGELOG.md` / `CHANGELOG_FR.md` - Release notes added
- This file (`RELEASE_NOTES_3.25.0.md`)

## Compatibility

- **ESP32-S3 (N16R8, N8R8)**: ✅ Fully supported
- **ESP32 Classic (DevKitC)**: ✅ Fully supported
- **PlatformIO**: espressif32@^6
- **Arduino Core**: ESP32 3.3.3
- **Backward Compatibility**: ⚠️ Requires firmware update from v3.24.0

## Known Issues

None reported for this release.

## Next Steps

For detailed technical documentation on GPIO pin management, see:
- `docs/PIN_POLICY.md` - GPIO mapping policy and naming conventions
- `docs/PIN_MAPPING.md` - Complete pin mapping reference

## Credits

This architectural improvement was developed to solve real-world preprocessor conflicts while maintaining the flexibility that users depend on for hardware prototyping and testing.

---

**Full Changelog**: See [CHANGELOG.md](../CHANGELOG.md) for complete version history
