# Configuration (FR) — v3.20.1

## Sélection de l'Environnement de Build

**Nouveau dans v3.15.0 :** Le firmware supporte trois configurations matérielles via les environnements PlatformIO :

- **`esp32s3_n16r8`** (par défaut) : ESP32-S3 avec Flash 16Mo, PSRAM 8Mo
- **`esp32s3_n8r8`** : ESP32-S3 avec Flash 8Mo, PSRAM 8Mo
- **`esp32devkitc`** : ESP32 Classic avec Flash 4Mo, sans PSRAM

Sélectionnez votre cible dans `platformio.ini` :
```ini
[platformio]
default_envs = esp32s3_n16r8  ; Changer pour esp32s3_n8r8 ou esp32devkitc
```

Ou via ligne de commande :
```bash
pio run -e esp32devkitc --target upload
```

## Mappings de Broches Spécifiques au Matériel

Les configurations de broches sont définies dans `include/board_config.h` (inclus automatiquement via `config.h`) via compilation conditionnelle :

- **Cibles ESP32-S3** (`esp32s3_n16r8`, `esp32s3_n8r8`) : Utilisent le define `TARGET_ESP32_S3`
- **ESP32 Classic** (`esp32devkitc`) : Utilise le define `TARGET_ESP32_CLASSIC`

Voir [PIN_MAPPING_FR.md](PIN_MAPPING_FR.md) pour la référence complète des broches par environnement.

## Identifiants Wi-Fi
Le firmware lit les identifiants dans `include/secrets.h` (copiez `include/secrets-example.h`). Le fichier est ignoré du dépôt via `.gitignore`.

```cpp
// secrets.h (exemple)
const char* WIFI_SSID_1 = "MonSSID";
const char* WIFI_PASS_1 = "MonMotDePasse";

// Réseau de secours optionnel
const char* WIFI_SSID_2 = "SecoursSSID";
const char* WIFI_PASS_2 = "SecoursMotDePasse";
```

- Déclarez au moins un couple SSID/mot de passe.
- Ne versionnez pas les identifiants sensibles.

## Paramètres de langue
- Le français est la langue par défaut.
- Utilisez `/api/set-language?lang=en` ou le sélecteur FR/EN dans l'interface pour basculer en anglais.
- Les chaînes de traduction résident dans `languages.h` au sein de la structure `Translations`. Ajoutez une langue en étendant cette structure et en l'exposant dans l'interface.
- La version 3.9.0 conserve le catalogue partagé synchronisé tout en ajoutant l'écran Wi-Fi et les garde-fous NimBLE sans configuration supplémentaire.

## Configuration des affichages

### Configuration OLED
- Broches par défaut : SDA=15, SCL=16 (défaut ESP32-S3 dans ce projet ; garder GPIO19/20 libres pour l’USB D-/D+).
- Modifiez le câblage à chaud via `/api/oled-config?sda=<pin>&scl=<pin>`.
- Après modification, l'écran est réinitialisé automatiquement et la cartographie reste en RAM.

### Configuration TFT ST7789 (v3.11.0+)
- **Les broches varient selon l'environnement** (configurées dans `include/config.h`) :

**ESP32-S3 (esp32s3_n16r8 / esp32s3_n8r8) :**
  - MOSI : GPIO 11, SCLK : GPIO 12
  - CS : GPIO 10, DC : GPIO 9
  - RST : GPIO 13, BL : GPIO 7

**ESP32 Classic (esp32devkitc) :**
  - MOSI : GPIO 23, SCLK : GPIO 18
  - CS : GPIO 15, DC : GPIO 2
  - RST : GPIO 4, BL : GPIO 32

- Résolution TFT : 240x240 pixels
- Affiche l'écran de démarrage et l'état WiFi/IP en temps réel
- Activation/désactivation via le flag `ENABLE_TFT_DISPLAY` dans config.h
- Support du fonctionnement simultané OLED et TFT (mode double affichage)
- **Note :** Sur ESP32 Classic, SCLK (GPIO 18) entre en conflit avec la broche ECHO du HC-SR04

## Configuration des capteurs

## Boutons matériels (v3.17.1+)
- Fonctionnalité optionnelle activable via `ENABLE_BUTTONS` dans `include/config.h`.
- Utilise les broches `PIN_BUTTON_1` et `PIN_BUTTON_2` définies par cible dans `board_config.h`.
- Actions par défaut :
  - BTN1 : appui court → bip bref du buzzer (feedback utilisateur).
  - BTN2 : appui court → cycle des couleurs de la LED RGB (rouge → vert → bleu → blanc).
- ESP32-S3 : BTN1=38, BTN2=39 pour éviter les conflits d'upload/reset.
- Note ESP32 Classic : les GPIO 34–39 sont en entrée seule et sans pull‑up interne. Prévoir des résistances de tirage externes si nécessaire lorsque BTN2 est câblé sur GPIO 35.


### Capteur de distance (HC-SR04)

- TRIG est une broche de sortie ; ECHO est une broche d'entrée.
- Alimentez le capteur en 5V et protégez la ligne ECHO avec un pont diviseur (5V vers 3,3V) avant l'entrée ESP32.
- ESP32‑S3 avec mémoire Octal PSRAM/Flash (ex. DevKitC‑1 N16R8) : évitez d'utiliser les GPIO 35..48 pour TRIG/ECHO car ces broches sont réservées par l'interface OPI. Le firmware signalera cette configuration comme invalide.
- Défauts : TRIG = GPIO 3, ECHO = GPIO 6.
- Cartographie alternative sur ESP32-S3 si le bus I2C secondaire est inactif : TRIG = GPIO 26 (sortie), ECHO = GPIO 25 (entrée).

## Règles de nommage Bluetooth®
- Les noms doivent comporter 3 à 31 caractères ASCII alphanumériques (+ tiret ou underscore).
- Une saisie invalide renvoie HTTP 400 sans relancer la diffusion.
- Un renommage valide persiste jusqu'au redémarrage.

## Options avancées
- Activez la PSRAM dans le menu carte de l'Arduino IDE pour les ESP32-S3 afin d'accéder aux diagnostics mémoire étendus.
- **Support BLE :** Non disponible dans cette version PlatformIO. La fonctionnalité BLE a été supprimée.
- **Support multi-cartes :** Sélectionner l'environnement approprié dans PlatformIO (`esp32s3_n16r8`, `esp32s3_n8r8`, ou `esp32devkitc`) avant compilation.
- Pour le debug, suivez le moniteur série : les retraits Wi-Fi (également reflétés sur l'écran OLED), les états BLE et la progression des tests y sont journalisés, puis interrogez `/api/memory-details` pour obtenir les métriques de fragmentation détaillées.
- NimBLE est sélectionné automatiquement sur les cartes ESP32-S3 et les cibles USB ; en 3.8.0, les résultats de scan sont validés avant exposition, évitant toute manipulation manuelle.
