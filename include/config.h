/**
 * ESP32 Diagnostic Suite - Default Configuration
 *
 * This file contains default configuration values for the project.
 * You can modify these values for your specific hardware setup.
 *
 * UPDATED:
 * - Integration of TFT ST7789 Configuration from config_test_gps.h
 * - Integration of GPS Configuration from config_test_gps.h
 * - Pin conflict resolution documented.
 * - ADDED: Voltage recommendations for each component.
 */

#ifndef CONFIG_H
#define CONFIG_H

// ========== VERSION & HOSTNAME ==========
#define DIAGNOSTIC_HOSTNAME "esp32-diagnostic"

// ========== DEFAULT LANGUAGE ==========
// Options: LANG_EN (English) or LANG_FR (French)
#define DEFAULT_LANGUAGE LANG_FR 

// ========== DEBUG OPTIONS ==========
#define DIAGNOSTIC_DEBUG 1        // Enabled for new hardware testing
#define DIAGNOSTIC_VERBOSE 0

// ========== NETWORK CONFIGURATION ==========
#define DIAGNOSTIC_PREFER_SECURE 0

// ========== I2C CONFIGURATION ==========
// ESP32-S3 Default: SDA=21, SCL=20
// VOLTAGE: 3.3V Logic
#define DEFAULT_I2C_SDA 21
#define DEFAULT_I2C_SCL 20

#define ENABLE_I2C_SCAN true
#define ENABLE_SECONDARY_I2C false
#define SECONDARY_I2C_SDA 25
#define SECONDARY_I2C_SCL 26

// ========== TFT DISPLAY CONFIGURATION (NEW) ==========
// Configuration for ST7789 (SPI) from config_test_gps.h
// VOLTAGE: VCC = 3.3V (or 5V if regulator present on module), Logic = 3.3V ONLY
#define ENABLE_TFT_DISPLAY true      // Set to true to enable TFT

#define TFT_WIDTH           240
#define TFT_HEIGHT          240
#define TFT_ROTATION        2        // 0=Portrait, 1=Land, 2=PortInv, 3=LandInv
#define TFT_BACKLIGHT_PWM   255      // 0-255

// SPI Pins for TFT (Fixed from config_test_gps.h)
#define TFT_MOSI            11
#define TFT_SCLK            12       // WARNING: Conflicts with original RGB_LED_B (12)
#define TFT_CS              10
#define TFT_DC              6        // WARNING: Conflicts with original Motion Sensor (6)
#define TFT_RST             7        // WARNING: Conflicts with original Distance_Trig (7)
#define TFT_BL              15       // WARNING: Conflicts with original PWM_Test (15)

// TFT Colors (RGB565 format)
#define TFT_COLOR_BG        0x0000
#define TFT_COLOR_HEADER    0x001F
#define TFT_COLOR_TEXT      0xFFFF
#define TFT_COLOR_VALUE     0x07E0
#define TFT_COLOR_WARNING   0xFFE0
#define TFT_COLOR_ERROR     0xF800
#define TFT_COLOR_SEPARATOR 0x4208

// ========== OLED DISPLAY CONFIGURATION ==========
// Kept for backward compatibility
// VOLTAGE: VCC = 3.3V or 5V, Logic = 3.3V
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define DEFAULT_OLED_ROTATION 0

// ========== LED CONFIGURATION ==========
// Built-in LED
#define DEFAULT_BUILTIN_LED_PIN -1

// NeoPixel/WS2812B LED Strip
// VOLTAGE: VCC = 5V (Recommended) or 3.3V, Logic = 3.3V
#define DEFAULT_NEOPIXEL_PIN 48              // ORIGINALLY: -1. UPDATED: 48
#define DEFAULT_NEOPIXEL_COUNT 1             // Number of LEDs
#define DEFAULT_NEOPIXEL_BRIGHTNESS 50
#define CUSTOM_LED_PIN -1

// RGB LED (separate R, G, B pins)
// VOLTAGE: 3.3V Source (requires resistors)
#define DEFAULT_RGB_LED_PIN_R 14
#define DEFAULT_RGB_LED_PIN_G 13
// Pin 12 is used by TFT_SCLK. Moving Blue to Pin 18.
#define DEFAULT_RGB_LED_PIN_B 18             // ORIGINALLY: 12. MOVED: 18

// ========== BUZZER CONFIGURATION ==========
// VOLTAGE: 3.3V (Passive) or 5V (Active with Transistor). Direct GPIO drive = 3.3V max.
#define DEFAULT_BUZZER_PIN 3                 // ORIGINALLY: 16. UPDATED: 3

// ========== SENSOR CONFIGURATION ==========
// DHT Temperature & Humidity Sensor
// VOLTAGE: 3.3V (Recommended for ESP32 to avoid level shifting)
#define DEFAULT_DHT_PIN 4                    // -1 to disable
#define DEFAULT_DHT_SENSOR_TYPE 22           // UPDATED: Changed to 22

// Photoresistor (Light Sensor)
// VOLTAGE: 3.3V (Analog reference voltage max)
#define DEFAULT_LIGHT_SENSOR_PIN 9           // ORIGINALLY: 17. UPDATED: 9 (ADC pin)

// HC-SR04 Ultrasonic Distance Sensor
// VOLTAGE WARNING: Standard HC-SR04 needs 5V VCC. 
// The ECHO pin outputs 5V logic which damages ESP32. Use a voltage divider (2k/1k) on Echo!
// If using HC-SR04P or RCWL-1601 (3.3V versions), connect directly to 3.3V.
#define DEFAULT_DISTANCE_TRIG_PIN 16         // ORIGINALLY: 7. MOVED: 16
#define DEFAULT_DISTANCE_ECHO_PIN 17         // ORIGINALLY: 8. MOVED: 17

// PIR Motion Sensor
// VOLTAGE: HC-SR501 needs 5V VCC (Logic out is 3.3V). AM312 works on 3.3V.
// Pin 6 is used by TFT_DC. Pin 5 is used by GPS_TXD.
#define DEFAULT_MOTION_SENSOR_PIN 41         // ORIGINALLY: 6. MOVED: 41

// ========== GPS CONFIGURATION (NEW) ==========
// From config_test_gps.h
// VOLTAGE: VCC = 3.3V - 5V (Check module specs), Logic = 3.3V
#define ENABLE_GPS true
#define PIN_GPS_RXD 8                        // Connects to GPS TX. CONFLICT: Was Distance Echo (8)
#define PIN_GPS_TXD 5                        // Connects to GPS RX.
#define PIN_GPS_PPS 38                       // Pulse Per Second
#define GPS_BAUD_RATE 9600

// ========== GPIO TEST CONFIGURATION ==========
#define ENABLE_GPIO_TEST false

// ========== PWM CONFIGURATION ==========
// Pin 15 is used by TFT_BL. Moving to Pin 42.
#define DEFAULT_PWM_PIN 42                   // ORIGINALLY: 15. MOVED: 42

// ========== OPTIONAL FEATURES ==========
// Enable MQTT reporting bridge
#define ENABLE_MQTT_BRIDGE false
#define MQTT_BROKER "mqtt.example.com"
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_TOPIC_PREFIX "esp32-diagnostic"

// Enable Bluetooth/BLE diagnostics
#define ENABLE_BLE_DIAGNOSTICS false

// Enable SPI bus scanning
#define ENABLE_SPI_SCAN true

// Enable memory stress testing
#define ENABLE_MEMORY_STRESS_TEST true

// Enable CPU benchmarking
#define ENABLE_CPU_BENCHMARK true

// ========== INPUT CONFIGURATION (NEW) ==========
// From config_test_gps.h
// VOLTAGE: 3.3V Pull-up (Internal or External)
#define PIN_BUTTON_PAGE     1
#define PIN_BUTTON_AUX      2
#define BUTTON_DEBOUNCE_MS  50

// ========== WEB SERVER CONFIGURATION ==========
#define WEB_SERVER_PORT 80
#define ENABLE_CORS false
#define MAX_WEB_CLIENTS 4

// ========== EXPORT CONFIGURATION ==========
#define ENABLE_AUTO_EXPORT false
#define AUTO_EXPORT_DELAY_SECONDS 30
#define ENABLE_TXT_EXPORT true
#define ENABLE_JSON_EXPORT true
#define ENABLE_CSV_EXPORT true

// ========== PERFORMANCE TUNING ==========
// Task stack sizes (bytes)
#define BUILTIN_LED_TASK_STACK 2048
#define NEOPIXEL_TASK_STACK 2048
#define OLED_TASK_STACK 4096
#define TFT_TASK_STACK 4096           // Added for TFT
#define RGB_LED_TASK_STACK 2048
#define BUZZER_TASK_STACK 2048
#define GPS_TASK_STACK 4096           // Added for GPS

// Task priorities
#define HARDWARE_TEST_TASK_PRIORITY 1
#define WEB_SERVER_TASK_PRIORITY 2
#define GPS_TASK_PRIORITY 1

// ========== WATCHDOG CONFIGURATION ==========
#define TASK_WATCHDOG_TIMEOUT_S 10
#define DISABLE_IDLE_TASK_WDT true

#endif // CONFIG_H