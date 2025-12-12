# Release Notes - Version 3.22.1

**Release Date**: December 12, 2025  
**Type**: Documentation Fix - ESP32 Classic Pin Mapping Corrections

## Overview

Version 3.22.1 corrects pin mapping duplicates in documentation for ESP32 Classic. The documentation incorrectly showed duplicate pin assignments that didn't match the actual hardware configuration defined in `board_config.h`. This release aligns all user-facing documentation with the authoritative pin definitions.

**No hardware changes required. No firmware functionality changes. Documentation consistency update only.**

> **ℹ️ INFO**: v3.22.1 fixes ESP32 Classic pin mapping documentation duplicates and retains ESP32-S3 GPIO remapping from v3.22.0. Ensure your wiring matches the documented pins for your target board.

## What's Changed

### ESP32 Classic Pin Mapping Documentation Corrections

The following pin assignments were corrected in all documentation to match `include/board_config.h`:

1. **Button 1**: GPIO 2 (was incorrectly documented as GPIO 32)
2. **Button 2**: GPIO 5 (was incorrectly documented as GPIO 33)
3. **Green LED**: GPIO 26 (was incorrectly documented as GPIO 14)
4. **Blue LED**: GPIO 33 (was incorrectly documented as GPIO 25)
5. **DHT Sensor**: GPIO 15 (was incorrectly documented as GPIO 32)
6. **HC-SR04 TRIG**: GPIO 12 (was incorrectly documented as GPIO 27)
7. **HC-SR04 ECHO**: GPIO 35 (was incorrectly documented as GPIO 33)

### ESP32-S3 Configuration

**No Changes from v3.22.0:**
- RGB LED remapping from v3.22.0 remains in effect
- Green LED: GPIO 41 (changed in v3.22.0 from GPIO 45)
- Blue LED: GPIO 42 (changed in v3.22.0 from GPIO 47)
- Red LED: GPIO 21 (unchanged)

## Impact Assessment

### What This Means for Users

**If you have working ESP32 Classic hardware:**
- ✅ Your hardware is likely wired according to `board_config.h` (the correct pins)
- ✅ No changes needed - documentation now matches your setup
- ✅ Continue using your current hardware configuration

**If you wired according to old documentation:**
- ⚠️ Your wiring may not match the firmware expectations
- ⚠️ Review PIN_MAPPING.md and verify your connections
- ⚠️ Rewire to match `board_config.h` definitions if needed

**ESP32-S3 users:**
- ✅ No changes from v3.22.0
- ✅ Use v3.22.0 wiring (GPIO 41/42 for Green/Blue LEDs)

## Corrected Pin Mapping Reference

### ESP32 Classic Complete Pin Mapping (v3.22.1 - Corrected)

| Component | GPIO | Previous Docs | Status |
|-----------|------|---------------|--------|
| Button 1 | 2 | 32 ❌ | ✅ Corrected |
| Button 2 | 5 | 33 ❌ | ✅ Corrected |
| Red LED | 13 | 13 ✅ | No change |
| Green LED | 26 | 14 ❌ | ✅ Corrected |
| Blue LED | 33 | 25 ❌ | ✅ Corrected |
| DHT Sensor | 15 | 32 ❌ | ✅ Corrected |
| HC-SR04 TRIG | 12 | 27 ❌ | ✅ Corrected |
| HC-SR04 ECHO | 35 | 33 ❌ | ✅ Corrected |
| PWM Output | 4 | 4 ✅ | No change |
| Buzzer | 19 | 19 ✅ | No change |
| NeoPixel | 2 | 2 ✅ | No change (configurable) |

### ESP32-S3 Pin Mapping (v3.22.1 - Unchanged from v3.22.0)

| Component | GPIO | Notes |
|-----------|------|-------|
| Red LED | 21 | No change |
| Green LED | 41 | Remapped in v3.22.0 |
| Blue LED | 42 | Remapped in v3.22.0 |
| NeoPixel | 48 | No change |

## Documentation Updates

### Files Updated to v3.22.1

**Version Headers:**
- `platformio.ini` - PROJECT_VERSION updated to "3.22.1"
- All root documentation files (README.md, README_FR.md)
- All `/docs` files (EN/FR versions):
  - API_REFERENCE, ARCHITECTURE, BUILD_AND_DEPLOY
  - CONFIG, CONTRIBUTING, DIAGNOSTIC_MODULES
  - FAQ, FEATURE_MATRIX, INSTALL
  - OVERVIEW, PIN_MAPPING, SECURITY
  - TROUBLESHOOTING, USAGE, WEB_INTERFACE

**Warning Text Updated:**
- All documentation now references v3.22.1
- Warning clarifies ESP32 Classic corrections + ESP32-S3 remapping retention
- Links to PIN_MAPPING.md for complete reference

**Pin Mapping Tables:**
- PIN_MAPPING.md and PIN_MAPPING_FR.md corrected
- ESP32 Classic table now matches `board_config.h` exactly
- Annotations added showing corrections from previous docs

## Code Changes

### No Firmware Changes

**board_config.h:**
- No changes - already had correct pin definitions
- Documentation now matches existing code

**Runtime Behavior:**
- No functional changes
- All hardware tests remain identical
- Firmware version bumped for tracking only

## Version Coherence

### Changelog Updates

**CHANGELOG.md and CHANGELOG_FR.md:**
- v3.22.1 entry added with numbered list of corrections
- Links to v3.22.0 for ESP32-S3 remapping reference

**Version Tracking:**
- All documentation synchronized to v3.22.1
- PROJECT_VERSION macro updated in platformio.ini
- Release notes created (this document)

## Migration Guide

### For All Users

**Action Required:**
1. Read [docs/PIN_MAPPING.md](PIN_MAPPING.md) or [docs/PIN_MAPPING_FR.md](PIN_MAPPING_FR.md)
2. Verify your hardware wiring matches the documented pins
3. Update any personal wiring diagrams or notes

**No Firmware Flash Required:**
- v3.22.1 is documentation-only
- Flash only if you want version number consistency
- Hardware behavior identical to v3.22.0

### Verification Steps

**ESP32 Classic:**
- [ ] Compare your wiring to PIN_MAPPING.md ESP32 Classic table
- [ ] Verify button pins (GPIO 2, 5)
- [ ] Verify RGB LED pins (GPIO 13, 26, 33)
- [ ] Verify sensor pins (GPIO 15, 12, 35)

**ESP32-S3:**
- [ ] Verify v3.22.0 RGB LED wiring (GPIO 21, 41, 42)
- [ ] No changes from v3.22.0

## Known Issues

None reported at release time.

## Previous Versions

- [RELEASE_NOTES_3.22.0.md](RELEASE_NOTES_3.22.0.md) - ESP32-S3 GPIO remapping
- [RELEASE_NOTES_3.21.1.md](RELEASE_NOTES_3.21.1.md) - NeoPixel Wi-Fi status indicator

## Support

For issues, questions, or hardware wiring assistance:
- GitHub Issues: https://github.com/morfredus/ESP32-Diagnostic/issues
- Review PIN_MAPPING.md for complete GPIO reference
- Compare your setup against board_config.h definitions

---

**Release Commit**: `v3.22.1: Correction doublons pin mapping ESP32 Classic + synchronisation docs`  
**Build Target**: `esp32s3_n16r8`, `esp32s3_n8r8`, `esp32_classic`  
**Minimum PlatformIO Core**: 6.1.0  
**Arduino Core**: 3.3.3
