#ifndef CONFIG_H
#define CONFIG_H

// ========== VERSION & HOSTNAME (Commun) ==========
#define DIAGNOSTIC_HOSTNAME "esp32-diagnostic"
#define DEFAULT_LANGUAGE LANG_EN
#define DIAGNOSTIC_DEBUG 0
#define DIAGNOSTIC_VERBOSE 0
#define DIAGNOSTIC_PREFER_SECURE 0

// --- Pin mapping spécifique à chaque carte ---
// Toutes les pins doivent être référencées UNIQUEMENT via board_config.h dans tout le projet !
#include "board_config.h"

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


// [PIN POLICY] Toutes les pins doivent être référencées exclusivement via board_config.h
// Les constantes de configuration LED suivantes sont conservées uniquement si elles ne sont PAS des numéros de GPIO :
// - DEFAULT_NEOPIXEL_COUNT, DEFAULT_NEOPIXEL_BRIGHTNESS
// Les pins doivent être utilisées via les macros de board_config.h (ex : NEOPIXEL, LED_BUILTIN, etc.)
#define DEFAULT_NEOPIXEL_COUNT 1
#define DEFAULT_NEOPIXEL_BRIGHTNESS 50

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