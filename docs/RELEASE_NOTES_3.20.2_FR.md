# Notes de version — 3.20.2 (2025-12-07)

## Modifié
- **Références des Pins dans l'Interface Web :** Les valeurs GPIO codées en dur dans l'interface web sont remplacées par des références dynamiques depuis `board_config.h`
   - Les pins LED RGB (R/G/B) référencent désormais `RGB_LED_PIN_R`, `RGB_LED_PIN_G`, `RGB_LED_PIN_B` au lieu des valeurs codées en dur
   - Le pin du capteur DHT référence désormais `DHT_PIN` au lieu d'une valeur codée en dur
   - Le pin du capteur de lumière référence désormais `LIGHT_SENSOR_PIN` au lieu d'une valeur codée en dur
   - Les pins de détente/écho du capteur de distance référencent désormais `DISTANCE_TRIG_PIN` / `DISTANCE_ECHO_PIN` au lieu de valeurs codées en dur
   - Le pin du capteur de mouvement référence désormais `MOTION_SENSOR_PIN` au lieu d'une valeur codée en dur
   - Le pin du buzzer/PWM référence désormais `PWM_PIN` au lieu d'une valeur codée en dur
- Les valeurs de pin sont injectées comme constantes JavaScript au chargement de la page via la route `/js/app.js`, garantissant que l'interface affiche toujours les pins compilés corrects pour la cible
- Ajout d'une bannière d'identification TARGET au démarrage avec les valeurs réelles des pins pour le débogage

## Corrigé
- L'interface web affiche désormais correctement les pins GPIO réels en fonction de la cible compilée (ESP32-S3 vs ESP32 CLASSIC)
- L'erreur ReferenceError en JavaScript est résolue en injectant les constantes de pins avant le code PROGMEM statique

## Détails Techniques
- Modification de `handleJavaScriptRoute()` dans `main.cpp` pour injecter les constantes de pins avant de servir le JavaScript
- Ajout de déclarations extern dans `include/web_interface.h` pour toutes les variables de pins
- Les valeurs de pins sont générées dynamiquement depuis les défauts `board_config.h` lors de la compilation
- Les logs de débogage affichent les valeurs de pins sur le moniteur série lorsque le JavaScript est chargé

## Notes
- Ce patch garantit que le tableau de bord web reflète toujours les pins réelles du firmware compilé, éliminant les décalages entre l'affichage de l'interface et la configuration matérielle.
- `PROJECT_VERSION` est positionné à `3.20.2` dans `platformio.ini`.

## Mise à jour
- Recompilé et flashé avec le dernier code (`pio run -e esp32devkitc --target upload` pour CLASSIC ou `pio run -e esp32s3_n16r8 --target upload` pour ESP32-S3).
- Actualisez fortement l'interface web (Ctrl+Shift+R dans les navigateurs) pour effacer tout JavaScript mis en cache.
- Consultez le moniteur série (115200 bauds) au démarrage pour voir la bannière TARGET avec les valeurs réelles des pins.

