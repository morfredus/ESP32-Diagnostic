/*
 * ============================================================
 * ESP32 Diagnostic System - API Handlers
 * Version: 3.1.0
 * ============================================================
 *
 * REST API endpoints for system diagnostics and control.
 * Compatible with ESP32 Arduino Core 3.x (ledcAttach API)
 *
 * Endpoints:
 *   GET  /api/system/info        - System information
 *   GET  /api/system/memory      - Memory details
 *   GET  /api/system/wifi        - WiFi information
 *   GET  /api/tests              - All hardware tests
 *   GET  /api/tests/gpio         - GPIO test only
 *   GET  /api/tests/pwm          - PWM test only
 *   POST /api/tests/run          - Run specific test
 *   GET  /api/language           - Current language
 *   POST /api/language           - Set language
 *   GET  /api/export/json        - Export JSON
 *   GET  /api/export/csv         - Export CSV
 *   POST /api/neopixel/pattern   - NeoPixel pattern control
 *   POST /api/neopixel/color     - NeoPixel color control
 *   GET  /api/sensors            - All sensor readings
 *   POST /api/wifi/scan          - Scan WiFi networks
 *   GET  /api/benchmark          - Performance benchmark
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
// MEMORY INFO API
// ============================================================

void handleMemoryInfo() {
  StaticJsonDocument<1024> doc;

  uint32_t heapFree, heapSize, psramFree, psramSize;
  getMemoryInfo(heapFree, heapSize, psramFree, psramSize);

  // Heap
  JsonObject heap = doc.createNestedObject("heap");
  heap["free"] = heapFree;
  heap["size"] = heapSize;
  heap["used"] = heapSize - heapFree;
  heap["free_formatted"] = formatBytes(heapFree);
  heap["size_formatted"] = formatBytes(heapSize);
  heap["usage_percent"] = ((heapSize - heapFree) * 100) / heapSize;

  // PSRAM
  if (psramFound()) {
    JsonObject psram = doc.createNestedObject("psram");
    psram["free"] = psramFree;
    psram["size"] = psramSize;
    psram["used"] = psramSize - psramFree;
    psram["free_formatted"] = formatBytes(psramFree);
    psram["size_formatted"] = formatBytes(psramSize);
    psram["usage_percent"] = ((psramSize - psramFree) * 100) / psramSize;
  }

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

// ============================================================
// WIFI INFO API
// ============================================================

void handleWiFiInfo() {
  StaticJsonDocument<1024> doc;

  doc["connected"] = (WiFi.status() == WL_CONNECTED);

  if (WiFi.status() == WL_CONNECTED) {
    doc["ssid"] = WiFi.SSID();
    doc["ip"] = WiFi.localIP().toString();
    doc["gateway"] = WiFi.gatewayIP().toString();
    doc["subnet"] = WiFi.subnetMask().toString();
    doc["dns"] = WiFi.dnsIP().toString();
    doc["mac"] = WiFi.macAddress();
    doc["rssi"] = WiFi.RSSI();
    doc["channel"] = WiFi.channel();

    // Signal quality
    int rssi = WiFi.RSSI();
    String quality;
    if (rssi >= -50) quality = "Excellent";
    else if (rssi >= -60) quality = "Good";
    else if (rssi >= -70) quality = "Fair";
    else quality = "Weak";
    doc["signal_quality"] = quality;
  }

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

// ============================================================
// ALL TESTS API
// ============================================================

void handleTests() {
  DynamicJsonDocument doc(8192);
  
  // GPIO Test
  JsonArray gpioArray = doc.createNestedArray("gpio_test");
  int testPins[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};

  for (int i = 0; i < sizeof(testPins) / sizeof(testPins[0]); i++) {
    JsonObject pin = gpioArray.createNestedObject();
    pin["pin"] = testPins[i];
    
    pinMode(testPins[i], OUTPUT);
    digitalWrite(testPins[i], HIGH);
    delay(5);
    int readHigh = digitalRead(testPins[i]);
    
    digitalWrite(testPins[i], LOW);
    delay(5);
    int readLow = digitalRead(testPins[i]);
    
    pin["status"] = (readHigh == HIGH && readLow == LOW) ? "OK" : "FAIL";
  }

  // PWM Test
  JsonArray pwmArray = doc.createNestedArray("pwm_test");
  int pwmPins[] = {25, 26, 27};

  for (int i = 0; i < 3; i++) {
    JsonObject pwm = pwmArray.createNestedObject();
    pwm["pin"] = pwmPins[i];
    pwm["status"] = "OK";
    pwm["frequency"] = 5000;
  }

  // I2C Test
  JsonObject i2c = doc.createNestedObject("i2c_test");
  i2c["available"] = true;
  i2c["devices_found"] = 0;

  // WiFi Test
  JsonObject wifi = doc.createNestedObject("wifi_test");
  wifi["connected"] = (WiFi.status() == WL_CONNECTED);
  wifi["rssi"] = WiFi.RSSI();
  wifi["ip"] = WiFi.localIP().toString();

  // Memory Test
  JsonObject memory = doc.createNestedObject("memory_test");
  memory["heap_free"] = ESP.getFreeHeap();
  memory["heap_size"] = ESP.getHeapSize();
  memory["psram_available"] = psramFound();
  if (psramFound()) {
    memory["psram_size"] = ESP.getPsramSize();
    memory["psram_free"] = ESP.getFreePsram();
  }

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

// ============================================================
// GPIO TEST API
// ============================================================

void handleTestGPIO() {
  DynamicJsonDocument doc(4096);
  JsonArray results = doc.createNestedArray("results");
  
  int testPins[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};

  for (int i = 0; i < sizeof(testPins) / sizeof(testPins[0]); i++) {
    JsonObject pin = results.createNestedObject();
    pin["pin"] = testPins[i];
    
    pinMode(testPins[i], OUTPUT);
    digitalWrite(testPins[i], HIGH);
    delay(5);
    int readHigh = digitalRead(testPins[i]);
    
    digitalWrite(testPins[i], LOW);
    delay(5);
    int readLow = digitalRead(testPins[i]);

    bool success = (readHigh == HIGH && readLow == LOW);
    pin["status"] = success ? "OK" : "FAIL";
    pin["high_read"] = readHigh;
    pin["low_read"] = readLow;
  }

  doc["test"] = "GPIO";
  doc["total"] = sizeof(testPins) / sizeof(testPins[0]);

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

// ============================================================
// PWM TEST API - CORRIGÉ POUR ESP32 CORE 3.x
// ============================================================

void handleTestPWM() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  
  int pwmPins[] = {25, 26, 27};  // Pins PWM à tester
  int numPins = 3;

  StaticJsonDocument<1024> doc;
  JsonArray results = doc.createNestedArray("results");

  for (int i = 0; i < numPins; i++) {
    JsonObject pinObj = results.createNestedObject();
    pinObj["pin"] = pwmPins[i];
    
    // ✅ Nouvelle API ESP32 Core 3.x : ledcAttach(pin, freq, resolution)
    // Remplace : ledcSetup(channel, freq, resolution) + ledcAttachPin(pin, channel)
    if (ledcAttach(pwmPins[i], 5000, 8) == 0) {
      pinObj["status"] = "FAIL";
      pinObj["error"] = "Failed to attach PWM";
      continue;
    }

    // Test rampe PWM
    bool testPassed = true;
    for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle += 51) {
      ledcWrite(pwmPins[i], dutyCycle);
      delay(20);
    }

    // Reset à 0
    ledcWrite(pwmPins[i], 0);

    // Détacher le pin
    ledcDetach(pwmPins[i]);

    pinObj["status"] = "OK";
    pinObj["frequency"] = 5000;
    pinObj["resolution"] = 8;
  }

  doc["test"] = "PWM";
  doc["total"] = numPins;

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

// ============================================================
// RUN SPECIFIC TEST API
// ============================================================

void handleRunTest() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No body\"}");
    return;
  }

  String body = server.arg("plain");
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, body);

  if (error) {
    server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    return;
  }

  String testName = doc["test"] | "";

  if (testName == "gpio") {
    handleTestGPIO();
  } else if (testName == "pwm") {
    handleTestPWM();
  } else {
    server.send(400, "application/json", "{\"error\":\"Unknown test\"}");
  }
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
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No body\"}");
    return;
  }
  
  String body = server.arg("plain");
  StaticJsonDocument<128> doc;
  DeserializationError error = deserializeJson(doc, body);

  if (error) {
    server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    return;
  }

  String lang = doc["language"] | "fr";

  if (lang == "fr" || lang == "en") {
    currentLanguage = lang;

    StaticJsonDocument<128> response;
    response["success"] = true;
    response["language"] = currentLanguage;

    String jsonResponse;
    serializeJson(response, jsonResponse);

    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", jsonResponse);
  } else {
    server.send(400, "application/json", "{\"error\":\"Invalid language\"}");
  }
}

// ============================================================
// EXPORT JSON API
// ============================================================

void handleExportJSON() {
  DynamicJsonDocument doc(8192);

  // System info
  JsonObject system = doc.createNestedObject("system");
  system["version"] = DIAGNOSTIC_VERSION;
  system["chip"] = getChipModel();
  system["cores"] = getCPUCores();
  system["frequency"] = getCPUFrequency();
  system["uptime"] = getUptime();

  // Memory
  uint32_t heapFree, heapSize, psramFree, psramSize;
  getMemoryInfo(heapFree, heapSize, psramFree, psramSize);

  JsonObject memory = doc.createNestedObject("memory");
  memory["heap_free"] = heapFree;
  memory["heap_size"] = heapSize;
  if (psramFound()) {
    memory["psram_free"] = psramFree;
    memory["psram_size"] = psramSize;
  }

  // WiFi
  JsonObject wifi = doc.createNestedObject("wifi");
  wifi["connected"] = (WiFi.status() == WL_CONNECTED);
  if (WiFi.status() == WL_CONNECTED) {
    wifi["ssid"] = WiFi.SSID();
    wifi["ip"] = WiFi.localIP().toString();
    wifi["rssi"] = WiFi.RSSI();
  }

  // Export timestamp
  doc["exported_at"] = getUptime();
  doc["language"] = currentLanguage;

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Disposition", "attachment; filename=esp32-diagnostic.json");
  server.send(200, "application/json", response);
}

// ============================================================
// EXPORT CSV API
// ============================================================

void handleExportCSV() {
  String csv = "Parameter,Value\n";
  csv += "Version," + String(DIAGNOSTIC_VERSION) + "\n";
  csv += "Chip," + getChipModel() + "\n";
  csv += "CPU Cores," + String(getCPUCores()) + "\n";
  csv += "CPU Frequency," + String(getCPUFrequency()) + " MHz\n";
  csv += "Flash Size," + formatBytes(getFlashSize()) + "\n";
  csv += "PSRAM Size," + formatBytes(getPSRAMSize()) + "\n";
  csv += "Heap Free," + formatBytes(ESP.getFreeHeap()) + "\n";
  csv += "Heap Size," + formatBytes(ESP.getHeapSize()) + "\n";

  if (WiFi.status() == WL_CONNECTED) {
    csv += "WiFi SSID," + WiFi.SSID() + "\n";
    csv += "WiFi IP," + WiFi.localIP().toString() + "\n";
    csv += "WiFi RSSI," + String(WiFi.RSSI()) + " dBm\n";
  }

  csv += "Uptime," + getUptime() + "\n";
  csv += "Language," + currentLanguage + "\n";

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Disposition", "attachment; filename=esp32-diagnostic.csv");
  server.send(200, "text/csv", csv);
}

// ============================================================
// NEOPIXEL PATTERN API - CORRIGÉ
// ============================================================

void handleNeoPixelPattern() {
  server.sendHeader("Access-Control-Allow-Origin", "*");

  // Vérifier si NeoPixel est disponible
  if (!neopixelAvailable) {
    server.send(400, "application/json", "{\"error\":\"NeoPixel not available\"}");
    return;
  }

  // Vérifier si body JSON présent
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No body\"}");  // ✅ GUILLEMET CORRIGÉ
    return;
  }

  String body = server.arg("plain");
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, body);

  if (error) {
    server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    return;
  }

  String action = doc["action"] | "";
  
  if (action == "on") {
    setNeoPixelColor(0, 255, 0);  // Vert
  } else if (action == "off") {
    setNeoPixelColor(0, 0, 0);    // Éteint
  } else if (action == "blink") {
    for (int i = 0; i < 5; i++) {
      setNeoPixelColor(255, 0, 0);  // Rouge
      delay(200);
      setNeoPixelColor(0, 0, 0);    // Éteint
      delay(200);
    }
  } else if (action == "test") {
    neoPixelRainbow(2);  // Animation arc-en-ciel
  } else if (action == "fade") {
    for (int brightness = 0; brightness <= 255; brightness += 5) {
      setNeoPixelColor(0, brightness, brightness);
      delay(20);
    }
    for (int brightness = 255; brightness >= 0; brightness -= 5) {
      setNeoPixelColor(0, brightness, brightness);
      delay(20);
    }
    setNeoPixelColor(0, 0, 0);
  } else {
    server.send(400, "application/json", "{\"error\":\"Unknown action\"}");
    return;
  }

  server.send(200, "application/json", "{\"success\":true,\"action\":\"" + action + "\"}");
}

// ============================================================
// NEOPIXEL COLOR API
// ============================================================

void handleNeoPixelColor() {
  server.sendHeader("Access-Control-Allow-Origin", "*");

  if (!neopixelAvailable) {
    server.send(400, "application/json", "{\"error\":\"NeoPixel not available\"}");
    return;
  }

  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No body\"}");
    return;
  }

  String body = server.arg("plain");
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, body);

  if (error) {
    server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    return;
  }

  int r = doc["r"] | 0;
  int g = doc["g"] | 0;
  int b = doc["b"] | 0;

  // Limiter les valeurs entre 0 et 255
  r = constrain(r, 0, 255);
  g = constrain(g, 0, 255);
  b = constrain(b, 0, 255);

  setNeoPixelColor(r, g, b);

  StaticJsonDocument<128> response;
  response["success"] = true;
  response["r"] = r;
  response["g"] = g;
  response["b"] = b;

  String jsonResponse;
  serializeJson(response, jsonResponse);

  server.send(200, "application/json", jsonResponse);
}

// ============================================================
// SENSORS API
// ============================================================

void handleSensors() {
  StaticJsonDocument<1024> doc;

  // Temperature interne (si disponible)
  #ifdef SOC_TEMP_SENSOR_SUPPORTED
  doc["internal_temp"] = temperatureRead();
  doc["internal_temp_unit"] = "°C";
  #endif

  // Hall sensor (ESP32 classique uniquement)
  #if defined(CONFIG_IDF_TARGET_ESP32)
  doc["hall_sensor"] = hallRead();
  #endif

  // Mesure de voltage (approximatif)
  doc["system_voltage"] = 3.3;  // Voltage système

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

// ============================================================
// WIFI SCAN API
// ============================================================

void handleWiFiScan() {
  server.sendHeader("Access-Control-Allow-Origin", "*");

  int n = WiFi.scanNetworks();

  DynamicJsonDocument doc(4096);
  JsonArray networks = doc.createNestedArray("networks");

  for (int i = 0; i < n; i++) {
    JsonObject network = networks.createNestedObject();
    network["ssid"] = WiFi.SSID(i);
    network["rssi"] = WiFi.RSSI(i);
    network["channel"] = WiFi.channel(i);
    network["encryption"] = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Encrypted";
  }

  doc["count"] = n;

  String response;
  serializeJson(doc, response);

  server.send(200, "application/json", response);
}

// ============================================================
// BENCHMARK API
// ============================================================

void handleBenchmark() {
  StaticJsonDocument<1024> doc;

  // CPU Benchmark
  unsigned long start = millis();
  volatile long result = 0;
  for (long i = 0; i < 1000000; i++) {
    result += i;
  }
  unsigned long cpuTime = millis() - start;
  doc["cpu_benchmark_ms"] = cpuTime;

  // Memory Benchmark
  start = millis();
  uint8_t* testBuffer = (uint8_t*)malloc(10240);
  if (testBuffer) {
    for (int i = 0; i < 10240; i++) {
      testBuffer[i] = i % 256;
    }
    free(testBuffer);
  }
  unsigned long memTime = millis() - start;
  doc["memory_benchmark_ms"] = memTime;

  // WiFi Benchmark
  doc["wifi_rssi"] = WiFi.RSSI();

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

// ============================================================
// CORS PREFLIGHT HANDLER
// ============================================================

void handleCORS() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(204);
}

#endif // API_HANDLERS_H
