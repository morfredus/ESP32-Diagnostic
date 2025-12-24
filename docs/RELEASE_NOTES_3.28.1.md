# Release Notes - Version 3.28.1

**Release Date:** 2025-12-24
**Type:** Patch Release (Bug Fixes)
**Branch:** dev/addfeatures

## Overview

Version 3.28.1 is a critical patch release that resolves all remaining issues from version 3.28.0. This release completes the MISO backend integration, fixes SD card functionality on ESP32-S3, and expands the MISO input range to support all ESP32 GPIO pins.

## 🐛 Critical Bug Fixes

### Fixed Issues from 3.28.0

#### 1. TFT MISO Backend Integration ✅

**Problem:**
- MISO field missing from `/api/screens-info` JSON response
- Web UI displayed "MISO: undefined" instead of GPIO 13
- Backend variable `tftMISO` was not declared

**Root Cause:**
- Variable `tftMISO` was never initialized from `TFT_MISO` constant
- `handleScreensInfo()` function didn't include `miso` field in TFT pins JSON

**Solution:**
```cpp
// src/main.cpp:261 - Added variable declaration
int tftMISO = TFT_MISO;

// src/main.cpp:4568 - Updated JSON response
json += "\"pins\":{\"miso\":" + String(tftMISO) + ",\"mosi\":" + String(tftMOSI) + ...
```

**Impact:**
- ✅ MISO now displays correctly as "MISO: 13" (ESP32-S3)
- ✅ Value properly sourced from `board_config.h`
- ✅ No more "undefined" in web interface

**Files Modified:**
- `src/main.cpp` (line 261, 4568)

---

#### 2. TFT MISO Configuration Sync ✅

**Problem:**
- JavaScript `configTFT()` function didn't retrieve MISO value from input field
- Backend `handleTFTConfig()` didn't accept MISO parameter
- MISO changes in UI had no effect

**Root Cause:**
- Missing `miso` parameter in configuration API call
- Backend handler didn't process MISO value

**Solution:**
```javascript
// include/web_interface.h:119 - Updated configTFT()
const miso = document.getElementById('tftMISO').value;
// ... send to backend
await fetch('/api/tft-config?miso='+miso+'&mosi='+mosi+...);
```

```cpp
// src/main.cpp:3814-3828 - Updated handleTFTConfig()
int newMISO = server.hasArg("miso") ? server.arg("miso").toInt() : tftMISO;
// ... validate and apply
if (newMISO >= -1 && newMISO <= 48 ...) {
  tftMISO = newMISO;
  // ...
}
```

**Impact:**
- ✅ Complete MISO configuration flow: UI ↔ API ↔ Firmware
- ✅ MISO changes in web UI now persist
- ✅ Full validation (-1 to 48 range)

**Files Modified:**
- `include/web_interface.h` (line 119)
- `src/main.cpp` (lines 3814-3828, 3850-3866)

---

#### 3. SD Card Support on ESP32-S3 ✅

**Problem:**
- SD card initialization failed on ESP32-S3
- Tests returned "not available" even with SD card inserted
- Compilation or runtime errors on ESP32-S3

**Root Cause:**
- Code used `HSPI` constant which only exists on ESP32 classic
- ESP32-S2/S3 use different SPI bus naming (`FSPI` instead of `HSPI`)

**Solution:**
```cpp
// src/main.cpp:2950-2954 - Conditional SPI bus selection
if (sdSPI == nullptr) {
#if defined(CONFIG_IDF_TARGET_ESP32)
  sdSPI = new SPIClass(HSPI);  // ESP32 classic uses HSPI
#else
  sdSPI = new SPIClass(FSPI);  // ESP32-S2/S3 use FSPI (SPI2)
#endif
}
```

**Impact:**
- ✅ SD card fully functional on ESP32-S3 N16R8
- ✅ All SD test endpoints work correctly
- ✅ Proper ESP32 variant abstraction
- ✅ No compilation errors

**SPI Bus Reference:**
- **ESP32 Classic:** HSPI = Hardware SPI bus 2
- **ESP32-S2/S3:** FSPI = Flexible SPI bus (equivalent to SPI2)

**Files Modified:**
- `src/main.cpp` (lines 2950-2954)

---

## Technical Details

### Backend Changes

**File:** `src/main.cpp`

1. **Line 261:** Added `int tftMISO = TFT_MISO;` variable declaration
2. **Line 4568:** Added `miso` field to TFT pins JSON in `handleScreensInfo()`
3. **Lines 3814-3828:** Updated `handleTFTConfig()` to accept and validate MISO parameter
4. **Lines 2950-2954:** Added conditional SPI bus selection for SD card initialization

**JSON Response Example:**
```json
{
  "tft": {
    "available": true,
    "status": "Ready",
    "width": 240,
    "height": 240,
    "rotation": 0,
    "pins": {
      "miso": 13,
      "mosi": 11,
      "sclk": 12,
      "cs": 10,
      "dc": 9,
      "rst": 14,
      "bl": 7
    }
  }
}
```

### Frontend Changes

**File:** `include/web_interface.h`

**Line 119:** Updated `configTFT()` function
```javascript
async function configTFT() {
  setStatus('tft-status', {key:'reconfiguring'}, null);
  const miso = document.getElementById('tftMISO').value;  // NEW
  const mosi = document.getElementById('tftMOSI').value;
  // ... other fields
  const r = await fetch('/api/tft-config?miso='+miso+'&mosi='+mosi+...);
  // ... handle response
}
```

### Version Update

**File:** `platformio.ini`

```ini
build_flags =
    -std=gnu++17
    -D PROJECT_VERSION='"3.28.1"'
    -D PROJECT_NAME='"ESP32 Diagnostic"'
```

## Compliance & Architecture

### board_config.h Immutability ✅
- **Requirement:** `board_config.h` must remain untouched
- **Compliance:** ✅ All values sourced from `board_config.h` constants
- **Validation:** ✅ No hardcoded GPIO values
- **Status:** ✅ CONFIRMED - `board_config.h` not modified

### ESP32 Variant Support ✅
- **Requirement:** Support ESP32 Classic, ESP32-S2, ESP32-S3
- **Implementation:** ✅ Conditional compilation with `CONFIG_IDF_TARGET_*`
- **SPI Abstraction:** ✅ Proper bus selection per variant
- **Testing:** ✅ Verified on ESP32-S3 N16R8

## Testing Results

### Verified Functionality
✅ **MISO Display:** Shows "MISO: 13" correctly (no more "undefined")
✅ **MISO Configuration:** Changes in UI persist and affect firmware
✅ **SD Card Detection:** Properly detects and initializes on ESP32-S3
✅ **SD Card Tests:** Read/write/format endpoints functional
✅ **GPIO 13 Warning:** Displayed correctly in SD Card section
✅ **BUTTON_BOOT:** Read-only, no JavaScript errors
✅ **Input Range:** MISO accepts full GPIO range (-1 to 48)

### Test Platforms
- ✅ ESP32-S3 N16R8 (16MB Flash + 8MB PSRAM)
- Expected to work: ESP32-S3 N8R8, ESP32 Classic (conditional compilation)

## Upgrade Notes

### From 3.28.0 to 3.28.1
- **Breaking Changes:** None
- **Hardware Changes:** None required
- **Configuration:** Automatic - no user action needed
- **Testing:** Verify SD card functionality after upgrade

### Migration Steps
1. Upload new firmware via PlatformIO
2. Verify MISO display shows GPIO 13 (not "undefined")
3. Test SD card read/write operations
4. Confirm Input Devices page loads without errors

## Known Issues

None. All issues from 3.28.0 are resolved.

## Files Modified

- `src/main.cpp`: MISO variable, JSON response, config handler, SD SPI bus
- `include/web_interface.h`: configTFT MISO parameter
- `platformio.ini`: Version 3.28.0 → 3.28.1
- `CHANGELOG.md` + `CHANGELOG_FR.md`: Complete 3.28.1 documentation

## Semantic Versioning Justification

**Version:** 3.28.1 (PATCH)
- **MAJOR:** No breaking changes → 3.x unchanged
- **MINOR:** No new features → .28 unchanged
- **PATCH:** Bug fixes only → .1 incremented

**Changes:**
- ✅ Fixed MISO undefined display (bug fix)
- ✅ Fixed MISO configuration sync (bug fix)
- ✅ Fixed SD card on ESP32-S3 (bug fix)

**Conclusion:** Strict PATCH version per SemVer 2.0.0

---

**Version:** 3.28.1
**Previous Version:** 3.28.0
**Next Version:** TBD
**Status:** ✅ STABLE - All known issues resolved
