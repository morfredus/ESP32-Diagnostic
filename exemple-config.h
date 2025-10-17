/*
 * ============================================================
 * ESP32 Diagnostic System - Main Configuration Template
 * Version: 3.2.0
 * ============================================================
 * 
 * INSTRUCTIONS:
 * 1. Copier ce fichier vers "config.h"
 * 2. Modifier les paramètres ci-dessous
 * 3. NE PAS commiter config.h dans Git (ajouté au .gitignore)
 * 
 * NOTE: La configuration WiFi est maintenant dans wifi-config.h
 * 
 * ============================================================
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
// CONFIGURATION PINS HARDWARE
// ============================================================

// --- LED INTERNE ---
#define LED_BUILTIN_PIN 2  // GPIO 2 (la plupart des ESP32)

// --- NEOPIXEL (Optionnel) ---
// Décommenter pour activer le support NeoPixel
// #define USE_NEOPIXEL

#ifdef USE_NEOPIXEL
  #define NEOPIXEL_PIN 48          // GPIO 48 pour ESP32-S3
  #define NEOPIXEL_COUNT 1         // Nombre de LEDs
  #define NEOPIXEL_BRIGHTNESS 50   // Luminosité (0-255)
#endif

// --- ÉCRAN OLED (Optionnel) ---
// Décommenter pour activer le support OLED
// #define USE_OLED

#ifdef USE_OLED
  #define OLED_SDA_PIN 21          // Pin SDA I2C
  #define OLED_SCL_PIN 22          // Pin SCL I2C
  #define OLED_ADDR 0x3C           // Adresse I2C (0x3C ou 0x3D)
  #define OLED_WIDTH 128           // Largeur en pixels
  #define OLED_HEIGHT 64           // Hauteur en pixels
#endif

// --- CAPTEUR TEMPÉRATURE (Optionnel) ---
// Décommenter pour activer un capteur de température
// #define USE_TEMP_SENSOR

#ifdef USE_TEMP_SENSOR
  // DHT22
  // #define TEMP_SENSOR_DHT22
  // #define DHT_PIN 4
  
  // DS18B20
  // #define TEMP_SENSOR_DS18B20
  // #define DS18B20_PIN 4
  
  // BME280 (I2C)
  // #define TEMP_SENSOR_BME280
  // #define BME280_ADDR 0x76
#endif

// --- BUZZER (Optionnel) ---
// #define USE_BUZZER
#ifdef USE_BUZZER
  #define BUZZER_PIN 25            // Pin du buzzer
#endif

// --- BOUTONS (Optionnel) ---
// #define USE_BUTTONS
#ifdef USE_BUTTONS
  #define BUTTON1_PIN 0            // Bouton 1 (BOOT sur la plupart)
  #define BUTTON2_PIN 35           // Bouton 2
#endif

// ============================================================
// CONFIGURATION TESTS HARDWARE
// ============================================================

// --- PINS GPIO À TESTER ---
// Modifier selon votre câblage
const int TEST_GPIO_PINS[] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
  11, 12, 13, 14, 15, 16, 17, 18
};
const int TEST_GPIO_COUNT = sizeof(TEST_GPIO_PINS) / sizeof(int);

// --- PINS PWM À TESTER ---
const int TEST_PWM_PINS[] = {25, 26, 27};
const int TEST_PWM_COUNT = sizeof(TEST_PWM_PINS) / sizeof(int);

// --- PINS ADC À TESTER ---
const int TEST_ADC_PINS[] = {32, 33, 34, 35, 36, 39};
const int TEST_ADC_COUNT = sizeof(TEST_ADC_PINS) / sizeof(int);

// ============================================================
// CONFIGURATION SERVEUR WEB
// ============================================================

#define WEB_SERVER_PORT 80         // Port du serveur web
#define DEFAULT_LANGUAGE "fr"       // Langue par défaut ("fr" ou "en")
#define AUTO_REFRESH_INTERVAL 5000  // Auto-refresh interface (ms)

// ============================================================
// CONFIGURATION SYSTÈME
// ============================================================

#define DEBUG_MODE true             // Activer les logs debug
#define SERIAL_BAUD_RATE 115200     // Vitesse du port série

// ============================================================
// PARAMÈTRES AVANCÉS
// ============================================================

#define JSON_BUFFER_SIZE 4096       // Taille buffer JSON
#define ENABLE_CORS true            // Activer CORS pour dev

// ============================================================
// CONFIGURATION PAR MODÈLE ESP32
// ============================================================

/*
 * ESP32 Classique:
 * - LED_BUILTIN_PIN: 2
 * - NEOPIXEL_PIN: 16 (recommandé)
 * - OLED: SDA=21, SCL=22
 * - Pas de PSRAM par défaut
 * 
 * ESP32-S2:
 * - LED_BUILTIN_PIN: 15
 * - NEOPIXEL_PIN: 18 (recommandé)
 * - OLED: SDA=8, SCL=9
 * - Pas de Bluetooth
 * 
 * ESP32-S3:
 * - LED_BUILTIN_PIN: 48 (varie selon carte)
 * - NEOPIXEL_PIN: 48 (recommandé)
 * - OLED: SDA=21, SCL=22
 * - PSRAM disponible (8MB sur certaines cartes)
 * 
 * ESP32-C3:
 * - LED_BUILTIN_PIN: 8
 * - NEOPIXEL_PIN: 8 (recommandé)
 * - OLED: SDA=5, SCL=6
 * - Single core
 * 
 * ⚠️ PINS RÉSERVÉES À ÉVITER:
 * - GPIO 6-11: Flash interne (NE JAMAIS UTILISER)
 * - GPIO 0: Boot (éviter si possible)
 * - GPIO 12: Strapping pin (attention au bootloop)
 */

#endif // CONFIG_H