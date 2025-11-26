# Changelog (FR)

Toutes les évolutions notables d'ESP32 Diagnostic Suite sont documentées ici. Ce projet suit [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Version 3.12.3] - 2025-11-26
### Modifications
- Pins par défaut du **HC‑SR04** définies à `TRIG=16`, `ECHO=17` dans `config.h` et valeurs par défaut alignées dans l'interface web.

### Ajouts
- Nouveau mémo de référence rapide : `docs/PIN_MAPPING_FR.md` (EN : `docs/PIN_MAPPING.md`).

### Changements Techniques
- Incrément de `PROJECT_VERSION` vers 3.12.3 dans `platformio.ini`.
- Aucun autre changement fonctionnel.

## [Version 3.12.2] - 2025-11-26
### Corrections
- **HC‑SR04** : Séquence de mesure durcie en attendant que ECHO repasse à LOW avant déclenchement et en utilisant `pulseInLong()` avec timeout étendu. Évite les faux résultats « No echo ».

### Changements Techniques
- Aucun changement de pin mapping. Ajustements limités à la séquence de mesure et à la gestion du timeout.

## [Version 3.12.1] - 2025-11-26
### Corrections
- **Activation PSRAM** : Activation garantie de la PSRAM sur ESP32-S3 DevKitC-1 N16R8 sous PlatformIO via `board_build.psram = enabled` et définition de `BOARD_HAS_PSRAM`. Corrige les échecs des tests mémoire utilisant la PSRAM externe.

### Améliorations
- Configuration PlatformIO affinée pour une détection et une utilisation cohérentes de la PSRAM avec Arduino-ESP32 3.3.x.
- Mise à jour des documentations FR/EN et des références de version vers 3.12.1.

### Changements Techniques
- Incrément de `PROJECT_VERSION` vers 3.12.1 dans `platformio.ini`.
- Aucun changement de pin mapping (config.h inchangé).

## [Version 3.12.0] - 2025-11-26
### Corrections
- **CONFIG** : Validation et confirmation de la configuration du pin backlight TFT (GPIO 15).
- Résolution des déclarations TFT dupliquées causant des avertissements de compilation.
- Correction de la documentation du mapping des pins dans config.h pour ESP32-S3 DevKitC-1 N16R8.

### Améliorations
- **DOCUMENTATION** : Suppression des fichiers de développement et débogage obsolètes pour une structure de dépôt plus claire.
- Suppression des fichiers temporaires : CORRECTIFS_APPLIQUES.md, CORRECTIF_v3.11.1.md, DEBUGGING_WEB_UI.md, PATCH_WEB_UI.cpp, RESUME_FINAL.md, RESUME_v3.11.1_FINAL.md.
- Consolidation des commentaires et de l'organisation du mapping des pins dans config.h.
- Amélioration de la configuration PlatformIO avec paramètres PSRAM optimisés.

### Changements Techniques
- Finalisation de la configuration des pins TFT pour utilisation en production sur ESP32-S3 N16R8.
- Nettoyage du dépôt des artefacts de développement et notes de maintenance.
- Standardisation de la structure et des commentaires du fichier de configuration.
- Mise à jour de la version vers 3.12.0 dans platformio.ini.

## [Version 3.11.4] - 2025-11-25
### Améliorations
- **MAINTENANCE** : Améliorations de la qualité du code et nettoyage.
- Suppression de l'historique obsolète des versions de développement dans les en-têtes du code source.
- Suppression de la fonction `handleJavaScript()` inutilisée (élimination du code mort).
- Simplification et standardisation du style de commentaires dans toute la base de code.
- Correction de la faute de frappe en français : "defaut" → "défaut" dans les messages de configuration.

### Changements Techniques
- Nettoyage des commentaires de versions obsolètes (v3.8.x-dev à v3.10.3).
- Suppression de la fonction `handleJavaScript()` jamais référencée dans le routage.
- Normalisation des délimiteurs de commentaires et suppression des annotations redondantes.
- Mise à jour de la version vers 3.11.4 dans platformio.ini.

## [Version 3.11.3] - 2025-11-25
### Corrections
- **CONFIG** : Correction du pin backlight TFT de GPIO 48 à GPIO 15 pour résoudre le conflit avec NeoPixel.
- Le rétro-éclairage du TFT utilise maintenant le GPIO 15 dédié au lieu du GPIO 48 (conflit NeoPixel).

### Changements Techniques
- Mise à jour de la définition `TFT_BL` dans `config.h` du pin 48 au pin 15.
- Assure le bon fonctionnement du rétro-éclairage TFT sans conflit matériel avec NeoPixel.

## [Version 3.11.2] - 2025-11-25
### Corrections
- **BUILD** : Correction de l'erreur de typage FPSTR() empêchant la compilation.
- Correction de la gestion des types de pointeurs pour les chaînes PROGMEM dans l'implémentation du transfert par morceaux.
- Changement de `const char* staticJs = FPSTR(...)` vers un modèle d'accès PROGMEM approprié pour ESP32.
- Résolution de l'erreur de compilation `cannot convert 'const __FlashStringHelper*' to 'const char*'`.

### Changements Techniques
- Mise à jour de `handleJavaScriptRoute()` pour utiliser l'accès direct au pointeur PROGMEM pour la vérification.
- Maintien de l'implémentation du transfert par morceaux tout en corrigeant la compatibilité des types.
- Ajout de commentaires expliquant le mapping mémoire PROGMEM spécifique à ESP32.

## [Version 3.11.1] - 2025-11-25
### Corrections
- **CRITIQUE** : Correction du chargement JavaScript de l'interface web utilisant l'encodage de transfert par morceaux.
- Remplacement de la génération JavaScript monolithique par un streaming mémoire-efficient par morceaux.
- Amélioration des logs de débogage pour afficher la répartition de la taille JavaScript (préambule, traductions, code statique).
- Correction des problèmes de dépassement mémoire quand le JavaScript dépassait la taille du tampon alloué.

### Changements Techniques
- Implémentation de l'encodage de transfert par morceaux dans `handleJavaScriptRoute()`.
- Séparation de la génération JavaScript en trois parties : préambule, traductions, code statique.
- Ajout de vérification PROGMEM pour détecter les fonctions manquantes avant envoi.
- Réduction de la pression mémoire en streamant le contenu JavaScript au lieu de le mettre en tampon.

## [Version 3.11.0] - 2025-11-25
### Ajouts
- **NOUVELLE FONCTIONNALITÉ** : Support de l'écran TFT ST7789 avec résolution 240x240.
- Écran de démarrage sur l'affichage TFT montrant l'initialisation du système.
- Visualisation en temps réel de l'état de connexion WiFi sur le TFT.
- Affichage de l'adresse IP sur le TFT une fois connecté.
- Pins TFT configurables (MOSI, SCLK, CS, DC, RST, Backlight) dans config.h.
- Nouveau fichier d'en-tête tft_display.h pour la gestion de l'affichage TFT.

### Corrections
- Améliorations de l'initialisation de l'interface web et du chargement des onglets.
- Amélioration de la gestion des erreurs JavaScript pour une meilleure réactivité de l'interface.

### Améliorations
- Meilleur retour visuel pendant le processus de démarrage avec l'écran TFT.
- Support de double affichage (OLED + TFT) pour des diagnostics améliorés.

## [Version 3.10.3] - 2025-11-25
### Ajouts
- Aucun.

### Corrections
- **CRITIQUE** : Correction de l'erreur de compilation `portGET_ARGUMENT_COUNT()` dans les macros FreeRTOS.
- Changement de la plateforme depuis la version git instable vers la version stable `espressif32@6.5.0`.
- Ajout du build flag `-DCONFIG_FREERTOS_ASSERT_ON_UNTESTED_FUNCTION=0` pour éviter les conflits de macros FreeRTOS.

### Améliorations
- Stabilité de compilation améliorée en utilisant une version stable de la plateforme au lieu du dépôt git.
- Compatibilité améliorée avec FreeRTOS et le framework Arduino-ESP32.

## [Version 3.10.2] - 2025-11-25
### Ajouts
- Aucun.

### Corrections
- **CRITIQUE** : Correction des flags de compilation C++17 causant un crash au démarrage.
- Ajout de `build_unflags = -std=gnu++11` pour forcer le remplacement du standard C++ par défaut.
- Changement de `-std=gnu++17` vers `-std=c++17` pour une conformité C++17 stricte.
- Résolution des problèmes d'initialisation des variables inline empêchant le fonctionnement de l'interface web et de l'écran OLED.

### Améliorations
- Nettoyage des flags de compilation dupliqués dans `platformio.ini`.
- Configuration du système de build améliorée pour un meilleur support C++17.

## [Version 3.9.0] - 2025-11-11
### Ajouts
- Mise à jour de la documentation du projet et des références du dépôt pour le déploiement basé sur PlatformIO.
- Migration d'Arduino IDE vers PlatformIO pour une meilleure cohérence de compilation et gestion des dépendances.

### Corrections
- Aucune.

### Améliorations
- Mise à jour de toute la documentation pour refléter la toolchain PlatformIO et la nouvelle URL du dépôt.
- Standardisation des références de version dans toute la documentation du projet.
- Structure de dépôt améliorée pour un flux de travail de développement professionnel.

## [Version 3.8.14] - 2025-11-11
### Ajouts
- Aucun.

### Corrections
- **Critique** : Ajout de la déclaration manquante de la variable `runtimeBLE` qui causait des erreurs de compilation sur les cibles ESP32-S2/S3/C3/C6/H2.
- Suppression du tableau `DIAGNOSTIC_VERSION_HISTORY` inutilisé pour réduire l'encombrement du code.

### Améliorations
- Nettoyage des initialisations redondantes de `String` (`String foo = ""` → `String foo`) sur 7 instances.
- Standardisation de l'espacement des boucles `for` (`for(` → `for `) sur 23 instances pour améliorer la lisibilité.
- Suppression des commentaires superflus pour clarifier le code.
- Mise à jour des références de version vers 3.8.14.

## [Version 3.8.0] - 2025-11-08
### Ajouts
- Version de production basée sur 3.7.30 avec mise à jour de version pour le déploiement.

### Corrections
- Aucune.

### Améliorations
- Standardisation des références de version dans toute la documentation et les constantes firmware.

## [Version 3.7.30] - 2025-11-08
### Ajouts
- Les cartes équipées d'un écran OLED affichent désormais les phases de connexion Wi-Fi (association, DHCP, réussite) dès le démarrage, sans passer par le moniteur série.

### Corrections
- Protection des descripteurs NimBLE optionnels et alignement des callbacks BLE pour supprimer les erreurs d'inclusion manquante et `override` sur ESP32 et ESP32-S3.
- Validation du statut `start()` de NimBLE avant de copier les périphériques annoncés afin d'éviter toute conversion invalide.

### Améliorations
- Réutilisation sûre des tampons de scan NimBLE, reprise de la diffusion après un échec et réponses HTTP cohérentes lorsque le scan n'est pas disponible.
- Documentation FR/EN actualisée pour détailler l'écran Wi-Fi de démarrage et les consignes de compatibilité BLE renforcées.

## [Version 3.5.1] - 2025-10-30
### Ajouts
- Extension des liaisons de traduction en direct sur les panneaux de diagnostics afin que les widgets sans fil, pager, GPIO et exports réutilisent le catalogue commun sans doublons.

### Corrections
- Résolution des métadonnées carte non traduites (cœurs CPU, adresses MAC, raisons de reset, métriques mémoire) pour que le changement de langue reste synchronisé sans rechargement.

### Améliorations
- Raffinement du rafraîchisseur de traduction côté client pour réappliquer instantanément préfixes, suffixes et placeholders lors d'une bascule de langue.

---

## [Version 3.4.0] - 2025-10-29
### Ajouts
- Mutualisation des prototypes des helpers JSON afin d'exposer des signatures cohérentes entre modules de diagnostic.

### Corrections
- Résolution des échecs de compilation causés par des déclarations de helpers JSON divergentes selon les modules.
- Rétablissement des conversions `String` sûres dans la chaîne de traduction après l'optimisation du catalogue.

### Améliorations
- Unification des builders de réponses JSON HTTP et des handlers périphériques pour réduire les duplications dans les exports.
- Harmonisation des libellés bilingues et retrait du diagnostic touchpad historique de la séquence par défaut.
- Mise à jour complète de la documentation FR/EN afin de refléter la stabilisation de la maintenance 3.3.x.

---

## [Version 3.3.0] - 2025-10-27
### Ajouts
- Néant.

### Corrections
- Suppression des bannières de commentaires « NEW FEATURE » devenues trompeuses pour les passages de maintenance.

### Améliorations
- Consolidation des notes historiques « Version de dev » dans une constante firmware afin de préserver la mémoire du projet sans dupliquer les bannières.
- Synchronisation du commentaire d'en-tête et de la macro `DIAGNOSTIC_VERSION` sur 3.3.0 avec une documentation mise à jour autour de cette maintenance.

---

## [Version 3.2.0] - 2025-10-29
### Ajouts
- Documentation de la requête `/api/memory-details` et des alertes de fragmentation issues de la dernière campagne de tests.
- Intégration, dans les guides d'utilisation et de dépannage, de la checklist Bluetooth® et Wi-Fi validée lors du débogage post-release.

### Corrections
- Remplacement des mentions 3.1.19 restantes dans les README et guides afin d'aligner bannière, documentation et changelog bilingue sur 3.2.0.
- Clarification des consignes de changement de langue pour éviter les instructions obsolètes lors de la vérification de `/api/set-language`.

### Améliorations
- Guides d'installation, de configuration et d'utilisation étoffés avec des étapes de vérification concrètes (exports mémoire, widget BLE, métadonnées de scan Wi-Fi).
- Guide de contribution renforcé pour rappeler la synchronisation systématique des contenus FR/EN lors des maintenances.

---

## [Version 3.1.19] - 2025-10-28
### Ajouts
- Néant.

### Corrections
- Mise à jour de l'ensemble des guides pour référencer le duo bilingue (`CHANGELOG.md` et `CHANGELOG_FR.md`) et conserver la cohérence FR/EN.

### Améliorations
- Scission du journal des modifications en fichiers dédiés anglais et français afin de faciliter les maintenances ciblées.
- Rafraîchissement du bandeau de version, de `DIAGNOSTIC_VERSION` et des encarts de version 3.1.19 dans toute la documentation.

---

## [Version 3.1.18] - 2025-10-27
### Ajouts
- Néant.

### Corrections
- Validation de `/api/set-language` pour renvoyer immédiatement HTTP 200 sur `fr`/`en` et HTTP 400 sur les codes non pris en charge, supprimant les réponses ambiguës.
- Synchronisation des journaux série et exports avec les nouvelles réponses JSON du changement de langue.

### Améliorations
- Réservation anticipée du tampon de `jsonEscape` afin de réduire les allocations pendant la génération d'exports.
- Inclusion explicite de `<cstring>` pour assurer la compatibilité des fonctions de chaîne C sur les toolchains Arduino récentes.
- Documentation FR/EN alignée sur la maintenance 3.1.18 (README, guides d'installation, configuration, utilisation, architecture, contribution et dépannage).

---

## [Version 3.1.16] - 2025-10-27
### Ajouts
- Bandeau sticky unifié affichant version, statut Wi-Fi/Bluetooth et lien d'accès direct depuis le tableau de bord web.
- Commandes Bluetooth® (activation, renommage, réinitialisation) exposées dans l'interface et via les endpoints REST dédiés.

### Corrections
- Délégation d'événements repositionnée pour la navigation par onglets afin de restaurer la sélection après rechargement dynamique.
- Actualisation des traductions sur les onglets et éléments dynamiques après un changement de langue pour éviter les libellés incohérents.
- Rétablissement du libellé « Non testé » sur les diagnostics additionnels dans l'interface, l'API et les exports.

### Améliorations
- Réponses de scan Wi-Fi enrichies (BSSID, bande, largeur de canal, mode PHY) pour faciliter l'analyse radio.
- Préallocation étendue de `htmlEscape` afin de limiter les réallocations lors du streaming HTML et des exports.
- Synchronisation du commentaire d'en-tête et de `DIAGNOSTIC_VERSION` sur 3.1.16 pour aligner journaux et fichiers générés.

---

## [Version 3.1.15-maint] - 2025-10-26
### Ajouts
- Néant.

### Corrections
- Uniformisation du libellé « Non testé » pour les diagnostics additionnels (ADC, tactile, PWM, stress) dans l'interface, les exports et l'API.
- Synchronisation du commentaire d'en-tête et de `DIAGNOSTIC_VERSION` sur 3.1.15-maint afin d'afficher le bon numéro dans les journaux et fichiers exportés.

### Améliorations
- Extension de la préallocation de `htmlEscape` pour réduire les réallocations lors de l'échappement des chaînes HTML générées.
- Documentation FR/EN actualisée (README, guides d'installation, configuration, utilisation, architecture, contribution et dépannage) pour refléter la maintenance 3.1.15-maint et les vérifications associées.

---

## [Version 3.1.14-maint] - 2025-10-26
### Ajouts
- Néant.

### Corrections
- Rétablissement des libellés "Non testé" pour les diagnostics additionnels afin d'assurer une interface cohérente en français.
- Alignement du commentaire de version et de `DIAGNOSTIC_VERSION` sur la révision 3.1.14-maint pour refléter la maintenance en cours.

### Améliorations
- Augmentation de la réserve mémoire utilisée par `htmlEscape` pour limiter les réallocations lors de l'échappement des chaînes.

---

## [3.1.1] - 2025-10-26
### Corrections
- Suppression des commentaires obsolètes et marqueurs internes pour clarifier la maintenance du firmware.

### Documentation
- Mise à jour des guides FR/EN, exports et constantes pour refléter le passage en version 3.1.1.

---

## [3.1.0] - 2025-10-24
### Points forts
- Activation automatique du service Bluetooth® Low Energy avec diffusion native sur les cibles compatibles (ESP32, S3, C3, C6, H2).
- Tableau de bord web enrichi d'une cartouche BLE détaillant l'état, le nom du périphérique et les journaux d'appairage récents.
- Jeux de documents FR/EN entièrement réécrits couvrant installation, configuration, utilisation, architecture, dépannage et contribution.

### Fonctionnalités
- Service GATT par défaut avec renommage dynamique du périphérique via l'interface web ou la console série.
- Nouveaux messages d'état Wi-Fi contextualisés (association, DHCP, authentification, portail captif) affichés dans le bandeau supérieur et l'API REST.
- Prise en charge initiale assouplie pour les révisions récentes des SoC ESP32 (S3/C3/C6/H2) sur les modules BLE et Wi-Fi.

### Interface & API
- Navigation par onglets fiabilisée via délégation d'événements JavaScript, évitant les blocages après un rafraîchissement partiel.
- Menu supérieur responsive avec regroupement monoligne des indicateurs de statut pour les écrans < 768 px.
- Export TXT/JSON/CSV et aperçu imprimable synchronisés avec les nouveaux états BLE et Wi-Fi.

### Documentation
- Regroupement des guides dans `docs/` avec renvois croisés vers les versions anglaises et françaises.
- Ajout d'un guide de contribution détaillé (workflow Git, validation, formatage) et de procédures de dépannage spécifiques BLE/Wi-Fi.

### Corrections
- Harmonisation des messages BLE entre la console série, l'API `/api/status` et l'interface web.
- Nettoyage du JSON de traduction dynamique pour éviter les clés orphelines et incohérences de casse.
- Clarification de la détection des onglets actifs dans le bundle JavaScript, supprimant les doublons d'écouteurs.

---

## [2.6.0] - 2025-10-15
### Fonctionnalités
- Boutons manuels sur l'interface web et endpoints REST associés pour piloter individuellement chaque animation de diagnostic OLED.
- Possibilité de déclencher et d'arrêter les séquences d'affichage directement depuis la console série.

### Améliorations
- Simplification du flux de reconfiguration I²C OLED : sélection des broches SDA/SCL et vitesse directement depuis l'interface.
- Actualisation du pack de traductions (FR/EN) pour tous les nouveaux libellés OLED et états d'exécution.

### Corrections
- Suppression complète de la prise en charge TFT (firmware, dépendances Arduino, fragments UI), réduisant la taille du binaire et les avertissements de compilation.

---

## [2.5.1] - 2025-10-10
### Corrections
- Statuts PSRAM clarifiés dans le tableau de bord, les exports et les rapports imprimables (valeur détectée, fréquence, mode octal).
- Ajustement de l'ordre de détection PSRAM pour éviter les faux négatifs sur ESP32-WROVER.

### Améliorations
- Mise en avant des cartes compatibles PSRAM avec rappels pour activer l'option dans l'IDE Arduino et la documentation d'installation.

---

## [2.5.0] - 2025-10-08
### Fonctionnalités
- Exports TXT/JSON/CSV intégralement traduits (FR/EN) avec inclusion de l'horodatage ISO 8601 et du numéro de build.
- Prévisualisation imprimable alignée sur la langue sélectionnée et sur les nouveaux blocs de diagnostic.

### Améliorations
- Nommage des fichiers d'export enrichi avec le numéro de version, la date et la carte détectée.
- Révision du workflow d'export REST pour réduire les temps de génération et homogénéiser les intitulés de colonnes.

### Corrections
- Harmonisation des formats de date (localisation FR/EN) dans les exports et la vue imprimable.
- Ajout des traductions manquantes sur les boutons de téléchargement et sur les libellés de sections.

---

## [2.4.0] - 2025-10-07
### Fonctionnalités
- Interface multilingue complète FR/EN avec bascule à chaud sans rechargement de page.
- Endpoints `/api/set-language` et `/api/get-translations` permettant aux clients externes de piloter la langue.
- Fichier `languages.h` regroupant plus de 150 chaînes traduites pour l'interface, les exports et la console série.

### Améliorations
- Tous les textes (web, API, exports, logs) reposent sur le catalogue de traductions centralisé.
- Ajout d'un indicateur visuel de langue active et persistance de la préférence dans le stockage local du navigateur.

---

## [2.3.0] - 2025-10-06
### Fonctionnalités
- Batterie de 10 tests OLED 0,96" I²C (bascule contrastes, inversion, scroll, trames personnalisées) avec messages explicatifs.
- Reconfiguration dynamique des broches SDA/SCL via l'interface web et l'API pour faciliter le recâblage.

### Améliorations
- Détection automatique de l'écran à l'adresse 0x3C avec relance des tests après connexion.
- Ajout d'un module de calibration de contraste pour optimiser les OLED selon la tension d'alimentation.

---

## [2.2.0] - 2025-10-05
### Fonctionnalités
- Support des écrans TFT SPI (ST7789/ILI9341) avec diagnostics dédiés (test couleurs, patterns, fréquence SPI).
- Assistant de configuration TFT (pinout, rotation, inversion des axes) accessible via l'interface et la console série.

### Notes
- Cette fonctionnalité a été retirée en 2.6.0 pour alléger le firmware et se concentrer sur les OLED.

---

## [2.0.0] - 2025-10-03
### Fonctionnalités
- Réécriture complète du diagnostic : GPIO, ADC, pads tactiles, PWM, I2C, SPI, PSRAM, partitions, stress test, benchmarks et exports.
- Support multi-Wi-Fi via `WiFiMulti`, découverte mDNS `ESP32-Diagnostic.local` et interface web modernisée.
- Ajout de l'API REST (lancement tests, récupération de rapports) et des exports TXT/JSON/CSV.

### Documentation
- Première version du guide d'utilisation détaillant la configuration réseau, les tests disponibles et la collecte des journaux.

---

## [1.0.0] - 2025-10-01
### Fonctionnalités
- Première version publique avec interface web basique, rapport mémoire, tests GPIO/ADC élémentaires et console série interactive.
- Génération d'un tableau de bord unique regroupant la configuration Wi-Fi, les mesures de capteurs et les actions rapides.

### Notes
- Base de code initiale posant les fondations du système de diagnostics évolutif.
