# Release Notes - Version 3.22.0

**Release Date**: December 11, 2025  
**Type**: Hardware Configuration Update - ESP32-S3 GPIO Remapping

## Overview

Version 3.22.0 resolves GPIO strapping conflicts on ESP32-S3 by relocating RGB LEDs from strapping pins to dedicated GPIO. This hardware change improves boot reliability and aligns with ESP32-S3 design best practices. **This release requires hardware rewiring for ESP32-S3 boards.**

> **⚠️ CRITICAL**: v3.22.0 remaps GPIOs for ESP32-S3. Hardware rewiring is required. Read this document completely before flashing.

## What's Changed

### ESP32-S3 RGB LED Remapping

**Green LED Relocated:**
- **Old**: GPIO 45 (strapping pin - conflicts with boot mode selection)
- **New**: GPIO 41 (dedicated GPIO - no strapping conflicts)
- **Impact**: Requires physical rewiring of green LED connection

**Blue LED Relocated:**
- **Old**: GPIO 47 (strapping pin - conflicts with boot mode selection)
- **New**: GPIO 42 (dedicated GPIO - no strapping conflicts)
- **Impact**: Requires physical rewiring of blue LED connection

**Red LED Unchanged:**
- **Pin**: GPIO 21 (no strapping conflicts - safe pin)
- **Impact**: No wiring change needed

### ESP32 Classic Configuration

**No Changes from v3.21.0:**
- All GPIO mappings remain unchanged
- RGB LEDs: R=13, G=26, B=33
- No hardware rewiring required for ESP32 Classic boards

## Why This Change?

### Technical Justification

**Strapping Pin Conflicts:**
- GPIO 45 and GPIO 47 are strapping pins on ESP32-S3
- Strapping pins determine boot mode, ROM message printing, and other critical behaviors
- External loads (like LEDs) on strapping pins can cause:
  - Boot failures or wrong boot mode selection
  - Unreliable startup behavior
  - Flash programming issues

**Resolution:**
- GPIO 41 and GPIO 42 are dedicated GPIO with no strapping functions
- Provides clean, conflict-free RGB LED operation
- Aligns with Espressif hardware design guidelines

## Migration Guide

### For ESP32-S3 Users

**Hardware Changes Required:**
1. **Disconnect power** from ESP32-S3 board
2. **Remove green LED connection** from GPIO 45
3. **Remove blue LED connection** from GPIO 47
4. **Connect green LED** to GPIO 41 (with 220-470Ω current-limiting resistor)
5. **Connect blue LED** to GPIO 42 (with 220-470Ω current-limiting resistor)
6. **Verify red LED** remains on GPIO 21
7. **Flash v3.22.0 firmware** after rewiring

**Safety Reminder:**
- Always use current-limiting resistors (220-470Ω, 0.25W) in series with LEDs
- Verify 3.3V logic levels - never apply 5V directly to GPIO

### For ESP32 Classic Users

**No Action Required:**
- Flash v3.22.0 firmware directly
- All GPIO mappings unchanged
- Hardware remains compatible

## Complete Pin Mapping Reference

### ESP32-S3 RGB LED Configuration (v3.22.0)

| Component | GPIO | Notes |
|-----------|------|-------|
| Red LED   | 21   | No change from v3.21.0 |
| Green LED | 41   | **Changed from GPIO 45** |
| Blue LED  | 42   | **Changed from GPIO 47** |
| NeoPixel  | 48   | No change |

### ESP32 Classic RGB LED Configuration (v3.22.0)

| Component | GPIO | Notes |
|-----------|------|-------|
| Red LED   | 13   | No change from v3.21.0 |
| Green LED | 26   | No change from v3.21.0 |
| Blue LED  | 33   | No change from v3.21.0 |
| NeoPixel  | 2    | No change (configurable via CUSTOM_LED_PIN) |

## Code Changes

### Updated Constants in board_config.h

**ESP32-S3:**
```cpp
#define DEFAULT_RGB_LED_PIN_R 21  // Red - unchanged
#define DEFAULT_RGB_LED_PIN_G 41  // Green - changed from 45
#define DEFAULT_RGB_LED_PIN_B 42  // Blue - changed from 47
```

**ESP32 Classic:**
```cpp
#define DEFAULT_RGB_LED_PIN_R 13  // No change
#define DEFAULT_RGB_LED_PIN_G 26  // No change
#define DEFAULT_RGB_LED_PIN_B 33  // No change
```

## Documentation Updates

### All User-Facing Documentation Updated

**Warning Added:**
- All root README files (EN/FR)
- All documentation files in `/docs` (EN/FR)
- Prominent remapping warning with links to PIN_MAPPING.md

**Pin Mapping Tables:**
- Updated PIN_MAPPING.md and PIN_MAPPING_FR.md
- Corrected GPIO assignments for ESP32-S3
- Added migration notes in PIN_MAPPING_CHANGES_FR.md

**Version Bumps:**
- All documentation headers updated to v3.22.0
- CHANGELOG.md and CHANGELOG_FR.md updated
- Release notes created (this document)

## Breaking Changes

### Hardware-Level Breaking Changes

**ESP32-S3:**
- ⚠️ RGB LED wiring must be updated
- ⚠️ Old wiring will not work with v3.22.0 firmware
- ⚠️ New wiring will not work with v3.21.x firmware

**ESP32 Classic:**
- ✅ No breaking changes
- ✅ Backward compatible with existing hardware

### Software Compatibility

**Firmware:**
- v3.22.0 firmware incompatible with v3.21.x hardware wiring on ESP32-S3
- v3.22.0 firmware compatible with all ESP32 Classic configurations

**Web Interface:**
- No API changes
- Pin displays automatically updated based on compiled target
- No user action required in web dashboard

## Testing Checklist

After flashing v3.22.0 and rewiring (ESP32-S3 only):

- [ ] System boots without errors
- [ ] Serial console shows correct GPIO assignments
- [ ] Red LED controls respond correctly (web interface + physical)
- [ ] Green LED controls respond correctly (web interface + physical)
- [ ] Blue LED controls respond correctly (web interface + physical)
- [ ] NeoPixel Wi-Fi status indicator functions normally
- [ ] All other hardware tests pass (sensors, displays, etc.)

## Rollback Instructions

If you need to revert to v3.21.1:

**ESP32-S3:**
1. Flash v3.21.1 firmware
2. Rewire green LED back to GPIO 45
3. Rewire blue LED back to GPIO 47

**ESP32 Classic:**
1. Flash v3.21.1 firmware (no wiring changes)

## Known Issues

None reported at release time.

## Previous Version

See [RELEASE_NOTES_3.21.1.md](RELEASE_NOTES_3.21.1.md) for NeoPixel Wi-Fi status indicator features.

## Support

For issues, questions, or hardware wiring assistance:
- GitHub Issues: https://github.com/morfredus/ESP32-Diagnostic/issues
- Review PIN_MAPPING.md for complete GPIO reference
- Review PIN_MAPPING_CHANGES_FR.md for detailed migration history

---

**Release Commit**: `v3.22.0: Remapping GPIO ESP32-S3 (G:45→41, B:47→42) + avertissements docs`  
**Build Target**: `esp32s3_n16r8`, `esp32s3_n8r8`, `esp32_classic`  
**Minimum PlatformIO Core**: 6.1.0  
**Arduino Core**: 3.3.3
