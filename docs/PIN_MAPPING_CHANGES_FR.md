# Modifications du Pin Mapping ESP32 Classic - Résolution des problèmes de boot et communication USB

## Date : 9 décembre 2025
## Version : 3.21.0 (à venir)

---

## Contexte

L'ancien mapping des pins ESP32 Classic (DevKitC) présentait plusieurs problèmes critiques :
- **Problèmes de boot** : utilisation de broches de strapping (GPIO 0, 2, 4, 5, 12, 15) sans précautions
- **Conflits USB-UART** : GPIO 1 (TX0) et GPIO 3 (RX0) utilisés pour des périphériques externes
- **Boutons inadaptés** : GPIO 34 et 35 utilisés comme boutons mais ce sont des pins input-only
- **LED RGB sur pins de strapping** : LED connectées à GPIO 12, 14, 15 causant des états forcés au boot

Ces problèmes pouvaient empêcher le boot, bloquer le flashing, ou causer des comportements imprévisibles.

---

## Modifications numérotées du Pin Mapping

### 1️⃣ **GPS PPS : GPIO 4 → GPIO 36**
**Ancien:** `#define PIN_GPS_PPS 4`  
**Nouveau:** `#define PIN_GPS_PPS 36`

**Raison:** GPIO 4 est une **broche de strapping** critique pour le boot (contrôle du mode SDIO). Un niveau forcé par le GPS PPS pourrait empêcher le démarrage. GPIO 36 (VP) est une entrée dédiée, idéale pour un signal PPS (Pulse Per Second).

---

### 2️⃣ **TFT CS (Chip Select) : GPIO 19 → GPIO 27**
**Ancien:** `#define TFT_CS 19`  
**Nouveau:** `#define TFT_CS 27`

**Raison:** GPIO 19 est proche des pins UART0 (USB-UART) et pouvait causer des interférences lors du flashing. GPIO 27 est plus sûr, sans conflit avec les interfaces critiques.

---

### 3️⃣ **TFT DC (Data/Command) : GPIO 27 → GPIO 14**
**Ancien:** `#define TFT_DC 27`  
**Nouveau:** `#define TFT_DC 14`

**Raison:** Échange de position avec CS pour une meilleure organisation du câblage. GPIO 14 reste une broche de strapping mais est acceptable pour un signal de contrôle (DC) car il n'est actif qu'après le boot.

---

### 4️⃣ **TFT RST (Reset) : GPIO 26 → GPIO 25**
**Ancien:** `#define TFT_RST 26`  
**Nouveau:** `#define TFT_RST 25`

**Raison:** GPIO 26 était proche de GPIO 27 (conflits potentiels). GPIO 25 offre un meilleur groupement physique des pins TFT et évite les interférences avec ADC2 lors de l'utilisation Wi-Fi.

---

### 5️⃣ **TFT BL (Backlight) : GPIO 13 → GPIO 32**
**Ancien:** `#define TFT_BL 13`  
**Nouveau:** `#define TFT_BL 32`

**Raison:** GPIO 13 est souvent utilisé pour la LED interne sur certaines cartes et peut causer des conflits. GPIO 32 est plus sûr et peut gérer le rétroéclairage sans interférence.

---

### 6️⃣ **LED RGB Rouge : GPIO 12 → GPIO 13**
**Ancien:** `#define DEFAULT_RGB_LED_PIN_R 12`  
**Nouveau:** `#define DEFAULT_RGB_LED_PIN_R 13`

**Raison:** **GPIO 12 est une broche de strapping critique** (MTDI, contrôle la tension flash). Une LED allumée au boot peut forcer un mauvais niveau et empêcher le démarrage. GPIO 13 est plus sûr pour une LED.

---

### 7️⃣ **LED RGB Bleue : GPIO 15 → GPIO 25**
**Ancien:** `#define DEFAULT_RGB_LED_PIN_B 15`  
**Nouveau:** `#define DEFAULT_RGB_LED_PIN_B 25`

**Raison:** **GPIO 15 est une broche de strapping** (MTDO, contrôle le mode debug JTAG). Une LED bleue allumée peut bloquer le boot. GPIO 25 n'est pas une broche de strapping et convient parfaitement.

---

### 8️⃣ **Bouton 1 : GPIO 34 → GPIO 32**
**Ancien:** `#define PIN_BUTTON_1 34`  
**Nouveau:** `#define PIN_BUTTON_1 32`

**Raison:** GPIO 34 est **input-only** (pas de pull-up interne), nécessitant une résistance externe. GPIO 32 supporte `INPUT_PULLUP` en interne, simplifiant le câblage.

---

### 9️⃣ **Distance TRIG (HC-SR04) : GPIO 32 → GPIO 27**
**Ancien:** `#define DEFAULT_DISTANCE_TRIG_PIN 32`  
**Nouveau:** `#define DEFAULT_DISTANCE_TRIG_PIN 27`

**Raison:** GPIO 32 était réaffecté au Bouton 1. GPIO 27 est une sortie standard parfaite pour le signal TRIG du capteur ultrason.

---

### 🔟 **DHT Température/Humidité : GPIO 25 → GPIO 32**
**Ancien:** `#define DEFAULT_DHT_PIN 25`  
**Nouveau:** `#define DEFAULT_DHT_PIN 32`

**Raison:** GPIO 25 était réaffecté à la LED bleue. GPIO 32 supporte le protocole DHT et peut partager la pin avec le Bouton 1 si géré correctement en temps (non simultané).

---

### 1️⃣1️⃣ **Capteur de mouvement (PIR) : GPIO 36 supprimé**
**Ancien:** `#define DEFAULT_MOTION_SENSOR_PIN 36`  
**Nouveau:** *(supprimé du mapping)*

**Raison:** GPIO 36 est maintenant utilisé par GPS PPS. Le capteur de mouvement peut être réaffecté dynamiquement via l'interface web si nécessaire (pin libre : GPIO 26, 33, 35, etc.).

---

## Ajout de la section de sécurité

Un bloc de **rappels de sécurité** a été ajouté dans `board_config.h` :
- Tension logique 3.3V uniquement
- Liste des broches de strapping à éviter
- GPIO input-only (34, 35, 36, 39)
- Protection UART0 (GPIO 1/3)
- Limites de courant GPIO (≤12 mA)
- Recommandations pull-up I2C (4.7 kΩ)

---

## Résumé des bénéfices

✅ **Résolution des problèmes de boot** : élimination des LED et périphériques sur GPIO 12, 15  
✅ **Stabilité du flashing** : libération des pins proches de UART0  
✅ **Boutons fonctionnels** : utilisation de GPIO avec pull-up interne (32, 33)  
✅ **Compatibilité Wi-Fi** : évitement des conflits ADC2  
✅ **Documentation renforcée** : annotations claires avec tensions et résistances recommandées  

---

## Tableau récapitulatif

| # | Périphérique | Ancien GPIO | Nouveau GPIO | Raison principale |
|---|--------------|-------------|--------------|-------------------|
| 1 | GPS PPS | 4 | 36 | GPIO4 = strapping (SDIO boot) |
| 2 | TFT CS | 19 | 27 | Éviter interférences USB-UART |
| 3 | TFT DC | 27 | 14 | Réorganisation câblage |
| 4 | TFT RST | 26 | 25 | Meilleur groupement pins TFT |
| 5 | TFT BL | 13 | 32 | GPIO13 conflit LED interne |
| 6 | RGB Rouge | 12 | 13 | GPIO12 = strapping (tension flash) |
| 7 | RGB Bleue | 15 | 25 | GPIO15 = strapping (JTAG debug) |
| 8 | Bouton 1 | 34 | 32 | GPIO34 = input-only (pas de pull-up) |
| 9 | Distance TRIG | 32 | 27 | GPIO32 réaffecté au bouton |
| 10 | DHT | 25 | 32 | GPIO25 réaffecté à LED bleue |
| 11 | Mouvement PIR | 36 | *(supprimé)* | GPIO36 réaffecté au GPS PPS |

---

## Impact pour les utilisateurs

### Migration matérielle requise
Les utilisateurs avec des cartes ESP32 Classic devront **recâbler leurs périphériques** selon le nouveau mapping. Le tableau ci-dessus indique précisément les changements à effectuer.

### Compatibilité ascendante
Les anciennes versions du firmware (≤ v3.20.3) utilisent l'ancien mapping. Après mise à jour vers v3.21.0+, le nouveau mapping s'applique automatiquement.

### Configuration dynamique
L'interface web permet toujours de modifier certains pins de capteurs sans recompiler, offrant de la flexibilité si le nouveau mapping pose problème dans un cas spécifique.

---

**Document généré le 9 décembre 2025**  
**Auteur:** Équipe ESP32-Diagnostic  
**Référence:** `include/board_config.h`, commit du 9 déc. 2025
