# 📁 STRUCTURE DU PROJET v3.1.0 - Vue d'Ensemble Complète

**[🇬🇧 English Version](PROJECT_STRUCTURE.en.md)**

---

## 🗂️ Arborescence du Projet

```
ESP32-Diagnostic/
│
├── 📄 ESP32-Diagnostic.ino          # Fichier principal Arduino
├── 📄 exemple-config.h               # Template configuration (GIT)
├── 📄 config.h                       # Votre configuration (GITIGNORE)
├── 📄 api_handlers.h                 # Gestionnaires API REST
├── 📄 web_interface.h                # Interface web HTML/CSS/JS
├── 📄 test_functions.h               # Fonctions de tests hardware
├── 📄 translations.h                 # Système de traductions
│
├── 📄 .gitignore                     # Exclusions Git
├── 📄 README.md                      # Documentation principale (Français)
├── 📄 README.en.md                   # Documentation principale (English)
├── 📄 CHANGELOG.md                   # Historique versions (Français)
├── 📄 CHANGELOG.en.md                # Historique versions (English)
├── 📄 INSTALLATION.md                # Guide installation (Français)
├── 📄 INSTALLATION.en.md             # Guide installation (English)
├── 📄 PROJECT_STRUCTURE.md           # Ce fichier (Français)
├── 📄 PROJECT_STRUCTURE.en.md        # Ce fichier (English)
└── 📄 LICENSE                        # Licence MIT
```

---

## 📋 Description des Fichiers

### 1. ESP32-Diagnostic.ino (Principal)

**Version :** 3.1.0  
**Taille :** ~300 lignes  
**Rôle :** Point d'entrée du programme

**Contenu :**
```cpp
✅ Includes bibliothèques (WiFi, WebServer, Wire, etc.)
✅ Définitions constantes (VERSION, GPIO pins)
✅ Include config.h (identifiants WiFi)
✅ Objets globaux (server, wifiMulti, strip)
✅ Fonctions système (getChipModel, getMemoryInfo, etc.)
✅ Includes modules (translations, test_functions, api_handlers, web_interface)
✅ Initialisation hardware (LED, NeoPixel)
✅ Connexion WiFi (Multi-WiFi support)
✅ Configuration serveur web (setupRoutes)
✅ setup() - Initialisation complète
✅ loop() - Boucle principale
```

**Dépendances :**
- Arduino Core ESP32 3.3.2+
- WiFi.h, WiFiMulti.h, WebServer.h
- ESPmDNS.h, Wire.h (I2C)
- Adafruit_NeoPixel.h
- esp_heap_caps.h, esp_system.h, esp_chip_info.h
- driver/adc.h

**Inclut :**
- config.h
- translations.h
- test_functions.h
- api_handlers.h
- web_interface.h

---

### 2. exemple-config.h (Template)

**Version :** 3.1.0  
**Taille :** ~200 lignes  
**Rôle :** Template de configuration WiFi

**Contenu :**
```cpp
✅ Documentation complète
✅ Instructions installation
✅ Mode Multi-WiFi (recommandé)
✅ Mode Single WiFi (simple)
✅ Configuration IP statique (optionnel)
✅ Paramètres WiFi avancés
✅ Configuration AP mode (fallback)
✅ Exemples commentés
```

**Caractéristiques :**
- ✅ Sans identifiants réels (safe pour Git)
- ✅ Commentaires en français
- ✅ Exemples multiples réseaux
- ✅ Guide pas à pas

**Usage :**
```bash
# Copier et renommer
cp exemple-config.h config.h

# Éditer config.h avec vos identifiants
nano config.h  # ou votre éditeur
```

---

### 3. config.h (Votre Configuration)

**Version :** Créé par utilisateur  
**Taille :** ~200 lignes  
**Rôle :** Configuration WiFi personnelle

**Contenu :**
```cpp
✅ Vos identifiants WiFi
✅ Configuration Multi-WiFi ou Single
✅ Paramètres personnalisés
✅ IP statique (optionnel)
```

**⚠️ IMPORTANT :**
- ❌ NE JAMAIS commiter sur Git
- ❌ NE JAMAIS partager
- ✅ Toujours dans .gitignore
- ✅ Garder backup sécurisé

**Format Multi-WiFi :**
```cpp
#define MULTI_WIFI_ENABLED
#define NUM_SSIDS 2

const char* ssid_list[] = {"WiFi1", "WiFi2"};
const char* password_list[] = {"Pass1", "Pass2"};
```

---

### 4. api_handlers.h (API REST)

**Version :** 3.1.0  
**Taille :** ~800 lignes  
**Rôle :** Tous les endpoints API

**Endpoints Implémentés :**

#### Système (v3.0.1)
- `handleSystemInfo()` → GET /api/system
- `handleTests()` → GET /api/tests
- `handleGetLanguage()` → GET /api/language
- `handleSetLanguage()` → POST /api/language

#### LEDs (v3.1.0 NOUVEAU)
- `handleBuiltinLED()` → POST /api/led/builtin
  - Actions : on, off, blink, test, fade
- `handleNeoPixelPattern()` → POST /api/neopixel/pattern
  - Motifs : rainbow, pulse, strobe, off
- `handleNeoPixelColor()` → POST /api/neopixel/color
  - RGB custom : {r, g, b}

#### Tests Avancés (v3.1.0 NOUVEAU)
- `handleTestGPIO()` → GET /api/test/gpio
  - Test 26 GPIO complet
- `handleTestADC()` → GET /api/test/adc
  - Lecture 10 canaux ADC
- `handleTestTouch()` → GET /api/test/touch
  - Test 14 touch pads
- `handleTestPWM()` → GET /api/test/pwm
  - Test 4 canaux PWM

#### Scanner & Benchmark (v3.1.0 NOUVEAU)
- `handleWiFiScan()` → GET /api/scan/wifi
  - Scan réseaux disponibles
- `handleBenchmark()` → GET /api/benchmark
  - Benchmarks CPU/Mémoire/Flash

#### Export
- `handleExportJSON()` → GET /api/export/json
- `handleExportCSV()` → GET /api/export/csv
- `handleExportTXT()` → GET /api/export/txt (v3.1.0 NOUVEAU)

**Dépendances :**
- ArduinoJson (v6.x)
- Fonctions de ESP32-Diagnostic.ino
- Objets globaux (server, strip, etc.)

---

### 5. web_interface.h (Interface Web)

**Version :** 3.1.0  
**Taille :** ~1200 lignes  
**Rôle :** Interface HTML/CSS/JavaScript complète

**Contenu :**

#### Structure HTML
```html
✅ DOCTYPE HTML5
✅ Meta viewport responsive
✅ Title avec version
✅ Styles CSS inline
✅ Container glassmorphism
✅ Header avec langue
✅ 8 onglets navigation
✅ 8 sections contenu
✅ JavaScript complet
```

#### Styles CSS
```css
✅ Reset CSS
✅ Gradient animé background
✅ Glassmorphism cards
✅ Responsive design
✅ Animations fluides
✅ Hover effects
✅ Loading spinners
✅ Progress bars
✅ Boutons colorés
✅ Tableaux stylisés
✅ Media queries mobile
```

#### Fonctions JavaScript
```javascript
✅ setLanguage(lang) - Changement langue
✅ showTab(tabName) - Navigation onglets
✅ loadData() - Chargement données système
✅ controlBuiltinLED(action) - Contrôle LED
✅ neoPattern(pattern) - Patterns NeoPixel
✅ neoColor() - Couleur custom NeoPixel
✅ testADC() - Test ADC
✅ testTouch() - Test Touch
✅ testPWM() - Test PWM
✅ testAllGPIO() - Test GPIO complet
✅ scanWiFi() - Scanner WiFi
✅ runBenchmark() - Benchmarks
✅ exportTXT() - Export texte
✅ exportJSON() - Export JSON
✅ exportCSV() - Export CSV
✅ startCountdown() - Countdown auto-update
```

#### Les 8 Pages Implémentées

1. **Overview** - Vue d'ensemble système
2. **LEDs** - Contrôle LED + NeoPixel
3. **Screens** - Configuration écrans TFT/OLED
4. **Advanced** - Tests ADC/Touch/PWM/SPI/Flash/Stress
5. **GPIO** - Test complet 26 GPIO
6. **WiFi** - Scanner réseaux
7. **Performance** - Benchmarks
8. **Export** - 4 formats export

**Caractéristiques :**
- 📱 Responsive (mobile/tablette/desktop)
- 🌐 Multilingue (FR/EN)
- 🎨 Design moderne glassmorphism
- ⚡ Mise à jour temps réel
- 🔄 Animations fluides

---

### 6. test_functions.h (Tests Hardware)

**Version :** 3.0.1  
**Taille :** ~400 lignes  
**Rôle :** Fonctions de tests matériels

**Fonctions Implémentées :**

```cpp
✅ testGPIO(JsonArray& gpioArray)
   - Test I/O sur pins
   - Vérification HIGH/LOW
   
✅ testI2C(JsonArray& i2cArray)
   - Scan adresses 0x00-0x7F
   - Identification périphériques
   
✅ testSPI(JsonArray& spiArray)
   - Détection bus SPI
   - Configuration pins
   
✅ testMemory(JsonObject& memTest)
   - Heap free/size/min
   - Fragmentation
   - PSRAM (si disponible)
   
✅ testWiFi(JsonObject& wifiTest)
   - Statut connexion
   - SSID, IP, RSSI
   - Qualité signal
   - Gateway, DNS, Subnet
   
✅ testSystem(JsonObject& sysTest)
   - Chip model/revision
   - CPU cores/frequency
   - Flash size/speed
   - Uptime
   - Reset reason
   - IDF version
   - Overall status
```

**Optimisations Core 3.3.2 :**
- heap_caps_get_free_size()
- heap_caps_get_minimum_free_size()
- heap_caps_get_largest_free_block()

---

### 7. translations.h (Traductions)

**Version :** 3.0.1  
**Taille :** ~300 lignes  
**Rôle :** Système de traductions FR/EN

**Contenu :**

```cpp
✅ Constantes françaises (TEXT_FR_*)
✅ Constantes anglaises (TEXT_EN_*)
✅ Fonction t() - Helper traduction
✅ Macros T_* - Raccourcis

// Catégories traduites :
- Titres onglets
- Informations système
- Labels mémoire
- Statuts WiFi
- Noms tests
- Boutons export
- Messages statuts
```

**Langues Supportées :**
- 🇫🇷 Français (par défaut)
- 🇬🇧 English

**Usage :**
```cpp
// Dans le code
String titre = t(TEXT_FR_TITLE, TEXT_EN_TITLE);

// Ou avec macro
String titre = T_TITLE;
```

**Extensibilité :**
Facile d'ajouter d'autres langues :
```cpp
const char* TEXT_ES_TITLE = "Diagnóstico ESP32";  // Espagnol
const char* TEXT_DE_TITLE = "ESP32 Diagnose";     // Allemand
```

---

## 🔗 Dépendances entre Fichiers

### Graphe de Dépendances

```
ESP32-Diagnostic.ino (PRINCIPAL)
    │
    ├──> config.h (Configuration WiFi)
    │
    ├──> translations.h (Traductions)
    │
    ├──> test_functions.h (Tests)
    │    └──> ArduinoJson
    │    └──> Wire.h
    │    └──> esp_*.h
    │
    ├──> api_handlers.h (API)
    │    └──> ArduinoJson
    │    └──> test_functions.h
    │    └──> translations.h
    │
    └──> web_interface.h (Interface)
         └──> Embedded HTML/CSS/JS
         └──> api_handlers.h (endpoints)
```

### Ordre d'Inclusion IMPORTANT

⚠️ **L'ordre des includes est critique :**

```cpp
// 1. Bibliothèques système
#include <WiFi.h>
#include <WebServer.h>
...

// 2. Configuration
#include "config.h"

// 3. Modules (ordre précis)
#include "translations.h"      // Doit être avant api_handlers
#include "test_functions.h"    // Doit être avant api_handlers
#include "api_handlers.h"      // Utilise translations et test_functions
#include "web_interface.h"     // Doit être en dernier
```

**Pourquoi cet ordre ?**
- translations.h définit `currentLanguage` et fonctions `t()`
- test_functions.h définit fonctions de test
- api_handlers.h utilise translations et test_functions
- web_interface.h utilise tout ce qui précède

---

## 📦 Bibliothèques Externes Requises

### Bibliothèques Obligatoires

| Bibliothèque | Version | Usage |
|--------------|---------|-------|
| **Arduino Core ESP32** | 3.3.2+ | Core principal |
| **Adafruit NeoPixel** | Dernière | Contrôle NeoPixel |
| **ArduinoJson** | 6.x | Parse/Serialize JSON |

### Bibliothèques Incluses

| Bibliothèque | Depuis | Usage |
|--------------|--------|-------|
| WiFi.h | ESP32 Core | Connexion WiFi |
| WiFiMulti.h | ESP32 Core | Multi-WiFi |
| WebServer.h | ESP32 Core | Serveur HTTP |
| ESPmDNS.h | ESP32 Core | Résolution mDNS |
| Wire.h | ESP32 Core | Communication I2C |
| esp_heap_caps.h | ESP32 Core | Gestion mémoire |
| esp_system.h | ESP32 Core | Info système |
| esp_chip_info.h | ESP32 Core | Info puce |
| driver/adc.h | ESP32 Core | ADC hardware |

---

## ⚙️ Configuration des GPIO

### GPIO Utilisés par Défaut

| GPIO | Usage | Direction | Description |
|------|-------|-----------|-------------|
| 97 | LED intégrée | Output | LED board ESP32-S3 |
| 48 | NeoPixel | Output | WS2812B data pin |
| 14 | TFT CS | Output | TFT Chip Select |
| 47 | TFT DC | Output | TFT Data/Command |
| 21 | TFT RST / OLED SDA | Output/I2C | TFT Reset / I2C Data |
| 20 | OLED SCL | I2C | I2C Clock |
| 1-10 | ADC | Input | Canaux ADC |
| 1-14 | Touch | Input | Touch capacitif |
| 2-5 | PWM | Output | Test PWM |

### GPIO Testés (Test GPIO)

```
1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 
21, 38, 39, 40, 41, 42, 47, 48
```

**Total :** 26 GPIO

**⚠️ GPIO Réservés (ne pas utiliser) :**
- GPIO 19, 20 : USB (ESP32-S3)
- GPIO 26-32 : Flash SPI (ESP32-S3)
- GPIO 33-37 : OPI PSRAM (ESP32-S3)

---

## 🔧 Variables Globales

### Dans ESP32-Diagnostic.ino

```cpp
// Objets principaux
WebServer server(80);              // Serveur HTTP port 80
WiFiMulti wifiMulti;               // Gestionnaire Multi-WiFi
Adafruit_NeoPixel* strip;          // Pointeur NeoPixel
bool neopixelAvailable = false;    // État NeoPixel
bool builtinLedAvailable = true;   // État LED intégrée

// État système
String currentLanguage = "fr";     // Langue actuelle
unsigned long lastUpdateTime = 0;  // Dernier update
const unsigned long UPDATE_INTERVAL = 5000;  // 5 secondes
```

### Constantes Définies

```cpp
#define DIAGNOSTIC_VERSION "3.1.0"
#define ARDUINO_CORE_VERSION "3.3.2"

#define BUILTIN_LED_PIN 97
#define NEOPIXEL_PIN 48
#define NEOPIXEL_COUNT 1

#define TFT_CS 14
#define TFT_DC 47
#define TFT_RST 21
#define OLED_SDA 21
#define OLED_SCL 20
```

---

## 📊 Buffers et Mémoire

### Tailles Buffers JSON

```cpp
// api_handlers.h

StaticJsonDocument<2048> doc;      // handleSystemInfo
DynamicJsonDocument doc(8192);     // handleTests
StaticJsonDocument<128> doc;       // handleGetLanguage
StaticJsonDocument<256> doc;       // handleNeoPixelPattern
DynamicJsonDocument doc(4096);     // handleTestGPIO
StaticJsonDocument<512> doc;       // handleTestADC/Touch/PWM
DynamicJsonDocument doc(4096);     // handleWiFiScan
StaticJsonDocument<512> doc;       // handleBenchmark
DynamicJsonDocument doc(8192);     // handleExportJSON
```

**Optimisation Mémoire :**
- Static pour tailles fixes < 1KB
- Dynamic pour tailles variables ou > 1KB
- Buffers ajustés selon contenu réel

---

## 🌐 Endpoints API Complets

### Tableau Récapitulatif

| Méthode | Endpoint | Fonction | Ajouté |
|---------|----------|----------|--------|
| GET | `/` | handleRoot | v3.0.0 |
| GET | `/api/system` | handleSystemInfo | v3.0.0 |
| GET | `/api/tests` | handleTests | v3.0.0 |
| GET | `/api/language` | handleGetLanguage | v3.0.0 |
| POST | `/api/language` | handleSetLanguage | v3.0.0 |
| POST | `/api/led/builtin` | handleBuiltinLED | **v3.1.0** |
| POST | `/api/neopixel/pattern` | handleNeoPixelPattern | v3.0.1 |
| POST | `/api/neopixel/color` | handleNeoPixelColor | v3.0.1 |
| GET | `/api/test/gpio` | handleTestGPIO | **v3.1.0** |
| GET | `/api/test/adc` | handleTestADC | **v3.1.0** |
| GET | `/api/test/touch` | handleTestTouch | **v3.1.0** |
| GET | `/api/test/pwm` | handleTestPWM | **v3.1.0** |
| GET | `/api/scan/wifi` | handleWiFiScan | **v3.1.0** |
| GET | `/api/benchmark` | handleBenchmark | **v3.1.0** |
| GET | `/api/export/json` | handleExportJSON | v3.0.0 |
| GET | `/api/export/csv` | handleExportCSV | v3.0.0 |
| GET | `/api/export/txt` | handleExportTXT | **v3.1.0** |

**Total :** 17 endpoints

---

## 📏 Statistiques du Code

### Lignes de Code par Fichier

| Fichier | Lignes | Commentaires | Code Effectif |
|---------|--------|--------------|---------------|
| ESP32-Diagnostic.ino | ~300 | ~80 | ~220 |
| exemple-config.h | ~200 | ~120 | ~80 |
| api_handlers.h | ~800 | ~150 | ~650 |
| web_interface.h | ~1200 | ~100 | ~1100 |
| test_functions.h | ~400 | ~80 | ~320 |
| translations.h | ~300 | ~40 | ~260 |
| **TOTAL** | **~3200** | **~570** | **~2630** |

### Répartition du Code

```
📊 Répartition :
- Interface Web : 37% (web_interface.h)
- API Handlers : 25% (api_handlers.h)
- Tests Hardware : 12% (test_functions.h)
- Traductions : 9% (translations.h)
- Configuration : 9% (Principal + config)
- Documentation : 8% (Commentaires)
```

---

## 🔐 Fichiers de Sécurité

### .gitignore (IMPORTANT)

```gitignore
# Configuration privée
config.h

# Build Arduino
build/
*.hex
*.elf
*.bin

# IDE
.vscode/
.idea/
*.swp
*~

# OS
.DS_Store
Thumbs.db

# Logs
*.log

# Backup
*.backup
*.bak
```

---

## 💾 Utilisation Mémoire

### Mémoire Flash (Programme)

**Tailles Estimées :**
```
Code compilé : ~450 KB
SPIFFS : ~1.5 MB (réservé)
OTA : 0 KB (pas utilisé v3.1.0)
Free : ~2 MB (sur partition 4MB)
```

### Mémoire RAM (Runtime)

**Utilisation Typique :**
```
Heap utilisé : ~50-80 KB
Heap libre : ~250-300 KB (ESP32-S3)
PSRAM utilisé : ~0 KB (non utilisé v3.1.0)
PSRAM libre : ~8 MB
Stack : ~8-12 KB par tâche
```

**Pics Mémoire :**
- Scanner WiFi : +20 KB temporaire
- Benchmarks : +10 KB temporaire
- Export JSON : +8 KB temporaire

---

<div align="center">

## 🎉 Fin de la Documentation Structure v3.1.0

**Projet :** ESP32 Diagnostic System  
**Version :** 3.1.0  
**Date :** 16 Octobre 2025  
**Auteur :** ESP32 Diagnostic Team  

---

**📚 Documents Connexes :**
- [README.md](README.md) - Documentation principale
- [README.en.md](README.en.md) - English documentation
- [CHANGELOG.md](CHANGELOG.md) - Historique versions
- [CHANGELOG.en.md](CHANGELOG.en.md) - Version history
- [INSTALLATION.md](INSTALLATION.md) - Guide installation
- [INSTALLATION.en.md](INSTALLATION.en.md) - Installation guide

---

*Merci d'utiliser ESP32 Diagnostic System v3.1.0 !* ⭐

**[🇬🇧 English Version](PROJECT_STRUCTURE.en.md)**

</div>