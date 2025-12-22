
# Synthèse des changements de mapping de pins (décembre 2025)

Ce document liste les changements appliqués pour garantir la cohérence totale entre le code source, la configuration et la documentation, en prenant `include/board_config.h` comme référence absolue pour le nommage et l'affectation des pins.

## Règle de référence
- **Seul le fichier `include/board_config.h` fait foi** pour le nommage et l'affectation des broches.
- Toutes les autres occurrences dans le code, la configuration et la documentation ont été alignées sur ce fichier.

## Changements principaux

### 1. Harmonisation des noms de pins dans le code source
- Tous les fichiers `.cpp` et `.h` du projet utilisent désormais **exclusivement** les macros et noms définis dans `board_config.h` pour accéder aux broches matérielles (ex : `GPS_RXD`, `TFT_MOSI`, `LED_RED`, etc.).
- Suppression des alias ou redéfinitions locales de pins (ex : `DEFAULT_I2C_SDA`, `DEFAULT_RGB_LED_PIN_R`, etc.) au profit des noms standards de `board_config.h`.
- Les initialisations dynamiques de pins dans le code utilisent les macros de `board_config.h`.

### 2. Mise à jour de la configuration
- Les sections de configuration (`config.h`, `config-example.h`) ne redéfinissent plus de pins déjà présentes dans `board_config.h`.
- Les valeurs par défaut pour les périphériques (I2C, LED, capteurs, etc.) pointent vers les macros de `board_config.h`.

### 3. Documentation utilisateur
- Tous les documents de la section `docs/` (notamment `PIN_MAPPING.md`, `PIN_MAPPING_FR.md`, `USAGE.md`, `USAGE_FR.md`, etc.) ont été relus et corrigés pour refléter **exactement** le mapping et les noms de pins de `board_config.h`.
- Les exemples de code, tableaux de correspondance et schémas de câblage utilisent les bons noms et numéros de GPIO.

### 4. Historique et traçabilité
- Un numéro de changement a été ajouté dans `platformio.ini` pour tracer cette harmonisation (voir la section [Changelog] dans ce document ou dans `platformio.ini`).

## Exemple de correspondance (extrait)
| Fonction           | Nom macro         | GPIO ESP32-S3 | GPIO ESP32 Classic |
|--------------------|-------------------|---------------|--------------------|
| I2C SDA            | I2C_SDA           | 15            | 21                 |
| I2C SCL            | I2C_SCL           | 16            | 22                 |
| LED Rouge          | LED_RED           | 21            | 13                 |
| LED Verte          | LED_GREEN         | 41            | 26                 |
| LED Bleue          | LED_BLUE          | 42            | 33                 |
| NeoPixel           | NEOPIXEL          | 48            | -1                 |
| Bouton BOOT        | BUTTON_BOOT       | 0             | 0                  |
| Bouton 1           | BUTTON_1          | 38            | 5                  |
| Bouton 2           | BUTTON_2          | 39            | 12                 |
| Buzzer             | BUZZER            | 6             | 19                 |
| Capteur DHT        | DHT               | 5             | 15                 |
| Capteur lumière    | LIGHT_SENSOR      | 4             | 39                 |
| Capteur distance T | DISTANCE_TRIG     | 2             | 1                  |
| Capteur distance E | DISTANCE_ECHO     | 35            | 35                 |
| GPS RX             | GPS_RXD           | 18            | 16                 |
| GPS TX             | GPS_TXD           | 17            | 17                 |
| GPS PPS            | GPS_PPS           | 8             | 36                 |
| TFT MOSI           | TFT_MOSI          | 11            | 23                 |
| TFT SCLK           | TFT_SCLK          | 12            | 18                 |
| TFT CS             | TFT_CS            | 10            | 27                 |
| TFT DC             | TFT_DC            | 9             | 14                 |
| TFT RST            | TFT_RST           | 13            | 25                 |
| TFT BL             | TFT_BL            | 7             | 32                 |

Pour la liste exhaustive, se référer à `include/board_config.h`.

---

**Remarque :**
Aucune fonctionnalité n'a été ajoutée ou supprimée. Seule la cohérence de nommage et de mapping a été assurée sur l'ensemble du projet.

Dernière mise à jour : 21 décembre 2025.

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
