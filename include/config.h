/**
 * ESP32 Diagnostic Suite - Default Configuration
 *
 * This file contains default configuration values for the project.
 * You can modify these values for your specific hardware setup.
 *
 * For reference and documentation of all options, see config-example.h
 */

#ifndef CONFIG_H
#define CONFIG_H

// ========== VERSION & HOSTNAME ==========
// These values should match the project version and can be customized for your deployment
#define DIAGNOSTIC_VERSION "3.9.0"
#define DIAGNOSTIC_HOSTNAME "esp32-diagnostic"

// ========== DEFAULT LANGUAGE ==========
// Set the default interface language
// Options: LANG_EN (English) or LANG_FR (French)
#define DEFAULT_LANGUAGE LANG_EN

// ========== DEBUG OPTIONS ==========
// Enable verbose serial output for troubleshooting
#define DIAGNOSTIC_DEBUG 0        // 0 = disabled, 1 = enabled
#define DIAGNOSTIC_VERBOSE 0      // 0 = normal, 1 = verbose logs

// ========== NETWORK CONFIGURATION ==========
// Prefer secure connections (HTTPS) over HTTP
#define DIAGNOSTIC_PREFER_SECURE 0

// ========== I2C CONFIGURATION ==========
// Default I2C pins for OLED and sensors
// ESP32:     SDA=21, SCL=22
// ESP32-S3:  SDA=21, SCL=20
// ESP32-C3:  SDA=8,  SCL=9
#define DEFAULT_I2C_SDA 21
#define DEFAULT_I2C_SCL 20

// Enable I2C bus scanning on startup
#define ENABLE_I2C_SCAN true

// Enable secondary I2C bus (ESP32 and ESP32-S3 only)
#define ENABLE_SECONDARY_I2C false
#define SECONDARY_I2C_SDA 25
#define SECONDARY_I2C_SCL 26

// ========== OLED DISPLAY CONFIGURATION ==========
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define DEFAULT_OLED_ROTATION 0      // 0, 1, 2, or 3 (90° increments)

// ========== LED CONFIGURATION ==========
// Built-in LED (set to -1 if not available or to disable)
// Common values: 2 (ESP32), 8 (ESP32-S3), -1 (disable)
#define DEFAULT_BUILTIN_LED_PIN -1

// NeoPixel/WS2812B LED Strip
#define DEFAULT_NEOPIXEL_PIN -1              // -1 to disable, or GPIO number
#define DEFAULT_NEOPIXEL_COUNT 8             // Number of LEDs in strip
#define DEFAULT_NEOPIXEL_BRIGHTNESS 50       // 0-255, default brightness
#define CUSTOM_LED_PIN -1                    // Custom LED pin override (-1 to use auto-detection)

// RGB LED (separate R, G, B pins)
#define DEFAULT_RGB_LED_PIN_R 14
#define DEFAULT_RGB_LED_PIN_G 13
#define DEFAULT_RGB_LED_PIN_B 12

// ========== BUZZER CONFIGURATION ==========
#define DEFAULT_BUZZER_PIN 16                // -1 to disable

// ========== SENSOR CONFIGURATION ==========
// DHT Temperature & Humidity Sensor
#define DEFAULT_DHT_PIN 4                    // -1 to disable
#define DEFAULT_DHT_SENSOR_TYPE 11           // 11 for DHT11, 22 for DHT22

// Photoresistor (Light Sensor)
#define DEFAULT_LIGHT_SENSOR_PIN 17          // -1 to disable

// HC-SR04 Ultrasonic Distance Sensor
#define DEFAULT_DISTANCE_TRIG_PIN 7          // -1 to disable
#define DEFAULT_DISTANCE_ECHO_PIN 8

// PIR Motion Sensor
#define DEFAULT_MOTION_SENSOR_PIN 6          // -1 to disable

// ========== GPIO TEST CONFIGURATION ==========
// Enable automatic GPIO testing on startup
#define ENABLE_GPIO_TEST false

// ========== PWM CONFIGURATION ==========
// Default PWM test pin
#define DEFAULT_PWM_PIN 15           // -1 to disable PWM tests

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
#define TASK_WATCHDOG_T