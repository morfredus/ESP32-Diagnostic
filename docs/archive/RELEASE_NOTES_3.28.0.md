# ESP32 Diagnostic Suite – Release Notes v3.28.0

**Release Date:** December 23, 2025
**Type:** Feature Release + Bug Fixes
**Severity:** Major Improvements

---

## 📋 Overview

Version 3.28.0 is a **major feature release** that adds comprehensive TFT MISO pin configuration, SD card testing API endpoints, GPIO sharing warnings, and attempts to fix the BUTTON_BOOT JavaScript error. This version significantly enhances the web interface capabilities for TFT and SD card management.

**Who should upgrade:**
- ✅ Users wanting to configure TFT MISO pin via web interface
- ✅ Users needing SD card read/write testing functionality
- ✅ Users experiencing BUTTON_BOOT JavaScript errors (partial fix)
- ✅ All users on v3.27.x seeking enhanced TFT/SD card features

**IMPORTANT NOTE:**
- ⚠️ The BUTTON_BOOT JavaScript error was NOT fully fixed in this version
- ⚠️ The fix was applied in the wrong location (`web_interface.h` instead of `main.cpp::handleJavaScriptRoute()`)
- ✅ See v3.28.2 release notes for the actual complete fix

---

## 🐛 Fixed Issues

### BUTTON_BOOT JavaScript ReferenceError ⚠️ (PARTIAL FIX)

#### Problem Description
When navigating to the "Input Devices" page in the web interface, the browser console displayed:
```
ReferenceError: BUTTON_BOOT is not defined
```

This error prevented the Input Devices page from loading correctly and made BUTTON_BOOT, BUTTON_1, and BUTTON_2 controls non-functional.

#### Solution Attempt (v3.28.0)
**File:** `include/web_interface.h`

Injected all missing GPIO pin constants from `board_config.h` into the JavaScript generation:

```javascript
// Added pin constants injection
const ROTARY_CLK_PIN = 47;   // ESP32-S3
const ROTARY_DT_PIN = 45;
const ROTARY_SW_PIN = 40;
const BUTTON_BOOT = 0;
const BUTTON_1 = 38;
const BUTTON_2 = 39;
const SD_MISO_PIN = 13;
const SD_MOSI_PIN = 11;
const SD_SCLK_PIN = 12;
const SD_CS_PIN = 1;
const TFT_MISO_PIN = 13;
```

**BUTTON_BOOT Configuration:**
- Changed BUTTON_BOOT from editable input to **read-only display**
- Marked as "(non configurable)" in UI
- Preserves native ESP32 boot button integrity (GPIO 0)

#### Why This Fix Was Incomplete
**Root Cause (discovered in v3.28.2):**
- Constants were injected in `web_interface.h::generateJavaScript()`
- **BUT** this function is only called to calculate JavaScript size for statistics
- The actual JavaScript served to the browser comes from `main.cpp::handleJavaScriptRoute()`
- Therefore, the injected constants **never reached the browser**

**Impact:**
- ⚠️ JavaScript error persisted despite fix attempt
- ✅ BUTTON_BOOT made read-only (correct change)
- ❌ Pin constants still unavailable in browser JavaScript

**Actual Fix:** See v3.28.2 release notes for the complete solution

---

## 🚀 New Features

### 1. TFT MISO Pin Configuration ✅

#### Overview
Added complete MISO pin configuration support in the TFT section, allowing users to view and configure the MISO pin alongside other SPI pins.

#### Implementation
**File:** `include/web_interface.h`

**Display Enhancement:**
- Added MISO to SPI pins display: `MISO, MOSI, SCLK, CS, DC, RST`
- MISO pin sourced from `board_config.h` (GPIO 13 for ESP32-S3)
- Complete 6-pin SPI interface visibility

**Configuration Enhancement:**
- Added configurable MISO input field in TFT configuration section
- Users can now configure all TFT SPI pins via web interface
- Input validation and error handling included

#### Benefits
- ✅ Complete SPI pin visibility in web UI
- ✅ MISO pin properly reflects `board_config.h` values
- ✅ Users can verify correct MISO wiring (GPIO 13)
- ⚠️ Configuration functionality completed in v3.28.1 (backend integration)

---

### 2. SD Card Test API Endpoints ✅

#### Overview
Added three new API endpoints for comprehensive SD card testing, allowing users to verify read/write operations and clean up test files directly from the web interface.

#### API Endpoints

**`/api/sd-test-read` – Test SD Card Read**
```json
// Success response
{
  "status": "success",
  "message": "SD card read test successful",
  "file": "/test_read.txt",
  "content": "Test data..."
}

// Error response
{
  "status": "error",
  "message": "SD card not available"
}
```

**`/api/sd-test-write` – Test SD Card Write**
```json
// Success response
{
  "status": "success",
  "message": "SD card write test successful",
  "file": "/test_write.txt",
  "timestamp": "2025-12-23 14:30:45"
}
```

**`/api/sd-format` – Clean SD Card Test Files**
```json
// Success response
{
  "status": "success",
  "message": "SD card test files cleaned",
  "files_removed": ["/test_read.txt", "/test_write.txt"]
}
```

#### Implementation Details
**File:** `src/main.cpp`

**Handlers Added:**
- `handleSDTestRead()`: Creates test file if needed, tests read capability
- `handleSDTestWrite()`: Tests write capability with timestamped data
- `handleSDFormat()`: Removes all test files (`/test_*.txt`)

**Features:**
- Automatic SD initialization if card unavailable
- Proper error handling for all failure scenarios
- JSON response format consistent with existing endpoints
- Safe cleanup (removes only test files, not low-level format)

#### Use Cases
- ✅ Verify SD card is properly detected and initialized
- ✅ Test read/write permissions and functionality
- ✅ Benchmark SD card performance
- ✅ Troubleshoot SD card wiring issues
- ✅ Clean up test files after diagnostics

---

### 3. GPIO 13 Sharing Warning ✅

#### Overview
Added a visible warning in the SD Card section to alert users that GPIO 13 is shared between the TFT display and SD card (MISO line), emphasizing the need for proper SPI wiring and software management.

#### Implementation
**File:** `include/web_interface.h`

**Visual Warning (Yellow Alert Box):**
```html
⚠️ Shared GPIO 13 (TFT + SD – MISO)
GPIO 13 is shared between TFT and SD card (MISO line).
Strict SPI wiring and proper software management required.
```

**Styling:**
- Bootstrap-like alert styling (yellow warning background)
- Prominent placement in SD Card section
- Clear, concise warning message

**Translations Added:**
**File:** `include/languages.h`

```cpp
// English
"gpio_shared_warning": "Shared GPIO 13 (TFT + SD – MISO)"
"gpio_13_shared_desc": "GPIO 13 is shared between TFT and SD card (MISO line). Strict SPI wiring and proper software management required."

// French
"gpio_shared_warning": "GPIO 13 partagé (TFT + SD – MISO)"
"gpio_13_shared_desc": "GPIO 13 est partagé entre TFT et carte SD (ligne MISO). Câblage SPI strict et gestion logicielle appropriée requis."
```

#### Why This Matters
**Hardware Consideration:**
- Both TFT and SD card use GPIO 13 as MISO (Master In Slave Out)
- MISO is a **shared input line** from both devices to ESP32
- Requires proper SPI bus management to avoid conflicts

**Best Practices:**
- ✅ Use proper SPI wiring (separate CS lines for TFT and SD)
- ✅ Ensure only one device active at a time (CS control)
- ✅ Verify 3.3V logic levels for both devices
- ⚠️ Avoid simultaneous TFT rendering and SD card access

---

## 📦 Files Modified

| File | Changes | Description |
|------|---------|-------------|
| `include/web_interface.h` | Pin injection, BOOT display, GPIO warning, MISO field | JavaScript pin constants, read-only BOOT button, GPIO 13 warning box, TFT MISO configuration field |
| `include/languages.h` | Added 2 translation keys | `gpio_shared_warning`, `gpio_13_shared_desc` (EN/FR) |
| `src/main.cpp` | Added 3 SD card endpoint handlers + routes | `handleSDTestRead()`, `handleSDTestWrite()`, `handleSDFormat()` + route registration |
| `platformio.ini` | Version bump: 3.27.2 → 3.28.0 | Project version update |

---

## ✅ Testing & Verification

### How to Verify the Features

**Test 1: TFT MISO Display**
1. Flash firmware v3.28.0 to your ESP32 board
2. Connect to the web interface via http://esp32.local
3. Navigate to "Screens" page
4. **Verify** TFT pins display shows: MISO, MOSI, SCLK, CS, DC, RST
5. **Check** MISO value is GPIO 13 (ESP32-S3) or correct value for your board

**Test 2: SD Card Testing**
1. Insert SD card into ESP32 board
2. Navigate to "Storage" → SD Card section
3. **Verify** GPIO 13 warning is visible (yellow alert box)
4. Click "Test Read" button
5. **Verify** JSON response: `{"status":"success",...}`
6. Click "Test Write" button
7. **Verify** write operation succeeds with timestamp
8. Click "Format" button
9. **Verify** test files are cleaned up

**Test 3: BUTTON_BOOT Display**
1. Navigate to "Input Devices" page
2. **Check** BUTTON_BOOT field is read-only (not editable)
3. **Verify** displays "GPIO 0 (non configurable)"
4. ⚠️ **Note:** JavaScript errors may still occur (incomplete fix)

### Expected Results
✅ TFT MISO pin displays correctly in web UI
✅ SD card test endpoints return successful JSON responses
✅ GPIO 13 warning visible in SD Card section
✅ BUTTON_BOOT is read-only and marked non-configurable
⚠️ JavaScript console may still show ReferenceError (see v3.28.2 for fix)

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
- **SD Card:** FAT32 formatted (optional, for SD card tests)

### Breaking Changes
❌ None – This is a feature release, fully backward compatible

---

## 🔄 Migration Guide

### Upgrading from v3.27.x

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

**Step 4:** Verify new features
- Navigate to http://esp32.local
- Check TFT MISO pin display (Screens page)
- Test SD card operations (Storage page)
- Verify BUTTON_BOOT is read-only (Input Devices page)

**Step 5 (Optional):** Test SD card
- Insert SD card (FAT32 formatted)
- Run read/write tests via web interface
- Verify GPIO 13 warning is visible

### Upgrading from v3.26.x or earlier

Follow the standard upgrade procedure:
1. Review previous release notes for intermediate versions
2. Flash firmware using PlatformIO
3. Clear browser cache to ensure latest web interface loads
4. Test all new features (TFT MISO, SD card, BUTTON_BOOT)

---

## 🔗 Related Documentation

- [CHANGELOG.md](../CHANGELOG.md) – Complete version history
- [RELEASE_NOTES_3.28.1.md](RELEASE_NOTES_3.28.1.md) – v3.28.1 MISO backend completion
- [RELEASE_NOTES_3.28.2.md](RELEASE_NOTES_3.28.2.md) – v3.28.2 BUTTON_BOOT actual fix
- [PIN_MAPPING.md](PIN_MAPPING.md) – GPIO pin assignments
- [TROUBLESHOOTING.md](TROUBLESHOOTING.md) – Common issues and solutions

---

## ❓ FAQ

### Q: Does this version fully fix the BUTTON_BOOT error?
**A:** No, the fix was incomplete. The constants were injected in the wrong location. Upgrade to v3.28.2 for the actual fix.

### Q: Why is BUTTON_BOOT read-only?
**A:** BUTTON_BOOT is the native ESP32 boot button (GPIO 0), which is critical for firmware flashing and boot mode selection. Making it read-only prevents accidental misconfiguration.

### Q: What does the GPIO 13 warning mean?
**A:** GPIO 13 serves as the MISO line for both TFT and SD card. This is normal and safe, but requires proper SPI wiring (separate CS lines) to avoid conflicts.

### Q: Will the SD card format endpoint erase all my data?
**A:** No, `/api/sd-format` only removes test files created by the diagnostic suite (`/test_*.txt`). It does NOT perform a low-level format or erase user data.

### Q: Can I use the SD card while the TFT is active?
**A:** Yes, but not simultaneously. The firmware manages SPI bus access using separate CS (Chip Select) lines. The shared MISO line is properly handled by the SPI protocol.

### Q: Why do I still see JavaScript errors after upgrading?
**A:** The BUTTON_BOOT fix in v3.28.0 was incomplete. Upgrade to v3.28.2 for the complete fix, then clear your browser cache.

---

## 🔍 Compliance

### board_config.h Immutability
- ✅ All GPIO definitions sourced from `board_config.h` constants
- ✅ No hardcoded pin values in JavaScript or C++ implementation
- ✅ Respects `board_config.h` as single source of truth
- ✅ No modifications to `board_config.h` itself (as required)

### Pin Constants Injection
- ⚠️ Injection location incorrect (web_interface.h instead of main.cpp)
- ✅ All constants sourced from immutable board_config.h
- ✅ No hardcoded values used
- ❌ Constants did not reach browser (fixed in v3.28.2)

---

## 📝 Credits

**Reported by:** Community testing, web UI integration feedback
**Developed by:** ESP32-Diagnostic development team
**Tested on:** ESP32-S3 DevKitC-1 N16R8, ESP32-S3 N8R8, ESP32 Classic DevKitC

---

## 🔖 Version Information

- **Current Version:** 3.28.0
- **Previous Version:** 3.27.2
- **Next Version:** 3.28.1 (MISO backend completion)
- **Release Branch:** `dev/addfeatures`
- **Tag:** `v3.28.0`

---

## 🎯 What's Next

**v3.28.1 (Coming Soon):**
- ✅ TFT MISO backend integration completion
- ✅ MISO configuration synchronization (UI → API → Firmware)
- ✅ SD card support fixes for ESP32-S3 (HSPI vs FSPI)

**v3.28.2 (Coming Soon):**
- ✅ Complete BUTTON_BOOT JavaScript error fix
- ✅ Pin constants injection in correct location

---

**For complete changelog, see [CHANGELOG.md](../CHANGELOG.md)**
