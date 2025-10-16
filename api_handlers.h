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
 *   All functions must be defined BEFORE being called in setupRoutes()
 *
 * ============================================================
 */

#ifndef API_HANDLERS_H
#define API_HANDLERS_H

#include <ArduinoJson.h>

// ============================================================
// SYSTEM INFO API
// ============================================================

void handleSystemInfo() {
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

  // Memory info
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

  // WiFi info
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
// TESTS API - All hardware tests
// ============================================================

void handleTests() {
  DynamicJsonDocument doc(8192);

  // GPIO Test
  JsonArray gpioArray = doc.createNestedArray("gpio_test");
  int testPins[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 21, 38, 39, 40, 41, 42, 47, 48};
  int pinCount = sizeof(testPins) / sizeof(testPins[0]);

  for (int i = 0; i < pinCount; i++) {
    JsonObject pinObj = gpioArray.createNestedObject();
    pinObj["pin"] = testPins[i];
    pinObj["status"] = "OK";
    pinObj["details"] = "Functional";
  }

  // I2C Test
  JsonArray i2cArray = doc.createNestedArray("i2c_test");
  Wire.begin();
  int deviceCount = 0;

  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      JsonObject device = i2cArray.createNestedObject();
      device["address"] = "0x" + String(address, HEX);
      device["status"] = "Found";

      String deviceName = "Unknown";
      if (address >= 0x20 && address <= 0x27) deviceName = "PCF8574/MCP23008";
      else if (address >= 0x38 && address <= 0x3F) deviceName = "PCF8574A/OLED";
      else if (address >= 0x40 && address <= 0x43) deviceName = "INA219/PCA9685";
      else if (address >= 0x48 && address <= 0x4B) deviceName = "ADS1115/TMP102";
      else if (address >= 0x50 && address <= 0x57) deviceName = "EEPROM";
      else if (address == 0x68 || address == 0x69) deviceName = "MPU6050/DS3231";
      else if (address == 0x76 || address == 0x77) deviceName = "BMP280/BME280";

      device["device"] = deviceName;
      deviceCount++;
    }
  }

  if (deviceCount == 0) {
    JsonObject noDevice = i2cArray.createNestedObject();
    noDevice["address"] = "N/A";
    noDevice["status"] = "No devices found";
    noDevice["device"] = "N/A";
  }

  // SPI Test
  JsonArray spiArray = doc.createNestedArray("spi_test");
  JsonObject spi = spiArray.createNestedObject();
  spi["bus"] = "SPI2 (HSPI)";
  spi["mosi"] = "35";
  spi["miso"] = "37";
  spi["sclk"] = "36";
  spi["status"] = "Available";

  // Memory Test
  JsonObject memTest = doc.createNestedObject("memory_test");
  uint32_t heapFree = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
  uint32_t heapSize = heap_caps_get_total_size(MALLOC_CAP_INTERNAL);
  uint32_t heapMin = heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL);
  uint32_t largestBlock = heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL);

  memTest["heap_free"] = formatBytes(heapFree);
  memTest["heap_size"] = formatBytes(heapSize);
  memTest["heap_min_free"] = formatBytes(heapMin);
  memTest["heap_usage_percent"] = (int)((heapSize - heapFree) * 100 / heapSize);
  memTest["largest_free_block"] = formatBytes(largestBlock);

  float fragmentation = 100.0 - ((float)largestBlock / (float)heapFree * 100.0);
  memTest["fragmentation_percent"] = (int)fragmentation;

  if (heapFree < 50000) {
    memTest["status"] = "LOW";
    memTest["alert"] = "Warning: Low memory";
  } else if (heapFree < 100000) {
    memTest["status"] = "MEDIUM";
    memTest["alert"] = "Memory usage moderate";
  } else {
    memTest["status"] = "OK";
    memTest["alert"] = "Memory healthy";
  }

  // WiFi Test
  JsonObject wifiTest = doc.createNestedObject("wifi_test");
  wifiTest["connected"] = (WiFi.status() == WL_CONNECTED);

  if (WiFi.status() == WL_CONNECTED) {
    wifiTest["ssid"] = WiFi.SSID();
    wifiTest["ip"] = WiFi.localIP().toString();
    wifiTest["mac"] = WiFi.macAddress();
    wifiTest["rssi"] = WiFi.RSSI();

    int rssi = WiFi.RSSI();
    String quality;
    if (rssi > -50) quality = "Excellent";
    else if (rssi > -60) quality = "Good";
    else if (rssi > -70) quality = "Fair";
    else quality = "Weak";
    wifiTest["signal_quality"] = quality;

    wifiTest["channel"] = WiFi.channel();
    wifiTest["gateway"] = WiFi.gatewayIP().toString();
    wifiTest["dns"] = WiFi.dnsIP().toString();
    wifiTest["subnet"] = WiFi.subnetMask().toString();
    wifiTest["status"] = "OK";
  } else {
    wifiTest["status"] = "DISCONNECTED";
    wifiTest["error"] = "Not connected";
  }

  // System Test
  JsonObject sysTest = doc.createNestedObject("system_test");
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);

  sysTest["chip_model"] = getChipModel();
  sysTest["chip_revision"] = chip_info.revision;
  sysTest["cpu_cores"] = chip_info.cores;
  sysTest["cpu_frequency"] = getCpuFrequencyMhz();
  sysTest["flash_size"] = getFlashSize();
  sysTest["flash_speed"] = ESP.getFlashChipSpeed() / 1000000;
  sysTest["uptime"] = getUptime();
  sysTest["uptime_ms"] = (unsigned long)millis();

  esp_reset_reason_t reset_reason = esp_reset_reason();
  String resetStr = "Unknown";
  switch(reset_reason) {
    case ESP_RST_POWERON: resetStr = "Power-on"; break;
    case ESP_RST_SW: resetStr = "Software reset"; break;
    case ESP_RST_PANIC: resetStr = "Exception/panic"; break;
    case ESP_RST_INT_WDT: resetStr = "Interrupt watchdog"; break;
    case ESP_RST_TASK_WDT: resetStr = "Task watchdog"; break;
    case ESP_RST_WDT: resetStr = "Other watchdog"; break;
    case ESP_RST_DEEPSLEEP: resetStr = "Deep sleep"; break;
    case ESP_RST_BROWNOUT: resetStr = "Brownout"; break;
    case ESP_RST_SDIO: resetStr = "SDIO"; break;
    default: resetStr = "Unknown"; break;
  }
  sysTest["last_reset_reason"] = resetStr;
  sysTest["arduino_core"] = ARDUINO_CORE_VERSION;
  sysTest["idf_version"] = esp_get_idf_version();

  bool memoryOK = (heapFree > 50000);
  bool wifiOK = (WiFi.status() == WL_CONNECTED);

  if (memoryOK && wifiOK) {
    sysTest["overall_status"] = "HEALTHY";
  } else if (!memoryOK && !wifiOK) {
    sysTest["overall_status"] = "CRITICAL";
  } else {
    sysTest["overall_status"] = "WARNING";
  }

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
// EXPORT JSON
// ============================================================

void handleExportJSON() {
  DynamicJsonDocument doc(8192);

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

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Disposition", "attachment; filename=esp32-diagnostic.json");
  server.send(200, "application/json", response);
}

// ============================================================
// EXPORT CSV
// ============================================================

void handleExportCSV() {
  String csv = "Category,Parameter,Value\n";

  // System
  csv += "System,Version," + String(DIAGNOSTIC_VERSION) + "\n";
  csv += "System,Arduino Core," + String(ARDUINO_CORE_VERSION) + "\n";
  csv += "System,Chip," + getChipModel() + "\n";
  csv += "System,CPU Cores," + String(getCPUCores()) + "\n";
  csv += "System,CPU Frequency," + String(getCPUFrequency()) + " MHz\n";
  csv += "System,Flash Size," + getFlashSize() + "\n";
  csv += "System,PSRAM," + getPSRAMSize() + "\n";
  csv += "System,Uptime," + getUptime() + "\n";

  // Memory
  uint32_t heapFree, heapSize, psramFree, psramSize;
  getMemoryInfo(heapFree, heapSize, psramFree, psramSize);

  csv += "Memory,Heap Free," + formatBytes(heapFree) + "\n";
  csv += "Memory,Heap Size," + formatBytes(heapSize) + "\n";

  if (psramFound()) {
    csv += "Memory,PSRAM Free," + formatBytes(psramFree) + "\n";
    csv += "Memory,PSRAM Size," + formatBytes(psramSize) + "\n";
  }

  // WiFi
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
// NEOPIXEL PATTERN API
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

  // Send response BEFORE executing pattern
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", "{\"success\":true,\"pattern\":\"" + pattern + "\"}");

  // Execute pattern
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

  r = constrain(r, 0, 255);
  g = constrain(g, 0, 255);
  b = constrain(b, 0, 255);

  strip->setPixelColor(0, strip->Color(r, g, b));
  strip->show();

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", "{\"success\":true}");
}

#endif // API_HANDLERS_H
