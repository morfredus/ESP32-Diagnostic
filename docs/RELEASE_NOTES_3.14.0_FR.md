# Notes de version 3.14.0

**Date de publication :** 27 novembre 2025  
**Type de version :** Nouvelle fonctionnalité (version mineure)

## Vue d'ensemble

La version 3.14.0 apporte la prise en charge complète du test d'affichage TFT via l'interface web, avec une nouvelle interface utilisateur dédiée permettant de tester tous les aspects de l'écran TFT ST7789 240x240.

## Principales fonctionnalités

### 1. Interface Web TFT Complète

Une nouvelle section **"Écran TFT"** a été ajoutée à l'interface web, offrant :

1. **Affichage des informations** :
   - Résolution de l'écran (240x240 pixels)
   - Configuration des pins SPI (MOSI, SCLK, CS, DC, RST)
   - Statut de détection automatique

2. **Bouton de test complet** :
   - Lance la séquence complète des 8 tests TFT
   - Feedback en temps réel via l'interface

3. **Bouton "Écran de démarrage"** :
   - Restaure l'écran de démarrage TFT à tout moment
   - Permet de revenir à l'état initial après les tests

4. **8 tests individuels** :
   - **Boot splash** : Affiche l'écran de démarrage avec logo et informations système
   - **Test des couleurs** : Affiche 7 couleurs primaires en séquence
   - **Formes géométriques** : Dessine rectangles, cercles, triangles, remplis et vides
   - **Rendu de texte** : Teste différentes tailles de police et alignements
   - **Motifs de lignes** : Génère des patterns de lignes horizontales, verticales, diagonales
   - **Animation** : Cercle animé qui se déplace sur l'écran
   - **Barre de progression** : Barre de progression animée de 0% à 100%
   - **Message final** : Message de fin de test avec feedback visuel

### 2. Amélioration de l'interface OLED

Le bouton **"Écran de démarrage"** a également été ajouté à la section OLED pour :
- Restaurer l'affichage de démarrage OLED après les tests
- Cohérence avec l'interface TFT

### 3. Nouvelles API REST

Quatre nouveaux endpoints ont été ajoutés :

1. **`GET /api/tft-test`** :
   - Lance la séquence complète des 8 tests TFT
   - Retour : JSON avec statut de réussite

2. **`GET /api/tft-step?step=<id>`** :
   - Exécute un test TFT individuel (boot, colors, shapes, text, lines, animation, progress, final)
   - Retour : JSON avec nom et statut du test

3. **`GET /api/tft-boot`** :
   - Restaure l'écran de démarrage TFT
   - Retour : JSON avec confirmation

4. **`GET /api/oled-boot`** :
   - Restaure l'écran de démarrage OLED
   - Retour : JSON avec confirmation

## Améliorations techniques

### 1. Architecture cohérente

L'implémentation TFT suit exactement la même architecture que l'OLED :
- Fonctions de test modulaires dans `src/main.cpp`
- Handlers HTTP avec réponses JSON standardisées
- Interface web avec grid layout et boutons individuels
- Système de traduction bilingue intégré

### 2. Internationalisation

13 nouvelles clés de traduction bilingues (EN/FR) ont été ajoutées :
- `tft_screen`, `tft_step_boot`, `tft_step_colors`, `tft_step_shapes`
- `tft_step_text`, `tft_step_lines`, `tft_step_animation`, `tft_step_progress`
- `tft_step_final`, `boot_screen`, `resolution`, `spi_pins`
- `tft_test_running`, `tft_step_running`, `restoring_boot_screen`

### 3. Tests TFT adaptés

Les tests TFT ont été spécifiquement adaptés aux capacités d'un écran couleur 240x240 :
- **Test de couleurs** : 7 couleurs (noir, rouge, vert, bleu, jaune, cyan, magenta, blanc)
- **Formes** : Utilisation de formes remplies et vides avec couleurs variées
- **Animation** : Mouvement fluide avec effacement partiel
- **Barre de progression** : Visualisation graphique avec texte de pourcentage

## Informations de compilation

### Environnements supportés

Les trois environnements compilent avec succès :

1. **esp32s3_n16r8** :
   - RAM : 17.3% (56 764 bytes / 327 680 bytes)
   - Flash : 33.6% (1 056 817 bytes / 3 145 728 bytes)
   - Temps de compilation : 38.05s

2. **esp32s3_n8r8** :
   - RAM : 17.4% (57 072 bytes / 327 680 bytes)
   - Flash : 34.0% (1 070 061 bytes / 3 145 728 bytes)
   - Temps de compilation : 37.88s

3. **esp32devkitc** :
   - RAM : 17.3% (56 812 bytes / 327 680 bytes)
   - Flash : 85.0% (1 113 857 bytes / 1 310 720 bytes)
   - Temps de compilation : 37.44s

### Notes importantes

- Les avertissements C++17 inline variables sont pré-existants et n'affectent pas la fonctionnalité
- Tous les environnements ont une marge suffisante de RAM
- L'environnement esp32devkitc utilise 85% de Flash mais dispose encore de 196 KB disponibles

## Migration depuis 3.13.1

Aucune action requise. Cette version est entièrement rétrocompatible :
- Les fonctionnalités existantes ne sont pas modifiées
- L'ajout de fonctionnalités TFT n'affecte pas les configurations OLED
- Les API existantes restent inchangées

## Configuration requise

Pour utiliser les tests TFT, il faut :
1. Un écran TFT ST7789 240x240 pixels
2. Configuration des pins SPI dans `include/tft_display.h`
3. Définir `TFT_ENABLED` à `true` dans la configuration

Si le TFT n'est pas détecté, la section n'apparaîtra pas dans l'interface web.

## Documentation mise à jour

Les documents suivants ont été mis à jour pour la v3.14.0 :
- README (EN/FR)
- CHANGELOG (EN/FR)
- BUILD_AND_DEPLOY (EN/FR)
- RELEASE_NOTES (EN/FR) - nouveau

## Remerciements

Cette version consolide l'architecture modulaire du projet et démontre la flexibilité du système pour supporter différents types d'affichages.

## Liens utiles

- **Repository GitHub** : https://github.com/[votre-username]/ESP32-Diagnostic
- **Documentation complète** : `/docs/`
- **Rapports de bugs** : Utiliser les GitHub Issues

---

**Checksums (MD5)**
- esp32s3_n16r8 : [calculer après upload]
- esp32s3_n8r8 : [calculer après upload]
- esp32devkitc : [calculer après upload]
