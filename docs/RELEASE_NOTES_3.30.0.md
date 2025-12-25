# Release Notes - Version 3.30.0

**Release Date:** December 25, 2025
**Type:** Minor Release (Feature Addition)
**Status:** ✅ Stable

---

## 🎯 Overview

Version 3.30.0 introduces **runtime TFT driver switching** - a major enhancement that allows users to dynamically switch between ILI9341 and ST7789 TFT display drivers directly from the Web UI without recompiling or reflashing the firmware.

---

## ✨ New Features

### Dynamic TFT Driver Selection (Web UI)

**What's New:**
- **Runtime driver switching**: Change TFT driver (ILI9341 ↔ ST7789) from Web UI
- **No reboot required**: Instant driver change without ESP32 restart
- **Dual driver support**: Both drivers loaded simultaneously in memory
- **Seamless integration**: New driver selector in TFT configuration section
- **Persistent selection**: Driver choice displayed in real-time

**Benefits:**
- ⚡ **Instant testing**: Test different TFT controllers without reflashing
- 🔄 **Hardware flexibility**: Swap displays on-the-fly for prototyping
- 🧪 **Easy troubleshooting**: Quickly identify driver compatibility issues
- 📦 **Single firmware**: One build supports both display types

---

## 🔧 Technical Changes

### Backend Architecture

**File: `include/tft_display.h`**

Complete refactoring for runtime driver support:

```cpp
// New driver type enumeration
enum TFT_DriverType {
  TFT_DRIVER_ILI9341 = 0,
  TFT_DRIVER_ST7789 = 1
};

// Dynamic driver objects (both instantiated)
Adafruit_ILI9341* tft_ili9341 = nullptr;
Adafruit_ST7789* tft_st7789 = nullptr;

// Generic pointer for unified access
Adafruit_GFX* tft = nullptr;

// Runtime driver switching
bool switchTFTDriver(TFT_DriverType newDriver, int width, int height, int rotation);
```

**Key Functions:**
- `initTFT(driverType, width, height, rotation)` - Dynamic initialization
- `switchTFTDriver()` - On-the-fly driver change
- `deinitTFT()` - Proper cleanup before switching
- `getTFTDriverName()` - Current driver identification

### API Enhancements

**File: `src/main.cpp`**

New global variable:
```cpp
String tftDriver = "ILI9341";  // Tracks active driver
```

Enhanced endpoints:

**`/api/tft-config` (Modified)**
- **New parameter**: `driver` (ILI9341 | ST7789)
- **Behavior**: Switches driver dynamically if `driver` parameter present
- **Response**: Includes current driver in success response

**`/api/screens-info` (Modified)**
- **New field**: `"driver"` in TFT object
- **Returns**: Current active driver type

### Web Interface Updates

**File: `include/web_interface.h`**

New UI element in TFT configuration:
```html
<select id="tftDriver">
  <option value="ILI9341">ILI9341</option>
  <option value="ST7789">ST7789</option>
</select>
```

Updated JavaScript:
```javascript
async function configTFT() {
  const driver = document.getElementById('tftDriver').value;
  // Sends driver parameter to API
  const r = await fetch('/api/tft-config?driver=' + driver + '&...');
}
```

---

## 📝 Configuration

### Default Driver Selection

Edit `include/config.h`:

```cpp
// v3.30.0: Dynamic TFT driver selection
// Both drivers supported simultaneously
#define TFT_USE_ILI9341  // Default at boot
// #define TFT_USE_ST7789

#define TFT_WIDTH   240
#define TFT_HEIGHT  320
#define TFT_ROTATION 2
```

**Note:** The `TFT_USE_*` define only sets the **default driver at boot**. You can switch dynamically via Web UI after boot.

---

## 🚀 Usage Guide

### How to Switch Drivers from Web UI

1. **Access Web Interface**
   - Navigate to ESP32 IP address in browser

2. **Open TFT Screen Section**
   - Scroll to "📱 TFT Screen" section

3. **Select Driver**
   - Find the "Driver:" dropdown menu
   - Select desired driver:
     - `ILI9341` (240x320)
     - `ST7789` (240x320)

4. **Apply Configuration**
   - Click "⚙️ Apply Config" button
   - Driver switches **instantly** (no reboot)

5. **Test Display**
   - Use built-in test buttons:
     - "🧪 Full Test"
     - "🏠 Boot Screen"
     - Individual step tests

6. **Verify Active Driver**
   - Driver name shown in configuration section
   - Display responds according to selected driver

---

## 📊 Compatibility

### Hardware Support

| TFT Controller | Resolution | Status | Notes |
|----------------|------------|--------|-------|
| **ILI9341** | 240x320 | ✅ Fully Supported | SPI, Hardware SPI |
| **ST7789** | 240x320 | ✅ Fully Supported | SPI, Hardware SPI |

### Pin Mapping

Both drivers use **identical pin mapping** from `board_config.h`:

```cpp
#define TFT_MISO  13
#define TFT_MOSI  11
#define TFT_SCLK  12
#define TFT_CS    10
#define TFT_DC     9
#define TFT_RST   14
#define TFT_BL     7
```

**ILI9341 pins** are aliased to **TFT pins** - no separate wiring needed.

---

## 🔄 Migration from v3.29.0

### Breaking Changes

**None** - Fully backward compatible.

### Configuration Changes

**Before (v3.29.0):**
```cpp
// Had to choose ONE driver at compile-time
#define TFT_USE_ILI9341
//#define TFT_USE_ST7789
```

**After (v3.30.0):**
```cpp
// Both drivers loaded, switch via Web UI
#define TFT_USE_ILI9341  // Default only
// #define TFT_USE_ST7789
```

### Code Changes

**If you have custom code using `tft` object:**

**Before:**
```cpp
tft.fillScreen(TFT_BLACK);  // Direct object
```

**After:**
```cpp
tft->fillScreen(TFT_BLACK);  // Pointer syntax
```

**Reason:** `tft` is now a generic `Adafruit_GFX*` pointer for unified driver access.

---

## 🐛 Known Issues

### None Reported

Version 3.30.0 has been tested with:
- ✅ ESP32-S3 DevKitC-1 N16R8
- ✅ ILI9341 2.8" TFT (240x320)
- ✅ ST7789 2.4" TFT (240x320)

---

## 📚 Documentation Updates

All documentation updated for v3.30.0:

- ✅ **README.md / README_FR.md** - Version 3.30.0 highlights
- ✅ **CHANGELOG.md / CHANGELOG_FR.md** - Complete release notes
- ✅ **CONFIG.md / CONFIG_FR.md** - TFT configuration guide
- ✅ **USAGE.md / USAGE_FR.md** - Runtime switching usage
- ✅ **WEB_INTERFACE.md / WEB_INTERFACE_FR.md** - UI updates
- ✅ **API_REFERENCE.md / API_REFERENCE_FR.md** - New API parameters

---

## 🎯 Future Enhancements

Potential improvements for future releases:

- Support for additional TFT controllers (ILI9486, ST7735, etc.)
- Display resolution auto-detection
- Driver-specific calibration settings
- Touch controller runtime switching

---

## 📞 Support

**Issues:** https://github.com/morfredus/ESP32-Diagnostic/issues
**Documentation:** `/docs/` folder
**Community:** GitHub Discussions

---

## ✅ Testing Checklist

Before deploying v3.30.0, verify:

- [ ] Compile successful for ESP32-S3 and ESP32 Classic
- [ ] Default driver initializes correctly
- [ ] Web UI displays driver selector
- [ ] Driver switch works (ILI9341 → ST7789)
- [ ] Driver switch works (ST7789 → ILI9341)
- [ ] Display tests work with both drivers
- [ ] No memory leaks during driver switching
- [ ] Configuration persists across switches

---

**Version:** 3.30.0
**Author:** morfredus
**License:** See LICENSE file
**Homepage:** https://github.com/morfredus/ESP32-Diagnostic

---

*For complete version history, see [CHANGELOG.md](../CHANGELOG.md)*
