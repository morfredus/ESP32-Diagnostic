# ESP32 Diagnostic Suite – Compilation & déploiement (v3.9.0)

Ce document décrit les toolchains supportées et la checklist recommandée pour livrer le firmware 3.8.0.

> **Important :** La version 3.9.0 a été migrée depuis Arduino IDE vers **PlatformIO**. Le dépôt original [ESP32-Diagnostic](https://github.com/morfredus/ESP32-Diagnostic) est maintenant **archivé**. Le support Bluetooth/BLE a été **supprimé**.

## Toolchains
Le projet utilise PlatformIO pour la gestion de compilation. Toutes les dépendances sont déclarées dans `platformio.ini` :

1. Ouvrir le projet dans **Visual Studio Code** avec l'extension **PlatformIO IDE**.
2. Sélectionner votre environnement cible (ex. `esp32-s3-devkit`).
3. Lancer **Build** pour compiler, puis **Upload** pour flasher.
4. Ouvrir **Serial Monitor** à 115200 bauds pour vérifier.

Alternativement, utiliser la ligne de commande PlatformIO :
```bash
pio run --target upload
pio device monitor --baud 115200
```

**Note :** Arduino IDE et Arduino CLI ne sont **plus supportés** pour cette version. Utiliser le dépôt archivé [ESP32-Diagnostic](https://github.com/morfredus/ESP32-Diagnostic) pour la compatibilité Arduino IDE.

## Checklist pré-déploiement
- [ ] Mettre à jour `wifi-config.h` avec les identifiants production et, le cas échéant, les paramètres entreprise.
- [ ] Vérifier que `DIAGNOSTIC_VERSION` vaut `3.8.0` dans le firmware et la documentation.
- [ ] Compiler les ressources multilingues sans avertissement (`languages.h`).
- [ ] Exécuter un cycle complet sur une carte de référence et exporter les rapports JSON/CSV.
- [ ] Capturer des captures d'écran ou impressions du tableau de bord pour les notes de version si nécessaire.

## Tests d'acceptation
| Test | Procédure | Résultat attendu |
|------|-----------|------------------|
| Smoke test connectivité | Connexion au Wi-Fi labo, lancer un diagnostic rapide. | PASS avec RSSI > -70 dBm, ping < 40 ms. |
| Endurance mémoire | Lancer 5 diagnostics complets consécutifs. | Aucune saturation du tas, PSRAM stable. |
| Validation périphériques | Brancher OLED + chaîne NeoPixel, activer les modules. | PASS avec FPS stable et animation cohérente. |
| Sanity REST API | Déclencher `/api/run` puis `/api/status`. | File d'attente reçue et payload JSON valide. |

## Packaging release
1. Taguer le dépôt avec `v3.9.0` (tag annoté).
2. Joindre les binaires compilés (`.bin`) pour chaque carte si diffusion via GitHub Releases.
3. Publier les notes en se référant au [CHANGELOG_FR.md](../CHANGELOG_FR.md).
4. Partager les liens du wiki mis à jour (`docs/home.md`) avec les équipes support.

## Considérations OTA
- La diffusion principale reste le flash USB. Pour l'OTA, intégrer votre bootloader et vérifier que le binaire reste sous la taille
  de partition OTA.
- Toujours valider la signature ou le checksum après transfert OTA pour éviter un diagnostic corrompu.

## Stratégie de rollback
- Conserver une image stable (ex. v3.2.21-maint) prête pour un retour arrière d'urgence.
- Documenter les étapes de rollback dans la procédure interne en se référant à ce guide pour le reflashing.

## Ressources associées
- [Guide d'installation](INSTALL_FR.md)
- [Guide de configuration](CONFIG_FR.md)
- [Notes de version](../CHANGELOG_FR.md)
