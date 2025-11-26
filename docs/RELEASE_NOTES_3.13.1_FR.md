# ESP32 Diagnostic Suite — Note de version 3.13.1 (2025-11-26)

Version de maintenance. Aucun changement fonctionnel du firmware. Améliorations de maintenance, dépendances et documentation.

## Résumé
1. Dépendances PlatformIO : passage aux intervalles avec chapeau (`^`) pour les bibliothèques Adafruit afin d'autoriser les mises à jour mineures/patch sans casse.
2. Build : compilation validée sur les trois environnements (`esp32s3_n16r8`, `esp32s3_n8r8`, `esp32devkitc`). Upload vérifié sur `esp32s3_n16r8`.
3. Documentation : READMEs et guides BUILD mis à jour (EN/FR) avec commandes `pio` par environnement et statut de build.

## Détails
1. `platformio.ini` — Dépendances
   - `adafruit/Adafruit NeoPixel` → `@^1.12.0`
   - `adafruit/DHT sensor library` → `@^1.4.6`
   - `adafruit/Adafruit BusIO` → `@^1.17.4`
   - `adafruit/Adafruit GFX Library` → `@^1.12.4`
   - `adafruit/Adafruit ST7735 and ST7789 Library` → `@^1.11.0`
   - Bénéfice : corrections de bugs et améliorations non-breaking via SemVer (minor/patch).

2. Documentation — Mises à jour
   - `README.md` / `README_FR.md` : ajout d'un bloc maintenance listant les changements (versions caret + builds validés).
   - `docs/BUILD_AND_DEPLOY.md` / `docs/BUILD_AND_DEPLOY_FR.md` : commandes `pio run -e ...` pour chaque environnement et section "Build status / Statut de build".

3. Qualité de build — État vérifié
   - `esp32s3_n16r8` : Build OK, Upload OK.
   - `esp32s3_n8r8` : Build OK.
   - `esp32devkitc` : Build OK.

## Compatibilité
- Aucune modification de compatibilité. Les environnements supportés restent : `esp32s3_n16r8` (principal), `esp32s3_n8r8`, `esp32devkitc`.

## Mise à niveau
1. Aucun changement de configuration requis. Un simple rebuild suffit.
2. Pour vérifier :
   - `pio run -e esp32s3_n16r8`
   - `pio run -e esp32s3_n8r8`
   - `pio run -e esp32devkitc`

## Remarques
- Ces modifications sont strictement de maintenance (dépendances/doc). Le numéro de firmware reste aligné sur la série 3.13.x.
