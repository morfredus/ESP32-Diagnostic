/*
 * MQTT_HANDLER.H - MQTT Publisher for ESP32 Diagnostic
 * Lightweight MQTT client for metric publishing
 * Compatible with Home Assistant, NodeRED, InfluxDB, etc.
 */

#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include "config.h"
#include <PubSubClient.h>
#include <WiFiClient.h>

// ========== MQTT State Machine ==========
enum class MQTTState {
  DISCONNECTED = 0,
  CONNECTING = 1,
  CONNECTED = 2,
  OFF = 3
};

class MQTTHandler {
private:
  WiFiClient wifiClient;
  PubSubClient client;
  
  char broker[128];
  uint16_t port;
  char username[64];
  char password[64];
  char topicPrefix[128];
  
  MQTTState state;
  unsigned long lastReconnectAttempt;
  unsigned long lastPublishTime;
  const unsigned long publishInterval = 30000; // Publish every 30s
  bool enabled;
  
public:
  MQTTHandler() : client(wifiClient), state(MQTTState::DISCONNECTED), 
                  lastReconnectAttempt(0), lastPublishTime(0), enabled(false) {
    strcpy(broker, MQTT_BROKER);
    port = MQTT_PORT;
    strcpy(username, MQTT_USER);
    strcpy(password, MQTT_PASSWORD);
    strcpy(topicPrefix, MQTT_TOPIC_PREFIX);
  }
  
  // ========== Configuration ==========
  void configure(const char* _broker, uint16_t _port, 
                 const char* _user, const char* _pass,
                 const char* _prefix) {
    strlcpy(broker, _broker, sizeof(broker));
    port = _port;
    strlcpy(username, _user, sizeof(username));
    strlcpy(password, _pass, sizeof(password));
    strlcpy(topicPrefix, _prefix, sizeof(topicPrefix));
  }
  
  void enable(bool en) {
    enabled = en;
    if (!en) {
      disconnect();
    }
  }
  
  bool isEnabled() const { return enabled; }
  MQTTState getState() const { return state; }
  
  const char* getStateString() const {
    switch (state) {
      case MQTTState::DISCONNECTED: return "Disconnected";
      case MQTTState::CONNECTING: return "Connecting";
      case MQTTState::CONNECTED: return "Connected";
      case MQTTState::OFF: return "Disabled";
      default: return "Unknown";
    }
  }
  
  // ========== Connection Management ==========
  bool connect() {
    if (!enabled) {
      state = MQTTState::OFF;
      return false;
    }
    
    if (!WiFi.isConnected()) {
      state = MQTTState::DISCONNECTED;
      return false;
    }
    
    if (state == MQTTState::CONNECTED && client.connected()) {
      return true;
    }
    
    unsigned long now = millis();
    if (now - lastReconnectAttempt < 5000) {
      return false; // Backoff 5s between attempts
    }
    
    lastReconnectAttempt = now;
    state = MQTTState::CONNECTING;
    
    if (client.connect(DIAGNOSTIC_HOSTNAME, username, password)) {
      state = MQTTState::CONNECTED;
      Serial.printf("[MQTT] Connected to %s:%u\r\n", broker, port);
      publishStartupMessage();
      return true;
    } else {
      state = MQTTState::DISCONNECTED;
      Serial.printf("[MQTT] Connection failed (code: %d)\r\n", client.state());
      return false;
    }
  }
  
  void disconnect() {
    if (client.connected()) {
      client.disconnect();
    }
    state = MQTTState::DISCONNECTED;
  }
  
  // ========== Main Loop ==========
  void maintain() {
    if (!enabled) {
      state = MQTTState::OFF;
      return;
    }
    
    if (!WiFi.isConnected()) {
      if (state != MQTTState::DISCONNECTED) {
        disconnect();
      }
      return;
    }
    
    if (!client.connected()) {
      connect();
      return;
    }
    
    client.loop();
    
    // Publish metrics periodically
    unsigned long now = millis();
    if (now - lastPublishTime >= publishInterval) {
      lastPublishTime = now;
      publishMetrics();
    }
  }
  
  // ========== Publishing ==========
  void publish(const char* topic, const char* payload, bool retain = false) {
    if (state != MQTTState::CONNECTED || !client.connected()) {
      return;
    }
    
    char fullTopic[256];
    snprintf(fullTopic, sizeof(fullTopic), "%s/%s", topicPrefix, topic);
    
    if (client.publish(fullTopic, payload, retain)) {
      if (DIAGNOSTIC_VERBOSE) {
        Serial.printf("[MQTT] Published %s = %s\r\n", fullTopic, payload);
      }
    } else {
      Serial.printf("[MQTT] Publish failed: %s\r\n", fullTopic);
    }
  }
  
  void publishStartupMessage() {
    char payload[256];
    snprintf(payload, sizeof(payload), 
             "{\"hostname\":\"%s\",\"version\":\"%s\",\"ip\":\"%s\",\"uptime\":0}",
             DIAGNOSTIC_HOSTNAME, PROJECT_VERSION, WiFi.localIP().toString().c_str());
    publish("status/startup", payload, true);
  }
  
  // Publish all diagnostics metrics
  void publishMetrics() {
    if (state != MQTTState::CONNECTED) return;
    
    // Uptime
    char buf[64];
    snprintf(buf, sizeof(buf), "%lu", millis() / 1000);
    publish("system/uptime_seconds", buf, false);
    
    // Memory
    snprintf(buf, sizeof(buf), "%lu", ESP.getFreeHeap());
    publish("memory/heap_free", buf, false);
    
    snprintf(buf, sizeof(buf), "%lu", ESP.getHeapSize());
    publish("memory/heap_total", buf, false);
    
    float heapUsage = 100.0 - (100.0 * ESP.getFreeHeap() / ESP.getHeapSize());
    snprintf(buf, sizeof(buf), "%.1f", heapUsage);
    publish("memory/heap_percent", buf, false);
    
    // PSRAM
    if (ESP.getPsramSize() > 0) {
      snprintf(buf, sizeof(buf), "%lu", ESP.getFreePsram());
      publish("memory/psram_free", buf, false);
      
      snprintf(buf, sizeof(buf), "%lu", ESP.getPsramSize());
      publish("memory/psram_total", buf, false);
      
      float psramUsage = 100.0 - (100.0 * ESP.getFreePsram() / ESP.getPsramSize());
      snprintf(buf, sizeof(buf), "%.1f", psramUsage);
      publish("memory/psram_percent", buf, false);
    }
    
    // WiFi Signal
    snprintf(buf, sizeof(buf), "%d", WiFi.RSSI());
    publish("network/wifi_rssi", buf, false);
    
    snprintf(buf, sizeof(buf), "%d", WiFi.channel());
    publish("network/wifi_channel", buf, false);
    
  }
  
  // Publish sensor data
  void publishSensorData(const char* sensor, float value, const char* unit = "") {
    if (state != MQTTState::CONNECTED) return;

    char topic[128];
    char payload[96];
    
    snprintf(topic, sizeof(topic), "sensors/%s", sensor);
    snprintf(payload, sizeof(payload), "%.2f %s", value, unit);
    
    publish(topic, payload, false);
  }
  
  // Publish environmental sensors
  void publishEnvironmentalData(float temp, float humidity, float pressure, float altitude) {
    if (state != MQTTState::CONNECTED) return;
    
    char buf[64];
    
    if (temp > -900.0) {
      snprintf(buf, sizeof(buf), "%.2f", temp);
      publish("sensors/env/temperature", buf, false);
    }
    
    if (humidity >= 0.0) {
      snprintf(buf, sizeof(buf), "%.1f", humidity);
      publish("sensors/env/humidity", buf, false);
    }
    
    if (pressure > 0) {
      snprintf(buf, sizeof(buf), "%.1f", pressure);
      publish("sensors/env/pressure_hpa", buf, false);
    }
    
    if (altitude > -900.0) {
      snprintf(buf, sizeof(buf), "%.1f", altitude);
      publish("sensors/env/altitude_m", buf, false);
    }
  }
  
  // Publish GPS data
  void publishGPSData(float lat, float lon, float alt, int satellites, float hdop) {
    if (state != MQTTState::CONNECTED) return;
    
    char buf[64];
    
    snprintf(buf, sizeof(buf), "%.6f", lat);
    publish("sensors/gps/latitude", buf, false);
    
    snprintf(buf, sizeof(buf), "%.6f", lon);
    publish("sensors/gps/longitude", buf, false);
    
    snprintf(buf, sizeof(buf), "%.2f", alt);
    publish("sensors/gps/altitude", buf, false);
    
    snprintf(buf, sizeof(buf), "%d", satellites);
    publish("sensors/gps/satellites", buf, false);
    
    snprintf(buf, sizeof(buf), "%.2f", hdop);
    publish("sensors/gps/hdop", buf, false);
  }
  
  // Publish test results
  void publishTestResult(const char* testName, const char* status, int duration_ms = -1) {
    if (state != MQTTState::CONNECTED) return;
    
    char topic[128];
    char payload[256];
    
    snprintf(topic, sizeof(topic), "tests/%s/status", testName);
    publish(topic, status, false);
    
    if (duration_ms >= 0) {
      snprintf(topic, sizeof(topic), "tests/%s/duration_ms", testName);
      snprintf(payload, sizeof(payload), "%d", duration_ms);
      publish(topic, payload, false);
    }
  }
};

// ========== Global Instance ==========
extern MQTTHandler mqttHandler;

#endif // MQTT_HANDLER_H
