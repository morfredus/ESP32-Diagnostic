# Notes de Version - Version 3.22.1

**Date de version** : 12 décembre 2025  
**Type** : Correction de documentation - Corrections du mapping des broches ESP32 Classic

## Vue d'ensemble

La version 3.22.1 corrige des doublons de mapping de broches dans la documentation pour l'ESP32 Classic. La documentation indiquait incorrectement des affectations de broches dupliquées qui ne correspondaient pas à la configuration matérielle réelle définie dans `board_config.h`. Cette version aligne toute la documentation utilisateur avec les définitions de broches de référence.

**Aucun changement matériel requis. Aucun changement de fonctionnalité du firmware. Mise à jour de cohérence documentaire uniquement.**

> **ℹ️ INFO** : v3.22.1 corrige des doublons de mapping pour ESP32 Classic dans la documentation et conserve le remapping GPIO pour ESP32-S3 de v3.22.0. Assurez-vous que votre câblage correspond aux broches documentées pour votre carte cible.

## Ce qui a changé

### Corrections de la documentation du mapping des broches ESP32 Classic

Les affectations de broches suivantes ont été corrigées dans toute la documentation pour correspondre à `include/board_config.h` :

1. **Bouton 1** : GPIO 2 (incorrectement documenté comme GPIO 32)
2. **Bouton 2** : GPIO 5 (incorrectement documenté comme GPIO 33)
3. **LED verte** : GPIO 26 (incorrectement documentée comme GPIO 14)
4. **LED bleue** : GPIO 33 (incorrectement documentée comme GPIO 25)
5. **Capteur DHT** : GPIO 15 (incorrectement documenté comme GPIO 32)
6. **HC-SR04 TRIG** : GPIO 12 (incorrectement documenté comme GPIO 27)
7. **HC-SR04 ECHO** : GPIO 35 (incorrectement documenté comme GPIO 33)

### Configuration ESP32-S3

**Aucun changement depuis v3.22.0 :**
- Le remapping des LED RGB de v3.22.0 reste en vigueur
- LED verte : GPIO 41 (changé en v3.22.0 depuis GPIO 45)
- LED bleue : GPIO 42 (changé en v3.22.0 depuis GPIO 47)
- LED rouge : GPIO 21 (inchangé)

## Évaluation de l'impact

### Ce que cela signifie pour les utilisateurs

**Si vous avez du matériel ESP32 Classic fonctionnel :**
- ✅ Votre matériel est probablement câblé selon `board_config.h` (les broches correctes)
- ✅ Aucun changement nécessaire - la documentation correspond maintenant à votre configuration
- ✅ Continuez à utiliser votre configuration matérielle actuelle

**Si vous avez câblé selon l'ancienne documentation :**
- ⚠️ Votre câblage peut ne pas correspondre aux attentes du firmware
- ⚠️ Consultez PIN_MAPPING_FR.md et vérifiez vos connexions
- ⚠️ Recâblez pour correspondre aux définitions de `board_config.h` si nécessaire

**Utilisateurs ESP32-S3 :**
- ✅ Aucun changement depuis v3.22.0
- ✅ Utilisez le câblage v3.22.0 (GPIO 41/42 pour LED verte/bleue)

## Référence du mapping des broches corrigé

### Mapping complet des broches ESP32 Classic (v3.22.1 - Corrigé)

| Composant | GPIO | Docs précédentes | Statut |
|-----------|------|------------------|--------|
| Bouton 1 | 2 | 32 ❌ | ✅ Corrigé |
| Bouton 2 | 5 | 33 ❌ | ✅ Corrigé |
| LED rouge | 13 | 13 ✅ | Pas de changement |
| LED verte | 26 | 14 ❌ | ✅ Corrigé |
| LED bleue | 33 | 25 ❌ | ✅ Corrigé |
| Capteur DHT | 15 | 32 ❌ | ✅ Corrigé |
| HC-SR04 TRIG | 12 | 27 ❌ | ✅ Corrigé |
| HC-SR04 ECHO | 35 | 33 ❌ | ✅ Corrigé |
| Sortie PWM | 4 | 4 ✅ | Pas de changement |
| Buzzer | 19 | 19 ✅ | Pas de changement |
| NeoPixel | 2 | 2 ✅ | Pas de changement (configurable) |

### Mapping des broches ESP32-S3 (v3.22.1 - Inchangé depuis v3.22.0)

| Composant | GPIO | Notes |
|-----------|------|-------|
| LED rouge | 21 | Pas de changement |
| LED verte | 41 | Remappé en v3.22.0 |
| LED bleue | 42 | Remappé en v3.22.0 |
| NeoPixel | 48 | Pas de changement |

## Mises à jour de la documentation

### Fichiers mis à jour vers v3.22.1

**En-têtes de version :**
- `platformio.ini` - PROJECT_VERSION mis à jour vers "3.22.1"
- Tous les fichiers de documentation racine (README.md, README_FR.md)
- Tous les fichiers `/docs` (versions EN/FR) :
  - API_REFERENCE, ARCHITECTURE, BUILD_AND_DEPLOY
  - CONFIG, CONTRIBUTING, DIAGNOSTIC_MODULES
  - FAQ, FEATURE_MATRIX, INSTALL
  - OVERVIEW, PIN_MAPPING, SECURITY
  - TROUBLESHOOTING, USAGE, WEB_INTERFACE

**Texte d'avertissement mis à jour :**
- Toute la documentation référence maintenant v3.22.1
- L'avertissement clarifie les corrections ESP32 Classic + conservation du remapping ESP32-S3
- Liens vers PIN_MAPPING_FR.md pour référence complète

**Tables de mapping des broches :**
- PIN_MAPPING.md et PIN_MAPPING_FR.md corrigés
- Le tableau ESP32 Classic correspond maintenant exactement à `board_config.h`
- Annotations ajoutées montrant les corrections par rapport aux docs précédentes

## Modifications du code

### Aucun changement de firmware

**board_config.h :**
- Aucun changement - avait déjà les définitions de broches correctes
- La documentation correspond maintenant au code existant

**Comportement à l'exécution :**
- Aucun changement fonctionnel
- Tous les tests matériels restent identiques
- Version du firmware incrémentée pour traçabilité uniquement

## Cohérence des versions

### Mises à jour du changelog

**CHANGELOG.md et CHANGELOG_FR.md :**
- Entrée v3.22.1 ajoutée avec liste numérotée des corrections
- Liens vers v3.22.0 pour référence du remapping ESP32-S3

**Suivi de version :**
- Toute la documentation synchronisée sur v3.22.1
- Macro PROJECT_VERSION mise à jour dans platformio.ini
- Notes de version créées (ce document)

## Guide de migration

### Pour tous les utilisateurs

**Action requise :**
1. Lisez [docs/PIN_MAPPING_FR.md](PIN_MAPPING_FR.md) ou [docs/PIN_MAPPING.md](PIN_MAPPING.md)
2. Vérifiez que votre câblage matériel correspond aux broches documentées
3. Mettez à jour vos schémas de câblage ou notes personnelles

**Aucun flash de firmware requis :**
- v3.22.1 est uniquement documentaire
- Flashez seulement si vous voulez la cohérence du numéro de version
- Comportement matériel identique à v3.22.0

### Étapes de vérification

**ESP32 Classic :**
- [ ] Comparez votre câblage au tableau ESP32 Classic de PIN_MAPPING_FR.md
- [ ] Vérifiez les broches des boutons (GPIO 2, 5)
- [ ] Vérifiez les broches des LED RGB (GPIO 13, 26, 33)
- [ ] Vérifiez les broches des capteurs (GPIO 15, 12, 35)

**ESP32-S3 :**
- [ ] Vérifiez le câblage des LED RGB v3.22.0 (GPIO 21, 41, 42)
- [ ] Aucun changement depuis v3.22.0

## Problèmes connus

Aucun signalé au moment de la version.

## Versions précédentes

- [RELEASE_NOTES_3.22.0_FR.md](RELEASE_NOTES_3.22.0_FR.md) - Remapping GPIO ESP32-S3
- [RELEASE_NOTES_3.21.1_FR.md](RELEASE_NOTES_3.21.1_FR.md) - Indicateur d'état Wi-Fi NeoPixel

## Support

Pour les problèmes, questions ou assistance de câblage matériel :
- GitHub Issues : https://github.com/morfredus/ESP32-Diagnostic/issues
- Consultez PIN_MAPPING_FR.md pour référence GPIO complète
- Comparez votre configuration aux définitions de board_config.h

---

**Commit de version** : `v3.22.1: Correction doublons pin mapping ESP32 Classic + synchronisation docs`  
**Cible de build** : `esp32s3_n16r8`, `esp32s3_n8r8`, `esp32_classic`  
**PlatformIO Core minimum** : 6.1.0  
**Arduino Core** : 3.3.3
