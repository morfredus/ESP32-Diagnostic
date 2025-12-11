# MQTT Quick Start - 5 Minutes

## What is MQTT?

MQTT (Message Queuing Telemetry Transport) is a lightweight protocol for publishing real-time data to a central broker. Perfect for IoT monitoring!

Example: ESP32 publishes `memory/heap_free=65536` → Broker stores it → Home Assistant displays it on dashboard.

---

## 1️⃣ Quick Setup (< 2 minutes)

### Option A: Use Public HiveMQ Broker (Testing)
```cpp
// In include/config.h
#define ENABLE_MQTT_BRIDGE true
#define MQTT_BROKER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_TOPIC_PREFIX "esp32-diagnostic"
```

Compile & flash:
```bash
pio run -e esp32s3_n16r8 -t upload
```

### Option B: Use Your Local Mosquitto
```bash
# Install Mosquitto locally (macOS)
brew install mosquitto
mosquitto -v

# Or Docker
docker run -it -p 1883:1883 eclipse-mosquitto
```

Then update config.h:
```cpp
#define MQTT_BROKER "localhost"   // or 192.168.1.100
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""
```

---

## 2️⃣ Verify Connection (< 1 minute)

### Check MQTT Status
```bash
curl http://192.168.1.100/api/mqtt-status
```

Should return:
```json
{
  "state": "connected",
  "broker": "broker.hivemq.com",
  "port": 1883,
  "topic_prefix": "esp32-diagnostic",
  "enabled": true
}
```

### Publish Test Message
```bash
curl http://192.168.1.100/api/mqtt-publish-test
```

### Watch Incoming Messages
```bash
# Terminal 1: Subscribe to all topics
mosquitto_sub -h broker.hivemq.com -t "esp32-diagnostic/#" -v

# Terminal 2: After a few seconds, you'll see:
# esp32-diagnostic/memory/heap_free 65536
# esp32-diagnostic/network/wifi_rssi -45
# esp32-diagnostic/system/uptime_seconds 3600
```

---

## 3️⃣ Home Assistant Integration (< 2 minutes)

Add to Home Assistant `configuration.yaml`:

```yaml
mqtt:
  broker: broker.hivemq.com
  port: 1883

sensor:
  - platform: mqtt
    name: "ESP32 Memory"
    state_topic: "esp32-diagnostic/memory/heap_free"
    unit_of_measurement: "bytes"
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

Then restart Home Assistant → Sensors appear automatically! 🎉

---

## 4️⃣ Common Topics

### System
```
esp32-diagnostic/system/uptime_seconds       → Device uptime
esp32-diagnostic/system/temperature_c        → CPU temperature
esp32-diagnostic/system/chip_model           → ESP32 model
```

### Memory
```
esp32-diagnostic/memory/heap_free            → Free RAM
esp32-diagnostic/memory/heap_percent         → Usage %
esp32-diagnostic/memory/psram_free           → PSRAM free (if available)
```

### Network
```
esp32-diagnostic/network/wifi_rssi           → Signal strength (dBm)
esp32-diagnostic/network/wifi_channel        → WiFi channel
```

### Sensors (Auto-detected)
```
esp32-diagnostic/sensors/env/temperature     → Environmental temp
esp32-diagnostic/sensors/gps/latitude        → GPS position
```

---

## 5️⃣ Troubleshooting

### "disconnected" Status

**Problem**: MQTT shows "disconnected"

**Solution**:
```bash
# Test broker connectivity
ping broker.hivemq.com

# Check if port is open
nc -zv broker.hivemq.com 1883

# Check ESP32 serial log
# Should show: [MQTT] Connected to broker.hivemq.com:1883
```

### Metrics Not Publishing

**Problem**: MQTT connected but no data

**Solution**:
1. Enable verbose mode in `config.h`:
   ```cpp
   #define DIAGNOSTIC_VERBOSE 1
   ```

2. Check serial output:
   ```
   [MQTT] Published esp32-diagnostic/memory/heap_free = 65536
   ```

3. Verify subscriber is listening:
   ```bash
   mosquitto_sub -v -h broker.hivemq.com -t "esp32-diagnostic/#"
   ```

### WiFi Connected but MQTT Failing

**Problem**: "WiFi OK but MQTT disconnected"

**Solution**: MQTT only starts after WiFi connects. Wait ~30s for first publish.

---

## 🎯 Next Level: NodeRED Dashboard

Create an interactive real-time dashboard:

```bash
# Install NodeRED
npm install -g node-red

# Run it
node-red

# Access at http://localhost:1880
```

Import this flow (Menu → Import):
```json
[
  {"id":"n1","type":"mqtt in","topic":"esp32-diagnostic/#"},
  {"id":"n2","type":"debug","name":"Show All Metrics"},
  {"id":"n3","type":"ui_gauge","title":"Memory %"},
  {"id":"n4","type":"ui_chart","title":"Memory Over Time"}
]
```

---

## 📚 Documentation

- Full guide: [MQTT_INTEGRATION.md](MQTT_INTEGRATION.md)
- French guide: [MQTT_INTEGRATION_FR.md](MQTT_INTEGRATION_FR.md)
- Example config: [mqtt-config-example.h](mqtt-config-example.h)

---

## ⚡ Key Points

✅ **Lightweight**: ~18KB flash, ~3-5KB RAM  
✅ **Non-blocking**: Publishes every 30 seconds  
✅ **Safe for Classic**: Works on 4MB ESP32  
✅ **Works with**: Home Assistant, NodeRED, InfluxDB, Grafana, etc.  
⚠️ **Not supported yet**: TLS/SSL, command subscriptions  

---

**Ready to go! 🚀**

Questions? Check [MQTT_INTEGRATION.md](MQTT_INTEGRATION.md) for full documentation.
