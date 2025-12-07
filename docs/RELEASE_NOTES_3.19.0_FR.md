# Notes de version — 3.19.0 (2025-12-07)

## Modifié
- **Pin mapping extrait dans `board_config.h` :** La cartographie GPIO spécifique carte est isolée dans un en-tête dédié, `config.h` ne contenant plus que les réglages communs. Les valeurs restent identiques ; les boutons ESP32-S3 restent sur GPIO 38/39 pour éviter les conflits d'upload/reset.
- **Renommage des secrets Wi-Fi :** `wifi-config.h` → `secrets.h` (avec `secrets-example.h`). Le firmware inclut désormais `secrets.h` ; les anciens fichiers génèrent une erreur de compilation pour éviter toute utilisation involontaire.
- **Renforcement `.gitignore` :** Mise en avant de `include/secrets.h` (l'ancien nom reste ignoré) pour éviter de publier des identifiants.
- **Version augmentée :** `PROJECT_VERSION` passé à `3.19.0` dans `platformio.ini`.

## Documentation
- README (EN/FR), guides CONFIG, références de mapping, checklists d'installation/déploiement, FAQ, dépannage, sécurité, schéma d'architecture et guide d'usage mis à jour pour refléter `board_config.h`, les nouveaux boutons et `secrets.h`.

## Notes de mise à jour
- Copiez `include/secrets-example.h` en `include/secrets.h` puis renseignez vos SSID/mots de passe (ne versionnez pas ce fichier).
- Si vous aviez des modifications locales de pins, déplacez-les dans `include/board_config.h` ; les autres options runtime restent dans `config.h`.