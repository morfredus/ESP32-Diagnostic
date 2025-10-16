# 📦 GUIDE D'INSTALLATION COMPLET - ESP32 Diagnostic v3.0.1

## 🎯 Vue d'ensemble

Ce guide vous accompagne pas à pas pour installer la version 3.0.1 du système de diagnostic ESP32 avec interface web dynamique, multilingue et moderne.

---

## 📋 Prérequis

### Matériel requis
- **ESP32** (ESP32-S2, ESP32-S3 ou ESP32-C3 recommandé)
- Câble USB pour la programmation
- Ordinateur avec Arduino IDE ou PlatformIO

### Logiciels nécessaires
- **Arduino IDE 2.x** ou **PlatformIO**
- **ESP32 Board Package** version **3.3.2 ou supérieure** ⚡ NOUVEAU
- Navigateur web moderne (Chrome, Firefox, Edge)

### Bibliothèques Arduino requises
```
- WiFi.h (incluse avec ESP32 core)
- WebServer.h (incluse avec ESP32 core)
- SPIFFS.h (incluse avec ESP32 core)
- Wire.h (incluse avec ESP32 core)
- Adafruit_NeoPixel.h (à installer depuis le gestionnaire)
- ArduinoJson.h version 6.x (à installer depuis le gestionnaire)
```

---

## 📁 Structure des fichiers

Votre projet doit contenir **6 fichiers de code** :

```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino          # Fichier principal
├── exemple-config.h              # Template configuration ⚡ À COPIER
├── config.h                      # Votre configuration (à créer) ⚡
├── api_handlers.h                # Handlers API REST
├── web_interface.h               # Interface HTML/CSS/JS
├── test_functions.h              # Fonctions de tests hardware
├── translations.h                # Traductions FR/EN
└── .gitignore                    # Exclusions Git
```

**⚠️ IMPORTANT** :
- `exemple-config.h` = Template fourni (safe pour Git)
- `config.h` = Votre fichier avec VOS identifiants (ignoré par Git)
- Ne JAMAIS commiter `config.h` avec vos vrais identifiants

---

## 🚀 Installation étape par étape

### Étape 1 : Préparer l'environnement Arduino IDE

1. **Installer ESP32 Board Manager** :
   - Ouvrir Arduino IDE
   - Aller dans `Fichier > Préférences`
   - Ajouter dans "URLs de gestionnaire de cartes supplémentaires" :
     ```
     https://espressif.github.io/arduino-esp32/package_esp32_index.json
     ```
   - Aller dans `Outils > Type de carte > Gestionnaire de cartes`
   - Rechercher "ESP32" et installer la version **3.3.2 ou supérieure** ⚡

2. **Installer les bibliothèques requises** :
   - `Croquis > Inclure une bibliothèque > Gérer les bibliothèques`
   - Installer :
     - **Adafruit NeoPixel** (dernière version)
     - **ArduinoJson** (version 6.x, **PAS la v7**)

### Étape 2 : Créer le projet

1. **Créer un nouveau dossier** nommé `ESP32-Diagnostic`
2. **Télécharger les fichiers** fournis dans les artifacts
3. **Placer tous les fichiers** dans le dossier `ESP32-Diagnostic`
4. **Créer votre fichier de configuration** :
   ```
   - Copier "exemple-config.h"
   - Renommer la copie en "config.h"
   - Garder "exemple-config.h" comme template
   ```

### Étape 3 : Configuration WiFi ⚠️ IMPORTANT

1. **Ouvrir le fichier `config.h`** (que vous venez de créer)

2. **Choisir votre mode WiFi** :

#### Option A : Multi-WiFi (RECOMMANDÉ) ✨

L'ESP32 essaiera plusieurs réseaux automatiquement :

```cpp
// Déjà activé par défaut
#define MULTI_WIFI_ENABLED
#define NUM_SSIDS 2

const char* ssid_list[] = {
  "MonWiFiMaison",      // Premier réseau
  "MonWiFiBureau"       // Second réseau (si premier échoue)
};

const char* password_list[] = {
  "PasswordMaison",
  "PasswordBureau"
};
```

**Avantages** :
- ✅ Fonctionne dans plusieurs lieux
- ✅ Bascule automatique entre réseaux
- ✅ Plus flexible

#### Option B : WiFi Simple

Un seul réseau :

```cpp
// Commenter Multi-WiFi et activer Single
// #define MULTI_WIFI_ENABLED  // ← Commenter
#define SINGLE_WIFI_ENABLED     // ← Activer

const char* ssid = "MonSSID";
const char* password = "MonPassword";
```

3. **Sauvegarder `config.h`**

**🔒 Sécurité :**
- ✅ `config.h` est dans `.gitignore` (ne sera pas commité)
- ✅ `exemple-config.h` peut être commité (pas de credentials)
- ❌ Ne JAMAIS partager votre `config.h` configuré

**Option avancée** : Vous pouvez ajouter 3, 4 réseaux ou plus en augmentant `NUM_SSIDS`

### Étape 4 : Configuration des pins (optionnel)

Si vous utilisez des composants matériels, ajustez les pins selon votre câblage :

```cpp
// Configuration NeoPixel (ligne ~40)
#define NEOPIXEL_PIN 48        // Pin de données NeoPixel
#define NEOPIXEL_COUNT 1       // Nombre de LEDs

// Autres pins si nécessaire
// #define BUTTON_PIN 9
// #define LED_PIN 2
```

### Étape 5 : Compilation et téléversement

1. **Sélectionner votre carte** :
   - `Outils > Type de carte > ESP32 Arduino`
   - Choisir votre modèle (ex: ESP32S3 Dev Module)

2. **Configurer les paramètres** (pour ESP32-S3) :
   ```
   USB CDC On Boot: Enabled
   Flash Size: 8MB
   Partition Scheme: Default 4MB with spiffs
   PSRAM: QSPI PSRAM (OPI PSRAM si disponible avec Core 3.3.2)
   Upload Speed: 921600
   ```

3. **Compiler** : Cliquer sur ✓ (Vérifier)
4. **Téléverser** : Cliquer sur → (Téléverser)

### Étape 6 : Vérification du démarrage

1. Ouvrir le **Moniteur Série** (`Outils > Moniteur série`)
2. Régler la vitesse à **115200 bauds**
3. Vous devriez voir :
   ```
   ====================================
   ESP32 Diagnostic System v3.0.1
   ====================================

   [INIT] Initializing system...
   [WIFI] Connecting to WiFi...
   [WIFI] Connected! IP: 192.168.1.xxx
   [MDNS] Started at: http://esp32-diag.local
   [WEB] Server started
   [READY] System ready!
   ```

---

## 🌐 Accès à l'interface web

### Méthode 1 : mDNS (recommandé)
Ouvrir dans votre navigateur :
```
http://esp32-diagnostic.local
```

### Méthode 2 : Adresse IP
Si mDNS ne fonctionne pas, utiliser l'adresse IP affichée dans le moniteur série :
```
http://192.168.1.xxx
```

---

## ✨ Fonctionnalités de l'interface v3.0.1

### 🎨 Design moderne
- **Glassmorphism** avec effets de transparence et flou
- **Dégradés animés** violet/bleu
- **Animations fluides** sur tous les éléments
- **Interface responsive** adaptée mobile/tablette/desktop

### 🗂️ Navigation par onglets
- **📊 Vue générale** : Informations système
- **🧪 Tests** : Tous les tests hardware
- **💾 Exports** : Export JSON et CSV

### 🔄 Mise à jour temps réel
- Actualisation automatique toutes les **5 secondes**
- Indicateur visuel pulsant
- Barre de progression du prochain refresh

### 🌍 Multilingue
- **🇫🇷 Français** (par défaut)
- **🇬🇧 Anglais**
- Changement instantané sans rechargement

### 📊 Tableaux de tests
- **GPIO** : Test de tous les pins
- **I2C** : Scan des périphériques I2C
- **SPI** : Détection des bus SPI
- **Memory** : Analyse détaillée de la mémoire
- **WiFi** : Statut et qualité du signal
- **System** : Informations CPU et uptime

---

## 🧪 Tester l'installation

### Test 1 : Page d'accueil
✅ L'interface doit charger avec design moderne
✅ Les animations doivent être fluides
✅ Le changement FR/EN doit fonctionner

### Test 2 : Onglet Tests
✅ Cliquer sur "Tests" doit afficher tous les tableaux
✅ Les données doivent se mettre à jour automatiquement

### Test 3 : Export JSON
✅ Cliquer sur "Export JSON" doit télécharger un fichier
✅ Le fichier doit contenir toutes les données système

### Test 4 : NeoPixel (si connecté)
✅ Les patterns doivent s'afficher correctement
✅ Changement de couleur doit être instantané

---

## 🔧 Dépannage

### Problème : ESP32 ne se connecte pas au WiFi

**Solution 1** : Vérifier les identifiants dans `config.h`
```cpp
const char* ssid = "VotreSSID";      // Vérifier majuscules/minuscules
const char* password = "MotDePasse";  // Vérifier caractères spéciaux
```

**Solution 2** : Forcer le WiFi 2.4GHz (ESP32 ne supporte pas le 5GHz)

**Solution 3** : Augmenter le timeout
```cpp
WiFi.begin(ssid, password);
int timeout = 30; // Augmenter à 30 secondes
while (WiFi.status() != WL_CONNECTED && timeout-- > 0) {
  delay(1000);
}
```

### Problème : "Sketch too big" lors de la compilation

**Solution** : Changer le schéma de partition
- `Outils > Partition Scheme > Huge APP (3MB No OTA/1MB SPIFFS)`

### Problème : mDNS ne fonctionne pas

**Cause** : Certains routeurs bloquent mDNS
**Solution** : Utiliser l'adresse IP directe affichée dans le moniteur série

### Problème : Interface s'affiche mal

**Solution** : Vider le cache du navigateur
- Chrome : `Ctrl+Shift+R` (ou `Cmd+Shift+R` sur Mac)
- Firefox : `Ctrl+F5`

### Problème : NeoPixel ne fonctionne pas

**Vérifications** :
1. Pin de données correcte (définie ligne ~40)
2. Alimentation suffisante (NeoPixel consomme beaucoup)
3. Niveau logique compatible (certains NeoPixel nécessitent 5V)

---

## 📊 Export des données

### Format JSON
Contient toutes les données système dans un format structuré :
```json
{
  "system": {
    "chip": "ESP32-S3",
    "cores": 2,
    "frequency": 240
  },
  "memory": {
    "heap_free": 245312,
    "heap_size": 327680
  },
  ...
}
```

### Format CSV
Tableau compatible Excel avec toutes les métriques

---

## 🎯 Prochaines étapes

Une fois l'installation terminée, vous pouvez :
1. **Personnaliser les couleurs** dans `web_interface.h`
2. **Ajouter vos propres tests** dans `test_functions.h`
3. **Configurer des alertes** sur les seuils mémoire
4. **Connecter des capteurs** et les intégrer dans l'interface

---

## 📞 Support

- **Documentation** : Lisez ce guide en entier
- **Moniteur série** : Activez le debug pour voir les logs
- **Tests progressifs** : Testez chaque fonctionnalité individuellement

---

## 📝 Changelog

### v3.0.1 (Octobre 2025) - Upgrade Core 3.3.2 ⚡
- **Upgrade** : Arduino Core ESP32 3.1.3 → 3.3.2
- **Optimisé** : Gestion mémoire améliorée avec nouvelles API
- **Optimisé** : Support OPI PSRAM pour ESP32-S3
- **Optimisé** : Performances WiFi améliorées
- **Corrigé** : Compatibilité avec nouvelles APIs Core 3.3.2

### v3.0.0 (Octobre 2025)
- Interface web dynamique avec mise à jour temps réel
- Design glassmorphism moderne
- Navigation par onglets
- Système de traduction FR/EN intégré
- Export JSON et CSV
- Animations et transitions fluides

---

## ✅ Checklist finale

Avant de considérer l'installation terminée, vérifiez :

- [ ] Arduino Core ESP32 version 3.3.2+ installé
- [ ] Tous les fichiers (6 code + 2 config) sont dans le bon dossier
- [ ] `exemple-config.h` copié et renommé en `config.h` ⚡
- [ ] Identifiants WiFi configurés dans `config.h`
- [ ] `config.h` ne sera PAS commité (vérifier .gitignore)
- [ ] Compilation sans erreur
- [ ] Téléversement réussi
- [ ] Messages de démarrage corrects dans le moniteur série
- [ ] Interface web accessible via http://esp32-diagnostic.local
- [ ] Design moderne s'affiche correctement
- [ ] Changement de langue fonctionne
- [ ] Mise à jour automatique active
- [ ] Export JSON télécharge un fichier valide
- [ ] Export CSV fonctionne
- [ ] (Optionnel) NeoPixel répond aux commandes

---

## 🎉 Félicitations !

Votre système de diagnostic ESP32 v3.0.1 est maintenant opérationnel !

**Accès** : http://esp32-diagnostic.local
**Version** : 3.0.1
**Core ESP32** : 3.3.2+
**Langues** : 🇫🇷 Français / 🇬🇧 English

---

*Guide créé pour ESP32 Diagnostic System v3.0.1*
*Dernière mise à jour : Octobre 2025*
*Arduino Core ESP32 : 3.3.2+*
