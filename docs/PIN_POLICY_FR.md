# Politique de Mapping des GPIO

**Suite de Diagnostic ESP32 - Version 3.25.0**

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

### 2. Architecture à Deux Couches (v3.25.0)

**Conception Actuelle :** Les broches GPIO utilisent une **architecture à deux couches** avec des conventions de nommage distinctes :

**Couche 1 : Valeurs par Défaut Compile-Time (MAJUSCULES dans `board_config.h`)**
```cpp
#define I2C_SDA       15  // Broche SDA par défaut
#define I2C_SCL       16  // Broche SCL par défaut
#define RGB_LED_PIN_R 21  // Canal rouge RGB par défaut
```

**Couche 2 : Variables Runtime (minuscules dans `main.cpp`)**
```cpp
int i2c_sda = I2C_SDA;          // Modifiable à l'exécution via interface Web
int i2c_scl = I2C_SCL;          // Modifiable à l'exécution via interface Web
int rgb_led_pin_r = RGB_LED_PIN_R;  // Modifiable à l'exécution via interface Web
```

**Point Clé :** Utiliser des conventions de nommage différentes (MAJUSCULES vs minuscules) empêche le préprocesseur d'expander les noms de variables en minuscules, éliminant les conflits tout en maintenant les deux couches.

**Avantages :**
- ✅ **Le remapping dynamique fonctionne** - Les utilisateurs peuvent changer les broches via l'interface Web sans recompilation
- ✅ **Aucun conflit de préprocesseur** - Des noms différents préviennent les problèmes d'expansion de macros
- ✅ **Distinction claire** - MAJUSCULES = valeurs par défaut compile-time, minuscules = broches actives runtime
- ✅ **Performance** - Le compilateur optimise l'initialisation constante
- ✅ **Flexibilité matérielle** - Testez différentes configurations de broches facilement

### 3. Comportement de l'Interface Web (v3.25.0)

L'interface Web supporte pleinement le **remapping dynamique des broches** :
- **Les changements de broches runtime fonctionnent** - Modifiez les broches GPIO via l'interface Web sans recompilation
- **Handlers de configuration actifs** - Tous les endpoints de configuration de broches sont fonctionnels :
  - Broches I2C (OLED et capteurs environnementaux)
  - Broches LED RGB (canaux R, V, B)
  - Buzzer, capteur DHT, capteur de lumière
  - Capteur de distance (Trigger et Echo)
  - Capteur de mouvement
- **Les changements prennent effet immédiatement** - Aucun upload de firmware requis

**Pour changer les broches par défaut de façon permanente**, vous pouvez :
  1. Éditer `include/board_config.h` (defines MAJUSCULES)
  2. Recompiler et uploader

**Pour changer les broches temporairement pour des tests** :
  1. Utiliser la configuration de broches de l'interface Web
  2. Les changements persistent jusqu'au redémarrage de l'appareil

```cpp
// Dans web_interface.h - Injection JavaScript
js += F("const i2c_sda=");
js += String(i2c_sda);  // Injecte la valeur runtime actuelle (minuscules)
```

---

## ⚠️ Règles Critiques pour les Développeurs

### ✅ À FAIRE :

1. **Utiliser les variables runtime en minuscules dans le code**
   ```cpp
   // BON - utilise la variable runtime en minuscules (peut être changée via interface Web)
   pinMode(rgb_led_pin_r, OUTPUT);
   digitalWrite(buzzer_pin, HIGH);
   ```

2. **Inclure board_config.h et déclarer extern pour les variables runtime**
   ```cpp
   // BON
   #include "board_config.h"
   extern int pwm_pin;  // Variable runtime depuis main.cpp
   analogWrite(pwm_pin, 128);
   ```

3. **Initialiser les variables runtime depuis les defines MAJUSCULES**
   ```cpp
   // BON - dans main.cpp
   int i2c_sda = I2C_SDA;  // Initialiser depuis la valeur par défaut de board_config.h
   ```

4. **Vérifier la disponibilité de la broche avant utilisation**
   ```cpp
   // BON
   #ifdef DHT_PIN
     extern int dht_pin;
     dht.begin(dht_pin);
   #endif
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

- **`PIN_`** : Broches matérielles fixes (boutons, GPS)
  - Exemple : `PIN_GPS_RXD`, `PIN_BUTTON_1`, `PIN_BUTTON_BOOT`
  - Utilisées pour les connexions matérielles spécifiques

- **Pas de préfixe** : Broches de capteurs et périphériques
  - Exemple : `RGB_LED_PIN_R`, `DHT_PIN`, `BUZZER_PIN`
  - Définies par variante de carte dans `board_config.h`

### Cas Spéciaux :

- **`NEOPIXEL_PIN`** : NeoPixel intégré sur ESP32-S3 (GPIO 48)
- **`TFT_*`** : Broches SPI de l'écran TFT
- **`I2C_SDA/SCL`** : Broches du bus I2C

---

## 🧪 Exemple : Ajouter un Nouveau Capteur

Supposons que vous vouliez ajouter un **nouveau capteur ultrasonique** sur une broche différente.

### Étape 1 : Définir dans `board_config.h` (MAJUSCULES)

```cpp
#if defined(TARGET_ESP32_S3)
  #define ULTRASONIC_TRIG_PIN 14
  #define ULTRASONIC_ECHO_PIN 21
#elif defined(TARGET_ESP32_CLASSIC)
  #define ULTRASONIC_TRIG_PIN 23
  #define ULTRASONIC_ECHO_PIN 34
#endif
```

### Étape 2 : Créer les variables runtime dans `main.cpp` (minuscules)

```cpp
int ultrasonic_trig_pin = ULTRASONIC_TRIG_PIN;  // Runtime, modifiable via Web UI
int ultrasonic_echo_pin = ULTRASONIC_ECHO_PIN;  // Runtime, modifiable via Web UI
```

### Étape 3 : Utiliser directement dans votre code

```cpp
void setupUltrasonic() {
  pinMode(ultrasonic_trig_pin, OUTPUT);
  pinMode(ultrasonic_echo_pin, INPUT);
}

long measureDistance() {
  digitalWrite(ultrasonic_trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonic_trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonic_trig_pin, LOW);

  long duration = pulseIn(ultrasonic_echo_pin, HIGH);
  return duration * 0.034 / 2;  // Convertir en cm
}
```

### Étape 4 : Exposer dans l'Interface Web (optionnel)

Ajouter à l'injection JavaScript dans `web_interface.h` :

```cpp
js += F(";const ultrasonic_trig_pin=");
js += String(ultrasonic_trig_pin);  // Variable runtime en minuscules
js += F(";const ultrasonic_echo_pin=");
js += String(ultrasonic_echo_pin);
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

1. **`board_config.h` est le SEUL endroit pour définir les valeurs par défaut des broches GPIO (MAJUSCULES)**
2. **Utiliser les variables runtime en minuscules** dans le code pour le remapping dynamique
3. Ne jamais coder en dur les numéros de GPIO dans votre code
4. **Architecture à deux couches** : defines MAJUSCULES (compile-time) → variables minuscules (runtime)
5. Suivre les conventions de nommage (MAJUSCULES pour les valeurs par défaut, minuscules pour runtime)
6. Toujours lire les commentaires de sécurité avant de câbler le matériel

En suivant cette politique, vous assurez :
- ✅ Compatibilité multi-cartes (ESP32-S3 / Classic)
- ✅ Remapping dynamique des broches via interface Web
- ✅ Aucun conflit de préprocesseur
- ✅ Aucun conflit ou doublon de broches
- ✅ Maintenance et débogage faciles
- ✅ Fonctionnement sûr du matériel

---

**Des questions ?** Consultez [FAQ_FR.md](FAQ_FR.md) ou ouvrez un ticket sur GitHub.
