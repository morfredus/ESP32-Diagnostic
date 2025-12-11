# Release Notes - Version 3.21.1

**Release Date**: December 11, 2025  
**Type**: Feature Release - NeoPixel Wi-Fi Status Indicator

## Overview

Version 3.21.1 enhances user experience with a real-time Wi-Fi status indicator using NeoPixel/WS2812 RGB LEDs. The firmware now provides immediate visual feedback during Wi-Fi connection, disconnection, and system operations without requiring web interface access.

## What's New

### NeoPixel Wi-Fi Status Indicator

**Real-Time Connection Feedback:**
- **Automatic initialization** during startup - NeoPixel lights up during boot sequence
- **Connection states visualized**:
  - **Yellow (50, 50, 0)**: Wi-Fi connection in progress during startup
  - **Green heartbeat (0, 50, 0) / (0, 10, 0)**: Successfully connected - pulsing every 1 second
  - **Red heartbeat (50, 0, 0) / (10, 0, 0)**: Wi-Fi disconnected - pulsing every 1 second
- **Non-blocking operation**: Heartbeat managed in main loop with 1 Hz update rate
- **Zero interference**: LED state pauses during hardware tests and manual pattern execution

### BOOT Button Long Press Confirmation

**Visual Reboot Confirmation:**
- **Violet flash (255, 0, 255)** displays immediately upon long press (2 seconds) of BOOT button
- Confirms reboot request before system restart
- Synchronized with TFT progress bar and serial output
- Provides clear visual feedback independent of web interface availability

### Test & Pattern Isolation

**Clean Test Environment:**
- NeoPixel Wi-Fi heartbeat automatically pauses when:
  - Running `/api/neopixel-test` (async and synchronous modes)
  - Running `/api/neopixel-pattern` (rainbow, blink, fade, chase, color modes)
  - Running manual color changes via `/api/neopixel-color`
- Heartbeat automatically restores upon test/pattern completion
- Prevents interference between system status and test animations

### Affected GPIO Mappings

**No changes from v3.21.0** - NeoPixel uses existing GPIO configuration:
- **ESP32-S3**: GPIO 48 (default)
- **ESP32 Classic**: GPIO 2 (default, configurable via `CUSTOM_LED_PIN` in `board_config.h`)

## Technical Details

### Implementation

**New Functions Added:**
- `neopixelShowConnecting()` - Display yellow during Wi-Fi connection attempts
- `neopixelSetWifiState(bool)` - Set NeoPixel state based on Wi-Fi connection status
- `updateNeoPixelWifiStatus()` - Non-blocking heartbeat update (called from main loop)
- `neopixelPauseStatus()` / `neopixelResumeStatus()` - Pause/resume Wi-Fi indicator
- `neopixelRestoreWifiStatus()` - Restore Wi-Fi status after test completion
- `neopixelShowRebootFlash()` - Display violet flash on BOOT button press

**Modified Functions:**
- `setup()` - Initialize NeoPixel before Wi-Fi connection, call pause/resume during Wi-Fi setup
- `loop()` - Added `updateNeoPixelWifiStatus()` call for heartbeat management
- `onButtonBootLongPress()` - Added `neopixelShowRebootFlash_Impl()` call
- `runNeopixelTestTask()` - Pause/resume Wi-Fi status around async tests
- `handleNeoPixelTest()` - Pause/resume Wi-Fi status around synchronous tests
- `handleNeoPixelPattern()` - Pause/resume Wi-Fi status during pattern execution

**New Global State Variables:**
- `volatile bool neopixelStatusPaused` - Flag to pause Wi-Fi heartbeat
- `unsigned long neopixelHeartbeatPreviousMillis` - Timing for 1 Hz heartbeat
- `const unsigned long NEOPIXEL_HEARTBEAT_INTERVAL_MS = 1000` - 1 second interval
- `bool neopixelHeartbeatState` - Toggle for brightness pulse effect
- `bool neopixelStatusKnown` - Track if Wi-Fi state has been initialized
- `bool neopixelLastWifiConnected` - Cache last known Wi-Fi state
- `bool neopixelConnecting` - Flag for connection-in-progress state

### Compatibility

- **Hardware**: Requires NeoPixel/WS2812 LED on configured GPIO
- **Versions**: Forward-compatible with ESP32-S3 and ESP32 Classic from v3.21.0
- **No breaking changes**: Purely additive feature, existing functionality unchanged
- **GPIO flexibility**: Supports dynamic GPIO configuration via web interface

### Testing

✅ **Compilation**: Successful on both ESP32-S3 (esp32s3_n16r8) and ESP32 Classic (esp32devkitc)  
✅ **Code Quality**: No new warnings, forward declarations properly organized  
✅ **Memory**: Minimal footprint - only constant state tracking, no dynamic allocations  

## Upgrade Instructions

### From v3.21.0 to v3.21.1

1. Pull the latest code from the repository
2. Compile: `pio run` or use PlatformIO build task
3. Upload: `pio run --target upload` or use upload button
4. **No configuration changes required**
5. No web interface or hardware changes needed (NeoPixel GPIO unchanged)

### Verification

After upload, observe NeoPixel behavior:
- **At boot**: Brief yellow flash during Wi-Fi connection attempt
- **After connection**: Steady green pulsing pattern (connected) or red pulsing pattern (not connected)
- **Test execution**: LED should pause during `/api/neopixel-test` or pattern requests
- **BOOT button**: Violet flash appears when holding BOOT for 2+ seconds

## Behavior Changes

| Scenario | v3.21.0 | v3.21.1 |
|----------|---------|---------|
| Wi-Fi connection | No LED feedback | Yellow during attempt, then green when connected |
| Wi-Fi disconnected | No LED feedback | Red pulsing pattern |
| NeoPixel test running | Test shows effects | Test shows effects, heartbeat paused, restored after |
| BOOT button reboot | No LED feedback | Violet flash confirms reboot |
| Main loop | No NeoPixel updates | Heartbeat updated every 1 second (non-blocking) |

## Breaking Changes

**None** - This is a purely additive feature release with full backward compatibility.

## Known Issues

None identified at release. Existing known issues from v3.21.0 remain unchanged.

## Dependencies

- **Adafruit_NeoPixel**: Already required, no new dependency
- **Arduino Core 3.3.3**: No change from v3.21.0
- **PlatformIO**: No version requirement change

## Credits

- NeoPixel Wi-Fi heartbeat feature: GitHub Copilot
- BOOT button reboot flash integration: GitHub Copilot
- Testing and validation: morfredus

## See Also

- **User Guide**: `docs/USAGE.md` / `docs/USAGE_FR.md` - NeoPixel behavior section
- **Feature Matrix**: `docs/FEATURE_MATRIX.md` / `docs/FEATURE_MATRIX_FR.md` - Updated LED indicators
- **Web Interface**: `docs/WEB_INTERFACE.md` / `docs/WEB_INTERFACE_FR.md` - NeoPixel API endpoints
