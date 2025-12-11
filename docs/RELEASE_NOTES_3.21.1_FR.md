# Notes de Version - Version 3.21.1

**Date de publication**: 11 décembre 2025  
**Type**: Version de fonctionnalité - Indicateur d'état Wi-Fi NeoPixel

## Aperçu

La version 3.21.1 améliore l'expérience utilisateur avec un indicateur d'état Wi-Fi en temps réel utilisant les LED RGB NeoPixel/WS2812. Le firmware fournit maintenant une rétroaction visuelle immédiate lors de la connexion Wi-Fi, de la déconnexion et des opérations système sans nécessiter d'accès à l'interface web.

## Nouveautés

### Indicateur d'état Wi-Fi NeoPixel

**Rétroaction de connexion en temps réel :**
- **Initialisation automatique** au démarrage - La NeoPixel s'allume pendant la séquence d'amorçage
- **États de connexion visualisés** :
  - **Jaune (50, 50, 0)** : Connexion Wi-Fi en cours lors du démarrage
  - **Battement vert (0, 50, 0) / (0, 10, 0)** : Connecté avec succès - pulsation toutes les 1 seconde
  - **Battement rouge (50, 0, 0) / (10, 0, 0)** : Wi-Fi déconnecté - pulsation toutes les 1 seconde
- **Fonctionnement non bloquant** : Le battement géré dans la boucle principale avec une fréquence de mise à jour de 1 Hz
- **Zéro interférence** : L'état de la LED se met en pause lors des tests matériels et de l'exécution de motifs manuels

### Confirmation du reboot du bouton BOOT

**Confirmation visuelle du redémarrage :**
- **Flash violet (255, 0, 255)** s'affiche immédiatement lors d'un appui long (2 secondes) du bouton BOOT
- Confirme la demande de redémarrage avant le redémarrage du système
- Synchronisé avec la barre de progression TFT et la sortie série
- Fournit une rétroaction visuelle claire indépendante de la disponibilité de l'interface web

### Isolation des tests et motifs

**Environnement de test propre :**
- Le battement Wi-Fi NeoPixel se met automatiquement en pause lors de :
  - L'exécution de `/api/neopixel-test` (modes asynchrone et synchrone)
  - L'exécution de `/api/neopixel-pattern` (modes arc-en-ciel, clignotement, fondu, poursuite, couleur)
  - Les changements de couleur manuels via `/api/neopixel-color`
- Le battement se restaure automatiquement à la fin du test/motif
- Empêche les interférences entre l'état du système et les animations de test

### Mappages GPIO affectés

**Aucun changement par rapport à v3.21.0** - NeoPixel utilise la configuration GPIO existante :
- **ESP32-S3** : GPIO 48 (défaut)
- **ESP32 Classic** : GPIO 2 (défaut, configurable via `CUSTOM_LED_PIN` dans `board_config.h`)

## Détails techniques

### Implémentation

**Nouvelles fonctions ajoutées :**
- `neopixelShowConnecting()` - Affiche le jaune pendant les tentatives de connexion Wi-Fi
- `neopixelSetWifiState(bool)` - Définit l'état NeoPixel en fonction de l'état de connexion Wi-Fi
- `updateNeoPixelWifiStatus()` - Mise à jour non bloquante du battement (appelée à partir de la boucle principale)
- `neopixelPauseStatus()` / `neopixelResumeStatus()` - Met en pause/reprend l'indicateur Wi-Fi
- `neopixelRestoreWifiStatus()` - Restaure l'état Wi-Fi après la fin du test
- `neopixelShowRebootFlash()` - Affiche un flash violet lors de l'appui sur le bouton BOOT

**Fonctions modifiées :**
- `setup()` - Initialise NeoPixel avant la connexion Wi-Fi, appelle pause/reprend lors de la configuration Wi-Fi
- `loop()` - Ajout de l'appel `updateNeoPixelWifiStatus()` pour la gestion du battement
- `onButtonBootLongPress()` - Ajout de l'appel `neopixelShowRebootFlash_Impl()`
- `runNeopixelTestTask()` - Pause/reprend l'état Wi-Fi autour des tests asynchrones
- `handleNeoPixelTest()` - Pause/reprend l'état Wi-Fi autour des tests synchrones
- `handleNeoPixelPattern()` - Pause/reprend l'état Wi-Fi pendant l'exécution du motif

**Nouvelles variables d'état global :**
- `volatile bool neopixelStatusPaused` - Drapeau pour mettre en pause le battement Wi-Fi
- `unsigned long neopixelHeartbeatPreviousMillis` - Timing pour le battement de 1 Hz
- `const unsigned long NEOPIXEL_HEARTBEAT_INTERVAL_MS = 1000` - Intervalle de 1 seconde
- `bool neopixelHeartbeatState` - Basculer pour l'effet de pulsation de luminosité
- `bool neopixelStatusKnown` - Suivre si l'état Wi-Fi a été initialisé
- `bool neopixelLastWifiConnected` - Mettre en cache le dernier état Wi-Fi connu
- `bool neopixelConnecting` - Drapeau pour l'état de connexion en cours

### Compatibilité

- **Matériel** : Nécessite une LED NeoPixel/WS2812 sur le GPIO configuré
- **Versions** : Compatible en avant avec ESP32-S3 et ESP32 Classic à partir de v3.21.0
- **Pas de changements cassants** : Fonctionnalité purement additive, fonctionnalité existante inchangée
- **Flexibilité GPIO** : Supporte la configuration dynamique de GPIO via l'interface web

### Tests

✅ **Compilation** : Succès sur ESP32-S3 (esp32s3_n16r8) et ESP32 Classic (esp32devkitc)  
✅ **Qualité du code** : Aucun nouvel avertissement, déclarations antérieures correctement organisées  
✅ **Mémoire** : Empreinte minimale - suivi d'état constant uniquement, pas d'allocations dynamiques  

## Instructions de mise à jour

### De v3.21.0 à v3.21.1

1. Récupérez le dernier code du référentiel
2. Compilez : `pio run` ou utilisez la tâche de build PlatformIO
3. Téléchargez : `pio run --target upload` ou utilisez le bouton de téléchargement
4. **Aucune modification de configuration requise**
5. Aucun changement d'interface web ou de matériel nécessaire (GPIO NeoPixel inchangé)

### Vérification

Après le téléchargement, observez le comportement NeoPixel :
- **Au démarrage** : Flash jaune bref pendant la tentative de connexion Wi-Fi
- **Après connexion** : Motif de pulsation vert régulier (connecté) ou motif de pulsation rouge (non connecté)
- **Exécution de test** : La LED doit se mettre en pause lors des demandes `/api/neopixel-test` ou de motif
- **Bouton BOOT** : Un flash violet apparaît lors du maintien du BOOT pendant 2+ secondes

## Changements de comportement

| Scénario | v3.21.0 | v3.21.1 |
|----------|---------|---------|
| Connexion Wi-Fi | Aucune rétroaction LED | Jaune pendant la tentative, puis vert une fois connecté |
| Wi-Fi déconnecté | Aucune rétroaction LED | Motif de pulsation rouge |
| Test NeoPixel en cours | Le test affiche les effets | Le test affiche les effets, le battement s'arrête, restauré après |
| Redémarrage du bouton BOOT | Aucune rétroaction LED | Flash violet confirme le redémarrage |
| Boucle principale | Aucune mise à jour NeoPixel | Battement mis à jour chaque 1 seconde (non bloquant) |

## Changements cassants

**Aucun** - C'est une version de fonctionnalité purement additive avec compatibilité complète en arrière.

## Problèmes connus

Aucun identifié à la publication. Les problèmes connus existants de v3.21.0 restent inchangés.

## Dépendances

- **Adafruit_NeoPixel** : Déjà requise, aucune nouvelle dépendance
- **Arduino Core 3.3.3** : Aucun changement par rapport à v3.21.0
- **PlatformIO** : Aucun changement d'exigence de version

## Crédits

- Fonctionnalité de battement Wi-Fi NeoPixel : GitHub Copilot
- Intégration du flash de redémarrage du bouton BOOT : GitHub Copilot
- Tests et validation : morfredus

## Voir aussi

- **Guide d'utilisation** : `docs/USAGE.md` / `docs/USAGE_FR.md` - Section comportement NeoPixel
- **Matrice de fonctionnalités** : `docs/FEATURE_MATRIX.md` / `docs/FEATURE_MATRIX_FR.md` - Indicateurs LED mis à jour
- **Interface Web** : `docs/WEB_INTERFACE.md` / `docs/WEB_INTERFACE_FR.md` - Points d'accès API NeoPixel
