# Mapping des Pins — Coup d'œil

- **Cartes supportées :**
  - ESP32-S3-DevKitC-1 N16R8 (16Mo Flash, 8Mo PSRAM) — `esp32s3_n16r8`
  - ESP32-S3-DevKitC-1 N8R8 (8Mo Flash, 8Mo PSRAM) — `esp32s3_n8r8`
  - ESP32-DevKitC (4Mo Flash, sans PSRAM) — `esp32devkitc`
- Firmware : ESP32 Diagnostic Suite v3.12.3
- **Important (ESP32-S3 uniquement) :** Sur les variantes ESP32‑S3 OPI, les GPIO `35..48` sont réservés par l'interface mémoire octale. Ne pas les utiliser pour les périphériques.
- **Note :** L'ESP32 classique (DevKitC) n'a pas cette restriction.

## Défauts (config.h)
- LED intégrée : `-1` (désactivée)
- NeoPixel : pin `-1`, nombre `8`, luminosité `50`
- LED RGB (séparées) : `R=14`, `G=13`, `B=18`
- Buzzer : `3`
- I2C (primaire) : `SDA=21`, `SCL=20`
- I2C (secondaire, optionnel) : `SDA=25`, `SCL=26`
- DHT (temp/humidité) : `19`
- Capteur de lumière (ADC) : `4`
- HC‑SR04 : `TRIG=16`, `ECHO=17`
- Détecteur de mouvement (PIR) : `39`
- Test PWM : `27`
- TFT ST7789 : `MOSI=11`, `SCLK=12`, `CS=10`, `DC=9`, `RST=7`, `BL=15`
- GPS (UART2) : `RXD=8` (GPS TX → ESP RX), `TXD=5` (GPS RX → ESP TX), `PPS=38`

## Notes ESP32‑S3
- Réservées (OPI) : `GPIO 35..48` → à ne pas assigner.
- Straps/spéciales : `GPIO 0`, `GPIO 45` sont des broches de strapping ; éviter si doute.
- E/S : privilégier des GPIO « classiques » pour `ECHO`.
- USB : USB natif via PHY interne ; éviter de réutiliser si CDC actif.

## Conseils HC‑SR04
- Alimentation : +5V, GND.
- ECHO vers 3,3V côté ESP32 (pont diviseur 5V→3,3V).
- `TRIG` = sortie ; `ECHO` = entrée.
- Défauts recommandés : `TRIG=16`, `ECHO=17`.

## Conflits à éviter
- N’utilisez pas `35..48` sur S3 OPI (flash/psram partagés).
- Évitez `GPIO 0` pour les capteurs (strap de boot).
- Si le TFT est activé, laissez libres `MOSI=11`, `SCLK=12`, `CS=10`, `DC=9`, `RST=7`, `BL=15`.

## Comment changer les pins
- Défauts statiques : éditez `include/config.h`.
- À chaud (UI) : section Capteurs pour modifier DHT, lumière, distance (HC‑SR04) et mouvement sans recompiler.
