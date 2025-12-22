# Notes de Version - Version 3.22.0

**Date de publication** : 11 décembre 2025  
**Type** : Mise à jour de configuration matérielle - Remapping GPIO ESP32-S3

## Aperçu

La version 3.22.0 résout les conflits GPIO de strapping sur ESP32-S3 en déplaçant les LED RGB des broches de strapping vers des GPIO dédiés. Ce changement matériel améliore la fiabilité du démarrage et s'aligne avec les meilleures pratiques de conception ESP32-S3. **Cette version nécessite un recâblage matériel pour les cartes ESP32-S3.**

> **⚠️ CRITIQUE** : v3.22.0 remappe les GPIO pour ESP32-S3. Un recâblage matériel est nécessaire. Lisez complètement ce document avant de flasher.

## Modifications apportées

### Remapping LED RGB ESP32-S3

**LED verte déplacée :**
- **Ancien** : GPIO 45 (broche de strapping - conflits avec sélection du mode de démarrage)
- **Nouveau** : GPIO 41 (GPIO dédié - aucun conflit de strapping)
- **Impact** : Nécessite un recâblage physique de la connexion LED verte

**LED bleue déplacée :**
- **Ancien** : GPIO 47 (broche de strapping - conflits avec sélection du mode de démarrage)
- **Nouveau** : GPIO 42 (GPIO dédié - aucun conflit de strapping)
- **Impact** : Nécessite un recâblage physique de la connexion LED bleue

**LED rouge inchangée :**
- **Broche** : GPIO 21 (aucun conflit de strapping - broche sûre)
- **Impact** : Aucun changement de câblage nécessaire

### Configuration ESP32 Classic

**Aucun changement par rapport à v3.21.0 :**
- Tous les mappages GPIO restent inchangés
- LED RGB : R=13, V=26, B=33
- Aucun recâblage matériel requis pour les cartes ESP32 Classic

## Pourquoi ce changement ?

### Justification technique

**Conflits de broches de strapping :**
- GPIO 45 et GPIO 47 sont des broches de strapping sur ESP32-S3
- Les broches de strapping déterminent le mode de démarrage, l'affichage des messages ROM et d'autres comportements critiques
- Les charges externes (comme les LED) sur les broches de strapping peuvent causer :
  - Des échecs de démarrage ou une mauvaise sélection du mode de démarrage
  - Un comportement de démarrage peu fiable
  - Des problèmes de programmation flash

**Résolution :**
- GPIO 41 et GPIO 42 sont des GPIO dédiés sans fonctions de strapping
- Fournit un fonctionnement LED RGB propre et sans conflit
- S'aligne avec les directives de conception matérielle Espressif

## Guide de migration

### Pour les utilisateurs ESP32-S3

**Changements matériels requis :**
1. **Débrancher l'alimentation** de la carte ESP32-S3
2. **Retirer la connexion LED verte** de GPIO 45
3. **Retirer la connexion LED bleue** de GPIO 47
4. **Connecter la LED verte** à GPIO 41 (avec résistance de limitation de courant 220-470Ω)
5. **Connecter la LED bleue** à GPIO 42 (avec résistance de limitation de courant 220-470Ω)
6. **Vérifier que la LED rouge** reste sur GPIO 21
7. **Flasher le firmware v3.22.0** après le recâblage

**Rappel de sécurité :**
- Toujours utiliser des résistances de limitation de courant (220-470Ω, 0.25W) en série avec les LED
- Vérifier les niveaux logiques 3.3V - ne jamais appliquer 5V directement sur GPIO

### Pour les utilisateurs ESP32 Classic

**Aucune action requise :**
- Flasher directement le firmware v3.22.0
- Tous les mappages GPIO inchangés
- Le matériel reste compatible

## Référence complète du mappage des broches

### Configuration LED RGB ESP32-S3 (v3.22.0)

| Composant | GPIO | Notes |
|-----------|------|-------|
| LED rouge | 21   | Aucun changement par rapport à v3.21.0 |
| LED verte | 41   | **Changée depuis GPIO 45** |
| LED bleue | 42   | **Changée depuis GPIO 47** |
| NeoPixel  | 48   | Aucun changement |

### Configuration LED RGB ESP32 Classic (v3.22.0)

| Composant | GPIO | Notes |
|-----------|------|-------|
| LED rouge | 13   | Aucun changement par rapport à v3.21.0 |
| LED verte | 26   | Aucun changement par rapport à v3.21.0 |
| LED bleue | 33   | Aucun changement par rapport à v3.21.0 |
| NeoPixel  | 2    | Aucun changement (mapping via board_config.h uniquement) |

## Modifications du code

### Constantes mises à jour dans board_config.h

**ESP32-S3 :**
```cpp
#define DEFAULT_RGB_LED_PIN_R 21  // Rouge - inchangé
#define DEFAULT_RGB_LED_PIN_G 41  // Vert - changé de 45
#define DEFAULT_RGB_LED_PIN_B 42  // Bleu - changé de 47
```

**ESP32 Classic :**
```cpp
#define DEFAULT_RGB_LED_PIN_R 13  // Aucun changement
#define DEFAULT_RGB_LED_PIN_G 26  // Aucun changement
#define DEFAULT_RGB_LED_PIN_B 33  // Aucun changement
```

## Mises à jour de la documentation

### Toute la documentation utilisateur mise à jour

**Avertissement ajouté :**
- Tous les fichiers README racine (EN/FR)
- Tous les fichiers de documentation dans `/docs` (EN/FR)
- Avertissement de remapping proéminent avec liens vers PIN_MAPPING.md

**Tables de mappage des broches :**
- Mise à jour de PIN_MAPPING.md et PIN_MAPPING_FR.md
- Affectations GPIO corrigées pour ESP32-S3
- Notes de migration ajoutées dans PIN_MAPPING_CHANGES_FR.md

**Mises à jour de version :**
- Tous les en-têtes de documentation mis à jour vers v3.22.0
- CHANGELOG.md et CHANGELOG_FR.md mis à jour
- Notes de version créées (ce document)

## Changements incompatibles

### Changements incompatibles au niveau matériel

**ESP32-S3 :**
- ⚠️ Le câblage LED RGB doit être mis à jour
- ⚠️ L'ancien câblage ne fonctionnera pas avec le firmware v3.22.0
- ⚠️ Le nouveau câblage ne fonctionnera pas avec le firmware v3.21.x

**ESP32 Classic :**
- ✅ Aucun changement incompatible
- ✅ Rétrocompatible avec le matériel existant

### Compatibilité logicielle

**Firmware :**
- Le firmware v3.22.0 est incompatible avec le câblage matériel v3.21.x sur ESP32-S3
- Le firmware v3.22.0 est compatible avec toutes les configurations ESP32 Classic

**Interface web :**
- Aucun changement d'API
- Affichages des broches automatiquement mis à jour en fonction de la cible compilée
- Aucune action utilisateur requise dans le tableau de bord web

## Liste de contrôle des tests

Après avoir flashé v3.22.0 et recâblé (ESP32-S3 uniquement) :

- [ ] Le système démarre sans erreurs
- [ ] La console série affiche les affectations GPIO correctes
- [ ] Les commandes de la LED rouge répondent correctement (interface web + physique)
- [ ] Les commandes de la LED verte répondent correctement (interface web + physique)
- [ ] Les commandes de la LED bleue répondent correctement (interface web + physique)
- [ ] L'indicateur d'état Wi-Fi NeoPixel fonctionne normalement
- [ ] Tous les autres tests matériels réussissent (capteurs, affichages, etc.)

## Instructions de retour arrière

Si vous devez revenir à v3.21.1 :

**ESP32-S3 :**
1. Flasher le firmware v3.21.1
2. Recâbler la LED verte vers GPIO 45
3. Recâbler la LED bleue vers GPIO 47

**ESP32 Classic :**
1. Flasher le firmware v3.21.1 (aucun changement de câblage)

## Problèmes connus

Aucun problème signalé au moment de la publication.

## Version précédente

Voir [RELEASE_NOTES_3.21.1_FR.md](RELEASE_NOTES_3.21.1_FR.md) pour les fonctionnalités de l'indicateur d'état Wi-Fi NeoPixel.

## Support

Pour les problèmes, questions ou assistance au câblage matériel :
- GitHub Issues : https://github.com/morfredus/ESP32-Diagnostic/issues
- Consultez PIN_MAPPING_FR.md pour la référence GPIO complète
- Consultez PIN_MAPPING_CHANGES_FR.md pour l'historique de migration détaillé

---

**Commit de version** : `v3.22.0: Remapping GPIO ESP32-S3 (G:45→41, B:47→42) + avertissements docs`  
**Cible de build** : `esp32s3_n16r8`, `esp32s3_n8r8`, `esp32_classic`  
**PlatformIO Core minimum** : 6.1.0  
**Arduino Core** : 3.3.3
