# Release Notes - Version 3.18.1

**Release Date**: December 6, 2025  
**Type**: Patch Release (Bug Fix)

## Overview

Version 3.18.1 is a critical patch release that fixes sensor data reading issues introduced in version 3.18.0. This update resolves incorrect temperature and humidity readings from the AHT20 sensor and improves the environmental sensors API for better web interface compatibility.

## What's Fixed

### AHT20 Sensor Data Reading (Critical Fix)
- **Issue**: Incorrect bit extraction algorithm caused wrong temperature and humidity values
- **Root Cause**: The 20-bit values were not properly extracted from the 6-byte sensor response
- **Solution**: Implemented correct bit manipulation to extract humidity from bytes 0-2 and temperature from bytes 2-4
- **Impact**: Temperature and humidity readings now display accurate values

### Environmental Sensors API Structure
- **Issue**: Web interface couldn't properly display sensor data due to nested JSON structure
- **Solution**: Changed API response from nested objects to flat structure
- **Before**: `{"aht20": {"available": true, ...}, "bmp280": {...}}`
- **After**: `{"aht20_available": true, "bmp280_available": true, "temperature_avg": 23.5, ...}`

### Sensor Status Reporting
- **Issue**: Unclear sensor status messages
- **Solution**: Implemented explicit status values: "OK", "Read error", "Not detected"
- **Benefit**: Users can now clearly identify sensor problems

## What's Added

### Translation Keys
Added missing French/English translation keys for UI elements:
- `refresh_gps` - "Refresh GPS" / "Rafraîchir GPS"
- `gps_module` - "GPS Module" / "Module GPS"  
- `gps_module_desc` - GPS module description
- `refresh_env_sensors` - "Refresh Sensors" / "Rafraîchir capteurs"
- `test_env_sensors` - "Test Sensors" / "Tester capteurs"

## Upgrade Notes

### From 3.18.0
- **Recommended**: This is a critical bug fix for environmental sensor readings
- **Breaking Changes**: None - fully backward compatible
- **Action Required**: Upload new firmware to fix incorrect sensor readings

### From Earlier Versions
- Review the [3.18.0 release notes](RELEASE_NOTES_3.18.0.md) for GPS and environmental sensors feature information

## Technical Details

### AHT20 Data Format
The AHT20 sensor returns 6 bytes:
```
Byte 0: Status byte
Bytes 1-2: Humidity bits [19:4]
Byte 3: Humidity bits [3:0] + Temperature bits [19:16]
Bytes 4-5: Temperature bits [15:0]
```

The corrected extraction algorithm:
```cpp
uint32_t humidity_raw = ((uint32_t)data[0] << 12) | ((uint32_t)data[1] << 4) | ((uint32_t)data[2] >> 4);
uint32_t temp_raw = (((uint32_t)data[2] & 0x0F) << 16) | ((uint32_t)data[3] << 8) | (uint32_t)data[4];
```

## Known Issues

None specific to this release. See the [FAQ](FAQ.md) for general troubleshooting.

## Next Steps

After upgrading:
1. Navigate to the "Sensors" page in the web interface
2. Verify that AHT20 and BMP280 sensors show "OK" status
3. Check that temperature and humidity values are now accurate
4. Test the "Refresh Sensors" and "Test Sensors" buttons

## Support

- Documentation: See [docs/](../docs/) folder
- Issues: Report on GitHub repository
- Compatibility: ESP32-S3 and ESP32 Classic boards

---

**Version**: 3.18.1  
**Build**: December 6, 2025  
**Semantic Versioning**: MAJOR.MINOR.PATCH (3.18.1)
