# Pin Mapping — Quick Reference

- **Supported boards:**
  - ESP32-S3-DevKitC-1 N16R8 (16MB Flash, 8MB PSRAM) — `esp32s3_n16r8`
  - ESP32-S3-DevKitC-1 N8R8 (8MB Flash, 8MB PSRAM) — `esp32s3_n8r8`
  - ESP32-DevKitC (4MB Flash, no PSRAM) — `esp32devkitc`
- Firmware: ESP32 Diagnostic Suite v3.12.3
- **Important (ESP32-S3 only):** On ESP32-S3 OPI variants, GPIO 35..48 are reserved by the octal memory interface. Do not use them for peripherals.
- **Note:** ESP32 classic (DevKitC) does not have this restriction.

## Defaults (config.h)
- Built-in LED: `-1` (disabled)
- NeoPixel: pin `-1`, count `8`, brightness `50`
- RGB LED (separate): `R=14`, `G=13`, `B=18`
- Buzzer: `3`
- I2C (primary): `SDA=21`, `SCL=20`
- I2C (secondary, optional): `SDA=25`, `SCL=26`
- DHT (temp/humidity): `19`
- Light sensor (ADC): `4`
- HC‑SR04: `TRIG=16`, `ECHO=17`
- PIR motion: `39`
- PWM test: `27`
- TFT ST7789: `MOSI=11`, `SCLK=12`, `CS=10`, `DC=9`, `RST=7`, `BL=15`
- GPS (UART2): `RXD=8` (GPS TX → ESP RX), `TXD=5` (GPS RX → ESP TX), `PPS=38`

## ESP32‑S3 Notes
- Reserved (OPI): `GPIO 35..48` → do not assign to sensors/LEDs.
- Strap/special: `GPIO 0`, `GPIO 45` are strapping; avoid if unsure.
- Input-only pins exist on ESP32 (not S3 OPI list), but prefer general‑purpose pins for ECHO.
- USB: native USB uses internal PHY; avoid repurposing if CDC is enabled.

## HC‑SR04 Wiring Tips
- Power: +5V, GND.
- ECHO to ESP32 at 3.3V (use a resistor divider 5V→3.3V).
- TRIG is output-capable; ECHO is input-capable.
- Recommended defaults: `TRIG=16`, `ECHO=17`.

## Conflicts to Avoid
- Do not use `35..48` on S3 OPI boards (flash/psram).
- Avoid `GPIO 0` for sensors (boot strap).
- If TFT is enabled, keep `MOSI=11`, `SCLK=12`, `CS=10`, `DC=9`, `RST=7`, `BL=15` free.

## How to Change Pins
- Static defaults: edit `include/config.h`.
- At runtime (UI): Sensors section allows changing DHT, light, distance (HC‑SR04) and motion pins without rebuilding.
