# Release Notes – Version 3.28.4

**Release Date:** 2025-12-24
**Type:** Patch Release
**Focus:** Button monitoring API endpoint fix

---

## 🐛 Bug Fix

This patch release resolves a critical API endpoint mismatch that prevented button monitoring from working.

### Button Monitoring Not Working - API Endpoint Mismatch ✅

**Problem:**
- Button monitoring buttons (BOOT, Button 1, Button 2) showed state always as "Released"
- Clicking "Monitor Button" had no effect - state never updated in the web interface
- Frontend JavaScript was calling wrong API endpoint
- Users could not monitor button states in real-time

**Root Cause:**
- Frontend JavaScript calls `/api/button-state` (singular) for individual button queries
- Backend only had `/api/button-states` (plural) endpoint that returns ALL buttons
- Endpoint mismatch: frontend expected individual button query, backend provided batch query only
- No route handler registered for `/api/button-state` (singular)
- Result: 404 errors on every monitoring attempt, silent failures in UI

**Solution:**
```cpp
// src/main.cpp:4395-4431 - Added individual button state handler
void handleButtonState() {
  if (!server.hasArg("button")) {
    sendActionResponse(400, false, "Missing 'button' parameter");
    return;
  }

  String buttonParam = server.arg("button");
  int state = -1;
  int pin = -1;

  if (buttonParam == "boot") {
    state = getButtonBootState();
    pin = buttonBootPin;
  } else if (buttonParam == "1" || buttonParam == "button1") {
    state = getButton1State();
    pin = button1Pin;
  } else if (buttonParam == "2" || buttonParam == "button2") {
    state = getButton2State();
    pin = button2Pin;
  } else {
    sendActionResponse(400, false, "Invalid button parameter");
    return;
  }

  // LOW = pressed (pull-up), HIGH = released
  bool pressed = (state == LOW && state != -1);
  bool available = (state != -1);

  sendJsonResponse(200, {
    jsonBoolField("pressed", pressed),
    jsonBoolField("released", !pressed && available),
    jsonBoolField("available", available),
    jsonNumberField("pin", pin),
    jsonNumberField("raw_state", state)
  });
}

// src/main.cpp:5798 - Registered route
server.on("/api/button-state", handleButtonState);
```

**API Response Format:**
```json
GET /api/button-state?button=boot
{
  "pressed": false,
  "released": true,
  "available": true,
  "pin": 0,
  "raw_state": 1
}

GET /api/button-state?button=1
{
  "pressed": true,
  "released": false,
  "available": true,
  "pin": 38,
  "raw_state": 0
}
```

**Impact:**
- ✅ Button monitoring now works correctly
- ✅ State updates in real-time (100ms polling) when monitoring enabled
- ✅ "Pressed" shown in red bold when button pressed
- ✅ "Released" shown in green when button released
- ✅ Works for BOOT (GPIO 0), Button 1, and Button 2
- ✅ Proper error handling for invalid button parameters

---

## 📝 Files Modified

**src/main.cpp:**
- Lines 4395-4431: Added `handleButtonState()` handler for individual button queries
- Line 5798: Registered `/api/button-state` route

**platformio.ini:**
- Version bumped from 3.28.3 → 3.28.4

---

## 🧪 Testing

### Button Monitoring Test:
1. Navigate to "Input Devices" page in web interface
2. Click "Monitor Button" for BOOT button
3. Press GPIO 0 (BOOT) button on ESP32 - state should change to "Pressed" (red bold) ✅
4. Release button - state should return to "Released" (green) ✅
5. Repeat for Button 1 (GPIO 38/5) and Button 2 (GPIO 39/12) ✅
6. Verify real-time updates every 100ms ✅
7. Check browser console - no 404 errors ✅

### API Endpoint Test:
```bash
# Test BOOT button
curl http://esp32-diagnostic.local/api/button-state?button=boot

# Test Button 1
curl http://esp32-diagnostic.local/api/button-state?button=1

# Test Button 2
curl http://esp32-diagnostic.local/api/button-state?button=2

# Test invalid parameter (should return error)
curl http://esp32-diagnostic.local/api/button-state?button=invalid
```

---

## 📚 Related Documentation

- [CHANGELOG.md](../CHANGELOG.md) - Complete version history
- [API_REFERENCE.md](API_REFERENCE.md) - REST API documentation
- [WEB_INTERFACE.md](WEB_INTERFACE.md) - Web UI guide
- [TROUBLESHOOTING.md](TROUBLESHOOTING.md) - Common issues and solutions

---

## ⬆️ Upgrade Notes

- **No breaking changes** - fully backward compatible with v3.28.3
- **No configuration changes required** - works with existing `board_config.h`
- **Automatic fix** - button monitoring works immediately after upgrade
- **Compatible with all environments:** `esp32s3_n16r8`, `esp32s3_n8r8`, `esp32devkitc`
- **Both endpoints available:** `/api/button-states` (all buttons) and `/api/button-state` (individual button)

---

## 🔄 Comparison with v3.28.3

| Feature | v3.28.3 | v3.28.4 |
|---------|---------|---------|
| Rotary encoder auto-init | ✅ | ✅ |
| `/api/button-states` (all) | ✅ | ✅ |
| `/api/button-state` (individual) | ❌ | ✅ |
| Frontend monitoring works | ❌ | ✅ |
| Real-time button state display | ❌ | ✅ |

---

## 🔗 Links

- [ESP32-Diagnostic Repository](https://github.com/morfredus/ESP32-Diagnostic)
- [Installation Guide](INSTALL.md)
- [Pin Mapping](PIN_MAPPING.md)
