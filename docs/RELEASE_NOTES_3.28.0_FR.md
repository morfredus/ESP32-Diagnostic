# ESP32 Diagnostic Suite – Notes de Version v3.28.0

**Date de sortie :** 23 décembre 2025
**Type :** Version de fonctionnalités + Corrections de bugs
**Sévérité :** Améliorations majeures

---

## 📋 Vue d'ensemble

La version 3.28.0 est une **version majeure de fonctionnalités** qui ajoute une configuration complète de la broche TFT MISO, des endpoints API de test de carte SD, des avertissements de partage GPIO, et tente de corriger l'erreur JavaScript BUTTON_BOOT. Cette version améliore significativement les capacités de l'interface web pour la gestion TFT et carte SD.

**Qui doit mettre à jour :**
- ✅ Utilisateurs souhaitant configurer la broche TFT MISO via l'interface web
- ✅ Utilisateurs nécessitant une fonctionnalité de test lecture/écriture de carte SD
- ✅ Utilisateurs rencontrant des erreurs JavaScript BUTTON_BOOT (correction partielle)
- ✅ Tous les utilisateurs sur v3.27.x recherchant des fonctionnalités TFT/SD améliorées

**NOTE IMPORTANTE :**
- ⚠️ L'erreur JavaScript BUTTON_BOOT n'a PAS été entièrement corrigée dans cette version
- ⚠️ La correction a été appliquée au mauvais endroit (`web_interface.h` au lieu de `main.cpp::handleJavaScriptRoute()`)
- ✅ Voir les notes de version v3.28.2 pour la correction complète réelle

---

## 🐛 Problèmes corrigés

### ReferenceError JavaScript BUTTON_BOOT ⚠️ (CORRECTION PARTIELLE)

#### Description du problème
Lors de la navigation vers la page "Dispositifs d'Entrée" de l'interface web, la console du navigateur affichait :
```
ReferenceError: BUTTON_BOOT is not defined
```

Cette erreur empêchait le chargement correct de la page Dispositifs d'Entrée et rendait les contrôles BUTTON_BOOT, BUTTON_1 et BUTTON_2 non fonctionnels.

#### Tentative de solution (v3.28.0)
**Fichier :** `include/web_interface.h`

Injection de toutes les constantes de broches GPIO manquantes depuis `board_config.h` dans la génération JavaScript :

```javascript
// Ajout de l'injection des constantes de broches
const ROTARY_CLK_PIN = 47;   // ESP32-S3
const ROTARY_DT_PIN = 45;
const ROTARY_SW_PIN = 40;
const BUTTON_BOOT = 0;
const BUTTON_1 = 38;
const BUTTON_2 = 39;
const SD_MISO_PIN = 13;
const SD_MOSI_PIN = 11;
const SD_SCLK_PIN = 12;
const SD_CS_PIN = 1;
const TFT_MISO_PIN = 13;
```

**Configuration BUTTON_BOOT :**
- BUTTON_BOOT modifié de champ éditable à **affichage lecture seule**
- Marqué comme "(non configurable)" dans l'UI
- Préserve l'intégrité du bouton de démarrage natif ESP32 (GPIO 0)

#### Pourquoi cette correction était incomplète
**Cause racine (découverte en v3.28.2) :**
- Les constantes étaient injectées dans `web_interface.h::generateJavaScript()`
- **MAIS** cette fonction est uniquement appelée pour calculer la taille JavaScript pour les statistiques
- Le JavaScript réellement servi au navigateur provient de `main.cpp::handleJavaScriptRoute()`
- Par conséquent, les constantes injectées **n'atteignaient jamais le navigateur**

**Impact :**
- ⚠️ L'erreur JavaScript a persisté malgré la tentative de correction
- ✅ BUTTON_BOOT rendu lecture seule (changement correct)
- ❌ Les constantes de broches toujours indisponibles dans le JavaScript du navigateur

**Correction réelle :** Voir les notes de version v3.28.2 pour la solution complète

---

## 🚀 Nouvelles fonctionnalités

### 1. Configuration de la Broche TFT MISO ✅

#### Vue d'ensemble
Ajout du support complet de configuration de la broche MISO dans la section TFT, permettant aux utilisateurs de voir et configurer la broche MISO aux côtés des autres broches SPI.

#### Implémentation
**Fichier :** `include/web_interface.h`

**Amélioration de l'affichage :**
- Ajout de MISO à l'affichage des broches SPI : `MISO, MOSI, SCLK, CS, DC, RST`
- Broche MISO provenant de `board_config.h` (GPIO 13 pour ESP32-S3)
- Visibilité complète de l'interface SPI à 6 broches

**Amélioration de la configuration :**
- Ajout d'un champ de saisie MISO configurable dans la section de configuration TFT
- Les utilisateurs peuvent maintenant configurer toutes les broches SPI TFT via l'interface web
- Validation d'entrée et gestion des erreurs incluses

#### Bénéfices
- ✅ Visibilité complète des broches SPI dans l'interface web
- ✅ La broche MISO reflète correctement les valeurs de `board_config.h`
- ✅ Les utilisateurs peuvent vérifier le câblage MISO correct (GPIO 13)
- ⚠️ Fonctionnalité de configuration complétée en v3.28.1 (intégration backend)

---

### 2. Endpoints API de Test de Carte SD ✅

#### Vue d'ensemble
Ajout de trois nouveaux endpoints API pour des tests complets de carte SD, permettant aux utilisateurs de vérifier les opérations de lecture/écriture et de nettoyer les fichiers de test directement depuis l'interface web.

#### Endpoints API

**`/api/sd-test-read` – Test Lecture Carte SD**
```json
// Réponse de succès
{
  "status": "success",
  "message": "SD card read test successful",
  "file": "/test_read.txt",
  "content": "Test data..."
}

// Réponse d'erreur
{
  "status": "error",
  "message": "SD card not available"
}
```

**`/api/sd-test-write` – Test Écriture Carte SD**
```json
// Réponse de succès
{
  "status": "success",
  "message": "SD card write test successful",
  "file": "/test_write.txt",
  "timestamp": "2025-12-23 14:30:45"
}
```

**`/api/sd-format` – Nettoyage des Fichiers de Test Carte SD**
```json
// Réponse de succès
{
  "status": "success",
  "message": "SD card test files cleaned",
  "files_removed": ["/test_read.txt", "/test_write.txt"]
}
```

#### Détails d'implémentation
**Fichier :** `src/main.cpp`

**Handlers ajoutés :**
- `handleSDTestRead()` : Crée un fichier de test si nécessaire, teste la capacité de lecture
- `handleSDTestWrite()` : Teste la capacité d'écriture avec des données horodatées
- `handleSDFormat()` : Supprime tous les fichiers de test (`/test_*.txt`)

**Fonctionnalités :**
- Initialisation automatique SD si carte indisponible
- Gestion appropriée des erreurs pour tous les scénarios d'échec
- Format de réponse JSON cohérent avec les endpoints existants
- Nettoyage sécurisé (supprime uniquement les fichiers de test, pas de formatage bas niveau)

#### Cas d'usage
- ✅ Vérifier que la carte SD est correctement détectée et initialisée
- ✅ Tester les permissions et fonctionnalités de lecture/écriture
- ✅ Benchmarker les performances de la carte SD
- ✅ Résoudre les problèmes de câblage de carte SD
- ✅ Nettoyer les fichiers de test après diagnostics

---

### 3. Avertissement de Partage GPIO 13 ✅

#### Vue d'ensemble
Ajout d'un avertissement visible dans la section Carte SD pour alerter les utilisateurs que GPIO 13 est partagé entre l'écran TFT et la carte SD (ligne MISO), soulignant le besoin de câblage SPI approprié et de gestion logicielle.

#### Implémentation
**Fichier :** `include/web_interface.h`

**Avertissement visuel (Boîte d'alerte jaune) :**
```html
⚠️ GPIO 13 partagé (TFT + SD – MISO)
GPIO 13 est partagé entre TFT et carte SD (ligne MISO).
Câblage SPI strict et gestion logicielle appropriée requis.
```

**Stylisation :**
- Stylisation d'alerte type Bootstrap (fond d'avertissement jaune)
- Placement proéminent dans la section Carte SD
- Message d'avertissement clair et concis

**Traductions ajoutées :**
**Fichier :** `include/languages.h`

```cpp
// Anglais
"gpio_shared_warning": "Shared GPIO 13 (TFT + SD – MISO)"
"gpio_13_shared_desc": "GPIO 13 is shared between TFT and SD card (MISO line). Strict SPI wiring and proper software management required."

// Français
"gpio_shared_warning": "GPIO 13 partagé (TFT + SD – MISO)"
"gpio_13_shared_desc": "GPIO 13 est partagé entre TFT et carte SD (ligne MISO). Câblage SPI strict et gestion logicielle appropriée requis."
```

#### Pourquoi c'est important
**Considération matérielle :**
- TFT et carte SD utilisent tous deux GPIO 13 comme MISO (Master In Slave Out)
- MISO est une **ligne d'entrée partagée** depuis les deux dispositifs vers l'ESP32
- Nécessite une gestion appropriée du bus SPI pour éviter les conflits

**Meilleures pratiques :**
- ✅ Utiliser un câblage SPI approprié (lignes CS séparées pour TFT et SD)
- ✅ S'assurer qu'un seul dispositif est actif à la fois (contrôle CS)
- ✅ Vérifier les niveaux logiques 3.3V pour les deux dispositifs
- ⚠️ Éviter le rendu TFT et l'accès carte SD simultanés

---

## 📦 Fichiers modifiés

| Fichier | Changements | Description |
|---------|-------------|-------------|
| `include/web_interface.h` | Injection broches, affichage BOOT, avertissement GPIO, champ MISO | Constantes broches JavaScript, bouton BOOT lecture seule, boîte avertissement GPIO 13, champ configuration TFT MISO |
| `include/languages.h` | Ajout de 2 clés de traduction | `gpio_shared_warning`, `gpio_13_shared_desc` (EN/FR) |
| `src/main.cpp` | Ajout de 3 handlers d'endpoints carte SD + routes | `handleSDTestRead()`, `handleSDTestWrite()`, `handleSDFormat()` + enregistrement routes |
| `platformio.ini` | Changement version : 3.27.2 → 3.28.0 | Mise à jour version projet |

---

## ✅ Tests et vérification

### Comment vérifier les fonctionnalités

**Test 1 : Affichage TFT MISO**
1. Flasher le firmware v3.28.0 sur votre carte ESP32
2. Se connecter à l'interface web via http://esp32.local
3. Naviguer vers la page "Écrans"
4. **Vérifier** que l'affichage des broches TFT montre : MISO, MOSI, SCLK, CS, DC, RST
5. **Vérifier** que la valeur MISO est GPIO 13 (ESP32-S3) ou la valeur correcte pour votre carte

**Test 2 : Tests Carte SD**
1. Insérer une carte SD dans la carte ESP32
2. Naviguer vers "Stockage" → section Carte SD
3. **Vérifier** que l'avertissement GPIO 13 est visible (boîte d'alerte jaune)
4. Cliquer sur le bouton "Test Read"
5. **Vérifier** la réponse JSON : `{"status":"success",...}`
6. Cliquer sur le bouton "Test Write"
7. **Vérifier** que l'opération d'écriture réussit avec horodatage
8. Cliquer sur le bouton "Format"
9. **Vérifier** que les fichiers de test sont nettoyés

**Test 3 : Affichage BUTTON_BOOT**
1. Naviguer vers la page "Dispositifs d'Entrée"
2. **Vérifier** que le champ BUTTON_BOOT est en lecture seule (non éditable)
3. **Vérifier** qu'il affiche "GPIO 0 (non configurable)"
4. ⚠️ **Note :** Des erreurs JavaScript peuvent toujours survenir (correction incomplète)

### Résultats attendus
✅ La broche TFT MISO s'affiche correctement dans l'interface web
✅ Les endpoints de test carte SD retournent des réponses JSON réussies
✅ L'avertissement GPIO 13 est visible dans la section Carte SD
✅ BUTTON_BOOT est en lecture seule et marqué non configurable
⚠️ La console JavaScript peut toujours afficher ReferenceError (voir v3.28.2 pour la correction)

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
- **Carte SD :** Formatée FAT32 (optionnel, pour tests carte SD)

### Changements incompatibles
❌ Aucun – Ceci est une version de fonctionnalités, entièrement rétrocompatible

---

## 🔄 Guide de migration

### Mise à niveau depuis v3.27.x

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

**Étape 4 :** Vérifier les nouvelles fonctionnalités
- Naviguer vers http://esp32.local
- Vérifier l'affichage de la broche TFT MISO (page Écrans)
- Tester les opérations carte SD (page Stockage)
- Vérifier que BUTTON_BOOT est en lecture seule (page Dispositifs d'Entrée)

**Étape 5 (Optionnel) :** Tester la carte SD
- Insérer une carte SD (formatée FAT32)
- Exécuter les tests lecture/écriture via l'interface web
- Vérifier que l'avertissement GPIO 13 est visible

### Mise à niveau depuis v3.26.x ou antérieure

Suivre la procédure de mise à niveau standard :
1. Consulter les notes de version précédentes pour les versions intermédiaires
2. Flasher le firmware avec PlatformIO
3. Vider le cache du navigateur pour s'assurer que la dernière interface web se charge
4. Tester toutes les nouvelles fonctionnalités (TFT MISO, carte SD, BUTTON_BOOT)

---

## 🔗 Documentation associée

- [CHANGELOG_FR.md](../CHANGELOG_FR.md) – Historique complet des versions
- [RELEASE_NOTES_3.28.1_FR.md](RELEASE_NOTES_3.28.1_FR.md) – v3.28.1 Complétion backend MISO
- [RELEASE_NOTES_3.28.2_FR.md](RELEASE_NOTES_3.28.2_FR.md) – v3.28.2 Correction réelle BUTTON_BOOT
- [PIN_MAPPING_FR.md](PIN_MAPPING_FR.md) – Assignation des broches GPIO
- [TROUBLESHOOTING_FR.md](TROUBLESHOOTING_FR.md) – Problèmes courants et solutions

---

## ❓ FAQ

### Q : Cette version corrige-t-elle entièrement l'erreur BUTTON_BOOT ?
**R :** Non, la correction était incomplète. Les constantes ont été injectées au mauvais endroit. Mettez à niveau vers v3.28.2 pour la correction réelle.

### Q : Pourquoi BUTTON_BOOT est-il en lecture seule ?
**R :** BUTTON_BOOT est le bouton de démarrage natif ESP32 (GPIO 0), qui est critique pour le flashage du firmware et la sélection du mode de démarrage. Le rendre lecture seule empêche une mauvaise configuration accidentelle.

### Q : Que signifie l'avertissement GPIO 13 ?
**R :** GPIO 13 sert de ligne MISO pour TFT et carte SD. C'est normal et sûr, mais nécessite un câblage SPI approprié (lignes CS séparées) pour éviter les conflits.

### Q : L'endpoint de formatage carte SD effacera-t-il toutes mes données ?
**R :** Non, `/api/sd-format` supprime uniquement les fichiers de test créés par la suite de diagnostic (`/test_*.txt`). Il ne réalise PAS de formatage bas niveau ni n'efface les données utilisateur.

### Q : Puis-je utiliser la carte SD pendant que le TFT est actif ?
**R :** Oui, mais pas simultanément. Le firmware gère l'accès au bus SPI en utilisant des lignes CS (Chip Select) séparées. La ligne MISO partagée est correctement gérée par le protocole SPI.

### Q : Pourquoi vois-je toujours des erreurs JavaScript après la mise à niveau ?
**R :** La correction BUTTON_BOOT en v3.28.0 était incomplète. Mettez à niveau vers v3.28.2 pour la correction complète, puis videz le cache de votre navigateur.

---

## 🔍 Conformité

### Immutabilité de board_config.h
- ✅ Toutes les définitions GPIO proviennent des constantes `board_config.h`
- ✅ Aucune valeur de broche codée en dur dans le JavaScript ou l'implémentation C++
- ✅ Respecte `board_config.h` comme source unique de vérité
- ✅ Aucune modification de `board_config.h` lui-même (comme requis)

### Injection des Constantes de Broches
- ⚠️ Emplacement d'injection incorrect (web_interface.h au lieu de main.cpp)
- ✅ Toutes les constantes proviennent de board_config.h immuable
- ✅ Aucune valeur codée en dur utilisée
- ❌ Les constantes n'ont pas atteint le navigateur (corrigé en v3.28.2)

---

## 📝 Crédits

**Signalé par :** Tests communautaires, retours d'intégration interface web
**Développé par :** Équipe de développement ESP32-Diagnostic
**Testé sur :** ESP32-S3 DevKitC-1 N16R8, ESP32-S3 N8R8, ESP32 Classic DevKitC

---

## 🔖 Informations de version

- **Version actuelle :** 3.28.0
- **Version précédente :** 3.27.2
- **Version suivante :** 3.28.1 (complétion backend MISO)
- **Branche de release :** `dev/addfeatures`
- **Tag :** `v3.28.0`

---

## 🎯 À venir

**v3.28.1 (Prochainement) :**
- ✅ Complétion de l'intégration backend TFT MISO
- ✅ Synchronisation de configuration MISO (UI → API → Firmware)
- ✅ Corrections support carte SD pour ESP32-S3 (HSPI vs FSPI)

**v3.28.2 (Prochainement) :**
- ✅ Correction complète de l'erreur JavaScript BUTTON_BOOT
- ✅ Injection des constantes de broches au bon endroit

---

**Pour le journal des modifications complet, voir [CHANGELOG_FR.md](../CHANGELOG_FR.md)**
