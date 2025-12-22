# GPIO Pin Mapping Policy

**ESP32 Diagnostic Suite - Version 3.23.0**

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
  #define PIN_GPS_RXD           18
  #define PIN_GPS_TXD           17
  #define DEFAULT_RGB_LED_PIN_R 21
  #define DEFAULT_RGB_LED_PIN_G 41
  #define DEFAULT_RGB_LED_PIN_B 42
  #define DEFAULT_PWM_PIN       20
  #define DEFAULT_BUZZER_PIN    6
  // ... more pins ...

#elif defined(TARGET_ESP32_CLASSIC)
  // ESP32 Classic DevKit pins
  #define PIN_GPS_RXD           16
  #define PIN_GPS_TXD           17
  #define DEFAULT_RGB_LED_PIN_R 13
  #define DEFAULT_RGB_LED_PIN_G 26
  #define DEFAULT_RGB_LED_PIN_B 33
  #define DEFAULT_PWM_PIN       4
  #define DEFAULT_BUZZER_PIN    19
  // ... more pins ...
#endif
```

### 2. Runtime Variables in `main.cpp`

In `src/main.cpp`, we create **runtime variables** initialized from `board_config.h`:

```cpp
#include "board_config.h"

// Runtime pin variables (initialized from board_config.h)
int RGB_LED_PIN_R = DEFAULT_RGB_LED_PIN_R;
int RGB_LED_PIN_G = DEFAULT_RGB_LED_PIN_G;
int RGB_LED_PIN_B = DEFAULT_RGB_LED_PIN_B;
int PWM_PIN = DEFAULT_PWM_PIN;
int BUZZER_PIN = DEFAULT_BUZZER_PIN;
int DHT_PIN = DEFAULT_DHT_PIN;
// ... more runtime variables ...
```

**Why runtime variables?** They allow **dynamic remapping** through the Web UI without recompiling!

### 3. Web Interface Access

The Web UI can:
- **Display** the current pin assignments
- **Modify** pin assignments at runtime for testing
- **Persist** changes across reboots (future feature)

The pins are injected into JavaScript when the page loads:

```cpp
// In web_interface.h
js += F("const RGB_LED_PIN_R=");
js += String(RGB_LED_PIN_R);  // Injects current value
```

---

## ⚠️ Critical Rules for Developers

### ✅ DO:

1. **Always reference pins from `board_config.h`**
   ```cpp
   // GOOD
   pinMode(DEFAULT_RGB_LED_PIN_R, OUTPUT);
   ```

2. **Use the runtime variables for dynamic pins**
   ```cpp
   // GOOD
   analogWrite(PWM_PIN, 128);  // Uses runtime variable
   ```

3. **Check pin availability before using**
   ```cpp
   // GOOD
   if (DHT_PIN >= 0) {
     dht.begin(DHT_PIN);
   }
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

- **`PIN_`**: Fixed hardware pins (GPS, buttons)
  - Example: `PIN_GPS_RXD`, `PIN_BUTTON_1`
  - **Cannot be changed** at runtime (hardware constraint)

- **`DEFAULT_`**: Default values for runtime-configurable pins
  - Example: `DEFAULT_RGB_LED_PIN_R`, `DEFAULT_DHT_PIN`
  - **Can be changed** at runtime via Web UI

### Special Cases:

- **`NEOPIXEL_PIN`**: Built-in NeoPixel on ESP32-S3 (GPIO 48)
- **`TFT_*`**: TFT display SPI pins
- **`DEFAULT_I2C_SDA/SCL`**: I2C bus pins

---

## 🧪 Example: Adding a New Sensor

Let's say you want to add a **new ultrasonic sensor** on a different pin.

### Step 1: Define in `board_config.h`

```cpp
#if defined(TARGET_ESP32_S3)
  #define DEFAULT_ULTRASONIC_TRIG_PIN 14
  #define DEFAULT_ULTRASONIC_ECHO_PIN 21
#elif defined(TARGET_ESP32_CLASSIC)
  #define DEFAULT_ULTRASONIC_TRIG_PIN 23
  #define DEFAULT_ULTRASONIC_ECHO_PIN 34
#endif
```

### Step 2: Create runtime variables in `main.cpp`

```cpp
int ULTRASONIC_TRIG_PIN = DEFAULT_ULTRASONIC_TRIG_PIN;
int ULTRASONIC_ECHO_PIN = DEFAULT_ULTRASONIC_ECHO_PIN;
```

### Step 3: Use in your code

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

1. **`board_config.h` is the ONLY place to define GPIO pins**
2. Never hardcode GPIO numbers in your code
3. Use runtime variables for dynamic pin remapping
4. Follow naming conventions (`PIN_*` vs `DEFAULT_*`)
5. Always read safety comments before wiring hardware

By following this policy, you ensure:
- ✅ Multi-board compatibility (ESP32-S3 / Classic)
- ✅ No pin conflicts or duplicates
- ✅ Easy maintenance and debugging
- ✅ Safe hardware operation

---

**Questions?** Check [FAQ.md](FAQ.md) or open an issue on GitHub.
