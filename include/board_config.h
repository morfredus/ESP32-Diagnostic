#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

// =========================================================
//         Configuration Pinout ESP32-S3 DevKitC-1 N16R8
// =========================================================
// Cette section définit les broches (Pins) de l'ESP32-S3 (une version plus récente et puissante).
#if defined(TARGET_ESP32_S3)

// ============================================================
// RAPPELS DE SÉCURITÉ ESP32-S3 (Basé sur le tableau de câblage)
// ============================================================
// !!! TRÈS IMPORTANT : Lisez ceci avant de connecter quoi que ce soit !!!
//
// - Tension logique: 3.3 V uniquement sur GPIO. 
//   -> L'ESP32-S3 fonctionne en 3.3 Volts. **Ne jamais appliquer 5V directement** sur les broches GPIO,
//      sauf si un circuit de protection/conversion (comme un diviseur de tension) est utilisé.
// - GPIO0 : broche BOOT (strapping) – réservée au boot, ne pas connecter d'élément maintenu à LOW.
//   -> Cette broche est utilisée par l'ESP32 pour démarrer. Ne l'utilisez pas pour autre chose.
// - GPIO46 : utilisé pour le capteur PIR. À vérifier pour l'impact potentiel sur le JTAG (moins critique que GPIO0).
//   -> C'est une broche spéciale qui peut être liée au débogage (JTAG). Attention lors de l'utilisation.
// - GPS TX (vers RXD 18) et HC-SR04 ECHO (vers GPIO 35) : nécessitent un DIVISEUR DE TENSION si le signal est 5V.
//   -> Si ces capteurs sont alimentés en 5V, leurs signaux de sortie doivent être abaissés à 3.3V pour l'ESP32. 
// - LEDs : ajouter résistance série 220–470 O (0.25 W).
//   -> **Obligatoire** pour limiter le courant et éviter de griller la LED et/ou la broche de l'ESP32.
// - I2C (GPIO 15/16) : pull-up 4.7 kO vers 3.3 V OBLIGATOIRE.
//   -> Pour le protocole I2C, des résistances de "pull-up" (tirage vers le haut) sont nécessaires 
//      sur les lignes SDA et SCL pour que la communication fonctionne correctement.
// - Buzzer : transistor nécessaire (avec résistance de base 1–10 kO) si passif ou courant >12 mA.
//   -> Un buzzer (surtout s'il est passif ou consomme beaucoup) demande plus de courant que l'ESP32 ne peut fournir directement. 
//      Un transistor agit comme un interrupteur puissant commandé par l'ESP32.
// ============================================================

// ------------------------------------
// DÉTAIL TECHNIQUE : Le Diviseur de Tension
// ------------------------------------
// OBJECTIF : Protéger les entrées de l'ESP32 (3.3V max) lorsqu'elles reçoivent un signal d'un composant 5V.
// FONCTIONNEMENT : Le diviseur utilise deux résistances (R1 et R2) pour "diviser" la tension d'entrée (V_in).
// Le signal de sortie (V_out) que reçoit l'ESP32 est calculé par la formule :
// $$V_{out} = V_{in} \cdot \frac{R_2}{R_1 + R_2}$$
// Pour abaisser 5V à 3.3V (ou moins) pour l'ESP32, on choisit généralement R1=10 kOhm et R2=20 kOhm, 
// ou plus simplement R1=2kOhm et R2=3kOhm.
// S'applique ici aux broches d'entrée (RXD) du GPS et au signal ECHO du HC-SR04 si ils sont en 5V.
// 
// ------------------------------------


// ------------------------------------
// GPS (UART1)
// ------------------------------------
// Configuration de la communication série (UART1) avec le module GPS.
#define PIN_GPS_RXD           18  // GPS TX (vers RXD de l'ESP32). **Si 5V, Diviseur de tension OBLIGATOIRE.**
#define PIN_GPS_TXD           17  // GPS RX (vers TXD de l'ESP32). Câblage direct.
#define PIN_GPS_PPS           8   // GPS PPS, Câblage direct.

// ------------------------------------
// TFT ST7789 (SPI)
// ------------------------------------
// Configuration de l'écran couleur (TFT) utilisant le protocole SPI.
#define TFT_MOSI              11  // SPI MOSI (Master Out Slave In) : Broche de données vers l'écran. Câblage direct.
#define TFT_SCLK              12  // SPI Clock (Horloge) : Broche de synchronisation. Câblage direct.
#define TFT_CS                10  // Chip Select : Broche pour activer spécifiquement cet écran. Câblage direct.
#define TFT_DC                9   // Data/Command : Broche pour indiquer si les données sont des pixels ou une commande. Câblage direct.
#define TFT_RST               13  // Reset écran. Pull-up/down selon librairie.
#define TFT_BL                7   // Backlight (Rétroéclairage) : Souvent utilisé avec le PWM pour ajuster la luminosité.

// ------------------------------------
// I2C
// ------------------------------------
// Configuration du bus de communication I2C.
#define I2C_SDA       15  // SDA (Serial Data Line). **Pull-Up 4.7kO vers 3.3V OBLIGATOIRE.**
#define I2C_SCL       16  // SCL (Serial Clock Line). **Pull-Up 4.7kO vers 3.3V OBLIGATOIRE.**

// ------------------------------------
// DÉTAIL TECHNIQUE : Le Bus I2C (Inter-Integrated Circuit)
// ------------------------------------
// OBJECTIF : Permettre à plusieurs capteurs/périphériques de communiquer avec l'ESP32 en utilisant seulement deux broches (SDA et SCL).
// L'IMPÉRATIF PULL-UP : Le bus I2C utilise des sorties à drain ouvert (open-drain). Des résistances externes (Pull-Up, typiquement 4.7 kOhm vers 3.3V) sont nécessaires pour tirer le signal vers le haut.
// 
// ------------------------------------


// ------------------------------------
// LED RGB
// ------------------------------------
#define RGB_LED_PIN_R 21 // Rouge. Résistance série **220O - 470O**.
#define RGB_LED_PIN_G 41 // Vert. Résistance série **220O - 470O**.
#define RGB_LED_PIN_B 42 // Bleu. Résistance série **220O - 470O**.
#define NEOPIXEL_PIN          48 // NeoPixel Intégrée. Data (Réservée).

// ------------------------------------
// Boutons
// ------------------------------------
#define PIN_BUTTON_BOOT       0   // Bouton BOOT (strap). Réservé au boot.
#define PIN_BUTTON_1          38  // Bouton vers GND. Utiliser **Pull-Up interne**.
#define PIN_BUTTON_2          39  // Bouton vers GND. Utiliser **Pull-Up interne**.

// ------------------------------------
// Capteurs & Sorties
// ------------------------------------
#define PWM_PIN       20  // PWM générique. Résistance série **220O - 470O** si LED.
#define BUZZER_PIN    6   // Buzzer. Transistor + **Résistance base 1–10kO**.
#define DHT_PIN       5   // DHT. **Pull-Up 10kO vers 3.3V**.
#define MOTION_SENSOR_PIN 46 // PIR. Câblage direct. **(Potentiel JTAG - attention au boot).**
#define LIGHT_SENSOR_PIN 4  // LDR. **Diviseur ~10kO**.

// ------------------------------------
// Capteurs de Distance
// ------------------------------------
#define DISTANCE_TRIG_PIN 2  // HC-SR04 TRIG. Câblage direct.
#define DISTANCE_ECHO_PIN 35 // HC-SR04 ECHO. **Si 5V, ajouter Diviseur de tension OBLIGATOIRE.**

// ------------------------------------
// Lecteur de Carte SD (SPI)
// ------------------------------------
// Configuration du lecteur de carte SD utilisant le protocole SPI.
// Note: Partage le bus SPI avec l'écran TFT (MOSI, SCLK), seul CS est dédié.
#define SD_MISO           37  // SPI MISO (Master In Slave Out) : Broche de données depuis la carte SD. Câblage direct.
#define SD_MOSI           11  // SPI MOSI (partagé avec TFT). Câblage direct.
#define SD_SCLK           12  // SPI Clock (partagé avec TFT). Câblage direct.
#define SD_CS             36  // Chip Select SD : Broche pour activer spécifiquement la carte SD. Câblage direct.

// ------------------------------------
// Bouton Codeur Rotatif HW-040
// ------------------------------------
// Configuration du bouton codeur rotatif (rotary encoder) avec bouton intégré.
#define ENCODER_CLK       14  // CLK (Clock/A) : Signal d'horloge du codeur. Utiliser **Pull-Up interne**.
#define ENCODER_DT        47  // DT (Data/B) : Signal de données du codeur. Utiliser **Pull-Up interne**.
#define ENCODER_SW        40  // SW (Switch) : Bouton-poussoir intégré. Utiliser **Pull-Up interne**.


// =========================================================
//         MAPPING ESP32 CLASSIQUE (DevKitC)
// =========================================================
#elif defined(TARGET_ESP32_CLASSIC)

// ============================================================
// RAPPELS DE SÉCURITÉ ESP32-WROOM (Basé sur le tableau de câblage)
// ============================================================
// - Tension logique: 3.3 V uniquement sur GPIO.
// - Éviter GPIO0 (strap), GPIO1, 3 (UART0 - console série).
// - GPIO34, 35, 36, 39: entrées seulement (pas de sortie, pas de pull interne).
// - GPS TX (vers RXD 16) et HC-SR04 ECHO (vers GPIO 35) : nécessitent un DIVISEUR DE TENSION si le signal est 5V.
// - LEDs : ajouter résistance série 220–470 O (0.25 W).
// - I2C (GPIO 21/22) : pull-up 4.7 kO vers 3.3 V OBLIGATOIRE.
// - Buzzer : transistor nécessaire (avec résistance de base 1–10 kO) si passif ou courant >12 mA.
// ============================================================

// ------------------------------------
// DÉTAIL TECHNIQUE : Le Diviseur de Tension
// ------------------------------------
// OBJECTIF : Protéger les entrées de l'ESP32 (3.3V max) lorsqu'elles reçoivent un signal d'un composant 5V.
// FONCTIONNEMENT : Le diviseur utilise deux résistances (R1 et R2) pour "diviser" la tension d'entrée (V_in).
// Le signal de sortie (V_out) que reçoit l'ESP32 est calculé par la formule :
// $$V_{out} = V_{in} \cdot \frac{R_2}{R_1 + R_2}$$
// Pour abaisser 5V à 3.3V (ou moins) pour l'ESP32, on choisit généralement R1=10 kOhm et R2=20 kOhm, 
// ou plus simplement R1=2kOhm et R2=3kOhm.
// S'applique ici aux broches d'entrée (RXD) du GPS et au signal ECHO du HC-SR04 si ils sont en 5V.
// 
// ------------------------------------

// ------------------------------------
// GPS (UART2)
// ------------------------------------
#define PIN_GPS_RXD           16  // GPS TX (vers RXD de l'ESP32). **Si 5V, Diviseur de tension OBLIGATOIRE.**
#define PIN_GPS_TXD           17  // GPS RX (vers TXD de l'ESP32). Câblage direct.
#define PIN_GPS_PPS           36  // GPS PPS. Entrée seule, câblage direct.

// ------------------------------------
// TFT ST7789 (SPI)
// ------------------------------------
#define TFT_SCLK              18  // SPI Clock. Câblage direct.
#define TFT_MOSI              23  // SPI MOSI. Câblage direct.
#define TFT_CS                27  // Chip Select. Câblage direct.
#define TFT_DC                14  // Data/Command. Câblage direct.
#define TFT_RST               25  // Reset écran. Pull-up/down selon librairie.
#define TFT_BL                32  // Backlight (PWM). Câblage direct (PWM).

// ------------------------------------
// I2C
// ------------------------------------
#define I2C_SDA       21  // SDA. **Pull-Up 4.7kO vers 3.3V OBLIGATOIRE.**
#define I2C_SCL       22  // SCL. **Pull-Up 4.7kO vers 3.3V OBLIGATOIRE.**

// ------------------------------------
// DÉTAIL TECHNIQUE : Le Bus I2C (Inter-Integrated Circuit)
// ------------------------------------
// OBJECTIF : Permettre à plusieurs capteurs/périphériques de communiquer avec l'ESP32 en utilisant seulement deux broches (SDA et SCL).
// L'IMPÉRATIF PULL-UP : Le bus I2C utilise des sorties à drain ouvert (open-drain). Des résistances externes (Pull-Up, typiquement 4.7 kOhm vers 3.3V) sont nécessaires pour tirer le signal vers le haut.
// 
// ------------------------------------

// ------------------------------------
// LED RGB
// ------------------------------------
#define RGB_LED_PIN_R 13  // Rouge. Résistance série **220O - 470O**. (Déplacé de 26 pour éviter le doublon avec LED V).
#define RGB_LED_PIN_G 26  // Vert. Résistance série **220O - 470O**.
#define RGB_LED_PIN_B 33  // Bleu. Résistance série **220O - 470O**.

// ------------------------------------
// Boutons
// ------------------------------------
#define PIN_BUTTON_BOOT       0   // Bouton BOOT (strap). Réservé au boot, ne pas utiliser comme bouton utilisateur.
#define PIN_BUTTON_1          2   // Bouton vers GND. Utiliser **Pull-Up interne**. (Déplacé de 32).
#define PIN_BUTTON_2          5   // Bouton vers GND. Utiliser **Pull-Up interne**. (Déplacé de 13 pour libérer LED R, et de 33 pour résoudre conflit).

// ------------------------------------
// Capteurs & Sorties
// ------------------------------------
#define PWM_PIN       4   // PWM générique. Résistance série **220O - 470O** si LED.
#define BUZZER_PIN    19  // Buzzer. Transistor + **Résistance base 1–10kO**.
#define DHT_PIN       15  // DHT. **Pull-Up 10kO vers 3.3V**. (Déplacé de 4 pour éviter le doublon avec PWM).
#define LIGHT_SENSOR_PIN 39 // LDR. Entrée seule. **Diviseur ~10kO**.

// ------------------------------------
// Capteurs de Distance
// ------------------------------------
#define DISTANCE_TRIG_PIN 12 // HC-SR04 TRIG. Sortie. (Déplacé de 32).
#define DISTANCE_ECHO_PIN 35 // HC-SR04 ECHO. Entrée seule. **Si 5V, Diviseur de tension OBLIGATOIRE.**

// ------------------------------------
// Lecteur de Carte SD (SPI)
// ------------------------------------
// Configuration du lecteur de carte SD utilisant le protocole SPI.
// Note: Partage le bus SPI avec l'écran TFT (MOSI, SCLK), seul CS est dédié.
#define SD_MISO           19  // SPI MISO (Master In Slave Out) : Broche de données depuis la carte SD. Câblage direct.
#define SD_MOSI           23  // SPI MOSI (partagé avec TFT). Câblage direct.
#define SD_SCLK           18  // SPI Clock (partagé avec TFT). Câblage direct.
#define SD_CS             5   // Chip Select SD : Broche pour activer spécifiquement la carte SD. Câblage direct. (Réutilise broche DHT si nécessaire).

// ------------------------------------
// Bouton Codeur Rotatif HW-040
// ------------------------------------
// Configuration du bouton codeur rotatif (rotary encoder) avec bouton intégré.
#define ENCODER_CLK       26  // CLK (Clock/A) : Signal d'horloge du codeur. Utiliser **Pull-Up interne**. (Partagé avec LED_V si non utilisée).
#define ENCODER_DT        27  // DT (Data/B) : Signal de données du codeur. Utiliser **Pull-Up interne**.
#define ENCODER_SW        33  // SW (Switch) : Bouton-poussoir intégré. Utiliser **Pull-Up interne**. (Partagé avec LED_B si non utilisée).

#else
    #error "Aucune cible definie ! Verifiez platformio.ini (TARGET_ESP32_...)"
#endif

#endif // BOARD_CONFIG_H