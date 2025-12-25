## Sélection Dynamique du Driver TFT (v3.30.0)

**NOUVEAU :** Vous pouvez maintenant basculer entre les contrôleurs TFT (`ILI9341` ou `ST7789`) dynamiquement depuis l'interface Web sans recompiler !

### Changement de Driver en Temps Réel (Interface Web)

1. **Accéder à l'Interface Web** → Naviguer vers la section Écran TFT
2. **Sélectionner le Driver** → Choisir dans le menu déroulant : ILI9341 ou ST7789
3. **Appliquer** → Le driver change instantanément (pas de redémarrage requis)
4. **Tester** → Utiliser les tests d'affichage intégrés immédiatement

### Driver par Défaut au Démarrage

Définir le driver par défaut dans `include/config.h` :
```cpp
// v3.30.0: Both drivers loaded simultaneously
#define TFT_USE_ST7789 // Uncomment to use ST7789 as default driver at boot
// #define TFT_USE_ILI9341  // Uncomment to use ILI9341 as default driver at boot

#define TFT_WIDTH   240
#define TFT_HEIGHT  320
```

**Note :** Le `#define` définit uniquement le driver initial. Vous pouvez le changer à tout moment depuis l'interface Web.

### Avantages

- ⚡ **Pas de Recompilation** : Testez différents drivers sans reflasher
- 🔄 **Changement Instantané** : Changez de driver à la volée
- 🧪 **Tests Faciles** : Identifiez rapidement les problèmes de compatibilité
- 📦 **Firmware Unique** : Une seule compilation supporte les deux types d'écran

Voir [RELEASE_NOTES_3.30.0_FR.md](RELEASE_NOTES_3.30.0_FR.md) pour tous les détails.

# Configuration (FR) — v3.30.0

> **AVERTISSEMENT** : Ce document reflète le firmware **v3.30.0** avec mappings de broches EXACTS depuis `include/board_config.h`. Toutes les assignations GPIO ont été vérifiées et synchronisées avec le code. Lisez [docs/PIN_MAPPING_FR.md](docs/PIN_MAPPING_FR.md) avant de flasher.

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
- La version 3.30.0 conserve la sélection dynamique du contrôleur TFT, la synchronisation complète des mappings GPIO, et le support multi-environnements. Voir [RELEASE_NOTES_3.30.0_FR.md](RELEASE_NOTES_3.30.0_FR.md) pour les nouveautés.

## Configuration des affichages

### Configuration OLED
- Broches par défaut : SDA=15, SCL=16 (défaut ESP32-S3 dans ce projet ; garder GPIO19/20 libres pour l’USB D-/D+).
- Modifiez le câblage à chaud via `/api/oled-config?sda=<pin>&scl=<pin>`.
- Après modification, l'écran est réinitialisé automatiquement et la cartographie reste en RAM.

### Configuration TFT ST7789 (v3.11.0+)
- **Les broches varient selon l'environnement** (configurées dans `include/config.h`) :

**ESP32-S3 (esp32s3_n16r8 / esp32s3_n8r8) :**
  - MISO : GPIO 13 (partagé avec carte SD), MOSI : GPIO 11, SCLK : GPIO 12
  - CS : GPIO 10, DC : GPIO 9
  - RST : GPIO 14 ⚠️ (était GPIO 13 dans anciennes docs - FAUX !), BL : GPIO 7

**ESP32 Classic (esp32devkitc) :**
  - MOSI : GPIO 23, SCLK : GPIO 18
  - CS : GPIO 27, DC : GPIO 14
  - RST : GPIO 25, BL : GPIO 32

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
- ESP32-S3 : BUTTON_BOOT=0 (lecture seule), BTN1=38, BTN2=39 pour éviter les conflits d'upload/reset.
- ESP32 Classic : BUTTON_BOOT=0 (lecture seule), BTN1=5, BTN2=12 avec pull-up interne ⚠️ (anciennes docs indiquaient 32/33 ou 34/35 - TOUS FAUX !).


### Capteur de distance (HC-SR04)

- TRIG est une broche de sortie ; ECHO est une broche d'entrée.
- Alimentez le capteur en 5V et protégez la ligne ECHO avec un pont diviseur (5V vers 3,3V) avant l'entrée ESP32.
- ESP32‑S3 avec mémoire Octal PSRAM/Flash (ex. DevKitC‑1 N16R8) : évitez d'utiliser les GPIO 35..48 pour TRIG/ECHO car ces broches sont réservées par l'interface OPI. Le firmware signalera cette configuration comme invalide.
- Défauts ESP32-S3 : TRIG = GPIO 2 ⚠️ (anciennes docs indiquaient 3 - FAUX !), ECHO = GPIO 35 ⚠️ (anciennes docs indiquaient 6 - FAUX !).
- Défauts ESP32 Classic : TRIG = GPIO 1, ECHO = GPIO 35.
- Voir `docs/PIN_MAPPING_FR.md` pour assignations GPIO actuelles depuis `board_config.h`.

## Règles de nommage Bluetooth®
- Les noms doivent comporter 3 à 31 caractères ASCII alphanumériques (+ tiret ou underscore).
- Une saisie invalide renvoie HTTP 400 sans relancer la diffusion.
- Un renommage valide persiste jusqu'au redémarrage.

## Options avancées
- Activez la PSRAM dans le menu carte de l'Arduino IDE pour les ESP32-S3 afin d'accéder aux diagnostics mémoire étendus.
- **Support BLE :** Non disponible dans cette version PlatformIO (v3.30.0). La fonctionnalité BLE a été supprimée.
- **Support multi-cartes :** Sélectionner l'environnement approprié dans PlatformIO (`esp32s3_n16r8`, `esp32s3_n8r8`, ou `esp32devkitc`) avant compilation.
- Pour le debug, suivez le moniteur série : les retraits Wi-Fi (également reflétés sur l'écran OLED), les états BLE et la progression des tests y sont journalisés, puis interrogez `/api/memory-details` pour obtenir les métriques de fragmentation détaillées.
- NimBLE est sélectionné automatiquement sur les cartes ESP32-S3 et les cibles USB ; en 3.8.0, les résultats de scan sont validés avant exposition, évitant toute manipulation manuelle.
