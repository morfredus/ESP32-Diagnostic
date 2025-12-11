# MQTT Integration Guide - ESP32 Diagnostic Suite

## Overview

Version 3.23.0 introduces **MQTT Publisher** capability for real-time metric streaming to brokers like Mosquitto, Home Assistant, NodeRED, or InfluxDB.

> **Note**: MQTT integration is **disabled by default** to save flash space. Enable via `ENABLE_MQTT_BRIDGE` in `config.h`.

---

## Configuration

### 1. Enable MQTT in `include/config.h`

```cpp
#define ENABLE_MQTT_BRIDGE true           // Enable MQTT
#define MQTT_BROKER "mqtt.example.com"    // Broker hostname/IP
#define MQTT_PORT 1883                     // Standard MQTT port (1883 or 8883 for TLS)
#define MQTT_USER "username"               // Username (leave empty if anonymous)
#define MQTT_PASSWORD "password"           // Password (leave empty if anonymous)
#define MQTT_TOPIC_PREFIX "esp32-diagnostic" // Topic prefix
```

### 2. Update `platformio.ini` (Already Done)

The PubSubClient library is already added:
```ini
knolleary/PubSubClient@^2.8.0
```

### 3. Compile & Flash

```bash
# For ESP32-S3 N16R8 (primary)
pio run -e esp32s3_n16r8 -t upload

# For ESP32 Classic
pio run -e esp32devkitc -t upload
```

---

## Published Topics

All metrics are published automatically every **30 seconds** when MQTT is connected.

### System Topics
```
esp32-diagnostic/system/uptime_seconds     → Uptime in seconds (e.g., "3600")
esp32-diagnostic/system/uptime_ms          → Uptime in milliseconds
esp32-diagnostic/system/temperature_c      → CPU temperature in °C (e.g., "42.3")
esp32-diagnostic/system/chip_model         → Chip model (e.g., "ESP32-S3")
esp32-diagnostic/system/cpu_freq           → CPU frequency (e.g., "240 MHz")
```

### Memory Topics
```
esp32-diagnostic/memory/heap_free          → Free heap in bytes
esp32-diagnostic/memory/heap_total         → Total heap in bytes
esp32-diagnostic/memory/heap_percent       → Heap usage percentage (0-100)
esp32-diagnostic/memory/psram_free         → Free PSRAM in bytes (if available)
esp32-diagnostic/memory/psram_total        → Total PSRAM in bytes
esp32-diagnostic/memory/psram_percent      → PSRAM usage percentage
```

### Network Topics
```
esp32-diagnostic/network/wifi_rssi         → WiFi signal strength in dBm (e.g., "-45")
esp32-diagnostic/network/wifi_channel      → WiFi channel (e.g., "6")
```

### Sensor Topics (Optional, When Detected)
```
esp32-diagnostic/sensors/env/temperature   → Environmental temp from AHT20
esp32-diagnostic/sensors/env/humidity      → Humidity from AHT20
esp32-diagnostic/sensors/env/pressure_hpa  → Pressure from BMP280
esp32-diagnostic/sensors/env/altitude_m    → Altitude from BMP280
esp32-diagnostic/sensors/gps/latitude      → GPS latitude
esp32-diagnostic/sensors/gps/longitude     → GPS longitude
esp32-diagnostic/sensors/gps/altitude      → GPS altitude
esp32-diagnostic/sensors/gps/satellites    → Satellite count
esp32-diagnostic/sensors/gps/hdop          → Horizontal dilution of precision
```

### Test Result Topics
```
esp32-diagnostic/tests/gpio/status         → GPIO test result (PASS/FAIL/WARN)
esp32-diagnostic/tests/gpio/duration_ms    → GPIO test duration
esp32-diagnostic/tests/memory/status       → Memory test result
esp32-diagnostic/tests/memory/duration_ms  → Memory test duration
```

### Startup Message
```
esp32-diagnostic/status/startup             → {"hostname":"esp32-diagnostic","version":"3.23.0","ip":"192.168.1.100","uptime":0}
(Published with retain flag when connecting)
```

---

## REST API Endpoints

### Check MQTT Status
```bash
curl http://192.168.1.100/api/mqtt-status
```

Response:
```json
{
  "state": "connected",        // "connected", "connecting", "disconnected", "disabled"
  "broker": "mqtt.example.com",
  "port": 1883,
  "topic_prefix": "esp32-diagnostic",
  "enabled": true
}
```

### Enable/Disable MQTT
```bash
# Enable MQTT
curl "http://192.168.1.100/api/mqtt-enable?enable=true"

# Disable MQTT
curl "http://192.168.1.100/api/mqtt-enable?enable=false"
```

### Publish Test Message
```bash
curl http://192.168.1.100/api/mqtt-publish-test
```

---

## Usage Examples

### 1. Home Assistant Integration

Add to `configuration.yaml`:

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
    name: "ESP32 Memory Free"
    state_topic: "esp32-diagnostic/memory/heap_free"
    unit_of_measurement: "bytes"
    
  - platform: mqtt
    name: "ESP32 WiFi RSSI"
    state_topic: "esp32-diagnostic/network/wifi_rssi"
    unit_of_measurement: "dBm"
    
  - platform: mqtt
    name: "ESP32 CPU Temperature"
    state_topic: "esp32-diagnostic/system/temperature_c"
    unit_of_measurement: "°C"
```

### 2. NodeRED Flow

Simple debug flow to monitor all metrics:

```json
[
  {
    "id": "mqtt-subscribe",
    "type": "mqtt in",
    "broker": "mqtt-broker",
    "topic": "esp32-diagnostic/#",
    "qos": "0",
    "datatype": "auto",
    "name": "ESP32 Metrics"
  },
  {
    "id": "debug-output",
    "type": "debug",
    "name": "Display Metrics"
  }
]
```

### 3. InfluxDB + Grafana

Configure ESP32 to publish metrics → InfluxDB → Visualize in Grafana.

**Simple Telegraf config** (bridge MQTT → InfluxDB):

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

Then create Grafana dashboard with queries like:
```
SELECT uptime FROM "esp32-diagnostic/system/uptime_seconds" WHERE time > now() - 1h
```

### 4. Custom MQTT Subscriber (Python)

```python
import paho.mqtt.client as mqtt
import json

def on_connect(client, userdata, flags, rc):
    print(f"Connected with code {rc}")
    client.subscribe("esp32-diagnostic/#")

def on_message(client, userdata, msg):
    topic = msg.topic
    payload = msg.payload.decode()
    print(f"{topic} = {payload}")
    
    # Custom logic: Alert if memory usage > 85%
    if "memory/heap_percent" in topic:
        usage = float(payload)
        if usage > 85:
            print("⚠️  WARNING: High heap usage!")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("mqtt.example.com", 1883, 60)
client.loop_forever()
```

---

## Troubleshooting

### MQTT State Shows "disconnected"

1. **Check broker connectivity:**
   ```bash
   mosquitto_sub -h mqtt.example.com -u username -P password -t "esp32-diagnostic/#"
   ```

2. **Verify credentials** in `config.h`

3. **Check firewall:** Port 1883 must be open (or 8883 for TLS)

4. **Check serial log:**
   ```
   [MQTT] Connected to mqtt.example.com:1883
   ```

### MQTT Enabled but Metrics Not Publishing

1. Enable DIAGNOSTIC_VERBOSE in `config.h`:
   ```cpp
   #define DIAGNOSTIC_VERBOSE 1
   ```

2. Check console output:
   ```
   [MQTT] Published esp32-diagnostic/memory/heap_free = 65536
   ```

3. Verify broker is receiving messages:
   ```bash
   mosquitto_sub -v -h mqtt.example.com -u username -P password -t "esp32-diagnostic/#"
   ```

### Connection Timeout

1. Check **WiFi first** - MQTT only starts if WiFi is connected
2. Broker might be unreachable - test ping
3. Try removing MQTT_USER/MQTT_PASSWORD if using anonymous broker

---

## Performance Impact

| Component | Flash (bytes) | SRAM (runtime) | Note |
|-----------|---------------|----------------|------|
| MQTT Handler Code | ~3KB | ~1KB state | Minimal overhead |
| PubSubClient Library | ~15KB | ~2-4KB buffers | Well-optimized |
| Publishing Loop | 0 | 0 (non-blocking) | Every 30 seconds |
| **Total** | **~18KB** | **~3-5KB** | **Safe for Classic!** |

---

## Limitations & Notes

- **TLS/SSL**: Current implementation uses plain MQTT (port 1883). For TLS (port 8883), additional configuration required.
- **QoS Level**: All publishes use QoS 0 (fire-and-forget). For QoS 1/2, modify `mqtt_handler.h`.
- **Subscription**: Currently publish-only. For subscribing to commands, extend `MQTTHandler` class.
- **Retained Messages**: Startup message uses retain flag; others don't. Customize in `mqtt_handler.h`.

---

## Next Steps

- ✅ Basic MQTT publishing
- 🔄 Future: TLS/SSL support
- 🔄 Future: Command subscriptions (e.g., `esp32-diagnostic/commands/restart`)
- 🔄 Future: OTA firmware updates via MQTT

---

**Author**: ESP32 Diagnostic Suite Team  
**Last Updated**: December 11, 2025
