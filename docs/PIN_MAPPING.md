# Pin Mapping — Quick Reference (v3.15.0)

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
- **Button 1:** GPIO 1
- **Button 2:** GPIO 2

### RGB LED (Separate pins)
- **Red:** GPIO 14
- **Green:** GPIO 13
- **Blue:** GPIO 18

### Sensors
- **DHT (Temperature/Humidity):** GPIO 19
- **Light Sensor (ADC):** GPIO 4
- **HC-SR04 Distance Sensor:**
  - **TRIG:** GPIO 16
  - **ECHO:** GPIO 17
- **PIR Motion Sensor:** GPIO 39
- **Buzzer:** GPIO 3
- **PWM Test:** GPIO 27

### TFT ST7789 Display (240x240)
- **MOSI:** GPIO 11
- **SCLK:** GPIO 12
- **CS:** GPIO 10
- **DC:** GPIO 9
- **RST:** GPIO 7
- **Backlight (BL):** GPIO 15

### GPS Module (UART2)
- **RXD:** GPIO 8 (GPS TX → ESP RX)
- **TXD:** GPIO 5 (GPS RX → ESP TX)
- **PPS:** GPIO 38

### NeoPixel / WS2812B
- **Pin:** -1 (disabled by default, can be set to GPIO 48 if needed)
- **Count:** 8 LEDs
- **Brightness:** 50 (0-255)

### ESP32-S3 Important Notes
- **Reserved GPIO (OPI variants):** GPIO 35..48 are used by octal PSRAM/Flash interface. Do not use for peripherals.
- **Strapping Pins:** GPIO 0, GPIO 45, GPIO 46 — avoid for sensors if unsure.
- **USB:** Native USB-OTG uses internal PHY; avoid GPIO 19/20 conflicts if USB CDC enabled.

---

## ESP32 Classic Pin Configuration (DevKitC)

**Applies to:** `esp32devkitc`

### I2C Interface
- **SDA:** GPIO 21
- **SCL:** GPIO 22 (different from S3!)

### Buttons
- **Button 1:** GPIO 0 (BOOT button)
- **Button 2:** GPIO 35 (Input only)

### RGB LED (Separate pins)
- **Red:** GPIO 25
- **Green:** GPIO 26
- **Blue:** GPIO 27

### Sensors
- **DHT (Temperature/Humidity):** GPIO 4
- **Light Sensor (ADC):** GPIO 34 (Input only, ADC1_CH6)
- **HC-SR04 Distance Sensor:**
  - **TRIG:** GPIO 5
  - **ECHO:** GPIO 18
- **PIR Motion Sensor:** GPIO 36 (VP, Input only)
- **Buzzer:** GPIO 13
- **PWM Test:** GPIO 14

### TFT ST7789 Display (240x240)
- **MOSI:** GPIO 23 (VSPI)
- **SCLK:** GPIO 18 (VSPI, shared with ECHO)
- **CS:** GPIO 15
- **DC:** GPIO 2
- **RST:** GPIO 4 (shared with DHT sensor)
- **Backlight (BL):** GPIO 32

### GPS Module (UART2)
- **RXD:** GPIO 16 (RX2, GPS TX → ESP RX)
- **TXD:** GPIO 17 (TX2, GPS RX → ESP TX)
- **PPS:** GPIO 39 (VN, Input only)

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
- Avoid GPIO 35-48 (OPI PSRAM/Flash)
- TFT backlight (GPIO 15) vs NeoPixel (GPIO 48) — resolved in v3.11.3
- GPS and TFT use different SPI/UART buses — no conflict

### ESP32 Classic Conflicts
- **TFT SCLK (GPIO 18) shares with HC-SR04 ECHO** — use one or the other
- **TFT RST (GPIO 4) shares with DHT sensor** — sequence carefully or reassign
- **ADC2 pins unusable with WiFi** — use ADC1 for sensors when WiFi active
- **Input-only pins (34-39)** — cannot drive LEDs or outputs

---

## How to Change Pins

### Static Configuration (Compile-time)
1. Edit `include/config.h`
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
- **v3.15.0:** Multi-environment support with dedicated pin mappings
- **v3.12.3:** HC-SR04 default pins set to TRIG=16, ECHO=17
- **v3.11.3:** TFT backlight corrected to GPIO 15 (was 48)
- **v3.11.0:** TFT ST7789 display support added
