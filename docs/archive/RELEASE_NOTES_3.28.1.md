# ESP32 Diagnostic Suite – Release Notes v3.28.1

**Release Date:** December 24, 2025
**Type:** Patch Release
**Severity:** Critical Bug Fixes

---

## 📋 Overview

Version 3.28.1 is a **patch release** that addresses three critical integration issues: TFT MISO backend integration, MISO configuration synchronization, and SD card support on ESP32-S3. This version ensures complete MISO pin handling across the full stack and resolves SPI bus compatibility issues on ESP32-S3 boards.

**Who should upgrade:**
- ✅ Users experiencing "MISO: undefined" in the web UI TFT section
- ✅ Users unable to configure TFT MISO pin via web interface
- ✅ ESP32-S3 users experiencing SD card initialization failures
- ✅ All users on v3.28.0 (partial MISO implementation)

**IMPORTANT NOTE:**
- ⚠️ The BUTTON_BOOT JavaScript error was NOT fully fixed in this version despite initial claims
- ✅ See v3.28.2 release notes for the actual BUTTON_BOOT fix

---

## 🐛 Fixed Issues

### 1. TFT MISO Backend Integration ✅

#### Problem Description
The TFT MISO pin value was not included in the `/api/screens-info` JSON response, causing the web UI to display "MISO: undefined" in the TFT pin information section.

#### Root Cause
- The backend lacked the `tftMISO` variable initialization
- The JSON response builder in `handleScreensInfo()` did not include the MISO field
- Frontend received incomplete pin data: `{pins: {mosi: 11, sclk: 12, ...}}` (missing `miso`)

#### Technical Solution
**File:** `src/main.cpp`
**Changes:**
- **Line 261:** Added `int tftMISO = TFT_MISO;` variable declaration
- **Line 4568:** Added `miso` field to TFT pins JSON structure

```cpp
// Variable initialization
int tftMISO = TFT_MISO;

// JSON response in handleScreensInfo()
"pins": {
  "miso": 13,    // ← Added in v3.28.1
  "mosi": 11,
  "sclk": 12,
  "cs": 10,
  "dc": 9,
  "rst": 14
}
```

#### Impact & Benefits
- ✅ TFT MISO pin now displays correctly in web UI (GPIO 13 for ESP32-S3)
- ✅ Complete SPI pin information available: MISO, MOSI, SCLK, CS, DC, RST
- ✅ Resolves "MISO: undefined" display issue

---

### 2. TFT MISO Configuration Sync ✅

#### Problem Description
The `configTFT()` JavaScript function did not send the MISO pin value to the backend API, preventing users from configuring the TFT MISO pin via the web interface.

#### Root Cause
- Frontend `configTFT()` function retrieved all SPI pins EXCEPT MISO
- Backend `handleTFTConfig()` did not accept or validate MISO parameter
- Configuration flow was incomplete: UI → API (missing MISO parameter)

#### Technical Solution
**File:** `include/web_interface.h`
**Line:** 119
```javascript
// Updated configTFT() to include MISO value
const miso = document.getElementById('tftMISO').value;
// API call now includes MISO parameter
fetch(`/api/tft-config?miso=${miso}&mosi=${mosi}&sclk=${sclk}&...`)
```

**File:** `src/main.cpp`
**Lines:** 3814-3828
```cpp
// Updated handleTFTConfig() to accept and validate MISO parameter
if (request->hasParam("miso")) {
  int miso = request->getParam("miso")->value().toInt();
  // Validate and process MISO value
}
```

#### Impact & Benefits
- ✅ MISO parameter properly included in `/api/tft-config` requests
- ✅ Backend now accepts and validates MISO configuration
- ✅ Completes full MISO configuration flow: UI ↔ API ↔ Firmware
- ✅ Users can now configure all TFT SPI pins via web interface

---

### 3. SD Card Support on ESP32-S3 ✅

#### Problem Description
SD card initialization failed on ESP32-S3 boards with compilation/runtime errors. The SD card test features were completely non-functional on ESP32-S3 N16R8 and N8R8 variants.

#### Root Cause
The code used the `HSPI` constant for SPI bus selection, which is **only available on ESP32 Classic**:

```cpp
// INCORRECT - Fails on ESP32-S3
SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
if (!SD.begin(SD_CS, SPI, 25000000, "/sd", 5, false)) {
  // Compilation error on ESP32-S3: HSPI not defined
}
```

ESP32 architecture differences:
- **ESP32 Classic:** Has `HSPI` (Hardware SPI bus 2) and `VSPI` (bus 3)
- **ESP32-S2/S3:** Uses `FSPI` (Flexible SPI, equivalent to SPI2) instead of HSPI/VSPI

#### Technical Solution
**File:** `src/main.cpp`
**Lines:** 2950-2954

Implemented conditional SPI bus selection:

```cpp
#if defined(CONFIG_IDF_TARGET_ESP32)
  // ESP32 Classic: Use HSPI (Hardware SPI bus 2)
  SPIClass spiSD(HSPI);
#else
  // ESP32-S2/S3: Use FSPI (Flexible SPI bus, equivalent to SPI2)
  SPIClass spiSD(FSPI);
#endif

spiSD.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
if (!SD.begin(SD_CS, spiSD, 25000000, "/sd", 5, false)) {
  // Now works on all ESP32 variants
}
```

#### Impact & Benefits
- ✅ SD card initialization now works on ESP32-S3 N16R8/N8R8
- ✅ SD card tests (`/api/sd-test-read`, `/api/sd-test-write`, `/api/sd-format`) fully functional
- ✅ Proper SPI bus abstraction for ESP32 variant compatibility
- ✅ No compilation errors on ESP32-S2/S3 boards

---

## 📦 Files Modified

| File | Changes | Lines |
|------|---------|-------|
| `src/main.cpp` | Added tftMISO variable declaration | 261 |
| `src/main.cpp` | Added MISO field to TFT pins JSON response | 4568 |
| `src/main.cpp` | Updated handleTFTConfig() to accept MISO parameter | 3814-3828 |
| `src/main.cpp` | Added conditional SPI bus selection for SD card | 2950-2954 |
| `include/web_interface.h` | Updated configTFT() to include MISO parameter | 119 |
| `platformio.ini` | Version bump: 3.28.0 → 3.28.1 | 27 |

---

## ✅ Testing & Verification

### How to Verify the Fixes

**Test 1: TFT MISO Display**
1. Flash firmware v3.28.1 to your ESP32 board
2. Connect to the web interface via http://esp32.local
3. Navigate to "Screens" page
4. **Verify** TFT MISO field displays "GPIO 13" (ESP32-S3) instead of "undefined"

**Test 2: TFT MISO Configuration**
1. Navigate to "Screens" → TFT Configuration section
2. Modify the MISO pin value in the input field
3. Click "Configure TFT"
4. **Verify** configuration request succeeds without errors
5. **Check** browser console (F12) for successful API response

**Test 3: SD Card on ESP32-S3**
1. Insert SD card into ESP32-S3 board
2. Navigate to "Storage" → SD Card section
3. Click "Test Read" button
4. **Verify** operation succeeds with JSON response: `{"status":"success",...}`
5. Click "Test Write" button
6. **Verify** write operation succeeds with timestamped data

### Expected Results
✅ TFT MISO displays "GPIO 13" (not "undefined")
✅ TFT configuration accepts MISO parameter without errors
✅ SD card tests work on ESP32-S3 N16R8/N8R8 boards
✅ No compilation errors when building for ESP32-S3 target
✅ Browser console shows successful API responses

---

## 📌 Compatibility

### Supported Boards
- ✅ ESP32-S3 DevKitC-1 N16R8 (16MB Flash, 8MB PSRAM)
- ✅ ESP32-S3 DevKitC-1 N8R8 (8MB Flash, 8MB PSRAM)
- ✅ ESP32 Classic DevKitC (4MB Flash)

### Requirements
- **PlatformIO:** espressif32@^6
- **ESP32 Arduino Core:** 3.0.0+
- **Browser:** Chrome 90+, Firefox 88+, Edge 90+, Safari 14+

### Breaking Changes
❌ None – This is a bug fix release, fully backward compatible

---

## 🔄 Migration Guide

### Upgrading from v3.28.0

**Step 1:** Pull latest code
```bash
git pull origin dev/addfeatures
```

**Step 2:** Clean build (recommended)
```bash
pio run -t clean
pio run -e esp32s3_n16r8
```

**Step 3:** Upload to ESP32
```bash
pio run -e esp32s3_n16r8 --target upload
```

**Step 4:** Verify web interface
- Navigate to http://esp32.local
- Check TFT MISO displays GPIO value (not "undefined")
- Test SD card read/write operations (ESP32-S3 users)

### Upgrading from v3.27.x or earlier

Follow the standard upgrade procedure:
1. Review `docs/RELEASE_NOTES_3.28.0.md` for v3.28.0 changes
2. Review this document for v3.28.1 changes
3. Note: BUTTON_BOOT error still present – see v3.28.2 for fix
4. Flash firmware using PlatformIO

---

## 🔗 Related Documentation

- [CHANGELOG.md](../CHANGELOG.md) – Complete version history
- [RELEASE_NOTES_3.28.0.md](RELEASE_NOTES_3.28.0.md) – v3.28.0 TFT MISO initial implementation
- [RELEASE_NOTES_3.28.2.md](RELEASE_NOTES_3.28.2.md) – v3.28.2 BUTTON_BOOT actual fix
- [TROUBLESHOOTING.md](TROUBLESHOOTING.md) – Common issues and solutions
- [PIN_MAPPING.md](PIN_MAPPING.md) – GPIO pin assignments

---

## ❓ FAQ

### Q: Does this fix the BUTTON_BOOT JavaScript error?
**A:** No, the BUTTON_BOOT error was NOT fully fixed in v3.28.1. Upgrade to v3.28.2 for the actual fix.

### Q: Why was SD card broken on ESP32-S3?
**A:** ESP32-S3 uses `FSPI` instead of `HSPI` for SPI bus 2. The code was hardcoded for ESP32 Classic's `HSPI` constant.

### Q: Will this affect my existing TFT configuration?
**A:** No, this is backward compatible. Existing configurations will continue to work, with MISO now properly displayed and configurable.

### Q: What if MISO still shows "undefined" after upgrading?
**A:** Clear your browser cache (Ctrl+Shift+Delete) and reload the page. If the issue persists, check the browser console for API errors.

### Q: Does this fix apply to both ESP32-S3 and ESP32 Classic?
**A:** Yes, all three fixes apply to all supported board variants. The SD card fix specifically addresses ESP32-S3 compatibility.

---

## 🔍 Compliance

### board_config.h Immutability
- ✅ All GPIO values sourced from `board_config.h` constants
- ✅ No hardcoded pin values in implementation
- ✅ Maintains `board_config.h` as single source of truth
- ✅ No modifications to `board_config.h` itself (as required)

### SPI Bus Abstraction
- ✅ Proper conditional compilation for ESP32 variants
- ✅ Uses `CONFIG_IDF_TARGET_ESP32` for platform detection
- ✅ Abstraction layer for HSPI/FSPI differences

---

## 📝 Credits

**Reported by:** Community testing, web UI integration testing
**Fixed by:** ESP32-Diagnostic development team
**Tested on:** ESP32-S3 DevKitC-1 N16R8, ESP32-S3 N8R8, ESP32 Classic DevKitC

---

## 🔖 Version Information

- **Current Version:** 3.28.1
- **Previous Version:** 3.28.0
- **Next Version:** 3.28.2 (BUTTON_BOOT actual fix)
- **Release Branch:** `dev/addfeatures`
- **Tag:** `v3.28.1`

---

**For complete changelog, see [CHANGELOG.md](../CHANGELOG.md)**
