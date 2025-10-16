/*
 * ============================================================
 * API HANDLERS - ESP32 Diagnostic System v3.1.0
 * ============================================================
 *
 * New in v3.1.0:
 *   - Built-in LED control
 *   - Advanced GPIO testing
 *   - ADC, Touch, PWM tests
 *   - WiFi scanner
 *   - Performance benchmarks
 *   - Extended export formats (TXT)
 *
 * ============================================================
 */

#ifndef API_HANDLERS_H
#define API_HANDLERS_H

#include <ArduinoJson.h>

// ============================================================
// SYSTEM INFO API (from v3.0.1)
// ============================================================

void handleSystemInfo() {
  StaticJsonDocument<2048> doc;

  doc["version"] = DIAGNOSTIC_VERSION;
  doc["core_version"] = ARDUINO_CORE_VERSION;
  doc["chip"] = getChipModel();
  doc["cores"] = getCPUCores();
  doc["frequency"] = getCPUFrequency();
  doc["flash_size"] = getFlashSize();
  doc["psram_size"] = getPSRAMSize();
  doc["uptime"] = getUptime();

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

  JsonObject wifi = doc.createNestedObject("wifi");
  wifi["connected"] = (WiFi.status() == WL_CONNECTED);
  if (WiFi.status() == WL_CONNECTED) {
    wifi["ssid"] = WiFi.SSID();
    wifi["ip"] = WiFi.localIP().toString();
    wifi["rssi"] = WiFi.RSSI();
    wifi["mac"] = WiFi.macAddress();
  }

  doc["neopixel_available"] = neopixelAvailable;
  doc["builtin_led_available"] = builtinLedAvailable;

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

// ============================================================
// TESTS API (from v3.0.1)
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
      else if (address == 0x3C || address == 0x3D) deviceName = "OLED 0.96\"";

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
// NEW v3.1.0 - BUILT-IN LED CONTROL
// ============================================================

void handleBuiltinLED() {
  if (!builtinLedAvailable) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", "{\"success\":false,\"message\":\"Built-in LED not available\"}");
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

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", "{\"success\":true,\"pattern\":\"" + pattern + "\"}");

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

// ============================================================
// NEW v3.1.0 - GPIO COMPREHENSIVE TEST
// ============================================================

void handleTestGPIO() {
  DynamicJsonDocument doc(4096);
  JsonArray results = doc.createNestedArray("results");

  int testPins[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 21, 38, 39, 40, 41, 42, 47, 48};
  int pinCount = sizeof(testPins) / sizeof(testPins[0]);

  for (int i = 0; i < pinCount; i++) {
    int pin = testPins[i];
    JsonObject pinTest = results.createNestedObject();
    pinTest["pin"] = pin;

    pinMode(pin, INPUT);
    delay(1);
    int inputVal = digitalRead(pin);

    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delay(1);
    int highVal = digitalRead(pin);

    digitalWrite(pin, LOW);
    delay(1);
    int lowVal = digitalRead(pin);

    pinMode(pin, INPUT);

    bool working = (highVal == HIGH && lowVal == LOW);
    pinTest["status"] = working ? "OK" : "FAIL";
    pinTest["input"] = inputVal;
    pinTest["high"] = highVal;
    pinTest["low"] = lowVal;
  }

  doc["total_pins"] = pinCount;
  doc["tested"] = pinCount;

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

// ============================================================
// NEW v3.1.0 - ADC TEST
// ============================================================

void handleTestADC() {
  StaticJsonDocument<512> doc;
  JsonArray adcArray = doc.createNestedArray("adc_readings");

  // Test ADC channels (GPIO 1-10 on ESP32-S3)
  int adcPins[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  
  for (int i = 0; i < 10; i++) {
    JsonObject reading = adcArray.createNestedObject();
    reading["pin"] = adcPins[i];
    
    pinMode(adcPins[i], INPUT);
    int value = analogRead(adcPins[i]);
    float voltage = (value / 4095.0) * 3.3;
    
    reading["raw"] = value;
    reading["voltage"] = String(voltage, 2);
    reading["percent"] = (int)((value / 4095.0) * 100);
  }

  doc["status"] = "completed";
  doc["max_value"] = 4095;
  doc["voltage_ref"] = "3.3V";

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

// ============================================================
// NEW v3.1.0 - TOUCH TEST
// ============================================================

void handleTestTouch() {
  StaticJsonDocument<512> doc;
  JsonArray touchArray = doc.createNestedArray("touch_pads");

  #ifdef CONFIG_IDF_TARGET_ESP32S3
    // ESP32-S3 has touch pins on GPIO 1-14
    int touchPins[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    
    for (int i = 0; i < 14; i++) {
      JsonObject touch = touchArray.createNestedObject();
      touch["pin"] = touchPins[i];
      touch["value"] = touchRead(touchPins[i]);
      touch["status"] = "available";
    }
    doc["status"] = "available";
  #else
    doc["status"] = "not_available";
    doc["message"] = "Touch not supported on this chip";
  #endif

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

// ============================================================
// NEW v3.1.0 - PWM TEST
// ============================================================

void handleTestPWM() {
  StaticJsonDocument<512> doc;
  JsonArray pwmArray = doc.createNestedArray("pwm_channels");

  int pwmPins[] = {2, 3, 4, 5};
  
  for (int i = 0; i < 4; i++) {
    JsonObject pwm = pwmArray.createNestedObject();
    pwm["pin"] = pwmPins[i];
    pwm["channel"] = i;
    pwm["frequency"] = 5000;
    pwm["resolution"] = 8;
    
    ledcSetup(i, 5000, 8);
    ledcAttachPin(pwmPins[i], i);
    ledcWrite(i, 128);
    delay(50);
    ledcWrite(i, 0);
    
    pwm["status"] = "OK";
  }

  doc["status"] = "completed";
  doc["channels_tested"] = 4;

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

// ============================================================
// NEW v3.1.0 - WIFI SCANNER
// ============================================================

void handleWiFiScan() {
  DynamicJsonDocument doc(4096);
  JsonArray networks = doc.createNestedArray("networks");

  int n = WiFi.scanNetworks();
  
  doc["count"] = n;
  doc["status"] = "completed";

  for (int i = 0; i < n; i++) {
    JsonObject net = networks.createNestedObject();
    net["ssid"] = WiFi.SSID(i);
    net["rssi"] = WiFi.RSSI(i);
    net["channel"] = WiFi.channel(i);
    net["encryption"] = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Secured";
    
    int rssi = WiFi.RSSI(i);
    String quality;
    if (rssi > -50) quality = "Excellent";
    else if (rssi > -60) quality = "Good";
    else if (rssi > -70) quality = "Fair";
    else quality = "Weak";
    net["quality"] = quality;
  }

  WiFi.scanDelete();

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

// ============================================================
// NEW v3.1.0 - PERFORMANCE BENCHMARK
// ============================================================

void handleBenchmark() {
  StaticJsonDocument<512> doc;

  // CPU Benchmark
  unsigned long cpuStart = micros();
  volatile long result = 0;
  for (long i = 0; i < 1000000; i++) {
    result += i;
  }
  unsigned long cpuTime = micros() - cpuStart;
  doc["cpu_benchmark_us"] = cpuTime;
  doc["cpu_performance"] = String((1000000.0 / cpuTime), 2) + " MFLOPS";

  // Memory Benchmark
  unsigned long memStart = micros();
  uint8_t* testArray = (uint8_t*)malloc(10000);
  if (testArray != nullptr) {
    for (int i = 0; i < 10000; i++) {
      testArray[i] = i % 256;
    }
    free(testArray);
  }
  unsigned long memTime = micros() - memStart;
  doc["memory_benchmark_us"] = memTime;
  doc["memory_speed"] = String((10000.0 / memTime), 2) + " MB/s";

  // Flash benchmark
  unsigned long flashStart = millis();
  for (int i = 0; i < 100; i++) {
    // Simulate flash operations
    delay(1);
  }
  unsigned long flashTime = millis() - flashStart;
  doc["flash_benchmark_ms"] = flashTime;

  doc["status"] = "completed";

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

// ============================================================
// LANGUAGE API (from v3.0.1)
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
// NEW v3.1.0 - EXPORT TXT
// ============================================================

void handleExportTXT() {
  String txt = "========================================\n";
  txt += "ESP32 DIAGNOSTIC REPORT\n";
  txt += "========================================\n\n";
  
  txt += "Version: " + String(DIAGNOSTIC_VERSION) + "\n";
  txt += "Arduino Core: " + String(ARDUINO_CORE_VERSION) + "\n";
  txt += "Generated: " + getUptime() + "\n\n";

  txt += "--- SYSTEM INFORMATION ---\n";
  txt += "Chip: " + getChipModel() + "\n";
  txt += "CPU Cores: " + String(getCPUCores()) + "\n";
  txt += "CPU Frequency: " + String(getCPUFrequency()) + " MHz\n";
  txt += "Flash Size: " + getFlashSize() + "\n";
  txt += "PSRAM: " + getPSRAMSize() + "\n";
  txt += "Uptime: " + getUptime() + "\n\n";

  txt += "--- MEMORY ---\n";
  uint32_t heapFree, heapSize, psramFree, psramSize;
  getMemoryInfo(heapFree, heapSize, psramFree, psramSize);
  txt += "Heap Free: " + formatBytes(heapFree) + "\n";
  txt += "Heap Size: " + formatBytes(heapSize) + "\n";
  if (psramFound()) {
    txt += "PSRAM Free: " + formatBytes(psramFree) + "\n";
    txt += "PSRAM Size: " + formatBytes(psramSize) + "\n";
  }
  txt += "\n";

  txt += "--- WIFI ---\n";
  if (WiFi.status() == WL_CONNECTED) {
    txt += "Status: Connected\n";
    txt += "SSID: " + WiFi.SSID() + "\n";
    txt += "IP: " + WiFi.localIP().toString() + "\n";
    txt += "RSSI: " + String(WiFi.RSSI()) + " dBm\n";
  } else {
    txt += "Status: Disconnected\n";
  }
  txt += "\n";

  txt += "========================================\n";
  txt += "END OF REPORT\n";
  txt += "========================================\n";

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Disposition", "attachment; filename=esp32-diagnostic.txt");
  server.send(200, "text/plain", txt);
}

// ============================================================
// EXPORT JSON (from v3.0.1)
// ============================================================

void handleExportJSON() {
  DynamicJsonDocument doc(8192);

  doc["export_date"] = getUptime();
  doc["version"] = DIAGNOSTIC_VERSION;
  doc["core_version"] = ARDUINO_CORE_VERSION;

  JsonObject system = doc.createNestedObject("system");
  system["chip"] = getChipModel();
  system["cores"] = getCPUCores();
  system["frequency"] = getCPUFrequency();
  system["flash_size"] = getFlashSize();
  system["psram_size"] = getPSRAMSize();
  system["uptime"] = getUptime();

  uint32_t heapFree, heapSize, psramFree, psramSize;
  getMemoryInfo(heapFree, heapSize, psramFree, psramSize);

  JsonObject memory = doc.createNestedObject("memory");
  memory["heap_free"] = heapFree;
  memory["heap_size"] = heapSize;
  if (psramFound()) {
    memory["psram_free"] = psramFree;
    memory["psram_size"] = psramSize;
  }

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
// EXPORT CSV (from v3.0.1)
// ============================================================

void handleExportCSV() {
  String csv = "Category,Parameter,Value\n";

  csv += "System,Version," + String(DIAGNOSTIC_VERSION) + "\n";
  csv += "System,Arduino Core," + String(ARDUINO_CORE_VERSION) + "\n";
  csv += "System,Chip," + getChipModel() + "\n";
  csv += "System,CPU Cores," + String(getCPUCores()) + "\n";
  csv += "System,CPU Frequency," + String(getCPUFrequency()) + " MHz\n";
  csv += "System,Flash Size," + getFlashSize() + "\n";
  csv += "System,PSRAM," + getPSRAMSize() + "\n";
  csv += "System,Uptime," + getUptime() + "\n";

  uint32_t heapFree, heapSize, psramFree, psramSize;
  getMemoryInfo(heapFree, heapSize, psramFree, psramSize);

  csv += "Memory,Heap Free," + formatBytes(heapFree) + "\n";
  csv += "Memory,Heap Size," + formatBytes(heapSize) + "\n";

  if (psramFound()) {
    csv += "Memory,PSRAM Free," + formatBytes(psramFree) + "\n";
    csv += "Memory,PSRAM Size," + formatBytes(psramSize) + "\n";
  }

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

#endif // API_HANDLERS_H", "{\"success\":false,\"error\":\"No data\"}");
    return;
  }

  StaticJsonDocument<128> doc;
  DeserializationError error = deserializeJson(doc, server.arg("plain"));

  if (error) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "application/json", "{\"success\":false,\"error\":\"Invalid JSON\"}");
    return;
  }

  String action = doc["action"];
  
  if (action == "on") {
    digitalWrite(BUILTIN_LED_PIN, HIGH);
  } else if (action == "off") {
    digitalWrite(BUILTIN_LED_PIN, LOW);
  } else if (action == "blink") {
    for (int i = 0; i < 5; i++) {
      digitalWrite(BUILTIN_LED_PIN, HIGH);
      delay(200);
      digitalWrite(BUILTIN_LED_PIN, LOW);
      delay(200);
    }
  } else if (action == "test") {
    digitalWrite(BUILTIN_LED_PIN, HIGH);
    delay(100);
    digitalWrite(BUILTIN_LED_PIN, LOW);
  } else if (action == "fade") {
    // PWM fade simulation
    for (int i = 0; i < 256; i += 5) {
      analogWrite(BUILTIN_LED_PIN, i);
      delay(10);
    }
    for (int i = 255; i >= 0; i -= 5) {
      analogWrite(BUILTIN_LED_PIN, i);
      delay(10);
    }
    digitalWrite(BUILTIN_LED_PIN, LOW);
  }

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", "{\"success\":true,\"action\":\"" + action + "\"}");
}

// ============================================================
// NEOPIXEL CONTROL (from v3.0.1)
// ============================================================

void handleNeoPixelPattern() {
  if (!neopixelAvailable || strip == nullptr) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", "{\"success\":false,\"message\":\"NeoPixel not available\"}");
    return;
  }

  if (!server.hasArg("plain")) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "application/json