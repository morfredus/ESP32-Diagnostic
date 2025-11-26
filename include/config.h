#ifndef CONFIG_H
#define CONFIG_H

// ========== VERSION & HOSTNAME (Commun) ==========
#define DIAGNOSTIC_HOSTNAME "esp32-diagnostic"
#define DEFAULT_LANGUAGE LANG_EN
#define DIAGNOSTIC_DEBUG 0
#define DIAGNOSTIC_VERBOSE 0
#define DIAGNOSTIC_PREFER_SECURE 0

// =========================================================
//                  MAPPING ESP32-S3 (N16R8 / N8R8)
// =========================================================
#if defined(TARGET_ESP32_S3)

    // --- I2C ---
    #define DEFAULT_I2C_SDA 21
    #define DEFAULT_I2C_SCL 20

    // --- BOUTONS ---
    #define PIN_BUTTON_1     1
    #define PIN_BUTTON_2     2

    // --- LED RGB ---
    #define DEFAULT_RGB_LED_PIN_R 14
    #define DEFAULT_RGB_LED_PIN_G 13
    #define DEFAULT_RGB_LED_PIN_B 18

    // --- CAPTEURS ---
    #define DEFAULT_DHT_PIN 19
    #define DEFAULT_LIGHT_SENSOR_PIN 4
    #define DEFAULT_DISTANCE_TRIG_PIN 16
    #define DEFAULT_DISTANCE_ECHO_PIN 17
    #define DEFAULT_MOTION_SENSOR_PIN 39
    #define DEFAULT_BUZZER_PIN 3
    #define DEFAULT_PWM_PIN 27

    // --- TFT ST7789 (S3 Specific SPI) ---
    #define TFT_MOSI            11
    #define TFT_SCLK            12
    #define TFT_CS              10
    #define TFT_DC              9
    #define TFT_RST             7
    #define TFT_BL              15

    // --- GPS ---
    #define PIN_GPS_RXD         8
    #define PIN_GPS_TXD         5
    #define PIN_GPS_PPS         38

// =========================================================
//                  MAPPING ESP32 CLASSIQUE (DevKitC)
// =========================================================
#elif defined(TARGET_ESP32_CLASSIC)

    // --- I2C (Standard ESP32) ---
    #define DEFAULT_I2C_SDA 21
    #define DEFAULT_I2C_SCL 22  // SCL est généralement 22 sur Classic

    // --- BOUTONS ---
    // Attention : GPIO 1/2 sont TX/RX sur Classic. On change.
    #define PIN_BUTTON_1     0   // Bouton BOOT
    #define PIN_BUTTON_2     35  // Input only

    // --- LED RGB ---
    #define DEFAULT_RGB_LED_PIN_R 25
    #define DEFAULT_RGB_LED_PIN_G 26
    #define DEFAULT_RGB_LED_PIN_B 27

    // --- CAPTEURS ---
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
    #define TFT_BL              32 // Ou autre GPIO libre

    // --- GPS ---
    #define PIN_GPS_RXD         16 // RX2
    #define PIN_GPS_TXD         17 // TX2
    #define PIN_GPS_PPS         39 // VN - Input Only

#else
    #error "Aucune cible definie ! Verifiez platformio.ini (TARGET_ESP32_...)"
#endif

// =========================================================
//            CONFIGURATION COMMUNE (Suite)
// =========================================================

// --- I2C Common ---
#define ENABLE_I2C_SCAN true

// --- OLED Common ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define DEFAULT_OLED_ROTATION 0

// --- LED Common ---
#define DEFAULT_BUILTIN_LED_PIN -1
#define DEFAULT_NEOPIXEL_PIN -1
#define DEFAULT_NEOPIXEL_COUNT 8
#define DEFAULT_NEOPIXEL_BRIGHTNESS 50
#define CUSTOM_LED_PIN -1

// --- Sensors Common ---
#define DEFAULT_DHT_SENSOR_TYPE 22

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

// --- Features Common ---
#define ENABLE_GPIO_TEST false
#define ENABLE_MQTT_BRIDGE false
#define MQTT_BROKER "mqtt.example.com"
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_TOPIC_PREFIX "esp32-diagnostic"

#define ENABLE_BLE_DIAGNOSTICS false
#define ENABLE_SPI_SCAN true
#define ENABLE_MEMORY_STRESS_TEST true
#define ENABLE_CPU_BENCHMARK true

#define WEB_SERVER_PORT 80
#define ENABLE_CORS false
#define MAX_WEB_CLIENTS 4

#define ENABLE_AUTO_EXPORT false
#define AUTO_EXPORT_DELAY_SECONDS 30
#define ENABLE_TXT_EXPORT true
#define ENABLE_JSON_EXPORT true
#define ENABLE_CSV_EXPORT true

// --- Performance Common ---
#define BUILTIN_LED_TASK_STACK 2048
#define NEOPIXEL_TASK_STACK 2048
#define OLED_TASK_STACK 4096
#define RGB_LED_TASK_STACK 2048
#define BUZZER_TASK_STACK 2048

#define HARDWARE_TEST_TASK_PRIORITY 1
#define WEB_SERVER_TASK_PRIORITY 2

#define TASK_WATCHDOG_TIMEOUT_S 10
#define DISABLE_IDLE_TASK_WDT true

#endif // CONFIG_H