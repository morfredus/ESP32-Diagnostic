# ESP32 Diagnostic Suite – Matrice des fonctionnalités (v3.22.0)

> **AVERTISSEMENT** : v3.22.0 remappe les GPIO pour ESP32-S3 et ESP32 Classic. Un recâblage matériel est nécessaire. Lisez [docs/PIN_MAPPING_FR.md](docs/PIN_MAPPING_FR.md) et [docs/PIN_MAPPING.md](docs/PIN_MAPPING.md) avant de flasher.

Cette matrice résume la couverture des diagnostics pour les cartes Espressif supportées par la version 3.22.0 et ses trois environnements de build.
Elle aide à planifier les campagnes de validation et à vérifier si des périphériques optionnels nécessitent un câblage supplémentaire.

> **Important :** La version 3.22.0 (PlatformIO) n'active toujours pas le Bluetooth/BLE. Pour les diagnostics BLE, utilisez la version Arduino IDE archivée [ESP32-Diagnostic-Arduino-IDE](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE).

**✨ Nouveau en v3.22.0 :** MQTT Publisher pour publication en temps réel des métriques système (mémoire, WiFi, capteurs) vers Home Assistant, NodeRED, InfluxDB et autres brokers MQTT. Non-bloquant avec auto-reconnexion. **Désactivé par défaut.**

**⚠️ Mis à jour en v3.21.0 :** Révision complète du pin mapping ESP32 Classic — 11 modifications pour résoudre les problèmes de boot et de communication USB. **Migration matérielle requise pour ESP32 Classic.** Voir `PIN_MAPPING_CHANGES_FR.md` pour détails. ESP32-S3 inchangé.

## Légende
- ✅ – Pris en charge nativement par le firmware.
- ⚙️ – Pris en charge avec configuration supplémentaire ou adaptation matérielle (voir colonne Notes).
- ⛔ – Non disponible pour la carte cible ou nécessite une extension firmware spécifique.

## Capacités des cartes principales
| Carte | Environnement | Scan Wi-Fi | Scan BLE | Sonde PSRAM | Intégrité flash | Test NeoPixel | Test OLED | Test TFT | Notes |
|-------|---------------|-----------|----------|-------------|-----------------|---------------|-----------|----------|-------|
| ESP32-S3 N16R8 | `esp32s3_n16r8` | ✅ | ⛔ | ✅ | ✅ | ✅ | ✅ | ✅ | Cible principale. 16Mo Flash, 8Mo PSRAM. Mémoire OPI (GPIO 35-48 réservés). |
| ESP32-S3 N8R8 | `esp32s3_n8r8` | ✅ | ⛔ | ✅ | ✅ | ✅ | ✅ | ✅ | 8Mo Flash, 8Mo PSRAM. Mêmes restrictions broches que N16R8. |
| ESP32 DevKitC | `esp32devkitc` | ✅ | ⛔ | ⛔ | ✅ | ✅ | ✅ | ✅ | ESP32 classique. 4Mo Flash, sans PSRAM. Pas de restriction GPIO 35-48. |

## Couverture des bus et périphériques
| Bus / Périphérique | Broches par cible | Cartes supportées | Notes |
|--------------------|--------------------|-------------------|-------|
| I2C principal | **ESP32-S3:** SDA=15, SCL=16 · **ESP32 Classic:** SDA=21, SCL=22 | Toutes les cartes supportées | Utilisé pour l'OLED, les packs capteurs, l'EEPROM (laisser GPIO19/20 libres pour l'USB). |
| LED RGB | **ESP32-S3:** R=21, G=45, B=47 · **ESP32 Classic (v3.21.0+):** R=13, G=14, B=25 | Toutes les cartes supportées | Broches PWM pour LED RGB. Classic : éloigné des broches de strapping (12/15). |
| **Indicateur Wi-Fi NeoPixel** | **ESP32-S3:** GPIO 48 · **ESP32 Classic:** GPIO 2 | Toutes les cartes supportées | LED RGB unique affichant l'état Wi-Fi : jaune (connexion), vert pulsant (connecté), rouge pulsant (déconnecté), flash violet (redémarrage). Nouveau en v3.21.1. |
| Boutons | **ESP32-S3:** BTN1=38, BTN2=39 · **ESP32 Classic (v3.21.0+):** BTN1=32, BTN2=33 | Toutes les cartes supportées | Classic : GPIO 32/33 avec pull-up interne (anciennement 34/35 input-only). |
| TFT ST7789 | **ESP32-S3:** MOSI=11, SCLK=12, CS=10, DC=9, RST=13, BL=7 · **ESP32 Classic (v3.21.0+):** MOSI=23, SCLK=18, CS=27, DC=14, RST=25, BL=32 | Toutes les cartes supportées | SPI dédié pour affichage TFT. Voir PIN_MAPPING_FR.md pour conflits potentiels. |
| Boucle UART | TX0/RX0 & UART1 optionnel | Toutes | Nécessite un cavalier TX↔RX sur l'UART choisi. |
| Température OneWire | Variable par cible | ESP32, ESP32-S3 | Détection DS18B20 lorsque `ENABLE_ONEWIRE` est actif. Voir PIN_MAPPING_FR.md. |

## Modules optionnels
| Module | Flag firmware | État par défaut | Description |
|--------|---------------|-----------------|-------------|
| Diagnostics OLED | `ENABLE_OLED_TESTS` | Activé si câblage OLED détecté | Lance balayage de pixels, contraste et récupération I2C. |
| Animateur NeoPixel | `ENABLE_NEOPIXEL_TESTS` | Activé | Exécute roue chromatique et stress test à 800 kHz. |
| **Battement Wi-Fi** | Aucun (toujours activé) | Activé | État Wi-Fi en temps réel sur LED NeoPixel. Nouveau en v3.21.1. |
| Reporter MQTT | `ENABLE_MQTT_BRIDGE` | Désactivé | Publie des résumés ; nécessite les identifiants broker dans `config.h`. |
| Automatisation REST | `ENABLE_REST_API` | Activé | Fournit des endpoints JSON pour CI/CD ou outils flotte. |
| Moniteur d'énergie | `ENABLE_POWER_MONITOR` | Désactivé | Lit les capteurs INA219/INA3221 via I2C. |

## Hypothèses environnementales
- Arduino Core ESP32 **3.3.3** (PlatformIO `espressif32@6.6.x` ou Boards Manager 3.1.x+).
- Alimentation USB délivrant au moins 500 mA durant les stress tests OLED/NeoPixel.
- Réseau Wi-Fi stable (2,4 GHz) pour les mesures de débit ; les portails captifs ne sont pas gérés.

## Documents liés
- [Guide de configuration](CONFIG_FR.md) – activation ou désactivation des modules optionnels.
- [Modules de diagnostic détaillés](DIAGNOSTIC_MODULES_FR.md) – ordre d'exécution et métriques collectées.
- [Dépannage](TROUBLESHOOTING_FR.md) – étapes à suivre lorsqu'un module est signalé indisponible.
