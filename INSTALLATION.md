# 📦 GUIDE D'INSTALLATION - ESP32 Diagnostic v3.2.0

[🇬🇧 Read in English](INSTALLATION.en.md) | [📖 Documentation](README.md) | [📋 Changelog](CHANGELOG.md)

---

## 🎯 Vue d'ensemble

Ce guide vous accompagne pas à pas pour installer le système de diagnostic ESP32 avec interface web dynamique, multilingue et moderne.

**Nouveautés v3.2.0** :
- ✨ Support WiFi multi-SSID avec connexion automatique
- 🔧 Configuration des pins matérielles via fichiers séparés
- 🔒 Fichiers de configuration sécurisés (exclus de Git)

---

## 📋 Prérequis

### Matériel requis
- **ESP32** (ESP32-S2, ESP32-S3 ou ESP32-C3 recommandé)
- Câble USB pour la programmation
- Ordinateur avec Arduino IDE ou PlatformIO
- **Optionnel :** LED NeoPixel WS2812B pour l'indicateur visuel

### Logiciels nécessaires
- **Arduino IDE 2.x** ou **PlatformIO**
- **ESP32 Board Package** version 3.1.3 ou supérieure
- Navigateur web moderne (Chrome, Firefox, Edge, Safari)

### Bibliothèques Arduino requises
```
WiFi.h                    (incluse avec ESP32 core)
WebServer.h               (incluse avec ESP32 core)
SPIFFS.h                  (incluse avec ESP32 core)
Wire.h                    (incluse avec ESP32 core)
Adafruit_NeoPixel.h       (à installer)
ArduinoJson.h v6.x        (à installer - PAS la v7)
```

---

## 📁 Structure des fichiers

Votre projet doit contenir **10 fichiers** :

**Fichiers Code Source :**
```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino          # Fichier principal
├── api_handlers.h                # Handlers API REST (18 endpoints)
├── web_interface.h               # Interface HTML/CSS/JS (8 pages)
├── translations.h                # Traductions FR/EN
├── exemple-wifi-config.h         # ⭐ Template configuration WiFi
├── exemple-config.h              # ⭐ Template configuration matérielle
└── .gitignore                    # ⭐ Protection fichiers sensibles
```

**Fichiers Configuration (à créer par vous) :**
```
├── wifi-config.h                 # 🔒 VOS identifiants WiFi (NON commité)
└── config.h                      # 🔒 VOS pins matérielles (NON commité)
```

⚠️ **Important** : Les fichiers `wifi-config.h` et `config.h` contiennent des informations sensibles et ne doivent JAMAIS être partagés ou commitées sur Git.

---

## 🚀 Installation étape par étape

### Étape 1 : Préparer l'environnement Arduino IDE

#### 1.1 Installer ESP32 Board Manager

1. Ouvrir Arduino IDE
2. Aller dans **Fichier > Préférences**
3. Ajouter dans "URLs de gestionnaire de cartes supplémentaires" :
   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
4. Aller dans **Outils > Type de carte > Gestionnaire de cartes**
5. Rechercher "ESP32" et installer la version **3.1.3 ou supérieure**

#### 1.2 Installer les bibliothèques requises

1. Aller dans **Croquis > Inclure une bibliothèque > Gérer les bibliothèques**
2. Installer :
   - **Adafruit NeoPixel** (dernière version)
   - **ArduinoJson** version **6.x** (⚠️ PAS la version 7)

---

### Étape 2 : Créer le projet

1. **Créer un nouveau dossier** nommé `ESP32-Diagnostic`
2. **Télécharger les 6 fichiers** du projet
3. **Placer tous les fichiers** dans le dossier `ESP32-Diagnostic`
4. **Ouvrir** le fichier `ESP32-Diagnostic.ino` avec Arduino IDE

---

### Étape 3 : Configuration WiFi Multi-SSID ⭐ NOUVEAU

#### 3.1 Créer le fichier de configuration WiFi

1. **Copier le template** :
   ```bash
   cp exemple-wifi-config.h wifi-config.h
   ```
   
   Ou manuellement : dupliquer `exemple-wifi-config.h` et le renommer en `wifi-config.h`

2. **Éditer** `wifi-config.h` et configurer vos réseaux WiFi :

```cpp
const WiFiCredentials wifiNetworks[] = {
  // Réseau principal (maison)
  {"MonWiFi-Maison", "motdepasse123"},
  
  // Réseau secondaire (bureau)
  {"WiFi-Bureau", "autremdp456"},
  
  // Réseau mobile (secours)
  {"iPhone-Hotspot", "mdphotspot"},
};
```

3. **Configurer le point d'accès de secours** (optionnel) :

```cpp
#define AP_SSID "ESP32-Diagnostic-Setup"
#define AP_PASSWORD "diagnostic123"
```

💡 **Astuce** : Le système tentera de se connecter aux réseaux dans l'ordre défini. Placez votre réseau le plus utilisé en premier.

#### 3.2 Créer le fichier de configuration matérielle

1. **Copier le template** :
   ```bash
   cp exemple-config.h config.h
   ```

2. **Éditer** `config.h` et ajuster selon votre matériel :

```cpp
// LED interne
#define LED_BUILTIN_PIN 2  // Ajuster selon votre carte

// NeoPixel (optionnel - décommenter si utilisé)
// #define USE_NEOPIXEL
// #define NEOPIXEL_PIN 48
// #define NEOPIXEL_COUNT 1

// OLED (optionnel - décommenter si utilisé)
// #define USE_OLED
// #define OLED_SDA_PIN 21
// #define OLED_SCL_PIN 22

// Pins GPIO à tester
const int TEST_GPIO_PINS[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
```

#### 3.3 Vérifier la sécurité

✅ Vérifiez que le fichier `.gitignore` contient :
```
wifi-config.h
config.h
```

⚠️ **IMPORTANT** : Ne commitez JAMAIS `wifi-config.h` ou `config.h` sur GitHub ! Ils contiennent vos mots de passe WiFi.

---

### Étape 4 : Configuration alternative (méthode rapide)

Si vous ne souhaitez pas utiliser les fichiers de configuration séparés, le programme fonctionnera avec des paramètres par défaut et affichera des avertissements :

```
warning: wifi-config.h not found! Using default WiFi settings
warning: config.h not found! Using default settings
```

Dans ce cas, l'ESP32 démarrera en mode Point d'Accès :
- SSID : `ESP32-Diagnostic-Setup`
- Mot de passe : `diagnostic123`
- IP : `192.168.4.1`

---

### Étape 4 (ancien Étape 3) : Configuration des pins (optionnel)

Si vous utilisez des composants matériels, ajustez les pins selon votre câblage :

```cpp
// Configuration NeoPixel (ligne ~40)
#define NEOPIXEL_PIN 48        // Pin de données NeoPixel
#define NEOPIXEL_COUNT 1       // Nombre de LEDs

// Autres pins si nécessaire
#define BUTTON_PIN 9           // Pin bouton (si utilisé)
#define LED_PIN 2              // Pin LED interne
```

**Pins courantes selon les modèles ESP32 :**

| Modèle | Pin NeoPixel recommandée | PSRAM |
|--------|--------------------------|-------|
| ESP32-S3 | GPIO 48 | Oui (8MB) |
| ESP32-S2 | GPIO 18 | Non |
| ESP32-C3 | GPIO 8 | Non |
| ESP32 classique | GPIO 16 | Non |

---

### Étape 5 : Compilation et téléversement

#### 5.1 Sélectionner votre carte

1. Aller dans **Outils > Type de carte > ESP32 Arduino**
2. Choisir votre modèle :
   - `ESP32S3 Dev Module` pour ESP32-S3
   - `ESP32S2 Dev Module` pour ESP32-S2
   - `ESP32C3 Dev Module` pour ESP32-C3
   - `ESP32 Dev Module` pour ESP32 classique

#### 5.2 Configurer les paramètres

**Pour ESP32-S3 (recommandé) :**
```
USB CDC On Boot: Enabled
Flash Size: 8MB (ou selon votre carte)
Partition Scheme: Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)
PSRAM: OPI PSRAM (si disponible, sinon QSPI PSRAM)
Upload Speed: 921600
CPU Frequency: 240MHz
```

**Pour ESP32-S2 :**
```
Flash Size: 4MB
Partition Scheme: Default 4MB with spiffs
Upload Speed: 921600
CPU Frequency: 240MHz
```

**Pour ESP32-C3 :**
```
Flash Size: 4MB
Partition Scheme: Default 4MB with spiffs
Upload Speed: 921600
CPU Frequency: 160MHz
```

#### 5.3 Compiler et téléverser

1. **Compiler** : Cliquer sur ✓ (Vérifier)
2. **Connecter l'ESP32** via USB
3. **Sélectionner le port** : `Outils > Port > COMx` (Windows) ou `/dev/ttyUSBx` (Linux)
4. **Téléverser** : Cliquer sur → (Téléverser)

⚠️ **Note :** Sur certaines cartes, il faut maintenir le bouton **BOOT** pendant le téléversement.

---

### Étape 6 : Vérification du démarrage

#### 6.1 Ouvrir le moniteur série

1. Ouvrir le **Moniteur Série** : `Outils > Moniteur série`
2. Régler la vitesse à **115200 bauds**

#### 6.2 Messages attendus

Vous devriez voir :

```
====================================
ESP32 Diagnostic System v3.2.0
====================================

[INIT] Initializing system...
[WIFI] Configuring WiFi Multi-SSID...
  - Added network: MonWiFi-Maison
  - Added network: WiFi-Bureau
  - Added network: iPhone-Hotspot
[WIFI] Connecting to WiFi...
..........
[WIFI] WiFi connected successfully!
  - SSID: MonWiFi-Maison
  - IP Address: 192.168.1.xxx
  - Signal Strength: -45 dBm
  - MAC Address: XX:XX:XX:XX:XX:XX
[WEB] Web server started on port 80
[READY] System ready!
====================================
Access web interface at:
  - http://192.168.1.xxx
  - http://esp32-diag.local
====================================
```

✅ Si vous voyez ces messages, l'installation est **réussie** !

---

## 🌐 Accès à l'interface web

### Méthode 1 : Via adresse IP (fonctionne toujours)

1. Noter l'adresse IP affichée dans le moniteur série
2. Ouvrir un navigateur web
3. Accéder à : `http://192.168.1.xxx`

### Méthode 2 : Via mDNS (plus pratique)

- Accéder à : `http://esp32-diag.local`
- ⚠️ mDNS ne fonctionne pas sur tous les réseaux (notamment certains réseaux d'entreprise)

### Méthode 3 : Via Point d'Accès WiFi (mode secours)

Si la connexion WiFi échoue, l'ESP32 créera un point d'accès :
- SSID : `ESP32-Diagnostic-Setup`
- Mot de passe : `diagnostic123`
- Adresse : `http://192.168.4.1`

---

## 🔧 Configuration avancée

### Ajuster la mémoire SPIFFS

Si vous avez besoin de plus d'espace pour l'application, modifiez la partition scheme :

```
Outils > Partition Scheme > Huge APP (3MB No OTA/1MB SPIFFS)
```

### Activer les logs de debug

Ajoutez au début du fichier `.ino` :

```cpp
#define DEBUG_MODE 1

#if DEBUG_MODE
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif
```

### Désactiver le NeoPixel

Si vous n'utilisez pas de LED NeoPixel, commentez cette ligne :

```cpp
// #define USE_NEOPIXEL  // Désactivé
```

---

## ⚠️ Résolution des problèmes

### Problème : Erreur de compilation

**Symptôme :** Erreurs de compilation liées à ArduinoJson

**Solution :** Vérifier que vous utilisez ArduinoJson **version 6.x** et non la v7
```
Gérer les bibliothèques > ArduinoJson > Installer version 6.21.3
```

---

### Problème : ESP32 ne se connecte pas au WiFi

**Symptômes :**
```
[WIFI] Connecting to WiFi...
[WIFI] .........
[WIFI] Connection failed!
```

**Solutions :**
1. Vérifier le SSID et le mot de passe (sensible à la casse)
2. Vérifier que le WiFi est en 2.4GHz (l'ESP32 ne supporte pas le 5GHz)
3. Se rapprocher du point d'accès WiFi
4. Vérifier que le WiFi n'utilise pas un filtrage MAC

---

### Problème : L'interface web ne charge pas

**Symptôme :** Page blanche ou erreur 404

**Solutions :**
1. Vérifier que la Partition Scheme inclut SPIFFS
2. Recompiler et téléverser le code complet
3. Vider le cache du navigateur (Ctrl + F5)
4. Essayer un autre navigateur

---

### Problème : "Out of memory" ou redémarrages

**Symptôme :** L'ESP32 redémarre de manière aléatoire

**Solutions :**
1. Activer le PSRAM si disponible : `Outils > PSRAM > OPI PSRAM`
2. Réduire la taille du buffer dans `web_interface.h`
3. Choisir une Partition Scheme avec plus de mémoire APP

---

### Problème : Téléversement impossible

**Symptôme :** Erreur "Failed to connect to ESP32"

**Solutions :**
1. Maintenir le bouton **BOOT** pendant le téléversement
2. Changer le port USB
3. Réduire la vitesse de téléversement : `Outils > Upload Speed > 115200`
4. Installer les drivers USB-Serial :
   - Windows : [CP210x drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
   - macOS : Inclus dans le système
   - Linux : Ajouter l'utilisateur au groupe `dialout`

---

## 📊 Tests post-installation

### Test 1 : Accès à l'interface

✅ Ouvrir `http://ESP32-IP` dans le navigateur
✅ L'interface doit afficher 8 onglets
✅ Le sélecteur de langue FR/EN doit fonctionner

### Test 2 : Tests hardware

✅ Onglet "Tests" doit lister tous les tests disponibles
✅ Cliquer sur "Run Test" doit afficher les résultats en temps réel
✅ Le NeoPixel doit changer de couleur (si connecté)

### Test 3 : API REST

Tester avec curl ou Postman :
```bash
curl http://ESP32-IP/api/system/info
```

Réponse attendue :
```json
{
  "version": "3.2.0",
  "chipModel": "ESP32-S3",
  "cpuFreq": 240,
  "flashSize": 8388608,
  "freeHeap": 245760
}
```

---

## 🎓 Prochaines étapes

Maintenant que l'installation est terminée :

1. 📖 Consultez le [README.md](README.md) pour comprendre les fonctionnalités
2. 🧪 Explorez l'onglet "Tests" pour diagnostiquer votre ESP32
3. 🌐 Testez l'API REST pour l'intégration avec d'autres systèmes
4. 🎨 Personnalisez l'interface selon vos besoins
5. 📊 Consultez le [CHANGELOG.md](CHANGELOG.md) pour les nouveautés

---

## 📞 Support

- **Documentation :** [README.md](README.md)
- **Structure du projet :** [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md)
- **Changelog :** [CHANGELOG.md](CHANGELOG.md)

---

## 📄 Licence

MIT License - Voir fichier LICENSE pour plus de détails

---

**Développé avec ❤️ pour la communauté ESP32**
