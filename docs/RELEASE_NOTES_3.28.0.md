# Release Notes - Version 3.28.0

**Release Date:** 2025-12-23
**Type:** Feature Release
**Branch:** dev/addfeatures

## Overview

Version 3.28.0 introduces critical bug fixes for JavaScript GPIO constants and completes the TFT MISO pin integration in the web interface. This release also adds comprehensive SD card testing endpoints and proper GPIO 13 sharing warnings for SPI devices.

## 🚀 New Features & Bug Fixes

### Fixed

#### BUTTON_BOOT JavaScript Error ✅
- **Issue:** `ReferenceError: BUTTON_BOOT is not defined` when accessing Input Devices page
- **Root Cause:** GPIO pin constants from `board_config.h` were not injected into JavaScript context
- **Solution:**
  - Injected all missing pin constants from `board_config.h` into JavaScript
  - Added: `ROTARY_CLK_PIN`, `ROTARY_DT_PIN`, `ROTARY_SW_PIN`
  - Added: `BUTTON_BOOT`, `BUTTON_1`, `BUTTON_2`
  - Added: `SD_MISO_PIN`, `SD_MOSI_PIN`, `SD_SCLK_PIN`, `SD_CS_PIN`
  - Added: `TFT_MISO_PIN`, `BUZZER_PIN`
- **Impact:** Input Devices page now loads without errors
- **Files Modified:** `include/web_interface.h` (lines 477-498)

#### BUTTON_BOOT Configuration ✅
- **Issue:** BUTTON_BOOT was editable in the UI, risking boot functionality
- **Solution:**
  - Changed from editable input to read-only display
  - Marked as "(non configurable)" in UI with visual indicator
  - Preserves native ESP32 boot button integrity (GPIO 0)
- **Impact:** Prevents accidental misconfiguration of critical boot button
- **Files Modified:** `include/web_interface.h` (line 98)

### Added

#### TFT MISO Pin Configuration ✅
- **Feature:** Complete MISO pin support in TFT SPI configuration
- **Display:** SPI Pins now show: `MISO`, `MOSI`, `SCLK`, `CS`, `DC`, `RST`
- **Configuration:** Added configurable MISO input field in TFT settings
- **Default Value:** GPIO 13 (from `board_config.h` for ESP32-S3)
- **Range:** -1 to 9 (limited in v3.28.0, expanded to -1 to 48 in v3.28.1)
- **Files Modified:** `include/web_interface.h` (line 90)

#### SD Card Test API Endpoints ✅
Three new REST endpoints for comprehensive SD card testing:

1. **`/api/sd-test-read`** - Test SD card read operations
   - Creates test file if needed
   - Reads and validates content
   - Returns success/failure status

2. **`/api/sd-test-write`** - Test SD card write operations
   - Writes timestamped data
   - Validates write success
   - Returns operation status

3. **`/api/sd-format`** - Clean SD card test files
   - Removes `/test_esp32.txt`, `/test_read.txt`, `/test_write.txt`
   - Safe cleanup (not low-level format)
   - Returns cleanup status

**Features:**
- Automatic SD initialization if unavailable
- JSON response format consistent with existing endpoints
- Proper error handling for missing SD cards
- Returns `"not available"` status if SD card not detected

**Files Modified:** `src/main.cpp` (lines 4185-4291, 5688-5693)

#### GPIO 13 Sharing Warning ✅
- **Feature:** Prominent warning about GPIO 13 shared usage
- **Location:** SD Card section in web interface
- **Styling:** Yellow warning box with Bootstrap-like alert styling
- **Content:** Alerts users that GPIO 13 is shared between TFT MISO and SD MISO
- **Languages:** Full bilingual support (EN/FR)
- **New Translation Keys:**
  - `gpio_shared_warning`: "Shared GPIO 13 (TFT + SD – MISO)"
  - `gpio_13_shared_desc`: Detailed explanation of SPI sharing requirements
- **Files Modified:**
  - `include/web_interface.h` (line 99)
  - `include/languages.h` (lines 486-487)

## Technical Details

### Web Interface Changes
**File:** `include/web_interface.h`

**GPIO Pin Injection** (lines 477-498):
```javascript
// Buzzer
js += F(";const BUZZER_PIN=");
js += String(BUZZER);

// Rotary Encoder
js += F(";const ROTARY_CLK_PIN=");
js += String(ROTARY_CLK);
js += F(";const ROTARY_DT_PIN=");
js += String(ROTARY_DT);
js += F(";const ROTARY_SW_PIN=");
js += String(ROTARY_SW);

// Buttons
js += F(";const BUTTON_BOOT=");
js += String(BUTTON_BOOT);
js += F(";const BUTTON_1=");
js += String(BUTTON_1);
js += F(";const BUTTON_2=");
js += String(BUTTON_2);

// SD Card
js += F(";const SD_MISO_PIN=");
js += String(SD_MISO);
// ... MOSI, SCLK, CS

// TFT
js += F(";const TFT_MISO_PIN=");
js += String(TFT_MISO);
```

**BUTTON_BOOT Read-Only Display** (line 98):
```javascript
h+='<div class="info-value">GPIO '+BUTTON_BOOT+' <span style="font-size:0.8em;color:#666">(non configurable)</span></div></div>';
```

**GPIO 13 Warning** (line 99):
```javascript
h+='<p style="margin-top:10px;padding:10px;background:#fff3cd;border-left:4px solid #ffc107;color:#856404;border-radius:4px"><strong>⚠️ '+tr('gpio_shared_warning')+'</strong><br>'+tr('gpio_13_shared_desc')+'</p>';
```

### API Implementation
**File:** `src/main.cpp`

**New Endpoint Handlers** (lines 4185-4291):
- `handleSDTestRead()`: Creates and reads test file, returns JSON status
- `handleSDTestWrite()`: Writes timestamped data, returns JSON status
- `handleSDFormat()`: Removes test files, returns JSON status

**Route Registration** (lines 5688-5693):
```cpp
server.on("/api/sd-test-read", handleSDTestRead);
server.on("/api/sd-test-write", handleSDTestWrite);
server.on("/api/sd-format", handleSDFormat);
```

### Translations
**File:** `include/languages.h` (lines 486-487)

```cpp
X(gpio_shared_warning, "Shared GPIO 13 (TFT + SD – MISO)", "GPIO 13 partagé (TFT + SD – MISO)") \
X(gpio_13_shared_desc, "GPIO 13 is shared between the TFT and SD Card (MISO line). Strictly compliant SPI wiring and proper software management are required.", "Le GPIO 13 est partagé entre le TFT et la carte SD (ligne MISO). Un câblage SPI strictement conforme et une gestion logicielle adaptée sont requis.") \
```

## Compliance & Architecture

### board_config.h Immutability ✅
- **Requirement:** `board_config.h` must remain untouched (single source of truth)
- **Compliance:** ✅ All GPIO values sourced from `board_config.h` constants
- **Validation:** ✅ No hardcoded pin values in JavaScript
- **Architecture:** ✅ JavaScript constants injected from C++ defines at runtime

### Code Quality
- Full bilingual support maintained (EN/FR)
- Consistent API response format
- Proper error handling
- Clean separation of concerns

## Known Limitations (Fixed in 3.28.1)

1. **MISO Backend Integration:** MISO field not yet included in `/api/screens-info` JSON response (shows as "undefined" in display)
2. **MISO Configuration:** MISO value not sent to backend when configuring TFT
3. **SD Card on ESP32-S3:** SD card initialization fails on ESP32-S3 due to HSPI/FSPI incompatibility

**Note:** All three issues above are resolved in version 3.28.1 (patch release)

## Files Modified

- `include/web_interface.h`: Pin injection, BOOT button display, GPIO warning, MISO field
- `include/languages.h`: Added 2 new translation keys
- `src/main.cpp`: Added 3 new SD card endpoint handlers + route registration
- `platformio.ini`: Version 3.27.2 → 3.28.0
- `CHANGELOG.md` + `CHANGELOG_FR.md`: Complete version 3.28.0 documentation

## Upgrade Notes

### From 3.27.x to 3.28.0
- No breaking changes
- No hardware modifications required
- Web interface improvements are automatic
- SD card tests require ESP32-S3 v3.28.1 for full functionality

### Testing Recommendations
1. Verify Input Devices page loads without errors
2. Confirm BUTTON_BOOT is read-only in UI
3. Test SD card endpoints (note: may show "not available" on ESP32-S3 until v3.28.1)
4. Verify GPIO 13 warning is displayed in SD Card section

## Next Steps

See version 3.28.1 release notes for:
- Complete MISO backend integration
- SD card functionality on ESP32-S3
- MISO input range expansion (48 GPIOs)

---

**Version:** 3.28.0
**Previous Version:** 3.27.2
**Next Version:** 3.28.1
