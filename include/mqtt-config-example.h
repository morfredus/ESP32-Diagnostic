/*
 * MQTT CONFIGURATION EXAMPLE
 * Copy this to include/config.h and customize for your setup
 */

// ========== MQTT Configuration ==========

// Enable MQTT Publisher
// Set to true to enable MQTT publishing, false to disable
#define ENABLE_MQTT_BRIDGE true

// MQTT Broker Settings
#define MQTT_BROKER "mqtt.example.com"    // Broker hostname or IP
#define MQTT_PORT 1883                     // Port (1883 standard, 8883 for TLS)
#define MQTT_USER "username"               // Username (empty for anonymous)
#define MQTT_PASSWORD "password"           // Password (empty for anonymous)

// Topic Prefix
// All published topics will be prefixed with this:
// e.g., "esp32-diagnostic/memory/heap_free"
#define MQTT_TOPIC_PREFIX "esp32-diagnostic"

// ========== Common MQTT Broker Examples ==========

// --- Mosquitto (Local) ---
// #define MQTT_BROKER "mosquitto.local"
// #define MQTT_PORT 1883
// #define MQTT_USER ""
// #define MQTT_PASSWORD ""

// --- Home Assistant (Local) ---
// #define MQTT_BROKER "homeassistant.local"
// #define MQTT_PORT 1883
// #define MQTT_USER "homeassistant"
// #define MQTT_PASSWORD "your-ha-password"

// --- Home Assistant Cloud (Nabu Casa) ---
// #define MQTT_BROKER "YOUR_CLOUD_SUBDOMAIN.nabucasa.com"
// #define MQTT_PORT 8883              // TLS port (not supported yet)
// #define MQTT_USER "your-email@example.com"
// #define MQTT_PASSWORD "your-cloud-password"

// --- HiveMQ Public Broker (Testing Only!) ---
// #define MQTT_BROKER "broker.hivemq.com"
// #define MQTT_PORT 1883
// #define MQTT_USER ""
// #define MQTT_PASSWORD ""

// --- Azure IoT Hub ---
// NOTE: Azure requires special configuration, not yet supported
// #define MQTT_BROKER "your-hub.azure-devices.net"
// #define MQTT_PORT 8883
// #define MQTT_USER "your-hub.azure-devices.net/esp32-diagnostic/?api-version=2021-04-12"
// #define MQTT_PASSWORD "shared-access-key"

// ========== Docker Example (Mosquitto) ==========
/*
  To run a local Mosquitto broker:
  
  docker run -it -p 1883:1883 -p 9001:9001 \
    -v mosquitto.conf:/mosquitto/config/mosquitto.conf \
    eclipse-mosquitto
  
  mosquitto.conf:
  -------
  listener 1883
  protocol mqtt
  
  listener 9001
  protocol websockets
  -------
  
  Then set:
  #define MQTT_BROKER "localhost"     (or container IP)
  #define MQTT_PORT 1883
  #define MQTT_USER ""
  #define MQTT_PASSWORD ""
*/

// ========== Testing MQTT Connection ==========

/*
  To test if MQTT is working:
  
  1. Subscribe to all topics:
     mosquitto_sub -h mqtt.example.com -u username -P password -t "esp32-diagnostic/#" -v
  
  2. From ESP32 web interface:
     - Navigate to /api/mqtt-status
     - Should show "connected" status
     - Click /api/mqtt-publish-test to send a test message
  
  3. You should see test message in subscriber:
     esp32-diagnostic/test Test message from esp32-diagnostic at 12345
*/

// ========== Publishing Interval ==========

// Metrics are published every 30 seconds (defined in mqtt_handler.h)
// To modify, edit MQTTHandler::publishInterval in include/mqtt_handler.h
// const unsigned long publishInterval = 30000; // milliseconds
