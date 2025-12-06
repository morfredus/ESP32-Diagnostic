# ESP32 Diagnostic Suite (v3.18.2)

Firmware de diagnostic complet pour microcontrôleurs ESP32 avec tableau de bord web interactif, tests matériels automatisés et contenu bilingue (FR/EN). Le firmware cible PlatformIO avec ESP32 Arduino Core 3.3.3 et supporte les cibles ESP32-S3 et ESP32 Classic.

## Nouveautés de la version 3.18.2
1. **Clés de traduction** : Ajout des clés de traduction manquantes pour le support complet de l'interface GPS et capteurs environnementaux.
   - `gps_status` : Indicateur de statut GPS
   - `temperature_avg` : Label de température moyenne pour affichage multi-capteurs
   - `pressure_hpa` : Pression barométrique avec affichage d'unité
   - `altitude_calculated` : Altitude calculée depuis la pression

## Nouveautés de la version 3.18.1
1. **Correction capteur AHT20** : Correction de l'algorithme d'extraction de bits pour les valeurs d'humidité et température (extraction correcte de 20 bits depuis la réponse 6 octets).
2. **API capteurs environnementaux** : Correction de la structure JSON pour compatibilité interface web.

## Nouveautés de la version 3.18.0
1. **Support module GPS** : Intégration complète du récepteur GPS NEO-6M/NEO-8M/NEO-M.
   - Parsing de phrases NMEA (RMC, GGA, GSA, GSV)
   - Suivi latitude, longitude, altitude, vitesse, cap
   - Nombre de satellites et qualité du signal (HDOP, VDOP, PDOP)
   - Intégration interface web avec affichage statut GPS
2. **Capteurs environnementaux** : AHT20 (température/humidité) + BMP280 (pression/altitude) sur I2C.
   - Streaming en temps réel vers interface web
   - Calcul d'altitude depuis pression barométrique
   - Indicateurs d'état capteur (OK/Erreur/Non détecté)
3. **Améliorations interface web** : Section module GPS page Sans fil, capteurs environnementaux page Capteurs.

## Nouveautés de la version 3.17.1
1. **Pin mapping actualisé (ESP32-S3 & Classic)** : Défauts ajustés dans `include/config.h` pour aligner GPS, TFT, LED RGB et capteurs sur le routage DevKitC-1 tout en conservant les boutons. ESP32-S3 : GPS RX=18/TX=17/PPS=8 ; TFT MOSI=11, SCLK=12, CS=10, DC=9, RST=13, BL=7 ; LED RGB R=47/G=48/B=45 ; BTN1=1, BTN2=2 ; PWM/Buzzer=14 ; DHT=5 ; Mouvement=4 ; Lumière=19 ; HC-SR04 TRIG=3/ECHO=6. ESP32 Classic : GPS RX=16/TX=17/PPS=4 ; TFT MOSI=23, SCLK=18, CS=19, DC=27, RST=26, BL=13 ; LED RGB R=12/G=14/B=15 ; BTN1=34, BTN2=35 ; PWM/Buzzer=5 ; DHT=25 ; Mouvement=36 ; Lumière=2 ; HC-SR04 TRIG=32/ECHO=33.
2. **Docs + version** : Guides de mapping (EN/FR), README, matrices de fonctionnalités, guides d’usage et notes de build alignés sur les nouveaux défauts ; `PROJECT_VERSION` passe à `3.17.1`.

## Nouveautés de la version 3.17.0
1. **Boutons matériels (optionnel)** : Actions simples liées à BTN1/BTN2 sans changer le pin mapping.
  - BTN1 : appui court → bip bref du buzzer (feedback utilisateur).
  - BTN2 : appui court → cycle des couleurs de la LED RGB (rouge → vert → bleu → blanc).
  - Activation via `ENABLE_BUTTONS` dans `include/config.h`.

## Nouveautés de la version 3.16.0
1. **Surveillance Réseau** : Journalisation automatique des adresses IP des clients connectés dans le moniteur série pour un diagnostic amélioré.
2. **Affichage Statut WiFi** : Affichage en temps réel du statut de connexion réseau dans l'onglet WiFi montrant l'adresse IP actuelle, le SSID, la passerelle, le DNS et la force du signal.
3. **Configuration Résolution OLED** : Configuration dynamique de la résolution de l'écran OLED (largeur × hauteur) via l'interface web.
4. **Interface Configuration TFT** : Configuration complète de l'écran TFT incluant le pin mapping (MOSI, SCLK, CS, DC, RST, BL), la résolution et les paramètres de rotation.
5. **Nouveau Point d'API** : `/api/tft-config` pour une configuration TFT complète avec validation en temps réel.
6. **Info Écrans Améliorée** : Mise à jour de `/api/screens-info` pour inclure les détails complets de résolution et de broches pour OLED et TFT.

## Nouveautés de la version 3.15.1
**Correction critique** : Résolution de l'échec de chargement de l'interface web sur ESP32 Classic (`esp32devkitc`) causé par épuisement mémoire. Le contenu JavaScript est maintenant streamé par petits morceaux depuis PROGMEM, éliminant les crashes heap sur cartes sans PSRAM. Tous les environnements bénéficient d'une efficacité mémoire améliorée.

## Nouveautés de la version 3.15.0
1. **Support Multi-Environnements** : Trois environnements de build distincts pour différentes cartes ESP32 :
   - `esp32s3_n16r8` (par défaut) : ESP32-S3 avec 16Mo Flash + 8Mo PSRAM
   - `esp32s3_n8r8` : ESP32-S3 avec 8Mo Flash + 8Mo PSRAM
   - `esp32devkitc` : ESP32 Classic avec 4Mo Flash (sans PSRAM)
2. **Pin Mapping Spécifique Matériel** : Configurations de broches dédiées dans `config.h` pour chaque cible.
3. **Configuration ESP32-S3 (ajustée en 3.17.1)** : I2C (SDA=21, SCL=20), LED RGB (R=47, G=48, B=45), boutons BTN1=1 / BTN2=2, GPS RX=18/TX=17/PPS=8, TFT MOSI=11/SCLK=12/CS=10/DC=9/RST=13/BL=7, capteurs (PWM/Buzzer=14, DHT=5, Mouvement=4, Lumière=19, HC-SR04 TRIG=3/ECHO=6).
4. **Configuration ESP32 Classic (ajustée en 3.17.1)** : I2C (SDA=21, SCL=22), LED RGB (R=12, G=14, B=15), boutons BTN1=34 / BTN2=35, GPS RX=16/TX=17/PPS=4, TFT MOSI=23/SCLK=18/CS=19/DC=27/RST=26/BL=13, capteurs (PWM/Buzzer=5, DHT=25, Mouvement=36, Lumière=2, HC-SR04 TRIG=32/ECHO=33).
5. **Mappings Partagés** : Affectations de broches communes entre ESP32-S3 N8R8 et ESP32 Classic là où le matériel le permet.

## Points clés de la version 3.14.0
1. **Interface web TFT** : Cartouche complet pour tester l'affichage TFT ST7789 (240x240) via l'interface web.
2. **Tests TFT individuels** : 8 tests disponibles (écran démarrage, couleurs, formes, texte, lignes, animation, progression, message final).
3. **Boutons de retour écran de démarrage** : Restauration rapide de l'affichage de démarrage pour OLED et TFT.
4. **API REST TFT** : `/api/tft-test`, `/api/tft-step`, `/api/tft-boot` pour contrôle programmatique.
5. **API REST OLED** : `/api/oled-boot` pour restaurer l'écran de démarrage OLED.

## Structure du projet
- `src/main.cpp` – point d'entrée du firmware, boucle principale, ordonnanceur et gestionnaires HTTP.
- `include/languages.h` – catalogue de traductions et sélecteur de langue à chaud.
- `include/web_interface.h` – modèles HTML/CSS/JS fournis par le firmware.
- `include/wifi-config-example.h` – modèle pour stocker les identifiants Wi-Fi.
- `platformio.ini` – configuration PlatformIO pour toutes les cibles ESP32 supportées.
- `docs/` – wiki complet regroupant installation, utilisation, API et guides de déploiement (FR/EN).

## Documentation
| Sujet | Anglais | Français |
|-------|---------|----------|
| Wiki (accueil) | [home.md](docs/home.md) | [home_FR.md](docs/home_FR.md) |
| Vue d'ensemble | [OVERVIEW.md](docs/OVERVIEW.md) | [OVERVIEW_FR.md](docs/OVERVIEW_FR.md) |
| Matrice des fonctionnalités | [FEATURE_MATRIX.md](docs/FEATURE_MATRIX.md) | [FEATURE_MATRIX_FR.md](docs/FEATURE_MATRIX_FR.md) |
| Modules de diagnostic | [DIAGNOSTIC_MODULES.md](docs/DIAGNOSTIC_MODULES.md) | [DIAGNOSTIC_MODULES_FR.md](docs/DIAGNOSTIC_MODULES_FR.md) |
| Interface web | [WEB_INTERFACE.md](docs/WEB_INTERFACE.md) | [WEB_INTERFACE_FR.md](docs/WEB_INTERFACE_FR.md) |
| REST API | [API_REFERENCE.md](docs/API_REFERENCE.md) | [API_REFERENCE_FR.md](docs/API_REFERENCE_FR.md) |
| Compilation & déploiement | [BUILD_AND_DEPLOY.md](docs/BUILD_AND_DEPLOY.md) | [BUILD_AND_DEPLOY_FR.md](docs/BUILD_AND_DEPLOY_FR.md) |
| Sécurité | [SECURITY.md](docs/SECURITY.md) | [SECURITY_FR.md](docs/SECURITY_FR.md) |
| FAQ | [FAQ.md](docs/FAQ.md) | [FAQ_FR.md](docs/FAQ_FR.md) |
| Installation | [INSTALL.md](docs/INSTALL.md) | [INSTALL_FR.md](docs/INSTALL_FR.md) |
| Configuration | [CONFIG.md](docs/CONFIG.md) | [CONFIG_FR.md](docs/CONFIG_FR.md) |
| Guide d'utilisation | [USAGE.md](docs/USAGE.md) | [USAGE_FR.md](docs/USAGE_FR.md) |
| Architecture du firmware | [ARCHITECTURE.md](docs/ARCHITECTURE.md) | [ARCHITECTURE_FR.md](docs/ARCHITECTURE_FR.md) |
| Dépannage | [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md) | [TROUBLESHOOTING_FR.md](docs/TROUBLESHOOTING_FR.md) |
| Contribuer | [CONTRIBUTING.md](CONTRIBUTING.md) | [CONTRIBUTING_FR.md](CONTRIBUTING_FR.md) |
| Journal des modifications | [CHANGELOG.md](CHANGELOG.md) | [CHANGELOG_FR.md](CHANGELOG_FR.md) |

## Capacités principales
- Interface multilingue avec bascule FR/EN en temps réel sans rechargement.
- Diagnostics matériels automatisés couvrant GPIO, ADC, pads tactiles, PWM, I2C, SPI, PSRAM, flash et scan Wi-Fi.
- **Support écran TFT ST7789** avec écran de démarrage et état WiFi/IP en temps réel (résolution 240x240).
- Batterie de tests OLED 0,96" I2C avec déclenchement pas-à-pas et animations prévisualisées.
- **Support double affichage** : fonctionnement simultané OLED et TFT pour diagnostics améliorés.
- Endpoints REST pour lancer les diagnostics et récupérer les rapports (TXT/JSON/CSV/vue imprimable).
- Support multi-AP Wi-Fi, découverte mDNS via `http://ESP32-Diagnostic.local` et exports complets.
- Motifs NeoPixel/WS2812B optionnels et outils de benchmark CPU/mémoire.

## Compatibilité & prérequis
- **Cartes :**
  - ESP32-S3-DevKitC-1 N16R8 (16Mo Flash, 8Mo PSRAM OPI/QSPI) — Cible principale
  - ESP32-S3-DevKitC-1 N8R8 (8Mo Flash, 8Mo PSRAM)
  - ESP32-DevKitC Classic (4Mo Flash, sans PSRAM)
- **Plateforme :** PlatformIO avec l'Arduino Core ESP32 3.3.3+
- **Environnements de build :**
  - `esp32s3_n16r8` (défaut) : ESP32-S3 avec partition huge_app, PSRAM activée, support OPI
  - `esp32s3_n8r8` : ESP32-S3 avec partition huge_app, 8Mo Flash
  - `esp32devkitc` : ESP32 Classic avec partition par défaut, 4Mo Flash
- **Sélection d'environnement :** Modifiez `default_envs` dans `platformio.ini` ou utilisez `pio run -e <environnement>`
- **Bibliothèques :** Adafruit BusIO ^1.17, Adafruit GFX ^1.12, Adafruit ST7735/ST7789 ^1.11, Adafruit NeoPixel ^1.12, U8g2 ^2.36 (installation automatique via platformio.ini).

## Support
- Licence : [MIT](LICENSE)
- Support & remontées : [ouvrir un ticket GitHub](https://github.com/morfredus/ESP32-Diagnostic/issues/new/choose) avec étapes de reproduction et détails carte.
- Contribution : [forkez le dépôt](https://github.com/morfredus/ESP32-Diagnostic/fork) puis consultez [CONTRIBUTING_FR.md](CONTRIBUTING_FR.md).
