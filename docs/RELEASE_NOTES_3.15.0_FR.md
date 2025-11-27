# ESP32 Diagnostic Suite — Notes de Version v3.15.0

**Date de sortie :** 27 novembre 2025  
**Dépôt :** https://github.com/morfredus/ESP32-Diagnostic  
**Branche :** dev

---

## Vue d'ensemble

La version 3.15.0 introduit le **support multi-environnements** pour l'ESP32 Diagnostic Suite, permettant aux développeurs de compiler et déployer un firmware adapté à trois configurations matérielles ESP32 distinctes. Cette version se concentre sur la flexibilité matérielle via compilation conditionnelle et mappings de broches dédiés.

---

## Nouveautés

### 1. Système de Build Multi-Environnements

Trois environnements PlatformIO distincts sont maintenant disponibles dans `platformio.ini` :

#### **esp32s3_n16r8** (Par défaut)
- ESP32-S3 DevKitC-1 N16R8
- Flash 16Mo (mode QIO, 80MHz)
- PSRAM 8Mo OPI/QSPI (80MHz)
- Partition : `huge_app.csv`
- Define cible : `TARGET_ESP32_S3`

#### **esp32s3_n8r8**
- ESP32-S3 DevKitC-1 N8R8
- Flash 8Mo
- PSRAM 8Mo
- Partition : `huge_app.csv`
- Define cible : `TARGET_ESP32_S3`

#### **esp32devkitc**
- ESP32 Classic DevKitC
- Flash 4Mo
- Pas de PSRAM
- Partition : `default.csv`
- Define cible : `TARGET_ESP32_CLASSIC`

### 2. Mappings de Broches Spécifiques au Matériel

Les configurations de broches sont maintenant définies de manière conditionnelle dans `include/config.h` :

**Configuration ESP32-S3 (N16R8 / N8R8) :**
- I2C : SDA=21, SCL=20
- LED RGB : R=14, G=13, B=18
- Capteurs : DHT=19, Lumière=4, Distance TRIG=16/ECHO=17, Mouvement=39, Buzzer=3
- TFT ST7789 : MOSI=11, SCLK=12, CS=10, DC=9, RST=7, BL=15
- GPS : RXD=8, TXD=5, PPS=38

**Configuration ESP32 Classic (DevKitC) :**
- I2C : SDA=21, SCL=22
- LED RGB : R=25, G=26, B=27
- Capteurs : DHT=4, Lumière=34, Distance TRIG=5/ECHO=18, Mouvement=36, Buzzer=13
- TFT ST7789 : MOSI=23, SCLK=18, CS=15, DC=2, RST=4, BL=32
- GPS : RXD=16, TXD=17, PPS=39
- Boutons : BTN1=0 (BOOT), BTN2=35

### 3. Compilation Conditionnelle

Le code firmware utilise maintenant des directives préprocesseur pour s'adapter au matériel cible :

```cpp
#if defined(TARGET_ESP32_S3)
    // Code et définitions de broches spécifiques ESP32-S3
#elif defined(TARGET_ESP32_CLASSIC)
    // Code et définitions de broches spécifiques ESP32 Classic
#else
    #error "Aucune cible définie"
#endif
```

### 4. Mappings de Broches Partagés

Lorsque les contraintes matérielles le permettent, ESP32-S3 N8R8 et ESP32 Classic partagent des affectations de broches communes pour simplifier les transitions entre cartes.

---

## Mises à Jour de la Documentation

Toute la documentation a été mise à jour pour refléter le support multi-environnements :

- **README.md / README_FR.md :** Bannière de version et matrice de compatibilité mises à jour
- **CHANGELOG.md / CHANGELOG_FR.md :** Section de version v3.15.0 ajoutée
- **docs/PIN_MAPPING.md / PIN_MAPPING_FR.md :** Référence complète des broches pour les trois environnements
- **docs/BUILD_AND_DEPLOY.md / BUILD_AND_DEPLOY_FR.md :** Instructions de build et guide de sélection d'environnement
- **docs/CONFIG.md / CONFIG_FR.md :** Détails de configuration pour chaque cible

---

## Compilation & Déploiement

### Sélection d'un Environnement

**Méthode 1 : Éditer platformio.ini**
```ini
[platformio]
default_envs = esp32s3_n16r8  ; Changer pour esp32s3_n8r8 ou esp32devkitc
```

**Méthode 2 : Ligne de Commande**
```bash
# Builder un environnement spécifique
pio run -e esp32s3_n16r8
pio run -e esp32s3_n8r8
pio run -e esp32devkitc

# Téléverser vers une carte spécifique
pio run -e esp32devkitc --target upload
```

### Statut de Build

Les trois environnements compilent avec succès :
- ✓ **esp32s3_n16r8 :** Build OK, Upload OK, Testé Matériel
- ✓ **esp32s3_n8r8 :** Build OK, Compilation Validée
- ✓ **esp32devkitc :** Build OK, Tests Matériel En Attente

---

## Guide de Migration

### Depuis v3.14.x

Aucun changement de code requis pour les déploiements existants sur ESP32-S3 N16R8. L'environnement par défaut reste `esp32s3_n16r8`.

Pour autre matériel :
1. Sélectionner l'environnement approprié dans `platformio.ini`
2. Réviser les mappings de broches dans `docs/PIN_MAPPING_FR.md`
3. Recompiler le firmware : `pio run -e <environnement>`

### Changements de Mapping des Broches

Les définitions de broches sont passées de variables runtime à des constantes compile-time basées sur la cible. Si vous avez des configurations personnalisées :

1. Éditer `include/config.h`
2. Modifier les broches dans le bloc `#if defined(TARGET_...)` approprié
3. Recompiler le firmware

Les changements de broches runtime via interface web restent supportés pour les capteurs.

---

## Problèmes Connus

1. **esp32devkitc :** Tests matériels en attente faute de carte de référence disponible
2. **Conflit TFT/HC-SR04 (ESP32 Classic) :** SCLK (GPIO 18) partagé avec broche ECHO—utiliser l'un ou l'autre
3. **Limitation ADC2 (ESP32 Classic) :** Broches ADC2 (GPIO 0, 2, 4, 12-15, 25-27) inutilisables quand WiFi actif

---

## Changements Incompatibles

**Aucun.** Cette version maintient la rétrocompatibilité avec le firmware v3.14.x pour les cartes ESP32-S3 N16R8.

---

## Chemin de Mise à Niveau

1. Récupérer le code le plus récent depuis la branche `dev`
2. Sélectionner l'environnement cible dans `platformio.ini`
3. Compiler : `pio run -e <environnement>`
4. Téléverser : `pio run -e <environnement> --target upload`
5. Vérifier via moniteur série (115200 bauds)

---

## Crédits

- **Développement :** morfredus
- **Tests :** Carte de référence ESP32-S3 N16R8 validée
- **Documentation :** Mise à jour bilingue complète (EN/FR)

---

## Prochaines Étapes

- Tests matériels sur ESP32-DevKitC Classic
- Support d'environnements additionnels (ESP32-S2, ESP32-C3) si la demande le justifie
- Outils de résolution de conflits de broches dans l'interface web

---

## Support

- **Issues :** https://github.com/morfredus/ESP32-Diagnostic/issues
- **Documentation :** https://github.com/morfredus/ESP32-Diagnostic/tree/dev/docs
- **Licence :** MIT

---

**Changelog Complet :** [CHANGELOG.md](../CHANGELOG.md) | [CHANGELOG_FR.md](../CHANGELOG_FR.md)
