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

// ============================================================
// RAPPELS DE SÉCURITÉ ESP32-WROOM
// ============================================================
// - Tension logique: 3.3 V uniquement sur les GPIO (n’alimentez aucun GPIO en 5 V).
// - Broches de strapping (boot): GPIO0, GPIO2, GPIO4, GPIO5, GPIO12, GPIO15.
//   Éviter tout état forcé au boot (LEDs, pulls externes) sur ces pins.
// - GPIO34, GPIO35, GPIO36, GPIO39: ENTRÉES SEULEMENT (pas de sortie, pas de pull-up interne).
// - UART0 (flash/log via USB-UART): U0TXD=GPIO1 (TX0) et U0RXD=GPIO3 (RX0).
//   Ne pas y connecter de périphériques qui forcent un niveau.
// - Courant par GPIO: viser ≤12 mA; pour charges >12 mA, utiliser un transistor/driver.
// - Bus I2C: si modules sans pull-ups, ajouter 4.7 kΩ vers 3.3 V (0.25 W) sur SDA/SCL.
// ============================================================


// ------------------------------------
// GPS (UART2)
// ------------------------------------
#define PIN_GPS_RXD          16  // GPS RX (vers TX du GPS), 3.3 V, pas de résistance
#define PIN_GPS_TXD          17  // GPS TX (vers RX du GPS), 3.3 V, pas de résistance
#define PIN_GPS_PPS          36  // GPS PPS (entrée), 3.3 V, entrée-seulement, pas de pull interne, pas de résistance

// ------------------------------------
// TFT ST7789 (SPI groupé côté gauche/droit pour câblage court)
// ------------------------------------
#define TFT_SCLK             18  // SPI Clock (HSPI CLK), 3.3 V, pas de résistance
#define TFT_MOSI             23  // SPI MOSI (HSPI MOSI), 3.3 V, pas de résistance
#define TFT_CS               27  // Chip Select, 3.3 V, pas de résistance (éviter pins de strapping)
#define TFT_DC               14  // Data/Command, 3.3 V, pas de résistance
#define TFT_RST              25  // Reset écran, 3.3 V, pas de résistance
#define TFT_BL               32  // Backlight logique 3.3 V; si LED directe: driver/transistor + résistance série calculée

// ------------------------------------
// I2C (voisins côté gauche)
// ------------------------------------
#define DEFAULT_I2C_SDA      21  // SDA, 3.3 V, pull-up 4.7 kΩ (0.25 W) si absent
#define DEFAULT_I2C_SCL      22  // SCL, 3.3 V, pull-up 4.7 kΩ (0.25 W) si absent

// ------------------------------------
// LED RGB discrète (pins non-strapping et voisins)
// ------------------------------------
#define DEFAULT_RGB_LED_PIN_R 13 // Rouge, 3.3 V, résistance série 220–1kΩ (0.25 W)
#define DEFAULT_RGB_LED_PIN_G 14 // Vert, 3.3 V, résistance série 220–1kΩ (0.25 W)
#define DEFAULT_RGB_LED_PIN_B 25 // Bleu, 3.3 V, résistance série 220–1kΩ (0.25 W)

// ------------------------------------
// Boutons utilisateur (pull-up interne, voisins côté droit)
// ------------------------------------
#define PIN_BUTTON_1         32  // Bouton vers GND, INPUT_PULLUP, 3.3 V, pas de résistance externe
#define PIN_BUTTON_2         33  // Bouton vers GND, INPUT_PULLUP, 3.3 V, pas de résistance externe
#define PIN_BUTTON_BOOT      0   // Bouton BOOT (strapping), réservé au boot, 3.3 V, ne pas utiliser comme bouton utilisateur

// ------------------------------------
// Capteurs & Sorties
// ------------------------------------
#define DEFAULT_PWM_PIN           5  // PWM/Buzzer, 3.3 V; si passif ou >12 mA → transistor + résistance base 1–10 kΩ (0.25 W)
#define DEFAULT_BUZZER_PIN        5  // Même broche que PWM (voir limite de courant GPIO)
#define DEFAULT_DISTANCE_TRIG_PIN 27 // HC-SR04 TRIG, sortie 3.3 V, pas de résistance
#define DEFAULT_DISTANCE_ECHO_PIN 33 // HC-SR04 ECHO, entrée 3.3 V; si capteur en 5 V → diviseur 10 kΩ/20 kΩ (0.25 W)
#define DEFAULT_DHT_PIN           32 // DHT, 3.3 V, pull-up 10 kΩ (0.25 W) si non intégré (partageable avec bouton si géré en temps)
#define DEFAULT_LIGHT_SENSOR_PIN  39 // LDR/photodiode sur ADC, 3.3 V max, pont diviseur avec ~10 kΩ (0.25 W), entrée-seulement


#else
    #error "Aucune cible definie ! Verifiez platformio.ini (TARGET_ESP32_...)"
#endif

#endif // BOARD_CONFIG_H
