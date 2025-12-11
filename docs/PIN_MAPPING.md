# Pin Mapping — Quick Reference (v3.21.1)

## Supported Environments

The firmware supports three distinct build environments, each with optimized pin configurations:

1. **`esp32s3_n16r8`** (default) — ESP32-S3 DevKitC-1 N16R8 (16MB Flash, 8MB PSRAM OPI/QSPI)
2. **`esp32s3_n8r8`** — ESP32-S3 DevKitC-1 N8R8 (8MB Flash, 8MB PSRAM)
3. **`esp32devkitc`** — ESP32 Classic DevKitC (4MB Flash, no PSRAM)

Pin mappings are defined in `include/board_config.h` using conditional compilation based on `TARGET_ESP32_S3` or `TARGET_ESP32_CLASSIC`.

---

## ESP32-S3 Pin Configuration (N16R8 / N8R8)

**Applies to:** `esp32s3_n16r8`, `esp32s3_n8r8`

### I2C Interface
- **SDA:** GPIO 15
- **SCL:** GPIO 16

### Buttons
- **Button 1:** GPIO 38
- **Button 2:** GPIO 39

### RGB LED (Separate pins)
- **Red:** GPIO 21
- **Green:** GPIO 45
- **Blue:** GPIO 47

### Sensors
- **DHT (Temperature/Humidity):** GPIO 5
- **Light Sensor (ADC):** GPIO 4
- **HC-SR04 Distance Sensor:** TRIG GPIO 3 / ECHO GPIO 6
- **PIR Motion Sensor:** GPIO 46
- **Buzzer:** GPIO 14
- **PWM Test:** GPIO 14

### TFT ST7789 Display (240x240)
- **MOSI:** GPIO 11
- **SCLK:** GPIO 12
- **CS:** GPIO 10
- **DC:** GPIO 9
- **RST:** GPIO 13
- **Backlight (BL):** GPIO 7

### GPS Module (UART1)
- **RXD:** GPIO 18 (GPS TX → ESP RX)
- **TXD:** GPIO 17 (GPS RX → ESP TX)
- **PPS:** GPIO 8

### NeoPixel / WS2812B
- **Pin:** GPIO 48 (now enabled)
- **Count:** 1 LED
- **Brightness:** 50 (0-255)

### ESP32-S3 Important Notes
- GPIO 35–44 are reserved by the octal PSRAM/Flash interface; keep them free.
- GPIO 48 is dedicated to the NeoPixel.
- RGB LED uses GPIO 21/45/47 (Red/Green/Blue respectively); GPIO45 is a strapping pin, keep LED off at boot.
- Strapping pins: GPIO 0, GPIO 45, GPIO 46 — keep stable during boot.
- USB CDC is enabled; keep GPIO 19/20 free for USB D-/D+ (I2C moved to 15/16).

---

## ESP32 Classic Pin Configuration (DevKitC)

**Applies to:** `esp32devkitc`

### I2C Interface
- **SDA:** GPIO 21
- **SCL:** GPIO 22 (different from S3!)

### Buttons
- **Button 1:** GPIO 32 (with internal pull-up)
- **Button 2:** GPIO 33 (with internal pull-up)

### RGB LED (Separate pins)
- **Red:** GPIO 13
- **Green:** GPIO 14
- **Blue:** GPIO 25

### Sensors
- **DHT (Temperature/Humidity):** GPIO 32
- **Light Sensor (ADC):** GPIO 39
- **HC-SR04 Distance Sensor:** TRIG GPIO 27 / ECHO GPIO 33
- **Buzzer:** GPIO 5
- **PWM Test:** GPIO 5

### TFT ST7789 Display (240x240)
- **MOSI:** GPIO 23
- **SCLK:** GPIO 18
- **CS:** GPIO 27
- **DC:** GPIO 14
- **RST:** GPIO 25
- **Backlight (BL):** GPIO 32

### GPS Module (UART2)
- **RXD:** GPIO 16 (RX2, GPS TX → ESP RX)
- **TXD:** GPIO 17 (TX2, GPS RX → ESP TX)
- **PPS:** GPIO 36

### NeoPixel / WS2812B
- **Pin:** -1 (disabled by default)
- **Count:** 8 LEDs
- **Brightness:** 50 (0-255)

### ESP32 Classic Important Notes
- **Input-Only Pins:** GPIO 34, 35, 36, 39 (VP/VN) — can only read, cannot output.
- **Strapping Pins:** GPIO 0, GPIO 2, GPIO 4, GPIO 5, GPIO 12, GPIO 15 — avoid LEDs/peripherals on these pins to prevent boot issues.
- **UART0 (USB-UART):** GPIO 1 (TX0) and GPIO 3 (RX0) are reserved for flashing/debugging — do not connect external devices.
- **Flash Pins:** GPIO 6-11 are connected to SPI Flash — do not use.
- **ADC2 Limitation:** ADC2 (GPIO 0, 2, 4, 12-15, 25-27) cannot be used when WiFi is active.
- **Current Limit:** Max 12 mA per GPIO; use transistor/driver for higher loads.
- **Hardware Migration Required:** v3.21.0 introduces 11 pin changes to resolve boot and USB communication issues. See `docs/PIN_MAPPING_CHANGES_FR.md` for detailed rationale.

---

## Common Configuration (All Targets)

### Built-in LED
- **Default:** -1 (disabled, auto-detect at runtime)

### OLED Display (0.96" I2C)
- **Address:** 0x3C
- **Resolution:** 128x64 pixels
- **Rotation:** 0 (configurable: 0, 1, 2, 3)

### DHT Sensor Type
- **Default:** DHT22 (can be changed to DHT11 in config.h)

---

## Pin Conflict Resolution

### ESP32-S3 Conflicts
- Keep GPIO 35–44 free (PSRAM/Flash bus).
- RGB LED uses GPIO 19/47/45; NeoPixel uses GPIO 48 (conflict resolved in v3.18.3).
- Motion sensor uses GPIO 46; Distance ECHO uses GPIO 6; Light sensor uses GPIO 4.
- TFT occupies GPIO 7–13; GPS uses GPIO 17–18. Avoid overlapping these ranges when attaching external modules.

### ESP32 Classic Conflicts
- Avoid LEDs/peripherals on strapping pins (GPIO 0/2/4/5/12/15) to prevent boot failures.
- Buttons now use GPIO 32/33 with internal pull-up (no external resistor needed).
- PWM/Buzzer uses GPIO 5 (strapping pin) — acceptable if LOW during boot.
- Protect UART0: GPIO 1 (TX0) and GPIO 3 (RX0) reserved for USB-UART bridge.
- TFT uses GPIO 18/23/27/14/25/32; avoid sharing these with other SPI devices unless chip select is managed.
- ADC2 pins are unusable with Wi-Fi active; prefer ADC1 (GPIO 32-39) for analog sensing.
- Input-only pins (34–39) cannot drive LEDs or outputs; GPIO 36/39 used for GPS PPS and light sensor.

---

## How to Change Pins

### Static Configuration (Compile-time)
1. Edit `include/board_config.h`
2. Modify pin definitions in the appropriate `#if defined(TARGET_ESP32_S3)` or `#elif defined(TARGET_ESP32_CLASSIC)` block
3. Rebuild firmware: `pio run -e <environment>`

### Dynamic Configuration (Runtime)
- **Web Interface:** Sensors section allows changing DHT, light sensor, HC-SR04, motion sensor, and buzzer pins without rebuilding
- **Serial Console:** Pin configuration commands available
- Changes persist in firmware memory until reset

---

## Selecting Build Environment

### Option 1: platformio.ini
Edit the `default_envs` line:
```ini
[platformio]
default_envs = esp32s3_n16r8  ; Change to esp32s3_n8r8 or esp32devkitc
```

### Option 2: Command Line
```bash
# Build specific environment
pio run -e esp32s3_n16r8
pio run -e esp32s3_n8r8
pio run -e esp32devkitc

# Upload to specific environment
pio run -e esp32devkitc --target upload
```

---

## Hardware Wiring Tips

### HC-SR04 Ultrasonic Sensor
- **Power:** 5V, GND
- **TRIG:** Direct connection (ESP32 output 3.3V is sufficient)
- **ECHO:** Use voltage divider (5V → 3.3V) or level shifter
  - Resistor divider: 1kΩ (ECHO to ESP) + 2kΩ (ESP to GND)

### TFT ST7789 Display
- **Power:** 3.3V (check display module specifications)
- **Backlight:** Connect BL pin through 100Ω resistor
- **SPI Speed:** 40-80 MHz typical

### DHT Sensor (DHT11/DHT22)
- **Power:** 3.3V or 5V
- **Pull-up:** 4.7k-10kΩ resistor from DATA to VCC
- **Capacitor:** 100nF decoupling capacitor recommended

### NeoPixel / WS2812B LEDs
- **Power:** 5V for full brightness
- **Data:** 3.3V logic compatible, but 5V preferred for reliability
- **Current:** ~60mA per LED at full white

---

## Version History
- **v3.21.0:** Complete ESP32 Classic pin mapping revision — 11 changes to resolve boot issues (strapping pins GPIO 4/12/15), USB-UART communication (protect GPIO 1/3), and buttons (GPIO 32/33 with internal pull-up instead of 34/35 input-only). Added detailed safety reminders in `board_config.h`. Hardware migration required for ESP32 Classic users. See `docs/PIN_MAPPING_CHANGES_FR.md` for complete numbered changelog.
- **v3.20.2:** Web UI pin references now dynamically sourced from `board_config.h`; web interface always displays correct compiled target pins.
- **v3.20.1:** ESP32-S3 I2C moved to 15/16 and RGB Red to 21 to free USB D-/D+ (19/20) and stabilize OTG; docs aligned.
- **v3.20.1:** Pin mapping moved to `board_config.h`; ESP32-S3 buttons set to GPIO 38/39 to avoid upload/reset conflicts; docs aligned.
- **v3.18.3:** Resolved GPIO 48 conflict between NeoPixel and RGB LED on ESP32-S3; complete S3 pin mapping reorganization.
- **v3.17.1:** Refreshed ESP32-S3 and Classic pin mappings (GPS, TFT, RGB, sensors, buttons) and aligned documentation.
- **v3.15.1:** Critical memory fix for ESP32 Classic web UI.
- **v3.15.0:** Multi-environment support with dedicated pin mappings.
- **v3.12.3:** HC-SR04 defaults set to TRIG=16, ECHO=17 (legacy).
- **v3.11.3:** TFT backlight corrected to GPIO 15 (legacy change).
- **v3.11.0:** TFT ST7789 display support added.
