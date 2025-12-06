# Release Notes — Version 3.18.3

**Release Date:** December 6, 2025  
**Type:** Patch Release (Pin Mapping Fix)

---

## Overview

Version 3.18.3 resolves a critical hardware conflict on ESP32-S3 boards where GPIO 48 was simultaneously assigned to the RGB LED Green channel and the NeoPixel/WS2812B LED. This release reorganizes the complete ESP32-S3 pin mapping to eliminate conflicts and enable NeoPixel LED functionality.

---

## Changes Summary

### Fixed
- **GPIO 48 Conflict Resolution**: NeoPixel and RGB LED were competing for GPIO 48
  - NeoPixel now exclusively uses GPIO 48 (enabled, previously disabled)
  - RGB LED pins reallocated: Green to GPIO 47, Blue to GPIO 45
  - RGB Red remains on GPIO 19

### Changed — ESP32-S3 Pin Mapping Refactoring

#### RGB LED Pins (ESP32-S3)
| Component | Old Pin | New Pin | Notes |
|-----------|---------|---------|-------|
| RGB Red   | GPIO 19 | GPIO 19 | Unchanged |
| RGB Green | GPIO 48 | GPIO 47 | **Relocated to resolve conflict** |
| RGB Blue  | GPIO 45 | GPIO 45 | Unchanged |

#### NeoPixel Configuration (ESP32-S3)
| Parameter | Old Value | New Value | Notes |
|-----------|-----------|-----------|-------|
| Pin       | -1 (disabled) | GPIO 48 | **Now enabled** |
| LED Count | 8         | 1         | Single onboard LED |
| Brightness| 50        | 50        | Unchanged |

#### Sensor Pins Reorganization (ESP32-S3)
| Sensor | Old Pin | New Pin | Rationale |
|--------|---------|---------|----------|
| Motion Sensor (PIR) | GPIO 6 | GPIO 46 | Freed GPIO 6 for Distance Echo |
| Light Sensor (ADC) | GPIO 19 | GPIO 4 | Freed GPIO 19 for RGB Red |
| HC-SR04 ECHO | GPIO 19 | GPIO 6 | Freed from RGB Blue conflict |

**Important Note:** GPIO 48 is now exclusively for NeoPixel. RGB and sensor pins have been reassigned to eliminate all conflicts.

#### Unchanged Pins (ESP32-S3)
- **I2C**: SDA=21, SCL=20
- **Buttons**: BTN1=1, BTN2=2
- **GPS (UART1)**: RXD=18, TXD=17, PPS=8
- **TFT ST7789**: MOSI=11, SCLK=12, CS=10, DC=9, RST=13, BL=7
- **PWM/Buzzer**: GPIO 14
- **DHT Sensor**: GPIO 5
- **HC-SR04 TRIG**: GPIO 3

### Documentation Updates
- `README.md` and `README_FR.md`: Updated version and ESP32-S3 pin summary
- `docs/PIN_MAPPING.md` and `docs/PIN_MAPPING_FR.md`: Complete pin mapping tables updated
- `include/config-example.h`: Synchronized with definitive ESP32-S3 configuration
- `platformio.ini`: Version bumped to 3.18.3

---

## Migration Guide

### If You're Using ESP32-S3 Hardware

1. **RGB LED Wiring** (ESP32-S3 DevKitC-1 N16R8):
   - RGB Red: GPIO 19 (no change)
   - RGB Green: GPIO 47 (moved from GPIO 48)
   - RGB Blue: GPIO 45 (no change)
   - Reroute Green wire from GPIO 48 to GPIO 47 on the physical board

2. **NeoPixel LED**:
   - If you have an onboard or external NeoPixel on GPIO 48, it will now work correctly
   - Set `DEFAULT_NEOPIXEL_PIN 48` and `DEFAULT_NEOPIXEL_COUNT 1` in `config.h`

3. **Sensor Adjustments** (ESP32-S3):
   - Motion Sensor: moved to GPIO 46 (from GPIO 6)
   - Light Sensor: GPIO 4 (from GPIO 19)
   - HC-SR04 ECHO: GPIO 6 (from GPIO 19)
   - Verify sensor pins match the updated `config.h` definitions

4. **All Conflicts Resolved**:
   - GPIO 48 is now exclusively for NeoPixel
   - No more GPIO conflicts between RGB and sensors
   - All pin mappings are unique and non-overlapping

### If You're Using ESP32 Classic

**No changes required.** ESP32 Classic pin mappings remain unchanged.

### Software Migration

1. **Update Configuration**:
   ```bash
   # Pull latest changes
   git pull origin main
   
   # Copy new template if needed
   cp include/config-example.h include/config.h
   ```

2. **Rebuild Firmware**:
   ```bash
   pio run -e esp32s3_n16r8
   pio run -e esp32s3_n16r8 --target upload
   ```

3. **Verify Pin Assignments**: Check serial output during boot to confirm pin detection

---

## Testing Recommendations

1. **RGB LED Test**: Verify all three color channels (red, green, blue) work independently
2. **NeoPixel Test**: Confirm NeoPixel on GPIO 48 responds to color commands
3. **Sensor Tests**: Test Light, Motion, and Distance sensors individually to verify new pin assignments
4. **Conflict Test**: Verify that using RGB Blue or HC-SR04 ECHO on GPIO 19 works as expected (but not both simultaneously)

---

## Known Issues

- GPIO 19 conflict between RGB Blue and HC-SR04 ECHO requires exclusive usage
- Consider using TFT display or OLED for visual feedback if RGB LED is disabled

---

## Semantic Versioning Rationale

This is a **patch release** (3.18.2 → 3.18.3) because:
- It fixes a hardware conflict bug (GPIO 48 assignment)
- No new features added
- No breaking changes to API or software interfaces
- Changes are limited to hardware pin configuration
- Backward compatible for ESP32 Classic users
- ESP32-S3 users need to adjust hardware wiring but software remains compatible

---

## Credits

- Pin conflict identified during hardware testing
- Resolution prioritizes onboard NeoPixel functionality
- Sensor pins reorganized to maintain full feature compatibility

---

For detailed pin mapping documentation, see:
- [PIN_MAPPING.md](PIN_MAPPING.md) (English)
- [PIN_MAPPING_FR.md](PIN_MAPPING_FR.md) (Français)

For questions or issues, please open a GitHub issue at:  
**https://github.com/morfredus/ESP32-Diagnostic/issues**
