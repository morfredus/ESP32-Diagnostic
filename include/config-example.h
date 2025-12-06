/**
 * ESP32 Diagnostic Suite - Configuration Template (v3.18.3)
 *
 * MULTI-ENVIRONMENT SUPPORT:
 * This configuration supports three build environments:
 *   - esp32s3_n16r8 (default): ESP32-S3 with 16MB Flash + 8MB PSRAM
 *   - esp32s3_n8r8: ESP32-S3 with 8MB Flash + 8MB PSRAM
 *   - esp32devkitc: ESP32 Classic with 4MB Flash (no PSRAM)
 *
 * Hardware-specific pin mappings are automatically selected at compile time
 * based on TARGET_ESP32_S3 or TARGET_ESP32_CLASSIC defines (set in platformio.ini).
 *
 * Copy this file to config.h and customize the settings for your setup.
 * The config.h file is excluded from version control to keep your
 * personal configuration private.
 *
 * Command: cp config-example.h config.h
 */

#ifndef CONFIG_H
#define CONFIG_H

// ========== VERSION & HOSTNAME (Common) ==========
// Version is now defined in platformio.ini as PROJECT_VERSION
#define DIAGNOSTIC_HOSTNAME "esp32-diagnostic"
#define DEFAULT_LANGUAGE LANG_EN
#define DIAGNOSTIC_DEBUG 0
#define DIAGNOSTIC_VERBOSE 0
#define DIAGNOSTIC_PREFER_SECURE 0

// =========================================================
//                  PIN MAPPING ESP32-S3 (N16R8 / N8R8)
// =========================================================
#if defined(TARGET_ESP32_S3)

    // --- I2C ---
    #define DEFAULT_I2C_SDA 21
    #define DEFAULT_I2C_SCL 20

    // --- BUTTONS ---
    #define PIN_BUTTON_1     1
    #define PIN_BUTTON_2     2

    // --- RGB LED (Pin 48 now reserved for NeoPixel) ---
    #define DEFAULT_RGB_LED_PIN_R 19
    #define DEFAULT_RGB_LED_PIN_G 47
    #define DEFAULT_RGB_LED_PIN_B 45

    // --- SENSORS ---
    #define DEFAULT_DHT_PIN 5
    #define DEFAULT_LIGHT_SENSOR_PIN 4
    #define DEFAULT_DISTANCE_TRIG_PIN 3
    #define DEFAULT_DISTANCE_ECHO_PIN 6
    #define DEFAULT_MOTION_SENSOR_PIN 46
    #define DEFAULT_BUZZER_PIN 14
    #define DEFAULT_PWM_PIN 14

    // --- TFT ST7789 (S3 Specific SPI) ---
    #define TFT_MOSI            11
    #define TFT_SCLK            12
    #define TFT_CS              10
    #define TFT_DC              9
    #define TFT_RST             13
    #define TFT_BL              7

    // --- GPS ---
    #define PIN_GPS_RXD         18
    #define PIN_GPS_TXD         17
    #define PIN_GPS_PPS         8

// =========================================================
//                  PIN MAPPING ESP32 CLASSIC (DevKitC)
// =========================================================
#elif defined(TARGET_ESP32_CLASSIC)

    // --- I2C (Standard ESP32) ---
    #define DEFAULT_I2C_SDA 21
    #define DEFAULT_I2C_SCL 22  // SCL is typically 22 on Classic

    // --- BUTTONS ---
    // Note: GPIO 1/2 are TX/RX on Classic, using alternative pins
    #define PIN_BUTTON_1     0   // BOOT button
    #define PIN_BUTTON_2     35  // Input only

    // --- RGB LED ---
    #define DEFAULT_RGB_LED_PIN_R 25
    #define DEFAULT_RGB_LED_PIN_G 26
    #define DEFAULT_RGB_LED_PIN_B 27

    // --- SENSORS ---
    #define DEFAULT_DHT_PIN 4
    #define DEFAULT_LIGHT_SENSOR_PIN 34 // Analog Input Only
    #define DEFAULT_DISTANCE_TRIG_PIN 5
    #define DEFAULT_DISTANCE_ECHO_PIN 18
    #define DEFAULT_MOTION_SENSOR_PIN 36 // VP - Input Only
    #define DEFAULT_BUZZER_PIN 13
    #define DEFAULT_PWM_PIN 14

    // --- TFT ST7789 (Standard VSPI) ---
    #define TFT_MOSI            23
    #define TFT_SCLK            18
    #define TFT_CS              15
    #define TFT_DC              2
    #define TFT_RST             4
    #define TFT_BL              32

    // --- GPS ---
    #define PIN_GPS_RXD         16 // RX2
    #define PIN_GPS_TXD         17 // TX2
    #define PIN_GPS_PPS         39 // VN - Input Only

#else
    #error "No target defined! Check platformio.ini (TARGET_ESP32_S3 or TARGET_ESP32_CLASSIC)"
#endif

// =========================================================
//            COMMON CONFIGURATION (All Environments)
// =========================================================

// --- I2C Common ---
#define ENABLE_I2C_SCAN true

// --- OLED Common ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define DEFAULT_OLED_ROTATION 0      // 0, 1, 2, or 3 (90° increments)

// --- LED Common ---
#define DEFAULT_BUILTIN_LED_PIN -1
#define DEFAULT_NEOPIXEL_PIN 48 // Now enabled on ESP32-S3 pin 48
#define DEFAULT_NEOPIXEL_COUNT 1
#define DEFAULT_NEOPIXEL_BRIGHTNESS 50
#define CUSTOM_LED_PIN -1

// --- Sensors Common ---
#define DEFAULT_DHT_SENSOR_TYPE 22   // 11 for DHT11, 22 for DHT22

// --- TFT Common ---
#define ENABLE_TFT_DISPLAY  true
#define TFT_WIDTH           240
#define TFT_HEIGHT          240
#define TFT_ROTATION        2
#define TFT_BACKLIGHT_PWM   255

// --- GPS Common ---
#define GPS_UPDATE_RATE     1000
#define GPS_TIMEOUT         5000
#define GPS_FIX_TIMEOUT     60000
#define HDOP_GOOD_THRESHOLD 2.0

// ========== GPIO TEST CONFIGURATION ==========
#define ENABLE_GPIO_TEST false

// ========== OPTIONAL FEATURES ==========
// Enable MQTT reporting bridge (requires MQTT broker configuration)
#define ENABLE_MQTT_BRIDGE false
#define MQTT_BROKER "mqtt.example.com"
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_TOPIC_PREFIX "esp32-diagnostic"

// Enable Bluetooth/BLE diagnostics (if supported by hardware)
#define ENABLE_BLE_DIAGNOSTICS false

// Enable SPI bus scanning
#define ENABLE_SPI_SCAN true

// Enable memory stress testing (can cause temporary slowdowns)
#define ENABLE_MEMORY_STRESS_TEST true

// Enable CPU benchmarking
#define ENABLE_CPU_BENCHMARK true

// ========== WEB SERVER CONFIGURATION ==========
#define WEB_SERVER_PORT 80

// Enable CORS for external web clients
#define ENABLE_CORS false

// Maximum number of simultaneous web clients
#define MAX_WEB_CLIENTS 4

// ========== EXPORT CONFIGURATION ==========
// Enable automatic export generation after boot
#define ENABLE_AUTO_EXPORT false
#define AUTO_EXPORT_DELAY_SECONDS 30

// Export file formats to generate
#define ENABLE_TXT_EXPORT true
#define ENABLE_JSON_EXPORT true
#define ENABLE_CSV_EXPORT true

// ========== PERFORMANCE TUNING ==========
// Task stack sizes (bytes)
#define BUILTIN_LED_TASK_STACK 2048
#define NEOPIXEL_TASK_STACK 2048
#define OLED_TASK_STACK 4096
#define RGB_LED_TASK_STACK 2048
#define BUZZER_TASK_STACK 2048

// Task priorities (0 = lowest, higher numbers = higher priority)
#define HARDWARE_TEST_TASK_PRIORITY 1
#define WEB_SERVER_TASK_PRIORITY 2

// ========== WATCHDOG CONFIGURATION ==========
// Task watchdog timeout (seconds)
#define TASK_WATCHDOG_TIMEOUT_S 10

// Disable watchdog for idle tasks (recommended for diagnostics)
#define DISABLE_IDLE_TASK_WDT true

// ========== MULTI-ENVIRONMENT NOTES ==========
// Version 3.18.3 fixes ESP32-S3 pin mapping conflicts (NeoPixel vs RGB LED on pin 48).
// Version 3.15.1 introduces critical memory optimizations for ESP32 Classic.
// JavaScript content is now streamed in chunks to prevent heap exhaustion.
//
// Hardware-specific pin mappings use conditional compilation:
//   #if defined(TARGET_ESP32_S3)      -> ESP32-S3 pins
//   #elif defined(TARGET_ESP32_CLASSIC) -> ESP32 Classic pins
//
// The TARGET_* defines are automatically set in platformio.ini build_flags.
//
// Select build environment in platformio.ini:
//   default_envs = esp32s3_n16r8   (ESP32-S3 16MB+8MB PSRAM - default)
//   default_envs = esp32s3_n8r8    (ESP32-S3 8MB+8MB PSRAM)
//   default_envs = esp32devkitc    (ESP32 Classic 4MB no PSRAM)
//
// See docs/BUILD_AND_DEPLOY.md for environment selection details.

#endif // CONFIG_H
