# Démarrage Rapide MQTT - 5 minutes

## Qu'est-ce que MQTT ?

MQTT (Message Queuing Telemetry Transport) est un protocole léger pour publier des données en temps réel vers un broker central. Parfait pour la surveillance IoT !

Exemple : ESP32 publie `memory/heap_free=65536` → Broker stocke → Home Assistant l'affiche sur le dashboard.

---

## 1️⃣ Configuration Rapide (< 2 minutes)

### Option A : Utiliser le Broker Public HiveMQ (Test)
```cpp
// Dans include/config.h
#define ENABLE_MQTT_BRIDGE true
#define MQTT_BROKER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_TOPIC_PREFIX "esp32-diagnostic"
```

Compiler et flasher :
```bash
pio run -e esp32s3_n16r8 -t upload
```

### Option B : Utiliser Mosquitto Localement
```bash
# Installer Mosquitto localement (macOS)
brew install mosquitto
mosquitto -v

# Ou Docker
docker run -it -p 1883:1883 eclipse-mosquitto
```

Ensuite mettre à jour config.h :
```cpp
#define MQTT_BROKER "localhost"   // ou 192.168.1.100
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""
```

---

## 2️⃣ Vérifier la Connexion (< 1 minute)

### Vérifier le Statut MQTT
```bash
curl http://192.168.1.100/api/mqtt-status
```

Devrait retourner :
```json
{
  "state": "connected",
  "broker": "broker.hivemq.com",
  "port": 1883,
  "topic_prefix": "esp32-diagnostic",
  "enabled": true
}
```

### Publier un Message de Test
```bash
curl http://192.168.1.100/api/mqtt-publish-test
```

### Observer les Messages Entrants
```bash
# Terminal 1 : S'abonner à tous les topics
mosquitto_sub -h broker.hivemq.com -t "esp32-diagnostic/#" -v

# Terminal 2 : Après quelques secondes, vous verrez :
# esp32-diagnostic/memory/heap_free 65536
# esp32-diagnostic/network/wifi_rssi -45
# esp32-diagnostic/system/uptime_seconds 3600
```

---

## 3️⃣ Intégration Home Assistant (< 2 minutes)

Ajouter à Home Assistant `configuration.yaml` :

```yaml
mqtt:
  broker: broker.hivemq.com
  port: 1883

sensor:
  - platform: mqtt
    name: "ESP32 Mémoire"
    state_topic: "esp32-diagnostic/memory/heap_free"
    unit_of_measurement: "octets"
    icon: "mdi:memory"
    
  - platform: mqtt
    name: "ESP32 WiFi"
    state_topic: "esp32-diagnostic/network/wifi_rssi"
    unit_of_measurement: "dBm"
    icon: "mdi:wifi"
    
  - platform: mqtt
    name: "ESP32 Uptime"
    state_topic: "esp32-diagnostic/system/uptime_seconds"
    unit_of_measurement: "s"
    icon: "mdi:clock"
```

Ensuite redémarrer Home Assistant → Les capteurs apparaissent automatiquement ! 🎉

---

## 4️⃣ Topics Courants

### Système
```
esp32-diagnostic/system/uptime_seconds       → Uptime de l'appareil
esp32-diagnostic/system/temperature_c        → Température CPU
esp32-diagnostic/system/chip_model           → Modèle ESP32
```

### Mémoire
```
esp32-diagnostic/memory/heap_free            → RAM libre
esp32-diagnostic/memory/heap_percent         → Pourcentage d'utilisation
esp32-diagnostic/memory/psram_free           → PSRAM libre (si disponible)
```

### Réseau
```
esp32-diagnostic/network/wifi_rssi           → Force du signal (dBm)
esp32-diagnostic/network/wifi_channel        → Canal WiFi
```

### Capteurs (Auto-détectés)
```
esp32-diagnostic/sensors/env/temperature     → Température env.
esp32-diagnostic/sensors/gps/latitude        → Position GPS
```

---

## 5️⃣ Dépannage

### Statut "disconnected"

**Problème** : MQTT affiche "disconnected"

**Solution** :
```bash
# Tester la connectivité du broker
ping broker.hivemq.com

# Vérifier si le port est ouvert
nc -zv broker.hivemq.com 1883

# Vérifier le log sériel ESP32
# Devrait afficher : [MQTT] Connected to broker.hivemq.com:1883
```

### Les Métriques Ne Sont Pas Publiées

**Problème** : MQTT connecté mais pas de données

**Solution** :
1. Activer le mode verbose dans `config.h` :
   ```cpp
   #define DIAGNOSTIC_VERBOSE 1
   ```

2. Vérifier la sortie sériel :
   ```
   [MQTT] Published esp32-diagnostic/memory/heap_free = 65536
   ```

3. Vérifier que l'abonné écoute :
   ```bash
   mosquitto_sub -v -h broker.hivemq.com -t "esp32-diagnostic/#"
   ```

### WiFi Connecté mais MQTT Échoue

**Problème** : "WiFi OK mais MQTT disconnected"

**Solution** : MQTT ne démarre qu'après WiFi. Attendre ~30s pour la première publication.

---

## 🎯 Niveau Suivant : Dashboard NodeRED

Créer un dashboard interactif en temps réel :

```bash
# Installer NodeRED
npm install -g node-red

# Le lancer
node-red

# Accéder à http://localhost:1880
```

Importer ce flux (Menu → Importer) :
```json
[
  {"id":"n1","type":"mqtt in","topic":"esp32-diagnostic/#"},
  {"id":"n2","type":"debug","name":"Afficher Toutes les Métriques"},
  {"id":"n3","type":"ui_gauge","title":"Mémoire %"},
  {"id":"n4","type":"ui_chart","title":"Mémoire au Fil du Temps"}
]
```

---

## 📚 Documentation

- Guide complet : [MQTT_INTEGRATION_FR.md](MQTT_INTEGRATION_FR.md)
- Guide anglais : [MQTT_INTEGRATION.md](MQTT_INTEGRATION.md)
- Config exemple : [mqtt-config-example.h](mqtt-config-example.h)

---

## ⚡ Points Clés

✅ **Léger** : ~18KB flash, ~3-5KB RAM  
✅ **Non-bloquant** : Publications toutes les 30 secondes  
✅ **Sûr pour Classic** : Fonctionne sur ESP32 4MB  
✅ **Compatible** : Home Assistant, NodeRED, InfluxDB, Grafana, etc.  
⚠️ **Pas encore supporté** : TLS/SSL, abonnement aux commandes  

---

**Prêt à y aller ! 🚀**

Des questions ? Consulter [MQTT_INTEGRATION_FR.md](MQTT_INTEGRATION_FR.md) pour la documentation complète.
