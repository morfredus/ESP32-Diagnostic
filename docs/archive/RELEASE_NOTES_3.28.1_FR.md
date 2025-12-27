# ESP32 Diagnostic Suite – Notes de Version v3.28.1

**Date de sortie :** 24 décembre 2025
**Type :** Version de correction
**Sévérité :** Corrections de bugs critiques

---

## 📋 Vue d'ensemble

La version 3.28.1 est une **version de correction** qui résout trois problèmes critiques d'intégration : l'intégration backend de TFT MISO, la synchronisation de configuration MISO, et le support de carte SD sur ESP32-S3. Cette version assure une gestion complète de la broche MISO sur toute la pile logicielle et résout les problèmes de compatibilité du bus SPI sur les cartes ESP32-S3.

**Qui doit mettre à jour :**
- ✅ Utilisateurs rencontrant "MISO: undefined" dans la section TFT de l'interface web
- ✅ Utilisateurs incapables de configurer la broche TFT MISO via l'interface web
- ✅ Utilisateurs ESP32-S3 rencontrant des échecs d'initialisation de carte SD
- ✅ Tous les utilisateurs sur v3.28.0 (implémentation MISO partielle)

**NOTE IMPORTANTE :**
- ⚠️ L'erreur JavaScript BUTTON_BOOT n'a PAS été entièrement corrigée dans cette version malgré les déclarations initiales
- ✅ Voir les notes de version v3.28.2 pour la correction réelle de BUTTON_BOOT

---

## 🐛 Problèmes corrigés

### 1. Intégration Backend TFT MISO ✅

#### Description du problème
La valeur de la broche TFT MISO n'était pas incluse dans la réponse JSON de `/api/screens-info`, ce qui faisait que l'interface web affichait "MISO: undefined" dans la section d'informations des broches TFT.

#### Cause racine
- Le backend manquait de l'initialisation de la variable `tftMISO`
- Le constructeur de réponse JSON dans `handleScreensInfo()` n'incluait pas le champ MISO
- Le frontend recevait des données de broches incomplètes : `{pins: {mosi: 11, sclk: 12, ...}}` (MISO manquant)

#### Solution technique
**Fichier :** `src/main.cpp`
**Changements :**
- **Ligne 261 :** Ajout de la déclaration de variable `int tftMISO = TFT_MISO;`
- **Ligne 4568 :** Ajout du champ `miso` à la structure JSON des broches TFT

```cpp
// Initialisation de variable
int tftMISO = TFT_MISO;

// Réponse JSON dans handleScreensInfo()
"pins": {
  "miso": 13,    // ← Ajouté en v3.28.1
  "mosi": 11,
  "sclk": 12,
  "cs": 10,
  "dc": 9,
  "rst": 14
}
```

#### Impact et bénéfices
- ✅ La broche TFT MISO s'affiche maintenant correctement dans l'interface web (GPIO 13 pour ESP32-S3)
- ✅ Informations complètes sur les broches SPI disponibles : MISO, MOSI, SCLK, CS, DC, RST
- ✅ Résout le problème d'affichage "MISO: undefined"

---

### 2. Synchronisation de Configuration TFT MISO ✅

#### Description du problème
La fonction JavaScript `configTFT()` n'envoyait pas la valeur de la broche MISO à l'API backend, empêchant les utilisateurs de configurer la broche TFT MISO via l'interface web.

#### Cause racine
- La fonction frontend `configTFT()` récupérait toutes les broches SPI SAUF MISO
- Le backend `handleTFTConfig()` n'acceptait ni ne validait le paramètre MISO
- Le flux de configuration était incomplet : UI → API (paramètre MISO manquant)

#### Solution technique
**Fichier :** `include/web_interface.h`
**Ligne :** 119
```javascript
// Mise à jour de configTFT() pour inclure la valeur MISO
const miso = document.getElementById('tftMISO').value;
// L'appel API inclut maintenant le paramètre MISO
fetch(`/api/tft-config?miso=${miso}&mosi=${mosi}&sclk=${sclk}&...`)
```

**Fichier :** `src/main.cpp`
**Lignes :** 3814-3828
```cpp
// Mise à jour de handleTFTConfig() pour accepter et valider le paramètre MISO
if (request->hasParam("miso")) {
  int miso = request->getParam("miso")->value().toInt();
  // Valider et traiter la valeur MISO
}
```

#### Impact et bénéfices
- ✅ Le paramètre MISO est correctement inclus dans les requêtes `/api/tft-config`
- ✅ Le backend accepte et valide maintenant la configuration MISO
- ✅ Complète le flux de configuration MISO complet : UI ↔ API ↔ Firmware
- ✅ Les utilisateurs peuvent maintenant configurer toutes les broches SPI TFT via l'interface web

---

### 3. Support de Carte SD sur ESP32-S3 ✅

#### Description du problème
L'initialisation de la carte SD échouait sur les cartes ESP32-S3 avec des erreurs de compilation/exécution. Les fonctionnalités de test de carte SD étaient complètement non fonctionnelles sur les variantes ESP32-S3 N16R8 et N8R8.

#### Cause racine
Le code utilisait la constante `HSPI` pour la sélection du bus SPI, qui est **disponible uniquement sur ESP32 Classic** :

```cpp
// INCORRECT - Échoue sur ESP32-S3
SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
if (!SD.begin(SD_CS, SPI, 25000000, "/sd", 5, false)) {
  // Erreur de compilation sur ESP32-S3 : HSPI non défini
}
```

Différences d'architecture ESP32 :
- **ESP32 Classic :** Dispose de `HSPI` (Hardware SPI bus 2) et `VSPI` (bus 3)
- **ESP32-S2/S3 :** Utilise `FSPI` (Flexible SPI, équivalent à SPI2) au lieu de HSPI/VSPI

#### Solution technique
**Fichier :** `src/main.cpp`
**Lignes :** 2950-2954

Implémentation de la sélection conditionnelle du bus SPI :

```cpp
#if defined(CONFIG_IDF_TARGET_ESP32)
  // ESP32 Classic : Utiliser HSPI (Hardware SPI bus 2)
  SPIClass spiSD(HSPI);
#else
  // ESP32-S2/S3 : Utiliser FSPI (Flexible SPI bus, équivalent à SPI2)
  SPIClass spiSD(FSPI);
#endif

spiSD.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
if (!SD.begin(SD_CS, spiSD, 25000000, "/sd", 5, false)) {
  // Fonctionne maintenant sur toutes les variantes ESP32
}
```

#### Impact et bénéfices
- ✅ L'initialisation de la carte SD fonctionne maintenant sur ESP32-S3 N16R8/N8R8
- ✅ Les tests de carte SD (`/api/sd-test-read`, `/api/sd-test-write`, `/api/sd-format`) sont entièrement fonctionnels
- ✅ Abstraction correcte du bus SPI pour la compatibilité des variantes ESP32
- ✅ Aucune erreur de compilation sur les cartes ESP32-S2/S3

---

## 📦 Fichiers modifiés

| Fichier | Changements | Lignes |
|---------|-------------|--------|
| `src/main.cpp` | Ajout de la déclaration de variable tftMISO | 261 |
| `src/main.cpp` | Ajout du champ MISO à la réponse JSON des broches TFT | 4568 |
| `src/main.cpp` | Mise à jour de handleTFTConfig() pour accepter le paramètre MISO | 3814-3828 |
| `src/main.cpp` | Ajout de la sélection conditionnelle du bus SPI pour carte SD | 2950-2954 |
| `include/web_interface.h` | Mise à jour de configTFT() pour inclure le paramètre MISO | 119 |
| `platformio.ini` | Changement de version : 3.28.0 → 3.28.1 | 27 |

---

## ✅ Tests et vérification

### Comment vérifier les corrections

**Test 1 : Affichage TFT MISO**
1. Flasher le firmware v3.28.1 sur votre carte ESP32
2. Se connecter à l'interface web via http://esp32.local
3. Naviguer vers la page "Écrans"
4. **Vérifier** que le champ TFT MISO affiche "GPIO 13" (ESP32-S3) au lieu de "undefined"

**Test 2 : Configuration TFT MISO**
1. Naviguer vers "Écrans" → section Configuration TFT
2. Modifier la valeur de la broche MISO dans le champ de saisie
3. Cliquer sur "Configurer TFT"
4. **Vérifier** que la requête de configuration réussit sans erreurs
5. **Vérifier** la console du navigateur (F12) pour la réponse API réussie

**Test 3 : Carte SD sur ESP32-S3**
1. Insérer une carte SD dans la carte ESP32-S3
2. Naviguer vers "Stockage" → section Carte SD
3. Cliquer sur le bouton "Test Read"
4. **Vérifier** que l'opération réussit avec une réponse JSON : `{"status":"success",...}`
5. Cliquer sur le bouton "Test Write"
6. **Vérifier** que l'opération d'écriture réussit avec des données horodatées

### Résultats attendus
✅ TFT MISO affiche "GPIO 13" (pas "undefined")
✅ La configuration TFT accepte le paramètre MISO sans erreurs
✅ Les tests de carte SD fonctionnent sur les cartes ESP32-S3 N16R8/N8R8
✅ Aucune erreur de compilation lors de la construction pour la cible ESP32-S3
✅ La console du navigateur affiche des réponses API réussies

---

## 📌 Compatibilité

### Cartes supportées
- ✅ ESP32-S3 DevKitC-1 N16R8 (16MB Flash, 8MB PSRAM)
- ✅ ESP32-S3 DevKitC-1 N8R8 (8MB Flash, 8MB PSRAM)
- ✅ ESP32 Classic DevKitC (4MB Flash)

### Prérequis
- **PlatformIO :** espressif32@^6
- **ESP32 Arduino Core :** 3.0.0+
- **Navigateur :** Chrome 90+, Firefox 88+, Edge 90+, Safari 14+

### Changements incompatibles
❌ Aucun – Ceci est une version de correction de bug, entièrement rétrocompatible

---

## 🔄 Guide de migration

### Mise à niveau depuis v3.28.0

**Étape 1 :** Récupérer le dernier code
```bash
git pull origin dev/addfeatures
```

**Étape 2 :** Compilation propre (recommandé)
```bash
pio run -t clean
pio run -e esp32s3_n16r8
```

**Étape 3 :** Téléverser vers l'ESP32
```bash
pio run -e esp32s3_n16r8 --target upload
```

**Étape 4 :** Vérifier l'interface web
- Naviguer vers http://esp32.local
- Vérifier que TFT MISO affiche la valeur GPIO (pas "undefined")
- Tester les opérations de lecture/écriture de carte SD (utilisateurs ESP32-S3)

### Mise à niveau depuis v3.27.x ou antérieure

Suivre la procédure de mise à niveau standard :
1. Consulter `docs/RELEASE_NOTES_3.28.0.md` pour les changements v3.28.0
2. Consulter ce document pour les changements v3.28.1
3. Note : l'erreur BUTTON_BOOT est toujours présente – voir v3.28.2 pour la correction
4. Flasher le firmware avec PlatformIO

---

## 🔗 Documentation associée

- [CHANGELOG_FR.md](../CHANGELOG_FR.md) – Historique complet des versions
- [RELEASE_NOTES_3.28.0_FR.md](RELEASE_NOTES_3.28.0_FR.md) – v3.28.0 Implémentation initiale TFT MISO
- [RELEASE_NOTES_3.28.2_FR.md](RELEASE_NOTES_3.28.2_FR.md) – v3.28.2 Correction réelle de BUTTON_BOOT
- [TROUBLESHOOTING_FR.md](TROUBLESHOOTING_FR.md) – Problèmes courants et solutions
- [PIN_MAPPING_FR.md](PIN_MAPPING_FR.md) – Assignation des broches GPIO

---

## ❓ FAQ

### Q : Cela corrige-t-il l'erreur JavaScript BUTTON_BOOT ?
**R :** Non, l'erreur BUTTON_BOOT n'a PAS été entièrement corrigée en v3.28.1. Mettez à niveau vers v3.28.2 pour la correction réelle.

### Q : Pourquoi la carte SD était-elle cassée sur ESP32-S3 ?
**R :** L'ESP32-S3 utilise `FSPI` au lieu de `HSPI` pour le bus SPI 2. Le code était codé en dur pour la constante `HSPI` de l'ESP32 Classic.

### Q : Cela affectera-t-il ma configuration TFT existante ?
**R :** Non, ceci est rétrocompatible. Les configurations existantes continueront de fonctionner, avec MISO maintenant correctement affiché et configurable.

### Q : Que faire si MISO affiche toujours "undefined" après la mise à niveau ?
**R :** Videz le cache de votre navigateur (Ctrl+Maj+Suppr) et rechargez la page. Si le problème persiste, vérifiez la console du navigateur pour les erreurs API.

### Q : Cette correction s'applique-t-elle à la fois à ESP32-S3 et ESP32 Classic ?
**R :** Oui, les trois corrections s'appliquent à toutes les variantes de cartes supportées. La correction de carte SD résout spécifiquement la compatibilité ESP32-S3.

---

## 🔍 Conformité

### Immutabilité de board_config.h
- ✅ Toutes les valeurs GPIO proviennent des constantes `board_config.h`
- ✅ Aucune valeur de broche codée en dur dans l'implémentation
- ✅ Maintient `board_config.h` comme source unique de vérité
- ✅ Aucune modification de `board_config.h` lui-même (comme requis)

### Abstraction du Bus SPI
- ✅ Compilation conditionnelle appropriée pour les variantes ESP32
- ✅ Utilise `CONFIG_IDF_TARGET_ESP32` pour la détection de plateforme
- ✅ Couche d'abstraction pour les différences HSPI/FSPI

---

## 📝 Crédits

**Signalé par :** Tests communautaires, tests d'intégration de l'interface web
**Corrigé par :** Équipe de développement ESP32-Diagnostic
**Testé sur :** ESP32-S3 DevKitC-1 N16R8, ESP32-S3 N8R8, ESP32 Classic DevKitC

---

## 🔖 Informations de version

- **Version actuelle :** 3.28.1
- **Version précédente :** 3.28.0
- **Version suivante :** 3.28.2 (correction réelle de BUTTON_BOOT)
- **Branche de release :** `dev/addfeatures`
- **Tag :** `v3.28.1`

---

**Pour le journal des modifications complet, voir [CHANGELOG_FR.md](../CHANGELOG_FR.md)**
