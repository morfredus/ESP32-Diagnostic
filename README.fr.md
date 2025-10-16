# Diagnostic ESP32 Complet v3.0.1 - Multilingue

<div align="center">

[![en](https://img.shields.io/badge/lang-en-red.svg)](README.md)
[![fr](https://img.shields.io/badge/lang-fr-blue.svg)](README.fr.md)

![ESP32](https://img.shields.io/badge/ESP32-Compatible-green)
![Version](https://img.shields.io/badge/version-3.0.1-blue)
![Core](https://img.shields.io/badge/Arduino%20Core-3.3.2+-orange)
![License](https://img.shields.io/badge/license-MIT-orange)

**[🇬🇧 Read in English](README.md)**

</div>

---

## 📋 Description

Outil de diagnostic exhaustif **multilingue** pour microcontrôleurs ESP32, accessible via interface web moderne. Teste automatiquement tous les composants matériels, analyse la mémoire, scanne les périphériques et génère des rapports détaillés.

**Nouveauté v3.0.1** : Mise à niveau vers **Arduino Core ESP32 3.3.2** avec gestion mémoire optimisée, stabilité WiFi améliorée et 12 nouvelles métriques système.

---

## ✨ Fonctionnalités

### 🌍 Interface Multilingue
- **Français** (langue par défaut)
- **Anglais**
- **Changement dynamique** : Boutons FR/EN dans l'interface
- **Mise à jour en temps réel** : Aucun rechargement nécessaire
- **Traductions complètes** : Tous les textes, labels, messages et statuts

### 📡 Connectivité Multi-Réseau ✨ NOUVEAU
- **Multi-WiFi** : Supporte plusieurs réseaux WiFi
- **Basculement automatique** : Essaie chaque réseau dans l'ordre
- **Flexible** : Fonctionne à la maison, au bureau, en déplacement
- **Fallback AP** : Mode Point d'Accès si aucun réseau disponible

### 🎨 Design Moderne
- **Glassmorphism** avec effets de transparence et flou
- **Dégradés animés** violet/bleu
- **Animations fluides** sur tous les éléments
- **Interface responsive** pour mobile/tablette/desktop

### 🔄 Mise à Jour Temps Réel
- Actualisation automatique toutes les **5 secondes**
- Indicateur visuel pulsant
- Barre de progression pour la prochaine mise à jour
- Surveillance système en direct

### 🧪 Tests Matériels Complets

#### Test GPIO
- Tous les pins disponibles testés
- Fonctionnalité Entrée/Sortie
- Statut : OK/FAIL avec détails

#### Test I2C
- Scan automatique des périphériques (0x00-0x7F)
- Identification des périphériques courants
- Affichage adresse et nom du périphérique

#### Test SPI
- Détection et configuration des bus
- Cartographie des pins (MOSI, MISO, SCLK)
- Vérification de compatibilité

#### Test Mémoire (Amélioré avec Core 3.3.2) ⚡
- Heap libre/total
- PSRAM libre/total (si disponible)
- **NOUVEAU** : Mémoire libre minimale depuis le démarrage
- **NOUVEAU** : Plus grand bloc libre
- **NOUVEAU** : Pourcentage de fragmentation mémoire
- Statut intelligent (OK/MOYEN/BAS)

#### Test WiFi (Amélioré avec Core 3.3.2) ⚡
- Statut de connexion
- Force du signal (RSSI)
- SSID et adresse IP
- Adresse MAC
- **NOUVEAU** : Canal WiFi
- **NOUVEAU** : Adresse passerelle
- **NOUVEAU** : Serveur DNS
- **NOUVEAU** : Masque sous-réseau
- **NOUVEAU** : Qualité du signal (Excellent/Bon/Moyen/Faible)

#### Test Système (Amélioré avec Core 3.3.2) ⚡
- Modèle et révision de la puce
- Cœurs CPU et fréquence
- Taille et vitesse de la Flash
- Durée de fonctionnement
- **NOUVEAU** : Raison du dernier reset
- **NOUVEAU** : Version ESP-IDF
- **NOUVEAU** : Statut système global (SAIN/AVERTISSEMENT/CRITIQUE)

### 💾 Export des Données
- **Format JSON** : Données structurées complètes
- **Format CSV** : Compatible Excel
- Téléchargement en un clic
- Toutes les données de diagnostic incluses

### 🎛️ Contrôle NeoPixel (Optionnel)
- Motifs prédéfinis (Arc-en-ciel, Pulsation, Stroboscope)
- Couleur RGB personnalisée
- Contrôle en temps réel

---

## 🚀 Démarrage Rapide

### Prérequis
- **ESP32** (ESP32-S2, ESP32-S3, ou ESP32-C3)
- **Arduino IDE 2.x** ou PlatformIO
- **Arduino Core ESP32 v3.3.2+** ⚡ (minimum requis)
- Navigateur web moderne

### Installation

1. **Installer ESP32 Board Manager v3.3.2+**
   ```
   Fichier > Préférences > URLs supplémentaires :
   https://espressif.github.io/arduino-esp32/package_esp32_index.json

   Outils > Gestionnaire de cartes > Rechercher "ESP32" > Installer 3.3.2+
   ```

2. **Installer les Bibliothèques Requises**
   ```
   Croquis > Inclure une bibliothèque > Gérer les bibliothèques
   - Adafruit NeoPixel (dernière version)
   - ArduinoJson 6.x (PAS la v7)
   ```

3. **Télécharger les Fichiers du Projet**
   - ESP32-Diagnostic.ino
   - exemple-config.h ⚡ IMPORTANT (template)
   - api_handlers.h
   - test_functions.h
   - translations.h
   - web_interface.h
   - .gitignore

4. **Créer votre fichier de configuration**
   ```bash
   # Copier le template
   cp exemple-config.h config.h
   ```

   Éditer `config.h` :
   ```cpp
   const char* ssid = "VotreSSID";
   const char* password = "VotreMotDePasse";
   ```

   ⚠️ **SÉCURITÉ** :
   - `config.h` contient VOS identifiants (dans .gitignore)
   - `exemple-config.h` est le template (safe pour Git)
   - Ne JAMAIS commiter `config.h` configuré

5. **Compiler et Téléverser**
   - Sélectionner votre carte (ESP32S3 Dev Module)
   - Configurer les paramètres (Flash: 8MB, PSRAM: OPI si disponible)
   - Téléverser ✓

6. **Accéder à l'Interface**
   - Ouvrir le navigateur : `http://esp32-diagnostic.local`
   - Ou utiliser l'adresse IP affichée dans le Moniteur Série

---

## 📊 Captures d'Écran de l'Interface

### Onglet Vue Générale
- Informations système (Puce, Cœurs, Fréquence, Flash, PSRAM)
- Statut mémoire avec pourcentage d'utilisation
- Statut WiFi et qualité du signal
- Mises à jour automatiques toutes les 5 secondes

### Onglet Tests
- Tableau complet des tests GPIO
- Résultats du scan I2C
- Configuration des bus SPI
- Analyse mémoire détaillée avec fragmentation
- Diagnostics WiFi avancés
- Informations système avec raison du reset

### Onglet Exports
- Export JSON en un clic
- Export CSV en un clic
- Toutes les données de diagnostic incluses

---

## 🔧 Configuration

### NeoPixel (Optionnel)
```cpp
#define NEOPIXEL_PIN 48        // Pin de données
#define NEOPIXEL_COUNT 1       // Nombre de LEDs
```

### Nom mDNS
```cpp
const char* mdnsName = "esp32-diagnostic";  // Accès via http://esp32-diagnostic.local
```

### Intervalle de Mise à Jour
```cpp
const unsigned long UPDATE_INTERVAL = 5000; // 5 secondes (dans web_interface.h)
```

---

## 🌐 Points d'Accès API

### Endpoints GET
- `/` - Interface web principale
- `/api/system` - Informations système (JSON)
- `/api/tests` - Tous les tests matériels (JSON)
- `/api/language` - Langue actuelle
- `/api/export/json` - Télécharger export JSON
- `/api/export/csv` - Télécharger export CSV

### Endpoints POST
- `/api/language` - Changer la langue (body: `{"language": "fr"}`)
- `/api/neopixel/pattern` - Définir motif (body: `{"pattern": "rainbow"}`)
- `/api/neopixel/color` - Définir couleur (body: `{"r": 255, "g": 0, "b": 0}`)

---

## 🆕 Nouveautés v3.0.1

### ⚡ Mise à Niveau Arduino Core
- **Arduino Core ESP32 3.1.3 → 3.3.2**
- Gestion mémoire améliorée avec API heap_caps
- Stabilité WiFi renforcée avec auto-reconnexion
- Support OPI PSRAM pour ESP32-S3

### 📊 Nouvelles Métriques (12 au total)
**Mémoire :**
- Heap libre minimale depuis le démarrage
- Plus grand bloc mémoire libre
- Pourcentage de fragmentation mémoire

**WiFi :**
- Canal WiFi utilisé
- Adresse IP de la passerelle
- Serveur DNS
- Masque de sous-réseau
- Évaluation de la qualité du signal

**Système :**
- Raison du dernier reset (Démarrage, Watchdog, etc.)
- Version ESP-IDF
- Vitesse de la puce Flash
- Statut de santé système global

### 🔧 Optimisations
- Auto-reconnexion WiFi activée par défaut
- Pas d'écriture flash pour les identifiants WiFi (persistent=false)
- Gestion des buffers JSON améliorée
- Meilleure gestion des erreurs

---

## 📖 Documentation

- [Guide d'Installation](INSTALLATION.md) - Guide complet étape par étape
- [Changelog](CHANGELOG.md) - Historique des versions
- [🇬🇧 English Installation Guide](INSTALLATION.md)
- [🇬🇧 English Changelog](CHANGELOG.md)

---

## 🛠️ Support Matériel

### Testé Sur
- ✅ ESP32-S3 (8MB Flash, OPI PSRAM) - Entièrement testé
- ✅ ESP32-S2 - Compatible
- ✅ ESP32-C3 - Compatible
- ✅ ESP32-C6 - Compatible (Core 3.3.2+)
- ✅ ESP32-H2 - Compatible (Core 3.3.2+)

### Exigences
- **Minimum** : ESP32 avec 4MB Flash
- **Recommandé** : ESP32-S3 avec 8MB Flash + PSRAM
- **Arduino Core** : 3.3.2 ou supérieur
- **ArduinoJson** : 6.x (pas 7.x)

---

## 🤝 Contribution

Les contributions sont les bienvenues ! N'hésitez pas à soumettre une Pull Request.

### Configuration Développement
1. Fork le dépôt
2. Créer votre branche de fonctionnalité (`git checkout -b feature/NouvelleFonctionnalite`)
3. Commit vos changements (`git commit -m 'Ajout d'une nouvelle fonctionnalité'`)
4. Push vers la branche (`git push origin feature/NouvelleFonctionnalite`)
5. Ouvrir une Pull Request

---

## 📝 Licence

Ce projet est sous licence MIT - voir le fichier LICENSE pour plus de détails.

---

## 🙏 Remerciements

- Espressif Systems pour la plateforme ESP32
- Adafruit pour la bibliothèque NeoPixel
- Communauté Arduino

---

## 📞 Support

- **Issues** : Signaler des bugs ou demander des fonctionnalités via GitHub Issues
- **Discussions** : Rejoindre les discussions pour questions et idées
- **Documentation** : Consulter INSTALLATION.md pour une aide détaillée

---

## 🗺️ Feuille de Route

### v3.1.0 (Prévu)
- Graphiques mémoire en temps réel
- Historique des métriques
- Alertes configurables
- Support capteurs (température, humidité)
- Bascule mode sombre/clair

### v3.2.0 (Prévu)
- Tableau de bord personnalisable
- Plugins pour capteurs tiers
- Exports automatiques planifiés
- Notifications push

### v4.0.0 (Futur)
- Refonte complète avec React
- Application mobile dédiée
- Synchronisation cloud optionnelle
- Surveillance multi-ESP32

---

**Version actuelle** : 3.0.1
**Arduino Core** : 3.3.2+
**Dernière mise à jour** : Octobre 2025
**Langues disponibles** : Français (défaut), Anglais

🌐 **Accès** : http://esp32-diagnostic.local
🇫🇷🇬🇧 **Changement de langue** : Boutons FR/EN dans l'interface

---

<div align="center">

Fait avec ❤️ pour la communauté ESP32

[⬆ Retour en haut](#diagnostic-esp32-complet-v301---multilingue)

</div>
