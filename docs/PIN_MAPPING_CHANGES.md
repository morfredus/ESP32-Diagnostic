# ESP32 Classic Pin Mapping Changes - Boot Issues and USB Communication Resolution

## Date: December 9, 2025
## Version: 3.21.0

---

## Context

The old ESP32 Classic (DevKitC) pin mapping had several critical issues:
- **Boot problems**: Use of strapping pins (GPIO 0, 2, 4, 5, 12, 15) without precautions
- **USB-UART conflicts**: GPIO 1 (TX0) and GPIO 3 (RX0) used for external peripherals
- **Inadequate buttons**: GPIO 34 and 35 used as buttons but they are input-only pins
- **RGB LED on strapping pins**: LEDs connected to GPIO 12, 14, 15 causing forced states at boot

These issues could prevent booting, block flashing, or cause unpredictable behavior.

---

## Numbered Pin Mapping Changes

### 1️⃣ **GPS PPS: GPIO 4 → GPIO 36**
**Old:** `#define PIN_GPS_PPS 4`
**New:** `#define PIN_GPS_PPS 36`

**Reason:** GPIO 4 is a **critical strapping pin** for boot (controls SDIO mode). A level forced by GPS PPS could prevent startup. GPIO 36 (VP) is a dedicated input, ideal for a PPS (Pulse Per Second) signal.

---

### 2️⃣ **TFT CS (Chip Select): GPIO 19 → GPIO 27**
**Old:** `#define TFT_CS 19`
**New:** `#define TFT_CS 27`

**Reason:** GPIO 19 is close to UART0 pins (USB-UART) and could cause interference during flashing. GPIO 27 is safer, with no conflicts with critical interfaces.

---

### 3️⃣ **TFT DC (Data/Command): GPIO 27 → GPIO 14**
**Old:** `#define TFT_DC 27`
**New:** `#define TFT_DC 14`

**Reason:** Position swap with CS for better wiring organization. GPIO 14 remains a strapping pin but is acceptable for a control signal (DC) as it's only active after boot.

---

### 4️⃣ **TFT RST (Reset): GPIO 26 → GPIO 25**
**Old:** `#define TFT_RST 26`
**New:** `#define TFT_RST 25`

**Reason:** GPIO 26 was close to GPIO 27 (potential conflicts). GPIO 25 offers better physical grouping of TFT pins and avoids interference with ADC2 during Wi-Fi usage.

---

### 5️⃣ **TFT BL (Backlight): GPIO 13 → GPIO 32**
**Old:** `#define TFT_BL 13`
**New:** `#define TFT_BL 32`

**Reason:** GPIO 13 is often used for the internal LED on some boards and can cause conflicts. GPIO 32 is safer and can handle backlight without interference.

---

### 6️⃣ **RGB LED Red: GPIO 12 → GPIO 13**
**Old:** `#define DEFAULT_RGB_LED_PIN_R 12`
**New:** `#define DEFAULT_RGB_LED_PIN_R 13`

**Reason:** **GPIO 12 is a critical strapping pin** (MTDI, controls flash voltage). An LED lit at boot can force a wrong level and prevent startup. GPIO 13 is safer for an LED.

---

### 7️⃣ **RGB LED Blue: GPIO 15 → GPIO 25**
**Old:** `#define DEFAULT_RGB_LED_PIN_B 15`
**New:** `#define DEFAULT_RGB_LED_PIN_B 25`

**Reason:** **GPIO 15 is a strapping pin** (MTDO, controls JTAG debug mode). A blue LED lit can block boot. GPIO 25 is not a strapping pin and is perfectly suitable.

---

### 8️⃣ **Button 1: GPIO 34 → GPIO 32**
**Old:** `#define PIN_BUTTON_1 34`
**New:** `#define PIN_BUTTON_1 32`

**Reason:** GPIO 34 is **input-only** (no internal pull-up), requiring an external resistor. GPIO 32 supports `INPUT_PULLUP` internally, simplifying wiring.

---

### 9️⃣ **Distance TRIG (HC-SR04): GPIO 32 → GPIO 27**
**Old:** `#define DEFAULT_DISTANCE_TRIG_PIN 32`
**New:** `#define DEFAULT_DISTANCE_TRIG_PIN 27`

**Reason:** GPIO 32 was reassigned to Button 1. GPIO 27 is a standard output perfect for the ultrasonic sensor TRIG signal.

---

### 🔟 **DHT Temperature/Humidity: GPIO 25 → GPIO 32**
**Old:** `#define DEFAULT_DHT_PIN 25`
**New:** `#define DEFAULT_DHT_PIN 32`

**Reason:** GPIO 25 was reassigned to blue LED. GPIO 32 supports the DHT protocol and can share the pin with Button 1 if managed properly in time (non-simultaneous).

---

### 1️⃣1️⃣ **Button 2: GPIO 35 → GPIO 33**
**Old:** `#define PIN_BUTTON_2 35`
**New:** `#define PIN_BUTTON_2 33`

**Reason:** GPIO 35 is **input-only** (no internal pull-up), requiring an external resistor. GPIO 33 supports `INPUT_PULLUP` internally, simplifying button handling.

---

## Impact Summary

| Component | Old GPIO | New GPIO | Reason |
|-----------|----------|----------|--------|
| GPS PPS | 4 | 36 | Strapping pin → Dedicated input |
| TFT CS | 19 | 27 | UART proximity → Safe GPIO |
| TFT DC | 27 | 14 | Swap with CS |
| TFT RST | 26 | 25 | Better grouping |
| TFT BL | 13 | 32 | Avoid internal LED conflict |
| RGB Red | 12 | 13 | Critical strapping → Safe GPIO |
| RGB Blue | 15 | 25 | Strapping pin → Safe GPIO |
| Button 1 | 34 | 32 | Input-only → Pull-up capable |
| Distance TRIG | 32 | 27 | Reassigned to Button 1 |
| DHT | 25 | 32 | Reassigned to RGB Blue |
| Button 2 | 35 | 33 | Input-only → Pull-up capable |

---

## Hardware Migration Required

**ESP32 Classic users must rewire their hardware to match the new pin mapping.**

⚠️ **Critical**: This change affects **ESP32 Classic ONLY**. ESP32-S3 pin mapping remains unchanged.

---

## Verification Steps

1. **Before flashing**: Review your current wiring against the new mapping
2. **Rewire as needed**: Follow the table above for each affected component
3. **Double-check strapping pins**: Ensure no LEDs or outputs are connected to GPIO 0, 2, 4, 5, 12, 15 at boot
4. **Flash firmware**: Upload v3.21.0 or later
5. **Test all peripherals**: Verify buttons, sensors, RGB LEDs, TFT display

---

## Technical References

For complete pin mapping details:
- `include/board_config.h` - Authoritative pin definitions
- `docs/PIN_MAPPING.md` - Complete pin mapping reference
- `docs/PIN_POLICY.md` - GPIO policy and naming conventions

---

## Safety Reminders

### Strapping Pins (ESP32 Classic)
- **GPIO 0**: BOOT mode (must be HIGH at boot for normal mode)
- **GPIO 2**: Must be LOW at boot for flash download
- **GPIO 4**: SDIO mode control
- **GPIO 5**: SDIO timing mode
- **GPIO 12**: Flash voltage selection (MTDI)
- **GPIO 15**: Debug mode selection (MTDO)

### Best Practices
- Keep strapping pins floating or with pull-ups/pull-downs as required
- Avoid connecting LEDs directly to strapping pins
- Use series resistors (220-470Ω) for all LEDs
- Respect 3.3V voltage levels (not 5V compatible)
- Use current-limiting for all outputs

---

**Last Updated:** December 9, 2025
**Version:** 3.21.0
