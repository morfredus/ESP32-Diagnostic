# 📟 ESP32 Diagnostic System v3.2.0

[🇬🇧 Read in English](README.en.md) | [📦 Installation](INSTALLATION.md) | [📋 Changelog](CHANGELOG.md) | [🏗️ Structure](PROJECT_STRUCTURE.md)

---

<div align="center">

![Version](https://img.shields.io/badge/version-3.2.0-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Arduino](https://img.shields.io/badge/Arduino-Compatible-cyan.svg)
![ESP32](https://img.shields.io/badge/ESP32-All%20Models-orange.svg)

**Système de diagnostic complet pour ESP32 avec interface web moderne et multilingue**

[Démarrage rapide](#-démarrage-rapide) • [Fonctionnalités](#-fonctionnalités) • [Documentation](#-documentation) • [Support](#-support)

</div>

---

## 🎉 Nouveautés v3.2.0

- ✨ **WiFi Multi-SSID** : Connexion automatique à plusieurs réseaux WiFi
- 🔧 **Configuration séparée** : Fichiers wifi-config.h et config.h
- 🔒 **Sécurité renforcée** : Fichiers sensibles exclus de Git
- 📌 **Pins configurables** : Configuration matérielle centralisée
- 🔄 **Reconnexion auto** : Maintien de la connexion WiFi

---

## 📋 Table des matières

- [Vue d'ensemble](#-vue-densemble)
- [Fonctionnalités](#-fonctionnalités)
- [Démarrage rapide](#-démarrage-rapide)
- [Configuration](#-configuration)
- [Interface Web](#-interface-web)
- [API REST](#-api-rest)
- [Matériel Compatible](#-matériel-compatible)
- [Documentation](#-documentation)
- [Support](#-support)

---

## 🌟 Vue d'ensemble

ESP32 Diagnostic System est un outil professionnel de diagnostic et de test pour cartes ESP32. Il offre :

- 🖥️ **Interface web moderne** responsive et intuitive
- 🌍 **Support multilingue** français/anglais
- 🧪 **Tests hardware complets** GPIO, PWM, I2C, WiFi, Mémoire
- 📊 **Monitoring en temps réel** système et capteurs
- 📡 **WiFi Multi-SSID** connexion automatique intelligente
- 💾 **Export de données** JSON et CSV
- ⚡ **Benchmarks** performance CPU et mémoire
- 🎨 **Support NeoPixel** indicateurs visuels LED RGB

---

## ✨ Fonctionnalités

### 🌐 Connectivité

- **WiFi Multi-SSID** avec WiFiMulti
  - Support de plusieurs réseaux (illimité)
  - Connexion automatique au réseau le plus fort
  - Ordre de priorité configurable
  - Reconnexion automatique en cas de perte
- **Mode Point d'Accès** failover automatique
- **mDNS** accès simplifié (esp32-diag.local)
- **CORS** développement cross-origin

### 🧪 Tests Hardware

- **GPIO** : Test de toutes les pins configurables
- **PWM** : Test des sorties PWM (API ESP32 Core 3.x)
- **I2C** : Scan des périphériques I2C
- **ADC** : Test des entrées analogiques
- **Mémoire** : Analyse Heap et PSRAM
- **WiFi** : Scanner de réseaux avec détails
- **Capteurs** : Support température interne

### 📊 Interface Web

- **8 pages thématiques** :
  - 📊 Dashboard : Vue d'ensemble système
  - 🧪 Tests : Tous les tests hardware
  - 💾 Mémoire : Analyse détaillée RAM/PSRAM
  - 📡 WiFi : Informations et scan réseaux
  - 🔌 GPIO : Configuration et tests pins
  - ⚡ Benchmarks : Tests de performance
  - 💾 Export : Exportation JSON/CSV
  - ℹ️ À propos : Informations projet

- **Design moderne** :
  - Mode sombre avec dégradés
  - Responsive mobile-friendly
  - Animations fluides
  - Auto-refresh 5 secondes

### 🔧 Configuration

- **Fichiers séparés sécurisés** :
  - `wifi-config.h` : Identifiants WiFi multi-SSID
  - `config.h` : Configuration pins matérielles
  - Exclusion automatique de Git
- **Configuration optionnelle** :
  - NeoPixel WS2812B
  - Écran OLED I2C
  - Capteurs température (DHT22, DS18B20, BME280)
  - Buzzer et boutons
- **Pins configurables** :
  - Arrays GPIO/PWM/ADC personnalisables
  - Support tous modèles ESP32

### 📡 API REST

18 endpoints complets :

**Système**
- `GET /api/system/info` - Informations système complètes
- `GET /api/system/memory` - Détails mémoire Heap/PSRAM
- `GET /api/system/wifi` - Informations WiFi détaillées

**Tests**
- `GET /api/tests` - Tous les tests hardware
- `GET /api/tests/gpio` - Test GPIO uniquement
- `GET /api/tests/pwm` - Test PWM uniquement
- `POST /api/tests/run` - Exécuter test spécifique

**Configuration**
- `GET /api/language` - Langue actuelle
- `POST /api/language` - Changer la langue

**Export**
- `GET /api/export/json` - Exporter en JSON
- `GET /api/export/csv` - Exporter en CSV

**NeoPixel**
- `POST /api/neopixel/pattern` - Contrôle patterns
- `POST /api/neopixel/color` - Contrôle couleur RGB

**Avancé**
- `GET /api/sensors` - Lecture capteurs
- `POST /api/wifi/scan` - Scanner réseaux WiFi
- `GET /api/benchmark` - Tests performance

---

## 🚀 Démarrage rapide

### Installation en 5 minutes

```bash
# 1. Cloner le projet
git clone https://github.com/votre-username/ESP32-Diagnostic.git
cd ESP32-Diagnostic

# 2. Configurer WiFi
cp exemple-wifi-config.h wifi-config.h
# Éditer wifi-config.h avec vos identifiants

# 3. Configurer matériel
cp exemple-config.h config.h
# Éditer config.h selon votre carte

# 4. Ouvrir dans Arduino IDE
# Ouvrir ESP32-Diagnostic.ino

# 5. Compiler et téléverser
# Sélectionner votre carte ESP32
# Cliquer sur Upload
```

### Premier lancement

1. Ouvrir le **Moniteur Série** (115200 bauds)
2. Noter l'**adresse IP** affichée
3. Ouvrir un navigateur : `http://ADRESSE-IP`
4. Profiter de l'interface ! 🎉

---

## ⚙️ Configuration

### Configuration WiFi Multi-SSID

Éditer `wifi-config.h` :

```cpp
const WiFiCredentials wifiNetworks[] = {
  {"WiFi-Maison", "motdepasse-maison"},
  {"WiFi-Bureau", "motdepasse-bureau"},
  {"Hotspot-Mobile", "motdepasse-mobile"},
};
```

### Configuration Matérielle

Éditer `config.h` :

```cpp
// LED interne
#define LED_BUILTIN_PIN 2

// NeoPixel (optionnel)
#define USE_NEOPIXEL
#define NEOPIXEL_PIN 48
#define NEOPIXEL_COUNT 1

// OLED (optionnel)
#define USE_OLED
#define OLED_SDA_PIN 21
#define OLED_SCL_PIN 22

// GPIO à tester
const int TEST_GPIO_PINS[] = {1, 2, 3, 4, 5};
```

---

## 💻 Matériel Compatible

### Cartes ESP32

| Modèle | Cores | PSRAM | Bluetooth | Status |
|--------|-------|-------|-----------|--------|
| ESP32 | 2 | ❌ | ✅ | ✅ Testé |
| ESP32-S2 | 1 | ❌ | ❌ | ✅ Testé |
| ESP32-S3 | 2 | ✅ 8MB | ✅ | ✅ Recommandé |
| ESP32-C3 | 1 | ❌ | ✅ | ✅ Testé |
| ESP32-C6 | 1 | ❌ | ✅ | ⚠️ Non testé |
| ESP32-H2 | 1 | ❌ | ✅ | ⚠️ Non testé |

### Composants Optionnels

- 💡 **NeoPixel WS2812B** : Indicateur LED RGB
- 🖥️ **OLED 128x64** : Affichage local (I2C)
- 🌡️ **Capteurs** : DHT22, DS18B20, BME280
- 🔊 **Buzzer** : Alertes sonores
- 🔘 **Boutons** : Contrôles physiques

---

## 📚 Documentation

- 📦 [Guide d'Installation](INSTALLATION.md) - Installation complète pas à pas
- 🇬🇧 [Installation Guide (EN)](INSTALLATION.en.md) - English version
- 📋 [Changelog](CHANGELOG.md) - Historique des versions
- 🏗️ [Structure du Projet](PROJECT_STRUCTURE.md) - Architecture détaillée
- 🌐 [README English](README.en.md) - English documentation

---

## 🤝 Support

### Problèmes courants

**WiFi ne se connecte pas** :
- Vérifier SSID/mot de passe dans `wifi-config.h`
- ESP32 supporte uniquement 2.4GHz (pas de 5GHz)
- Vérifier que le réseau n'a pas de filtrage MAC

**Erreur de compilation** :
- Installer ESP32 Board Package 3.1.3+
- Installer ArduinoJson 6.x (PAS v7)
- Vérifier que tous les fichiers sont présents

**Interface web ne charge pas** :
- Vérifier Partition Scheme inclut SPIFFS
- Vider le cache navigateur (Ctrl+F5)
- Essayer un autre navigateur

### Aide et Contributions

- 🐛 [Signaler un bug](https://github.com/votre-username/ESP32-Diagnostic/issues)
- 💡 [Proposer une fonctionnalité](https://github.com/votre-username/ESP32-Diagnostic/issues)
- 🤝 [Contribuer](CONTRIBUTING.md)

---

## 📄 Licence

Ce projet est sous licence MIT. Voir le fichier [LICENSE](LICENSE) pour plus de détails.

---

## 🙏 Remerciements

- Espressif pour le formidable SDK ESP32
- Communauté Arduino pour les bibliothèques
- Tous les contributeurs du projet

---

<div align="center">

**Développé avec ❤️ pour la communauté ESP32**

⭐ Si ce projet vous est utile, n'hésitez pas à lui donner une étoile !

[⬆ Retour en haut](#-esp32-diagnostic-system-v320)

</div># 📟 ESP32 Diagnostic System v3.1.1

[🇬🇧 Read in English](README.en.md) | [📦 Installation](INSTALLATION.md) | [📋 Changelog](CHANGELOG.md) | [🏗️ Structure](PROJECT_STRUCTURE.md)

---