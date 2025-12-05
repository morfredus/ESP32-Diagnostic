#ifndef CONFIG_H
#define CONFIG_H

// ========== VERSION & HOSTNAME (Commun) ==========
#define DIAGNOSTIC_HOSTNAME "esp32-diagnostic"
#define DEFAULT_LANGUAGE LANG_EN
#define DIAGNOSTIC_DEBUG 0
#define DIAGNOSTIC_VERBOSE 0
#define DIAGNOSTIC_PREFER_SECURE 0

// =========================================================
//      Configuration Pinout ESP32-S3 DevKitC-1 N16R8
// =========================================================
#if defined(TARGET_ESP32_S3)

// BROCHES RÉSERVÉES POUR FUTUR UART2
// Ces broches sont laissées libres pour toute communication série additionnelle.
//#define UART2_TX_PIN         15    // GPIO15: LIBRE - Futur UART2 Transmission.
//#define UART2_RX_PIN         16    // GPIO16: LIBRE - Futur UART2 Réception.

// ------------------------------------
// GPS (UART1)
// ------------------------------------
#define PIN_GPS_RXD          18    // Côté Gauche
#define PIN_GPS_TXD          17    // Côté Gauche
#define PIN_GPS_PPS          8     // Côté Gauche

// ------------------------------------
// TFT ST7789 (SPI - TOUT Côté Gauche)
// ------------------------------------
#define TFT_MOSI             11    // SPI Master Out Slave In (MOSI).
#define TFT_SCLK             12    // SPI Clock (SCLK).
#define TFT_CS               10    // Chip Select.
#define TFT_DC               9     // Data/Command.
#define TFT_RST              13    // Reset de l'écran.
#define TFT_BL               7     // Backlight (Rétroéclairage).

// ------------------------------------
// I2C (Côté Droit)
// ------------------------------------
#define DEFAULT_I2C_SDA      21    // I2C Data (SDA).
#define DEFAULT_I2C_SCL      20    // I2C Clock (SCL).

// ------------------------------------
// LED RGB & Boutons (Côté Droit)
// ------------------------------------
#define DEFAULT_RGB_LED_PIN_R 47   // Côté Droit
#define DEFAULT_RGB_LED_PIN_G 48   // Côté Droit
#define DEFAULT_RGB_LED_PIN_B 45   // Côté Droit

#define PIN_BUTTON_1         1     // Côté Droit
#define PIN_BUTTON_2         2     // Côté Droit

// ------------------------------------
// Capteurs & Sorties
// ------------------------------------
#define DEFAULT_PWM_PIN      14    // Sortie PWM/Buzzer (broche libre idéale Côté Gauche).
#define DEFAULT_BUZZER_PIN   14    // Peut être la même broche que le PWM.

#define DEFAULT_DHT_PIN      5     // Capteur DHT (Temp/Hum).
#define DEFAULT_MOTION_SENSOR_PIN 4  // Capteur de mouvement (PIR)
#define DEFAULT_LIGHT_SENSOR_PIN 19  // Capteur de lumière (ADC - Côté Droit).

// ------------------------------------
// Capteurs de Distance (utilisation de broches voisines)
// ------------------------------------
#define DEFAULT_DISTANCE_TRIG_PIN 3  // Sortie (Trigger).
#define DEFAULT_DISTANCE_ECHO_PIN 6  // Entrée (Écho).

// =========================================================
//                  MAPPING ESP32 CLASSIQUE (DevKitC)
// =========================================================
#elif defined(TARGET_ESP32_CLASSIC)

// ------------------------------------
// GPS (UART2)
// ------------------------------------
#define PIN_GPS_RXD          16    // UART2 RX (vers TX du GPS).
#define PIN_GPS_TXD          17    // UART2 TX (vers RX du GPS).
#define PIN_GPS_PPS          4    // Signal Pulse Per Second.

// ------------------------------------
// TFT ST7789 (Interface SPI - Broches groupées)
// ------------------------------------
#define TFT_MOSI             23    // SPI Master Out Slave In (MOSI).
#define TFT_SCLK             18    // SPI Clock (SCLK).
#define TFT_CS               19    // Chip Select.
#define TFT_DC               27    // Data/Command.
#define TFT_RST              26    // Reset de l'écran.
#define TFT_BL               13    // Backlight (Utilisation de GPIO13, broche sûre).

// ------------------------------------
// I2C (Contrainte Appliquée)
// ------------------------------------
#define DEFAULT_I2C_SDA      21    // I2C Data (SDA).
#define DEFAULT_I2C_SCL      22    // I2C Clock (SCL).

// ------------------------------------
// LED RGB & Boutons
// ------------------------------------
#define DEFAULT_RGB_LED_PIN_R 12   // RGB Rouge (Broche Strapping, mais souvent utilisable).
#define DEFAULT_RGB_LED_PIN_G 14   // RGB Vert.
#define DEFAULT_RGB_LED_PIN_B 15   // RGB Bleu.

#define PIN_BUTTON_1         34    // Bouton 1 (Entrée Seulement).
#define PIN_BUTTON_2         35    // Bouton 2 (Entrée Seulement).

// ------------------------------------
// Capteurs & Sorties
// ------------------------------------
#define DEFAULT_PWM_PIN      5     // Sortie PWM dédiée (Broche Strapping, mais fonctionnelle).
#define DEFAULT_BUZZER_PIN   5     // Même broche que PWM.

#define DEFAULT_DISTANCE_TRIG_PIN 32 // Sortie (Trigger).
#define DEFAULT_DISTANCE_ECHO_PIN 33 // Entrée (Écho).

#define DEFAULT_DHT_PIN      25     // Capteur DHT (Broche Strapping, mais OK en entrée/sortie).
#define DEFAULT_MOTION_SENSOR_PIN 36 // Mouvement (Entrée Seulement - VP).
#define DEFAULT_LIGHT_SENSOR_PIN 2  // Capteur de lumière (ADC - Broche sûre).

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

// --- Buttons Common ---
#define ENABLE_BUTTONS true

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