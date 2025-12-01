# Notes de version — 3.17.0 (2025-12-01)

## Ajouts
- Prise en charge des boutons matériels (BTN1/BTN2) via `ENABLE_BUTTONS` sans modifier le pin mapping.
  - BTN1 : appui court → bip bref du buzzer (feedback utilisateur).
  - BTN2 : appui court → cycle des couleurs de la LED RGB (rouge → vert → bleu → blanc).

## Documentation
- Mise à jour des README (EN/FR) avec la version `v3.17.0` et le comportement des boutons.
- Mise à jour de `docs/CONFIG*.md` pour documenter `ENABLE_BUTTONS` et les remarques liées aux broches.
- Mise à jour des entêtes de `docs/FEATURE_MATRIX*.md` vers `v3.17.0` ; ajout de la ligne Boutons (EN) et correction du mapping des broches TFT.

## Build
- Incrément de `PROJECT_VERSION` à `3.17.0` dans `platformio.ini`.

## Notes
- Aucun changement de mapping ; `PIN_BUTTON_1`/`PIN_BUTTON_2` restent définis par cible dans `include/config.h`.
- Sur ESP32 Classic, les GPIO 34–39 n'ont pas de pull-up interne ; prévoir une résistance externe si nécessaire lorsque BTN2=35.
