# Politique de mapping des broches (GPIO) – ESP32 Diagnostic Suite

## Objectif
Garantir que l'intégralité du code et de la documentation du projet utilise exclusivement les macros définies dans `include/board_config.h` pour toute référence à une broche matérielle (GPIO).

## Règles obligatoires

- **Source unique de vérité** :
  - Le fichier `board_config.h` est la seule référence autorisée pour le mapping des broches (GPIO).
  - Aucun alias local, variable dynamique ou numéro de pin en dur n'est toléré dans le code ou la documentation.

- **Utilisation dans le code** :
  - Toute référence à une broche doit utiliser la macro correspondante de `board_config.h` (ex : `NEOPIXEL`, `LED_BUILTIN`, `DHT`, `BUZZER`, `TFT_MOSI`, etc.).
  - Les fichiers `config.h` et `config-example.h` ne doivent contenir aucune définition de pin, seulement des constantes de configuration non liées à un GPIO (ex : nombre de LEDs, luminosité par défaut, etc.).
  - Les fichiers source (`.cpp`) et d'en-tête (`.h`) doivent inclure `board_config.h` et utiliser directement les macros définies.

- **Documentation** :
  - Toute documentation utilisateur ou technique doit référencer explicitement `board_config.h` comme source unique du mapping des broches.
  - Les exemples de code et tableaux de correspondance doivent utiliser les noms de macros, jamais de numéros de GPIO en dur.

## Exemples corrects

```cpp
#include "board_config.h"
...
digitalWrite(NEOPIXEL, HIGH);
int led = LED_BUILTIN;
```

## Exemples incorrects (interdits)

```cpp
#define LED_PIN 2 // Interdit
int myLed = 13;   // Interdit
extern int RGB_LED_PIN_R; // Interdit
```

## Historique des corrections (décembre 2025)
- Suppression de toutes les définitions de pins dans `config.h` et `config-example.h`.
- Suppression de toutes les variables dynamiques de pins dans `web_interface.h`.
- Remplacement de tous les alias locaux et #define de pins dans `main.cpp` par l'utilisation directe des macros de `board_config.h`.
- Mise à jour de la documentation et des notes de version pour supprimer toute référence à des pins configurables ailleurs que dans `board_config.h`.

## Pour toute modification future
- Ajouter toute nouvelle broche ou changement de mapping exclusivement dans `board_config.h`.
- Mettre à jour la documentation pour refléter les nouveaux noms de macros si nécessaire.

---

**Ce document est à joindre à chaque livraison et à toute documentation utilisateur.**
