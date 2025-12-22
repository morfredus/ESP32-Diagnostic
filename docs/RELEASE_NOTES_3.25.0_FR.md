# Suite de Diagnostic ESP32 - Notes de Version v3.25.0

**Date de Publication :** 22 décembre 2025

## Vue d'Ensemble

La version 3.25.0 représente une **amélioration architecturale majeure** qui restaure la fonctionnalité de remapping dynamique des broches GPIO tout en résolvant les conflits de préprocesseur survenus dans les versions précédentes. Cette version introduit une solution élégante basée sur une convention de nommage qui maintient à la fois l'efficacité au moment de la compilation et la flexibilité à l'exécution.

## Fonctionnalité Clé : Remapping Dynamique des Broches GPIO Restauré

### Problème Résolu

Dans la version 3.24.0, une simplification architecturale avait supprimé le préfixe `DEFAULT_` des noms de broches GPIO dans `board_config.h`, ce qui éliminait les variables runtime de broches. Cela causait des conflits de préprocesseur et supprimait la capacité de remapper dynamiquement les broches GPIO via l'interface Web—une fonctionnalité critique pour la flexibilité matérielle.

### Solution Implémentée

**Architecture GPIO à Deux Couches avec Conventions de Nommage Distinctes :**

1. **Valeurs par défaut compile-time** (MAJUSCULES dans `board_config.h`) :
   ```cpp
   #define I2C_SDA       15  // Broche SDA
   #define I2C_SCL       16  // Broche SCL
   #define RGB_LED_PIN_R 21  // Canal rouge RGB
   ```

2. **Variables runtime** (minuscules dans `main.cpp`) :
   ```cpp
   int i2c_sda = I2C_SDA;          // Modifiable à l'exécution
   int i2c_scl = I2C_SCL;          // Modifiable à l'exécution
   int rgb_led_pin_r = RGB_LED_PIN_R;  // Modifiable à l'exécution
   ```

**Point Clé :** Utiliser des conventions de nommage différentes (MAJUSCULES vs minuscules) empêche le préprocesseur d'expander les noms de variables en minuscules, éliminant les conflits tout en maintenant les deux couches.

## Nouveautés

### Ajouts

- **Variables Runtime de Broches en Minuscules**
  - Déclarées dans `src/main.cpp:201-216`
  - Déclarations externes dans `include/web_interface.h:24-35`
  - Variables : `i2c_sda`, `i2c_scl`, `rgb_led_pin_r`, `rgb_led_pin_g`, `rgb_led_pin_b`, `pwm_pin`, `buzzer_pin`, `dht_pin`, `light_sensor_pin`, `distance_trig_pin`, `distance_echo_pin`, `motion_sensor_pin`

### Restaurations

- **Remapping Dynamique des Broches via l'UI Web** - Tous les handlers de configuration pleinement fonctionnels :
  - Broches I2C (OLED et capteurs environnementaux)
  - Broches LED RGB (canaux R, V, B)
  - Broche buzzer
  - Broche capteur DHT
  - Broche capteur de lumière
  - Broches capteur de distance (Trigger et Echo)
  - Broche capteur de mouvement

### Modifications

- **Toutes les Références aux Broches GPIO Mises à Jour** (~100+ occurrences) :
  - `src/main.cpp` : Fonctions de test, handlers de configuration, injection JavaScript
  - `src/environmental_sensors.cpp` : Références broches I2C
  - Maintient les defines MAJUSCULES dans `board_config.h` comme source autoritaire

## Architecture Technique

### Avantages du Système à Deux Couches

| Couche | Emplacement | Format | Objectif | Modifiable |
|--------|-------------|--------|----------|------------|
| Compile-time | `board_config.h` | `#define I2C_SDA 15` | Valeurs par défaut | Non (nécessite recompilation) |
| Runtime | `main.cpp` | `int i2c_sda = I2C_SDA;` | Mapping actif des broches | Oui (via UI Web) |

### Pourquoi Cela Fonctionne

- **Pas de conflits préprocesseur** : Les noms de variables en minuscules ne déclenchent pas l'expansion de macros
- **Distinction claire** : MAJUSCULES = compile-time, minuscules = runtime
- **Performance** : Le compilateur optimise l'initialisation constante
- **Flexibilité** : Les utilisateurs peuvent tester différentes configurations de broches sans recompilation

## Guide de Migration

### Depuis v3.24.0

1. **Aucune modification de code requise** - l'architecture est rétrocompatible
2. **Mise à jour firmware recommandée** - flasher la nouvelle version pour restaurer le remapping de broches via l'UI Web
3. **Aucun changement matériel** - les mappings de broches restent identiques aux valeurs par défaut v3.24.0

### Depuis v3.23.x

- **Entièrement compatible** - toutes les fonctionnalités de v3.23.x sont préservées
- **Architecture améliorée** - convention de nommage plus claire prévient les conflits

## Fichiers Modifiés

### Fichiers Code
- `src/main.cpp` - Variables runtime, handlers, 100+ références broches
- `include/web_interface.h` - Déclarations externes
- `src/environmental_sensors.cpp` - Références broches I2C

### Configuration
- `include/board_config.h` - Maintient les defines MAJUSCULES (depuis modifications utilisateur)
- `platformio.ini` - Passage version à 3.25.0

### Documentation
- `CHANGELOG.md` / `CHANGELOG_FR.md` - Notes de version ajoutées
- Ce fichier (`RELEASE_NOTES_3.25.0_FR.md`)

## Compatibilité

- **ESP32-S3 (N16R8, N8R8)** : ✅ Entièrement supporté
- **ESP32 Classic (DevKitC)** : ✅ Entièrement supporté
- **PlatformIO** : espressif32@^6
- **Arduino Core** : ESP32 3.3.3
- **Rétrocompatibilité** : ⚠️ Nécessite mise à jour firmware depuis v3.24.0

## Problèmes Connus

Aucun problème rapporté pour cette version.

## Prochaines Étapes

Pour la documentation technique détaillée sur la gestion des broches GPIO, voir :
- `docs/PIN_POLICY.md` - Politique de mapping GPIO et conventions de nommage
- `docs/PIN_MAPPING.md` - Référence complète du mapping de broches

## Crédits

Cette amélioration architecturale a été développée pour résoudre de réels conflits de préprocesseur tout en maintenant la flexibilité dont les utilisateurs dépendent pour le prototypage et les tests matériels.

---

**Changelog Complet** : Voir [CHANGELOG_FR.md](../CHANGELOG_FR.md) pour l'historique complet des versions
