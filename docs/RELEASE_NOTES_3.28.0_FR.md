# Notes de Version - Version 3.28.0

**Date de Sortie :** 2025-12-23
**Type :** Version de Fonctionnalités
**Branche :** dev/addfeatures

## Vue d'ensemble

La version 3.28.0 introduit des corrections critiques pour les constantes GPIO JavaScript et complète l'intégration de la broche MISO TFT dans l'interface web. Cette version ajoute également des endpoints de test complets pour la carte SD et des avertissements appropriés sur le partage du GPIO 13 pour les périphériques SPI.

## 🚀 Nouvelles Fonctionnalités & Corrections

### Corrigé

#### Erreur JavaScript BUTTON_BOOT ✅
- **Problème :** `ReferenceError: BUTTON_BOOT is not defined` lors de l'accès à la page Input Devices
- **Cause Racine :** Les constantes de broches GPIO depuis `board_config.h` n'étaient pas injectées dans le contexte JavaScript
- **Solution :**
  - Injection de toutes les constantes de broches manquantes depuis `board_config.h` vers JavaScript
  - Ajouté : `ROTARY_CLK_PIN`, `ROTARY_DT_PIN`, `ROTARY_SW_PIN`
  - Ajouté : `BUTTON_BOOT`, `BUTTON_1`, `BUTTON_2`
  - Ajouté : `SD_MISO_PIN`, `SD_MOSI_PIN`, `SD_SCLK_PIN`, `SD_CS_PIN`
  - Ajouté : `TFT_MISO_PIN`, `BUZZER_PIN`
- **Impact :** La page Input Devices se charge maintenant sans erreurs
- **Fichiers Modifiés :** `include/web_interface.h` (lignes 477-498)

#### Configuration BUTTON_BOOT ✅
- **Problème :** BUTTON_BOOT était éditable dans l'UI, risquant la fonctionnalité de démarrage
- **Solution :**
  - Changé d'un input éditable à un affichage en lecture seule
  - Marqué "(non configurable)" dans l'UI avec indicateur visuel
  - Préserve l'intégrité du bouton boot natif ESP32 (GPIO 0)
- **Impact :** Empêche la mauvaise configuration accidentelle du bouton boot critique
- **Fichiers Modifiés :** `include/web_interface.h` (ligne 98)

### Ajouté

#### Configuration Broche MISO TFT ✅
- **Fonctionnalité :** Support complet de la broche MISO dans la configuration SPI TFT
- **Affichage :** Les broches SPI affichent maintenant : `MISO`, `MOSI`, `SCLK`, `CS`, `DC`, `RST`
- **Configuration :** Ajout d'un champ de saisie MISO configurable dans les paramètres TFT
- **Valeur Par Défaut :** GPIO 13 (depuis `board_config.h` pour ESP32-S3)
- **Plage :** -1 à 9 (limité en v3.28.0, étendu à -1 à 48 en v3.28.1)
- **Fichiers Modifiés :** `include/web_interface.h` (ligne 90)

#### Endpoints API Test Carte SD ✅
Trois nouveaux endpoints REST pour des tests complets de carte SD :

1. **`/api/sd-test-read`** - Test des opérations de lecture carte SD
   - Crée un fichier de test si nécessaire
   - Lit et valide le contenu
   - Retourne le statut succès/échec

2. **`/api/sd-test-write`** - Test des opérations d'écriture carte SD
   - Écrit des données horodatées
   - Valide le succès de l'écriture
   - Retourne le statut de l'opération

3. **`/api/sd-format`** - Nettoyage des fichiers de test carte SD
   - Supprime `/test_esp32.txt`, `/test_read.txt`, `/test_write.txt`
   - Nettoyage sécurisé (pas formatage bas niveau)
   - Retourne le statut du nettoyage

**Fonctionnalités :**
- Initialisation automatique de la SD si indisponible
- Format de réponse JSON cohérent avec les endpoints existants
- Gestion appropriée des erreurs pour cartes SD manquantes
- Retourne le statut `"not available"` si la carte SD n'est pas détectée

**Fichiers Modifiés :** `src/main.cpp` (lignes 4185-4291, 5688-5693)

#### Avertissement Partage GPIO 13 ✅
- **Fonctionnalité :** Avertissement proéminent sur l'utilisation partagée du GPIO 13
- **Localisation :** Section Carte SD dans l'interface web
- **Style :** Boîte d'avertissement jaune avec style d'alerte type Bootstrap
- **Contenu :** Alerte les utilisateurs que le GPIO 13 est partagé entre MISO TFT et MISO SD
- **Langues :** Support bilingue complet (EN/FR)
- **Nouvelles Clés de Traduction :**
  - `gpio_shared_warning`: "GPIO 13 partagé (TFT + SD – MISO)"
  - `gpio_13_shared_desc`: Explication détaillée des exigences de partage SPI
- **Fichiers Modifiés :**
  - `include/web_interface.h` (ligne 99)
  - `include/languages.h` (lignes 486-487)

## Détails Techniques

### Modifications Interface Web
**Fichier :** `include/web_interface.h`

**Injection Broches GPIO** (lignes 477-498) :
```javascript
// Buzzer
js += F(";const BUZZER_PIN=");
js += String(BUZZER);

// Encodeur Rotatif
js += F(";const ROTARY_CLK_PIN=");
js += String(ROTARY_CLK);
js += F(";const ROTARY_DT_PIN=");
js += String(ROTARY_DT);
js += F(";const ROTARY_SW_PIN=");
js += String(ROTARY_SW);

// Boutons
js += F(";const BUTTON_BOOT=");
js += String(BUTTON_BOOT);
js += F(";const BUTTON_1=");
js += String(BUTTON_1);
js += F(";const BUTTON_2=");
js += String(BUTTON_2);

// Carte SD
js += F(";const SD_MISO_PIN=");
js += String(SD_MISO);
// ... MOSI, SCLK, CS

// TFT
js += F(";const TFT_MISO_PIN=");
js += String(TFT_MISO);
```

**Affichage BUTTON_BOOT Lecture Seule** (ligne 98) :
```javascript
h+='<div class="info-value">GPIO '+BUTTON_BOOT+' <span style="font-size:0.8em;color:#666">(non configurable)</span></div></div>';
```

**Avertissement GPIO 13** (ligne 99) :
```javascript
h+='<p style="margin-top:10px;padding:10px;background:#fff3cd;border-left:4px solid #ffc107;color:#856404;border-radius:4px"><strong>⚠️ '+tr('gpio_shared_warning')+'</strong><br>'+tr('gpio_13_shared_desc')+'</p>';
```

### Implémentation API
**Fichier :** `src/main.cpp`

**Nouveaux Gestionnaires Endpoints** (lignes 4185-4291) :
- `handleSDTestRead()`: Crée et lit un fichier de test, retourne le statut JSON
- `handleSDTestWrite()`: Écrit des données horodatées, retourne le statut JSON
- `handleSDFormat()`: Supprime les fichiers de test, retourne le statut JSON

**Enregistrement Routes** (lignes 5688-5693) :
```cpp
server.on("/api/sd-test-read", handleSDTestRead);
server.on("/api/sd-test-write", handleSDTestWrite);
server.on("/api/sd-format", handleSDFormat);
```

### Traductions
**Fichier :** `include/languages.h` (lignes 486-487)

```cpp
X(gpio_shared_warning, "Shared GPIO 13 (TFT + SD – MISO)", "GPIO 13 partagé (TFT + SD – MISO)") \
X(gpio_13_shared_desc, "GPIO 13 is shared between the TFT and SD Card (MISO line). Strictly compliant SPI wiring and proper software management are required.", "Le GPIO 13 est partagé entre le TFT et la carte SD (ligne MISO). Un câblage SPI strictement conforme et une gestion logicielle adaptée sont requis.") \
```

## Conformité & Architecture

### Immuabilité de board_config.h ✅
- **Exigence :** `board_config.h` doit rester intact (source unique de vérité)
- **Conformité :** ✅ Toutes les valeurs GPIO proviennent des constantes de `board_config.h`
- **Validation :** ✅ Aucune valeur de broche codée en dur dans JavaScript
- **Architecture :** ✅ Constantes JavaScript injectées depuis les defines C++ au runtime

### Qualité du Code
- Support bilingue complet maintenu (EN/FR)
- Format de réponse API cohérent
- Gestion appropriée des erreurs
- Séparation claire des préoccupations

## Limitations Connues (Corrigées en 3.28.1)

1. **Intégration Backend MISO :** Le champ MISO n'est pas encore inclus dans la réponse JSON `/api/screens-info` (s'affiche comme "undefined")
2. **Configuration MISO :** La valeur MISO n'est pas envoyée au backend lors de la configuration TFT
3. **Carte SD sur ESP32-S3 :** L'initialisation de la carte SD échoue sur ESP32-S3 en raison d'une incompatibilité HSPI/FSPI

**Note :** Les trois problèmes ci-dessus sont résolus dans la version 3.28.1 (version corrective)

## Fichiers Modifiés

- `include/web_interface.h`: Injection broches, affichage bouton BOOT, avertissement GPIO, champ MISO
- `include/languages.h`: Ajout de 2 nouvelles clés de traduction
- `src/main.cpp`: Ajout de 3 nouveaux gestionnaires d'endpoints carte SD + enregistrement routes
- `platformio.ini`: Version 3.27.2 → 3.28.0
- `CHANGELOG.md` + `CHANGELOG_FR.md`: Documentation complète version 3.28.0

## Notes de Mise à Jour

### De 3.27.x vers 3.28.0
- Aucun changement incompatible
- Aucune modification matérielle requise
- Les améliorations de l'interface web sont automatiques
- Les tests de carte SD nécessitent ESP32-S3 v3.28.1 pour une fonctionnalité complète

### Recommandations de Test
1. Vérifier que la page Input Devices se charge sans erreurs
2. Confirmer que BUTTON_BOOT est en lecture seule dans l'UI
3. Tester les endpoints de carte SD (note : peut afficher "not available" sur ESP32-S3 jusqu'à v3.28.1)
4. Vérifier que l'avertissement GPIO 13 est affiché dans la section Carte SD

## Prochaines Étapes

Voir les notes de version 3.28.1 pour :
- Intégration backend MISO complète
- Fonctionnalité carte SD sur ESP32-S3
- Extension de la plage d'entrée MISO (48 GPIOs)

---

**Version :** 3.28.0
**Version Précédente :** 3.27.2
**Version Suivante :** 3.28.1
