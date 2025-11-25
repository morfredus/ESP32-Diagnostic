# Troubleshooting (EN)

## BLE advertising not visible
- Confirm your board supports BLE (ESP32, ESP32-S3, ESP32-C3, ESP32-C6, ESP32-H2).
- Check the Serial Monitor for BLE initialisation errors.
- Ensure no other application is holding the BLE stack (restart the board).

## Language switch does not change texts
- Ensure you are running firmware 3.8.0 so the live translation bindings remain intact while the Wi-Fi splash and NimBLE safeguards are active.
- Verify `languages.h` is present alongside the sketch and compiles correctly.
- Clear the browser cache (Ctrl+F5) to reload the translation bundle.
- Confirm `/api/get-translations` returns HTTP 200 using the browser console or a REST client.

## Additional diagnostics show "Non teste" without accent
- Flash firmware 3.8.0 (or any later 3.x maintenance build) so the French catalogue keeps the "Non testé" placeholder and benefits from the instant translation refresh across diagnostics tabs.
- Refresh the dashboard (Ctrl+F5) to reload cached translations after updating.

## OLED screen stays blank
- Re-run `/api/oled-config?sda=<pin>&scl=<pin>` with the correct wiring.
- Check the I2C address (0x3C by default). Replace the display if it does not answer on the bus.
- Ensure the display has adequate power (3.3 V, GND shared with the ESP32).

## TFT screen stays blank (v3.11.0+)
- Verify TFT pin configuration in `include/config.h` matches your hardware wiring.
- Ensure backlight pin (TFT_BL) is correctly set to GPIO 15 and not conflicting with NeoPixel (GPIO 48).
- Check SPI connections: MOSI (GPIO 11), SCLK (GPIO 12), CS (GPIO 10), DC (GPIO 9), RST (GPIO 46).
- Verify `ENABLE_TFT_DISPLAY` is set to `true` in config.h.
- Ensure the TFT display has adequate power (3.3V or 5V depending on model, GND shared with ESP32).
- Check Serial Monitor for TFT initialization messages: "[TFT] Initializing ST7789 display..."

## Wi-Fi connection fails
- Double-check SSID/password pairs in `wifi-config.h`.
- Watch the Serial Monitor for `WL_NO_SSID_AVAIL` or `WL_CONNECT_FAILED` codes.
- Move closer to the access point or try a 2.4 GHz-only SSID.

## Memory fragmentation warnings in exports
- Call `/api/memory-details` and review the `fragmentation` percentage alongside the PSRAM flags while confirming the translated labels match the active language in 3.8.0.
- Ensure PSRAM is enabled on ESP32-S3 boards to keep the `psramAvailable` flag consistent with the new documentation.
- If fragmentation stays above 20%, power-cycle the board and re-run diagnostics to clear transient heap usage.

## GPIO tests report conflicts
- Free the pin from other peripherals (I2C, SPI, UART) before launching the sweep.
- Some pins are input-only or reserved (e.g., GPIO34-39). Verify your board schematic.

## Exports download empty files
- Wait until the diagnostic run completes before triggering an export.
- Ensure the browser allows downloads from the device IP.
- Check Serial logs for memory warnings that may interrupt streaming.
