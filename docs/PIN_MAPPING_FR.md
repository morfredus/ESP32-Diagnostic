# Mapping des pins — Référence rapide (v3.17.1)

- **Cartes supportées :**
  - ESP32-S3-DevKitC-1 N16R8 (16Mo Flash, 8Mo PSRAM) — `esp32s3_n16r8`
  - ESP32-S3-DevKitC-1 N8R8 (8Mo Flash, 8Mo PSRAM) — `esp32s3_n8r8`
  - ESP32-DevKitC (4Mo Flash, sans PSRAM) — `esp32devkitc`
- Pin mapping défini dans `include/config.h` via `TARGET_ESP32_S3` ou `TARGET_ESP32_CLASSIC`.

## ESP32-S3 (N16R8 / N8R8)
- I2C : SDA=21, SCL=20
- Boutons : BTN1=1, BTN2=2
- LED RGB : R=47, G=48, B=45
- Capteurs : DHT=5, Lumière=19, HC-SR04 TRIG=3/ECHO=6, Mouvement=4, PWM/Buzzer=14
- TFT ST7789 : MOSI=11, SCLK=12, CS=10, DC=9, RST=13, BL=7
- GPS (UART1) : RXD=18, TXD=17, PPS=8
- NeoPixel : pin `-1` (désactivé) — si activé, utiliser une broche libre comme 38
- Notes : GPIO 35–44 réservés (PSRAM/Flash) ; GPIO 45/47/48 utilisés par la LED RGB ; broches de strapping 0/45/46 à laisser stables au boot.

## ESP32 Classic (DevKitC)
- I2C : SDA=21, SCL=22
- Boutons : BTN1=34, BTN2=35 (entrées uniquement)
- LED RGB : R=12, G=14, B=15
- Capteurs : DHT=25, Lumière=2, HC-SR04 TRIG=32/ECHO=33, Mouvement=36, PWM/Buzzer=5
- TFT ST7789 : MOSI=23, SCLK=18, CS=19, DC=27, RST=26, BL=13
- GPS (UART2) : RXD=16, TXD=17, PPS=4
- NeoPixel : pin `-1` (désactivé par défaut), nombre 8, luminosité 50
- Notes : GPIO 34/35/36/39 sont en entrée uniquement ; broches de strapping 0/2/5/12/15 ; GPIO 6-11 réservées au flash ; ADC2 inutilisable quand le Wi-Fi est actif.

## Conflits à éviter
- S3 : réserver GPIO 35–44 ; ne pas réutiliser 45/47/48 (LED RGB) ; TFT occupe 7–13, GPS 17–18 ; choisir un pin libre (ex. 38) pour NeoPixel.
- Classic : boutons en entrée seulement ; PWM/Buzzer sur GPIO 5 est une broche de boot ; éviter de partager les GPIO 18/19/23/13/26/27 du TFT sans gestion stricte des chip select ; privilégier ADC1 pour les mesures analogiques.

## Comment changer les pins
- Statique : éditez `include/config.h`, reconstruisez avec `pio run -e <environment>`.
- Dynamique : via l'interface web (capteurs) ou la console série ; persiste jusqu'au redémarrage.

## Historique des versions
- v3.17.1 : Pin mapping ESP32-S3 et Classic actualisé (GPS, TFT, LED RGB, capteurs, boutons) + documentation alignée.
- v3.15.1 : Correctif mémoire critique pour l'interface web sur ESP32 Classic.
- v3.15.0 : Support multi-environnements avec mappings dédiés.
