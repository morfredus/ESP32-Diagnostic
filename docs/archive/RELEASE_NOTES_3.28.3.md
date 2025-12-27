# Release Notes – Version 3.28.3

**Release Date:** 2025-12-24
**Type:** Patch Release
**Focus:** Rotary encoder initialization + Button monitoring API

---

## 🐛 Bug Fixes

This patch release resolves two critical issues affecting input device functionality:

### 1. Rotary Encoder Not Working Until Manual Test ✅

**Problem:**
- Rotary encoder did not respond to rotation or button presses after boot
- Only worked after navigating to "Input Devices" page and clicking "Test Rotary Encoder"
- Made the rotary encoder unusable for immediate operation after power-on

**Root Cause:**
- `initRotaryEncoder()` was NEVER called during startup in `setup()`
- Function was only called inside `testRotaryEncoder()` which requires manual trigger via web UI
- GPIO pins were not configured and interrupts were not attached at boot time
- Left encoder in non-functional state until explicit user action

**Solution:**
```cpp
// src/main.cpp:5757-5765 - Added rotary encoder initialization to setup()
// Initialize rotary encoder on startup (v3.28.3 fix)
Serial.println("Initialisation de l'encodeur rotatif...");
initRotaryEncoder();
if (rotaryAvailable) {
  Serial.printf("Encodeur rotatif OK: CLK=%d, DT=%d, SW=%d\r\n",
                rotary_clk_pin, rotary_dt_pin, rotary_sw_pin);
} else {
  Serial.println("Encodeur rotatif: non disponible ou configuration invalide");
}
```

**Impact:**
- ✅ Rotary encoder now initializes automatically on boot
- ✅ Rotation detection works immediately without manual test
- ✅ Button presses detected from power-on
- ✅ Real-time position tracking available via `/api/rotary-position` immediately

---

### 2. Button Monitoring Not Functional ✅

**Problem:**
- "Monitor Button" buttons in web UI did nothing when clicked
- No way to see real-time button state (pressed/released)
- JavaScript monitoring functions existed but backend API endpoints were missing
- Frontend polling failed silently

**Root Cause:**
- Frontend code referenced monitoring functions (`toggleBootButtonMonitoring()`, etc.)
- BUT no backend API endpoint existed to read button states in real-time
- Missing `/api/button-states` route handler
- Frontend polling returned 404 errors

**Solution:**
```cpp
// src/main.cpp:3182-3196 - Added button state reader functions
int getButtonBootState() {
  if (buttonBootPin < 0 || buttonBootPin > 48) return -1;
  return digitalRead(buttonBootPin);
}

int getButton1State() {
  if (button1Pin < 0 || button1Pin > 48) return -1;
  return digitalRead(button1Pin);
}

int getButton2State() {
  if (button2Pin < 0 || button2Pin > 48) return -1;
  return digitalRead(button2Pin);
}

// src/main.cpp:4375-4393 - Added HTTP handler for all buttons
void handleButtonStates() {
  int bootState = getButtonBootState();
  int button1State = getButton1State();
  int button2State = getButton2State();

  // LOW = pressed (pull-up), HIGH = released
  sendJsonResponse(200, {
    jsonBoolField("boot_pressed", bootState == LOW && bootState != -1),
    jsonBoolField("boot_available", bootState != -1),
    jsonBoolField("button1_pressed", button1State == LOW && button1State != -1),
    jsonBoolField("button1_available", button1State != -1),
    jsonBoolField("button2_pressed", button2State == LOW && button2State != -1),
    jsonBoolField("button2_available", button2State != -1),
    jsonNumberField("boot_pin", buttonBootPin),
    jsonNumberField("button1_pin", button1Pin),
    jsonNumberField("button2_pin", button2Pin)
  });
}

// src/main.cpp:5759 - Registered route
server.on("/api/button-states", handleButtonStates);
```

**API Response Format:**
```json
GET /api/button-states
{
  "boot_pressed": false,
  "boot_available": true,
  "button1_pressed": false,
  "button1_available": true,
  "button2_pressed": false,
  "button2_available": true,
  "boot_pin": 0,
  "button1_pin": 38,
  "button2_pin": 39
}
```

**Impact:**
- ✅ New API endpoint `/api/button-states` returns real-time states for ALL buttons
- ✅ Returns JSON with pressed state for BOOT, Button 1, and Button 2
- ✅ Includes pin numbers and availability status
- ✅ Frontend monitoring can now poll this endpoint for live updates (100ms interval)
- ✅ Real-time feedback when user presses physical buttons

---

## 📝 Files Modified

**src/main.cpp:**
- Lines 3182-3196: Added button state reader functions (`getButtonBootState()`, `getButton1State()`, `getButton2State()`)
- Lines 4375-4393: Added `handleButtonStates()` HTTP handler for `/api/button-states`
- Lines 5757-5765: Initialize rotary encoder in `setup()` for immediate functionality
- Line 5759: Registered `/api/button-states` route

**platformio.ini:**
- Version bumped from 3.28.2 → 3.28.3

---

## ✅ Testing

### Rotary Encoder Test:
1. Power on ESP32
2. Rotate encoder immediately - position should change ✅
3. Press encoder button - should register ✅
4. Navigate to "Input Devices" page - encoder already working ✅
5. Position tracking accurate without manual initialization ✅

### Button Monitoring Test:
1. Navigate to "Input Devices" page
2. Press BOOT button (GPIO 0) - LED/buzzer feedback should work ✅
3. Check `/api/button-states` endpoint - should return current states ✅
4. Click "Monitor Button" for any button - real-time state should update ✅
5. Press/release monitored button - UI should reflect changes ✅

---

## 📚 Related Documentation

- [CHANGELOG.md](../CHANGELOG.md) - Complete version history
- [PIN_MAPPING.md](PIN_MAPPING.md) - GPIO pin assignments
- [API_REFERENCE.md](API_REFERENCE.md) - REST API documentation
- [WEB_INTERFACE.md](WEB_INTERFACE.md) - Web UI guide

---

## ⬆️ Upgrade Notes

- **No breaking changes** - fully backward compatible with v3.28.2
- **No configuration changes required** - works with existing `board_config.h`
- **Automatic improvement** - rotary encoder now works immediately after flash
- **Compatible with all environments:** `esp32s3_n16r8`, `esp32s3_n8r8`, `esp32devkitc`

---

## 🔗 Links

- [ESP32-Diagnostic Repository](https://github.com/morfredus/ESP32-Diagnostic)
- [Installation Guide](INSTALL.md)
- [Troubleshooting](TROUBLESHOOTING.md)
