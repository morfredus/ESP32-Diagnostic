# Release Notes — 3.20.2 (2025-12-07)

## Changed
- **Web UI Pin References:** Hardcoded GPIO pin values in web interface replaced with dynamic variable references from `board_config.h`
   - RGB LED pins (R/G/B) now reference `RGB_LED_PIN_R`, `RGB_LED_PIN_G`, `RGB_LED_PIN_B` instead of hardcoded values
   - DHT sensor pin now references `DHT_PIN` instead of hardcoded value
   - Light sensor pin now references `LIGHT_SENSOR_PIN` instead of hardcoded value  
   - Distance sensor trigger/echo pins now reference `DISTANCE_TRIG_PIN` / `DISTANCE_ECHO_PIN` instead of hardcoded values
   - Motion sensor pin now references `MOTION_SENSOR_PIN` instead of hardcoded value
   - Buzzer/PWM pin now references `PWM_PIN` instead of hardcoded value
- Pin values are now injected as JavaScript constants at page load via the `/js/app.js` route, ensuring UI always displays the correct compiled target pins
- Added TARGET identification banner at startup with actual pin values for debugging

## Fixed
- Web UI now correctly displays the actual GPIO pins based on compiled target (ESP32-S3 vs ESP32 CLASSIC)
- JavaScript ReferenceError resolved by injecting pin constants before PROGMEM static code

## Technical Details
- Modified `handleJavaScriptRoute()` in `main.cpp` to inject pin constants before serving JavaScript
- Added extern declarations in `include/web_interface.h` for all pin variables
- Pin values are dynamically generated from `board_config.h` defaults during compilation
- Debug logging shows pin values in Serial Monitor when JavaScript is loaded

## Notes
- This patch ensures that the web dashboard always reflects the actual pins of the compiled firmware, eliminating mismatches between UI display and hardware configuration.
- `PROJECT_VERSION` is set to `3.20.2` in `platformio.ini`.

## Upgrade
- Rebuild and flash with the latest code (`pio run -e esp32devkitc --target upload` for CLASSIC or `pio run -e esp32s3_n16r8 --target upload` for ESP32-S3).
- Hard refresh the web interface (Ctrl+Shift+R in browsers) to clear any cached JavaScript.
- Check Serial Monitor (115200 baud) at startup to see the TARGET banner with actual pin values.









