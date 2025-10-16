
/*
 * API_HANDLERS.H - Handlers API REST v3.0.0
 * Tous les endpoints pour l'interface dynamique
 */

#ifndef API_HANDLERS_H
#define API_HANDLERS_H

// ========== FONCTIONS DE TESTS (à inclure depuis le fichier principal) ==========
// Déclarations externes
extern bool testSingleGPIO(int pin);
extern void testAllGPIOs();
extern void scanWiFiNetworks();
extern void scanI2C();
extern void resetBuiltinLEDTest();
extern void testBuiltinLED();
extern void resetNeoPixelTest();
extern void testNeoPixel();
extern void neopixelRainbow();
extern void neopixelBlink(uint32_t color, int times);
extern void neopixelFade(uint32_t color);
extern void resetTFTTest();
extern void testTFT();
extern void tftTestColors();
extern void tftTestCheckerboard();
extern void tftClear();
extern void resetOLEDTest();
extern void testOLED();
extern void oledShowMessage(String message);
extern void testADC();
extern void testTouchPads();
extern void testPWM();
extern void scanSPI();
extern void listPartitions();
extern void memoryStressTest();
extern unsigned long benchmarkCPU();
extern unsigned long benchmarkMemory();

// Variables externes
extern WebServer server;
extern DiagnosticInfo diagnosticData;
extern DetailedMemoryInfo detailedMemory;
extern std::vector<GPIOTestResult> gpioResults;
extern std::vector<WiFiNetwork> wifiNetworks;
extern std::vector<ADCReading> adcReadings;
extern std::vector<TouchReading> touchReadings;
extern int BUILTIN_LED_PIN;
extern bool builtinLedTested;
extern bool builtinLedAvailable;
extern String builtinLedTestResult;
extern int LED_PIN;
extern int LED_COUNT;
extern Adafruit_NeoPixel *strip;
extern bool neopixelTested;
extern bool neopixelAvailable;
extern String neopixelTestResult;
extern bool tftTested;
extern bool tftAvailable;
extern String tftTestResult;
extern int tftWidth;
extern int tftHeight;
extern bool oledTested;
extern bool oledAvailable;
extern String oledTestResult;
extern int I2C_SDA;
extern int I2C_SCL;
extern String adcTestResult;
extern String touchTestResult;
extern String pwmTestResult;
extern String spiInfo;
extern String partitionsInfo;
extern String stressTestResult;
extern Language currentLanguage;

// ========== API STATUS (Temps réel) ==========
void handleAPIStatus() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  String json = "{";
  json += "\"uptime\":" + String(diagnosticData.uptime) + ",";
  json += "\"temperature\":" + String(diagnosticData.temperature, 1) + ",";
  json += "\"sram\":{";
  json += "\"total\":" + String(detailedMemory.sramTotal) + ",";
  json += "\"free\":" + String(detailedMemory.sramFree) + ",";
  json += "\"used\":" + String(detailedMemory.sramUsed);
  json += "},";

  if (detailedMemory.psramAvailable) {
    json += "\"psram\":{";
    json += "\"total\":" + String(detailedMemory.psramTotal) + ",";
    json += "\"free\":" + String(detailedMemory.psramFree) + ",";
    json += "\"used\":" + String(detailedMemory.psramUsed);
    json += "},";
  }

  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1);
  json += "}";

  server.send(200, "application/json", json);
}

// ========== API OVERVIEW ==========
void handleAPIOverview() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  String json = "{";
  json += "\"chip\":{";
  json += "\"model\":\"" + diagnosticData.chipModel + "\",";
  json += "\"revision\":\"" + diagnosticData.chipRevision + "\",";
  json += "\"cores\":" + String(diagnosticData.cpuCores) + ",";
  json += "\"freq\":" + String(diagnosticData.cpuFreqMHz) + ",";
  json += "\"mac\":\"" + diagnosticData.macAddress + "\",";
  json += "\"features\":\"" + getChipFeatures() + "\",";
  json += "\"sdk\":\"" + diagnosticData.sdkVersion + "\",";
  json += "\"idf\":\"" + diagnosticData.idfVersion + "\",";
  json += "\"uptime\":" + String(diagnosticData.uptime) + ",";
  json += "\"temperature\":" + String(diagnosticData.temperature, 1) + ",";
  json += "\"reset_reason\":\"" + getResetReason() + "\"";
  json += "},";

  json += "\"memory\":{";
  json += "\"flash\":{";
  json += "\"real\":" + String(detailedMemory.flashSizeReal) + ",";
  json += "\"config\":" + String(detailedMemory.flashSizeChip) + ",";
  json += "\"type\":\"" + getFlashType() + "\",";
  json += "\"speed\":\"" + getFlashSpeed() + "\"";
  json += "},";
  json += "\"sram\":{";
  json += "\"total\":" + String(detailedMemory.sramTotal) + ",";
  json += "\"free\":" + String(detailedMemory.sramFree) + ",";
  json += "\"used\":" + String(detailedMemory.sramUsed);
  json += "},";

  if (detailedMemory.psramAvailable) {
    json += "\"psram\":{";
    json += "\"total\":" + String(detailedMemory.psramTotal) + ",";
    json += "\"free\":" + String(detailedMemory.psramFree) + ",";
    json += "\"used\":" + String(detailedMemory.psramUsed);
    json += "},";
  }

  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1);
  json += "},";

  json += "\"wifi\":{";
  json += "\"ssid\":\"" + diagnosticData.wifiSSID + "\",";
  json += "\"rssi\":" + String(diagnosticData.wifiRSSI) + ",";
  json += "\"quality\":\"" + getWiFiSignalQuality() + "\",";
  json += "\"ip\":\"" + diagnosticData.ipAddress + "\",";
  json += "\"subnet\":\"" + WiFi.subnetMask().toString() + "\",";
  json += "\"gateway\":\"" + WiFi.gatewayIP().toString() + "\"";
  json += "},";

  json += "\"gpio\":{";
  json += "\"total\":" + String(diagnosticData.totalGPIO) + ",";
  json += "\"i2c_count\":" + String(diagnosticData.i2cCount) + ",";
  json += "\"i2c_devices\":\"" + diagnosticData.i2cDevices + "\"";
  json += "}";
  json += "}";

  server.send(200, "application/json", json);
}

// ========== API LEDS INFO ==========
void handleAPILedsInfo() {
  String json = "{";
  json += "\"builtin\":{";
  json += "\"pin\":" + String(BUILTIN_LED_PIN) + ",";
  json += "\"tested\":" + String(builtinLedTested ? "true" : "false") + ",";
  json += "\"available\":" + String(builtinLedAvailable ? "true" : "false") + ",";
  json += "\"status\":\"" + builtinLedTestResult + "\"";
  json += "},";
  json += "\"neopixel\":{";
  json += "\"pin\":" + String(LED_PIN) + ",";
  json += "\"count\":" + String(LED_COUNT) + ",";
  json += "\"tested\":" + String(neopixelTested ? "true" : "false") + ",";
  json += "\"available\":" + String(neopixelAvailable ? "true" : "false") + ",";
  json += "\"status\":\"" + neopixelTestResult + "\"";
  json += "}";
  json += "}";

  server.send(200, "application/json", json);
}

// ========== API SCREENS INFO ==========
void handleAPIScreensInfo() {
  String json = "{";
  json += "\"tft\":{";
  json += "\"tested\":" + String(tftTested ? "true" : "false") + ",";
  json += "\"available\":" + String(tftAvailable ? "true" : "false") + ",";
  json += "\"status\":\"" + tftTestResult + "\",";
  json += "\"width\":" + String(tftWidth) + ",";
  json += "\"height\":" + String(tftHeight);
  json += "},";
  json += "\"oled\":{";
  json += "\"tested\":" + String(oledTested ? "true" : "false") + ",";
  json += "\"available\":" + String(oledAvailable ? "true" : "false") + ",";
  json += "\"status\":\"" + oledTestResult + "\",";
  json += "\"pins\":{";
  json += "\"sda\":" + String(I2C_SDA) + ",";
  json += "\"scl\":" + String(I2C_SCL);
  json += "}";
  json += "}";
  json += "}";

  server.send(200, "application/json", json);
}

// ========== CHANGEMENT DE LANGUE ==========
void handleSetLanguage() {
  if (server.hasArg("lang")) {
    String lang = server.arg("lang");
    if (lang == "fr") {
      setLanguage(LANG_FR);
    } else if (lang == "en") {
      setLanguage(LANG_EN);
    }
    server.send(200, "application/json", "{\"success\":true,\"lang\":\"" + lang + "\"}");
  } else {
    server.send(400, "application/json", "{\"success\":false}");
  }
}

void handleGetTranslations() {
  String json = "{";
  json += "\"title\":\"" + String(T().title) + "\",";
  json += "\"nav_overview\":\"" + String(T().nav_overview) + "\",";
  json += "\"nav_leds\":\"" + String(T().nav_leds) + "\",";
  json += "\"nav_screens\":\"" + String(T().nav_screens) + "\",";
  json += "\"nav_tests\":\"" + String(T().nav_tests) + "\",";
  json += "\"nav_gpio\":\"" + String(T().nav_gpio) + "\",";
  json += "\"nav_wifi\":\"" + String(T().nav_wifi) + "\",";
  json += "\"nav_benchmark\":\"" + String(T().nav_benchmark) + "\",";
  json += "\"nav_export\":\"" + String(T().nav_export) + "\",";
  json += "\"chip_info\":\"" + String(T().chip_info) + "\",";
  json += "\"memory_details\":\"" + String(T().memory_details) + "\",";
  json += "\"wifi_connection\":\"" + String(T().wifi_connection) + "\",";
  json += "\"gpio_interfaces\":\"" + String(T().gpio_interfaces) + "\",";
  json += "\"builtin_led\":\"" + String(T().builtin_led) + "\",";
  json += "\"neopixel\":\"" + String(T().neopixel) + "\",";
  json += "\"tft_screen\":\"" + String(T().tft_screen) + "\",";
  json += "\"oled_screen\":\"" + String(T().oled_screen) + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

// ========== TEST GPIO ==========
void handleTestGPIO() {
  testAllGPIOs();
  String json = "{\"results\":[";
  for (size_t i = 0; i < gpioResults.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"pin\":" + String(gpioResults[i].pin) + ",\"working\":" + String(gpioResults[i].working ? "true" : "false") + "}";
  }
  json += "]}";
  server.send(200, "application/json", json);
}

// ========== WIFI SCAN ==========
void handleWiFiScan() {
  scanWiFiNetworks();
  String json = "{\"networks\":[";
  for (size_t i = 0; i < wifiNetworks.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"ssid\":\"" + wifiNetworks[i].ssid + "\",\"rssi\":" + String(wifiNetworks[i].rssi) +
            ",\"channel\":" + String(wifiNetworks[i].channel) + ",\"encryption\":\"" + wifiNetworks[i].encryption +
            "\",\"bssid\":\"" + wifiNetworks[i].bssid + "\"}";
  }
  json += "]}";
  server.send(200, "application/json", json);
}

// ========== LED BUILTIN ==========
void handleBuiltinLEDTest() {
  resetBuiltinLEDTest();
  testBuiltinLED();
  server.send(200, "application/json", "{\"success\":" + String(builtinLedAvailable ? "true" : "false") + ",\"result\":\"" + builtinLedTestResult + "\"}");
}

void handleBuiltinLEDControl() {
  if (!server.hasArg("action")) {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }

  String action = server.arg("action");
  if (BUILTIN_LED_PIN == -1) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"LED non configuree\"}");
    return;
  }

  pinMode(BUILTIN_LED_PIN, OUTPUT);
  String message = "";

  if (action == "on") {
    digitalWrite(BUILTIN_LED_PIN, HIGH);
    message = "LED allumee";
  } else if (action == "blink") {
    for(int i = 0; i < 5; i++) {
      digitalWrite(BUILTIN_LED_PIN, HIGH);
      delay(200);
      digitalWrite(BUILTIN_LED_PIN, LOW);
      delay(200);
    }
    message = "Clignotement OK";
  } else if (action == "fade") {
    for(int i = 0; i <= 255; i += 5) {
      analogWrite(BUILTIN_LED_PIN, i);
      delay(10);
    }
    for(int i = 255; i >= 0; i -= 5) {
      analogWrite(BUILTIN_LED_PIN, i);
      delay(10);
    }
    digitalWrite(BUILTIN_LED_PIN, LOW);
    message = "Fade OK";
  } else if (action == "off") {
    digitalWrite(BUILTIN_LED_PIN, LOW);
    builtinLedTested = false;
    message = "LED eteinte";
  } else {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }

  server.send(200, "application/json", "{\"success\":true,\"message\":\"" + message + "\"}");
}

// ========== NEOPIXEL ==========
void handleNeoPixelTest() {
  resetNeoPixelTest();
  testNeoPixel();
  server.send(200, "application/json", "{\"success\":" + String(neopixelAvailable ? "true" : "false") + ",\"result\":\"" + neopixelTestResult + "\"}");
}

void handleNeoPixelPattern() {
  if (!server.hasArg("pattern")) {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }

  String pattern = server.arg("pattern");
  if (!strip) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"NeoPixel non init\"}");
    return;
  }

  String message = "";
  if (pattern == "rainbow") {
    neopixelRainbow();
    message = "Arc-en-ciel OK";
  } else if (pattern == "blink") {
    neopixelBlink(strip->Color(255, 0, 0), 5);
    message = "Blink OK";
  } else if (pattern == "fade") {
    neopixelFade(strip->Color(0, 0, 255));
    message = "Fade OK";
  } else if (pattern == "off") {
    strip->clear();
    strip->show();
    neopixelTested = false;
    message = "Off";
  } else {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }

  server.send(200, "application/json", "{\"success\":true,\"message\":\"" + message + "\"}");
}

void handleNeoPixelColor() {
  if (!server.hasArg("r") || !server.hasArg("g") || !server.hasArg("b") || !strip) {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }

  int r = server.arg("r").toInt();
  int g = server.arg("g").toInt();
  int b = server.arg("b").toInt();

  strip->fill(strip->Color(r, g, b));
  strip->show();
  neopixelTested = false;

  server.send(200, "application/json", "{\"success\":true,\"message\":\"RGB(" + String(r) + "," + String(g) + "," + String(b) + ")\"}");
}

// ========== TFT ==========
void handleTFTTest() {
  resetTFTTest();
  testTFT();
  server.send(200, "application/json", "{\"success\":" + String(tftAvailable ? "true" : "false") +
              ",\"result\":\"" + tftTestResult + "\",\"width\":" + String(tftWidth) + ",\"height\":" + String(tftHeight) + "}");
}

void handleTFTPattern() {
  if (!server.hasArg("pattern")) {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }

  String pattern = server.arg("pattern");
  String message = "";

  if (pattern == "colors") {
    tftTestColors();
    message = "Couleurs OK";
  } else if (pattern == "checkerboard") {
    tftTestCheckerboard();
    message = "Damier OK";
  } else if (pattern == "clear") {
    tftClear();
    tftTested = false;
    message = "Ecran efface";
  } else {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }

  server.send(200, "application/json", "{\"success\":true,\"message\":\"" + message + "\"}");
}

// ========== OLED ==========
void handleOLEDTest() {
  resetOLEDTest();
  testOLED();
  server.send(200, "application/json", "{\"success\":" + String(oledAvailable ? "true" : "false") + ",\"result\":\"" + oledTestResult + "\"}");
}

void handleOLEDMessage() {
  if (!server.hasArg("message")) {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }

  String message = server.arg("message");
  oledShowMessage(message);
  server.send(200, "application/json", "{\"success\":true,\"message\":\"Message affiche\"}");
}

// ========== ADC ==========
void handleADCTest() {
  testADC();
  String json = "{\"readings\":[";
  for (size_t i = 0; i < adcReadings.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"pin\":" + String(adcReadings[i].pin) + ",\"raw\":" + String(adcReadings[i].rawValue) +
            ",\"voltage\":" + String(adcReadings[i].voltage, 2) + "}";
  }
  json += "],\"result\":\"" + adcTestResult + "\"}";
  server.send(200, "application/json", json);
}

// ========== BENCHMARK ==========
void handleBenchmark() {
  unsigned long cpuTime = benchmarkCPU();
  unsigned long memTime = benchmarkMemory();

  diagnosticData.cpuBenchmark = cpuTime;
  diagnosticData.memBenchmark = memTime;

  server.send(200, "application/json", "{\"cpu\":" + String(cpuTime) + ",\"memory\":" + String(memTime) +
              ",\"cpuPerf\":" + String(100000.0 / cpuTime, 2) + ",\"memSpeed\":" + String((10000 * sizeof(int) * 2) / (float)memTime, 2) + "}");
}

// ========== EXPORTS (inchangés) ==========
void handleExportTXT() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  String txt = "========================================\r\n";
  txt += String(T().title) + " " + String(T().version) + "3.0.0\r\n";
  txt += "========================================\r\n\r\n";

  txt += "=== CHIP ===\r\n";
  txt += String(T().model) + ": " + diagnosticData.chipModel + " " + String(T().revision) + diagnosticData.chipRevision + "\r\n";
  txt += "CPU: " + String(diagnosticData.cpuCores) + " " + String(T().cores) + " @ " + String(diagnosticData.cpuFreqMHz) + " MHz\r\n";

  // ... (reste du code export TXT identique)

  server.sendHeader("Content-Disposition", "attachment; filename=esp32_diagnostic_v3.0.0.txt");
  server.send(200, "text/plain; charset=utf-8", txt);
}

void handleExportJSON() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  String json = "{";
  json += "\"version\":\"3.0.0\",";
  json += "\"chip\":{";
  json += "\"model\":\"" + diagnosticData.chipModel + "\",";
  json += "\"revision\":\"" + diagnosticData.chipRevision + "\"";
  // ... (reste identique)
  json += "}";

  server.sendHeader("Content-Disposition", "attachment; filename=esp32_diagnostic_v3.0.0.json");
  server.send(200, "application/json", json);
}

void handleExportCSV() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  String csv = String(T().category) + "," + String(T().parameter) + "," + String(T().value) + "\r\n";
  // ... (reste identique)

  server.sendHeader("Content-Disposition", "attachment; filename=esp32_diagnostic_v3.0.0.csv");
  server.send(200, "text/csv; charset=utf-8", csv);
}

#endif // API_HANDLERS_H
