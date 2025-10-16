/*
 * ============================================================
 * TEST FUNCTIONS - ESP32 Diagnostic System
 * ============================================================
 *
 * Version: 3.0.1
 * Date: October 2025
 * Arduino Core: 3.3.2+
 *
 * Description:
 *   Hardware testing functions for GPIO, I2C, SPI, Memory,
 *   WiFi and System diagnostics. Optimized for Core 3.3.2.
 *
 * ============================================================
 */

#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H

#include <ArduinoJson.h>

// ============================================================
// GPIO TEST
// ============================================================

void testGPIO(JsonArray& gpioArray) {
  // List of commonly available GPIO pins for ESP32-S3
  int testPins[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 21, 38, 39, 40, 41, 42, 47, 48};
  int pinCount = sizeof(testPins) / sizeof(testPins[0]);

  for (int i = 0; i < pinCount; i++) {
    int pin = testPins[i];

    JsonObject pinObj = gpioArray.createNestedObject();
    pinObj["pin"] = pin;

    // Test pin configuration
    pinMode(pin, INPUT);
    delay(1);
    int inputValue = digitalRead(pin);

    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delay(1);
    int highValue = digitalRead(pin);

    digitalWrite(pin, LOW);
    delay(1);
    int lowValue = digitalRead(pin);

    // Restore to input mode
    pinMode(pin, INPUT);

    // Determine status
    bool isWorking = (highValue == HIGH && lowValue == LOW);
    pinObj["status"] = isWorking ? "OK" : "FAIL";
    pinObj["details"] = isWorking ? "Functional" : "Check connection";
  }
}

// ============================================================
// I2C TEST - Enhanced for Core 3.3.2
// ============================================================

void testI2C(JsonArray& i2cArray) {
  Serial.println("[TEST] Starting I2C scan...");

  int deviceCount = 0;

  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      JsonObject device = i2cArray.createNestedObject();
      device["address"] = "0x" + String(address, HEX);
      device["status"] = "Found";

      // Identify common I2C devices
      String deviceName = "Unknown";
      switch(address) {
        case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
          deviceName = "PCF8574/MCP23008";
          break;
        case 0x38: case 0x39: case 0x3A: case 0x3B: case 0x3C: case 0x3D: case 0x3E: case 0x3F:
          deviceName = "PCF8574A/OLED";
          break;
        case 0x40: case 0x41: case 0x42: case 0x43:
          deviceName = "INA219/PCA9685";
          break;
        case 0x48: case 0x49: case 0x4A: case 0x4B:
          deviceName = "ADS1115/TMP102";
          break;
        case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
          deviceName = "EEPROM (24C)";
          break;
        case 0x68: case 0x69:
          deviceName = "MPU6050/DS3231";
          break;
        case 0x76: case 0x77:
          deviceName = "BMP280/BME280";
          break;
      }

      device["device"] = deviceName;
      deviceCount++;

      Serial.print("[I2C] Found device at 0x");
      Serial.print(address, HEX);
      Serial.print(" - ");
      Serial.println(deviceName);
    }
  }

  if (deviceCount == 0) {
    JsonObject noDevice = i2cArray.createNestedObject();
    noDevice["address"] = "N/A";
    noDevice["status"] = "No devices found";
    noDevice["device"] = "N/A";
    Serial.println("[I2C] No devices found");
  } else {
    Serial.print("[I2C] Found ");
    Serial.print(deviceCount);
    Serial.println(" device(s)");
  }
}

// ============================================================
// SPI TEST
// ============================================================

void testSPI(JsonArray& spiArray) {
  JsonObject spi = spiArray.createNestedObject();

  #if defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32S2)
    spi["bus"] = "SPI2 (HSPI)";
    spi["mosi"] = "35";
    spi["miso"] = "37";
    spi["sclk"] = "36";
    spi["status"] = "Available";
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    spi["bus"] = "SPI2";
    spi["mosi"] = "7";
    spi["miso"] = "2";
    spi["sclk"] = "6";
    spi["status"] = "Available";
  #else
    spi["bus"] = "SPI (VSPI)";
    spi["mosi"] = "23";
    spi["miso"] = "19";
    spi["sclk"] = "18";
    spi["status"] = "Available";
  #endif
}

// ============================================================
// MEMORY TEST - Optimized for Core 3.3.2 heap capabilities
// ============================================================

void testMemory(JsonObject& memTest) {
  // Using improved heap_caps API from Core 3.3.2
  uint32_t heapFree = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
  uint32_t heapSize = heap_caps_get_total_size(MALLOC_CAP_INTERNAL);
  uint32_t heapMin = heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL);

  memTest["heap_free"] = formatBytes(heapFree);
  memTest["heap_size"] = formatBytes(heapSize);
  memTest["heap_min_free"] = formatBytes(heapMin);
  memTest["heap_usage_percent"] = (int)((heapSize - heapFree) * 100 / heapSize);

  // Fragmentation analysis (Core 3.3.2 improvement)
  uint32_t largestBlock = heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL);
  memTest["largest_free_block"] = formatBytes(largestBlock);

  float fragmentation = 100.0 - ((float)largestBlock / (float)heapFree * 100.0);
  memTest["fragmentation_percent"] = (int)fragmentation;

  // PSRAM test if available
  if (psramFound()) {
    uint32_t psramFree = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    uint32_t psramSize = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
    uint32_t psramMin = heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM);

    memTest["psram_free"] = formatBytes(psramFree);
    memTest["psram_size"] = formatBytes(psramSize);
    memTest["psram_min_free"] = formatBytes(psramMin);
    memTest["psram_usage_percent"] = (int)((psramSize - psramFree) * 100 / psramSize);
  } else {
    memTest["psram_available"] = false;
  }

  // Memory status
  if (heapFree < 50000) {
    memTest["status"] = "LOW";
    memTest["alert"] = "Warning: Low memory available";
  } else if (heapFree < 100000) {
    memTest["status"] = "MEDIUM";
    memTest["alert"] = "Memory usage is moderate";
  } else {
    memTest["status"] = "OK";
    memTest["alert"] = "Memory is healthy";
  }
}

// ============================================================
// WIFI TEST - Enhanced with Core 3.3.2 improvements
// ============================================================

void testWiFi(JsonObject& wifiTest) {
  wifiTest["connected"] = (WiFi.status() == WL_CONNECTED);

  if (WiFi.status() == WL_CONNECTED) {
    wifiTest["ssid"] = WiFi.SSID();
    wifiTest["ip"] = WiFi.localIP().toString();
    wifiTest["mac"] = WiFi.macAddress();
    wifiTest["rssi"] = WiFi.RSSI();

    // Signal quality assessment
    int rssi = WiFi.RSSI();
    String quality;
    if (rssi > -50) quality = "Excellent";
    else if (rssi > -60) quality = "Good";
    else if (rssi > -70) quality = "Fair";
    else quality = "Weak";

    wifiTest["signal_quality"] = quality;

    // Channel info (Core 3.3.2 enhanced WiFi API)
    wifiTest["channel"] = WiFi.channel();

    // Gateway and DNS (Core 3.3.2 stability improvements)
    wifiTest["gateway"] = WiFi.gatewayIP().toString();
    wifiTest["dns"] = WiFi.dnsIP().toString();
    wifiTest["subnet"] = WiFi.subnetMask().toString();

    wifiTest["status"] = "OK";
  } else {
    wifiTest["status"] = "DISCONNECTED";
    wifiTest["error"] = "Not connected to WiFi";
  }
}

// ============================================================
// SYSTEM TEST - Complete system diagnostics
// ============================================================

void testSystem(JsonObject& sysTest) {
  // Chip information
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);

  sysTest["chip_model"] = getChipModel();
  sysTest["chip_revision"] = chip_info.revision;
  sysTest["cpu_cores"] = chip_info.cores;
  sysTest["cpu_frequency"] = getCpuFrequencyMhz();

  // Flash information
  sysTest["flash_size"] = getFlashSize();
  sysTest["flash_speed"] = ESP.getFlashChipSpeed() / 1000000;

  // Uptime
  sysTest["uptime"] = getUptime();
  sysTest["uptime_ms"] = (unsigned long)millis();

  // Temperature (if available on chip)
  #ifdef CONFIG_IDF_TARGET_ESP32S3
    // ESP32-S3 has internal temperature sensor
    // Note: temperatureRead() may not be available on all boards
    // sysTest["temperature"] = temperatureRead();
  #endif

  // Reset reason
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

  // Core version info
  sysTest["arduino_core"] = ARDUINO_CORE_VERSION;
  sysTest["idf_version"] = esp_get_idf_version();

  // Overall system status
  uint32_t heapFree = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
  bool memoryOK = (heapFree > 50000);
  bool wifiOK = (WiFi.status() == WL_CONNECTED);

  if (memoryOK && wifiOK) {
    sysTest["overall_status"] = "HEALTHY";
  } else if (!memoryOK && !wifiOK) {
    sysTest["overall_status"] = "CRITICAL";
  } else {
    sysTest["overall_status"] = "WARNING";
  }
}

#endif // TEST_FUNCTIONS_H
