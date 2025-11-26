# ESP32 Diagnostic Suite – Accueil du wiki (v3.12.0)

Bienvenue sur le wiki officiel de la version **3.12.0**. Cet espace centralise tous les guides, références et checklists nécessaires
pour exploiter, personnaliser et maintenir la suite ESP32 Diagnostic au sein d'équipes bilingues.

## Sommaire
- [Vue d'ensemble](OVERVIEW_FR.md)
- [Matrice des fonctionnalités](FEATURE_MATRIX_FR.md)
- [Modules de diagnostic](DIAGNOSTIC_MODULES_FR.md)
- [Guide de l'interface web](WEB_INTERFACE_FR.md)
- [Référence REST API](API_REFERENCE_FR.md)
- [Compilation & déploiement](BUILD_AND_DEPLOY_FR.md)
- [Directives de sécurité](SECURITY_FR.md)
- [FAQ](FAQ_FR.md)
- Guides fondamentaux : [Installation](INSTALL_FR.md), [Configuration](CONFIG_FR.md), [Utilisation](USAGE_FR.md),
  [Dépannage](TROUBLESHOOTING_FR.md), [Architecture](ARCHITECTURE_FR.md), [Contribution](../CONTRIBUTING_FR.md)

## Mise en avant – 3.12.0
- **Version de production :** Configuration TFT validée et structure de dépôt nettoyée pour déploiement en production.
- **Support écran TFT ST7789 :** Support complet des écrans TFT 240x240 avec écran de démarrage et statut WiFi/IP en temps réel (v3.11.0+).
- **Double affichage :** Fonctionnement simultané OLED et TFT pour visualisation diagnostique améliorée.
- **Stabilité améliorée :** Résolution des problèmes de chargement JavaScript, erreurs de compilation FPSTR(), et optimisation mémoire avec livraison en streaming.
- **Validation configuration :** Finalisation du mapping des pins pour ESP32-S3 DevKitC-1 N16R8 avec rétro-éclairage TFT sur GPIO 15.
- **Optimisation PlatformIO :** Configuration PSRAM améliorée avec stabilité et performance accrues.

## Démarrage rapide
1. Suivre le [guide d'installation](INSTALL_FR.md) pour configurer la toolchain PlatformIO dans Visual Studio Code.
2. Configurer les identifiants Wi-Fi, la langue par défaut et les modules optionnels via [CONFIG_FR.md](CONFIG_FR.md).
3. Flasher le firmware et ouvrir le tableau de bord web embarqué sur `http://esp32-diagnostic.local/`.
4. Lancer un diagnostic rapide, consulter les [modules de diagnostic](DIAGNOSTIC_MODULES_FR.md) et exporter les rapports.

## Cartographie des connaissances
| Sujet | Anglais | Français |
|-------|---------|----------|
| Accueil | [home.md](home.md) | [home_FR.md](home_FR.md) |
| Vue d'ensemble | [OVERVIEW.md](OVERVIEW.md) | [OVERVIEW_FR.md](OVERVIEW_FR.md) |
| Matrice des fonctionnalités | [FEATURE_MATRIX.md](FEATURE_MATRIX.md) | [FEATURE_MATRIX_FR.md](FEATURE_MATRIX_FR.md) |
| Modules de diagnostic | [DIAGNOSTIC_MODULES.md](DIAGNOSTIC_MODULES.md) | [DIAGNOSTIC_MODULES_FR.md](DIAGNOSTIC_MODULES_FR.md) |
| Interface web | [WEB_INTERFACE.md](WEB_INTERFACE.md) | [WEB_INTERFACE_FR.md](WEB_INTERFACE_FR.md) |
| REST API | [API_REFERENCE.md](API_REFERENCE.md) | [API_REFERENCE_FR.md](API_REFERENCE_FR.md) |
| Compilation & déploiement | [BUILD_AND_DEPLOY.md](BUILD_AND_DEPLOY.md) | [BUILD_AND_DEPLOY_FR.md](BUILD_AND_DEPLOY_FR.md) |
| Sécurité | [SECURITY.md](SECURITY.md) | [SECURITY_FR.md](SECURITY_FR.md) |
| FAQ | [FAQ.md](FAQ.md) | [FAQ_FR.md](FAQ_FR.md) |
| Installation | [INSTALL.md](INSTALL.md) | [INSTALL_FR.md](INSTALL_FR.md) |
| Configuration | [CONFIG.md](CONFIG.md) | [CONFIG_FR.md](CONFIG_FR.md) |
| Utilisation | [USAGE.md](USAGE.md) | [USAGE_FR.md](USAGE_FR.md) |
| Dépannage | [TROUBLESHOOTING.md](TROUBLESHOOTING.md) | [TROUBLESHOOTING_FR.md](TROUBLESHOOTING_FR.md) |
| Architecture | [ARCHITECTURE.md](ARCHITECTURE.md) | [ARCHITECTURE_FR.md](ARCHITECTURE_FR.md) |
| Contribution | [../CONTRIBUTING.md](../CONTRIBUTING.md) | [../CONTRIBUTING_FR.md](../CONTRIBUTING_FR.md) |

## Support & escalade
- Licence : [MIT](../LICENSE)
- Suivi : [Issues GitHub](https://github.com/morfredus/ESP32-Diagnostic/issues)
- Historique : [CHANGELOG.md](../CHANGELOG.md) / [CHANGELOG_FR.md](../CHANGELOG_FR.md)
- Contributions : forkez le dépôt puis suivez le [guide de contribution](../CONTRIBUTING_FR.md).
- **Note :** La version originale Arduino IDE [ESP32-Diagnostic-Arduino-IDE](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE) est archivée.

Bon diagnostic ! Pensez à relier vos procédures internes à ce wiki pour garantir l'alignement avec la version 3.9.0.
