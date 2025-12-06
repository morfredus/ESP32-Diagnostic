# Release Notes - Version 3.18.2

**Release Date**: December 6, 2025  
**Type**: Patch Release  
**Branch**: `dev/maint`

---

## Overview

Version 3.18.2 is a minor patch release that adds missing translation keys for GPS and environmental sensors display in the web interface. This ensures all UI elements have proper French and English translations.

---

## What's Fixed

### Missing Translation Keys
Added 4 missing translation keys for better internationalization:

1. **`gps_status`** (EN: "GPS Status" / FR: "Statut GPS")
   - Used to display GPS connection status in the wireless interface
   
2. **`temperature_avg`** (EN: "Average Temperature" / FR: "Température moyenne")
   - Label for average temperature when multiple temperature sensors are present
   
3. **`pressure_hpa`** (EN: "Pressure (hPa)" / FR: "Pression (hPa)")
   - Explicit label for barometric pressure with unit display
   
4. **`altitude_calculated`** (EN: "Calculated Altitude" / FR: "Altitude calculée")
   - Label for altitude calculated from barometric pressure (BMP280 sensor)

---

## Technical Details

### Translation System
All translation keys are defined in `include/languages.h` using the `TEXT_RESOURCE_MAP` macro system:

```cpp
X(gps_status, "GPS Status", "Statut GPS") \
X(temperature_avg, "Average Temperature", "Température moyenne") \
X(pressure_hpa, "Pressure (hPa)", "Pression (hPa)") \
X(altitude_calculated, "Calculated Altitude", "Altitude calculée") \
```

These keys are accessible via the `Texts::` namespace in C++ code and rendered dynamically in the JavaScript web interface based on the selected language.

---

## Upgrade Notes

### From 3.18.1 to 3.18.2
- **No code changes required** - this is purely a translation update
- **No configuration changes** - all existing configurations remain valid
- **No API changes** - all existing API endpoints unchanged
- Simply rebuild and upload the new firmware

### Compatibility
- ✅ Fully backward compatible with 3.18.1 and 3.18.0
- ✅ All existing features and configurations work without changes
- ✅ No hardware modifications required

---

## Documentation Updates

- Updated `CHANGELOG.md` with version 3.18.2 entry
- Updated `CHANGELOG_FR.md` with French version 3.18.2 entry
- Created `RELEASE_NOTES_3.18.2.md` (this document)
- Created `RELEASE_NOTES_3.18.2_FR.md` (French version)

---

## Build Information

- **Firmware Version**: `3.18.2`
- **PlatformIO Version**: `6.12.0`
- **Framework**: Arduino (ESP32 v3.20017.241212)
- **Compiler**: GCC 8.4.0 (xtensa-esp32s3)
- **Build Flag**: `-D PROJECT_VERSION='"3.18.2"'`

---

## Known Issues

None identified in this release. All issues from previous versions remain as documented in their respective release notes.

---

## Credits

- **Development**: ESP32-Diagnostic Team
- **Testing**: Community feedback
- **Translation**: Bilingual review completed

---

## Next Steps

After upgrading to 3.18.2:
1. Verify GPS and environmental sensors display correctly in the web UI
2. Test language switching (EN ↔ FR) to confirm all labels are translated
3. Report any remaining untranslated strings for inclusion in future patches

For detailed information about GPS and environmental sensors features, see:
- `RELEASE_NOTES_3.18.0.md` - Initial GPS and environmental sensors implementation
- `RELEASE_NOTES_3.18.1.md` - AHT20 sensor bug fixes
