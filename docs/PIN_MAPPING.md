# Pin Mapping — Quick Reference (v3.19.0)

## Supported Environments

The firmware supports three distinct build environments, each with optimized pin configurations:

1. **`esp32s3_n16r8`** (default) — ESP32-S3 DevKitC-1 N16R8 (16MB Flash, 8MB PSRAM OPI/QSPI)
2. **`esp32s3_n8r8`** — ESP32-S3 DevKitC-1 N8R8 (8MB Flash, 8MB PSRAM)
3. **`esp32devkitc`** — ESP32 Classic DevKitC (4MB Flash, no PSRAM)

Pin mappings are defined in `include/config.h` using conditional compilation based on `TARGET_ESP32_S3` or `TARGET_ESP32_CLASSIC`.

---

## ESP32-S3 Pin Configuration (N16R8 / N8R8)

**Applies to:** `esp32s3_n16r8`, `esp32s3_n8r8`

### I2C Interface
- **SDA:** GPIO 21
- **SCL:** GPIO 20

### Buttons
- **Button 1:** GPIO 38
- **Button 2:** GPIO 39

### RGB LED (Separate pins)
- **Red:** GPIO 19
- **Green:** GPIO 47
- **Blue:** GPIO 45

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
- GPIO 48 is now dedicated to the NeoPixel (conflict with RGB LED resolved).
- RGB LED uses GPIO 19/47/45 (Red/Green/Blue respectively).
- Strapping pins: GPIO 0, GPIO 45, GPIO 46 — keep stable during boot.
- USB CDC is enabled; keep I2C (GPIO 21/20) free for sensors.

---

## ESP32 Classic Pin Configuration (DevKitC)

**Applies to:** `esp32devkitc`

### I2C Interface
- **SDA:** GPIO 21
- **SCL:** GPIO 22 (different from S3!)

### Buttons
- **Button 1:** GPIO 34 (Input only)
- **Button 2:** GPIO 35 (Input only)

### RGB LED (Separate pins)
- **Red:** GPIO 12
- **Green:** GPIO 14
- **Blue:** GPIO 15

### Sensors
- **DHT (Temperature/Humidity):** GPIO 25
- **Light Sensor (ADC):** GPIO 2
- **HC-SR04 Distance Sensor:** TRIG GPIO 32 / ECHO GPIO 33
- **PIR Motion Sensor:** GPIO 36 (VP, Input only)
- **Buzzer:** GPIO 5
- **PWM Test:** GPIO 5

### TFT ST7789 Display (240x240)
- **MOSI:** GPIO 23
- **SCLK:** GPIO 18
- **CS:** GPIO 19
- **DC:** GPIO 27
- **RST:** GPIO 26
- **Backlight (BL):** GPIO 13

### GPS Module (UART2)
- **RXD:** GPIO 16 (RX2, GPS TX → ESP RX)
- **TXD:** GPIO 17 (TX2, GPS RX → ESP TX)
- **PPS:** GPIO 4

### NeoPixel / WS2812B
- **Pin:** -1 (disabled by default)
- **Count:** 8 LEDs
- **Brightness:** 50 (0-255)

### ESP32 Classic Important Notes
- **Input-Only Pins:** GPIO 34, 35, 36, 39 (VP/VN) — can only read, cannot output.
- **Strapping Pins:** GPIO 0, GPIO 2, GPIO 5, GPIO 12, GPIO 15 — check state during boot.
- **Flash Pins:** GPIO 6-11 are connected to SPI Flash — do not use.
- **ADC2 Limitation:** ADC2 (GPIO 0, 2, 4, 12-15, 25-27) cannot be used when WiFi is active.

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
- Buttons on GPIO 34/35 are input only; keep pull-ups in place.
- PWM/Buzzer uses GPIO 5 (strapping pin) — keep LOW during boot if hardware is attached.
- TFT uses GPIO 18/19/23 plus 13/26/27; avoid sharing these with other SPI devices unless chip select is managed.
- ADC2 pins are unusable with Wi-Fi active; prefer ADC1 for analog sensing.
- Input-only pins (34–39) cannot drive LEDs or outputs.

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
- **v3.19.0:** Pin mapping moved to `board_config.h`; ESP32-S3 buttons set to GPIO 38/39 to avoid upload/reset conflicts; docs aligned.
- **v3.18.3:** Resolved GPIO 48 conflict between NeoPixel and RGB LED on ESP32-S3; complete S3 pin mapping reorganization.
- **v3.17.1:** Refreshed ESP32-S3 and Classic pin mappings (GPS, TFT, RGB, sensors, buttons) and aligned documentation.
- **v3.15.1:** Critical memory fix for ESP32 Classic web UI.
- **v3.15.0:** Multi-environment support with dedicated pin mappings.
- **v3.12.3:** HC-SR04 defaults set to TRIG=16, ECHO=17 (legacy).
- **v3.11.3:** TFT backlight corrected to GPIO 15 (legacy change).
- **v3.11.0:** TFT ST7789 display support added.
