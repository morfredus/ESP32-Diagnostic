# Mapping des pins — Référence rapide (v3.21.0)

- **Cartes supportées :**
  - ESP32-S3-DevKitC-1 N16R8 (16Mo Flash, 8Mo PSRAM) — `esp32s3_n16r8`
  - ESP32-S3-DevKitC-1 N8R8 (8Mo Flash, 8Mo PSRAM) — `esp32s3_n8r8`
  - ESP32-DevKitC (4Mo Flash, sans PSRAM) — `esp32devkitc`
- Pin mapping défini dans `include/board_config.h` via `TARGET_ESP32_S3` ou `TARGET_ESP32_CLASSIC`.

## ESP32-S3 (N16R8 / N8R8)
- I2C : SDA=15, SCL=16
- Boutons : BTN1=38, BTN2=39
- LED RGB : R=21, G=45, B=47
- Capteurs : DHT=5, Lumière=4, HC-SR04 TRIG=3/ECHO=6, Mouvement=46, PWM/Buzzer=14
- TFT ST7789 : MOSI=11, SCLK=12, CS=10, DC=9, RST=13, BL=7
- GPS (UART1) : RXD=18, TXD=17, PPS=8
- NeoPixel : pin 48 (activé), 1 LED, luminosité 50
- Notes : GPIO 35–44 réservés (PSRAM/Flash) ; pin 48 dédiée NeoPixel ; broches de strapping 0/45/46 à laisser stables au boot ; garder GPIO 19/20 libres pour l’USB D-/D+ (I2C déplacé en 15/16) ; GPIO45 est une broche de strapping (LED verte éteinte au boot).

## ESP32 Classic (DevKitC)
- I2C : SDA=21, SCL=22
- Boutons : BTN1=32, BTN2=33 (avec pull-up interne)
- LED RGB : R=13, G=14, B=25
- Capteurs : DHT=32, Lumière=39, HC-SR04 TRIG=27/ECHO=33, PWM/Buzzer=5
- TFT ST7789 : MOSI=23, SCLK=18, CS=27, DC=14, RST=25, BL=32
- GPS (UART2) : RXD=16, TXD=17, PPS=36
- NeoPixel : pin `-1` (désactivé par défaut), nombre 8, luminosité 50
- Notes : GPIO 34/35/36/39 sont en entrée uniquement ; broches de strapping 0/2/4/5/12/15 évitées pour LED/périphériques ; GPIO 6-11 réservées au flash ; UART0 (GPIO1/3) protégé ; ADC2 inutilisable quand le Wi-Fi est actif ; voir docs/PIN_MAPPING_CHANGES_FR.md pour détails des 11 modifications.

## Conflits à éviter
- S3 : réserver GPIO 35–44 ; RGB utilise 21/45/47 ; NeoPixel sur pin 48 ; I2C sur 15/16, laisser 19/20 libres pour l'USB ; GPIO45 est une broche de strapping (laisser la LED éteinte au boot) ; Mouvement=46, Distance ECHO=6, Lumière=4 ; TFT occupe 7–13, GPS 17–18.
- Classic : éviter LED/périphériques sur broches de strapping (0/2/4/5/12/15) ; boutons utilisent GPIO avec pull-up interne (32/33) ; PWM/Buzzer sur GPIO 5 acceptable si LOW au boot ; protéger UART0 (GPIO1/3 réservés au USB-UART) ; GPIO 34/35/36/39 en entrée uniquement ; privilégier ADC1 pour les mesures analogiques ; TFT occupe GPIO 18/23/27/14/25/32.

## Comment changer les pins
- Statique : éditez `include/board_config.h`, reconstruisez avec `pio run -e <environment>`.
- Dynamique : via l'interface web (capteurs) ou la console série ; persiste jusqu'au redémarrage.

## Historique des versions
- v3.21.0 : Révision complète du pin mapping ESP32 Classic — 11 modifications pour résoudre les problèmes de boot (broches de strapping GPIO 4/12/15), communication USB-UART (protection GPIO 1/3), et boutons (GPIO 32/33 avec pull-up interne au lieu de 34/35 input-only). Ajout de rappels de sécurité détaillés dans `board_config.h`. Migration matérielle requise pour ESP32 Classic. Voir `docs/PIN_MAPPING_CHANGES_FR.md` pour la liste complète numérotée des changements.
- v3.20.2 : Les références des pins dans l'interface web sont désormais dynamiquement sourcées depuis `board_config.h` ; l'interface web affiche toujours les pins de la cible compilée correcte.
- v3.20.1 : I2C ESP32-S3 déplacé vers 15/16 et LED RGB Rouge vers 21 pour libérer l'USB D-/D+ (19/20) et stabiliser l'OTG ; documentation alignée.
- v3.20.1 : Pin mapping déplacé dans `board_config.h` ; boutons ESP32-S3 sur GPIO 38/39 pour éviter les conflits d'upload/reset ; documentation alignée.
- v3.18.3 : Résolution du conflit pin 48 entre NeoPixel et LED RGB sur ESP32-S3 ; réorganisation complète du pin mapping S3.
- v3.17.1 : Pin mapping ESP32-S3 et Classic actualisé (GPS, TFT, LED RGB, capteurs, boutons) + documentation alignée.
- v3.15.1 : Correctif mémoire critique pour l'interface web sur ESP32 Classic.
- v3.15.0 : Support multi-environnements avec mappings dédiés.
