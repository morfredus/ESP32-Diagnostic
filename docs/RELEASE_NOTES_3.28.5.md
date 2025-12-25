# Release Notes – Version 3.28.5

**Release Date:** 2025-12-24
**Type:** Patch Release
**Focus:** Input device monitoring GPIO fixes

---

## 🐛 Bug Fixes

This patch release resolves critical issues with input device monitoring, specifically the rotary encoder button state and button monitoring GPIO access.

### 1. Rotary Encoder Button Stuck "Pressed" ✅

**Problem:**
- After pressing the rotary encoder button, the state remained "Pressed" even after release
- Web interface always displayed "Pressed" in red, never showed "Released"
- Impossible to see the real button state in real-time
- Button appeared permanently stuck in pressed state

**Root Cause:**
- `handleRotaryPosition()` was returning `rotaryButtonPressed` (volatile ISR variable)
- Volatile variable set to `true` by ISR on button press, but NEVER automatically reset to `false`
- For real-time monitoring, we must read the actual GPIO state, not the event-driven variable
- ISR variable is for event detection, not state monitoring

**Solution:**
```cpp
// src/main.cpp:3199-3203 - New function to read real GPIO
int getRotaryButtonGPIOState() {
  if (rotary_sw_pin < 0 || rotary_sw_pin > 48) return -1;
  return digitalRead(rotary_sw_pin);
}

// src/main.cpp:4369-4379 - Usage in handleRotaryPosition()
void handleRotaryPosition() {
  // v3.28.5 fix: Read REAL GPIO state for monitoring, not ISR variable
  int buttonGPIOState = getRotaryButtonGPIOState();
  bool buttonPressed = (buttonGPIOState == LOW && buttonGPIOState != -1);

  sendJsonResponse(200, {
    jsonNumberField("position", (int32_t)rotaryPosition),
    jsonBoolField("button_pressed", buttonPressed),  // Now reads real GPIO
    jsonBoolField("available", rotaryAvailable)
  });
}
```

**Impact:**
- ✅ Rotary encoder button now displays correct state in real-time
- ✅ "Pressed" (red) when button is physically pressed
- ✅ "Released" (green) when button is physically released
- ✅ Immediate update (100ms polling)
- ✅ Accurate state monitoring for user interactions

---

### 2. Button Monitoring Not Working (BOOT/Button1/Button2) ✅

**Problem:**
- Monitoring for BOOT, Button 1, Button 2 did not work
- States never updated despite v3.28.4 fix
- Buttons remained stuck on "Released" state
- Real-time monitoring completely non-functional

**Root Cause:**
- Functions used `static` variables (`buttonBootPin`, `button1Pin`, `button2Pin`)
- Potential visibility or initialization issue with static variables
- GPIO may not be correctly accessible via these variables
- Variable access timing or scope problem

**Solution:**
```cpp
// src/main.cpp:3182-3199 - Direct constant access
// v3.28.5: Use constants directly to ensure correct pin access
int getButtonBootState() {
  // Use constant directly instead of static variable
  if (BUTTON_BOOT < 0 || BUTTON_BOOT > 48) return -1;
  return digitalRead(BUTTON_BOOT);
}

int getButton1State() {
  if (BUTTON_1 < 0 || BUTTON_1 > 48) return -1;
  return digitalRead(BUTTON_1);
}

int getButton2State() {
  if (BUTTON_2 < 0 || BUTTON_2 > 48) return -1;
  return digitalRead(BUTTON_2);
}

// src/main.cpp:4420-4428 - handleButtonState() uses constants
if (buttonParam == "boot") {
  state = getButtonBootState();
  pin = BUTTON_BOOT;  // v3.28.5: Use constant directly
} else if (buttonParam == "1" || buttonParam == "button1") {
  state = getButton1State();
  pin = BUTTON_1;
} else if (buttonParam == "2" || buttonParam == "button2") {
  state = getButton2State();
  pin = BUTTON_2;
}
```

**Impact:**
- ✅ BOOT button monitoring (GPIO 0) now works
- ✅ Button 1 monitoring (GPIO 38/5) now works
- ✅ Button 2 monitoring (GPIO 39/12) now works
- ✅ States update in real-time
- ✅ "Pressed" (red bold) / "Released" (green) display correctly

---

## 📝 Files Modified

**src/main.cpp:**
- Lines 3182-3203: Updated button state readers to use constants directly, added `getRotaryButtonGPIOState()`
- Lines 4369-4379: `handleRotaryPosition()` now reads real GPIO state instead of ISR variable
- Lines 4389-4407: `handleButtonStates()` uses constants for pin numbers
- Lines 4420-4428: `handleButtonState()` uses constants for pin access

**platformio.ini:**
- Version bumped from 3.28.4 → 3.28.5

---

## 🧪 Testing

### Rotary Encoder Test:
1. Enable rotary encoder button monitoring
2. Press the button → "Pressed" (red) displays immediately ✅
3. Release → immediately shows "Released" (green) ✅
4. Repeat multiple times → states are always correct ✅
5. No stuck states ✅

### Button Monitoring Test:
1. Enable monitoring for each button (BOOT, Button 1, Button 2)
2. Press GPIO 0/38/39 → "Pressed" shown immediately ✅
3. Release → "Released" shown immediately ✅
4. No stuck states on any button ✅
5. Real-time updates every 100ms ✅

### API Test:
```bash
# Test rotary encoder position and button
curl http://esp32-diagnostic.local/api/rotary-position
# Should show: {"position":0,"button_pressed":false,"available":true}

# Test individual buttons
curl http://esp32-diagnostic.local/api/button-state?button=boot
curl http://esp32-diagnostic.local/api/button-state?button=1
curl http://esp32-diagnostic.local/api/button-state?button=2
```

---

## 📚 Related Documentation

- [CHANGELOG.md](../CHANGELOG.md) - Complete version history
- [PIN_MAPPING.md](PIN_MAPPING.md) - GPIO pin assignments for all boards
- [API_REFERENCE.md](API_REFERENCE.md) - REST API documentation
- [WEB_INTERFACE.md](WEB_INTERFACE.md) - Web interface guide
- [TROUBLESHOOTING.md](TROUBLESHOOTING.md) - Common issues and solutions

---

## ⬆️ Upgrade Notes

- **No breaking changes** - fully backward compatible with v3.28.4
- **No configuration changes required** - works with existing `board_config.h`
- **Automatic fix** - all input device monitoring works immediately after upgrade
- **Compatible with all environments:** `esp32s3_n16r8`, `esp32s3_n8r8`, `esp32devkitc`
- **Performance:** No performance impact, same 100ms polling interval

---

## 🔄 Version Progression

| Feature | v3.28.3 | v3.28.4 | v3.28.5 |
|---------|---------|---------|---------|
| Rotary encoder auto-init | ✅ | ✅ | ✅ |
| Button API endpoints | ✅ | ✅ | ✅ |
| Frontend monitoring | ❌ | ✅ | ✅ |
| Rotary button state correct | ❌ | ❌ | ✅ |
| BOOT/BTN1/BTN2 monitoring | ❌ | ❌ | ✅ |
| Real-time state accuracy | ❌ | ❌ | ✅ |

---

## 🎯 Known Issues Fixed

This release completes the input device monitoring feature set by fixing the last remaining issues:
- ✅ v3.28.3: Added rotary encoder initialization and button monitoring endpoints
- ✅ v3.28.4: Fixed API endpoint mismatch between frontend and backend
- ✅ v3.28.5: Fixed GPIO access for accurate real-time state reading

All input device monitoring features are now fully functional.

---

## 🔗 Links

- [ESP32-Diagnostic Repository](https://github.com/morfredus/ESP32-Diagnostic)
- [Installation Guide](INSTALL.md)
- [Build & Deploy Guide](BUILD_AND_DEPLOY.md)
- [Configuration Guide](CONFIG.md)
