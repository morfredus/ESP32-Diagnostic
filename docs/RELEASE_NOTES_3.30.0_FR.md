# Notes de Version - Version 3.30.0

**Date de Sortie :** 25 décembre 2025
**Type :** Version Mineure (Ajout de Fonctionnalité)
**Statut :** ✅ Stable

---

## 🎯 Vue d'Ensemble

La version 3.30.0 introduit **le changement de driver TFT en temps réel** - une amélioration majeure qui permet aux utilisateurs de basculer dynamiquement entre les drivers d'affichage TFT ILI9341 et ST7789 directement depuis l'interface Web sans recompiler ni reflasher le firmware.

---

## ✨ Nouvelles Fonctionnalités

### Sélection Dynamique du Driver TFT (Interface Web)

**Nouveautés :**
- **Changement de driver en temps réel** : Basculez entre les drivers TFT (ILI9341 ↔ ST7789) depuis l'interface Web
- **Aucun redémarrage requis** : Changement instantané sans redémarrage de l'ESP32
- **Support dual driver** : Les deux drivers chargés simultanément en mémoire
- **Intégration transparente** : Nouveau sélecteur dans la section de configuration TFT
- **Sélection persistante** : Choix du driver affiché en temps réel

**Avantages :**
- ⚡ **Tests instantanés** : Testez différents contrôleurs TFT sans reflasher
- 🔄 **Flexibilité matérielle** : Échangez les écrans à la volée pour le prototypage
- 🧪 **Dépannage facile** : Identifiez rapidement les problèmes de compatibilité
- 📦 **Firmware unique** : Une seule compilation supporte les deux types d'écran

---

## 🔧 Modifications Techniques

### Architecture Backend

**Fichier : `include/tft_display.h`**

Refactorisation complète pour le support runtime :

```cpp
// Nouvelle énumération de type de driver
enum TFT_DriverType {
  TFT_DRIVER_ILI9341 = 0,
  TFT_DRIVER_ST7789 = 1
};

// Objets drivers dynamiques (tous deux instanciés)
Adafruit_ILI9341* tft_ili9341 = nullptr;
Adafruit_ST7789* tft_st7789 = nullptr;

// Pointeur générique pour accès unifié
Adafruit_GFX* tft = nullptr;

// Changement de driver en runtime
bool switchTFTDriver(TFT_DriverType newDriver, int width, int height, int rotation);
```

**Fonctions Clés :**
- `initTFT(driverType, width, height, rotation)` - Initialisation dynamique
- `switchTFTDriver()` - Changement de driver à la volée
- `deinitTFT()` - Nettoyage propre avant changement
- `getTFTDriverName()` - Identification du driver actuel

### Améliorations API

**Fichier : `src/main.cpp`**

Nouvelle variable globale :
```cpp
String tftDriver = "ILI9341";  // Suit le driver actif
```

Endpoints enrichis :

**`/api/tft-config` (Modifié)**
- **Nouveau paramètre** : `driver` (ILI9341 | ST7789)
- **Comportement** : Change le driver dynamiquement si paramètre `driver` présent
- **Réponse** : Inclut le driver actuel dans la réponse de succès

**`/api/screens-info` (Modifié)**
- **Nouveau champ** : `"driver"` dans l'objet TFT
- **Retourne** : Type de driver actuellement actif

### Mises à Jour Interface Web

**Fichier : `include/web_interface.h`**

Nouvel élément UI dans la configuration TFT :
```html
<select id="tftDriver">
  <option value="ILI9341">ILI9341</option>
  <option value="ST7789">ST7789</option>
</select>
```

JavaScript mis à jour :
```javascript
async function configTFT() {
  const driver = document.getElementById('tftDriver').value;
  // Envoie le paramètre driver à l'API
  const r = await fetch('/api/tft-config?driver=' + driver + '&...');
}
```

---

## 📝 Configuration

### Sélection du Driver par Défaut

Éditez `include/config.h` :

```cpp
// v3.30.0 : Sélection dynamique du driver TFT
// Les deux drivers supportés simultanément
#define TFT_USE_ILI9341  // Par défaut au démarrage
// #define TFT_USE_ST7789

#define TFT_WIDTH   240
#define TFT_HEIGHT  320
#define TFT_ROTATION 2
```

**Note :** Le `#define TFT_USE_*` définit uniquement le **driver par défaut au démarrage**. Vous pouvez changer dynamiquement via l'interface Web après le boot.

---

## 🚀 Guide d'Utilisation

### Comment Changer de Driver depuis l'Interface Web

1. **Accéder à l'Interface Web**
   - Naviguez vers l'adresse IP de l'ESP32 dans votre navigateur

2. **Ouvrir la Section Écran TFT**
   - Faites défiler jusqu'à la section "📱 TFT Screen"

3. **Sélectionner le Driver**
   - Trouvez le menu déroulant "Driver:"
   - Sélectionnez le driver désiré :
     - `ILI9341` (240x320)
     - `ST7789` (240x320)

4. **Appliquer la Configuration**
   - Cliquez sur le bouton "⚙️ Appliquer"
   - Le driver change **instantanément** (pas de redémarrage)

5. **Tester l'Affichage**
   - Utilisez les boutons de test intégrés :
     - "🧪 Test Complet"
     - "🏠 Écran de Démarrage"
     - Tests par étapes individuels

6. **Vérifier le Driver Actif**
   - Nom du driver affiché dans la section de configuration
   - L'écran répond selon le driver sélectionné

---

## 📊 Compatibilité

### Support Matériel

| Contrôleur TFT | Résolution | Statut | Notes |
|----------------|------------|--------|-------|
| **ILI9341** | 240x320 | ✅ Entièrement Supporté | SPI, Hardware SPI |
| **ST7789** | 240x320 | ✅ Entièrement Supporté | SPI, Hardware SPI |

### Mappage des Broches

Les deux drivers utilisent le **même mappage de broches** depuis `board_config.h` :

```cpp
#define TFT_MISO  13
#define TFT_MOSI  11
#define TFT_SCLK  12
#define TFT_CS    10
#define TFT_DC     9
#define TFT_RST   14
#define TFT_BL     7
```

**Les broches ILI9341** sont des alias vers les **broches TFT** - aucun câblage séparé nécessaire.

---

## 🔄 Migration depuis v3.29.0

### Changements Cassants

**Aucun** - Entièrement rétrocompatible.

### Modifications de Configuration

**Avant (v3.29.0) :**
```cpp
// Devait choisir UN driver à la compilation
#define TFT_USE_ILI9341
//#define TFT_USE_ST7789
```

**Après (v3.30.0) :**
```cpp
// Les deux drivers chargés, changement via Web UI
#define TFT_USE_ILI9341  // Par défaut uniquement
// #define TFT_USE_ST7789
```

### Modifications du Code

**Si vous avez du code personnalisé utilisant l'objet `tft` :**

**Avant :**
```cpp
tft.fillScreen(TFT_BLACK);  // Objet direct
```

**Après :**
```cpp
tft->fillScreen(TFT_BLACK);  // Syntaxe pointeur
```

**Raison :** `tft` est maintenant un pointeur générique `Adafruit_GFX*` pour un accès unifié aux drivers.

---

## 🐛 Problèmes Connus

### Aucun Signalé

La version 3.30.0 a été testée avec :
- ✅ ESP32-S3 DevKitC-1 N16R8
- ✅ ILI9341 2.8" TFT (240x320)
- ✅ ST7789 2.4" TFT (240x320)

---

## 📚 Mises à Jour Documentation

Toute la documentation mise à jour pour v3.30.0 :

- ✅ **README.md / README_FR.md** - Points forts version 3.30.0
- ✅ **CHANGELOG.md / CHANGELOG_FR.md** - Notes de version complètes
- ✅ **CONFIG.md / CONFIG_FR.md** - Guide configuration TFT
- ✅ **USAGE.md / USAGE_FR.md** - Utilisation changement runtime
- ✅ **WEB_INTERFACE.md / WEB_INTERFACE_FR.md** - Mises à jour UI
- ✅ **API_REFERENCE.md / API_REFERENCE_FR.md** - Nouveaux paramètres API

---

## 🎯 Améliorations Futures

Améliorations potentielles pour les versions futures :

- Support de contrôleurs TFT additionnels (ILI9486, ST7735, etc.)
- Auto-détection de la résolution d'affichage
- Paramètres de calibration spécifiques aux drivers
- Changement runtime du contrôleur tactile

---

## 📞 Support

**Issues :** https://github.com/morfredus/ESP32-Diagnostic/issues
**Documentation :** Dossier `/docs/`
**Communauté :** GitHub Discussions

---

## ✅ Liste de Vérification Tests

Avant de déployer v3.30.0, vérifiez :

- [ ] Compilation réussie pour ESP32-S3 et ESP32 Classic
- [ ] Driver par défaut s'initialise correctement
- [ ] Interface Web affiche le sélecteur de driver
- [ ] Changement de driver fonctionne (ILI9341 → ST7789)
- [ ] Changement de driver fonctionne (ST7789 → ILI9341)
- [ ] Tests d'affichage fonctionnent avec les deux drivers
- [ ] Pas de fuites mémoire lors des changements
- [ ] Configuration persiste entre les changements

---

**Version :** 3.30.0
**Auteur :** morfredus
**Licence :** Voir fichier LICENSE
**Page d'accueil :** https://github.com/morfredus/ESP32-Diagnostic

---

*Pour l'historique complet des versions, voir [CHANGELOG_FR.md](../CHANGELOG_FR.md)*
