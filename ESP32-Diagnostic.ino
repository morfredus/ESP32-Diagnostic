/*
 * ============================================================
 * ESP32 Diagnostic System - Main File
 * ============================================================
 * 
 * Version: 3.1.0
 * Date: October 2025
 * Author: ESP32 Diagnostic Team
 *
 * Description:
 *   Complete diagnostic system for ESP32 with modern web interface,
 *   real-time updates, multilingual support (FR/EN), Multi-WiFi, and
 *   comprehensive hardware testing including LEDs, Screens, ADC, etc.
 *
 * Requirements:
 *   - Arduino Core ESP32 v3.3.2 or higher
 *   - Adafruit NeoPixel library
 *   - ArduinoJson v6.x
 *
 * New in v3.1.0:
 *   - LED control (Built-in LED + NeoPixel)
 *   - Screen testing (TFT SPI + OLED I2C)
 *   - Advanced tests (ADC, Touch, PWM, SPI scan)
 *   - GPIO comprehensive testing
 *   - WiFi scanner
 *   - Performance benchmarks
 *   - Extended export formats (TXT, JSON, CSV, PDF preview)
 *
 * ============================================================
 */

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <esp_heap_caps.h>
#include <esp_system.h>
#include <esp_chip_info.h>
#include <driver/adc.h>

// ============================================================
// VERSION & CONFIGURATION
// ============================================================
#define DIAGNOSTIC_VERSION "3.1.0"
#define ARDUINO_CORE_VERSION "3.3.2"

// Hardware Configuration
#define BUILTIN_LED_PIN 97
#define NEOPIXEL_PIN 48
#define NEOPIXEL_COUNT 1

// Screen Configuration (optional)
#define TFT_CS 14
#define TFT_DC 47
#define TFT_RST 21
#define OLED_SDA 21
#define OLED_SCL 20

// Include WiFi configuration
#include "config.h"

// mDNS Configuration
const char* mdnsName = "esp32-diagnostic";

// ============================================================
// GLOBAL OBJECTS
// ============================================================
WebServer server(80);
WiFiMulti wifiMulti;
Adafruit_NeoPixel* strip = nullptr;
bool neopixelAvailable = false;
bool builtinLedAvailable = true;

// System state
String currentLanguage = "fr";
unsigned long lastUpdateTime = 0;
const unsigned long UPDATE_INTERVAL = 5000;

// ============================================================
// SYSTEM INFORMATION FUNCTIONS
// ============================================================

String getChipModel() {
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  
  String model = "ESP32";
  #ifdef CONFIG_IDF_TARGET_ESP32S2
    model = "ESP32-S2";
  #elif CONFIG_IDF_TARGET_ESP32S3
    model = "ESP32-S3";
  #elif CONFIG_IDF_TARGET_ESP32C3
    model = "ESP32-C3";
  #elif CONFIG_IDF_TARGET_ESP32C6
    model = "ESP32-C6";
  #elif CONFIG_IDF_TARGET_ESP32H2
    model = "ESP32-H2";
  #endif
  
  return model;
}

int getCPUCores() {
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  return chip_info.cores;
}

int getCPUFrequency() {
  return getCpuFrequencyMhz();
}

String getFlashSize() {
  uint32_t flash_size = ESP.getFlashChipSize();
  if (flash_size >= 1024 * 1024) {
    return String(flash_size / (1024 * 1024)) + " MB";
  }
  return String(flash_size / 1024) + " KB";
}

String getPSRAMSize() {
  if (psramFound()) {
    uint32_t psram_size = ESP.getPsramSize();
    if (psram_size >= 1024 * 1024) {
      return String(psram_size / (1024 * 1024)) + " MB";
    }
    return String(psram_size / 1024) + " KB";
  }
  return "N/A";
}

String formatBytes(size_t bytes) {
  if (bytes < 1024) return String(bytes) + " B";
  else if (bytes < (1024 * 1024)) return String(bytes / 1024.0, 2) + " KB";
  else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0, 2) + " MB";
  else return String(bytes / 1024.0 / 1024.0 / 1024.0, 2) + " GB";
}

void getMemoryInfo(uint32_t &heapFree, uint32_t &heapSize, uint32_t &psramFree, uint32_t &psramSize) {
  heapFree = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
  heapSize = heap_caps_get_total_size(MALLOC_CAP_INTERNAL);
  
  if (psramFound()) {
    psramFree = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    psramSize = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
  } else {
    psramFree = 0;
    psramSize = 0;
  }
}

String getUptime() {
  unsigned long seconds = millis() / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  
  seconds %= 60;
  minutes %= 60;
  hours %= 24;
  
  String uptime = "";
  if (days > 0) uptime += String(days) + "j ";
  if (hours > 0 || days > 0) uptime += String(hours) + "h ";
  uptime += String(minutes) + "m " + String(seconds) + "s";
  
  return uptime;
}

// ============================================================
// INCLUDE MODULES
// ============================================================
#include "translations.h"
#include "test_functions.h"
#include "api_handlers.h"
#include "web_interface.h"

// ============================================================
// HARDWARE INITIALIZATION
// ============================================================

void initBuiltinLED() {
  Serial.println("[LED] Initializing built-in LED...");
  pinMode(BUILTIN_LED_PIN, OUTPUT);
  digitalWrite(BUILTIN_LED_PIN, LOW);
  builtinLedAvailable = true;
  Serial.println("[LED] Built-in LED initialized on GPIO " + String(BUILTIN_LED_PIN));
}

void initNeoPixel() {
  Serial.println("[NEOPIXEL] Initializing...");
  
  strip = new Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
  
  if (strip != nullptr) {
    strip->begin();
    strip->setBrightness(50);
    strip->show();
    neopixelAvailable = true;
    Serial.println("[NEOPIXEL] Initialized successfully on GPIO " + String(NEOPIXEL_PIN));

    // Quick test pattern
    strip->setPixelColor(0, strip->Color(0, 255, 0));
    strip->show();
    delay(200);
    strip->setPixelColor(0, strip->Color(0, 0, 0));
    strip->show();
  } else {
    neopixelAvailable = false;
    Serial.println("[NEOPIXEL] Failed to initialize");
  }
}

// ============================================================
// WIFI CONNECTION
// ============================================================

void connectWiFi() {
  Serial.println("\n[WIFI] ========== WiFi Connection ==========");
  
  #ifdef MULTI_WIFI_ENABLED
    Serial.println("[WIFI] Mode: Multi-WiFi");
    Serial.print("[WIFI] Networks configured: ");
    Serial.println(NUM_SSIDS);

    for(int i = 0; i < NUM_SSIDS; i++) {
      Serial.print("[WIFI] Adding network ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(ssid_list[i]);
      wifiMulti.addAP(ssid_list[i], password_list[i]);
    }

    Serial.print("[WIFI] Connecting");
    int attempts = 0;
    while (wifiMulti.run() != WL_CONNECTED && attempts < 40) {
      delay(500);
      Serial.print(".");
      attempts++;
    }
    Serial.println();

  #else
    Serial.println("[WIFI] Mode: Single WiFi");
    Serial.print("[WIFI] SSID: ");
    Serial.println(ssid);

    WiFi.disconnect(true);
    delay(1000);

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(false);

    WiFi.begin(ssid, password);

    int attempts = 0;
    Serial.print("[WIFI] Connecting");
    while (WiFi.status() != WL_CONNECTED && attempts < 40) {
      delay(500);
      Serial.print(".");
      attempts++;
    }
    Serial.println();
  #endif
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("[WIFI] ========== Connection SUCCESS ==========");
    Serial.print("[WIFI] Connected to: ");
    Serial.println(WiFi.SSID());
    Serial.print("[WIFI] IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("[WIFI] Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("[WIFI] Signal (RSSI): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    Serial.println("[WIFI] ==========================================");
  } else {
    Serial.println("[WIFI] ========== Connection FAILED ==========");
    Serial.println("[WIFI] Switching to AP mode...");

    WiFi.disconnect(true);
    delay(100);
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP32-Diagnostic", "12345678");
    delay(100);

    Serial.println("[WIFI] ========== AP MODE STARTED ==========");
    Serial.println("[WIFI] AP SSID: ESP32-Diagnostic");
    Serial.println("[WIFI] AP Password: 12345678");
    Serial.print("[WIFI] AP IP: ");
    Serial.println(WiFi.softAPIP());
    Serial.println("[WIFI] ==========================================");
  }
}

// ============================================================
// WEB SERVER ROUTES
// ============================================================

void setupRoutes() {
  server.on("/", HTTP_GET, handleRoot);
  
  // API endpoints - System
  server.on("/api/system", HTTP_GET, handleSystemInfo);
  server.on("/api/tests", HTTP_GET, handleTests);
  server.on("/api/language", HTTP_GET, handleGetLanguage);
  server.on("/api/language", HTTP_POST, handleSetLanguage);
  
  // API endpoints - LEDs (NEW v3.1.0)
  server.on("/api/led/builtin", HTTP_POST, handleBuiltinLED);
  server.on("/api/neopixel/pattern", HTTP_POST, handleNeoPixelPattern);
  server.on("/api/neopixel/color", HTTP_POST, handleNeoPixelColor);
  
  // API endpoints - Advanced Tests (NEW v3.1.0)
  server.on("/api/test/gpio", HTTP_GET, handleTestGPIO);
  server.on("/api/test/adc", HTTP_GET, handleTestADC);
  server.on("/api/test/touch", HTTP_GET, handleTestTouch);
  server.on("/api/test/pwm", HTTP_GET, handleTestPWM);
  server.on("/api/scan/wifi", HTTP_GET, handleWiFiScan);
  server.on("/api/benchmark", HTTP_GET, handleBenchmark);
  
  // API endpoints - Export
  server.on("/api/export/json", HTTP_GET, handleExportJSON);
  server.on("/api/export/csv", HTTP_GET, handleExportCSV);
  server.on("/api/export/txt", HTTP_GET, handleExportTXT);
  
  server.onNotFound([]() {
    server.send(404, "text/plain", "404: Not Found");
  });
}

// ============================================================
// SETUP
// ============================================================

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n====================================");
  Serial.println("ESP32 Diagnostic System v" + String(DIAGNOSTIC_VERSION));
  Serial.println("Arduino Core: " + String(ARDUINO_CORE_VERSION));
  Serial.println("====================================\n");
  
  Serial.println("[INFO] Chip: " + getChipModel());
  Serial.println("[INFO] CPU Cores: " + String(getCPUCores()));
  Serial.println("[INFO] CPU Frequency: " + String(getCPUFrequency()) + " MHz");
  Serial.println("[INFO] Flash Size: " + getFlashSize());
  Serial.println("[INFO] PSRAM: " + getPSRAMSize());
  
  Serial.println("\n[INIT] Initializing I2C...");
  Wire.begin();
  
  initBuiltinLED();
  initNeoPixel();
  
  connectWiFi();
  
  if (MDNS.begin(mdnsName)) {
    Serial.println("[MDNS] Started at: http://" + String(mdnsName) + ".local");
    MDNS.addService("http", "tcp", 80);
  } else {
    Serial.println("[MDNS] Failed to start");
  }
  
  setupRoutes();
  
  server.begin();
  Serial.println("[WEB] Server started");
  Serial.println("\n[READY] System ready!");
  Serial.println("====================================\n");
}

// ============================================================
// MAIN LOOP
// ============================================================

void loop() {
  server.handleClient();
  
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
    lastUpdateTime = currentTime;
  }
  
  delay(1);
}