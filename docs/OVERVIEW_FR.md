# ESP32 Diagnostic Suite – Vue d'ensemble (v3.30.0)

> **AVERTISSEMENT** : Ce document reflète le firmware **v3.30.0** avec mappings de broches EXACTS depuis `include/board_config.h`. Toutes les assignations GPIO ont été vérifiées et synchronisées avec le code. Lisez [docs/PIN_MAPPING_FR.md](docs/PIN_MAPPING_FR.md) avant de flasher.

La suite ESP32 Diagnostic fournit une boîte à outils prête pour la production afin de valider les cartes basées sur ESP32 avant déploiement. La version **3.30.0** inclut la sélection dynamique du contrôleur TFT, la synchronisation complète des mappings GPIO, et le support multi-environnements avec trois configurations de build distinctes optimisées pour différentes variantes matérielles ESP32.

## Mission
## Dernière Mise à Jour : v3.30.0

**Nouveau en v3.30.0 :** Sélection dynamique du contrôleur TFT (ILI9341 ou ST7789) depuis l'interface Web et configuration de la résolution dans `config.h` sans recompilation. Voir [RELEASE_NOTES_3.30.0_FR.md](RELEASE_NOTES_3.30.0_FR.md) pour les détails.

---


- Proposer une méthodologie homogène pour qualifier les cartes ESP32-S3 et ESP32 avec plusieurs variantes matérielles.
- Supporter trois cartes spécifiques avec environnements dédiés : **ESP32-S3 N16R8** (principale), **ESP32-S3 N8R8**, et **ESP32 Classic DevKitC**.
- Mappings de broches spécifiques au matériel via compilation conditionnelle pour placement périphérique optimal.
- Offrir une interface web dédiée aux diagnostics en laboratoire comme sur le terrain, avec un minimum de configuration.
- Maintenir l'alignement entre firmware, documentation et rapports exportés afin de faciliter la traçabilité des anomalies.

| Domaine | Points clés |
|---------|-------------|
| Tableau de bord web | Interface HTML réactive servie directement par l'ESP32 avec indicateurs temps réel, journal d'activité et exports. |
| Vérifications automatiques | Planificateurs couvrant la connectivité, la mémoire, les bus de périphériques, l'affichage et les LEDs. |
| Reporting | Téléchargements en TXT/JSON/CSV, vue imprimable et hooks REST API pour l'intégration CI. |
| Localisation | Bascule FR/EN à l'exécution côté firmware (sortie série) et interface web. |
| Extensibilité | Registre modulaire de tests, bus d'événements pour callbacks personnalisés et helpers OLED/NeoPixel optionnels. |

## Nouveautés v3.30.0

- Sélection dynamique du contrôleur TFT (`ILI9341` ou `ST7789`) et configuration de la résolution dans `config.h` ou via l'interface web.
- Synchronisation complète des mappings GPIO avec le code source (`board_config.h`).
- Corrections et harmonisation des avertissements et de la documentation sur tous les modules.
- Voir la documentation et le changelog pour plus de détails.



## Carte du wiki
Consultez cette carte pour accéder directement à la bonne rubrique :

- [Accueil](home_FR.md) – porte d'entrée, points forts et objectifs généraux.
- [Installation](INSTALL_FR.md) – préparation de l'environnement, toolchain, pilotes USB et méthodes de flash.
- [Configuration](CONFIG_FR.md) – identifiants Wi-Fi, langue par défaut, ordonnancement des diagnostics et options de journalisation.
- [Utilisation](USAGE_FR.md) – workflows opérateur, formats d'export et interprétation du tableau de bord.
- [Architecture](ARCHITECTURE_FR.md) – couches firmware, planification des tâches, structure mémoire et gabarits d'interface.
- [Dépannage](TROUBLESHOOTING_FR.md) – procédures de récupération, matrice des codes d'erreur et scénarios de reset sécurisé.
- [Contribution](CONTRIBUTING_FR.md) – conventions de code, processus de revue et attentes en matière de tests.
- [Matrice des fonctionnalités](FEATURE_MATRIX_FR.md) – couverture matérielle par famille de cartes et périphériques optionnels.
- [Modules de diagnostic](DIAGNOSTIC_MODULES_FR.md) – description détaillée de chaque test automatisé et des métriques produites.
- [Guide de l'interface web](WEB_INTERFACE_FR.md) – anatomie de l'UI, états des composants et directives d'accessibilité.
- [Référence REST API](API_REFERENCE_FR.md) – endpoints, payloads, limites de débit et scénarios d'automatisation.
- [Compilation & déploiement](BUILD_AND_DEPLOY_FR.md) – compilation firmware, contrôles qualité et checklist de publication.
- [Directives de sécurité](SECURITY_FR.md) – renforcement Wi-Fi, rotation des identifiants et sécurité OTA.
- [FAQ](FAQ_FR.md) – réponses rapides aux questions d'intégration, support et déploiement les plus fréquentes.

## Terminologie
- **Cycle de diagnostic** – exécution ordonnée de tous les modules activés avec agrégation en RAM jusqu'à l'export.
- **Rapport instantané** – copie figée du dernier cycle, exportable via l'interface web, la REST API ou la commande série.
- **Profil contrôleur** – ensemble de configuration pré-définie (type de carte, options d'interface) chargé au démarrage.
- **Version de maintenance** – livraison axée sur la documentation et la stabilité sans modification de la surface fonctionnelle.

## Ressources associées
- [CHANGELOG_FR.md](../CHANGELOG_FR.md) – suivi chronologique des corrections et améliorations.
- [Table de compatibilité ESP-IDF](https://docs.espressif.com/projects/esp-idf/fr/latest/esp32/) pour comparer avec les attentes de la plateforme.
- [Issues GitHub](https://github.com/morfredus/ESP32-Diagnostic/issues) – suivi des anomalies et propositions.
- **Note :** La version originale Arduino IDE [ESP32-Diagnostic-Arduino-IDE](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE) est archivée.
