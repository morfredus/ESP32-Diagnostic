# Politique de Mapping des GPIO

**Suite de Diagnostic ESP32 - Version 3.24.0**

---

## 🎯 Objectif de ce Document

Ce document explique la **politique de mapping des GPIO** pour le projet Suite de Diagnostic ESP32. Si vous êtes débutant ou contributeur à ce projet, il est **crucial** de comprendre comment fonctionnent les assignations de broches pour éviter de casser la configuration matérielle.

---

## 📍 La Règle d'Or : `board_config.h` est l'Unique Source de Vérité

### Que Signifie Cela ?

Dans ce projet, **toutes les assignations de broches GPIO** sont définies dans **un seul et unique fichier** :

```
include/board_config.h
```

Ce fichier contient le **mapping de référence** pour toutes les broches GPIO utilisées par :
- Les indicateurs LED (RGB, NeoPixel)
- Les capteurs (DHT, mouvement, lumière, distance)
- Les interfaces de communication (I2C, SPI, UART/GPS)
- L'écran TFT
- Les boutons
- Le buzzer et les sorties PWM

### Pourquoi Est-ce Important ?

**Problème :** Dans de nombreux projets, les broches GPIO sont définies à plusieurs endroits (code principal, fichiers de config, en-têtes, etc.). Cela crée :
- **De la confusion** : Quelle définition est correcte ?
- **Des bugs** : Modifier une broche à un endroit mais oublier les autres
- **Des conflits matériels** : Deux périphériques essayant d'utiliser la même GPIO

**Solution :** En ayant **UNE source autoritaire** (`board_config.h`), nous éliminons complètement ces problèmes.

---

## 🔧 Comment Ça Fonctionne

### 1. Définitions des Broches dans `board_config.h`

Le fichier définit les broches différemment pour chaque variante d'ESP32 :

```cpp
#if defined(TARGET_ESP32_S3)
  // Broches ESP32-S3 DevKitC-1
  #define PIN_GPS_RXD        18
  #define PIN_GPS_TXD        17
  #define RGB_LED_PIN_R      21
  #define RGB_LED_PIN_G      41
  #define RGB_LED_PIN_B      42
  #define PWM_PIN            20
  #define BUZZER_PIN         6
  // ... plus de broches ...

#elif defined(TARGET_ESP32_CLASSIC)
  // Broches ESP32 Classic DevKit
  #define PIN_GPS_RXD        16
  #define PIN_GPS_TXD        17
  #define RGB_LED_PIN_R      13
  #define RGB_LED_PIN_G      26
  #define RGB_LED_PIN_B      33
  #define PWM_PIN            4
  #define BUZZER_PIN         19
  // ... plus de broches ...
#endif
```

### 2. Constantes de Compilation (Depuis v3.24.0)

**Changement Important :** Les broches GPIO sont maintenant des **constantes de compilation**. Cela signifie :

- **Pas de variables runtime** - les broches sont accessibles directement via des macros `#define`
- **Les changements de broches nécessitent une recompilation** - l'interface Web ne supporte plus le remapping dynamique
- **Architecture simplifiée** - élimine le système à deux couches (plus de préfixe `DEFAULT_`)
- **Meilleures performances** - le compilateur peut optimiser l'accès aux broches plus efficacement

**Architecture Précédente (v3.23.x et antérieures) :**
```cpp
#define DEFAULT_RGB_LED_PIN_R 21  // Valeur par défaut compile-time
int RGB_LED_PIN_R = DEFAULT_RGB_LED_PIN_R;  // Variable runtime
```

**Architecture Actuelle (v3.24.0+) :**
```cpp
#define RGB_LED_PIN_R 21  // Constante de compilation (utilisée directement)
```

### 3. Comportement de l'Interface Web

L'interface Web affiche toujours les assignations de broches actuelles pour référence, mais :
- **La configuration des broches est en lecture seule** - les changements ne sont pas sauvegardés ni appliqués
- **Pour changer les broches**, vous devez :
  1. Éditer `include/board_config.h`
  2. Recompiler le projet
  3. Uploader le nouveau firmware

```cpp
// Dans web_interface.h (reste inchangé)
int RGB_LED_PIN_R = DEFAULT_RGB_LED_PIN_R;
int RGB_LED_PIN_G = DEFAULT_RGB_LED_PIN_G;
int RGB_LED_PIN_B = DEFAULT_RGB_LED_PIN_B;
int PWM_PIN = DEFAULT_PWM_PIN;
int BUZZER_PIN = DEFAULT_BUZZER_PIN;
int DHT_PIN = DEFAULT_DHT_PIN;
// ... plus de variables runtime ...
```

**Pourquoi des variables runtime ?** Elles permettent le **remapping dynamique** via l'interface Web sans recompiler !

### 3. Accès via l'Interface Web

L'interface Web peut :
- **Afficher** les assignations de broches actuelles
- **Modifier** les assignations de broches à l'exécution pour les tests
- **Persister** les changements entre les redémarrages (fonctionnalité future)

Les broches sont injectées dans le JavaScript au chargement de la page :

```cpp
// Dans web_interface.h
js += F("const RGB_LED_PIN_R=");
js += String(RGB_LED_PIN_R);  // Injecte la valeur actuelle
```

---

## ⚠️ Règles Critiques pour les Développeurs

### ✅ À FAIRE :

1. **Toujours référencer les broches depuis `board_config.h`**
   ```cpp
   // BON
   pinMode(DEFAULT_RGB_LED_PIN_R, OUTPUT);
   ```

2. **Utiliser les variables runtime pour les broches dynamiques**
   ```cpp
   // BON
   analogWrite(PWM_PIN, 128);  // Utilise la variable runtime
   ```

3. **Vérifier la disponibilité de la broche avant utilisation**
   ```cpp
   // BON
   if (DHT_PIN >= 0) {
     dht.begin(DHT_PIN);
   }
   ```

### ❌ À NE PAS FAIRE :

1. **Jamais coder en dur les numéros de GPIO**
   ```cpp
   // MAUVAIS - casse le support multi-cartes
   pinMode(21, OUTPUT);  // Et si on est sur ESP32 Classic ?
   ```

2. **Jamais redéfinir les broches dans d'autres fichiers**
   ```cpp
   // MAUVAIS - crée un conflit avec board_config.h
   #define RGB_LED_PIN_R 13  // Déjà défini ailleurs !
   ```

3. **Jamais utiliser des noms ambigus**
   ```cpp
   // MAUVAIS - vient-il de board_config.h ou d'ailleurs ?
   #define LED_PIN 48  // Utiliser NEOPIXEL_PIN à la place
   ```

---

## 🔍 Conventions de Nommage des Broches

### Signification des Préfixes :

- **`PIN_`** : Broches matérielles fixes (GPS, boutons)
  - Exemple : `PIN_GPS_RXD`, `PIN_BUTTON_1`
  - **Ne peuvent pas être modifiées** à l'exécution (contrainte matérielle)

- **`DEFAULT_`** : Valeurs par défaut pour les broches configurables à l'exécution
  - Exemple : `DEFAULT_RGB_LED_PIN_R`, `DEFAULT_DHT_PIN`
  - **Peuvent être modifiées** à l'exécution via l'interface Web

### Cas Spéciaux :

- **`NEOPIXEL_PIN`** : NeoPixel intégré sur ESP32-S3 (GPIO 48)
- **`TFT_*`** : Broches SPI de l'écran TFT
- **`DEFAULT_I2C_SDA/SCL`** : Broches du bus I2C

---

## 🧪 Exemple : Ajouter un Nouveau Capteur

Supposons que vous vouliez ajouter un **nouveau capteur ultrasonique** sur une broche différente.

### Étape 1 : Définir dans `board_config.h`

```cpp
#if defined(TARGET_ESP32_S3)
  #define DEFAULT_ULTRASONIC_TRIG_PIN 14
  #define DEFAULT_ULTRASONIC_ECHO_PIN 21
#elif defined(TARGET_ESP32_CLASSIC)
  #define DEFAULT_ULTRASONIC_TRIG_PIN 23
  #define DEFAULT_ULTRASONIC_ECHO_PIN 34
#endif
```

### Étape 2 : Créer les variables runtime dans `main.cpp`

```cpp
int ULTRASONIC_TRIG_PIN = DEFAULT_ULTRASONIC_TRIG_PIN;
int ULTRASONIC_ECHO_PIN = DEFAULT_ULTRASONIC_ECHO_PIN;
```

### Étape 3 : Utiliser dans votre code

```cpp
void setupUltrasonic() {
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
}

long measureDistance() {
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  return duration * 0.034 / 2;  // Convertir en cm
}
```

### Étape 4 : Exposer dans l'Interface Web (optionnel)

Ajouter à l'injection JavaScript dans `web_interface.h` :

```cpp
js += F(";const ULTRASONIC_TRIG_PIN=");
js += String(ULTRASONIC_TRIG_PIN);
js += F(";const ULTRASONIC_ECHO_PIN=");
js += String(ULTRASONIC_ECHO_PIN);
```

---

## 🛡️ Considérations de Sécurité

### Protection Matérielle

`board_config.h` inclut des **commentaires de sécurité détaillés** pour chaque broche :

```cpp
#define DEFAULT_DHT_PIN 5  // DHT. **Pull-Up 10kΩ vers 3.3V**.
#define PIN_GPS_RXD     18 // GPS TX. **Si 5V, Diviseur de tension OBLIGATOIRE.**
```

**Lisez toujours ces commentaires** avant de connecter du matériel !

### Erreurs Courantes à Éviter :

1. **Signaux 5V sur broches 3.3V** → Utiliser un diviseur de tension
2. **Résistances pull-up manquantes** → L'I2C ne fonctionnera pas
3. **Broches de strapping** (GPIO 0, 46) → Échecs de démarrage
4. **Broches en entrée seule** (34-39 sur Classic) → Ne peuvent pas être utilisées en sortie

---

## 📚 Documentation Associée

- **[PIN_MAPPING_FR.md](PIN_MAPPING_FR.md)** - Tables complètes des broches pour chaque carte
- **[CONFIG_FR.md](CONFIG_FR.md)** - Vue d'ensemble des fichiers de configuration
- **[ARCHITECTURE_FR.md](ARCHITECTURE_FR.md)** - Détails de l'architecture système

---

## ✨ Résumé

1. **`board_config.h` est le SEUL endroit pour définir les broches GPIO**
2. Ne jamais coder en dur les numéros de GPIO dans votre code
3. Utiliser les variables runtime pour le remapping dynamique des broches
4. Suivre les conventions de nommage (`PIN_*` vs `DEFAULT_*`)
5. Toujours lire les commentaires de sécurité avant de câbler le matériel

En suivant cette politique, vous assurez :
- ✅ Compatibilité multi-cartes (ESP32-S3 / Classic)
- ✅ Aucun conflit ou doublon de broches
- ✅ Maintenance et débogage faciles
- ✅ Fonctionnement sûr du matériel

---

**Des questions ?** Consultez [FAQ_FR.md](FAQ_FR.md) ou ouvrez un ticket sur GitHub.
