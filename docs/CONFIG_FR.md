# Configuration (FR)

## Identifiants Wi-Fi
Le firmware lit les identifiants dans `include/wifi-config.h`.

```cpp
// wifi-config.h (exemple)
#pragma once
#include <vector>
#include <WiFiMulti.h>

static WiFiMulti wifiMulti;

static const std::vector<std::pair<const char*, const char*>> WIFI_NETWORKS = {
    {"MonSSID", "MonMotDePasse"},
    {"SecoursSSID", "SecoursMotDePasse"}
};
```

- Déclarez au moins un couple SSID/mot de passe.
- Le firmware parcourt la liste jusqu'à réussir la connexion.
- Évitez de versionner des identifiants sensibles.

## Paramètres de langue
- Le français est la langue par défaut.
- Utilisez `/api/set-language?lang=en` ou le sélecteur FR/EN dans l'interface pour basculer en anglais.
- Les chaînes de traduction résident dans `languages.h` au sein de la structure `Translations`. Ajoutez une langue en étendant cette structure et en l'exposant dans l'interface.
- La version 3.9.0 conserve le catalogue partagé synchronisé tout en ajoutant l'écran Wi-Fi et les garde-fous NimBLE sans configuration supplémentaire.

## Configuration des affichages

### Configuration OLED
- Broches par défaut : SDA=21, SCL=20 (défaut ESP32-S3 dans ce projet).
- Modifiez le câblage à chaud via `/api/oled-config?sda=<pin>&scl=<pin>`.
- Après modification, l'écran est réinitialisé automatiquement et la cartographie reste en RAM.

### Configuration TFT ST7789 (v3.11.0+)
- Broches par défaut configurées dans `include/config.h` :
  - MOSI : GPIO 11
  - SCLK : GPIO 12
  - CS : GPIO 10
  - DC : GPIO 9
  - RST : GPIO 46
  - BL (rétro-éclairage) : GPIO 15
- Résolution TFT : 240x240 pixels
- Affiche l'écran de démarrage et l'état WiFi/IP en temps réel
- Activation/désactivation via le flag `ENABLE_TFT_DISPLAY` dans config.h
- Support du fonctionnement simultané OLED et TFT (mode double affichage)

## Configuration des capteurs

### Capteur de distance (HC-SR04)

- TRIG est une broche de sortie ; ECHO est une broche d'entrée.
- Alimentez le capteur en 5V et protégez la ligne ECHO avec un pont diviseur (5V vers 3,3V) avant l'entrée ESP32.
- ESP32‑S3 avec mémoire Octal PSRAM/Flash (ex. DevKitC‑1 N16R8) : évitez d'utiliser les GPIO 35..48 pour TRIG/ECHO car ces broches sont réservées par l'interface OPI. Le firmware signalera cette configuration comme invalide.
- Défauts depuis v3.12.3 : TRIG = GPIO 16, ECHO = GPIO 17.
- Cartographie alternative sur ESP32‑S3 si le bus I2C secondaire est inactif : TRIG = GPIO 26 (sortie), ECHO = GPIO 25 (entrée).

## Règles de nommage Bluetooth®
- Les noms doivent comporter 3 à 31 caractères ASCII alphanumériques (+ tiret ou underscore).
- Une saisie invalide renvoie HTTP 400 sans relancer la diffusion.
- Un renommage valide persiste jusqu'au redémarrage.

## Options avancées
- Activez la PSRAM dans le menu carte de l'Arduino IDE pour les ESP32-S3 afin d'accéder aux diagnostics mémoire étendus.
- Le firmware détecte automatiquement les variantes et active le support BLE sur les cibles compatibles (ESP32, ESP32-S3, ESP32-C3, ESP32-C6, ESP32-H2).
- Pour le debug, suivez le moniteur série : les retraits Wi-Fi (également reflétés sur l'écran OLED), les états BLE et la progression des tests y sont journalisés, puis interrogez `/api/memory-details` pour obtenir les métriques de fragmentation détaillées.
- NimBLE est sélectionné automatiquement sur les cartes ESP32-S3 et les cibles USB ; en 3.8.0, les résultats de scan sont validés avant exposition, évitant toute manipulation manuelle.
