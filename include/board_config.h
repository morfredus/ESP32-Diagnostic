#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

// =========================================================
//         Configuration Pinout ESP32-S3 DevKitC-1 N16R8
// =========================================================
#if defined(TARGET_ESP32_S3)

// ============================================================
// RAPPELS DE SÉCURITÉ ESP32-S3 (Basé sur le tableau de câblage)
// ============================================================
// - Tension logique: 3.3 V uniquement sur GPIO.
// - GPIO0 : broche BOOT (strapping) – réservée au boot, ne pas connecter d'élément maintenu à LOW. [cite: 55]
// - GPIO46 : utilisé pour le capteur PIR. À vérifier pour l'impact potentiel sur le JTAG (moins critique que GPIO0). [cite: 64, 65, 66]
// - GPS TX (vers RXD 18) et HC-SR04 ECHO (vers GPIO 35) : nécessitent un DIVISEUR DE TENSION si le signal est 5V. [cite: 5, 75]
// - LEDs : ajouter résistance série 220–470 O (0.25 W). [cite: 41, 44, 47, 50]
// - I2C (GPIO 15/16) : pull-up 4.7 kO vers 3.3 V OBLIGATOIRE. [cite: 32, 35]
// - Buzzer : transistor nécessaire (avec résistance de base 1–10 kO) si passif ou courant >12 mA. [cite: 52]
// ============================================================


// ------------------------------------
// GPS (UART1)
// ------------------------------------
#define PIN_GPS_RXD           18  // GPS TX (vers RXD de l'ESP32). **Si 5V, Diviseur de tension OBLIGATOIRE.** [cite: 4, 5]
#define PIN_GPS_TXD           17  // GPS RX (vers TXD de l'ESP32). Câblage direct. [cite: 6, 7]
#define PIN_GPS_PPS           8   // GPS PPS, Câblage direct. [cite: 9, 10]

// ------------------------------------
// TFT ST7789 (SPI)
// ------------------------------------
#define TFT_MOSI              11  // SPI MOSI. Câblage direct. [cite: 13, 14]
#define TFT_SCLK              12  // SPI Clock. Câblage direct. [cite: 16, 17]
#define TFT_CS                10  // Chip Select. Câblage direct. [cite: 19, 20]
#define TFT_DC                9   // Data/Command. Câblage direct. [cite: 22, 23]
#define TFT_RST               13  // Reset écran. Pull-up/down selon librairie. [cite: 25, 26]
#define TFT_BL                7   // Backlight (PWM). Câblage direct (PWM). [cite: 28, 29]

// ------------------------------------
// I2C
// ------------------------------------
#define DEFAULT_I2C_SDA       15  // SDA. **Pull-Up 4.7kO vers 3.3V OBLIGATOIRE.** [cite: 31, 32]
#define DEFAULT_I2C_SCL       16  // SCL. **Pull-Up 4.7kO vers 3.3V OBLIGATOIRE.** [cite: 34, 35]

// ------------------------------------
// LED RGB
// ------------------------------------
#define DEFAULT_RGB_LED_PIN_R 21 // Rouge. Résistance série **220O - 470O**. [cite: 39, 41]
#define DEFAULT_RGB_LED_PIN_G 41 // Vert. Résistance série **220O - 470O**. [cite: 42, 44]
#define DEFAULT_RGB_LED_PIN_B 42 // Bleu. Résistance série **220O - 470O**. [cite: 45, 47]
#define NEOPIXEL_PIN          48 // NeoPixel Intégrée. Data (Réservée). [cite: 37, 38]

// ------------------------------------
// Boutons
// ------------------------------------
#define PIN_BUTTON_BOOT       0   // Bouton BOOT (strap). Réservé au boot. [cite: 53, 55]
#define PIN_BUTTON_1          38  // Bouton vers GND. Utiliser **Pull-Up interne**. [cite: 56, 57]
#define PIN_BUTTON_2          39  // Bouton vers GND. Utiliser **Pull-Up interne**. [cite: 59, 60]

// ------------------------------------
// Capteurs & Sorties
// ------------------------------------
#define DEFAULT_PWM_PIN       20  // PWM générique. Résistance série **220O - 470O** si LED. [cite: 48, 50]
#define DEFAULT_BUZZER_PIN    6   // Buzzer. Transistor + **Résistance base 1–10kO**. [cite: 51, 52]
#define DEFAULT_DHT_PIN       5   // DHT. **Pull-Up 10kO vers 3.3V**. [cite: 62, 63]
#define DEFAULT_MOTION_SENSOR_PIN 46 // PIR. Câblage direct. **(Potentiel JTAG - attention au boot).** [cite: 64, 66]
#define DEFAULT_LIGHT_SENSOR_PIN 4  // LDR. **Diviseur ~10kO**. [cite: 68, 69]

// ------------------------------------
// Capteurs de Distance
// ------------------------------------
#define DEFAULT_DISTANCE_TRIG_PIN 2  // HC-SR04 TRIG. Câblage direct. [cite: 71, 72]
#define DEFAULT_DISTANCE_ECHO_PIN 35 // HC-SR04 ECHO. **Si 5V, ajouter Diviseur de tension OBLIGATOIRE.** [cite: 74, 75]


// =========================================================
//            MAPPING ESP32 CLASSIQUE (DevKitC)
// =========================================================
#elif defined(TARGET_ESP32_CLASSIC)

// ============================================================
// RAPPELS DE SÉCURITÉ ESP32-WROOM (Basé sur le tableau de câblage)
// ============================================================
// - Tension logique: 3.3 V uniquement sur GPIO.
// - Éviter GPIO0 (strap), GPIO1, 3 (UART0).
// - GPIO34, 35, 36, 39: entrées seulement (pas de sortie, pas de pull interne). [cite: 85, 137]
// - GPS TX (vers RXD 16) et HC-SR04 ECHO (vers GPIO 35) : nécessitent un DIVISEUR DE TENSION si le signal est 5V. [cite: 81, 145]
// - LEDs : ajouter résistance série 220–470 O (0.25 W). [cite: 114, 117, 120, 123]
// - I2C (GPIO 21/22) : pull-up 4.7 kO vers 3.3 V OBLIGATOIRE. [cite: 107, 110]
// - Buzzer : transistor nécessaire (avec résistance de base 1–10 kO) si passif ou courant >12 mA. [cite: 125]
// ============================================================

// ------------------------------------
// GPS (UART2)
// ------------------------------------
#define PIN_GPS_RXD           16  // GPS TX (vers RXD de l'ESP32). **Si 5V, Diviseur de tension OBLIGATOIRE.** [cite: 80, 81]
#define PIN_GPS_TXD           17  // GPS RX (vers TXD de l'ESP32). Câblage direct. [cite: 82, 83]
#define PIN_GPS_PPS           36  // GPS PPS. Entrée seule, câblage direct. [cite: 85, 86]

// ------------------------------------
// TFT ST7789 (SPI)
// ------------------------------------
#define TFT_SCLK              18  // SPI Clock. Câblage direct. [cite: 91, 92]
#define TFT_MOSI              23  // SPI MOSI. Câblage direct. [cite: 88, 89]
#define TFT_CS                27  // Chip Select. Câblage direct. [cite: 94, 95]
#define TFT_DC                14  // Data/Command. Câblage direct. [cite: 97, 98]
#define TFT_RST               25  // Reset écran. Pull-up/down selon librairie. [cite: 100, 101]
#define TFT_BL                32  // Backlight (PWM). Câblage direct (PWM). [cite: 102, 103]

// ------------------------------------
// I2C
// ------------------------------------
#define DEFAULT_I2C_SDA       21  // SDA. **Pull-Up 4.7kO vers 3.3V OBLIGATOIRE.** [cite: 106, 107]
#define DEFAULT_I2C_SCL       22  // SCL. **Pull-Up 4.7kO vers 3.3V OBLIGATOIRE.** [cite: 109, 110]

// ------------------------------------
// LED RGB
// ------------------------------------
#define DEFAULT_RGB_LED_PIN_R 13 // Rouge. Résistance série **220O - 470O**. [cite: 113, 114]
#define DEFAULT_RGB_LED_PIN_G 26 // Vert. Résistance série **220O - 470O**. [cite: 116, 117]
#define DEFAULT_RGB_LED_PIN_B 33 // Bleu. Résistance série **220O - 470O**. [cite: 119, 120]

// ------------------------------------
// Boutons
// ------------------------------------
#define PIN_BUTTON_BOOT       0   // Bouton BOOT (strap). Réservé au boot, ne pas utiliser comme bouton utilisateur. [cite: 126, 128]
#define PIN_BUTTON_1          32  // Bouton vers GND. Utiliser **Pull-Up interne**. (32 est aussi TFT_BL - vérifier si un bouton est nécessaire). [cite: 129, 130]
#define PIN_BUTTON_2          33  // Bouton vers GND. Utiliser **Pull-Up interne**. (33 est aussi LED B et DHT - vérifier le conflit). [cite: 132, 133]

// ------------------------------------
// Capteurs & Sorties
// ------------------------------------
#define DEFAULT_PWM_PIN       4   // PWM générique. Résistance série **220O - 470O** si LED. [cite: 121, 123]
#define DEFAULT_BUZZER_PIN    19  // Buzzer. Transistor + **Résistance base 1–10kO**. [cite: 124, 125]
#define DEFAULT_DHT_PIN       33  // DHT. **Pull-Up 10kO vers 3.3V**. (33 est aussi LED B et Bouton 2 - **Conflit potentiel !**) [cite: 134, 136]
#define DEFAULT_LIGHT_SENSOR_PIN 39 // LDR. Entrée seule. **Diviseur ~10kO**. [cite: 137, 139]

// ------------------------------------
// Capteurs de Distance
// ------------------------------------
#define DEFAULT_DISTANCE_TRIG_PIN 32 // HC-SR04 TRIG. Sortie (32 est aussi TFT_BL et Bouton 1 - **Conflit potentiel !**) [cite: 141, 142]
#define DEFAULT_DISTANCE_ECHO_PIN 35 // HC-SR04 ECHO. Entrée seule. **Si 5V, Diviseur de tension OBLIGATOIRE.** [cite: 144, 145]

#else
    #error "Aucune cible definie ! Verifiez platformio.ini (TARGET_ESP32_...)"
#endif

#endif // BOARD_CONFIG_H