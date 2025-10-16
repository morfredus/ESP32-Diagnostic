/*
 * ============================================================
 * API HANDLERS - ESP32 Diagnostic System
 * ============================================================
 *
 * Version: 3.0.1
 * Date: October 2025
 * Arduino Core: 3.3.2+
 *
 * Description:
 *   REST API endpoints for the diagnostic system.
 *   Optimized for Arduino Core ESP32 3.3.2 with improved
 *   memory management and WiFi stability.
 *
 * Endpoints:
 *   GET  /api/system         - System information
 *   GET  /api/tests          - All hardware tests
 *   GET  /api/language       - Current language
 *   POST /api/language       - Set language
 *   GET  /api/export/json    - Export JSON
 *   GET  /api/export/csv     - Export CSV
 *   POST /api/neopixel/pattern - NeoPixel pattern
 *   POST /api/neopixel/color   - NeoPixel color
 *
 * ============================================================
 */

#ifndef API_HANDLERS_H
#define API_HANDLERS_H

#include <ArduinoJson.h>

// ============================================================
// SYSTEM INFO API - Optimized for Core 3.3.2
// ============================================================

void handleSystemInfo() {
  // Use larger buffer for Core 3.3.2 (improved JSON handling)
  StaticJsonDocument<2048> doc;

  // System information
  doc["version"] = DIAGNOSTIC_VERSION;
  doc["core_version"] = ARDUINO_CORE_VERSION;
  doc["chip"] = getChipModel();
  doc["cores"] = getCPUCores();
  doc["frequency"] = getCPUFrequency();
  doc["flash_size"] = getFlashSize();
  doc["psram_size"] = getPSRAMSize();
  doc["uptime"] = getUptime();

  // Memory info - Using improved heap caps API
  uint32_t heapFree, heapSize, psramFree, psramSize;
  getMemoryInfo(heapFree, heapSize, psramFree, psramSize);

  JsonObject memory = doc.createNestedObject("memory");
  memory["heap_free"] = heapFree;
  memory["heap_size"] = heapSize;
  memory["heap_free_formatted"] = formatBytes(heapFree);
  memory["heap_size_formatted"] = formatBytes(heapSize);

  if (psramFound()) {
    memory["psram_free"] = psramFree;
    memory["psram_size"] = psramSize;
    memory["psram_free_formatted"] = formatBytes(psramFree);
    memory["psram_size_formatted"] = formatBytes(psramSize);
  }

  // WiFi info - Core 3.3.2 improved WiFi stability
  JsonObject wifi = doc.createNestedObject("wifi");
  wifi["connected"] = (WiFi.status() == WL_CONNECTED);
  if (WiFi.status() == WL_CONNECTED) {
    wifi["ssid"] = WiFi.SSID();
    wifi["ip"] = WiFi.localIP().toString();
    wifi["rssi"] = WiFi.RSSI();
    wifi["mac"] = WiFi.macAddress();
  }

  // NeoPixel status
  doc["neopixel_available"] = neopixelAvailable;

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

// ============================================================
// TESTS API - Complete hardware testing
// ============================================================

void handleTests() {
  // Large buffer for complete test results
  DynamicJsonDocument doc(4096);

  // GPIO Test
  JsonArray gpioArray = doc.createNestedArray("gpio_test");
  testGPIO(gpioArray);

  // I2C Test
  JsonArray i2cArray = doc.createNestedArray("i2c_test");
  testI2C(i2cArray);

  // SPI Test
  JsonArray spiArray = doc.createNestedArray("spi_test");
  testSPI(spiArray);

  // Memory Test
  JsonObject memTest = doc.createNestedObject("memory_test");
  testMemory(memTest);

  // WiFi Test
  JsonObject wifiTest = doc.createNestedObject("wifi_test");
  testWiFi(wifiTest);

  // System Test
  JsonObject sysTest = doc.createNestedObject("system_test");
  testSystem(sysTest);

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

// ============================================================
// LANGUAGE API
// ============================================================

void handleGetLanguage() {
  StaticJsonDocument<128> doc;
  doc["language"] = currentLanguage;

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

void handleSetLanguage() {
  if (server.hasArg("plain")) {
    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, server.arg("plain"));

    if (!error) {
      String lang = doc["language"];
      if (lang == "fr" || lang == "en") {
        currentLanguage = lang;

        StaticJsonDocument<128> response;
        response["success"] = true;
        response["language"] = currentLanguage;

        String responseStr;
        serializeJson(response, responseStr);

        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "application/json", responseStr);
        return;
      }
    }
  }

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(400, "application/json", "{\"success\":false,\"error\":\"Invalid language\"}");
}

// ============================================================
// EXPORT JSON - Complete system data
// ============================================================

void handleExportJSON() {
  // Large buffer for complete export
  DynamicJsonDocument doc(8192);

  // Basic info
  doc["export_date"] = getUptime();
  doc["version"] = DIAGNOSTIC_VERSION;
  doc["core_version"] = ARDUINO_CORE_VERSION;

  // System info
  JsonObject system = doc.createNestedObject("system");
  system["chip"] = getChipModel();
  system["cores"] = getCPUCores();
  system["frequency"] = getCPUFrequency();
  system["flash_size"] = getFlashSize();
  system["psram_size"] = getPSRAMSize();
  system["uptime"] = getUptime();

  // Memory info
  uint32_t heapFree, heapSize, psramFree, psramSize;
  getMemoryInfo(heapFree, heapSize, psramFree, psramSize);

  JsonObject memory = doc.createNestedObject("memory");
  memory["heap_free"] = heapFree;
  memory["heap_size"] = heapSize;
  if (psramFound()) {
    memory["psram_free"] = psramFree;
    memory["psram_size"] = psramSize;
  }

  // WiFi info
  JsonObject wifi = doc.createNestedObject("wifi");
  wifi["connected"] = (WiFi.status() == WL_CONNECTED);
  if (WiFi.status() == WL_CONNECTED) {
    wifi["ssid"] = WiFi.SSID();
    wifi["ip"] = WiFi.localIP().toString();
    wifi["rssi"] = WiFi.RSSI();
  }

  // Tests
  JsonArray gpioArray = doc.createNestedArray("gpio_test");
  testGPIO(gpioArray);

  JsonArray i2cArray = doc.createNestedArray("i2c_test");
  testI2C(i2cArray);

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Disposition", "attachment; filename=esp32-diagnostic.json");
  server.send(200, "application/json", response);
}

// ============================================================
// EXPORT CSV - Formatted data export
// ============================================================

void handleExportCSV() {
  String csv = "Category,Parameter,Value\n";

  // System information
  csv += "System,Version," + String(DIAGNOSTIC_VERSION) + "\n";
  csv += "System,Arduino Core," + String(ARDUINO_CORE_VERSION) + "\n";
  csv += "System,Chip," + getChipModel() + "\n";
  csv += "System,CPU Cores," + String(getCPUCores()) + "\n";
  csv += "System,CPU Frequency," + String(getCPUFrequency()) + " MHz\n";
  csv += "System,Flash Size," + getFlashSize() + "\n";
  csv += "System,PSRAM," + getPSRAMSize() + "\n";
  csv += "System,Uptime," + getUptime() + "\n";

  // Memory information
  uint32_t heapFree, heapSize, psramFree, psramSize;
  getMemoryInfo(heapFree, heapSize, psramFree, psramSize);

  csv += "Memory,Heap Free," + formatBytes(heapFree) + "\n";
  csv += "Memory,Heap Size," + formatBytes(heapSize) + "\n";

  if (psramFound()) {
    csv += "Memory,PSRAM Free," + formatBytes(psramFree) + "\n";
    csv += "Memory,PSRAM Size," + formatBytes(psramSize) + "\n";
  }

  // WiFi information
  if (WiFi.status() == WL_CONNECTED) {
    csv += "WiFi,Status,Connected\n";
    csv += "WiFi,SSID," + WiFi.SSID() + "\n";
    csv += "WiFi,IP," + WiFi.localIP().toString() + "\n";
    csv += "WiFi,RSSI," + String(WiFi.RSSI()) + " dBm\n";
  } else {
    csv += "WiFi,Status,Disconnected\n";
  }

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Disposition", "attachment; filename=esp32-diagnostic.csv");
  server.send(200, "text/csv", csv);
}

// ============================================================
// NEOPIXEL PATTERN API - Optimized response timing
// ============================================================

void handleNeoPixelPattern() {
  if (!neopixelAvailable || strip == nullptr) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", "{\"success\":false,\"message\":\"NeoPixel not available\"}");
    return;
  }

  if (!server.hasArg("plain")) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "application/json", "{\"success\":false,\"error\":\"No data\"}");
    return;
  }

  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, server.arg("plain"));

  if (error) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "application/json", "{\"success\":false,\"error\":\"Invalid JSON\"}");
    return;
  }

  String pattern = doc["pattern"];

  // CRITICAL: Send response BEFORE executing pattern
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", "{\"success\":true,\"pattern\":\"" + pattern + "\"}");

  // Now execute the pattern
  if (pattern == "rainbow") {
    for(int i=0; i<256; i++) {
      strip->setPixelColor(0, strip->ColorHSV(i * 256));
      strip->show();
      delay(10);
    }
  } else if (pattern == "pulse") {
    for(int i=0; i<255; i+=5) {
      strip->setPixelColor(0, strip->Color(i, 0, i));
      strip->show();
      delay(20);
    }
    for(int i=255; i>=0; i-=5) {
      strip->setPixelColor(0, strip->Color(i, 0, i));
      strip->show();
      delay(20);
    }
  } else if (pattern == "strobe") {
    for(int i=0; i<10; i++) {
      strip->setPixelColor(0, strip->Color(255, 255, 255));
      strip->show();
      delay(50);
      strip->setPixelColor(0, strip->Color(0, 0, 0));
      strip->show();
      delay(50);
    }
  }

  // Turn off after pattern
  strip->setPixelColor(0, strip->Color(0, 0, 0));
  strip->show();
}

// ============================================================
// NEOPIXEL COLOR API
// ============================================================

void handleNeoPixelColor() {
  if (!neopixelAvailable || strip == nullptr) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", "{\"success\":false,\"message\":\"NeoPixel not available\"}");
    return;
  }

  if (!server.hasArg("plain")) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "application/json", "{\"success\":false,\"error\":\"No data\"}");
    return;
  }

  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, server.arg("plain"));

  if (error) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "application/json", "{\"success\":false,\"error\":\"Invalid JSON\"}");
    return;
  }

  int r = doc["r"] | 0;
  int g = doc["g"] | 0;
  int b = doc["b"] | 0;

  // Constrain values
  r = constrain(r, 0, 255);
  g = constrain(g, 0, 255);
  b = constrain(b, 0, 255);

  // Set color
  strip->setPixelColor(0, strip->Color(r, g, b));
  strip->show();

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", "{\"success\":true}");
}

#endif // API_HANDLERS_H
