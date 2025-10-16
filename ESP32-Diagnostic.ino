/*
 * ============================================================
 * ESP32 Diagnostic System - Main File
 * ============================================================
 * 
 * Version: 3.0.1
 * Date: October 2025
 * Author: ESP32 Diagnostic Team
 *
 * Description:
 *   Complete diagnostic system for ESP32 with modern web interface,
 *   real-time updates, multilingual support (FR/EN), and hardware testing.
 *
 * Requirements:
 *   - Arduino Core ESP32 v3.3.2 or higher
 *   - Adafruit NeoPixel library
 *   - ArduinoJson v6.x
 *
 * Features:
 *   - Dynamic web interface with glassmorphism design
 *   - Real-time data updates (5s interval)
 *   - Multilingual interface (French/English)
 *   - Hardware testing (GPIO, I2C, SPI, Memory, WiFi)
 *   - JSON/CSV export capabilities
 *   - NeoPixel control
 *
 * Hardware Support:
 *   - ESP32-S2, ESP32-S3, ESP32-C3
 *   - Tested on ESP32-S3 with 8MB Flash, OPI PSRAM
 *
 * ============================================================
 */

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <esp_heap_caps.h>
#include <esp_system.h>
#include <esp_chip_info.h>

// ============================================================
// VERSION & CONFIGURATION
// ============================================================
#define DIAGNOSTIC_VERSION "3.0.1"
#define ARDUINO_CORE_VERSION "3.3.2"

// NeoPixel Configuration
#define NEOPIXEL_PIN 48
#define NEOPIXEL_COUNT 1

// WiFi Configuration
const char* ssid = "VotreSSID";
const char* password = "VotreMotDePasse";

// mDNS Configuration
const char* mdnsName = "esp32-diag";

// ============================================================
// GLOBAL OBJECTS
// ============================================================
WebServer server(80);
Adafruit_NeoPixel* strip = nullptr;
bool neopixelAvailable = false;

// System state
String currentLanguage = "fr";
unsigned long lastUpdateTime = 0;
const unsigned long UPDATE_INTERVAL = 5000; // 5 seconds

// ============================================================
// INCLUDE MODULES
// ============================================================
#include "translations.h"
#include "test_functions.h"
#include "api_handlers.h"
#include "web_interface.h"

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

// ============================================================
// MEMORY MANAGEMENT - Optimized for Core 3.3.2
// ============================================================

String formatBytes(size_t bytes) {
  if (bytes < 1024) return String(bytes) + " B";
  else if (bytes < (1024 * 1024)) return String(bytes / 1024.0, 2) + " KB";
  else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0, 2) + " MB";
  else return String(bytes / 1024.0 / 1024.0 / 1024.0, 2) + " GB";
}

// Optimized memory info using Core 3.3.2 heap capabilities
void getMemoryInfo(uint32_t &heapFree, uint32_t &heapSize, uint32_t &psramFree, uint32_t &psramSize) {
  // Use esp_heap_caps for more accurate memory reporting (Core 3.3.2)
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

// ============================================================
// UPTIME CALCULATION
// ============================================================

String getUptime() {
  unsigned long seconds = millis() / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  
  seconds %= 60;
  minutes %= 60;
  hours %= 24;
  
  String uptime = "";
  if (days > 0) uptime += String(days) + "d ";
  if (hours > 0 || days > 0) uptime += String(hours) + "h ";
  uptime += String(minutes) + "m " + String(seconds) + "s";
  
  return uptime;
}

// ============================================================
// NEOPIXEL INITIALIZATION
// ============================================================

void initNeoPixel() {
  Serial.println("[NEOPIXEL] Initializing...");
  
  strip = new Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
  
  if (strip != nullptr) {
    strip->begin();
    strip->setBrightness(50);
    strip->show();
    neopixelAvailable = true;
    Serial.println("[NEOPIXEL] Initialized successfully");

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
// WIFI CONNECTION - Optimized for Core 3.3.2
// ============================================================

void connectWiFi() {
  Serial.println("\n[WIFI] Connecting to WiFi...");
  Serial.print("[WIFI] SSID: ");
  Serial.println(ssid);
  
  // Configure WiFi for better stability (Core 3.3.2 improvements)
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(false); // Don't save to flash
  
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WIFI] Connected!");
    Serial.print("[WIFI] IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("[WIFI] Signal Strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\n[WIFI] Connection failed!");
    Serial.println("[WIFI] Starting in AP mode...");
    WiFi.softAP("ESP32-Diagnostic", "12345678");
    Serial.print("[WIFI] AP IP: ");
    Serial.println(WiFi.softAPIP());
  }
}

// ============================================================
// WEB SERVER ROUTES
// ============================================================

void setupRoutes() {
  // Main page
  server.on("/", HTTP_GET, handleRoot);
  
  // API endpoints
  server.on("/api/system", HTTP_GET, handleSystemInfo);
  server.on("/api/tests", HTTP_GET, handleTests);
  server.on("/api/language", HTTP_GET, handleGetLanguage);
  server.on("/api/language", HTTP_POST, handleSetLanguage);
  server.on("/api/export/json", HTTP_GET, handleExportJSON);
  server.on("/api/export/csv", HTTP_GET, handleExportCSV);
  
  // NeoPixel control
  server.on("/api/neopixel/pattern", HTTP_POST, handleNeoPixelPattern);
  server.on("/api/neopixel/color", HTTP_POST, handleNeoPixelColor);
  
  // 404 handler
  server.onNotFound([]() {
    server.send(404, "text/plain", "404: Not Found");
  });
}

// ============================================================
// SETUP
// ============================================================

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n====================================");
  Serial.println("ESP32 Diagnostic System v" + String(DIAGNOSTIC_VERSION));
  Serial.println("Arduino Core: " + String(ARDUINO_CORE_VERSION));
  Serial.println("====================================\n");
  
  // System info
  Serial.println("[INFO] Chip: " + getChipModel());
  Serial.println("[INFO] CPU Cores: " + String(getCPUCores()));
  Serial.println("[INFO] CPU Frequency: " + String(getCPUFrequency()) + " MHz");
  Serial.println("[INFO] Flash Size: " + getFlashSize());
  Serial.println("[INFO] PSRAM: " + getPSRAMSize());
  
  // Initialize I2C
  Serial.println("\n[INIT] Initializing I2C...");
  Wire.begin();
  
  // Initialize NeoPixel
  initNeoPixel();
  
  // Connect WiFi
  connectWiFi();
  
  // Setup mDNS
  if (MDNS.begin(mdnsName)) {
    Serial.println("[MDNS] Started at: http://" + String(mdnsName) + ".local");
    MDNS.addService("http", "tcp", 80);
  } else {
    Serial.println("[MDNS] Failed to start");
  }
  
  // Setup web server routes
  setupRoutes();
  
  // Start server
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
  
  // Handle mDNS
  MDNS.update();

  // Optional: Add periodic tasks here
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
    lastUpdateTime = currentTime;
    
    // You can add periodic tasks here
    // Example: monitor memory, check sensors, etc.
  }
  
  // Small delay to prevent watchdog issues
  delay(1);
}
