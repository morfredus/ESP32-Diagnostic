<!-- 
Available in multiple languages:
- English: README.md
- Français: README.fr.md
-->
# Diagnostic ESP32 Complet v3.0.0

🇫🇷 Version française | [🇬🇧 English Version](README.md)

> **Note :** Ceci est la version française. For the English version, [click here](README.md).

---

## 📋 Description

Outil de diagnostic exhaustif **dynamique temps réel** pour microcontrôleurs ESP32, accessible via interface web. Teste automatiquement tous les composants matériels, analyse la mémoire, scanne les périphériques et génère des rapports détaillés **sans rechargement de page**.

**Nouveauté v3.0.0** : Interface **entièrement dynamique** avec **mises à jour en temps réel** toutes les 5 secondes et **changement de langue instantané** (FR/EN) sans rechargement.

## ✨ Fonctionnalités

### 🔄 Interface Dynamique Temps Réel (NOUVEAU v3.0.0)
- **Aucun rechargement de page** - Tout se met à jour dynamiquement via API REST
- **Actualisation automatique toutes les 5 secondes** - Surveillance en direct de l'uptime, température, mémoire
- **Changement de langue instantané** - Bouton FR/EN sans rechargement
- **Indicateur d'état de connexion** - Retour visuel en ligne/hors ligne (point pulsant)
- **Notifications de mise à jour** - Badge flottant indiquant les actualisations
- **Animations fluides** - Effets fadeIn, interactions au survol, transitions partout
- **UI/UX moderne** - Design en cartes avec profondeur et ombres

### 🌐 Interface Multilingue (v2.4+)
- **Français** (langue par défaut)
- **Anglais**
- **Changement dynamique** : Boutons FR/EN dans l'interface
- **Mise à jour en temps réel** : Aucun rechargement nécessaire
- **Traductions complètes** : Tous les textes, labels, messages et statuts

### Tests Matériels
- **LED intégrée** - Configuration et test avec patterns (blink, fade, on/off)
- **NeoPixel/WS2812B** - Support multi-LED avec effets RGB (arc-en-ciel, fade, couleurs personnalisées)
- **Écran TFT 320x240** - Test couleurs, formes, texte
- **Écran OLED 0.96" I2C** - 10 tests d'affichage incluant animations
- **GPIO** - Test automatique de tous les GPIO disponibles
- **ADC** - Lecture de tous les canaux analogiques
- **Touch Pads** - Test capteurs tactiles capacitifs
- **PWM/LEDC** - Test modulation largeur d'impulsion

### Analyse Système
- **Mémoire détaillée** - Flash, PSRAM (OPI/QSPI), SRAM avec tests d'intégrité
- **WiFi Scanner** - Scan réseaux avec RSSI, canal, encryption
- **Scan I2C** - Détection automatique périphériques (0x01-0x7F)
- **Scan SPI** - Informations bus SPI disponibles
- **Partitions Flash** - Liste complète des partitions
- **Benchmarks** - Performance CPU et mémoire
- **Stress Test** - Test limite allocation mémoire

### Interface Web
- **8 onglets** - Navigation intuitive (Vue d'ensemble, LEDs, Écrans, Tests avancés, GPIO, WiFi, Performance, Export)
- **Sélecteur de langue** - Boutons FR/EN en haut à droite
- **Configuration dynamique** - Pins I2C OLED modifiables via web
- **Temps réel** - Actualisation données sans rechargement (toutes les 5s)
- **Responsive** - Compatible mobile/tablette/desktop
- **Exports complets** - TXT, JSON, CSV, version imprimable PDF

### Accès Réseau
- **mDNS** - Accès via http://ESP32-Diagnostic.local
- **Multi-WiFi** - Connexion automatique avec failover
- **API REST** - Endpoints JSON pour intégration et mises à jour temps réel

## 🎯 Cartes Compatibles

Testé et optimisé pour :
- ✅ **ESP32** (original)
- ✅ **ESP32-S2**
- ✅ **ESP32-S3** (recommandé - avec PSRAM OPI)
- ✅ **ESP32-C3**
- ⚠️ **ESP32-C6, ESP32-H2** (non testé mais devrait fonctionner)

## 📦 Prérequis

### Matériel
- Carte ESP32 (n'importe quel modèle)
- Câble USB
- Optionnel : LED NeoPixel, écran TFT, écran OLED

### Logiciel
- **Arduino IDE** 2.x ou supérieur
- **ESP32 Arduino Core** 3.1.3 ou supérieur

### Bibliothèques Arduino Requises

**Obligatoires (via Gestionnaire de bibliothèques) :**
```
1. Adafruit BusIO
2. Adafruit GFX Library
3. Adafruit SSD1306
4. Adafruit NeoPixel
5. TFT_eSPI
```

**Incluses avec ESP32 Core :**
```
WiFi, WebServer, ESPmDNS, Wire, SPI
esp_chip_info, esp_mac, esp_flash, esp_heap_caps, esp_partition
```

## 🚀 Installation

### 1. Téléchargement

Téléchargez ou clonez ce projet dans votre dossier Arduino.

### 2. Fichiers requis

**Structure du projet :**
```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino              (fichier principal)
├── api_handlers.h                    (handlers API REST)
├── web_interface.h                   (interface web dynamique)
├── languages.h                       (traductions FR/EN)
├── config.h.example                  (template configuration)
├── config.h                          (vos identifiants - à créer)
└── README.md
```

### 3. Configuration WiFi

**IMPORTANT :** Créez un fichier `config.h` dans le même dossier que le `.ino`

**Option A - Renommer le fichier exemple :**
Renommez le fichier `exemple-config.h` en `config.h`
Éditez le fichier et ajoutez votre/vos réseau(x) WiFi en renseignant les valeurs

```cpp
const char* WIFI_SSID_1 = "SSID1";
const char* WIFI_PASS_1 = "MotDePasse1";

const char* WIFI_SSID_2 = "SSID2";
const char* WIFI_PASS_2 = "MotDePasse2";

// Ajoutez autant de réseaux que nécessaire
```

**Option B - Créer manuellement `config.h` :**
```cpp
#ifndef CONFIG_H
#define CONFIG_H

// ========== CONFIGURATION WIFI ==========
const char* WIFI_SSID_1 = "VotreSSID";
const char* WIFI_PASS_1 = "VotreMotDePasse";

// Ajoutez autant de réseaux que nécessaire
// const char* WIFI_SSID_2 = "SSID2";
// const char* WIFI_PASS_2 = "MotDePasse2";

#endif
```

### 4. Copier les fichiers requis

Copiez les fichiers fournis dans le même dossier que le `.ino` :
- `languages.h` (traductions)
- `api_handlers.h` (endpoints API)
- `web_interface.h` (interface dynamique)

### 5. Configuration matérielle

**PSRAM (si disponible) :**
- ESP32-S3 : `Outils → PSRAM → OPI PSRAM`
- ESP32 : `Outils → PSRAM → Enabled`

**Flash Size :**
```
Outils → Flash Size → [Taille réelle de votre carte]
```

### 6. Compilation et téléversement

```
Croquis → Téléverser
```

### 7. Accès à l'interface

- **http://ESP32-Diagnostic.local** (mDNS)
- **http://[IP_AFFICHEE]** (IP directe)

## 🌐 Utilisation des Fonctionnalités Dynamiques

### Mises à Jour Temps Réel

L'interface s'actualise automatiquement toutes les 5 secondes :
- ✅ **Uptime** - Mis à jour en continu
- ✅ **Température** - Température CPU en direct (si supporté)
- ✅ **Mémoire** - Utilisation SRAM/PSRAM avec barres de progression animées
- ✅ **Fragmentation** - Pourcentage de fragmentation mémoire

**État de connexion :**
- 🟢 **Point vert pulsant** - Connecté et en cours de mise à jour
- 🔴 **Point rouge pulsant** - Connexion perdue

### Changement de Langue

**Dans l'interface web :**
1. Cliquez sur le bouton **FR** ou **EN** en haut à droite
2. L'interface se met à jour **instantanément** (< 100ms)
3. Aucun rechargement nécessaire
4. Tous les onglets et contenus sont actualisés dans la langue sélectionnée

**Par défaut :** Interface en français

**Via API :**
```bash
# Passer en anglais
curl http://ESP32-Diagnostic.local/api/set-language?lang=en

# Passer en français
curl http://ESP32-Diagnostic.local/api/set-language?lang=fr

# Obtenir les traductions courantes (avec cache-busting)
curl http://ESP32-Diagnostic.local/api/get-translations?lang=fr&t=1234567890
```

### Onglets disponibles

1. **Vue d'ensemble** - Informations complètes système avec mises à jour temps réel
2. **LEDs** - Tests LED intégrée et NeoPixel avec contrôle instantané
3. **Écrans** - Tests TFT et OLED
4. **Tests Avancés** - ADC, Touch, PWM, SPI, Partitions, Stress Test
5. **GPIO** - Test automatique tous GPIO
6. **WiFi** - Scanner réseaux WiFi
7. **Performance** - Benchmarks CPU/Mémoire
8. **Export** - TXT, JSON, CSV, Version imprimable

## 📊 API REST

**Nouveaux endpoints v3.0.0 :**
```
GET /api/status                      → État système temps réel (auto-refresh)
GET /api/overview                    → Vue d'ensemble complète du système
GET /api/leds-info                   → État LED et NeoPixel
GET /api/screens-info                → Informations TFT et OLED
GET /api/set-language?lang=fr        → Changer langue (fr/en)
GET /api/get-translations?lang=XX    → Obtenir traductions JSON (cache-busted)
```

**Routes existantes :**
```
GET /                                → Interface web dynamique
GET /api/test-gpio                   → Test GPIO
GET /api/wifi-scan                   → Scanner WiFi
GET /api/builtin-led-test            → Test LED intégrée
GET /api/builtin-led-control?action  → Contrôle LED (on/off/blink/fade)
GET /api/neopixel-test               → Test NeoPixel
GET /api/neopixel-pattern?pattern    → Patterns NeoPixel
GET /api/neopixel-color?r&g&b        → Couleur NeoPixel personnalisée
GET /api/benchmark                   → Benchmarks performance
GET /export/txt                      → Export TXT
GET /export/json                     → Export JSON
GET /export/csv                      → Export CSV
```

## 🛠️ Personnalisation

### Modifier la langue par défaut

Dans `languages.h` :
```cpp
Language currentLanguage = LANG_EN; // Pour anglais par défaut
```

### Ajouter une nouvelle langue

1. Définir l'enum dans `languages.h` :
```cpp
enum Language {
  LANG_FR = 0,
  LANG_EN = 1,
  LANG_ES = 2  // Espagnol
};
```

2. Créer la structure de traductions :
```cpp
const Translations LANG_ES_TEXTS = {
  "Diagnóstico ESP32 Completo",
  "v",
  // ... toutes les traductions
};
```

3. Modifier la fonction `T()` pour inclure la nouvelle langue.

## 🔧 Configuration Pins

### Pins par défaut

**I2C (OLED) - Modifiable via web :**
| Modèle   | SDA | SCL |
|----------|-----|-----|
| ESP32    | 21  | 22  |
| ESP32-S2 | 8   | 9   |
| ESP32-S3 | 8   | 9   |
| ESP32-C3 | 5   | 6   |

**TFT SPI (fixes) :**
```
MOSI: 45, SCLK: 3, CS: 14, DC: 47, RST: 21, MISO: 46
```

**LED Intégrée (auto-détectée) :**
- ESP32-S3 : GPIO 2
- ESP32-C3 : GPIO 8
- ESP32 : GPIO 2

**NeoPixel (auto-détectée) :**
- ESP32-S3 : GPIO 48
- ESP32-C3 : GPIO 8

## 🛠️ Dépannage

### Problèmes d'Interface Dynamique

**Interface ne se met pas à jour :**
1. Vérifier la console navigateur pour erreurs (F12)
2. Vérifier que l'ESP32 est connecté au WiFi
3. Vérifier l'indicateur de connexion (doit être vert et pulsant)
4. Essayer un rafraîchissement forcé (Ctrl+F5)

**Le changement de langue ne fonctionne pas :**
1. Vérifier que `languages.h` est dans le dossier
2. Vider complètement le cache du navigateur
3. Vérifier la console navigateur pour erreurs API
4. S'assurer que JavaScript est activé

**Les appels API échouent :**
1. Vérifier le Moniteur Série pour l'état de l'ESP32
2. Vérifier que l'adresse IP est correcte
3. Tester l'endpoint API directement : `http://[IP]/api/status`
4. Vérifier la force du signal WiFi

### Problèmes de Mémoire

**Avertissements mémoire faible :**
- Normal avec beaucoup de fonctionnalités actives
- PSRAM aide significativement si disponible
- Réduire le nombre de LEDs dans la configuration NeoPixel
- Désactiver les fonctionnalités inutilisées

## 📈 Performances

- **Temps de démarrage** : ~5s
- **Chargement initial page** : ~200ms (60% plus rapide que v2.x)
- **Changement de langue** : <100ms (sans rechargement)
- **Intervalle auto-refresh** : 5000ms (configurable)
- **Temps de réponse API** : <50ms par endpoint
- **Mémoire heap utilisée** : ~250KB
- **Surcharge multilingue** : ~15KB Flash
- **Empreinte JavaScript** : ~30KB

## 🔒 Sécurité

⚠️ **Usage local/développement uniquement.**

**Considérations de sécurité :**
- Aucune authentification implémentée
- Pas de support HTTPS
- Identifiants WiFi en clair dans config.h
- Tous les endpoints API sont ouverts

**Ne jamais exposer cette interface sur Internet public.**

**Ne jamais partager `config.h` avec vos identifiants WiFi.**

## 📁 Structure du Projet v3.0.0

```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino              (code principal + setup)
├── api_handlers.h                    (endpoints API REST - NOUVEAU)
├── web_interface.h                   (interface dynamique - NOUVEAU)
├── languages.h                       (système traduction FR/EN)
├── config.h.example                  (template)
├── config.h                          (vos identifiants - gitignore)
├── README.md                         (version anglaise)
├── README.fr.md                      (ce fichier)
├── CHANGELOG.md                      (historique versions)
└── .gitignore                        (exclut config.h)
```

## 📄 Changelog

### v3.0.0 (16/01/2025) - INTERFACE DYNAMIQUE
- ✨ **Interface entièrement dynamique** sans rechargement de page
- ✨ **Mises à jour temps réel** toutes les 5 secondes
- ✨ **Changement de langue instantané** (FR/EN)
- ✨ **Architecture API REST** complète
- ✨ **Indicateur d'état de connexion** avec retour visuel
- ✨ **Animations fluides** et UI/UX moderne
- ✨ Toutes les fonctionnalités v2.5 préservées

### v2.5 (10/01/2025) - EXPORTS MULTILINGUES
- ✨ **Fichiers exportés traduits** (FR/EN)

### v2.4 (09/01/2025) - MULTILINGUE
- ✨ **Système multilingue complet** (FR/EN)
- ✨ Nouveau fichier `languages.h` avec traductions
- ✨ Boutons FR/EN dans l'interface
- ✨ API REST pour changement de langue

### v2.3 (08/01/2025)
- Test OLED 0.96" avec 10 animations
- Configuration pins I2C dynamique

### v2.2 (07/01/2025)
- Support écran TFT SPI

### v2.1 (06/01/2025)
- Support NeoPixel/WS2812B

### v2.0 (05/01/2025)
- Refonte complète du système de diagnostic

## 📝 Licence

Ce projet est libre d'utilisation, modification et distribution sous licence MIT.

## 🤝 Contribution

**Pour ajouter une langue :**
1. Fork le projet
2. Ajoutez votre langue dans `languages.h`
3. Testez tous les écrans et fonctionnalités
4. Pull Request avec captures d'écran

**Pour signaler des bugs ou suggérer des fonctionnalités :**
- Ouvrir une issue sur GitHub
- Inclure le modèle ESP32 et la version Arduino Core
- Fournir la sortie du Moniteur Série si applicable

## 🎓 Crédits

Développé pour la communauté ESP32.

**v3.0.0 - Interface dynamique temps réel**
**v2.5 - Exports multilingues**
**v2.4 - Système multilingue**
**v2.3 - Base exhaustive**

---

## 🚀 Guide de Démarrage Rapide

1. **Installer Arduino IDE 2.x** et ESP32 Core 3.1.3+
2. **Installer les bibliothèques requises** via le Gestionnaire
3. **Télécharger le projet** et créer `config.h` avec vos identifiants WiFi
4. **Copier tous les fichiers .h** dans le dossier du sketch
5. **Configurer PSRAM** si votre carte le supporte
6. **Téléverser vers ESP32**
7. **Ouvrir le Moniteur Série** pour obtenir l'adresse IP
8. **Accéder** à http://ESP32-Diagnostic.local ou http://[IP]
9. **Profiter de la surveillance en temps réel !**

## 📞 Support

- **Documentation** : Voir commentaires dans le code et ce README
- **Issues** : GitHub Issues
- **Email** : Vérifier le dépôt pour informations de contact

---

**Version actuelle** : 3.0.0 Dynamique Temps Réel
**Dernière mise à jour** : 16 janvier 2025
**Langues disponibles** : Français (défaut), Anglais  
**Support** : ESP32 Arduino Core 3.1.3+
**Navigateurs requis** : ES6+ (Chrome 51+, Firefox 54+, Safari 10+)

🌐 **Accès** : http://ESP32-Diagnostic.local  
🇫🇷🇬🇧 **Changement de langue** : Boutons FR/EN dans l'interface (instantané, sans rechargement)
⚡ **Temps réel** : Actualisation automatique toutes les 5 secondes
🔄 **Dynamique** : Aucun rechargement de page requis

---

## 🌟 Résumé des Fonctionnalités Clés

| Fonctionnalité | v2.x | v3.0.0 |
|----------------|------|--------|
| Rechargements page | Requis | ❌ Aucun |
| Changement langue | Rechargement nécessaire | ✅ Instantané |
| Mises à jour temps réel | Rafraîchissement manuel | ✅ Auto (5s) |
| État connexion | Aucun | ✅ Indicateur visuel |
| Architecture API | Partielle | ✅ REST complet |
| Performance | Baseline | ✅ 60% plus rapide |
| Utilisation mémoire | Baseline | ✅ Optimisée |
| Animation/UX | Basique | ✅ Moderne |

**Passez à la v3.0.0 pour la meilleure expérience !**
