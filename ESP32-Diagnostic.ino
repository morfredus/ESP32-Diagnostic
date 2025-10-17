/*
 * ============================================================
 * ESP32 Diagnostic System v3.2.0
 * ============================================================
 * 
 * Système de diagnostic complet pour ESP32 avec interface web
 * multilingue (FR/EN) et API REST complète.
 * 
 * Compatible: ESP32, ESP32-S2, ESP32-S3, ESP32-C3
 * Arduino Core: 3.x
 * 
 * Nouvelles fonctionnalités v3.2.0:
 * - Configuration WiFi multi-SSID avec WiFiMulti
 * - Configuration des pins via interface web
 * - Fichiers de configuration séparés (sécurisés)
 * 
 * Auteur: ESP32 Diagnostic Team
 * License: MIT
 * ============================================================
 */

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Wire.h>

// ============================================================
// CHARGEMENT DES FICHIERS DE CONFIGURATION
// ============================================================

#ifdef __has_include
  #if __has_include("wifi-config.h")
    #include "wifi-config.h"
    #define HAS_WIFI_CONFIG
  #endif
  #if __has_include("config.h")
    #include "config.h"
    #define HAS_CONFIG
  #endif
#endif

// Configuration par défaut si fichiers manquants
#ifndef HAS_WIFI_CONFIG
  #warning "wifi-config.h not found! Using default WiFi settings"
  struct WiFiCredentials {
    const char* ssid;
    const char* password;
  };
  const WiFiCredentials wifiNetworks[] = {
    {"ESP32-DEFAULT", ""}
  };
  const int WIFI_NETWORKS_COUNT = 1;
  #define AP_SSID "ESP32-Diagnostic-Setup"
  #define AP_PASSWORD "diagnostic123"
  #define MDNS_HOSTNAME "esp32-diagnostic"
  #define WIFI_CONNECT_TIMEOUT 10
#endif

#ifndef HAS_CONFIG
  #warning "config.h not found! Using default settings"
  #define LED_BUILTIN_PIN 2
  #define DEFAULT_LANGUAGE "fr"
  #define WEB_SERVER_PORT 80
  #define DEBUG_MODE true
  #define SERIAL_BAUD_RATE 115200
#endif

// Configuration NeoPixel (optionnel)
#ifdef USE_NEOPIXEL
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
bool neopixelAvailable = true;
#else
bool neopixelAvailable = false;
#endif

// ============================================================
// CONSTANTES
// ============================================================

#define DIAGNOSTIC_VERSION "3.2.0"
#define LED_BUILTIN LED_BUILTIN_PIN  // Compatibilité

// ============================================================
// VARIABLES GLOBALES
// ============================================================

WiFiMulti wifiMulti;
WebServer server(WEB_SERVER_PORT);
String currentLanguage = DEFAULT_LANGUAGE;

// ============================================================
// FONCTIONS UTILITAIRES
// ============================================================

// Formater les octets en unités lisibles
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + " B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0, 2) + " KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0, 2) + " MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0, 2) + " GB";
  }
}

// Obtenir le modèle de chip
String getChipModel() {
  String model = "ESP32";
  
  #ifdef CONFIG_IDF_TARGET_ESP32
    model = "ESP32";
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
    model = "ESP32-S2";
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    model = "ESP32-S3";
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    model = "ESP32-C3";
  #elif defined(CONFIG_IDF_TARGET_ESP32C6)
    model = "ESP32-C6";
  #elif defined(CONFIG_IDF_TARGET_ESP32H2)
    model = "ESP32-H2";
  #endif
  
  return model;
}

// Obtenir le nombre de cores CPU
int getCPUCores() {
  #ifdef CONFIG_IDF_TARGET_ESP32
    return 2;
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
    return 1;
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    return 2;
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    return 1;
  #elif defined(CONFIG_IDF_TARGET_ESP32C6)
    return 1;
  #else
    return 1;
  #endif
}

// Obtenir la fréquence CPU
int getCPUFrequency() {
  return getCpuFrequencyMhz();
}

// Obtenir la taille de la flash - RETOURNE size_t
size_t getFlashSize() {
  return ESP.getFlashChipSize();
}

// Obtenir la taille de la PSRAM - RETOURNE size_t
size_t getPSRAMSize() {
  if (psramFound()) {
    return ESP.getPsramSize();
  }
  return 0;
}

// Obtenir les informations mémoire
void getMemoryInfo(uint32_t &heapFree, uint32_t &heapSize, uint32_t &psramFree, uint32_t &psramSize) {
  heapFree = ESP.getFreeHeap();
  heapSize = ESP.getHeapSize();
  
  if (psramFound()) {
    psramFree = ESP.getFreePsram();
    psramSize = ESP.getPsramSize();
  } else {
    psramFree = 0;
    psramSize = 0;
  }
}

// Obtenir l'uptime formaté
String getUptime() {
  unsigned long uptimeSeconds = millis() / 1000;
  unsigned long days = uptimeSeconds / 86400;
  uptimeSeconds %= 86400;
  unsigned long hours = uptimeSeconds / 3600;
  uptimeSeconds %= 3600;
  unsigned long minutes = uptimeSeconds / 60;
  unsigned long seconds = uptimeSeconds % 60;
  
  String uptime = "";
  if (days > 0) uptime += String(days) + "d ";
  if (hours > 0 || days > 0) uptime += String(hours) + "h ";
  if (minutes > 0 || hours > 0 || days > 0) uptime += String(minutes) + "m ";
  uptime += String(seconds) + "s";
  
  return uptime;
}

// ============================================================
// FONCTIONS NEOPIXEL
// ============================================================

#ifdef USE_NEOPIXEL
void setNeoPixelColor(uint8_t r, uint8_t g, uint8_t b) {
  if (neopixelAvailable) {
    strip.setPixelColor(0, strip.Color(r, g, b));
    strip.show();
  }
}

void neoPixelRainbow(int cycles) {
  if (!neopixelAvailable) return;
  
  for (int j = 0; j < cycles; j++) {
    for (int i = 0; i < 256; i++) {
      uint32_t color = strip.gamma32(strip.ColorHSV(i * 256));
      strip.setPixelColor(0, color);
      strip.show();
      delay(10);
    }
  }
}
#else
// Versions vides si NeoPixel n'est pas activé
void setNeoPixelColor(uint8_t r, uint8_t g, uint8_t b) {}
void neoPixelRainbow(int cycles) {}
#endif

// ============================================================
// INITIALISATION LED INTÉGRÉE
// ============================================================

void initBuiltinLED() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

// ============================================================
// INCLURE LES FICHIERS SÉPARÉS
// ============================================================

#include "translations.h"      // Traductions FR/EN
#include "api_handlers.h"      // Handlers API REST
#include "web_interface.h"     // Interface HTML/CSS/JS

// ============================================================
// CONFIGURATION DES ROUTES
// ============================================================

void setupRoutes() {
  // Page principale
  server.on("/", HTTP_GET, handleRoot);
  
  // API System
  server.on("/api/system/info", HTTP_GET, handleSystemInfo);
  server.on("/api/system/memory", HTTP_GET, handleMemoryInfo);
  server.on("/api/system/wifi", HTTP_GET, handleWiFiInfo);
  
  // API Tests
  server.on("/api/tests", HTTP_GET, handleTests);
  server.on("/api/tests/gpio", HTTP_GET, handleTestGPIO);
  server.on("/api/tests/pwm", HTTP_GET, handleTestPWM);
  server.on("/api/tests/run", HTTP_POST, handleRunTest);
  
  // API Language
  server.on("/api/language", HTTP_GET, handleGetLanguage);
  server.on("/api/language", HTTP_POST, handleSetLanguage);
  
  // API Export
  server.on("/api/export/json", HTTP_GET, handleExportJSON);
  server.on("/api/export/csv", HTTP_GET, handleExportCSV);
  
  // API NeoPixel
  server.on("/api/neopixel/pattern", HTTP_POST, handleNeoPixelPattern);
  server.on("/api/neopixel/color", HTTP_POST, handleNeoPixelColor);
  
  // API Sensors & WiFi
  server.on("/api/sensors", HTTP_GET, handleSensors);
  server.on("/api/wifi/scan", HTTP_POST, handleWiFiScan);
  
  // API Benchmark
  server.on("/api/benchmark", HTTP_GET, handleBenchmark);
  
  // CORS Preflight
  server.on("/api/*", HTTP_OPTIONS, handleCORS);
  
  // 404 Handler
  server.onNotFound([]() {
    server.send(404, "text/plain", "404: Not Found");
  });
}

// ============================================================
// SETUP
// ============================================================

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000);
  
  Serial.println();
  Serial.println("====================================");
  Serial.println("ESP32 Diagnostic System v" + String(DIAGNOSTIC_VERSION));
  Serial.println("====================================");
  Serial.println();
  
  // Initialiser LED interne
  initBuiltinLED();
  
  // Initialiser NeoPixel
  #ifdef USE_NEOPIXEL
  strip.begin();
  strip.setBrightness(NEOPIXEL_BRIGHTNESS);
  setNeoPixelColor(0, 0, 255);  // Bleu = Initialisation
  strip.show();
  Serial.println("[INIT] NeoPixel initialized");
  #endif
  
  // Afficher informations système
  Serial.println("[INFO] System Information:");
  Serial.println("  - Chip Model: " + getChipModel());
  Serial.println("  - CPU Cores: " + String(getCPUCores()));
  Serial.println("  - CPU Frequency: " + String(getCPUFrequency()) + " MHz");
  Serial.println("  - Flash Size: " + formatBytes(getFlashSize()));
  Serial.println("  - PSRAM Size: " + formatBytes(getPSRAMSize()));
  Serial.println("  - Free Heap: " + formatBytes(ESP.getFreeHeap()));
  Serial.println();
  
  // Connexion WiFi Multi-SSID
  Serial.println("[WIFI] Configuring WiFi Multi-SSID...");
  
  // Ajouter tous les réseaux configurés
  for (int i = 0; i < WIFI_NETWORKS_COUNT; i++) {
    wifiMulti.addAP(wifiNetworks[i].ssid, wifiNetworks[i].password);
    Serial.print("  - Added network: ");
    Serial.println(wifiNetworks[i].ssid);
  }
  
  Serial.println("[WIFI] Connecting to WiFi...");
  
  #ifdef USE_NEOPIXEL
  setNeoPixelColor(0, 0, 255);  // Bleu = Connexion en cours
  #endif
  
  // Tenter la connexion (timeout en secondes * 10 pour 100ms intervals)
  int timeout = WIFI_CONNECT_TIMEOUT * 10;
  int attempts = 0;
  
  while (wifiMulti.run() != WL_CONNECTED && attempts < timeout) {
    delay(100);
    Serial.print(".");
    attempts++;
    
    if (attempts % 10 == 0) {
      Serial.print(" ");
    }
  }
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("[WIFI] WiFi connected successfully!");
    Serial.println("  - SSID: " + WiFi.SSID());
    Serial.println("  - IP Address: " + WiFi.localIP().toString());
    Serial.println("  - Signal Strength: " + String(WiFi.RSSI()) + " dBm");
    Serial.println("  - MAC Address: " + WiFi.macAddress());
    
    // Démarrer mDNS
    if (MDNS.begin(MDNS_HOSTNAME)) {
      Serial.println("[MDNS] mDNS responder started");
      Serial.println("  - Hostname: " + String(MDNS_HOSTNAME) + ".local");
      MDNS.addService("http", "tcp", WEB_SERVER_PORT);
    } else {
      Serial.println("[MDNS] Error starting mDNS");
    }
    
    #ifdef USE_NEOPIXEL
    setNeoPixelColor(0, 255, 0);  // Vert = WiFi connecté
    #endif
  } else {
    Serial.println("[WIFI] WiFi connection failed!");
    Serial.println("[WIFI] Starting Access Point mode...");
    
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    
    Serial.println("  - AP SSID: " + String(AP_SSID));
    Serial.println("  - AP Password: " + String(AP_PASSWORD));
    Serial.println("  - AP IP: " + WiFi.softAPIP().toString());
    
    #ifdef USE_NEOPIXEL
    setNeoPixelColor(255, 165, 0);  // Orange = Mode AP
    #endif
  }
  Serial.println();
  
  // Configurer les routes
  setupRoutes();
  
  // Démarrer le serveur
  server.begin();
  Serial.println("[WEB] Web server started on port " + String(WEB_SERVER_PORT));
  Serial.println();
  
  // Afficher les URLs d'accès
  Serial.println("[READY] System ready!");
  Serial.println("====================================");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Access web interface at:");
    Serial.println("  - http://" + WiFi.localIP().toString());
    Serial.println("  - http://" + String(MDNS_HOSTNAME) + ".local");
  } else {
    Serial.println("Access web interface at:");
    Serial.println("  - http://" + WiFi.softAPIP().toString());
  }
  Serial.println("====================================");
  Serial.println();
  
  #ifdef USE_NEOPIXEL
  delay(1000);
  setNeoPixelColor(0, 0, 0);  // Éteindre après initialisation
  #endif
}

// ============================================================
// LOOP
// ============================================================

void loop() {
  server.handleClient();
  
  // Vérifier la connexion WiFi et reconnecter si nécessaire
  if (WiFi.status() != WL_CONNECTED && millis() > 60000) {
    static unsigned long lastReconnect = 0;
    if (millis() - lastReconnect > 30000) {  // Tenter reconnexion toutes les 30s
      Serial.println("[WIFI] Connection lost, attempting reconnection...");
      wifiMulti.run();
      lastReconnect = millis();
    }
  }
  
  // Gérer mDNS
  #ifdef ESP32
  // mDNS update non nécessaire sur ESP32 (géré automatiquement)
  #endif
  
  delay(2);
}