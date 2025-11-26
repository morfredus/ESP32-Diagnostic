/**
 * ESP32 Diagnostic Suite - Default Configuration
 *
 * Default configuration values for ESP32-S3 DevKitC-1 N16R8.
 * Mapping consolidé et commenté.
 */

#ifndef CONFIG_H
#define CONFIG_H

// ========== VERSION & HOSTNAME ==========
#define DIAGNOSTIC_HOSTNAME "esp32-diagnostic"

// ========== DEFAULT LANGUAGE ==========
#define DEFAULT_LANGUAGE LANG_EN   // Interface par défaut en anglais

// ========== DEBUG OPTIONS ==========
#define DIAGNOSTIC_DEBUG 0         // 0 = désactivé, 1 = activé
#define DIAGNOSTIC_VERBOSE 0       // 0 = normal, 1 = logs détaillés

// ========== NETWORK CONFIGURATION ==========
#define DIAGNOSTIC_PREFER_SECURE 0 // Préférer HTTPS si possible

// ========== I2C CONFIGURATION ==========
#define DEFAULT_I2C_SDA 21         // I2C SDA (OLED + capteurs)
#define DEFAULT_I2C_SCL 20         // I2C SCL (OLED + capteurs)
#define ENABLE_I2C_SCAN true       // Scan du bus I2C au démarrage

#define ENABLE_SECONDARY_I2C false
#define SECONDARY_I2C_SDA 25       // Bus I2C secondaire SDA (si activé)
#define SECONDARY_I2C_SCL 26       // Bus I2C secondaire SCL (si activé)

// ========== BOUTONS ==========
#define PIN_BUTTON_1     1         // Bouton 1 (entrée utilisateur)
#define PIN_BUTTON_2     2         // Bouton 2 (entrée utilisateur)

// ========== OLED DISPLAY CONFIGURATION ==========
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define DEFAULT_OLED_ROTATION 0      // 0, 1, 2, or 3 (90° increments)


// ========== LED CONFIGURATION ==========
#define DEFAULT_BUILTIN_LED_PIN -1 // LED interne désactivée
#define DEFAULT_NEOPIXEL_PIN -1    // Pas de bandeau NeoPixel
#define DEFAULT_NEOPIXEL_COUNT 8
#define DEFAULT_NEOPIXEL_BRIGHTNESS 50
#define CUSTOM_LED_PIN -1

// LED RGB (R, G, B séparés)
#define DEFAULT_RGB_LED_PIN_R 14   // LED RGB Rouge (R)
#define DEFAULT_RGB_LED_PIN_G 13   // LED RGB Vert (G)
#define DEFAULT_RGB_LED_PIN_B 18   // LED RGB Bleu (B)

// ========== BUZZER ==========
#define DEFAULT_BUZZER_PIN 3       // Buzzer piezo

// ========== CAPTEURS ==========
#define DEFAULT_DHT_PIN 19         // DHT11/DHT22 (Température/Humidité)
#define DEFAULT_DHT_SENSOR_TYPE 22 // 11=DHT11, 22=DHT22
#define DEFAULT_LIGHT_SENSOR_PIN 4 // Capteur de lumière (photoresistor)

// HC-SR04 Ultrason
#define DEFAULT_DISTANCE_TRIG_PIN 41  // TRIG (GPIO output-capable)
#define DEFAULT_DISTANCE_ECHO_PIN 36  // ECHO (GPIO input-only OK)

// PIR Motion Sensor
#define DEFAULT_MOTION_SENSOR_PIN 39  // PIR (GPIO input-capable)

// ========== TFT DISPLAY ST7789 ==========
#define ENABLE_TFT_DISPLAY  true  // Active l'écran TFT ST7789
#define TFT_WIDTH           240
#define TFT_HEIGHT          240
#define TFT_MOSI            11    // SPI MOSI
#define TFT_SCLK            12    // SPI SCLK
#define TFT_CS              10    // Chip Select
#define TFT_DC              9     // Data/Command
#define TFT_RST             46    // Reset
#define TFT_BL              15    // Backlight
#define TFT_ROTATION        0     // Orientation par défaut (0, 1, 2, or 3)
#define TFT_BACKLIGHT_PWM   255   // Luminosité max

// ========== GPS (UART2) ==========
#define PIN_GPS_RXD         8     // GPS TX → ESP32 RXD
#define PIN_GPS_TXD         5     // GPS RX → ESP32 TXD
#define PIN_GPS_PPS         38    // Pulse Per Second
#define GPS_UPDATE_RATE     1000  // 1 Hz
#define GPS_TIMEOUT         5000
#define GPS_FIX_TIMEOUT     60000
#define HDOP_GOOD_THRESHOLD 2.0

// ========== GPIO TEST ==========
#define ENABLE_GPIO_TEST false

// ========== PWM ==========
#define DEFAULT_PWM_PIN 27        // GPIO pour tests PWM (libre et sans conflit)

// ========== OPTIONAL FEATURES ==========
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

// ========== WEB SERVER ==========
#define WEB_SERVER_PORT 80
#define ENABLE_CORS false
#define MAX_WEB_CLIENTS 4

// ========== EXPORT ==========
#define ENABLE_AUTO_EXPORT false
#define AUTO_EXPORT_DELAY_SECONDS 30
#define ENABLE_TXT_EXPORT true
#define ENABLE_JSON_EXPORT true
#define ENABLE_CSV_EXPORT true

// ========== PERFORMANCE ==========
#define BUILTIN_LED_TASK_STACK 2048
#define NEOPIXEL_TASK_STACK 2048
#define OLED_TASK_STACK 4096
#define RGB_LED_TASK_STACK 2048
#define BUZZER_TASK_STACK 2048

#define HARDWARE_TEST_TASK_PRIORITY 1
#define WEB_SERVER_TASK_PRIORITY 2

// ========== WATCHDOG ==========
#define TASK_WATCHDOG_TIMEOUT_S 10
#define DISABLE_IDLE_TASK_WDT true

#endif // CONFIG_H
