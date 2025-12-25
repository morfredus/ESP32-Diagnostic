# Guide d'installation (FR)

> **AVERTISSEMENT** : v3.28.5 corrige des doublons de mapping pour ESP32 Classic et conserve le remapping GPIO pour ESP32-S3. Assurez-vous que votre câblage et la cible compilée correspondent aux broches documentées. Lisez [docs/PIN_MAPPING_FR.md](docs/PIN_MAPPING_FR.md) et [docs/PIN_MAPPING.md](docs/PIN_MAPPING.md) avant de flasher.

Suivez ces étapes pour déployer ESP32 Diagnostic Suite sur votre carte.

> **Version 3.12.0 :** Cette version a été migrée depuis Arduino IDE vers **PlatformIO dans Visual Studio Code**. La version originale Arduino IDE [ESP32-Diagnostic-Arduino-IDE](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE) est maintenant **archivée**. Le support Bluetooth/BLE a été **supprimé** car il n'est pas correctement géré sous la configuration de plateforme PlatformIO.

## 1. Prérequis

### Matériel
- **Cartes supportées (v3.21.0) :**
  - **ESP32-S3-DevKitC-1 N16R8** (16Mo Flash, 8Mo PSRAM) — Cible principale, environnement : `esp32s3_n16r8`
  - **ESP32-S3-DevKitC-1 N8R8** (8Mo Flash, 8Mo PSRAM) — Environnement : `esp32s3_n8r8`
  - **ESP32-DevKitC Classic** (4Mo Flash, sans PSRAM) — Environnement : `esp32devkitc`
- Câble USB avec transfert de données.
- Optionnel : bandeau NeoPixel/WS2812B, écran OLED 0,96", écran TFT ST7789, capteurs (DHT, HC-SR04, etc.).

### Logiciel
- **Visual Studio Code** avec l'extension **PlatformIO IDE** installée.
- PlatformIO gèrera automatiquement la toolchain ESP32 (plateforme espressif32 ^6).
- Git pour cloner le dépôt.

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
1. Dupliquez `secrets-example.h` puis renommez la copie en `secrets.h`.
2. Ouvrez `secrets.h` et remplacez les paires SSID/MOT_DE_PASSE par vos réseaux.
3. Vous pouvez déclarer plusieurs réseaux ; le firmware les testera dans l'ordre.

Les précisions de configuration sont détaillées dans [CONFIG_FR.md](CONFIG_FR.md).

## 5. Compilation et téléversement
1. Dans Visual Studio Code, ouvrez la barre latérale PlatformIO.
2. Sélectionnez votre environnement cible depuis `platformio.ini` :
   - **esp32s3_n16r8** (défaut) — ESP32-S3 avec 16Mo Flash, 8Mo PSRAM
   - **esp32s3_n8r8** — ESP32-S3 avec 8Mo Flash, 8Mo PSRAM
   - **esp32devkitc** — ESP32 classique avec 4Mo Flash, sans PSRAM
3. Cliquez sur **Build** pour compiler, puis sur **Upload** pour flasher le firmware.
4. Ouvrez le **Serial Monitor** (115200 bauds) pour visualiser la séquence de démarrage.

En cas de succès, le moniteur série affiche la version du firmware et l'état de connexion. L'OLED (si présent) reflète les phases de connexion Wi-Fi (association, DHCP, réussite).

**Note :** La fonctionnalité Bluetooth/BLE n'est pas disponible dans cette version PlatformIO.
