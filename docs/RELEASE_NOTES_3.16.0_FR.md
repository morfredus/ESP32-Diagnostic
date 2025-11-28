# ESP32 Diagnostic Suite – Notes de version v3.16.0

**Date de sortie :** 28 novembre 2025  
**Version précédente :** 3.15.1  
**Dépôt :** [github.com/morfredus/ESP32-Diagnostic](https://github.com/morfredus/ESP32-Diagnostic)

---

## Vue d'ensemble

La version 3.16.0 introduit des capacités complètes de configuration d'affichage via l'interface web, une surveillance réseau améliorée et des outils de diagnostic perfectionnés. Cette version permet la configuration dynamique des écrans OLED et TFT sans recompilation du code, rendant le système plus flexible pour diverses configurations matérielles.

---

## Nouveautés

### 1. Amélioration de la surveillance réseau
- **Journalisation des IP clients** : Toutes les connexions clients sont maintenant automatiquement enregistrées dans le moniteur série avec leurs adresses IP
- **Format** : `[Client] <endpoint> connected from IP: <address>`
- **Avantages** : Dépannage facilité de la connectivité réseau et surveillance de sécurité

### 2. Affichage du statut WiFi dans l'interface web
- **Informations de connexion en temps réel** : L'onglet WiFi affiche maintenant l'état de la connexion réseau actuelle avant le scanner WiFi
- **Informations affichées** :
  - État de connexion (Connecté/Déconnecté) avec code couleur
  - SSID actuel
  - Adresse IP actuelle
  - Adresse de la passerelle
  - Adresse du serveur DNS
  - Force du signal (RSSI en dBm)
- **Chargement automatique** : Les informations sont automatiquement récupérées depuis `/api/wifi-info` lors de l'ouverture de l'onglet Sans fil
- **Mises à jour en direct** : L'état de connexion est rafraîchi dynamiquement

### 3. Configuration OLED via interface web
- **Configuration de résolution** : Ajustement dynamique de la résolution de l'écran OLED (largeur × hauteur)
- **Configuration des broches** : Configuration existante SDA/SCL améliorée avec support de résolution
- **Paramètres de rotation** : Combiné avec les contrôles de rotation existants pour une configuration complète de l'affichage
- **API** : Endpoint `/api/oled-config` amélioré avec nouveaux paramètres `width` et `height`

### 4. Configuration TFT via interface web
- **Pin mapping complet** : Configuration de toutes les broches TFT via l'interface web :
  - MOSI (Données SPI)
  - SCLK (Horloge SPI)
  - CS (Chip Select)
  - DC (Data/Command)
  - RST (Reset)
  - BL (Rétroéclairage)
- **Configuration de résolution** : Ajustement de la résolution TFT (largeur × hauteur)
- **Paramètres de rotation** : Réglage de la rotation d'affichage (0-3)
- **Nouveau point d'API** : `/api/tft-config` pour une configuration complète avec validation

### 5. API d'information d'écrans améliorée
- **`/api/screens-info` mis à jour** : Retourne maintenant les détails de configuration complets pour les deux affichages :
  - OLED : SDA, SCL, rotation, largeur, hauteur
  - TFT : MOSI, SCLK, CS, DC, RST, BL, largeur, hauteur, rotation
- **État en temps réel** : Informations de disponibilité et d'état pour les deux écrans

---

## Améliorations techniques

### Structure du code
- Ajout de variables de configuration runtime : `oledWidth`, `oledHeight`, `tftMOSI`, `tftSCLK`, `tftCS`, `tftDC`, `tftRST`, `tftBL`, `tftWidth`, `tftHeight`, `tftRotation`
- Implémentation de la fonction helper `logClientConnection()` pour une journalisation cohérente des IP
- Amélioration de `handleOLEDConfig()` avec paramètres de résolution
- Création du nouveau handler `handleTFTConfig()` pour configuration TFT complète

### Modifications de l'API
- **Nouveau point d'API** : `POST /api/tft-config` avec paramètres :
  - Requis : `mosi`, `sclk`, `cs`, `dc`, `rst`
  - Optionnels : `bl`, `width`, `height`, `rotation`
- **Point d'API amélioré** : `/api/oled-config` accepte maintenant :
  - Existants : `sda`, `scl`, `rotation`
  - Nouveaux : `width`, `height`
- **Réponse mise à jour** : `/api/screens-info` inclut les détails complets de configuration

### Validation
- Validation de plage de broches (0-48 pour la plupart, -1 autorisé pour RST et BL)
- Validation de rotation (0-3)
- Mises à jour de configuration en temps réel avec retour d'information

---

## Compatibilité

### Support matériel
- **ESP32-S3 N16R8** : Entièrement supporté (16Mo Flash + 8Mo PSRAM)
- **ESP32-S3 N8R8** : Entièrement supporté (8Mo Flash + 8Mo PSRAM)
- **ESP32 Classic** : Entièrement supporté (4Mo Flash, sans PSRAM)

### Exigences logicielles
- **PlatformIO Core** : 6.0 ou supérieur
- **ESP32 Arduino Core** : 3.3.3 (recommandé)
- **Plateforme** : `espressif32@^6`

### Changements incompatibles
**Aucun.** Il s'agit d'une version de fonctionnalités rétrocompatible.

---

## Exemples d'utilisation

### Configuration OLED avec résolution
```bash
# Configurer l'OLED avec résolution personnalisée
curl -X GET "http://esp32-diagnostic.local/api/oled-config?sda=21&scl=20&rotation=0&width=128&height=64"
```

**Réponse :**
```json
{
  "success": true,
  "message": "I2C reconfigure: SDA:21 SCL:20 Rot:0 Res:128x64",
  "sda": 21,
  "scl": 20,
  "rotation": 0,
  "width": 128,
  "height": 64
}
```

### Configuration complète TFT
```bash
# Configurer complètement l'écran TFT
curl -X GET "http://esp32-diagnostic.local/api/tft-config?mosi=11&sclk=12&cs=10&dc=9&rst=7&bl=15&width=240&height=240&rotation=2"
```

**Réponse :**
```json
{
  "success": true,
  "message": "TFT config updated: MOSI:11 SCLK:12 CS:10 DC:9 RST:7 Res:240x240 Rot:2",
  "mosi": 11,
  "sclk": 12,
  "cs": 10,
  "dc": 9,
  "rst": 7,
  "bl": 15,
  "width": 240,
  "height": 240,
  "rotation": 2
}
```

### Interroger la configuration d'écran
```bash
# Obtenir la configuration actuelle de tous les écrans
curl -X GET "http://esp32-diagnostic.local/api/screens-info"
```

**Réponse :**
```json
{
  "oled": {
    "available": true,
    "status": "OK",
    "pins": {"sda": 21, "scl": 20},
    "rotation": 0,
    "width": 128,
    "height": 64
  },
  "tft": {
    "available": true,
    "status": "Ready",
    "width": 240,
    "height": 240,
    "rotation": 2,
    "pins": {
      "mosi": 11,
      "sclk": 12,
      "cs": 10,
      "dc": 9,
      "rst": 7,
      "bl": 15
    }
  }
}
```

---

## Sortie du moniteur série

### Journalisation des connexions clients
```
[Client] Web Interface connected from IP: 192.168.1.105
[Client] JS Resource connected from IP: 192.168.1.105
[Client] Web Interface connected from IP: 192.168.1.142
```

---

## Guide de migration

### De 3.15.1 vers 3.16.0

**Aucune migration requise.** Il suffit de mettre à jour le firmware :

1. Récupérer le dernier code depuis GitHub
2. Compiler avec PlatformIO
3. Téléverser sur votre appareil

**Optionnel :** Utiliser les nouvelles fonctionnalités de l'interface web pour configurer vos écrans dynamiquement.

---

## Limitations connues

1. **Réinitialisation TFT** : Bien que la configuration soit sauvegardée, la réinitialisation complète du TFT nécessite un redémarrage de l'appareil pour les changements matériels.
2. **Conflits de broches** : Aucune validation automatique des conflits de broches entre différents périphériques. Les utilisateurs doivent s'assurer que les broches sélectionnées n'entrent pas en conflit avec d'autres dispositifs.
3. **Types OLED** : La configuration de résolution suppose un écran OLED I2C compatible. Tailles courantes : 128×64, 128×32, 64×48.

---

## Améliorations futures

Fonctionnalités prévues pour les versions à venir :
- Stockage de configuration persistant (EEPROM/SPIFFS)
- Détection automatique de conflits de broches
- Rechargement à chaud du TFT sans redémarrage
- Suite de tests d'affichage pour résolutions personnalisées
- Support multi-écrans

---

## Mises à jour de la documentation

- **CHANGELOG_FR.md** : Historique complet des modifications
- **CHANGELOG.md** : Version anglaise des modifications
- **README_FR.md** : Liste des fonctionnalités mise à jour
- **README.md** : Liste des fonctionnalités mise à jour
- **API_REFERENCE_FR.md** : Mise à jour avec nouveaux endpoints (en attente)
- **BUILD_AND_DEPLOY_FR.md** : Références de version mises à jour

---

## Crédits

**Auteur :** morfredus  
**Contributeurs :** Retours et tests de la communauté  
**Licence :** [Type de licence]

---

## Support

- **Issues** : [GitHub Issues](https://github.com/morfredus/ESP32-Diagnostic/issues)
- **Discussions** : [GitHub Discussions](https://github.com/morfredus/ESP32-Diagnostic/discussions)
- **Documentation** : [docs/](https://github.com/morfredus/ESP32-Diagnostic/tree/main/docs)

---

**Merci d'utiliser ESP32 Diagnostic Suite !**
