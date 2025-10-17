# 📋 CHANGELOG - ESP32 Diagnostic

[🇬🇧 Read in English](CHANGELOG.en.md) | [📖 Documentation](README.md)

Toutes les modifications notables de ce projet seront documentées dans ce fichier.

Le format est basé sur [Keep a Changelog](https://keepachangelog.com/fr/1.0.0/),
et ce projet adhère au [Versioning Sémantique](https://semver.org/lang/fr/).

---

## [3.2.0] - 2025-10-17

### ✨ Nouvelles fonctionnalités

#### Configuration WiFi Multi-SSID
- **WiFiMulti** : Support de plusieurs réseaux WiFi
  - Configuration de multiples SSID/mots de passe
  - Connexion automatique au réseau disponible le plus fort
  - Ordre de priorité configurable
  - Reconnexion automatique en cas de perte de signal
- **Fichier wifi-config.h** : Configuration WiFi séparée et sécurisée
  - Template `exemple-wifi-config.h` fourni
  - Supporte un nombre illimité de réseaux
  - Exclusion automatique de Git (.gitignore)
  - Configuration du point d'accès de secours

#### Configuration matérielle dynamique
- **Fichier config.h** : Configuration centralisée des pins
  - Configuration LED interne
  - Configuration NeoPixel (optionnel)
  - Configuration OLED (optionnel)
  - Configuration capteurs température (optionnel)
  - Configuration buzzer et boutons (optionnels)
  - Arrays configurables pour tests GPIO/PWM/ADC

#### Interface Web améliorée
- **Configuration des pins** : Édition via interface web (à venir)
  - Modifier les pins sans recompiler
  - Sauvegarde persistante de la configuration
  - Validation des pins en temps réel

### 🔒 Sécurité

- **Fichiers sensibles protégés** :
  - `wifi-config.h` exclu de Git
  - `config.h` exclu de Git
  - `.gitignore` complet fourni
- **Mode Point d'Accès** : Failover automatique si aucun WiFi disponible
- **Détection fichiers manquants** : Warnings si configuration absente

### 🔧 Améliorations

#### Architecture
- **Séparation des configurations** : Code/Configuration découplés
- **Templates fournis** : Exemples détaillés pour toutes configurations
- **Détection automatique** : Utilise `__has_include` pour fichiers optionnels
- **Compatibilité** : Fonctionne sans fichiers de configuration (mode par défaut)

#### Performance
- **Reconnexion intelligente** : Tente de se reconnecter toutes les 30s si déconnecté
- **Multi-réseau** : Pas d'interruption lors du changement de réseau
- **Timeout configurable** : Ajustable par réseau

### 📝 Documentation

- **INSTALLATION.md** : Section ajoutée pour configuration multi-WiFi
- **INSTALLATION.en.md** : Version anglaise mise à jour
- **README.md** : Documentation de la nouvelle architecture
- **README.en.md** : Version anglaise mise à jour
- **Exemples détaillés** : Dans wifi-config.h et config.h

### ⚙️ Configuration requise

**Nouveaux fichiers à créer** :
1. Copier `exemple-wifi-config.h` → `wifi-config.h`
2. Copier `exemple-config.h` → `config.h`  
3. Configurer vos réseaux WiFi dans `wifi-config.h`
4. Ajuster les pins matérielles dans `config.h`

---

## [3.1.1] - 2025-10-17

### 🐛 Correctifs

#### Correction de compilation
- **API PWM** : Migration vers la nouvelle API `ledcAttach()` pour ESP32 Core 3.x
  - Remplacement de `ledcSetup()` + `ledcAttachPin()` par `ledcAttach()`
  - Compatible avec toutes les versions récentes du core ESP32
- **Typage des fonctions** : Correction du type de retour
  - `getFlashSize()` retourne maintenant `size_t` au lieu de `String`
  - `getPSRAMSize()` retourne maintenant `size_t` au lieu de `String`
  - Correction de `handleExportCSV()` pour utiliser les valeurs numériques

#### Fonctions manquantes
- **NeoPixel** : Ajout des fonctions dans le fichier principal
  - `setNeoPixelColor(r, g, b)` : Définir la couleur RGB
  - `neoPixelRainbow(cycles)` : Animation arc-en-ciel
  - Versions vides si `USE_NEOPIXEL` non défini
- **LED intégrée** : Ajout de `initBuiltinLED()`

#### Corrections syntaxiques
- **Guillemets manquants** : Correction ligne 820 dans `api_handlers.h`
  - Ajout du guillemet fermant dans `server.send()`
- **Accolades** : Vérification de toutes les fermetures de fonctions

#### Routes API
- **Simplification** : Suppression des routes non implémentées
  - Retrait de `/api/led/builtin` (non implémenté)
  - Retrait de `/api/test/adc` (non implémenté)
  - Retrait de `/api/test/touch` (non implémenté)
  - Retrait de `/api/export/txt` (non implémenté)

### 📝 Documentation

- **INSTALLATION.md** : Mise à jour de la version
- **INSTALLATION.en.md** : Mise à jour de la version
- Correction des références de version dans tous les exemples

---

## [3.1.0] - 2025-10-16

### ✨ Nouvelles fonctionnalités

#### Interface Web
- Interface moderne et responsive avec design professionnel
- 8 pages thématiques : Dashboard, Tests, Mémoire, WiFi, GPIO, Benchmarks, Exportation, À propos
- Sélecteur de langue FR/EN avec mémorisation
- Animations et transitions fluides
- Mode sombre élégant avec dégradés

#### API REST
- 18 endpoints REST complets
- Support CORS pour développement cross-origin
- Export JSON et CSV
- Gestion multilingue via API

#### Système NeoPixel
- Support LED NeoPixel WS2812B
- Contrôle couleurs RGB via API
- Patterns prédéfinis (blink, fade, rainbow, test)
- Indicateurs visuels d'état système

#### Tests Hardware
- Tests GPIO complets
- Tests PWM avec nouvelle API
- Scan WiFi avec détails réseaux
- Benchmarks performance (CPU, mémoire)
- Tests capteurs internes

### 🔧 Améliorations

#### Performance
- Optimisation mémoire pour ESP32-S3
- Gestion efficace des buffers JSON
- Réduction latence serveur web

#### Compatibilité
- Support ESP32 Core 3.x
- Compatible ESP32, ESP32-S2, ESP32-S3, ESP32-C3
- Détection automatique PSRAM

#### Documentation
- README complet en FR et EN
- Guide d'installation détaillé
- Structure projet documentée
- Exemples API REST

---

## [3.0.0] - 2025-10-15

### ✨ Version initiale

#### Fonctionnalités de base
- Système de diagnostic ESP32
- Interface web basique
- Informations système
- Tests hardware simples
- Export de données

#### Compatibilité
- ESP32 classique
- Support WiFi basique
- Moniteur série

---

## 🔗 Liens

- **Documentation** : [README.md](README.md)
- **Installation** : [INSTALLATION.md](INSTALLATION.md)
- **Structure** : [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md)
- **Repository** : [GitHub](https://github.com/votre-username/ESP32-Diagnostic)

---

## 📌 Convention de versionnement

Ce projet suit le [Versioning Sémantique](https://semver.org/lang/fr/) :

- **MAJOR** (X.0.0) : Changements incompatibles avec les versions précédentes
- **MINOR** (0.X.0) : Ajout de fonctionnalités compatibles avec les versions précédentes
- **PATCH** (0.0.X) : Corrections de bugs compatibles avec les versions précédentes

### Types de changements

- ✨ **Nouvelles fonctionnalités** : Ajout de nouvelles capacités
- 🔧 **Améliorations** : Optimisations et améliorations
- 🐛 **Correctifs** : Corrections de bugs
- 🔒 **Sécurité** : Correctifs de vulnérabilités
- 📝 **Documentation** : Mises à jour de la documentation
- ⚠️ **Déprécié** : Fonctionnalités bientôt supprimées
- 🗑️ **Supprimé** : Fonctionnalités supprimées

---

**Dernière mise à jour** : 17 octobre 2025  
**Version actuelle** : 3.1.1