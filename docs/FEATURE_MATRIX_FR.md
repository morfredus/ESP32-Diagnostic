# ESP32 Diagnostic Suite – Matrice des fonctionnalités (v3.12.0)

Cette matrice résume la couverture des diagnostics pour les cartes Espressif les plus courantes prises en charge par la version
3.8.0. Elle aide à planifier les campagnes de validation et à vérifier si des périphériques optionnels nécessitent un câblage
supplémentaire.

> **Important :** La version 3.9.0 (PlatformIO) **ne supporte pas** la fonctionnalité Bluetooth/BLE. Pour les diagnostics BLE, utiliser la version Arduino IDE archivée [ESP32-Diagnostic-Arduino-IDE](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE).

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
| Bus / Périphérique | Broches par défaut | Cartes supportées | Notes |
|--------------------|--------------------|-------------------|-------|
| I2C principal | SDA 21, SCL 20 (reconfigurable) | Toutes les cartes supportées | Utilisé pour l'OLED, les packs capteurs, l'EEPROM. |
| I2C secondaire | Désactivé par défaut | ESP32, ESP32-S3 | Activer via le flag `ENABLE_SECONDARY_I2C` dans `config.h`. |
| Bus SPI test | MOSI 23, MISO 19, CLK 18, CS 5 | ESP32, ESP32-S3 | Valide la flash externe / adaptateurs SD. |
| Boucle UART | TX0/RX0 & UART1 optionnel | Toutes | Nécessite un cavalier TX↔RX sur l'UART choisi. |
| Sonde CAN (TWAI) | GPIO 4 / 5 | ESP32, ESP32-S3 | Demande un transceiver externe (ex. SN65HVD230). |
| Température OneWire | GPIO 15 | ESP32, ESP32-S3 | Détection DS18B20 lorsque `ENABLE_ONEWIRE` est actif. |

## Modules optionnels
| Module | Flag firmware | État par défaut | Description |
|--------|---------------|-----------------|-------------|
| Diagnostics OLED | `ENABLE_OLED_TESTS` | Activé si câblage OLED détecté | Lance balayage de pixels, contraste et récupération I2C. |
| Animateur NeoPixel | `ENABLE_NEOPIXEL_TESTS` | Activé | Exécute roue chromatique et stress test à 800 kHz. |
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
