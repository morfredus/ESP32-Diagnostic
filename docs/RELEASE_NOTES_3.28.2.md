# ESP32 Diagnostic Suite – Release Notes v3.28.2

**Release Date:** December 24, 2025
**Type:** Emergency Patch
**Severity:** Critical Bug Fix

---

## 📋 Overview

Version 3.28.2 is an **emergency patch** that fixes the BUTTON_BOOT JavaScript error that persisted in versions 3.28.0 and 3.28.1 despite previous fix attempts. This version correctly identifies and resolves the root cause by injecting GPIO constants in the proper location.

**Who should upgrade:**
- ✅ ALL users on v3.28.0 or v3.28.1 (incomplete fix)
- ✅ Users experiencing "ReferenceError: BUTTON_BOOT is not defined" in browser console
- ✅ Users unable to access the Input Devices page in the web interface

---

## 🐛 Fixed Issues

### BUTTON_BOOT JavaScript ReferenceError ✅ (REALLY FIXED NOW)

#### Problem Description
When navigating to the "Input Devices" page in the web interface, the browser console displayed:
```
ReferenceError: BUTTON_BOOT is not defined
```

This error:
- Prevented the Input Devices page from loading correctly
- Made BUTTON_BOOT, BUTTON_1, and BUTTON_2 controls non-functional
- Occurred despite fix attempts in v3.28.0 and v3.28.1

#### Root Cause
The GPIO constants (`BUTTON_BOOT`, `BUTTON_1`, `BUTTON_2`, `TFT_MISO_PIN`) were injected in the **wrong location**:

1. **v3.28.0/3.28.1 approach (INCORRECT):**
   - Constants were added to `web_interface.h::generateJavaScript()`
   - **BUT** this function is only called to calculate JavaScript size for statistics
   - The actual JavaScript served to the browser comes from `main.cpp::handleJavaScriptRoute()`
   - Therefore, injected constants **never reached the browser**

2. **v3.28.2 solution (CORRECT):**
   - Constants are now injected in `main.cpp::handleJavaScriptRoute()`
   - This is the function that actually generates JavaScript for the browser
   - Constants are now properly available before any JavaScript functions execute

#### Technical Solution
**File:** `src/main.cpp`
**Lines:** 5397-5405

```cpp
// Button pins (v3.28.2 fix - added to handleJavaScriptRoute)
pinVars += ";const BUTTON_BOOT=";
pinVars += String(BUTTON_BOOT);
pinVars += ";const BUTTON_1=";
pinVars += String(BUTTON_1);
pinVars += ";const BUTTON_2=";
pinVars += String(BUTTON_2);

// TFT MISO pin
pinVars += ";const TFT_MISO_PIN=";
pinVars += String(TFT_MISO);
```

#### Impact & Benefits
- ✅ Input Devices page now loads **WITHOUT** JavaScript errors
- ✅ BUTTON_BOOT displays correctly as **read-only GPIO 0**
- ✅ BUTTON_1 and BUTTON_2 are fully functional
- ✅ All GPIO constants properly injected **before** JavaScript execution
- ✅ Browser console shows **no ReferenceError**

---

## 📦 Files Modified

| File | Changes | Lines |
|------|---------|-------|
| `src/main.cpp` | Added button/TFT MISO constants to pinVars in handleJavaScriptRoute() | 5397-5415 |
| `platformio.ini` | Version bump: 3.28.1 → 3.28.2 | 27 |

---

## ✅ Testing & Verification

### How to Verify the Fix

1. **Flash firmware v3.28.2** to your ESP32 board
2. **Connect to the web interface** via http://esp32.local or IP address
3. **Navigate** to "Input Devices" page using the menu
4. **Check** that the page loads without errors
5. **Open browser console** (F12) and verify NO ReferenceError messages
6. **Verify** BUTTON_BOOT shows "GPIO 0 (non configurable)"
7. **Test** BUTTON_1 and BUTTON_2 monitoring functions

### Expected Results
✅ Input Devices page loads completely
✅ BUTTON_BOOT field displays "GPIO 0 (non configurable)"
✅ Browser console shows NO JavaScript errors
✅ Button monitoring works when clicking "Monitor Button"

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

### Upgrading from v3.28.0 or v3.28.1

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
- Go to "Input Devices" page
- Confirm no JavaScript errors in console

### Upgrading from v3.27.x or earlier

Follow the standard upgrade procedure:
1. Review `docs/RELEASE_NOTES_3.28.0.md` for v3.28.0 changes
2. Review `docs/RELEASE_NOTES_3.28.1.md` for v3.28.1 changes
3. Review this document for v3.28.2 changes
4. Flash firmware using PlatformIO

---

## 🔗 Related Documentation

- [CHANGELOG.md](../CHANGELOG.md) – Complete version history
- [RELEASE_NOTES_3.28.0.md](RELEASE_NOTES_3.28.0.md) – v3.28.0 Initial BUTTON_BOOT fix attempt
- [RELEASE_NOTES_3.28.1.md](RELEASE_NOTES_3.28.1.md) – v3.28.1 MISO & SD card fixes
- [TROUBLESHOOTING.md](TROUBLESHOOTING.md) – Common issues and solutions
- [PIN_MAPPING.md](PIN_MAPPING.md) – GPIO pin assignments

---

## ❓ FAQ

### Q: Should I skip v3.28.0 and v3.28.1?
**A:** Yes, upgrade directly to v3.28.2. Versions 3.28.0 and 3.28.1 contain an incomplete fix for the BUTTON_BOOT error.

### Q: Will this fix affect my existing hardware wiring?
**A:** No, this is a software-only bug fix. No hardware changes required.

### Q: What if I still see JavaScript errors after upgrading?
**A:** Clear your browser cache (Ctrl+Shift+Delete) and reload the page. If errors persist, check browser console for specific error messages and report them via GitHub Issues.

### Q: Does this fix apply to both ESP32-S3 and ESP32 Classic?
**A:** Yes, the fix applies to all supported board variants.

---

## 📝 Credits

**Reported by:** Community testing
**Fixed by:** ESP32-Diagnostic development team
**Tested on:** ESP32-S3 DevKitC-1 N16R8, ESP32-S3 N8R8, ESP32 Classic DevKitC

---

## 🔖 Version Information

- **Current Version:** 3.28.2
- **Previous Version:** 3.28.1
- **Release Branch:** `dev/addfeatures`
- **Tag:** `v3.28.2`

---

**For complete changelog, see [CHANGELOG.md](../CHANGELOG.md)**
