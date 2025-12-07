# Notes de version — 3.20.1 (2025-12-07)

## Corrigé
- **Stabilité USB/OTG (ESP32-S3) :** Libération des lignes USB D-/D+ (GPIO19/20) en déplaçant les défauts vers des broches sûres.
  - I2C par défaut : SDA=15, SCL=16
  - LED RGB : Rouge=21, Vert=45, Bleu=47 (Rouge quitte GPIO19)
  - GPIO45 est une broche de strapping ; la LED reste éteinte au boot pour éviter un mode de démarrage incorrect.

## Notes
- L'instabilité OTG venait du trafic I2C et de la LED rouge sur GPIO19/20, qui sont les lignes USB natives. Avec le nouveau mapping, laissez GPIO19/20 libres de tout périphérique.
- `PROJECT_VERSION` est positionné à `3.20.1` dans `platformio.ini`.

## Mise à niveau
- Recompilez et flashez avec les nouveaux défauts (`pio run -e esp32s3_n16r8 --target upload`).
- Si vous aviez des surcharges de pins, alignez-les sur le nouveau mapping pour garder l'USB stable.
