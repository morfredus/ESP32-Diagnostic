# Diagnostic ESP32 Complet v3.1.0 - Interface Complète

<div align="center">

[![fr](https://img.shields.io/badge/lang-fr-blue.svg)](README.md)
[![en](https://img.shields.io/badge/lang-en-red.svg)](README.en.md)

![ESP32](https://img.shields.io/badge/ESP32-Compatible-green)
![Version](https://img.shields.io/badge/version-3.1.0-blue)
![Core](https://img.shields.io/badge/Arduino%20Core-3.3.2+-orange)
![License](https://img.shields.io/badge/license-MIT-orange)

**[🇬🇧 Read in English](README.en.md)**

</div>

---

## 📋 Description

Outil de diagnostic exhaustif **multilingue** pour microcontrôleurs ESP32, accessible via interface web moderne. **8 pages complètes fonctionnelles** incluant contrôle LEDs, tests écrans, tests hardware avancés, scanner WiFi, benchmarks performance, et exports multi-formats.

**Nouveau v3.1.0** : Intégration complète de **TOUTES les fonctionnalités v2.5** dans l'architecture moderne v3.0.1 avec **8 pages web totalement fonctionnelles**.

---

## ✨ Fonctionnalités

### 🌐 Interface Complète - 8 Pages

#### 1️⃣ **Vue d'ensemble** 📊
- Informations système temps réel
- État mémoire Heap/PSRAM
- Détails connexion WiFi
- Mise à jour auto toutes les 5 secondes

#### 2️⃣ **LEDs** 💡 ✨ NOUVEAU
**LED Intégrée (GPIO 97)**
- Contrôle On/Off
- Clignotement (5x)
- Test rapide
- Effet fade PWM

**NeoPixel (GPIO 48)**
- Motifs : Rainbow, Pulse, Strobe
- Sélecteur couleur RGB personnalisé
- Contrôle temps réel

#### 3️⃣ **Écrans** 🖥️ ✨ NOUVEAU
**TFT 320x240 SPI**
- Configuration : CS:14, DC:47, RST:21
- Boutons test : Complet, Couleurs, Motif, Effacer
- Prêt connexion matérielle

**OLED 0.96" I2C**
- Auto-détection 0x3C/0x3D
- Pins SDA/SCL configurables
- Test complet + affichage message personnalisé

#### 4️⃣ **Tests Avancés** 🧪 ✨ NOUVEAU
- **Test ADC** - 10 canaux (GPIO 1-10)
- **Test Touch Pads** - 14 pads (ESP32-S3)
- **Test PWM** - 4 canaux, 5kHz
- **Bus SPI** - Détection SPI2, SPI3
- **Partitions Flash** - Liste complète
- **Stress Test Mémoire** - Test intensif RAM

#### 5️⃣ **GPIO** 📌 ✨ NOUVEAU
- Test complet **26 GPIO**
- Vérification Input/Output
- Test HIGH/LOW
- Tableau statut détaillé

#### 6️⃣ **Scanner WiFi** 📶 ✨ NOUVEAU
- Scan réseaux disponibles
- Affichage : SSID, Signal, Canal, Qualité, Sécurité
- Tri par puissance signal
- Évaluation qualité signal

#### 7️⃣ **Performance** ⚡ ✨ NOUVEAU
**Benchmarks :**
- Benchmark CPU (1M opérations, MFLOPS)
- Benchmark Mémoire (10KB, MB/s)
- Benchmark Flash (vitesse lecture/écriture)

#### 8️⃣ **Export** 💾 (Amélioré)
- **Format TXT** ✨ NOUVEAU - Rapport texte structuré
- **Format JSON** - Données structurées
- **Format CSV** - Compatible Excel
- **PDF Imprimable** ✨ NOUVEAU - Fonction print navigateur

### 🌐 Interface Multilingue
- **Français** (langue par défaut)
- **Anglais**
- Changement dynamique boutons FR/EN
- Traductions complètes tous éléments UI

### 📡 Connectivité Multi-Réseau
- **Support Multi-WiFi** - Plusieurs réseaux WiFi
- **Basculement Automatique** - Essaie chaque réseau
- **Flexible** - Maison, bureau, déplacement
- **Mode AP Fallback** - Point d'accès si échec

### 🎨 Design Moderne
- **Glassmorphism** avec transparence et flou
- **Dégradés animés** violet/bleu
- **Animations fluides** sur tous éléments
- **Interface responsive** mobile/tablette/desktop

---

## 🚀 Démarrage Rapide

### Prérequis
- **ESP32** (ESP32-S2, ESP32-S3, ou ESP32-C3)
- **Arduino IDE 2.x** ou PlatformIO
- **Arduino Core ESP32 v3.3.2+** ⚡ (minimum requis)
- Navigateur web moderne

### Installation

1. **Installer ESP32 Board Manager v3.3.2+**
2. **Installer bibliothèques** (Adafruit NeoPixel, ArduinoJson 6.x)
3. **Créer config.h** depuis exemple-config.h
4. **Configurer WiFi** (Multi-WiFi recommandé)
5. **Compiler et téléverser**
6. **Accéder** : http://esp32-diagnostic.local

📖 **[Guide d'installation complet](INSTALLATION.md)**

---

## 📊 Comparaison Versions

| Fonctionnalité | v2.5 | v3.0.1 | v3.1.0 |
|----------------|------|--------|--------|
| Pages interface | 8 | 3 | **8** ✅ |
| LED intégrée | ✅ | ❌ | **✅** |
| NeoPixel | ✅ | ✅ | **✅** |
| Écrans TFT/OLED | ✅ | ❌ | **✅** |
| Tests ADC/Touch/PWM | ✅ | ❌ | **✅** |
| Scanner WiFi | ✅ | ❌ | **✅** |
| Benchmarks | ✅ | ❌ | **✅** |
| Export TXT | ✅ | ❌ | **✅** |
| Multi-WiFi | ❌ | ✅ | **✅** |
| Arduino Core | 3.1.3 | 3.3.2 | **3.3.2** |
| Design moderne | ⚠️ | ✅ | **✅** |
| Traductions FR/EN | ⚠️ | ✅ | **✅** |

**Résultat :** v3.1.0 = **TOUTES** fonctionnalités v2.5 + améliorations v3.0.1 🎉

---

## 🆕 Nouveautés v3.1.0

### Interface Complète - 8 Pages
- ✨ **5 nouvelles pages** ajoutées
- ✨ Contrôle LED intégrée (GPIO 97)
- ✨ Contrôle NeoPixel RGB (GPIO 48)
- ✨ Tests écrans TFT/OLED
- ✨ Test ADC (10 canaux)
- ✨ Test Touch Pads (14 pads)
- ✨ Test PWM (4 canaux)
- ✨ Scanner réseaux WiFi
- ✨ Benchmarks performance
- ✨ Format export TXT ajouté

---

## 🌐 Endpoints API

### Système
- `GET /api/system` - Informations système
- `GET /api/tests` - Tous tests hardware
- `GET /api/language` - Langue actuelle
- `POST /api/language` - Changer langue

### LEDs (NOUVEAU v3.1.0)
- `POST /api/led/builtin` - Contrôle LED intégrée
- `POST /api/neopixel/pattern` - Motif NeoPixel
- `POST /api/neopixel/color` - Couleur RGB personnalisée

### Tests Avancés (NOUVEAU v3.1.0)
- `GET /api/test/gpio` - Test GPIO complet
- `GET /api/test/adc` - Test ADC 10 canaux
- `GET /api/test/touch` - Test Touch Pads
- `GET /api/test/pwm` - Test PWM

### Scanner & Benchmark (NOUVEAU v3.1.0)
- `GET /api/scan/wifi` - Scanner réseaux
- `GET /api/benchmark` - Benchmarks performance

### Export
- `GET /api/export/txt` - Export texte (NOUVEAU)
- `GET /api/export/json` - Export JSON
- `GET /api/export/csv` - Export CSV

---

## 🔧 Configuration

### Pins GPIO
```cpp
#define BUILTIN_LED_PIN 97    // LED intégrée
#define NEOPIXEL_PIN 48       // NeoPixel
#define TFT_CS 14             // TFT
#define OLED_SDA 21           // OLED
```

### Intervalle Mise à Jour
```cpp
const unsigned long UPDATE_INTERVAL = 5000; // 5 secondes
```

---

## 🛠️ Support Matériel

### Testé & Validé
- ✅ ESP32-S3 (8MB Flash, OPI PSRAM) - Excellent
- ✅ ESP32-S2 (4MB Flash) - Bon

### Compatible Non Testé
- ⚠️ ESP32-C3, C6, H2

### Configuration Mémoire Recommandée
- **Minimum** : Flash 4MB, Heap libre 100KB+
- **Recommandé** : Flash 8MB, PSRAM 8MB (OPI)

---

## 📖 Documentation

- **[Guide d'Installation](INSTALLATION.md)** - Guide complet pas à pas
- **[Changelog](CHANGELOG.md)** - Historique versions
- **[Structure Projet](PROJECT_STRUCTURE.md)** - Vue architecture
- **[🇬🇧 English Installation](INSTALLATION.en.md)**
- **[🇬🇧 English Changelog](CHANGELOG.en.md)**
- **[🇬🇧 English Structure](PROJECT_STRUCTURE.en.md)**

---

## 🔍 Dépannage

### ESP32 ne se connecte pas au WiFi
✅ Vérifier identifiants dans `config.h`  
✅ WiFi 2.4GHz uniquement (pas 5GHz)  
✅ Mode AP activé automatiquement en échec  

### Interface ne charge pas
✅ Vider cache navigateur (Ctrl+Shift+R)  
✅ Vérifier adresse IP dans Moniteur Série  
✅ Essayer http://192.168.x.x au lieu de .local  

### LEDs ne fonctionnent pas
✅ Vérifier configuration GPIO  
✅ NeoPixel nécessite alimentation suffisante  
✅ LED intégrée sur GPIO 97 (ESP32-S3)  

---

## 🤝 Contribution

Les contributions sont les bienvenues ! N'hésitez pas à soumettre une Pull Request.

---

## 📄 Licence

MIT License - Utilisation libre

---

## 🗺️ Roadmap

### v3.2.0 (Prévu - T1 2026)
- Graphiques temps réel Chart.js
- Historique métriques
- Alertes configurables
- Support capteurs I2C

### v4.0.0 (Futur - T3 2026)
- Refonte React
- App mobile
- Cloud sync optionnel

---

## 📊 Statistiques Projet

**Version :** 3.1.0  
**Date :** 16 Octobre 2025  
**Lignes Code :** ~4500  
**Fichiers :** 6  
**Pages Interface :** 8  
**Endpoints API :** 18  
**Langues :** 2 (FR/EN)  

---

## ✅ Checklist Installation

- [ ] Arduino Core ESP32 3.3.2+ installé
- [ ] Bibliothèques installées
- [ ] config.h créé et configuré
- [ ] Compilation sans erreur
- [ ] Interface accessible
- [ ] 8 onglets fonctionnels

---

<div align="center">

**Version actuelle :** 3.1.0  
**Arduino Core :** 3.3.2+  
**Mise à jour :** 16 Octobre 2025  

🌐 **Accès :** http://esp32-diagnostic.local  
🇫🇷🇬🇧 **Langues :** Boutons FR/EN dans interface

---

Fait avec ❤️ pour la communauté ESP32

**[🇬🇧 Read in English](README.en.md)**

</div>