# Changelog - ESP32 Diagnostic System

Toutes les modifications notables de ce projet sont documentées dans ce fichier.

**[🇬🇧 English Version](CHANGELOG.en.md)**

---

## [3.1.0] - 2025-10-16

### 🎉 Version Majeure - Interface Complète 8 Pages

**Cette version intègre TOUTES les fonctionnalités de la v2.5 dans l'architecture moderne de la v3.0.1**

### ✨ Nouvelles Fonctionnalités Majeures

#### Interface Web Complète
- **8 pages fonctionnelles** au lieu de 3
- Navigation par onglets améliorée
- Design glassmorphism conservé
- Responsive mobile/tablette/desktop

#### Page 2: LEDs (NOUVEAU)
- **LED Intégrée (GPIO 97)**
  - Allumer/Éteindre
  - Clignotement (5x)
  - Test rapide
  - Effet fade PWM
- **NeoPixel (GPIO 48)**
  - Motifs: Rainbow, Pulse, Strobe
  - Couleur RGB personnalisée
  - Contrôle en temps réel

#### Page 3: Écrans (NOUVEAU)
- **Écran TFT 320x240 SPI**
  - Configuration CS:14, DC:47, RST:21
  - Boutons: Test, Couleurs, Damier, Effacer
  - Prêt pour connexion matérielle
- **Écran OLED 0.96" I2C**
  - Auto-détection 0x3C/0x3D
  - Configuration SDA/SCL modifiable
  - Test complet + message personnalisé

#### Page 4: Tests Avancés (NOUVEAU)
- **Test ADC**: 10 canaux (GPIO 1-10)
- **Test Touch Pads**: 14 pads sur ESP32-S3
- **Test PWM**: 4 canaux, 5kHz, 8 bits
- **Bus SPI**: Scan SPI2, SPI3
- **Partitions Flash**: Liste complète avec adresses
- **Stress Test Mémoire**: Test intensif RAM

#### Page 5: GPIO (NOUVEAU)
- Test complet **26 GPIO**
- Vérification Input/Output
- Test HIGH/LOW
- Tableau récapitulatif détaillé

#### Page 6: Scanner WiFi (NOUVEAU)
- Scan réseaux disponibles
- SSID, Signal, Canal, Qualité, Sécurité
- Tri par puissance signal
- Interface claire et lisible

#### Page 7: Performance (NOUVEAU)
- **CPU Benchmark**: 1M opérations, MFLOPS
- **Memory Benchmark**: Test 10KB, MB/s
- **Flash Benchmark**: Lecture/écriture

#### Page 8: Export (AMÉLIORÉ)
- **Format TXT** (NOUVEAU): Rapport texte structuré
- **Format JSON** (existant)
- **Format CSV** (existant)
- **Version Imprimable PDF** (NOUVEAU): print()

### 🔧 Nouveaux Endpoints API

#### LEDs
- `POST /api/led/builtin` - Contrôle LED intégrée

#### Tests Avancés
- `GET /api/test/gpio` - Test GPIO complet
- `GET /api/test/adc` - Test ADC tous canaux
- `GET /api/test/touch` - Test Touch Pads
- `GET /api/test/pwm` - Test PWM canaux

#### Scanner & Benchmark
- `GET /api/scan/wifi` - Scanner réseaux
- `GET /api/benchmark` - Benchmarks performance

#### Export
- `GET /api/export/txt` - Export texte (NOUVEAU)

### 📊 Améliorations Interface

- Navigation 8 onglets au lieu de 3
- Boutons colorés par catégorie
- Icônes emoji intuitives
- Grilles export en 4 colonnes
- Animations fluides optimisées

### 🔐 Sécurité & Configuration

- Système Multi-WiFi conservé v3.0.1
- Fichier config.h protégé (.gitignore)
- exemple-config.h template sûr

### ⚡ Performance

- Optimisation buffers JSON
- Gestion async requêtes API
- Cache navigateur optimisé

### 🐛 Corrections

- Fix affichage OLED basé scan I2C
- Fix responsive tabs overflow mobile
- Fix progression bar countdown
- Fix détection NeoPixel disponible
- Fix ordre définition handlers API
- Fix tailles optimales buffers JSON

### 📦 Fichiers Modifiés

- `ESP32-Diagnostic.ino` → v3.1.0
- `api_handlers.h` → v3.1.0 (tous endpoints)
- `web_interface.h` → v3.1.0 (8 pages complètes)
- `test_functions.h` → v3.0.1 (inchangé)
- `translations.h` → v3.0.1 (inchangé)

### 🎯 Points Forts v3.1.0

✅ **100% des fonctionnalités v2.5** intégrées  
✅ **Architecture moderne v3.0.1** conservée  
✅ **Multi-WiFi support** maintenu  
✅ **Arduino Core 3.3.2** optimisations  
✅ **8 pages complètes** fonctionnelles  
✅ **Design glassmorphism** magnifique  
✅ **Traductions FR/EN** complètes  
✅ **Export 4 formats** (TXT, JSON, CSV, PDF)  

---

## [3.0.1] - 2025-10-16

### ⚡ Améliorations Majeures

#### Upgrade Arduino Core
- **Arduino Core ESP32 3.1.3 → 3.3.2**
- Support optimisations Espressif
- Stabilité WiFi améliorée
- API heap capabilities améliorées

#### Multi-WiFi Support ✨ NOUVEAU
- **WiFiMulti intégré**: Support plusieurs réseaux
- **Basculement automatique**: Essaie chaque réseau
- **Flexible**: Maison, bureau, déplacement
- **Fallback AP**: Mode Point d'Accès si échec

#### Système Configuration Sécurisé
- **exemple-config.h**: Template public (safe Git)
- **config.h**: Configuration privée (ignorée Git)
- **.gitignore**: Protection automatique

#### Optimisations Mémoire (Core 3.3.2)
- `heap_caps_get_free_size()` précision
- `heap_caps_get_minimum_free_size()` tracking
- `heap_caps_get_largest_free_block()` fragmentation
- Analyse fragmentation pourcentage
- Support OPI PSRAM ESP32-S3

#### Nouvelles Métriques (12 au total)

**Mémoire:**
- Heap libre minimale depuis boot
- Plus grand bloc libre
- Pourcentage fragmentation

**WiFi:**
- Canal WiFi utilisé
- Adresse Gateway
- Serveur DNS
- Masque sous-réseau
- Qualité signal (Excellent/Good/Fair/Weak)

**Système:**
- Raison dernier reset
- Version ESP-IDF
- Vitesse puce Flash
- Statut santé système global

---

## [3.0.0] - 2025-10-10

### 🎉 Version Initiale

#### Fonctionnalités Principales
- Interface web dynamique temps réel
- Design glassmorphism moderne
- Navigation par onglets
- Système traduction FR/EN intégré
- Export JSON et CSV
- Animations transitions fluides

#### Architecture
- Structure modulaire 5 fichiers
- API REST complète
- Système traduction intégré
- WebServer optimisé

#### Support Matériel
- ESP32-S2, ESP32-S3, ESP32-C3
- Arduino Core ESP32 3.1.3+
- PSRAM optionnel

---

## 🔄 Migration Entre Versions

### De v3.0.1 vers v3.1.0

**✅ Compatible** - Aucun changement non rétrocompatible

**Étapes:**
1. Remplacer tous fichiers par v3.1.0
2. Conserver votre `config.h` existant
3. Compiler et téléverser
4. Profiter des 8 pages ! 🎉

**Améliorations visibles:**
- 5 nouveaux onglets interface
- Contrôles LEDs opérationnels
- Tests avancés fonctionnels
- Scanner WiFi actif
- Benchmarks disponibles
- Export TXT ajouté

### De v2.5 vers v3.1.0

**⚠️ Changements Importants**

**Étapes:**
1. Mettre à jour Arduino Core → 3.3.2+
2. Créer `config.h` depuis `exemple-config.h`
3. Configurer Multi-WiFi (recommandé)
4. Remplacer tous fichiers
5. Compiler et téléverser

---

## 🗺️ Roadmap Future

### v3.2.0 (Prévu - T1 2026)
- [ ] Graphiques temps réel Chart.js
- [ ] Historique métriques (SPIFFS)
- [ ] Alertes configurables
- [ ] Support capteurs I2C (BME280, DHT22)
- [ ] Mode dark/light toggle
- [ ] API WebSocket latence réduite

### v3.3.0 (Prévu - T2 2026)
- [ ] Dashboard personnalisable (drag & drop)
- [ ] Plugins capteurs tiers
- [ ] Export automatique planifié (CRON)
- [ ] Notifications push
- [ ] Logs système rotation

### v4.0.0 (Futur - T3 2026)
- [ ] Refonte complète React/Vue.js
- [ ] Application mobile (Flutter)
- [ ] Cloud sync optionnel (MQTT/HTTP)
- [ ] Monitoring multi-ESP32 réseau
- [ ] OTA Update via interface

---

## 📝 Notes Importantes

### Compatibilité Hardware

**✅ Testé et validé:**
- ESP32-S3 (8MB Flash, OPI PSRAM) - Excellent
- ESP32-S3 (4MB Flash, QSPI PSRAM) - Bon
- ESP32-S2 (4MB Flash) - Bon

**⚠️ Compatible non testé:**
- ESP32-C3, ESP32-C6, ESP32-H2

**❌ Non compatible:**
- ESP32 classique (manque fonctionnalités Core 3.3.2)

---

## 📄 Licence

MIT License - Utilisation, modification et distribution libres

---

## ⭐ Remerciements

- **Espressif Systems** pour plateforme ESP32
- **Adafruit** pour bibliothèque NeoPixel
- **Communauté Arduino** pour support
- **Tous les testeurs** v3.1.0

---

**Dernière mise à jour:** 16 Octobre 2025  
**Version actuelle:** 3.1.0  
**Prochaine version:** 3.2.0 (T1 2026)

---

*Pour signaler un bug ou suggérer une fonctionnalité, créez une issue sur le dépôt GitHub.*

**[🇬🇧 English Version](CHANGELOG.en.md)**