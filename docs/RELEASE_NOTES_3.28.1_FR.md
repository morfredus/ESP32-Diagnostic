# Notes de Version - Version 3.28.1

**Date de Sortie :** 2025-12-24
**Type :** Version Corrective (Corrections de Bugs)
**Branche :** dev/addfeatures

## Vue d'ensemble

La version 3.28.1 est une version corrective critique qui résout tous les problèmes restants de la version 3.28.0. Cette version complète l'intégration backend MISO, corrige la fonctionnalité de carte SD sur ESP32-S3, et étend la plage d'entrée MISO pour supporter toutes les broches GPIO ESP32.

## 🐛 Corrections Critiques de Bugs

### Problèmes Corrigés depuis 3.28.0

#### 1. Intégration Backend MISO TFT ✅

**Problème :**
- Champ MISO manquant dans la réponse JSON `/api/screens-info`
- L'interface web affichait "MISO: undefined" au lieu de GPIO 13
- La variable backend `tftMISO` n'était pas déclarée

**Cause Racine :**
- La variable `tftMISO` n'a jamais été initialisée depuis la constante `TFT_MISO`
- La fonction `handleScreensInfo()` n'incluait pas le champ `miso` dans le JSON des broches TFT

**Solution :**
```cpp
// src/main.cpp:261 - Ajout de la déclaration de variable
int tftMISO = TFT_MISO;

// src/main.cpp:4568 - Mise à jour de la réponse JSON
json += "\"pins\":{\"miso\":" + String(tftMISO) + ",\"mosi\":" + String(tftMOSI) + ...
```

**Impact :**
- ✅ MISO s'affiche maintenant correctement comme "MISO: 13" (ESP32-S3)
- ✅ Valeur correctement sourcée depuis `board_config.h`
- ✅ Plus de "undefined" dans l'interface web

**Fichiers Modifiés :**
- `src/main.cpp` (lignes 261, 4568)

---

#### 2. Synchronisation Configuration MISO TFT ✅

**Problème :**
- La fonction JavaScript `configTFT()` ne récupérait pas la valeur MISO depuis le champ de saisie
- Le backend `handleTFTConfig()` n'acceptait pas le paramètre MISO
- Les changements MISO dans l'UI n'avaient aucun effet

**Cause Racine :**
- Paramètre `miso` manquant dans l'appel API de configuration
- Le gestionnaire backend ne traitait pas la valeur MISO

**Solution :**
```javascript
// include/web_interface.h:119 - Mise à jour de configTFT()
const miso = document.getElementById('tftMISO').value;
// ... envoi au backend
await fetch('/api/tft-config?miso='+miso+'&mosi='+mosi+...);
```

```cpp
// src/main.cpp:3814-3828 - Mise à jour de handleTFTConfig()
int newMISO = server.hasArg("miso") ? server.arg("miso").toInt() : tftMISO;
// ... validation et application
if (newMISO >= -1 && newMISO <= 48 ...) {
  tftMISO = newMISO;
  // ...
}
```

**Impact :**
- ✅ Flux de configuration MISO complet : UI ↔ API ↔ Firmware
- ✅ Les changements MISO dans l'UI web persistent maintenant
- ✅ Validation complète (plage -1 à 48)

**Fichiers Modifiés :**
- `include/web_interface.h` (ligne 119)
- `src/main.cpp` (lignes 3814-3828, 3850-3866)

---

#### 3. Support Carte SD sur ESP32-S3 ✅

**Problème :**
- L'initialisation de la carte SD échouait sur ESP32-S3
- Les tests retournaient "not available" même avec une carte SD insérée
- Erreurs de compilation ou d'exécution sur ESP32-S3

**Cause Racine :**
- Le code utilisait la constante `HSPI` qui n'existe que sur ESP32 classique
- ESP32-S2/S3 utilisent une nomenclature de bus SPI différente (`FSPI` au lieu de `HSPI`)

**Solution :**
```cpp
// src/main.cpp:2950-2954 - Sélection conditionnelle du bus SPI
if (sdSPI == nullptr) {
#if defined(CONFIG_IDF_TARGET_ESP32)
  sdSPI = new SPIClass(HSPI);  // ESP32 classique utilise HSPI
#else
  sdSPI = new SPIClass(FSPI);  // ESP32-S2/S3 utilisent FSPI (SPI2)
#endif
}
```

**Impact :**
- ✅ Carte SD pleinement fonctionnelle sur ESP32-S3 N16R8
- ✅ Tous les endpoints de test SD fonctionnent correctement
- ✅ Abstraction appropriée des variantes ESP32
- ✅ Aucune erreur de compilation

**Référence Bus SPI :**
- **ESP32 Classic :** HSPI = Bus SPI matériel 2
- **ESP32-S2/S3 :** FSPI = Bus SPI flexible (équivalent à SPI2)

**Fichiers Modifiés :**
- `src/main.cpp` (lignes 2950-2954)

---

## Détails Techniques

### Modifications Backend

**Fichier :** `src/main.cpp`

1. **Ligne 261 :** Ajout de la déclaration de variable `int tftMISO = TFT_MISO;`
2. **Ligne 4568 :** Ajout du champ `miso` au JSON des broches TFT dans `handleScreensInfo()`
3. **Lignes 3814-3828 :** Mise à jour de `handleTFTConfig()` pour accepter et valider le paramètre MISO
4. **Lignes 2950-2954 :** Ajout de la sélection conditionnelle du bus SPI pour l'initialisation de la carte SD

**Exemple de Réponse JSON :**
```json
{
  "tft": {
    "available": true,
    "status": "Ready",
    "width": 240,
    "height": 240,
    "rotation": 0,
    "pins": {
      "miso": 13,
      "mosi": 11,
      "sclk": 12,
      "cs": 10,
      "dc": 9,
      "rst": 14,
      "bl": 7
    }
  }
}
```

### Modifications Frontend

**Fichier :** `include/web_interface.h`

**Ligne 119 :** Mise à jour de la fonction `configTFT()`
```javascript
async function configTFT() {
  setStatus('tft-status', {key:'reconfiguring'}, null);
  const miso = document.getElementById('tftMISO').value;  // NOUVEAU
  const mosi = document.getElementById('tftMOSI').value;
  // ... autres champs
  const r = await fetch('/api/tft-config?miso='+miso+'&mosi='+mosi+...);
  // ... traitement réponse
}
```

### Mise à Jour Version

**Fichier :** `platformio.ini`

```ini
build_flags =
    -std=gnu++17
    -D PROJECT_VERSION='"3.28.1"'
    -D PROJECT_NAME='"ESP32 Diagnostic"'
```

## Conformité & Architecture

### Immuabilité de board_config.h ✅
- **Exigence :** `board_config.h` doit rester intact
- **Conformité :** ✅ Toutes les valeurs proviennent des constantes de `board_config.h`
- **Validation :** ✅ Aucune valeur GPIO codée en dur
- **Statut :** ✅ CONFIRMÉ - `board_config.h` non modifié

### Support Variantes ESP32 ✅
- **Exigence :** Support ESP32 Classic, ESP32-S2, ESP32-S3
- **Implémentation :** ✅ Compilation conditionnelle avec `CONFIG_IDF_TARGET_*`
- **Abstraction SPI :** ✅ Sélection appropriée du bus par variante
- **Tests :** ✅ Vérifié sur ESP32-S3 N16R8

## Résultats des Tests

### Fonctionnalités Vérifiées
✅ **Affichage MISO :** Affiche "MISO: 13" correctement (plus de "undefined")
✅ **Configuration MISO :** Les changements dans l'UI persistent et affectent le firmware
✅ **Détection Carte SD :** Détecte et initialise correctement sur ESP32-S3
✅ **Tests Carte SD :** Endpoints lecture/écriture/formatage fonctionnels
✅ **Avertissement GPIO 13 :** Affiché correctement dans la section Carte SD
✅ **BUTTON_BOOT :** Lecture seule, aucune erreur JavaScript
✅ **Plage Entrée :** MISO accepte la plage complète GPIO (-1 à 48)

### Plateformes de Test
- ✅ ESP32-S3 N16R8 (16MB Flash + 8MB PSRAM)
- Attendu de fonctionner : ESP32-S3 N8R8, ESP32 Classic (compilation conditionnelle)

## Notes de Mise à Jour

### De 3.28.0 vers 3.28.1
- **Changements Incompatibles :** Aucun
- **Changements Matériels :** Aucun requis
- **Configuration :** Automatique - aucune action utilisateur nécessaire
- **Tests :** Vérifier la fonctionnalité de la carte SD après mise à jour

### Étapes de Migration
1. Téléverser le nouveau firmware via PlatformIO
2. Vérifier que l'affichage MISO montre GPIO 13 (pas "undefined")
3. Tester les opérations de lecture/écriture de la carte SD
4. Confirmer que la page Input Devices se charge sans erreurs

## Problèmes Connus

Aucun. Tous les problèmes de 3.28.0 sont résolus.

## Fichiers Modifiés

- `src/main.cpp`: Variable MISO, réponse JSON, gestionnaire config, bus SPI SD
- `include/web_interface.h`: Paramètre MISO dans configTFT
- `platformio.ini`: Version 3.28.0 → 3.28.1
- `CHANGELOG.md` + `CHANGELOG_FR.md`: Documentation complète 3.28.1

## Justification Versionnage Sémantique

**Version :** 3.28.1 (PATCH)
- **MAJOR :** Aucun changement incompatible → 3.x inchangé
- **MINOR :** Aucune nouvelle fonctionnalité → .28 inchangé
- **PATCH :** Corrections de bugs uniquement → .1 incrémenté

**Changements :**
- ✅ Correction affichage MISO undefined (correction de bug)
- ✅ Correction synchronisation configuration MISO (correction de bug)
- ✅ Correction carte SD sur ESP32-S3 (correction de bug)

**Conclusion :** Version PATCH stricte selon SemVer 2.0.0

---

**Version :** 3.28.1
**Version Précédente :** 3.28.0
**Version Suivante :** À déterminer
**Statut :** ✅ STABLE - Tous les problèmes connus résolus
