# Configuration (EN)

## Wi-Fi credentials
The firmware reads credentials from `include/wifi-config.h`.

```cpp
// wifi-config.h (example)
#pragma once
#include <vector>
#include <WiFiMulti.h>

static WiFiMulti wifiMulti;

static const std::vector<std::pair<const char*, const char*>> WIFI_NETWORKS = {
    {"MySSID", "MyPassword"},
    {"BackupSSID", "BackupPassword"}
};
```

- Declare at least one SSID/password pair.
- The firmware iterates over the list until a connection succeeds.
- Avoid storing production credentials in version control.

## Language settings
- French is the default language.
- Use `/api/set-language?lang=en` or the UI toggle to switch to English.
- Translation strings live in `languages.h` inside the `Translations` structure. Add new languages by extending this file and exposing them in the UI.
- Release 3.8.0 keeps the shared catalog in sync while adding the Wi-Fi splash and NimBLE safeguards with no extra configuration required.

## Display configuration

### OLED configuration
- Default pins: SDA=21, SCL=20 (ESP32-S3 default in this project).
- Change wiring on the fly with `/api/oled-config?sda=<pin>&scl=<pin>`.
- After updating pins, the firmware automatically reinitialises the display and stores the mapping in RAM.

### TFT ST7789 configuration (v3.11.0+)
- Default pins configured in `include/config.h`:
  - MOSI: GPIO 11
  - SCLK: GPIO 12
  - CS: GPIO 10
  - DC: GPIO 9
  - RST: GPIO 46
  - BL (backlight): GPIO 15
- TFT resolution: 240x240 pixels
- Displays boot splash screen and real-time WiFi/IP status
- Enable/disable via `ENABLE_TFT_DISPLAY` flag in config.h
- Supports simultaneous OLED and TFT operation (dual display mode)

## Sensors configuration

### Distance Sensor (HC-SR04)

- TRIG is an output pin; ECHO is an input pin.
- Power the sensor at 5V and protect the ECHO line with a resistor divider (5V to 3.3V) before the ESP32 input.
- ESP32-S3 with Octal PSRAM/Flash (e.g., DevKitC-1 N16R8): avoid using GPIO 35..48 for TRIG/ECHO because these pins are reserved by the OPI memory interface. The firmware will flag this configuration as invalid.
- Defaults since v3.12.3: TRIG = GPIO 16, ECHO = GPIO 17.
- Suggested alternative on ESP32-S3 if the secondary I2C bus is disabled: TRIG = GPIO 26 (output), ECHO = GPIO 25 (input).

## Bluetooth® naming rules
- Names must be 3–31 characters, ASCII alphanumeric plus hyphen or underscore.
- Invalid input returns HTTP 400 without restarting advertising.
- Successful renaming persists until reboot.

## Advanced options
- **PSRAM:** Automatically enabled for ESP32-S3 N16R8 and N8R8 builds via `platformio.ini` configuration.
- **BLE support:** Not available in this PlatformIO version. BLE functionality has been removed.
- **Debugging:** Open the Serial Monitor to observe Wi-Fi connection retries (mirrored on OLED if present), diagnostic progress, and sensor readings. Poll `/api/memory-details` for precise fragmentation metrics.
- **Multi-board support:** Select the appropriate environment in PlatformIO (`esp32s3_n16r8`, `esp32s3_n8r8`, or `esp32devkitc`) before building.
