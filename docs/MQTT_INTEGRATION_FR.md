# Guide d'Intégration MQTT - Suite de Diagnostic ESP32

## Vue d'ensemble

La version 3.23.0 introduit la capacité **MQTT Publisher** pour la diffusion en temps réel des métriques vers des brokers comme Mosquitto, Home Assistant, NodeRED ou InfluxDB.

> **Note** : L'intégration MQTT est **désactivée par défaut** pour économiser l'espace flash. Activez-la via `ENABLE_MQTT_BRIDGE` dans `config.h`.

---

## Configuration

### 1. Activer MQTT dans `include/config.h`

```cpp
#define ENABLE_MQTT_BRIDGE true           // Activer MQTT
#define MQTT_BROKER "mqtt.example.com"    // Hostname/IP du broker
#define MQTT_PORT 1883                     // Port MQTT standard (1883 ou 8883 pour TLS)
#define MQTT_USER "username"               // Utilisateur (laisser vide si anonyme)
#define MQTT_PASSWORD "password"           // Mot de passe (laisser vide si anonyme)
#define MQTT_TOPIC_PREFIX "esp32-diagnostic" // Préfixe des topics
```

### 2. Mise à jour de `platformio.ini` (Déjà fait)

La librairie PubSubClient est déjà ajoutée :
```ini
knolleary/PubSubClient@^2.8.0
```

### 3. Compilation & Flash

```bash
# Pour ESP32-S3 N16R8 (principal)
pio run -e esp32s3_n16r8 -t upload

# Pour ESP32 Classic
pio run -e esp32devkitc -t upload
```

---

## Topics Publiés

Toutes les métriques sont publiées automatiquement toutes les **30 secondes** lorsque MQTT est connecté.

### Topics Système
```
esp32-diagnostic/system/uptime_seconds     → Uptime en secondes (ex: "3600")
esp32-diagnostic/system/uptime_ms          → Uptime en millisecondes
esp32-diagnostic/system/temperature_c      → Température CPU en °C (ex: "42.3")
esp32-diagnostic/system/chip_model         → Modèle de chip (ex: "ESP32-S3")
esp32-diagnostic/system/cpu_freq           → Fréquence CPU (ex: "240 MHz")
```

### Topics Mémoire
```
esp32-diagnostic/memory/heap_free          → Heap libre en octets
esp32-diagnostic/memory/heap_total         → Total du heap en octets
esp32-diagnostic/memory/heap_percent       → Pourcentage d'utilisation du heap (0-100)
esp32-diagnostic/memory/psram_free         → PSRAM libre en octets (si disponible)
esp32-diagnostic/memory/psram_total        → Total PSRAM en octets
esp32-diagnostic/memory/psram_percent      → Pourcentage d'utilisation PSRAM
```

### Topics Réseau
```
esp32-diagnostic/network/wifi_rssi         → Force du signal WiFi en dBm (ex: "-45")
esp32-diagnostic/network/wifi_channel      → Canal WiFi (ex: "6")
```

### Topics Capteurs (Optionnel, si détectés)
```
esp32-diagnostic/sensors/env/temperature   → Temp. environnementale (AHT20)
esp32-diagnostic/sensors/env/humidity      → Humidité (AHT20)
esp32-diagnostic/sensors/env/pressure_hpa  → Pression (BMP280)
esp32-diagnostic/sensors/env/altitude_m    → Altitude (BMP280)
esp32-diagnostic/sensors/gps/latitude      → Latitude GPS
esp32-diagnostic/sensors/gps/longitude     → Longitude GPS
esp32-diagnostic/sensors/gps/altitude      → Altitude GPS
esp32-diagnostic/sensors/gps/satellites    → Nombre de satellites
esp32-diagnostic/sensors/gps/hdop          → Précision horizontale
```

### Topics Résultats de Tests
```
esp32-diagnostic/tests/gpio/status         → Résultat test GPIO (PASS/FAIL/WARN)
esp32-diagnostic/tests/gpio/duration_ms    → Durée test GPIO
esp32-diagnostic/tests/memory/status       → Résultat test mémoire
esp32-diagnostic/tests/memory/duration_ms  → Durée test mémoire
```

### Message au Démarrage
```
esp32-diagnostic/status/startup             → {"hostname":"esp32-diagnostic","version":"3.23.0","ip":"192.168.1.100","uptime":0}
(Publié avec le flag retain à la connexion)
```

---

## Endpoints API REST

### Vérifier le Statut MQTT
```bash
curl http://192.168.1.100/api/mqtt-status
```

Réponse :
```json
{
  "state": "connected",        // "connected", "connecting", "disconnected", "disabled"
  "broker": "mqtt.example.com",
  "port": 1883,
  "topic_prefix": "esp32-diagnostic",
  "enabled": true
}
```

### Activer/Désactiver MQTT
```bash
# Activer MQTT
curl "http://192.168.1.100/api/mqtt-enable?enable=true"

# Désactiver MQTT
curl "http://192.168.1.100/api/mqtt-enable?enable=false"
```

### Publier un Message de Test
```bash
curl http://192.168.1.100/api/mqtt-publish-test
```

---

## Exemples d'Utilisation

### 1. Intégration Home Assistant

Ajouter à `configuration.yaml` :

```yaml
mqtt:
  broker: mqtt.example.com
  port: 1883
  username: homeassistant
  password: password

sensor:
  - platform: mqtt
    name: "ESP32 Uptime"
    state_topic: "esp32-diagnostic/system/uptime_seconds"
    unit_of_measurement: "s"
    
  - platform: mqtt
    name: "ESP32 Mémoire libre"
    state_topic: "esp32-diagnostic/memory/heap_free"
    unit_of_measurement: "octets"
    
  - platform: mqtt
    name: "ESP32 Signal WiFi"
    state_topic: "esp32-diagnostic/network/wifi_rssi"
    unit_of_measurement: "dBm"
    
  - platform: mqtt
    name: "ESP32 Température CPU"
    state_topic: "esp32-diagnostic/system/temperature_c"
    unit_of_measurement: "°C"
```

### 2. Flow NodeRED

Simple flux de débogage pour surveiller toutes les métriques :

```json
[
  {
    "id": "mqtt-subscribe",
    "type": "mqtt in",
    "broker": "mqtt-broker",
    "topic": "esp32-diagnostic/#",
    "qos": "0",
    "datatype": "auto",
    "name": "Métriques ESP32"
  },
  {
    "id": "debug-output",
    "type": "debug",
    "name": "Afficher les Métriques"
  }
]
```

### 3. InfluxDB + Grafana

Configurer ESP32 pour publier les métriques → InfluxDB → Visualiser dans Grafana.

**Configuration Telegraf simple** (pont MQTT → InfluxDB) :

```toml
[[inputs.mqtt_consumer]]
  servers = ["tcp://mqtt.example.com:1883"]
  topics = ["esp32-diagnostic/#"]
  data_format = "value"
  data_type = "float"

[[outputs.influxdb_v2]]
  urls = ["http://influxdb.example.com:8086"]
  token = "your-influx-token"
  organization = "your-org"
  bucket = "esp32"
```

Ensuite créer un dashboard Grafana avec des requêtes comme :
```
SELECT uptime FROM "esp32-diagnostic/system/uptime_seconds" WHERE time > now() - 1h
```

### 4. Abonné MQTT Personnalisé (Python)

```python
import paho.mqtt.client as mqtt
import json

def on_connect(client, userdata, flags, rc):
    print(f"Connecté avec le code {rc}")
    client.subscribe("esp32-diagnostic/#")

def on_message(client, userdata, msg):
    topic = msg.topic
    payload = msg.payload.decode()
    print(f"{topic} = {payload}")
    
    # Logique personnalisée : Alerte si utilisation mémoire > 85%
    if "memory/heap_percent" in topic:
        usage = float(payload)
        if usage > 85:
            print("⚠️  ATTENTION : Utilisation heap élevée !")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("mqtt.example.com", 1883, 60)
client.loop_forever()
```

---

## Dépannage

### MQTT Affiche "disconnected"

1. **Vérifier la connectivité du broker :**
   ```bash
   mosquitto_sub -h mqtt.example.com -u username -P password -t "esp32-diagnostic/#"
   ```

2. **Vérifier les identifiants** dans `config.h`

3. **Vérifier le pare-feu** : Le port 1883 doit être ouvert (ou 8883 pour TLS)

4. **Vérifier le log sériel :**
   ```
   [MQTT] Connected to mqtt.example.com:1883
   ```

### MQTT Activé mais Métriques Non Publiées

1. Activer DIAGNOSTIC_VERBOSE dans `config.h` :
   ```cpp
   #define DIAGNOSTIC_VERBOSE 1
   ```

2. Vérifier la sortie console :
   ```
   [MQTT] Published esp32-diagnostic/memory/heap_free = 65536
   ```

3. Vérifier que le broker reçoit les messages :
   ```bash
   mosquitto_sub -v -h mqtt.example.com -u username -P password -t "esp32-diagnostic/#"
   ```

### Timeout de Connexion

1. Vérifier **WiFi en premier** - MQTT ne démarre que si WiFi est connecté
2. Le broker peut être inaccessible - tester le ping
3. Essayer de supprimer MQTT_USER/MQTT_PASSWORD si le broker est anonyme

---

## Impact Performance

| Composant | Flash (octets) | SRAM (runtime) | Note |
|-----------|----------------|----------------|------|
| Code MQTT Handler | ~3KB | ~1KB état | Surcharge minimale |
| Librairie PubSubClient | ~15KB | ~2-4KB buffers | Bien optimisée |
| Boucle Publication | 0 | 0 (non-bloquant) | Toutes les 30s |
| **Total** | **~18KB** | **~3-5KB** | **Sûr pour Classic !** |

---

## Limitations & Remarques

- **TLS/SSL** : L'implémentation actuelle utilise MQTT brut (port 1883). Pour TLS (port 8883), configuration supplémentaire requise.
- **Niveau QoS** : Toutes les publications utilisent QoS 0 (fire-and-forget). Pour QoS 1/2, modifier `mqtt_handler.h`.
- **Abonnement** : Actuellement publication seule. Pour s'abonner à des commandes, étendre la classe `MQTTHandler`.
- **Messages Retenus** : Le message de démarrage utilise le flag retain ; les autres non. Personnaliser dans `mqtt_handler.h`.

---

## Prochaines Étapes

- ✅ Publication MQTT de base
- 🔄 Futur : Support TLS/SSL
- 🔄 Futur : Abonnement à des commandes (ex: `esp32-diagnostic/commands/restart`)
- 🔄 Futur : Mises à jour de firmware via MQTT

---

**Auteur** : Équipe ESP32 Diagnostic Suite  
**Dernière Mise à Jour** : 11 décembre 2025
