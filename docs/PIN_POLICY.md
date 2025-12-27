# GPIO Pin Mapping Policy (v3.32.0)

**ESP32 Diagnostic Suite - Version 3.32.0**
> WARNING: This document reflects the GPIO mapping policy for firmware v3.32.0. All assignments are synchronized with `include/board_config.h`. See also [RELEASE_NOTES_3.32.0.md].

---

## 🎯 Purpose of This Document

This document explains the **GPIO pin mapping policy** for the ESP32 Diagnostic Suite project. If you're a beginner or contributing to this project, it's **critical** to understand how pin assignments work to avoid breaking the hardware configuration.

---

## 📍 The Golden Rule: `board_config.h` is the Single Source of Truth

### What Does This Mean?

In this project, **all GPIO pin assignments** are defined in **one file and one file only**:

```
include/board_config.h
```

This file contains the **reference mapping** for all GPIO pins used by:
- LED indicators (RGB, NeoPixel)
- Sensors (DHT, motion, light, distance)
- Communication interfaces (I2C, SPI, UART/GPS)
- TFT display
- Buttons
- Buzzer and PWM outputs

### Why Is This Important?

**Problem:** In many projects, GPIO pins are defined in multiple places (main code, config files, headers, etc.). This creates:
- **Confusion**: Which definition is correct?
- **Bugs**: Changing a pin in one place but forgetting others
- **Hardware conflicts**: Two peripherals trying to use the same GPIO

**Solution:** By having **ONE authoritative source** (`board_config.h`), we eliminate these problems entirely.

---

## 🔧 How It Works

### 1. Pin Definitions in `board_config.h`

The file defines pins differently for each ESP32 variant:

```cpp
#if defined(TARGET_ESP32_S3)
  // ESP32-S3 DevKitC-1 pins
  #define PIN_GPS_RXD        18
  #define PIN_GPS_TXD        17
  #define RGB_LED_PIN_R      21
  #define RGB_LED_PIN_G      41
  #define RGB_LED_PIN_B      42
  #define PWM_PIN            20
  #define BUZZER_PIN         6
  // ... more pins ...

#elif defined(TARGET_ESP32_CLASSIC)
  // ESP32 Classic DevKit pins
  #define PIN_GPS_RXD        16
  #define PIN_GPS_TXD        17
  #define RGB_LED_PIN_R      13
  #define RGB_LED_PIN_G      26
  #define RGB_LED_PIN_B      33
  #define PWM_PIN            4
  #define BUZZER_PIN         19
  // ... more pins ...
#endif
```

### 2. Two-Layer Architecture (v3.25.0)

**Current Design:** GPIO pins use a **two-layer architecture** with distinct naming conventions:

**Layer 1: Compile-Time Defaults (UPPERCASE in `board_config.h`)**
```cpp
#define I2C_SDA       15  // SDA pin default
#define I2C_SCL       16  // SCL pin default
#define RGB_LED_PIN_R 21  // RGB Red channel default
```

**Layer 2: Runtime Variables (lowercase in `main.cpp`)**
```cpp
int i2c_sda = I2C_SDA;          // Modifiable at runtime via Web UI
int i2c_scl = I2C_SCL;          // Modifiable at runtime via Web UI
int rgb_led_pin_r = RGB_LED_PIN_R;  // Modifiable at runtime via Web UI
```

**Key Insight:** Using different naming conventions (UPPERCASE vs lowercase) prevents the preprocessor from expanding lowercase variable names, eliminating conflicts while maintaining both layers.

**Benefits:**
- ✅ **Dynamic pin remapping works** - Users can change pins via Web UI without recompilation
- ✅ **No preprocessor conflicts** - Different names prevent macro expansion issues
- ✅ **Clear distinction** - UPPERCASE = compile-time defaults, lowercase = runtime active pins
- ✅ **Performance** - Compiler optimizes constant initialization
- ✅ **Hardware flexibility** - Test different pin configurations easily

### 3. Web Interface Behavior (v3.25.0)

The Web UI fully supports **dynamic pin remapping**:
- **Runtime pin changes work** - Modify GPIO pins via Web UI without recompilation
- **Configuration handlers active** - All pin configuration endpoints functional:
  - I2C pins (OLED and environmental sensors)
  - RGB LED pins (R, G, B channels)
  - Buzzer, DHT sensor, Light sensor
  - Distance sensor (Trigger and Echo)
  - Motion sensor
- **Changes take effect immediately** - No firmware upload required

**To change default pins permanently**, you can:
  1. Edit `include/board_config.h` (UPPERCASE defines)
  2. Recompile and upload

**To change pins temporarily for testing**:
  1. Use Web UI pin configuration
  2. Changes persist until device reboot

```cpp
// In web_interface.h - JavaScript injection
js += F("const i2c_sda=");
js += String(i2c_sda);  // Injects current runtime value (lowercase)
```

---

## ⚠️ Critical Rules for Developers

### ✅ DO:

1. **Use lowercase runtime variables in code**
   ```cpp
   // GOOD - uses lowercase runtime variable (can be changed via Web UI)
   pinMode(rgb_led_pin_r, OUTPUT);
   digitalWrite(buzzer_pin, HIGH);
   ```

2. **Include board_config.h and declare extern for runtime variables**
   ```cpp
   // GOOD
   #include "board_config.h"
   extern int pwm_pin;  // Runtime variable from main.cpp
   analogWrite(pwm_pin, 128);
   ```

3. **Initialize runtime variables from UPPERCASE defines**
   ```cpp
   // GOOD - in main.cpp
   int i2c_sda = I2C_SDA;  // Initialize from board_config.h default
   ```

4. **Check pin availability before using**
   ```cpp
   // GOOD
   #ifdef DHT_PIN
     extern int dht_pin;
     dht.begin(dht_pin);
   #endif
   ```

### ❌ DON'T:

1. **Never hardcode GPIO numbers**
   ```cpp
   // BAD - breaks multi-board support
   pinMode(21, OUTPUT);  // What if we're on ESP32 Classic?
   ```

2. **Never redefine pins in other files**
   ```cpp
   // BAD - creates conflict with board_config.h
   #define RGB_LED_PIN_R 13  // Already defined elsewhere!
   ```

3. **Never use ambiguous names**
   ```cpp
   // BAD - is this from board_config.h or elsewhere?
   #define LED_PIN 48  // Use NEOPIXEL_PIN instead
   ```

---

## 🔍 Pin Naming Conventions

### Prefix Meanings:

- **`PIN_`**: Fixed hardware pins (buttons, GPS)
  - Example: `PIN_GPS_RXD`, `PIN_BUTTON_1`, `PIN_BUTTON_BOOT`
  - Used for hardware-specific connections

- **No prefix**: Sensor and peripheral pins
  - Example: `RGB_LED_PIN_R`, `DHT_PIN`, `BUZZER_PIN`
  - Defined per board variant in `board_config.h`

### Special Cases:

- **`NEOPIXEL_PIN`**: Built-in NeoPixel on ESP32-S3 (GPIO 48)
- **`TFT_*`**: TFT display SPI pins
- **`I2C_SDA/SCL`**: I2C bus pins

---

## 🧪 Example: Adding a New Sensor

Let's say you want to add a **new ultrasonic sensor** on a different pin.

### Step 1: Define in `board_config.h`

```cpp
#if defined(TARGET_ESP32_S3)
  #define ULTRASONIC_TRIG_PIN 14
  #define ULTRASONIC_ECHO_PIN 21
#elif defined(TARGET_ESP32_CLASSIC)
  #define ULTRASONIC_TRIG_PIN 23
  #define ULTRASONIC_ECHO_PIN 34
#endif
```

### Step 2: Use directly in your code

```cpp
#include "board_config.h"

void initUltrasonicSensor() {
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
}
```

### Step 3: Compile and upload

```cpp
void setupUltrasonic() {
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
}

long measureDistance() {
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  return duration * 0.034 / 2;  // Convert to cm
}
```

### Step 4: Expose in Web UI (optional)

Add to JavaScript injection in `web_interface.h`:

```cpp
js += F(";const ULTRASONIC_TRIG_PIN=");
js += String(ULTRASONIC_TRIG_PIN);
js += F(";const ULTRASONIC_ECHO_PIN=");
js += String(ULTRASONIC_ECHO_PIN);
```

---

## 🛡️ Safety Considerations

### Hardware Protection

`board_config.h` includes **detailed safety comments** for each pin:

```cpp
#define DEFAULT_DHT_PIN 5  // DHT. **Pull-Up 10kΩ vers 3.3V**.
#define PIN_GPS_RXD     18 // GPS TX. **Si 5V, Diviseur de tension OBLIGATOIRE.**
```

**Always read these comments** before connecting hardware!

### Common Mistakes to Avoid:

1. **5V signals to 3.3V pins** → Use voltage divider
2. **Missing pull-up resistors** → I2C won't work
3. **Strapping pins** (GPIO 0, 46) → Boot failures
4. **Input-only pins** (34-39 on Classic) → Can't use as outputs

---

## 📚 Related Documentation

- **[PIN_MAPPING.md](PIN_MAPPING.md)** - Complete pin tables for each board
- **[CONFIG.md](CONFIG.md)** - Configuration file overview
- **[ARCHITECTURE.md](ARCHITECTURE.md)** - System architecture details

---

## ✨ Summary

1. **`board_config.h` is the ONLY place to define GPIO pin defaults (UPPERCASE)**
2. **Use lowercase runtime variables** in code for dynamic pin remapping
3. Never hardcode GPIO numbers in your code
4. **Two-layer architecture**: UPPERCASE defines (compile-time) → lowercase variables (runtime)
5. Follow naming conventions (UPPERCASE for defaults, lowercase for runtime)
6. Always read safety comments before wiring hardware

By following this policy, you ensure:
- ✅ Multi-board compatibility (ESP32-S3 / Classic)
- ✅ Dynamic pin remapping via Web UI
- ✅ No preprocessor conflicts
- ✅ No pin conflicts or duplicates
- ✅ Easy maintenance and debugging
- ✅ Safe hardware operation

---

**Questions?** Check [FAQ.md](FAQ.md) or open an issue on GitHub.
