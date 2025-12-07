#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

// =========================================================
//        Configuration Pinout ESP32-S3 DevKitC-1 N16R8
// =========================================================
#if defined(TARGET_ESP32_S3)

// BROCHES RÉSERVÉES
// ?? GPIO48 : LIBRE pour la Neopixel intégrée.

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
#define DEFAULT_I2C_SDA      15    // I2C Data (SDA).
#define DEFAULT_I2C_SCL      16    // I2C Clock (SCL).

// ------------------------------------
// LED RGB (Exclut 48) & Boutons
#define DEFAULT_RGB_LED_PIN_R 21   // Côté Droit
#define DEFAULT_RGB_LED_PIN_G 45   // Côté Droit
#define DEFAULT_RGB_LED_PIN_B 47   // Côté Droit

#define PIN_BUTTON_BOOT      0     // Bouton BOOT intégré
#define PIN_BUTTON_1         38    // Côté Droit
#define PIN_BUTTON_2         39    // Côté Droit

// ------------------------------------
// Capteurs & Sorties
// ------------------------------------
#define DEFAULT_PWM_PIN      14    // Sortie PWM/Buzzer (Côté Gauche).
#define DEFAULT_BUZZER_PIN   14    // Même broche que PWM.

#define DEFAULT_DHT_PIN      5     // Capteur DHT (Temp/Hum).
#define DEFAULT_MOTION_SENSOR_PIN 46  // Capteur de mouvement (PIR)
#define DEFAULT_LIGHT_SENSOR_PIN 4  // Capteur de lumière (ADC).

// ------------------------------------
// Capteurs de Distance
// ------------------------------------
#define DEFAULT_DISTANCE_TRIG_PIN 3  // Sortie (Trigger).
#define DEFAULT_DISTANCE_ECHO_PIN 6 // Entrée (Écho).

// =========================================================
//                  MAPPING ESP32 CLASSIQUE (DevKitC)
// =========================================================
#elif defined(TARGET_ESP32_CLASSIC)

// ------------------------------------
// GPS (UART2)
// ------------------------------------
#define PIN_GPS_RXD          16    // UART2 RX (vers TX du GPS).
#define PIN_GPS_TXD          17    // UART2 TX (vers RX du GPS).
#define PIN_GPS_PPS          4     // Signal Pulse Per Second.

// ------------------------------------
// TFT ST7789 (Interface SPI - Broches groupées)
// ------------------------------------
#define TFT_MOSI             23    // SPI Master Out Slave In (MOSI).
#define TFT_SCLK             18    // SPI Clock (SCLK).
#define TFT_CS               19    // Chip Select.
#define TFT_DC               27    // Data/Command.
#define TFT_RST              26    // Reset de l'écran.
#define TFT_BL               13    // Backlight.

// ------------------------------------
// I2C (Contrainte Appliquée)
// ------------------------------------
#define DEFAULT_I2C_SDA      21    // I2C Data (SDA).
#define DEFAULT_I2C_SCL      22    // I2C Clock (SCL).

// ------------------------------------
// LED RGB & Boutons
// ------------------------------------
#define DEFAULT_RGB_LED_PIN_R 12   // RGB Rouge.
#define DEFAULT_RGB_LED_PIN_G 14   // RGB Vert.
#define DEFAULT_RGB_LED_PIN_B 15   // RGB Bleu.

#define PIN_BUTTON_BOOT      0     // Bouton BOOT intégré
#define PIN_BUTTON_1         34    // Bouton 1 (Entrée Seulement).
#define PIN_BUTTON_2         35    // Bouton 2 (Entrée Seulement).

// ------------------------------------
// Capteurs & Sorties
// ------------------------------------
#define DEFAULT_PWM_PIN      5     // Sortie PWM dédiée.
#define DEFAULT_BUZZER_PIN   5     // Même broche que PWM.

#define DEFAULT_DISTANCE_TRIG_PIN 32 // Sortie (Trigger).
#define DEFAULT_DISTANCE_ECHO_PIN 33 // Entrée (Écho).

#define DEFAULT_DHT_PIN      25    // Capteur DHT (Plus sûr que GPIO2).
#define DEFAULT_MOTION_SENSOR_PIN 36 // Mouvement (Entrée Seulement - VP).
#define DEFAULT_LIGHT_SENSOR_PIN 2  // Capteur de lumière (ADC - Broche Strapping).

#else
    #error "Aucune cible definie ! Verifiez platformio.ini (TARGET_ESP32_...)"
#endif

#endif // BOARD_CONFIG_H
