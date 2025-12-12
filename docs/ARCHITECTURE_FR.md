# Architecture du firmware (FR) – v3.22.1

> **AVERTISSEMENT** : v3.22.1 corrige des doublons de mapping pour ESP32 Classic et conserve le remapping GPIO pour ESP32-S3. Assurez-vous que votre câblage et la cible compilée correspondent aux broches documentées. Lisez [docs/PIN_MAPPING_FR.md](docs/PIN_MAPPING_FR.md) et [docs/PIN_MAPPING.md](docs/PIN_MAPPING.md) avant de flasher.

## Vue d'ensemble
Le firmware est un projet PlatformIO avec le code principal dans `src/main.cpp` qui orchestre les diagnostics, sert l'interface web et expose des endpoints REST. Les en-têtes dans `include/` regroupent les traductions, les modèles HTML et les utilitaires de configuration.

```
ESP32-Diagnostic/
├── platformio.ini            // Configuration PlatformIO pour toutes les cibles ESP32
├── src/
│   └── main.cpp              // boucle principale, ordonnanceur, gestion HTTP
├── include/
│   ├── languages.h           // tables de traductions et helpers
│   ├── web_interface.h       // ressources HTML/CSS/JS stockées en PROGMEM
│   ├── board_config.h        // mapping GPIO spécifique à la carte
│   ├── config.h              // drapeaux de configuration et constantes
│   ├── secrets-example.h     // modèle pour les identifiants Wi-Fi (copie vers secrets.h)
│   └── json_helpers.h        // utilitaires de formatage JSON
├── lib/                      // bibliothèques personnalisées (le cas échéant)
├── test/                     // fichiers de test
└── docs/                     // documentation (EN/FR)
```

## Cycle d'exécution
1. **Boot** – initialisation du port série, chargement des identifiants Wi-Fi, connexion via WiFiMulti.
2. **Services** – activation mDNS, démarrage du serveur HTTP et du service BLE lorsque la carte le permet.
3. **Diagnostics** – exposition d'actions REST déclenchant les tests (balayage GPIO, scan Wi-Fi, routines OLED, benchmarks, exports).
4. **Interface web** – les ressources statiques proviennent de `include/web_interface.h`; les données dynamiques sont injectées via des réponses JSON.
5. **Traductions** – `include/languages.h` stocke les chaînes FR et EN. Le client récupère `/api/get-translations` puis met à jour les textes à la volée.

## Modules clés
- **Pile Wi-Fi** – utilise `WiFiMulti` pour tester les réseaux configurés jusqu'à réussite.
- **Gestionnaire BLE** – détecte les capacités du chip, configure le service GATT et expose l'état côté interface.
- **Moteur de diagnostics** – exécute les tests matériels (GPIO, ADC, tactile, OLED, benchmarks) et formate les résultats pour le tableau de bord et les exports.
- **Sous-système d'export** – assemble les charges TXT/JSON/CSV et la vue imprimable.
- La version 3.9.0 introduit l'écran Wi-Fi de démarrage et des helpers de scan BLE mutualisés, maintenant le même ordonnanceur que la pile soit Bluedroid ou NimBLE.

## Structure front-end
- Les modèles HTML définissent chaque onglet avec des sections sémantiques.
- Un routeur léger gère la navigation via délégation d'événements.
- Le design responsive garantit des en-têtes monolignes et une navigation compacte sur mobiles.

## Flux de données
- Le client appelle `/api/get-translations` au chargement, puis consomme `/api/overview` (injecté dans le HTML) pour l'état initial.
- Les boutons de test invoquent leurs endpoints dédiés ; les réponses mettent l'UI à jour côté JavaScript.
- Les exports sont générés à la volée sur le microcontrôleur sans stockage externe.

## Principes d'extension
- Ajoutez un diagnostic en étendant l'ordonnanceur dans `src/main.cpp` et en exposant l'endpoint REST minimal.
- Conservez des clés de traduction cohérentes entre les entrées FR et EN dans `include/languages.h`.
- Évitez les appels bloquants dans la boucle principale pour préserver la réactivité web.
