# ESP32 Diagnostic Suite – Release Notes v3.16.0

**Release Date:** November 28, 2025  
**Previous Version:** 3.15.1  
**Repository:** [github.com/morfredus/ESP32-Diagnostic](https://github.com/morfredus/ESP32-Diagnostic)

---

## Overview

Version 3.16.0 introduces comprehensive display configuration capabilities through the web interface, enhanced network monitoring, and improved diagnostic tools. This release allows dynamic configuration of OLED and TFT displays without code recompilation, making the system more flexible for various hardware setups.

---

## What's New

### 1. Network Monitoring Enhancement
- **Client IP Logging**: All client connections are now automatically logged in the Serial Monitor with their IP addresses
- **Format**: `[Client] <endpoint> connected from IP: <address>`
- **Benefits**: Easier troubleshooting of network connectivity and security monitoring

### 2. WiFi Status Display in Web Interface
- **Real-time Connection Info**: WiFi tab now displays current network connection status before the WiFi scanner
- **Information Displayed**:
  - Connection status (Connected/Disconnected) with color coding
  - Current SSID
  - Current IP address
  - Gateway address
  - DNS server address
  - Signal strength (RSSI in dBm)
- **Automatic Loading**: Information is automatically fetched from `/api/wifi-info` when the Wireless tab is opened
- **Live Updates**: Connection status is refreshed dynamically

### 3. OLED Configuration via Web Interface
- **Resolution Configuration**: Dynamic adjustment of OLED screen resolution (width × height)
- **Pin Configuration**: Existing SDA/SCL pin configuration enhanced with resolution support
- **Rotation Settings**: Combined with existing rotation controls for complete display configuration
- **API**: Enhanced `/api/oled-config` endpoint with new `width` and `height` parameters

### 4. TFT Configuration via Web Interface
- **Complete Pin Mapping**: Configure all TFT pins through the web interface:
  - MOSI (SPI Data)
  - SCLK (SPI Clock)
  - CS (Chip Select)
  - DC (Data/Command)
  - RST (Reset)
  - BL (Backlight)
- **Resolution Configuration**: Adjust TFT resolution (width × height)
- **Rotation Settings**: Set display rotation (0-3)
- **New API Endpoint**: `/api/tft-config` for comprehensive configuration with validation

### 5. Enhanced Screen Information API
- **Updated `/api/screens-info`**: Now returns complete configuration details for both displays:
  - OLED: SDA, SCL, rotation, width, height
  - TFT: MOSI, SCLK, CS, DC, RST, BL, width, height, rotation
- **Real-time Status**: Availability and status information for both displays

---

## Technical Improvements

### Code Structure
- Added runtime configuration variables: `oledWidth`, `oledHeight`, `tftMOSI`, `tftSCLK`, `tftCS`, `tftDC`, `tftRST`, `tftBL`, `tftWidth`, `tftHeight`, `tftRotation`
- Implemented `logClientConnection()` helper function for consistent IP logging
- Enhanced `handleOLEDConfig()` with resolution parameters
- Created new `handleTFTConfig()` handler for complete TFT configuration

### API Changes
- **New Endpoint**: `POST /api/tft-config` with parameters:
  - Required: `mosi`, `sclk`, `cs`, `dc`, `rst`
  - Optional: `bl`, `width`, `height`, `rotation`
- **Enhanced Endpoint**: `/api/oled-config` now accepts:
  - Existing: `sda`, `scl`, `rotation`
  - New: `width`, `height`
- **Updated Response**: `/api/screens-info` includes full configuration details

### Validation
- Pin range validation (0-48 for most pins, -1 allowed for RST and BL)
- Rotation validation (0-3)
- Real-time configuration updates with feedback

---

## Compatibility

### Hardware Support
- **ESP32-S3 N16R8**: Fully supported (16MB Flash + 8MB PSRAM)
- **ESP32-S3 N8R8**: Fully supported (8MB Flash + 8MB PSRAM)
- **ESP32 Classic**: Fully supported (4MB Flash, no PSRAM)

### Software Requirements
- **PlatformIO Core**: 6.0 or higher
- **ESP32 Arduino Core**: 3.3.3 (recommended)
- **Platform**: `espressif32@^6`

### Breaking Changes
**None.** This is a backward-compatible feature release.

---

## Usage Examples

### OLED Configuration with Resolution
```bash
# Configure OLED with custom resolution
curl -X GET "http://esp32-diagnostic.local/api/oled-config?sda=21&scl=20&rotation=0&width=128&height=64"
```

**Response:**
```json
{
  "success": true,
  "message": "I2C reconfigure: SDA:21 SCL:20 Rot:0 Res:128x64",
  "sda": 21,
  "scl": 20,
  "rotation": 0,
  "width": 128,
  "height": 64
}
```

### TFT Complete Configuration
```bash
# Configure TFT display completely
curl -X GET "http://esp32-diagnostic.local/api/tft-config?mosi=11&sclk=12&cs=10&dc=9&rst=7&bl=15&width=240&height=240&rotation=2"
```

**Response:**
```json
{
  "success": true,
  "message": "TFT config updated: MOSI:11 SCLK:12 CS:10 DC:9 RST:7 Res:240x240 Rot:2",
  "mosi": 11,
  "sclk": 12,
  "cs": 10,
  "dc": 9,
  "rst": 7,
  "bl": 15,
  "width": 240,
  "height": 240,
  "rotation": 2
}
```

### Query Screen Configuration
```bash
# Get current configuration of all screens
curl -X GET "http://esp32-diagnostic.local/api/screens-info"
```

**Response:**
```json
{
  "oled": {
    "available": true,
    "status": "OK",
    "pins": {"sda": 21, "scl": 20},
    "rotation": 0,
    "width": 128,
    "height": 64
  },
  "tft": {
    "available": true,
    "status": "Ready",
    "width": 240,
    "height": 240,
    "rotation": 2,
    "pins": {
      "mosi": 11,
      "sclk": 12,
      "cs": 10,
      "dc": 9,
      "rst": 7,
      "bl": 15
    }
  }
}
```

---

## Serial Monitor Output

### Client Connection Logging
```
[Client] Web Interface connected from IP: 192.168.1.105
[Client] JS Resource connected from IP: 192.168.1.105
[Client] Web Interface connected from IP: 192.168.1.142
```

---

## Migration Guide

### From 3.15.1 to 3.16.0

**No migration required.** Simply update the firmware:

1. Pull the latest code from GitHub
2. Compile with PlatformIO
3. Upload to your device

**Optional:** Use the new web interface features to configure your displays dynamically.

---

## Known Limitations

1. **TFT Reinitialization**: While configuration is saved, complete TFT reinitialization requires device reboot for hardware changes.
2. **Pin Conflicts**: No automatic validation of pin conflicts between different peripherals. Users must ensure selected pins don't conflict with other devices.
3. **OLED Types**: Resolution configuration assumes compatible I2C OLED display. Common sizes: 128×64, 128×32, 64×48.

---

## Future Enhancements

Planned features for upcoming releases:
- Persistent configuration storage (EEPROM/SPIFFS)
- Automatic pin conflict detection
- Hot-reload of TFT without reboot
- Display test suite for custom resolutions
- Multi-display support

---

## Documentation Updates

- **CHANGELOG.md**: Complete change history
- **CHANGELOG_FR.md**: Version française des modifications
- **README.md**: Updated feature list
- **README_FR.md**: Liste des fonctionnalités mise à jour
- **API_REFERENCE.md**: Updated with new endpoints (pending)
- **BUILD_AND_DEPLOY.md**: Version references updated

---

## Credits

**Author:** morfredus  
**Contributors:** Community feedback and testing  
**License:** [License Type]

---

## Support

- **Issues**: [GitHub Issues](https://github.com/morfredus/ESP32-Diagnostic/issues)
- **Discussions**: [GitHub Discussions](https://github.com/morfredus/ESP32-Diagnostic/discussions)
- **Documentation**: [docs/](https://github.com/morfredus/ESP32-Diagnostic/tree/main/docs)

---

**Thank you for using ESP32 Diagnostic Suite!**
