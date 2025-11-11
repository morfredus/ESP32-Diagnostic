# Guide d'installation (FR)

Suivez ces étapes pour déployer ESP32 Diagnostic Suite sur votre carte.

> **Version 3.9.0 :** Cette version a été migrée depuis Arduino IDE vers **PlatformIO dans Visual Studio Code**. La version originale Arduino IDE [ESP32-Diagnostic-Arduino-IDE](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE) est maintenant **archivée**. Le support Bluetooth/BLE a été **supprimé** car il n'est pas correctement géré sous la configuration de plateforme PlatformIO.

## 1. Prérequis
- Carte ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6 ou ESP32-H2 (S3 recommandée).
- Câble USB avec transfert de données.
- Optionnel : bandeau NeoPixel/WS2812B et écran OLED 0,96" SSD1306 pour les tests avancés.
- **Visual Studio Code** avec l'extension **PlatformIO IDE** installée.
- PlatformIO gèrera automatiquement la toolchain ESP32 (plateforme espressif32).

## 2. Dépendances des bibliothèques
Toutes les bibliothèques requises sont déclarées dans `platformio.ini` et seront installées automatiquement par PlatformIO :
- **Adafruit BusIO**
- **Adafruit GFX Library**
- **Adafruit SSD1306**
- **Adafruit NeoPixel**

Les bibliothèques suivantes sont fournies avec la plateforme ESP32 : `WiFi`, `WebServer`, `ESPmDNS`, `Wire`, `SPI`, `esp_chip_info`, `esp_flash`, `esp_heap_caps`, `esp_partition`.

## 3. Récupérer le code source
1. Clonez le dépôt depuis GitHub :
   ```bash
   git clone https://github.com/morfredus/ESP32-Diagnostic.git
   cd ESP32-Diagnostic
   ```
2. Ouvrez le dossier du projet dans Visual Studio Code avec PlatformIO.

**Note :** La version originale Arduino IDE [ESP32-Diagnostic-Arduino-IDE](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE) est maintenant archivée.

## 4. Préparer les identifiants Wi-Fi
1. Dupliquez `wifi-config-example.h` puis renommez la copie en `wifi-config.h`.
2. Ouvrez `wifi-config.h` et remplacez les paires SSID/MOT_DE_PASSE par vos réseaux.
3. Vous pouvez déclarer plusieurs réseaux ; le firmware les testera dans l'ordre.

Les précisions de configuration sont détaillées dans [CONFIG_FR.md](CONFIG_FR.md).

## 5. Compilation et téléversement
1. Dans Visual Studio Code, ouvrez la barre latérale PlatformIO.
2. Sélectionnez votre environnement cible depuis `platformio.ini` (ex. `esp32-s3-devkit`).
3. Assurez-vous que la **PSRAM** est activée dans votre configuration de carte pour les ESP32-S3 afin de profiter de tous les diagnostics.
4. Cliquez sur **Build** pour compiler, puis sur **Upload** pour flasher le firmware.
5. Ouvrez le **Serial Monitor** (115200 bauds) pour visualiser la séquence de démarrage.

En cas de succès, le moniteur série affiche la version du firmware et l'état de connexion. L'OLED (si présent) reflète les phases de connexion Wi-Fi (association, DHCP, réussite).

**Note :** La fonctionnalité Bluetooth/BLE n'est pas disponible dans cette version PlatformIO.
