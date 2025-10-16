# Changelog - ESP32 Diagnostic System

Toutes les modifications notables de ce projet sont documentées dans ce fichier.

---

## [3.0.1] - 2025-10-16

### ⚡ Améliorations majeures

#### Upgrade Arduino Core
- **Arduino Core ESP32 3.1.3 → 3.3.2**
- Support des dernières optimisations d'Espressif
- Amélioration de la stabilité WiFi
- API heap capabilities améliorées

#### Multi-WiFi Support ✨ NOUVEAU
- **WiFiMulti intégré** : Support de plusieurs réseaux WiFi
- **Basculement automatique** : L'ESP32 essaie chaque réseau dans l'ordre
- **Flexible** : Fonctionne à la maison, au bureau, en déplacement
- **Configuration simple** : Ajoutez autant de réseaux que nécessaire
- **Fallback AP** : Mode Point d'Accès si aucun réseau disponible

Configuration Multi-WiFi exemple :
```cpp
#define MULTI_WIFI_ENABLED
#define NUM_SSIDS 2

const char* ssid_list[] = {"Home", "Office"};
const char* password_list[] = {"pass1", "pass2"};
```

#### Système de configuration sécurisé
- **exemple-config.h** : Template public (safe pour Git)
- **config.h** : Votre configuration privée (ignorée par Git)
- **.gitignore** : Protection automatique des identifiants
- **Documentation complète** : Guide de sécurité inclus

#### Optimisations mémoire (Core 3.3.2)
- Utilisation de `heap_caps_get_free_size()` pour une meilleure précision
- Ajout de `heap_caps_get_minimum_free_size()` pour tracking de la mémoire minimale
- Ajout de `heap_caps_get_largest_free_block()` pour détection de fragmentation
- Analyse de fragmentation mémoire en pourcentage
- Support amélioré de l'OPI PSRAM pour ESP32-S3

#### Améliorations WiFi
- Configuration WiFi optimisée avec `WiFi.persistent(false)`
- Auto-reconnexion activée par défaut
- Informations réseau étendues (Gateway, DNS, Subnet Mask)
- Informations sur le canal WiFi
- Meilleure détection de la qualité du signal

#### Tests système enrichis
- Raison du dernier reset (Power-on, Software, Watchdog, etc.)
- Version IDF (ESP-IDF framework)
- Révision du chip
- Vitesse de la mémoire Flash
- Statut système global (HEALTHY/WARNING/CRITICAL)

### 🔧 Modifications techniques

#### Fichiers modifiés
- `ESP32-Diagnostic.ino` : Version 3.0.1, optimisations Core 3.3.2
- `api_handlers.h` : Buffers JSON optimisés, nouvelles APIs mémoire
- `test_functions.h` : Tests mémoire avancés, analyses WiFi complètes
- `translations.h` : Pas de changements fonctionnels
- `web_interface.h` : Affichage des nouvelles métriques
- `INSTALLATION.md` : Mise à jour prérequis Core 3.3.2

#### Compatibilité
- **Minimum requis** : Arduino Core ESP32 3.3.2+
- **Testé sur** : ESP32-S3 avec 8MB Flash, OPI PSRAM
- **Compatible** : ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6, ESP32-H2

### 📊 Nouvelles métriques affichées

#### Mémoire
- Heap minimum libre (depuis le démarrage)
- Plus grand bloc libre
- Pourcentage de fragmentation
- PSRAM minimum libre (si disponible)

#### WiFi
- Canal WiFi utilisé
- Adresse Gateway
- Serveur DNS
- Masque de sous-réseau
- Qualité du signal (Excellent/Good/Fair/Weak)

#### Système
- Raison du dernier reset
- Version ESP-IDF
- Révision du chip
- Fréquence Flash
- Statut global du système

### 🐛 Corrections

- Amélioration de la gestion mémoire dans les handlers API
- Meilleure stabilité avec les buffers JSON dynamiques
- Optimisation des réponses HTTP pour NeoPixel

### 📝 Documentation

- Guide d'installation mis à jour avec Core 3.3.2
- Instructions de configuration OPI PSRAM pour ESP32-S3
- Checklist d'installation complétée
- Section dépannage enrichie

---

## [3.0.0] - 2025-10-10

### 🎉 Version initiale

#### Fonctionnalités principales
- Interface web dynamique avec design glassmorphism
- Mise à jour en temps réel (5 secondes)
- Support multilingue (Français/Anglais)
- Navigation par onglets (Vue générale, Tests, Exports)
- Tests hardware complets (GPIO, I2C, SPI, Memory, WiFi, System)
- Export JSON et CSV
- Contrôle NeoPixel
- Design responsive mobile/tablette/desktop

#### Architecture
- Structure modulaire en 5 fichiers
- API REST complète
- Système de traduction intégré
- WebServer optimisé

#### Support matériel
- ESP32-S2, ESP32-S3, ESP32-C3
- Arduino Core ESP32 3.1.3+
- PSRAM optionnel

---

## Légende des types de changements

- **⚡ Améliorations majeures** : Nouvelles fonctionnalités importantes
- **🔧 Modifications techniques** : Changements techniques internes
- **🐛 Corrections** : Résolution de bugs
- **📊 Nouvelles métriques** : Ajout de nouvelles données affichées
- **📝 Documentation** : Améliorations de la documentation
- **🔒 Sécurité** : Corrections de sécurité
- **⚠️ Déprécié** : Fonctionnalités obsolètes
- **🗑️ Supprimé** : Fonctionnalités retirées

---

## Migration depuis v3.0.0 → v3.0.1

### Étapes requises

1. **Mettre à jour Arduino Core**
   ```
   Outils > Gestionnaire de cartes > ESP32 > 3.3.2
   ```

2. **Remplacer tous les fichiers**
   - Télécharger les 5 fichiers v3.0.1
   - Remplacer dans votre projet

3. **Compiler et téléverser**
   - Vérifier les nouveaux paramètres PSRAM si ESP32-S3
   - Compiler et téléverser

4. **Vérifier le fonctionnement**
   - Moniteur série doit afficher "v3.0.1"
   - Interface web doit afficher "Arduino Core: 3.3.2"
   - Nouvelles métriques visibles dans les tests

### Changements non rétrocompatibles

⚠️ **Aucun** - La migration de 3.0.0 vers 3.0.1 est totalement compatible.

---

## Roadmap future

### v3.1.0 (Prévu)
- [ ] Graphiques temps réel de la mémoire
- [ ] Historique des métriques
- [ ] Alertes configurables
- [ ] Support capteurs (température, humidité)
- [ ] Mode dark/light
- [ ] API WebSocket pour latence réduite

### v3.2.0 (Prévu)
- [ ] Dashboard personnalisable
- [ ] Plugins pour capteurs tiers
- [ ] Export automatique planifié
- [ ] Notifications push
- [ ] Mode économie d'énergie

### v4.0.0 (Futur)
- [ ] Refonte complète avec React
- [ ] Application mobile dédiée
- [ ] Cloud sync optionnel
- [ ] Multi-ESP32 monitoring

---

*Pour signaler un bug ou suggérer une fonctionnalité, créez une issue sur le dépôt GitHub.*

*Dernière mise à jour : 16 octobre 2025*
