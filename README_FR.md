# ESP32 Diagnostic Suite (v3.23.0)

> **AVERTISSEMENT** : v3.22.0 remappe les GPIO pour ESP32-S3 et ESP32 Classic. Un recâblage matériel est nécessaire. Lisez [docs/PIN_MAPPING_FR.md](docs/PIN_MAPPING_FR.md) et [docs/PIN_MAPPING.md](docs/PIN_MAPPING.md) avant de flasher.

Firmware de diagnostic complet pour microcontrôleurs ESP32 avec tableau de bord web interactif, tests matériels automatisés et contenu bilingue (FR/EN). Le firmware cible PlatformIO avec ESP32 Arduino Core 3.3.3 et supporte les cibles ESP32-S3 et ESP32 Classic.

## 🌐 Nouveautés de la version 3.23.0 - Intégration MQTT Publisher

**Intégration en temps réel à l'écosystème IoT via broker MQTT :**
- **MQTT Publisher** - Diffusion de 20+ métriques système vers n'importe quel broker MQTT
- **Home Assistant prêt** - Compatible immédiatement avec les capteurs MQTT
- **Auto-publication 30s** - Mises à jour automatiques non bloquantes
- **Contrôle API REST** - Activer/désactiver, vérification d'état, test de publication
- **NodeRED/InfluxDB** - Tableaux de bord temps réel et journalisation série temporelle
- **Mémoire sécurisée** - Seulement 18KB d'impact, fonctionne sur ESP32 Classic 4MB
- **Documentation** - Guides complets EN/FR : [MQTT_INTEGRATION_FR.md](docs/MQTT_INTEGRATION_FR.md)

## Précédent : Version 3.22.0 - Remapping GPIO

**Mise à jour de configuration matérielle** - GPIO ESP32-S3 repositionnés pour éviter conflits de strapping.

## Précédent : Version 3.21.1 - Indicateur d'état Wi-Fi NeoPixel

**Révision complète du pin mapping pour ESP32-S3 et ESP32 Classic** - Migration matérielle requise :
- **Corrections ESP32-S3** : LED RGB repositionnées pour libérer GPIO 45/47 (broches de strapping) vers GPIO 41/42 (broches dédiées)
- **ESP32 Classic inchangé** : Configuration maintenue après optimisation v3.21.0
- **Documentation détaillée** : Voir [docs/PIN_MAPPING_FR.md](docs/PIN_MAPPING_FR.md) pour les tables complètes et [docs/PIN_MAPPING_CHANGES_FR.md](docs/PIN_MAPPING_CHANGES_FR.md) pour l'historique des modifications
- **Impact** : ESP32-S3 nécessite recâblage matériel, ESP32 Classic inchangé
- **Sécurité** : Configuration optimisée pour éviter les conflits avec les broches de strapping et USB

## Précédent : version 3.21.1 - Indicateur d'état Wi-Fi NeoPixel

**Rétroaction d'état de connectivité Wi-Fi en temps réel via LED RGB NeoPixel/WS2812 :**
- **Jaune (connexion en cours)** pendant la tentative de connexion Wi-Fi au démarrage
- **Battement vert (0, 50, 0) pulsation chaque 1 seconde** quand Wi-Fi connecté
- **Battement rouge (50, 0, 0) pulsation chaque 1 seconde** quand Wi-Fi déconnecté
- **Flash violet (255, 0, 255)** lors de la confirmation de l'appui long du bouton BOOT
- **Battement non bloquant** géré dans la boucle principale - mises à jour chaque 1 seconde
- **Isolation des tests** - le battement se met en pause lors des appels `/api/neopixel-test` et `/api/neopixel-pattern`, reprise automatique après

## Précédent : version 3.21.0

**Révision complète du pin mapping ESP32 Classic** - Migration matérielle requise :
- **11 modifications de pins** pour résoudre les problèmes de boot (broches de strapping GPIO 4/12/15) et de communication USB-UART (protection GPIO 1/3)
- **Boutons améliorés** : GPIO 32/33 avec pull-up interne (au lieu de 34/35 input-only)
- **LED RGB sécurisées** : Éloignées des broches de strapping (12→13, 15→25)
- **Stabilité GPS** : PPS déplacé de GPIO 4 (strapping) vers GPIO 36 (input-only dédié)
- **Documentation détaillée** : Voir `docs/PIN_MAPPING_CHANGES_FR.md` pour la liste numérotée des changements avec explications techniques
- **Impact** : ESP32-S3 inchangé, ESP32 Classic nécessite recâblage matériel
- **Sécurité** : Ajout de rappels détaillés dans `board_config.h` (tensions 3.3V, strapping, limites courant)

## Précédent : version 3.20.4

**Refactorisation du nom de projet** - Élimination des chaînes de nom de projet codées en dur :
- Toutes les occurrences de "ESP32 Diagnostic" remplacées par la macro `PROJECT_NAME` de platformio.ini
- Fichiers affectés : `main.cpp` (5 emplacements), `languages.h`, `web_interface.h`
- Le nom du projet est maintenant défini de manière centralisée dans la configuration de build
- Améliore la maintenabilité et la flexibilité de personnalisation

## Précédent : version 3.20.3

**Version d'optimisation du code** - Améliorations systématiques de l'efficacité mémoire :
- Application de 9 optimisations ([OPT-001] à [OPT-009]) éliminant 90+ allocations de chaînes
- Formatage basé sur des buffers pour les chaînes de version, uptime, listes GPIO et résultats de tests
- Création de constantes réutilisables (DEFAULT_TEST_RESULT_STR, OK_STR, FAIL_STR) utilisées dans 30+ emplacements
- Conversion de 13 messages de debug/statut en approche snprintf (2-9 allocations → 1)
- **Aucun changement de fonctionnalité** - tous les pins, tests et fonctionnalités restent identiques
- Compilé et validé avec succès sur les cibles ESP32-S3 et ESP32 CLASSIC

## Précédent : version 3.20.2
1. **Références Dynamiques des Pins dans l'Interface Web** : L'interface web affiche désormais les pins GPIO corrects en fonction de la cible compilée
   - Toutes les valeurs de pins codées en dur remplacées par des références dynamiques depuis `board_config.h`
   - Les pins LED RGB, capteur DHT, capteur de lumière, capteur de distance, capteur de mouvement et PWM lisent maintenant la configuration
   - Les valeurs de pins sont injectées comme constantes JavaScript, garantissant que l'interface affiche toujours les pins matériels réels
   - La bannière de démarrage série affiche l'identification TARGET et les valeurs réelles des pins pour vérification

## Nouveautés de la version 3.20.1
1. **Stabilité USB/OTG (ESP32-S3)** : Les lignes USB D-/D+ sont libérées en déplaçant les défauts hors GPIO19/20.
   - I2C par défaut : SDA=15, SCL=16
   - LED RGB : Rouge=21, Vert=45, Bleu=47 (Rouge quitte GPIO19)
   - OTG stabilisé ; rappel : GPIO45 est une broche de strapping, LED laissée éteinte au boot.

## Nouveautés de la version 3.20.0
1. **Gestion Avancée des Boutons** : Fonctionnalités de boutons améliorées avec retour visuel et contrôles interactifs
   - **Bouton BOOT (GPIO 0)** : Appui long (2 secondes) déclenche un redémarrage système avec barre de progression sur l'écran TFT ; relâcher avant 2s efface l'écran
   - **Bouton 1 (GPIO 38)** : Appui court cycle à travers les couleurs LED RGB (Éteint → Rouge → Vert → Bleu → Blanc)
   - **Bouton 2 (GPIO 39)** : Appui court déclenche un bip de confirmation
   - Retour visuel en temps réel durant les opérations d'appui long sur TFT
   - Antirebond et détection d'appui long pour une opération fiable

## Nouveautés de la version 3.19.0
1. **Isolement Pin Mapping** : Définitions GPIO des cartes extraites vers `include/board_config.h` ; `config.h` contient maintenant uniquement les paramètres de configuration communes.
   - Toutes les définitions de broches pour ESP32-S3 (N16R8 / N8R8) et ESP32 Classic consolidées en un seul endroit
   - Les boutons restent sur GPIO 38/39 (ESP32-S3) pour éviter les conflits de chargement/réinitialisation USB
   - Aucune autre valeur de broche n'a changé
2. **Gestion des secrets** : Fichier de configuration Wi-Fi renommé de `wifi-config.h` en `secrets.h`
   - Les anciens en-têtes `wifi-config.h` émettent maintenant une erreur de compilation pour forcer la migration
   - `secrets.h` est protégé dans `.gitignore` pour éviter les fuites d'identifiants
   - Utilisez `include/secrets-example.h` comme modèle
3. **Documentation complète** : Tous les documents pour utilisateurs mis à jour pour refléter les changements d'architecture et les fonctionnalités v3.19.0
4. **Cohérence des versions** : `PROJECT_VERSION` mise à jour vers 3.19.0 dans `platformio.ini`

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
2. **Pin Mapping Spécifique Matériel** : Configurations de broches dédiées dans `board_config.h` (automatiquement inclus par `config.h`) pour chaque cible.
3. **Configuration ESP32-S3 (ajustée en 3.19.0)** : I2C (SDA=21, SCL=20), LED RGB (R=19, G=47, B=45), NeoPixel=48, boutons BTN1=38 / BTN2=39, GPS RX=18/TX=17/PPS=8, TFT MOSI=11/SCLK=12/CS=10/DC=9/RST=13/BL=7, capteurs (PWM/Buzzer=14, DHT=5, Mouvement=46, Lumière=4, HC-SR04 TRIG=3/ECHO=6).
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
- `include/board_config.h` – mapping de broches spécifique à la carte (sélection via TARGET_ESP32_*).
- `include/secrets-example.h` – modèle pour stocker les identifiants Wi-Fi (copier vers `include/secrets.h`).
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
