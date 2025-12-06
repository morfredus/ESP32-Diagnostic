# Notes de version — Version 3.18.3

**Date de publication :** 6 décembre 2025  
**Type :** Version corrective (Patch - Correction Pin Mapping)

---

## Vue d'ensemble

La version 3.18.3 résout un conflit matériel critique sur les cartes ESP32-S3 où le GPIO 48 était simultanément assigné au canal Vert de la LED RGB et à la LED NeoPixel/WS2812B. Cette version réorganise complètement le pin mapping ESP32-S3 pour éliminer tous les conflits et activer la fonctionnalité LED NeoPixel.

---

## Résumé des changements

### Corrigé
- **Résolution du conflit GPIO 48** : NeoPixel et LED RGB Vert étaient tous deux assignés au GPIO 48, causant un conflit matériel
  - NeoPixel utilise maintenant exclusivement GPIO 48 (activé, était précédemment désactivé)
  - LED RGB Vert déplacée du GPIO 48 vers GPIO 46

### Modifié — Refactorisation du Pin Mapping ESP32-S3

#### Broches LED RGB (ESP32-S3)
| Composant | Ancien pin | Nouveau pin | Notes |
|-----------|-----------|-----------|-------|
| RGB Rouge | GPIO 19    | GPIO 19     | Inchangé |
| RGB Vert  | GPIO 48    | GPIO 47     | **Déplacé pour résoudre le conflit** |
| RGB Bleu  | GPIO 45    | GPIO 45     | Inchangé |

#### Configuration NeoPixel (ESP32-S3)
| Paramètre | Ancienne valeur | Nouvelle valeur | Notes |
|-----------|-----------------|-----------------|-------|
| Pin       | -1 (désactivé)  | GPIO 48         | **Maintenant activé** |
| Nombre LED| 8               | 1               | LED unique intégrée |
| Luminosité| 50              | 50              | Inchangé |

#### Réorganisation des broches capteurs (ESP32-S3)
| Capteur | Ancien Pin | Nouveau Pin | Justification |
|---------|------------|-------------|---------------|
| Capteur Lumière (ADC) | GPIO 19 | GPIO 4 | GPIO 19 maintenant utilisé pour RGB Bleu |
| Capteur Mouvement (PIR) | GPIO 4 | GPIO 6 | GPIO 4 maintenant utilisé pour Capteur Lumière |
| HC-SR04 ECHO | GPIO 6 | GPIO 19 | GPIO 6 maintenant utilisé pour Capteur Mouvement |

**Note importante :** GPIO 19 est maintenant partagé entre la LED RGB Bleu et HC-SR04 ECHO. Ces utilisations sont mutuellement exclusives — utiliser un seul à la fois.

#### Broches inchangées (ESP32-S3)
- **I2C** : SDA=21, SCL=20
- **Boutons** : BTN1=1, BTN2=2
- **GPS (UART1)** : RXD=18, TXD=17, PPS=8
- **TFT ST7789** : MOSI=11, SCLK=12, CS=10, DC=9, RST=13, BL=7
- **PWM/Buzzer** : GPIO 14
- **Capteur DHT** : GPIO 5
- **HC-SR04 TRIG** : GPIO 3

### Mises à jour de la documentation
- `README.md` et `README_FR.md` : Version et résumé des pins ESP32-S3 mis à jour
- `docs/PIN_MAPPING.md` et `docs/PIN_MAPPING_FR.md` : Tableaux complets de pin mapping mis à jour
- `include/config-example.h` : Synchronisé avec la configuration ESP32-S3 définitive
- `platformio.ini` : Version passée à 3.18.3

---

## Guide de migration

### Si vous utilisez du matériel ESP32-S3

1. **Câblage LED RGB** (ESP32-S3 DevKitC-1 N16R8) :
   - RGB Rouge : GPIO 19 (inchangé)
   - RGB Vert : GPIO 47 (déplacé de GPIO 48)
   - RGB Bleu : GPIO 45 (inchangé)
   - Recâbler le fil Vert de GPIO 48 vers GPIO 47 sur la carte

2. **LED NeoPixel** :
   - Si vous avez une NeoPixel intégrée ou externe sur GPIO 48, elle fonctionnera maintenant correctement
   - Définissez `DEFAULT_NEOPIXEL_PIN 48` et `DEFAULT_NEOPIXEL_COUNT 1` dans `config.h`

3. **Ajustements des capteurs** :
   - Déplacez le Capteur Lumière du GPIO 19 vers GPIO 4
   - Déplacez le Capteur Mouvement du GPIO 4 vers GPIO 6
   - Déplacez HC-SR04 ECHO du GPIO 6 vers GPIO 19

4. **Gestion des conflits** :
   - GPIO 19 est partagé entre RGB Bleu et HC-SR04 ECHO
   - Si vous utilisez la LED RGB, n'utilisez pas le capteur ultrasonique HC-SR04
   - Si vous utilisez HC-SR04, désactivez la LED RGB ou évitez le canal bleu

### Si vous utilisez ESP32 Classic

**Aucun changement requis.** Les pin mappings ESP32 Classic restent inchangés.

### Migration logicielle

1. **Mise à jour de la configuration** :
   ```bash
   # Récupérer les derniers changements
   git pull origin main
   
   # Copier le nouveau template si nécessaire
   cp include/config-example.h include/config.h
   ```

2. **Recompilation du firmware** :
   ```bash
   pio run -e esp32s3_n16r8
   pio run -e esp32s3_n16r8 --target upload
   ```

3. **Vérification des assignations de pins** : Consultez la sortie série au démarrage pour confirmer la détection des pins

---

## Recommandations de test

1. **Test LED RGB** : Vérifiez que les trois canaux de couleur (rouge, vert, bleu) fonctionnent indépendamment
2. **Test NeoPixel** : Confirmez que la NeoPixel sur GPIO 48 répond aux commandes de couleur
3. **Tests capteurs** : Testez les capteurs Lumière, Mouvement et Distance individuellement pour vérifier les nouvelles assignations de pins
4. **Test de conflit** : Vérifiez que l'utilisation du RGB Bleu ou du HC-SR04 ECHO sur GPIO 19 fonctionne comme prévu (mais pas les deux simultanément)

---

## Problèmes connus

- Le conflit GPIO 19 entre RGB Bleu et HC-SR04 ECHO nécessite une utilisation exclusive
- Envisagez d'utiliser l'écran TFT ou OLED pour le retour visuel si la LED RGB est désactivée

---

## Justification du versionnage sémantique

Il s'agit d'une **version corrective** (3.18.2 → 3.18.3) car :
- Elle corrige un bug de conflit matériel (assignation GPIO 48)
- Aucune nouvelle fonctionnalité ajoutée
- Aucun changement cassant de l'API ou des interfaces logicielles
- Les changements sont limités à la configuration des broches matérielles
- Rétrocompatible pour les utilisateurs ESP32 Classic
- Les utilisateurs ESP32-S3 doivent ajuster le câblage matériel mais le logiciel reste compatible

---

## Crédits

- Conflit de pin identifié lors des tests matériels
- La résolution privilégie la fonctionnalité NeoPixel intégrée
- Broches des capteurs réorganisées pour maintenir la compatibilité complète des fonctionnalités

---

Pour la documentation détaillée du pin mapping, voir :
- [PIN_MAPPING.md](PIN_MAPPING.md) (English)
- [PIN_MAPPING_FR.md](PIN_MAPPING_FR.md) (Français)

Pour toute question ou problème, veuillez ouvrir un ticket GitHub à :  
**https://github.com/morfredus/ESP32-Diagnostic/issues**
