/*
 * ESP32 Diagnostic Suite v3.10.0
 * Compatible: ESP32-S3 with TFT ST7789 and GPS
 * Optimized for ESP32 Arduino Core 3.3.3 / PlatformIO
 * Tested board: ESP32-S3 DevKitC-1 N16R8 with PSRAM OPI (Core 3.3.3)
 * Author: morfredus
 * v3.8.01-dev - Modern responsive UI with sticky header
 * v3.8.03-dev - Complete UI redesign with shadow effects and compact header
 * v3.8.04-dev - Compact header, chip type inline, language buttons repositioned
 * v3.8.05-dev - Unified font sizes across header elements to match UI
 * v3.8.06-dev - Premium color palette with enhanced shadow system
 * v3.8.08-dev - Fixed PlatformIO compilation errors (LEDC API, MDNS)
 * v3.9.3 - Major hardware refactoring for TFT and GPS support
 * v3.10.0 - Integration of TFT ST7789 and GPS functionalities
 */

// Configuration file - customize your setup
// Copy include/config-example.h to include/config.h and customize your settings
#include "config.h"


#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>

#if defined(ARDUINO_ARCH_ESP32)
  #include <freertos/FreeRTOS.h>
  #include <freertos/task.h>
  #if defined(__has_include)
    #if __has_include(<ESPmDNS.h>)
      #include <ESPmDNS.h>
      #define DIAGNOSTIC_HAS_MDNS 1
      #define DIAGNOSTIC_USES_ESPMDNS 1
    #elif __has_include(<mdns.h>)
      #include <mdns.h>
      #define DIAGNOSTIC_HAS_MDNS 1
      #define DIAGNOSTIC_USES_IDF_MDNS 1
    #else
      #define DIAGNOSTIC_HAS_MDNS 0
    #endif
  #else
    #include <ESPmDNS.h>
    #define DIAGNOSTIC_HAS_MDNS 1
    #define DIAGNOSTIC_USES_ESPMDNS 1
  #endif
#else
  #define DIAGNOSTIC_HAS_MDNS 0
#endif

#if !defined(DIAGNOSTIC_HAS_MDNS)
  #define DIAGNOSTIC_HAS_MDNS 0
#endif

#if DIAGNOSTIC_HAS_MDNS && defined(DIAGNOSTIC_USES_ESPMDNS)
  #if defined(NO_GLOBAL_MDNS) || defined(NO_GLOBAL_INSTANCES)
    static MDNSResponder diagnosticMDNSResponder;
    #define DIAGNOSTIC_MDNS_SERVER diagnosticMDNSResponder
  #else
    #define DIAGNOSTIC_MDNS_SERVER MDNS
  #endif
#endif
#include <esp_chip_info.h>
#include <esp_err.h>
#include <esp_mac.h>
#include <esp_flash.h>
#include <esp_heap_caps.h>
#include <esp_partition.h>
#include <esp_wifi.h>
#include <esp_task_wdt.h>
#if defined(__has_include)
  #if __has_include(<sdkconfig.h>)
    #include <sdkconfig.h>
    #define DIAGNOSTIC_HAS_SDKCONFIG 1
  #endif
  #if __has_include(<esp_netif.h>)
    #include <esp_netif.h>
    #define DIAGNOSTIC_HAS_ESP_NETIF 1
  #endif
#else
  #include <esp_netif.h>
  #define DIAGNOSTIC_HAS_ESP_NETIF 1
#endif
#if !defined(DIAGNOSTIC_HAS_SDKCONFIG)
  #define DIAGNOSTIC_HAS_SDKCONFIG 0
#endif
#include <soc/soc.h>
#include <soc/rtc.h>
#if defined(__has_include)
  #if __has_include(<soc/soc_caps.h>)
    #include <soc/soc_caps.h>
  #endif
#else
  #include <soc/soc_caps.h>
#endif
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <DHT.h>
#include <SPI.h>
#if ENABLE_TFT_DISPLAY
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#endif
#if ENABLE_GPS
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#endif
#include <vector>
#include <cstring>
#include <string>
#include <initializer_list>
#include "json_helpers.h"

#if ENABLE_GPS
SoftwareSerial SerialGPS(PIN_GPS_RXD, PIN_GPS_TXD);
#endif

// WiFi configuration file - customize your network credentials
// For reference, see include/wifi-config-example.h
// wifi-config.h is excluded from version control via .gitignore
#include "wifi-config.h"

// Dual-language UI strings
#include "languages.h"

// Set default language from config.h
Language currentLanguage = DEFAULT_LANGUAGE;

namespace Texts {
  #define DEFINE_TEXT(identifier, en, fr) const TextField identifier(F(en), F(fr));
  TEXT_RESOURCE_MAP(DEFINE_TEXT)
  #undef DEFINE_TEXT
}
#undef TEXT_RESOURCE_MAP

static String buildActionResponseJson(bool success,
                                      const String& message,
                                      std::initializer_list<JsonFieldSpec> extraFields = {});
String htmlEscape(const String& raw);
String jsonEscape(const char* raw);
inline void sendJsonResponse(int statusCode, std::initializer_list<JsonFieldSpec> fields);
String buildTranslationsJSON();
String buildTranslationsJSON(Language lang);
inline void sendActionResponse(int statusCode,
                               bool success,
                               const String& message,
                               std::initializer_list<JsonFieldSpec> extraFields = {});
inline void sendOperationSuccess(const String& message,
                                 std::initializer_list<JsonFieldSpec> extraFields = {});
inline void sendOperationError(int statusCode,
                               const String& message,
                               std::initializer_list<JsonFieldSpec> extraFields = {});

#if defined(__has_include)
  #if __has_include(<esp_arduino_version.h>)
    #include <esp_arduino_version.h>
  #endif
#endif

#ifndef ESP_ARDUINO_VERSION_VAL
#define ESP_ARDUINO_VERSION_VAL(major, minor, patch) ((major << 16) | (minor << 8) | (patch))
#endif

#ifndef ESP_ARDUINO_VERSION
#if defined(ESP_ARDUINO_VERSION_MAJOR) && defined(ESP_ARDUINO_VERSION_MINOR) && defined(ESP_ARDUINO_VERSION_PATCH)
#define ESP_ARDUINO_VERSION ESP_ARDUINO_VERSION_VAL(ESP_ARDUINO_VERSION_MAJOR, ESP_ARDUINO_VERSION_MINOR, ESP_ARDUINO_VERSION_PATCH)
#else
#define ESP_ARDUINO_VERSION ESP_ARDUINO_VERSION_VAL(0, 0, 0)
#endif
#endif

// ========== CONFIGURATION (from config.h) ==========
// Version is defined in platformio.ini (PROJECT_VERSION), hostname is in config.h
const char* DIAGNOSTIC_VERSION_STR = PROJECT_VERSION;
const char* MDNS_HOSTNAME_STR = DIAGNOSTIC_HOSTNAME;

// HTTPS/HTTP scheme constants
const char* const DIAGNOSTIC_SECURE_SCHEME = "https://";
const char* const DIAGNOSTIC_LEGACY_SCHEME = "http://";

#ifndef DIAGNOSTIC_UNUSED
#define DIAGNOSTIC_UNUSED __attribute__((unused))
#endif

static inline String buildAccessUrl(const String& hostOrAddress,
                                    bool preferSecure = (DIAGNOSTIC_PREFER_SECURE != 0)) {
  if (hostOrAddress.length() == 0) {
    return String();
  }
  const String secureScheme = String(DIAGNOSTIC_SECURE_SCHEME);
  const String legacyScheme = String(DIAGNOSTIC_LEGACY_SCHEME);
  return (preferSecure ? secureScheme : legacyScheme) + hostOrAddress;
}

static inline String getStableAccessHost() {
  return String(DIAGNOSTIC_HOSTNAME) + ".local";
}

String getArduinoCoreVersionString() {
#if defined(ESP_ARDUINO_VERSION_MAJOR) && defined(ESP_ARDUINO_VERSION_MINOR) && defined(ESP_ARDUINO_VERSION_PATCH)
  return String(ESP_ARDUINO_VERSION_MAJOR) + "." + String(ESP_ARDUINO_VERSION_MINOR) + "." + String(ESP_ARDUINO_VERSION_PATCH);
#else
  uint32_t value = ESP_ARDUINO_VERSION;
  int major = (value >> 16) & 0xFF;
  int minor = (value >> 8) & 0xFF;
  int patch = value & 0xFF;
  return String(major) + "." + String(minor) + "." + String(patch);
#endif
}

// ========== HARDWARE PIN CONFIGURATION (from config.h) ==========
// Pins I2C pour OLED (modifiables via web ou config.h)
int I2C_SCL = DEFAULT_I2C_SCL;
int I2C_SDA = DEFAULT_I2C_SDA;

// OLED display settings (from config.h)
uint8_t oledRotation = DEFAULT_OLED_ROTATION;

// Runtime pin variables (initialized from config.h, modifiable via web interface)
int RGB_LED_PIN_R = DEFAULT_RGB_LED_PIN_R;
int RGB_LED_PIN_G = DEFAULT_RGB_LED_PIN_G;
int RGB_LED_PIN_B = DEFAULT_RGB_LED_PIN_B;
int BUZZER_PIN = DEFAULT_BUZZER_PIN;
int DHT_PIN = DEFAULT_DHT_PIN;
uint8_t DHT_SENSOR_TYPE = DEFAULT_DHT_SENSOR_TYPE;
int LIGHT_SENSOR_PIN = DEFAULT_LIGHT_SENSOR_PIN;
int DISTANCE_TRIG_PIN = DEFAULT_DISTANCE_TRIG_PIN;
int DISTANCE_ECHO_PIN = DEFAULT_DISTANCE_ECHO_PIN;
int MOTION_SENSOR_PIN = DEFAULT_MOTION_SENSOR_PIN;

WebServer server(WEB_SERVER_PORT);
WiFiMulti wifiMulti;
#if DIAGNOSTIC_HAS_MDNS
bool mdnsServiceActive = false;
bool wifiPreviouslyConnected = false;
unsigned long lastMDNSAttempt = 0;
bool mdnsLastAttemptFailed = false;
#if defined(DIAGNOSTIC_USES_IDF_MDNS)
bool mdnsResponderInitialized = false;
#endif
#endif

// ========== OBJETS GLOBAUX ==========
#if ENABLE_TFT_DISPLAY
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
TaskHandle_t tft_task_handle = NULL;
SemaphoreHandle_t tftSemaphore;
bool tftReady = false;
#endif

#if ENABLE_GPS
TinyGPSPlus gps;
#endif

DHT dht(DEFAULT_DHT_PIN, DEFAULT_DHT_SENSOR_TYPE);

// NeoPixel (from config.h)
int LED_PIN = DEFAULT_NEOPIXEL_PIN;
int LED_COUNT = DEFAULT_NEOPIXEL_COUNT;
Adafruit_NeoPixel *strip = nullptr;

bool neopixelTested = false;
bool neopixelAvailable = false;
bool neopixelSupported = false;
String neopixelTestResult = String(Texts::not_tested);

// Built-in LED (from config.h)
int BUILTIN_LED_PIN = DEFAULT_BUILTIN_LED_PIN;
bool builtinLedTested = false;
bool builtinLedAvailable = false;
String builtinLedTestResult = String(Texts::not_tested);

bool tftTested = false;
bool tftAvailable = false;
String tftTestResult = String(Texts::not_tested);

// Exécution asynchrone des tests matériels
typedef void (*TestRoutine)();

struct AsyncTestRunner {
  const char* taskName;
  TaskHandle_t taskHandle;
  volatile bool running;
};

struct AsyncTestTaskArgs {
  AsyncTestRunner* runner;
  TestRoutine routine;
};

static void asyncTestTask(void* parameters) {
  AsyncTestTaskArgs* args = static_cast<AsyncTestTaskArgs*>(parameters);
  if (args && args->routine) {
    args->routine();
  }
  if (args) {
    if (args->runner) {
      args->runner->running = false;
      args->runner->taskHandle = nullptr;
    }
    delete args;
  }
  vTaskDelete(nullptr);
}

static bool startAsyncTest(AsyncTestRunner& runner,
                           TestRoutine routine,
                           bool& alreadyRunning,
                           uint32_t stackSize = 4096,
                           UBaseType_t priority = 1) {
  alreadyRunning = runner.running;
  if (runner.running) {
    return false;
  }

  AsyncTestTaskArgs* args = new AsyncTestTaskArgs{&runner, routine};
  runner.running = true;

#if CONFIG_FREERTOS_UNICORE
  const BaseType_t targetCore = tskNO_AFFINITY;
#else
  const BaseType_t targetCore = 1;
#endif

  BaseType_t result = xTaskCreatePinnedToCore(asyncTestTask,
                                              runner.taskName,
                                              stackSize,
                                              args,
                                              priority,
                                              &runner.taskHandle,
                                              targetCore);
  if (result != pdPASS) {
    runner.running = false;
    runner.taskHandle = nullptr;
    delete args;
    alreadyRunning = false;
    return false;
  }

  alreadyRunning = false;
  return true;
}

// Orchestrateurs asynchrones des tests lents
static AsyncTestRunner builtinLedTestRunner = {"BuiltinLEDTest", nullptr, false};
static AsyncTestRunner neopixelTestRunner = {"NeoPixelTest", nullptr, false};
static AsyncTestRunner tftTestRunner = {"TFTTest", nullptr, false};
static AsyncTestRunner rgbLedTestRunner = {"RgbLedTest", nullptr, false};
static AsyncTestRunner buzzerTestRunner = {"BuzzerTest", nullptr, false};

bool runtimeBLE = false;

String adcTestResult = String(Texts::not_tested);
String pwmTestResult = String(Texts::not_tested);
String partitionsInfo;
String spiInfo;
String stressTestResult = String(Texts::not_tested);
// Memory stress telemetry cache
size_t stressAllocationCount = 0;
unsigned long stressDurationMs = 0;

// Résultats de tests des nouveaux capteurs
String rgbLedTestResult = String(Texts::not_tested);
bool rgbLedAvailable = false;

String buzzerTestResult = String(Texts::not_tested);
bool buzzerAvailable = false;

String dhtTestResult = String(Texts::not_tested);
bool dhtAvailable = false;
float dhtTemperature = -999.0;
float dhtHumidity = -999.0;

String lightSensorTestResult = String(Texts::not_tested);
bool lightSensorAvailable = false;
int lightSensorValue = -1;

String distanceSensorTestResult = String(Texts::not_tested);
bool distanceSensorAvailable = false;
float distanceValue = -1.0;

String motionSensorTestResult = String(Texts::not_tested);
bool motionSensorAvailable = false;
bool motionDetected = false;

// GPS Data
#if ENABLE_GPS
bool gpsFix = false;
int gpsSatellites = 0;
float gpsLat = 0.0;
float gpsLon = 0.0;
float gpsAltitude = 0.0;
float gpsSpeed = 0.0;
#endif

// ========== STRUCTURES ==========
struct DiagnosticInfo {
  String chipModel;
  String chipRevision;
  int cpuCores;
  uint32_t cpuFreqMHz;
  uint32_t flashSize;
  uint32_t psramSize;
  String macAddress;
  
  uint32_t heapSize;
  uint32_t freeHeap;
  uint32_t minFreeHeap;
  uint32_t maxAllocHeap;
  
  bool hasWiFi;
  bool hasBT;
  bool hasBLE;
  String wifiSSID;
  int wifiRSSI;
  String ipAddress;
  bool mdnsAvailable;
  bool bluetoothEnabled;
  bool bluetoothAdvertising;
  String bluetoothName;
  String bluetoothAddress;

  String gpioList;
  int totalGPIO;
  
  String sdkVersion;
  String idfVersion;
  unsigned long uptime;
  float temperature;
  
  bool neopixelTested;
  bool neopixelAvailable;
  String neopixelResult;
  
  unsigned long cpuBenchmark;
  unsigned long memBenchmark;
  
  String i2cDevices;
  int i2cCount;
  
  bool tftTested;
  bool tftAvailable;
  String tftResult;
} diagnosticData;

// Include web interface after DiagnosticInfo definition
#include "web_interface.h"

struct DetailedMemoryInfo {
  uint32_t flashSizeReal;
  uint32_t flashSizeChip;
  
  uint32_t psramTotal;
  uint32_t psramFree;
  uint32_t psramUsed;
  uint32_t psramLargestBlock;
  bool psramAvailable;
  bool psramConfigured;
  bool psramBoardSupported;
  const char* psramType;

  uint32_t sramTotal;
  uint32_t sramFree;
  uint32_t sramUsed;
  uint32_t sramLargestBlock;
  
  bool sramTestPassed;
  bool psramTestPassed;
  float fragmentationPercent;
  String memoryStatus;
};

DetailedMemoryInfo detailedMemory;

struct GPIOTestResult {
  int pin;
  bool tested;
  bool working;
  String mode;
  String notes;
};

std::vector<GPIOTestResult> gpioResults;

struct WiFiNetwork {
  String ssid;
  int rssi;
  int channel;
  String encryption;
  String bssid;
  int freqMHz;
  String band;
  String bandwidth;
  String phyModes;
};

std::vector<WiFiNetwork> wifiNetworks;

struct ADCReading {
  int pin;
  int rawValue;
  float voltage;
};

std::vector<ADCReading> adcReadings;

#define HISTORY_SIZE 60
float heapHistory[HISTORY_SIZE];
float tempHistory[HISTORY_SIZE];
int historyIndex = 0;

String getStableAccessURL() {
  return buildAccessUrl(getStableAccessHost());
}

void configureNetworkHostname() {
  if (!WiFi.setHostname(DIAGNOSTIC_HOSTNAME)) {
    Serial.printf("[WiFi] Impossible de définir le nom d'hôte %s\r\n", DIAGNOSTIC_HOSTNAME);
  } else {
    Serial.printf("[WiFi] Nom d'hôte défini : %s.local\r\n", DIAGNOSTIC_HOSTNAME);
  }
#if defined(ARDUINO_ARCH_ESP32)
  #if defined(DIAGNOSTIC_HAS_ESP_NETIF)
    esp_netif_t* staNetif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (staNetif != nullptr) {
      esp_err_t netifStatus = esp_netif_set_hostname(staNetif, DIAGNOSTIC_HOSTNAME);
      if (netifStatus != ESP_OK) {
        Serial.printf("[WiFi] Échec assignation hostname netif (0x%X)\r\n", netifStatus);
      }
    } else {
      Serial.println("[WiFi] Interface STA introuvable pour l'assignation du hostname");
    }
  #elif defined(DIAGNOSTIC_HAS_TCPIP_ADAPTER)
    err_t adapterStatus = tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, DIAGNOSTIC_HOSTNAME);
    if (adapterStatus != ERR_OK) {
      Serial.printf("[WiFi] Échec assignation hostname TCP/IP (%d)\r\n", static_cast<int>(adapterStatus));
    }
  #endif
#endif
}

void stopMDNSService(const char* reason) {
#if DIAGNOSTIC_HAS_MDNS
  if (!mdnsServiceActive) {
    return;
  }
#if defined(DIAGNOSTIC_USES_ESPMDNS)
  DIAGNOSTIC_MDNS_SERVER.end();
#elif defined(DIAGNOSTIC_USES_IDF_MDNS)
  if (mdnsResponderInitialized) {
    mdns_free();
    mdnsResponderInitialized = false;
  }
#endif
  mdnsServiceActive = false;
  if (reason != nullptr) {
    Serial.printf("[mDNS] Service arrêté (%s)\r\n", reason);
  } else {
    Serial.println("[mDNS] Service arrêté");
  }
#else
  (void) reason;
#endif
}

bool startMDNSService(bool verbose) {
#if DIAGNOSTIC_HAS_MDNS
  if (!WiFi.isConnected()) {
    return false;
  }
  if (mdnsServiceActive) {
    return true;
  }

  unsigned long now = millis();
  if (!verbose && (now - lastMDNSAttempt < 5000)) {
    return false;
  }

  lastMDNSAttempt = now;
#if defined(DIAGNOSTIC_USES_ESPMDNS)
  if (!DIAGNOSTIC_MDNS_SERVER.begin(DIAGNOSTIC_HOSTNAME)) {
    if (verbose || !mdnsLastAttemptFailed) {
      Serial.println("[mDNS] Échec du démarrage - nouvel essai dans 5s");
    }
    mdnsLastAttemptFailed = true;
    return false;
  }

  DIAGNOSTIC_MDNS_SERVER.addService("http", "tcp", 80);
#elif defined(DIAGNOSTIC_USES_IDF_MDNS)
  esp_err_t mdnsInitStatus = mdns_init();
  if (mdnsInitStatus != ESP_OK) {
    if (verbose || !mdnsLastAttemptFailed) {
      Serial.printf("[mDNS] Initialisation IDF échouée: 0x%02X\r\n", mdnsInitStatus);
    }
    mdnsLastAttemptFailed = true;
    return false;
  }

  mdnsResponderInitialized = true;

  esp_err_t hostStatus = mdns_hostname_set(DIAGNOSTIC_HOSTNAME);
  if (hostStatus != ESP_OK) {
    if (verbose || !mdnsLastAttemptFailed) {
      Serial.printf("[mDNS] Attribution du nom d'hôte échouée: 0x%02X\r\n", hostStatus);
    }
    mdns_free();
    mdnsResponderInitialized = false;
    mdnsLastAttemptFailed = true;
    return false;
  }

  mdns_instance_name_set("ESP32 Diagnostic");
  esp_err_t serviceStatus = mdns_service_add("ESP32 Diagnostic", "_http", "_tcp", 80, nullptr, 0);
  if (serviceStatus != ESP_OK) {
    if (verbose || !mdnsLastAttemptFailed) {
      Serial.printf("[mDNS] Publication du service échouée: 0x%02X\r\n", serviceStatus);
    }
    mdns_free();
    mdnsResponderInitialized = false;
    mdnsLastAttemptFailed = true;
    return false;
  }
  mdns_service_txt_item_set("_http", "_tcp", "path", "/");
#endif
  mdnsServiceActive = true;
  mdnsLastAttemptFailed = false;
  Serial.printf("[mDNS] Service actif sur %s\r\n", getStableAccessURL().c_str());
  return true;
#else
  (void) verbose;
  return false;
#endif
}

void maintainNetworkServices() {
#if DIAGNOSTIC_HAS_MDNS
  bool wifiConnectedNow = (WiFi.status() == WL_CONNECTED);
  if (wifiConnectedNow) {
    if (!wifiPreviouslyConnected) {
      startMDNSService(true);
    } else {
      startMDNSService(false);
    }
  } else if (mdnsServiceActive) {
    stopMDNSService("WiFi déconnecté");
  }
  wifiPreviouslyConnected = wifiConnectedNow;
#endif
}

// ========== DÉTECTION MODÈLE ==========
String detectChipModel() {
  #ifdef CONFIG_IDF_TARGET_ESP32
    return "ESP32";
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
    return "ESP32-S2";
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    return "ESP32-S3";
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    return "ESP32-C3";
  #elif defined(CONFIG_IDF_TARGET_ESP32C6)
    return "ESP32-C6";
  #elif defined(CONFIG_IDF_TARGET_ESP32H2)
    return "ESP32-H2";
  #else
    return "ESP32";
  #endif
}

// ========== FONCTIONS UTILITAIRES ==========
String getChipFeatures() {
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  String features;

  if (chip_info.features & CHIP_FEATURE_WIFI_BGN) features += "WiFi 2.4GHz, ";
  if (chip_info.features & CHIP_FEATURE_BT) features += "Bluetooth Classic, ";
  if (chip_info.features & CHIP_FEATURE_BLE) features += "Bluetooth LE, ";
  
  if (features.length() > 0) {
    features = features.substring(0, features.length() - 2);
  } else {
    features = Texts::none.str();
  }
  
  return features;
}

String getFlashType() {
  #ifdef CONFIG_ESPTOOLPY_FLASHMODE_QIO
    return "QIO";
  #elif defined(CONFIG_ESPTOOLPY_FLASHMODE_QOUT)
    return "QOUT";
  #elif defined(CONFIG_ESPTOOLPY_FLASHMODE_DIO)
    return "DIO";
  #elif defined(CONFIG_ESPTOOLPY_FLASHMODE_DOUT)
    return "DOUT";
  #else
    return Texts::unknown.str();
  #endif
}

String getFlashSpeed() {
  #ifdef CONFIG_ESPTOOLPY_FLASHFREQ_80M
    return "80 MHz";
  #elif defined(CONFIG_ESPTOOLPY_FLASHFREQ_40M)
    return "40 MHz";
  #elif defined(CONFIG_ESPTOOLPY_FLASHFREQ_26M)
    return "26 MHz";
  #elif defined(CONFIG_ESPTOOLPY_FLASHFREQ_20M)
    return "20 MHz";
  #else
    return Texts::unknown.str();
  #endif
}

const char* getResetReasonKey() {
  esp_reset_reason_t reason = esp_reset_reason();
  switch (reason) {
    case ESP_RST_POWERON: return "poweron";
    case ESP_RST_SW: return "software_reset";
    case ESP_RST_DEEPSLEEP: return "deepsleep_exit";
    case ESP_RST_BROWNOUT: return "brownout";
    default: return "other";
  }
}

String getResetReason() {
  esp_reset_reason_t reason = esp_reset_reason();
  switch (reason) {
    case ESP_RST_POWERON: return Texts::poweron.str();
    case ESP_RST_SW: return Texts::software_reset.str();
    case ESP_RST_DEEPSLEEP: return Texts::deepsleep_exit.str();
    case ESP_RST_BROWNOUT: return Texts::brownout.str();
    default: return Texts::other.str();
  }
}

String formatUptime(unsigned long days, unsigned long hours, unsigned long minutes) {
  String formatted;
  if (days > 0) {
    formatted += String(days) + " " + String(Texts::days);
  }
  if (hours > 0 || formatted.length() > 0) {
    if (formatted.length() > 0) {
      formatted += " ";
    }
    formatted += String(hours) + " " + String(Texts::hours);
  }
  if (minutes > 0 || formatted.length() == 0) {
    if (formatted.length() > 0) {
      formatted += " ";
    }
    formatted += String(minutes) + " " + String(Texts::minutes);
  }
  return formatted;
}

String getMemoryStatus() {
  float heapUsagePercent = ((float)(diagnosticData.heapSize - diagnosticData.freeHeap) / diagnosticData.heapSize) * 100;
  if (heapUsagePercent < 50) return Texts::excellent.str();
  else if (heapUsagePercent < 70) return Texts::good.str();
  else if (heapUsagePercent < 85) return Texts::warning.str();
  else return Texts::critical.str();
}

const char* getWiFiSignalQualityKey() {
  if (diagnosticData.wifiRSSI >= -50) return "excellent";
  else if (diagnosticData.wifiRSSI >= -60) return "very_good";
  else if (diagnosticData.wifiRSSI >= -70) return "good";
  else if (diagnosticData.wifiRSSI >= -80) return "weak";
  else return "very_weak";
}

String getWiFiSignalQuality() {
  const char* key = getWiFiSignalQualityKey();
  if (key == nullptr) {
    return Texts::unknown.str();
  }
  if (strcmp(key, "excellent") == 0) return Texts::excellent.str();
  if (strcmp(key, "very_good") == 0) return Texts::very_good.str();
  if (strcmp(key, "good") == 0) return Texts::good.str();
  if (strcmp(key, "weak") == 0) return Texts::weak.str();
  if (strcmp(key, "very_weak") == 0) return Texts::very_weak.str();
  return Texts::unknown.str();
}

// Use translation keys for WiFi auth modes
String wifiAuthModeToString(wifi_auth_mode_t mode) {
  switch (mode) {
    case WIFI_AUTH_OPEN: return Texts::wifi_open_auth.str();
#ifdef WIFI_AUTH_WEP
    case WIFI_AUTH_WEP: return "WEP";
#endif
    case WIFI_AUTH_WPA_PSK: return "WPA-PSK";
    case WIFI_AUTH_WPA2_PSK: return "WPA2-PSK";
    case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
#ifdef WIFI_AUTH_WPA2_ENTERPRISE
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-ENT";
#endif
#ifdef WIFI_AUTH_WPA3_PSK
    case WIFI_AUTH_WPA3_PSK: return "WPA3-PSK";
#endif
#ifdef WIFI_AUTH_WPA2_WPA3_PSK
    case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2/WPA3";
#endif
#ifdef WIFI_AUTH_WAPI_PSK
    case WIFI_AUTH_WAPI_PSK: return "WAPI-PSK";
#endif
#ifdef WIFI_AUTH_OWE
    case WIFI_AUTH_OWE: return "OWE";
#endif
    default: return Texts::unknown.str();
  }
}

#if defined(WIFI_CIPHER_TYPE_NONE)
String wifiCipherToString(wifi_cipher_type_t cipher) {
  switch (cipher) {
    case WIFI_CIPHER_TYPE_NONE: return "None";
    case WIFI_CIPHER_TYPE_WEP40: return "WEP40";
    case WIFI_CIPHER_TYPE_WEP104: return "WEP104";
    case WIFI_CIPHER_TYPE_TKIP: return "TKIP";
    case WIFI_CIPHER_TYPE_CCMP: return "CCMP";
    case WIFI_CIPHER_TYPE_TKIP_CCMP: return "TKIP/CCMP";
#ifdef WIFI_CIPHER_TYPE_GCMP
    case WIFI_CIPHER_TYPE_GCMP: return "GCMP";
#endif
#ifdef WIFI_CIPHER_TYPE_GCMP256
    case WIFI_CIPHER_TYPE_GCMP256: return "GCMP256";
#endif
#ifdef WIFI_CIPHER_TYPE_AES_CMAC128
    case WIFI_CIPHER_TYPE_AES_CMAC128: return "CMAC128";
#endif
#ifdef WIFI_CIPHER_TYPE_SMS4
    case WIFI_CIPHER_TYPE_SMS4: return "SMS4";
#endif
    default: return "-";
  }
}
#endif

String wifiBandwidthToString(wifi_second_chan_t secondary) {
  switch (secondary) {
    case WIFI_SECOND_CHAN_NONE: return "20 MHz";
    case WIFI_SECOND_CHAN_ABOVE:
    case WIFI_SECOND_CHAN_BELOW: return "40 MHz";
    default: return "Auto";
  }
}

String wifiChannelToBand(int channel) {
  if (channel <= 0) return "?";
  if (channel <= 14) return "2.4 GHz";
  if (channel >= 180) return "6 GHz";
  return "5 GHz";
}

int wifiChannelToFrequency(int channel) {
  if (channel <= 0) return 0;
  if (channel <= 14) return 2407 + channel * 5;
  if (channel >= 180) return 5950 + channel * 5;
  return 5000 + channel * 5;
}

String wifiPhyModesToString(const wifi_ap_record_t& record) {
  String modes;
  if (record.phy_11b) {
    if (modes.length()) modes += "/";
    modes += "11b";
  }
  if (record.phy_11g) {
    if (modes.length()) modes += "/";
    modes += "11g";
  }
  if (record.phy_11n) {
    if (modes.length()) modes += "/";
    modes += "11n";
  }
#ifdef CONFIG_ESP_WIFI_80211AC_SUPPORT
  if (record.phy_11ac) {
    if (modes.length()) modes += "/";
    modes += "11ac";
  }
#endif
#ifdef CONFIG_ESP_WIFI_80211AX_SUPPORT
  if (record.phy_11ax) {
    if (modes.length()) modes += "/";
    modes += "11ax";
  }
#endif
#ifdef CONFIG_ESP_WIFI_80211BE_SUPPORT
  if (record.phy_11be) {
    if (modes.length()) modes += "/";
    modes += "11be";
  }
#endif
  if (record.phy_lr) {
    if (modes.length()) modes += "/";
    modes += "LR";
  }
  if (modes.length() == 0) {
    modes = "-";
  }
  return modes;
}

String getGPIOList() {
  String gpioList;

  #ifdef CONFIG_IDF_TARGET_ESP32
    int gpios[] = {0,2,4,5,12,13,14,15,16,17,18,19,21,22,23,25,26,27,32,33};
    int numGPIO = 20;
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
    int gpios[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
    int numGPIO = 22;
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    int gpios[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
    int numGPIO = 22;
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    int gpios[] = {0,1,2,3,4,5,6,7,8,9,10};
    int numGPIO = 11;
  #else
    int gpios[] = {0,2,4,5};
    int numGPIO = 4;
  #endif
  
  for (int i = 0; i < numGPIO; i++) {
    gpioList += String(gpios[i]);
    if (i < numGPIO - 1) gpioList += ", ";
  }
  
  return gpioList;
}

int countGPIO() {
  #ifdef CONFIG_IDF_TARGET_ESP32
    return 26;
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
    return 37;
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    return 39;
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    return 15;
  #else
    return 20;
  #endif
}

// ========== MÉMOIRE DÉTAILLÉE ==========
bool testSRAMQuick() {
  const size_t testSize = 1024;
  uint8_t* testBuffer = (uint8_t*)malloc(testSize);
  if (!testBuffer) return false;
  
  memset(testBuffer, 0xAA, testSize);
  bool ok = (testBuffer[0] == 0xAA && testBuffer[testSize-1] == 0xAA);
  free(testBuffer);
  return ok;
}

bool testPSRAMQuick() {
  if (ESP.getPsramSize() == 0) return false;
  
  const size_t testSize = 1024;
  uint8_t* testBuffer = (uint8_t*)heap_caps_malloc(testSize, MALLOC_CAP_SPIRAM);
  if (!testBuffer) return false;
  
  memset(testBuffer, 0x55, testSize);
  bool ok = (testBuffer[0] == 0x55 && testBuffer[testSize-1] == 0x55);
  heap_caps_free(testBuffer);
  return ok;
}

void collectDetailedMemory() {
  uint32_t flashSizeReal;
  esp_flash_get_size(NULL, &flashSizeReal);
  
  detailedMemory.flashSizeChip = flashSizeReal;
  detailedMemory.flashSizeReal = ESP.getFlashChipSize();
  
  detailedMemory.psramTotal = ESP.getPsramSize();
  detailedMemory.psramAvailable = (detailedMemory.psramTotal > 0);

  detailedMemory.psramConfigured = false;
  detailedMemory.psramBoardSupported = false;
  detailedMemory.psramType = "PSRAM";

  #if defined(CONFIG_SPIRAM)
    detailedMemory.psramConfigured = true;
    detailedMemory.psramBoardSupported = true;
  #endif
  #if defined(CONFIG_SPIRAM_SUPPORT)
    detailedMemory.psramConfigured = true;
    detailedMemory.psramBoardSupported = true;
  #endif
  #if defined(BOARD_HAS_PSRAM)
    detailedMemory.psramConfigured = true;
    detailedMemory.psramBoardSupported = true;
  #endif
  #if defined(CONFIG_SPIRAM_MODE_OCT)
    detailedMemory.psramConfigured = true;
    detailedMemory.psramBoardSupported = true;
    detailedMemory.psramType = "OPI";
  #endif
  #if defined(CONFIG_SPIRAM_MODE_QUAD)
    detailedMemory.psramConfigured = true;
    detailedMemory.psramBoardSupported = true;
    detailedMemory.psramType = "QSPI";
  #endif

  if (detailedMemory.psramTotal == 0) {
    detailedMemory.psramConfigured = false;
  }
  
  if (detailedMemory.psramAvailable) {
    detailedMemory.psramFree = ESP.getFreePsram();
    detailedMemory.psramUsed = detailedMemory.psramTotal - detailedMemory.psramFree;
    
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
    detailedMemory.psramLargestBlock = info.largest_free_block;
  } else {
    detailedMemory.psramFree = 0;
    detailedMemory.psramUsed = 0;
    detailedMemory.psramLargestBlock = 0;
  }
  
  multi_heap_info_t infoInternal;
  heap_caps_get_info(&infoInternal, MALLOC_CAP_INTERNAL);
  detailedMemory.sramTotal = infoInternal.total_free_bytes + infoInternal.total_allocated_bytes;
  detailedMemory.sramFree = infoInternal.total_free_bytes;
  detailedMemory.sramUsed = infoInternal.total_allocated_bytes;
  detailedMemory.sramLargestBlock = infoInternal.largest_free_block;
  
  size_t largestBlock = ESP.getMaxAllocHeap();
  size_t freeHeap = ESP.getFreeHeap();
  if (freeHeap > 0) {
    detailedMemory.fragmentationPercent = 100.0 - (100.0 * largestBlock / freeHeap);
  } else {
    detailedMemory.fragmentationPercent = 0;
  }
  
  detailedMemory.sramTestPassed = testSRAMQuick();
  detailedMemory.psramTestPassed = testPSRAMQuick();
  
  if (detailedMemory.fragmentationPercent < 20) {
    detailedMemory.memoryStatus = Texts::excellent.str();
  } else if (detailedMemory.fragmentationPercent < 40) {
    detailedMemory.memoryStatus = Texts::good.str();
  } else if (detailedMemory.fragmentationPercent < 60) {
    detailedMemory.memoryStatus = "Moyen"; // Pas traduit (statut technique)
  } else {
    detailedMemory.memoryStatus = Texts::critical.str();
  }
}

void printPSRAMDiagnostic() {
  Serial.println("\r\n=== DIAGNOSTIC PSRAM DETAILLE ===");
  Serial.printf("ESP.getPsramSize(): %u octets (%.2f MB)\r\n", 
                ESP.getPsramSize(), ESP.getPsramSize() / 1048576.0);
  
  Serial.println("\r\nFlags de compilation detectes (indication du type supporte par la carte):");
  bool anyFlag = false;
  String psramType = detailedMemory.psramType ? detailedMemory.psramType : Texts::unknown.str();
  
  #ifdef CONFIG_SPIRAM
    Serial.println("  ✓ CONFIG_SPIRAM");
    anyFlag = true;
  #endif
  #ifdef CONFIG_SPIRAM_SUPPORT
    Serial.println("  ✓ CONFIG_SPIRAM_SUPPORT");
    anyFlag = true;
  #endif
  #ifdef BOARD_HAS_PSRAM
    Serial.println("  ✓ BOARD_HAS_PSRAM");
    anyFlag = true;
  #endif
  #ifdef CONFIG_SPIRAM_MODE_OCT
    Serial.println("  ✓ CONFIG_SPIRAM_MODE_OCT (OPI) - Type supporte par la carte");
    psramType = "OPI";
    anyFlag = true;
  #endif
  #ifdef CONFIG_SPIRAM_MODE_QUAD
    Serial.println("  ✓ CONFIG_SPIRAM_MODE_QUAD (QSPI) - Type supporte par la carte");
    psramType = "QSPI";
    anyFlag = true;
  #endif
  
  if (!anyFlag) {
    Serial.println("  ✗ Aucun flag PSRAM detecte");
  }
  
  Serial.println("\r\nResultat logique:");
  Serial.printf("  psramTotal = %u octets\r\n", detailedMemory.psramTotal);
  Serial.printf("  psramConfigured = %s\r\n", detailedMemory.psramConfigured ? "TRUE" : "FALSE");
  Serial.printf("  psramAvailable = %s\r\n", detailedMemory.psramAvailable ? "TRUE" : "FALSE");
  
  Serial.println("\r\nConclusion:");
  if (ESP.getPsramSize() > 0) {
    Serial.printf("  → PSRAM fonctionnelle! Type: %s\r\n", psramType.c_str());
  } else if (anyFlag) {
    Serial.printf("  → Carte compatible PSRAM %s mais DESACTIVEE dans IDE\r\n", psramType.c_str());
    Serial.println("  → Pour activer: Tools → PSRAM → OPI PSRAM (ou QSPI)");
  } else {
    Serial.println("  → Carte sans support PSRAM");
  }
  Serial.println("=====================================\r\n");
}

// ========== BENCHMARKS ==========
unsigned long benchmarkCPU() {
  Serial.println("\r\n=== BENCHMARK CPU ===");
  unsigned long start = micros();
  
  volatile float result = 0;
  for (int i = 0; i < 100000; i++) {
    result += sqrt(i) * sin(i);
  }

  unsigned long duration = micros() - start;
  Serial.printf("CPU: %lu us (%.2f MFLOPS)\r\n", duration, 100000.0 / duration);
  return duration;
}

unsigned long benchmarkMemory() {
  Serial.println("=== BENCHMARK MEMOIRE ===");
  unsigned long start = micros();

  const int size = 10000;
  int* testArray = (int*)malloc(size * sizeof(int));
  if (!testArray) return 0;

  for (int i = 0; i < size; i++) testArray[i] = i;
  volatile int sum = 0;
  for (int i = 0; i < size; i++) sum += testArray[i];
  
  free(testArray);
  unsigned long duration = micros() - start;
  Serial.printf("Memory: %lu us\r\n", duration);
  return duration;
}

// ========== SCAN I2C ==========
void ensureI2CBusConfigured() {
  Wire.end();
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 3, 0)
  Wire.setPins(I2C_SDA, I2C_SCL);
  Wire.begin();
#else
  Wire.begin(I2C_SDA, I2C_SCL);
#endif
  Wire.setClock(400000);
}

void scanI2C() {
  if (!ENABLE_I2C_SCAN) return;

  Serial.println("\r\n=== SCAN I2C ===");
  ensureI2CBusConfigured();
  Serial.printf("I2C: SDA=%d, SCL=%d\r\n", I2C_SDA, I2C_SCL);
  
  diagnosticData.i2cDevices = "";
  diagnosticData.i2cCount = 0;
  
  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      char addr[6];
      sprintf(addr, "0x%02X", address);
      if (diagnosticData.i2cCount > 0) diagnosticData.i2cDevices += ", ";
      diagnosticData.i2cDevices += String(addr);
      diagnosticData.i2cCount++;
    }
  }
  
  if (diagnosticData.i2cCount == 0) {
    diagnosticData.i2cDevices = "Aucun";
  }
  Serial.printf("I2C: %d peripherique(s)\r\n", diagnosticData.i2cCount);
}

// ========== SCAN WIFI ==========
void scanWiFiNetworks() {
  Serial.println("\r\n=== SCAN WIFI ===");
  wifiNetworks.clear();

#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 3, 0)
  int n = WiFi.scanNetworks(false, true, true);
#else
  int n = WiFi.scanNetworks();
#endif

  if (n < 0) {
    Serial.println("WiFi: scan en echec");
    return;
  }

  for (int i = 0; i < n; i++) {
    WiFiNetwork net;

#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 3, 0)
    wifi_ap_record_t* info = static_cast<wifi_ap_record_t*>(WiFi.getScanInfoByIndex(i));
    if (info != nullptr) {
      net.ssid = String(reinterpret_cast<const char*>(info->ssid));
      net.rssi = info->rssi;
      net.channel = info->primary;

      char bssidStr[18];
      sprintf(bssidStr, "%02X:%02X:%02X:%02X:%02X:%02X",
              info->bssid[0], info->bssid[1], info->bssid[2], info->bssid[3], info->bssid[4], info->bssid[5]);
      net.bssid = String(bssidStr);

      net.encryption = wifiAuthModeToString(info->authmode);
#if defined(WIFI_CIPHER_TYPE_NONE)
      String pairwise = wifiCipherToString(info->pairwise_cipher);
      String group = wifiCipherToString(info->group_cipher);
      if ((pairwise != "-" && pairwise != "None") || (group != "-" && group != "None")) {
        net.encryption += " (" + pairwise + "/" + group + ")";
      }
#endif
      net.bandwidth = wifiBandwidthToString(info->second);
      net.band = wifiChannelToBand(info->primary);
      net.freqMHz = wifiChannelToFrequency(info->primary);
      net.phyModes = wifiPhyModesToString(*info);
    } else
#endif
    {
      net.ssid = WiFi.SSID(i);
      net.rssi = WiFi.RSSI(i);
      net.channel = WiFi.channel(i);

      uint8_t* bssid = WiFi.BSSID(i);
      char bssidFallback[18];
      sprintf(bssidFallback, "%02X:%02X:%02X:%02X:%02X:%02X",
              bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
      net.bssid = String(bssidFallback);

      wifi_auth_mode_t auth = WiFi.encryptionType(i);
      net.encryption = wifiAuthModeToString(auth);
      net.band = wifiChannelToBand(net.channel);
      net.bandwidth = "-";
      net.freqMHz = wifiChannelToFrequency(net.channel);
      net.phyModes = "-";
    }

    if (net.bandwidth.length() == 0) net.bandwidth = "-";
    wifiNetworks.push_back(net);
  }

#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 3, 0)
  WiFi.scanDelete();
#endif

  Serial.printf("WiFi: %d reseaux trouves\r\n", n);
}

// ========== TEST GPIO ==========
bool testSingleGPIO(int pin) {
  if (pin == 1 || pin == 3) return false;
  if (pin >= 6 && pin <= 11) return false;
  
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delayMicroseconds(250);
  bool highOk = digitalRead(pin) == HIGH;

  digitalWrite(pin, LOW);
  delayMicroseconds(250);
  bool lowOk = digitalRead(pin) == LOW;
  
  pinMode(pin, INPUT);
  return highOk && lowOk;
}

void testAllGPIOs() {
  Serial.println("\r\n=== TEST GPIO ===");
  gpioResults.clear();
  
  #ifdef CONFIG_IDF_TARGET_ESP32
    int gpios[] = {0,2,4,5,12,13,14,15,16,17,18,19,21,22,23,25,26,27,32,33};
    int numGPIO = 20;
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    int gpios[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
    int numGPIO = 22;
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    int gpios[] = {0,1,2,3,4,5,6,7,8,9,10};
    int numGPIO = 11;
  #else
    int gpios[] = {0,2,4,5};
    int numGPIO = 4;
  #endif
  
  for (int i = 0; i < numGPIO; i++) {
    GPIOTestResult result;
    result.pin = gpios[i];
    result.tested = true;
    result.working = testSingleGPIO(gpios[i]);
    result.mode = "Digital I/O";
    result.notes = result.working ? String(Texts::ok) : String(Texts::fail);
    gpioResults.push_back(result);
  }
  Serial.printf("GPIO: %d testes\r\n", numGPIO);
}

// ========== LED INTÉGRÉE ==========
void detectBuiltinLED() {
  String chipModel = detectChipModel();
  
  #ifdef LED_BUILTIN
    BUILTIN_LED_PIN = LED_BUILTIN;
  #else
    if (chipModel == "ESP32-S3") BUILTIN_LED_PIN = 2;
    else if (chipModel == "ESP32-C3") BUILTIN_LED_PIN = 8;
    else if (chipModel == "ESP32-S2") BUILTIN_LED_PIN = 15;
    else BUILTIN_LED_PIN = 2;
  #endif
  
  builtinLedTestResult = String(Texts::gpio) + " " + String(BUILTIN_LED_PIN) + " - " + String(Texts::not_tested);
  Serial.printf("LED integree: GPIO %d\r\n", BUILTIN_LED_PIN);
}

void testBuiltinLED() {
  if (BUILTIN_LED_PIN == -1) return;
  
  Serial.println("\r\n=== TEST LED ===");
  pinMode(BUILTIN_LED_PIN, OUTPUT);

  for (int i = 0; i < 5; i++) {
    digitalWrite(BUILTIN_LED_PIN, HIGH);
    delay(80);
    digitalWrite(BUILTIN_LED_PIN, LOW);
    delay(80);
  }

  for (int i = 0; i <= 255; i += 51) {
    analogWrite(BUILTIN_LED_PIN, i);
    delay(25);
    yield();
  }
  for (int i = 255; i >= 0; i -= 51) {
    analogWrite(BUILTIN_LED_PIN, i);
    delay(25);
    yield();
  }
  
  digitalWrite(BUILTIN_LED_PIN, LOW);
  builtinLedAvailable = true;
  builtinLedTestResult = String(Texts::test) + " " + String(Texts::ok) + " - GPIO " + String(BUILTIN_LED_PIN);
  builtinLedTested = true;
  Serial.println("LED: OK");
}

void resetBuiltinLEDTest() {
  builtinLedTested = false;
  builtinLedAvailable = false;
  if (BUILTIN_LED_PIN != -1) digitalWrite(BUILTIN_LED_PIN, LOW);
}

// ========== NEOPIXEL ==========
void detectNeoPixelSupport() {
  String chipModel = detectChipModel();
  
  if (CUSTOM_LED_PIN != -1) {
    LED_PIN = CUSTOM_LED_PIN;
    neopixelSupported = true;
  }
  else if (chipModel == "ESP32-S3") {
    LED_PIN = 48;
    neopixelSupported = true;
  } 
  else if (chipModel == "ESP32-C3") {
    LED_PIN = 8;
    neopixelSupported = true;
  }
  else {
    LED_PIN = 2;
    neopixelSupported = false;
  }
  
  if (strip != nullptr) delete strip;
  strip = new Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
  neopixelTestResult = String(Texts::gpio) + " " + String(LED_PIN) + " - " + String(Texts::not_tested);
  Serial.printf("NeoPixel: GPIO %d\r\n", LED_PIN);
}

void testNeoPixel() {
  if (!strip) return;

  Serial.println("\r\n=== TEST NEOPIXEL ===");
  strip->begin();
  strip->clear();
  strip->show();

  strip->setPixelColor(0, strip->Color(255, 0, 0));
  strip->show();
  delay(160);

  strip->setPixelColor(0, strip->Color(0, 255, 0));
  strip->show();
  delay(160);

  strip->setPixelColor(0, strip->Color(0, 0, 255));
  strip->show();
  delay(160);

  for (int i = 0; i < 256; i += 64) {
    strip->setPixelColor(0, strip->gamma32(strip->ColorHSV(i * 256)));
    strip->show();
    delay(28);
    yield();
  }

  strip->clear();
  strip->show();

  neopixelAvailable = true;
  neopixelTestResult = String(Texts::test) + " " + String(Texts::ok) + " - GPIO " + String(LED_PIN);
  neopixelTested = true;
  Serial.println("NeoPixel: OK");
}

void resetNeoPixelTest() {
  neopixelTested = false;
  neopixelAvailable = false;
  if (strip) {
    strip->clear();
    strip->show();
  }
}

void neopixelRainbow() {
  if (!strip) return;
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < LED_COUNT; j++) {
      strip->setPixelColor(j, strip->gamma32(strip->ColorHSV(i * 256)));
    }
    strip->show();
    delay(10);
  }
}

void neopixelBlink(uint32_t color, int times) {
  if (!strip) return;
  for (int i = 0; i < times; i++) {
    strip->fill(color);
    strip->show();
    delay(120);
    strip->clear();
    strip->show();
    delay(120);
  }
}

void neopixelFade(uint32_t color) {
  if (!strip) return;
  for (int brightness = 0; brightness <= 255; brightness += 5) {
    strip->setBrightness(brightness);
    strip->fill(color);
    strip->show();
    delay(20);
  }
  for (int brightness = 255; brightness >= 0; brightness -= 5) {
    strip->setBrightness(brightness);
    strip->fill(color);
    strip->show();
    delay(20);
  }
  strip->setBrightness(255);
}

void neopixelChase() {
  if (!strip) return;
  uint32_t colors[] = {
    strip->Color(255, 0, 0),
    strip->Color(0, 255, 0),
    strip->Color(0, 0, 255),
    strip->Color(255, 255, 0),
    strip->Color(255, 0, 255),
    strip->Color(0, 255, 255)
  };
  int numColors = sizeof(colors) / sizeof(colors[0]);

  for (int cycle = 0; cycle < 3; cycle++) {
    for (int colorIndex = 0; colorIndex < numColors; colorIndex++) {
      for (int pos = 0; pos < LED_COUNT; pos++) {
        strip->clear();
        for (int i = 0; i < LED_COUNT; i++) {
          if ((i + pos) % 3 == 0) {
            strip->setPixelColor(i, colors[colorIndex]);
          }
        }
        strip->show();
        delay(100);
      }
    }
  }
  strip->clear();
  strip->show();
}

#if ENABLE_TFT_DISPLAY
void detectTFT() {
  Serial.println("\r\n=== DETECTION TFT ===");
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH); // Turn on backlight
  tft.init(TFT_WIDTH, TFT_HEIGHT, SPI_MODE3);
  tft.setRotation(TFT_ROTATION);
  tft.fillScreen(TFT_COLOR_BG);
  tftAvailable = true;
  tftReady = true;
  tftTestResult = String(Texts::detected);
  Serial.println("TFT: Detected and initialized!\r\n");
}

void tftTask(void *pvParameters) {
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = pdMS_TO_TICKS(2000);
  xLastWakeTime = xTaskGetTickCount();

  for (;;) {
    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    if (xSemaphoreTake(tftSemaphore, (TickType_t)10) == pdTRUE) {
      tft.fillScreen(TFT_COLOR_BG);
      tft.setTextColor(TFT_COLOR_HEADER);
      tft.setTextSize(2);
      tft.setCursor(5, 5);
      tft.println("ESP32-DIAG");

      tft.drawFastHLine(0, 25, TFT_WIDTH, TFT_COLOR_SEPARATOR);

      tft.setTextColor(TFT_COLOR_TEXT);
      tft.setTextSize(2);

      // IP Address
      tft.setCursor(5, 35);
      tft.print("IP: ");
      tft.setTextColor(TFT_COLOR_VALUE);
      tft.println(WiFi.localIP().toString());

      // Uptime
      unsigned long seconds = millis() / 1000;
      unsigned long minutes = seconds / 60;
      unsigned long hours = minutes / 60;
      unsigned long days = hours / 24;
      char uptimeStr[32];
      snprintf(uptimeStr, sizeof(uptimeStr), "%lud %luh %lum", days, hours % 24, minutes % 60);
      tft.setTextColor(TFT_COLOR_TEXT);
      tft.setCursor(5, 60);
      tft.print("Up: ");
      tft.setTextColor(TFT_COLOR_VALUE);
      tft.println(uptimeStr);

      // Temp/Humidity
      tft.setTextColor(TFT_COLOR_TEXT);
      tft.setCursor(5, 85);
      if (dhtAvailable) {
        tft.print("T:");
        tft.setTextColor(TFT_COLOR_VALUE);
        tft.print(dhtTemperature, 1);
        tft.print("C ");
        tft.setTextColor(TFT_COLOR_TEXT);
        tft.print("H:");
        tft.setTextColor(TFT_COLOR_VALUE);
        tft.print(dhtHumidity, 1);
        tft.println("%");
      } else {
        tft.print("DHT: ");
        tft.setTextColor(TFT_COLOR_WARNING);
        tft.println("N/A");
      }

      // GPS
      #if ENABLE_GPS
        tft.setTextColor(TFT_COLOR_TEXT);
        tft.setCursor(5, 110);
        tft.print("GPS: ");
        if (gpsFix) {
          tft.setTextColor(TFT_COLOR_VALUE);
          tft.print(gpsSatellites);
          tft.println(" sats");
        } else {
          tft.setTextColor(TFT_COLOR_WARNING);
          tft.println("No Fix");
        }
      #endif

      // Free Heap
      tft.setTextColor(TFT_COLOR_TEXT);
      tft.setCursor(5, 135);
      tft.print("Heap: ");
      tft.setTextColor(TFT_COLOR_VALUE);
      tft.print(ESP.getFreeHeap() / 1024);
      tft.println(" KB");

      xSemaphoreGive(tftSemaphore);
    }
  }
}

void testTFT() {
  if (!tftAvailable) return;
  Serial.println("\r\n=== TEST TFT ===");

  if (xSemaphoreTake(tftSemaphore, (TickType_t)10) == pdTRUE) {
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.println("TFT Test!");
    tft.drawRect(10, 30, 20, 20, ST77XX_RED);
    tft.fillRect(40, 30, 20, 20, ST77XX_GREEN);
    tft.drawCircle(75, 40, 10, ST77XX_BLUE);
    tft.fillCircle(105, 40, 10, ST77XX_YELLOW);
    xSemaphoreGive(tftSemaphore);
    delay(2000);
    tftTested = true;
    tftTestResult = String(Texts::test) + " " + String(Texts::ok);
    Serial.println("TFT: Test OK");
  } else {
    Serial.println("TFT: Could not get semaphore");
  }
}

void resetTFTTest() {
  tftTested = false;
}

void tftShowMessage(String message) {
  if (!tftAvailable) return;
  if (xSemaphoreTake(tftSemaphore, (TickType_t)10) == pdTRUE) {
    tft.fillScreen(TFT_COLOR_BG);
    tft.setCursor(5, 5);
    tft.setTextColor(TFT_COLOR_TEXT);
    tft.setTextSize(2);
    tft.println(message);
    xSemaphoreGive(tftSemaphore);
  }
}

void tftShowWiFiStatus(const String& title,
                        const String& detail,
                        const String& footer,
                        int progressPercent) {
  if (!tftAvailable) {
    return;
  }

  int clampedProgress = progressPercent;
  if (clampedProgress < 0) {
    clampedProgress = -1;
  } else if (clampedProgress > 100) {
    clampedProgress = 100;
  }

  if (xSemaphoreTake(tftSemaphore, (TickType_t)10) == pdTRUE) {
    tft.fillScreen(TFT_COLOR_BG);
    tft.setTextColor(TFT_COLOR_HEADER);
    tft.setTextSize(2);
    tft.setCursor(5, 30);
    tft.println(title);

    tft.setTextColor(TFT_COLOR_TEXT);
    tft.setCursor(5, 60);
    tft.println(detail);

    tft.setTextColor(TFT_COLOR_VALUE);
    tft.setCursor(5, 90);
    tft.println(footer);

    if (clampedProgress >= 0) {
      int barWidth = TFT_WIDTH - 20;
      tft.drawRect(10, 120, barWidth, 15, TFT_COLOR_SEPARATOR);
      int fillWidth = (barWidth - 4) * clampedProgress / 100;
      if (fillWidth > 0) {
        tft.fillRect(12, 122, fillWidth, 11, TFT_COLOR_VALUE);
      }
    }
    xSemaphoreGive(tftSemaphore);
  }
}
#endif

#if ENABLE_GPS
void gpsTask(void *pvParameters) {
  // Initialisation du port série matériel avec les bonnes broches
  // Le format est : begin(baud_rate, config, rxPin, txPin)
  SerialGPS.begin(GPS_BAUD_RATE, SERIAL_8N1, PIN_GPS_RXD, PIN_GPS_TXD);
  for (;;) {
    while (SerialGPS.available() > 0) {
      // On lit les données du GPS et on les passe à l'encodeur TinyGPS++
      if (gps.encode(SerialGPS.read())) {
        if (gps.location.isUpdated()) {
          gpsFix = gps.location.isValid();
          gpsSatellites = gps.satellites.value();
          gpsLat = gps.location.lat();
          gpsLon = gps.location.lng();
          gpsAltitude = gps.altitude.meters();
          gpsSpeed = gps.speed.kmph();
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
#endif

// ========== TEST ADC ==========
void testADC() {
  Serial.println("\r\n=== TEST ADC ===");
  adcReadings.clear();
  
  #if defined(CONFIG_IDF_TARGET_ESP32S3)
    int adcPins[] = {1, 2, 3, 4, 5, 6};
    int numADC = 10;
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    int adcPins[] = {0, 1, 2, 3, 4};
    int numADC = 5;
  #else
    int adcPins[] = {36, 39};
    int numADC = 2;
  #endif

  for (int i = 0; i < numADC; i++) {
    ADCReading reading;
    reading.pin = adcPins[i];
    reading.rawValue = analogRead(adcPins[i]);
    reading.voltage = (reading.rawValue / 4095.0) * 3.3;
    adcReadings.push_back(reading);

    Serial.printf("GPIO%d: %d (%.2fV)\r\n", reading.pin, reading.rawValue, reading.voltage);
  }
  
  adcTestResult = String(numADC) + " " + String(Texts::channels) + " - " + String(Texts::ok);
  Serial.printf("ADC: %d canaux testes\r\n", numADC);
}

// ========== TEST PWM ==========
void testPWM() {
  Serial.println("\r\n=== TEST PWM ===");
  int testPin = DEFAULT_PWM_PIN;

  if (testPin < 0) {
    pwmTestResult = String(Texts::configuration_invalid);
    return;
  }

  Serial.printf("Test PWM sur GPIO%d\r\n", testPin);

  ledcSetup(4, 5000, 8);
  ledcAttachPin(testPin, 4);

  for (int duty = 0; duty <= 255; duty += 51) {
    ledcWrite(4, duty);
    Serial.printf("PWM duty: %d/255\r\n", duty);
    delay(80);
    yield();
  }

  ledcWrite(4, 0);
  ledcDetachPin(testPin);                // pin

  pwmTestResult = String(Texts::test) + " " + String(Texts::ok) + " - GPIO " + String(testPin);
  Serial.println("PWM: OK");
}

// ========== SCAN SPI ==========
void scanSPI() {
  Serial.println("\r\n=== SCAN SPI ===");
  spiInfo = "";
  
  #ifdef CONFIG_IDF_TARGET_ESP32
    spiInfo = "HSPI (12,13,14,15), VSPI (18,19,23,5)";
    Serial.println("SPI: HSPI + VSPI disponibles");
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
    spiInfo = "SPI2, SPI3 disponibles";
    Serial.println("SPI: SPI2, SPI3");
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    spiInfo = "SPI2, SPI3 disponibles";
    Serial.println("SPI: SPI2, SPI3");
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    spiInfo = "SPI2 disponible";
    Serial.println("SPI: SPI2");
  #else
    spiInfo = "Info SPI non disponible";
  #endif
}

// ========== LISTING PARTITIONS ==========
void listPartitions() {
  Serial.println("\r\n=== PARTITIONS FLASH ===");
  partitionsInfo = "";
  
  esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
  
  int count = 0;
  while (it != NULL) {
    const esp_partition_t *part = esp_partition_get(it);
    
    char line[128];
    sprintf(line, "%s - Type:0x%02x Addr:0x%08x Size:%dKB", 
            part->label, part->type, part->address, part->size / 1024);
    
    if (count > 0) partitionsInfo += "\r\n";
    partitionsInfo += String(line);
    
    Serial.println(line);
    
    it = esp_partition_next(it);
    count++;
  }
  
  esp_partition_iterator_release(it);
  Serial.printf("Total: %d partitions\r\n", count);
}

// TEST LED RGB
void testRGBLed() {
  Serial.println("\r\n=== TEST LED RGB ===");

  if (RGB_LED_PIN_R < 0 || RGB_LED_PIN_G < 0 || RGB_LED_PIN_B < 0) {
    rgbLedTestResult = String(Texts::configuration_invalid);
    rgbLedAvailable = false;
    Serial.println("LED RGB: Configuration invalide");
    return;
  }

  pinMode(RGB_LED_PIN_R, OUTPUT);
  pinMode(RGB_LED_PIN_G, OUTPUT);
  pinMode(RGB_LED_PIN_B, OUTPUT);

  // Initialize PWM channels for RGB LED if using analogWrite
  ledcSetup(0, 5000, 8); ledcAttachPin(RGB_LED_PIN_R, 0);
  ledcSetup(1, 5000, 8); ledcAttachPin(RGB_LED_PIN_G, 1);
  ledcSetup(2, 5000, 8); ledcAttachPin(RGB_LED_PIN_B, 2);
  Serial.printf("Test LED RGB - R:%d G:%d B:%d\r\n", RGB_LED_PIN_R, RGB_LED_PIN_G, RGB_LED_PIN_B);

  digitalWrite(RGB_LED_PIN_R, LOW);
  digitalWrite(RGB_LED_PIN_G, LOW);
  digitalWrite(RGB_LED_PIN_B, LOW);
  delay(120);

  digitalWrite(RGB_LED_PIN_R, HIGH);
  delay(150);
  digitalWrite(RGB_LED_PIN_R, LOW);

  digitalWrite(RGB_LED_PIN_G, HIGH);
  delay(150);
  digitalWrite(RGB_LED_PIN_G, LOW);

  digitalWrite(RGB_LED_PIN_B, HIGH);
  delay(150);
  digitalWrite(RGB_LED_PIN_B, LOW);

  rgbLedTestResult = String(Texts::ok);
  rgbLedAvailable = true;
  Serial.println("LED RGB: OK");
}

void setRGBLedColor(int r, int g, int b) {
  if (RGB_LED_PIN_R >= 0 && RGB_LED_PIN_G >= 0 && RGB_LED_PIN_B >= 0) {
    ledcWrite(0, r);
    ledcWrite(1, g);
    ledcWrite(2, b);
  }
}

// TEST BUZZER
void testBuzzer() {
  Serial.println("\r\n=== TEST BUZZER ===");

  if (BUZZER_PIN < 0) {
    buzzerTestResult = String(Texts::configuration_invalid);
    buzzerAvailable = false;
    Serial.println("Buzzer: Configuration invalide");
    return;
  }

  pinMode(BUZZER_PIN, OUTPUT);
  // Setup PWM channel for buzzer tone
  ledcSetup(3, 1000, 8);
  ledcAttachPin(BUZZER_PIN, 3);
  Serial.printf("Test Buzzer - Pin:%d\r\n", BUZZER_PIN);

  tone(BUZZER_PIN, 1000, 160);
  delay(220);
  tone(BUZZER_PIN, 1500, 160);
  delay(220);
  tone(BUZZER_PIN, 2000, 160);
  delay(220);
  ledcDetachPin(BUZZER_PIN);

  buzzerTestResult = String(Texts::ok);
  buzzerAvailable = true;
  Serial.println("Buzzer: OK");
}

void playBuzzerTone(int frequency, int duration) {
  if (BUZZER_PIN >= 0) {
    ledcAttachPin(BUZZER_PIN, 3);
    ledcWriteTone(3, frequency);
    delay(duration);
    ledcWriteTone(3, 0);
  }
}

// DHT sensor helpers
static inline const char* getDhtSensorName() {
  return (DHT_SENSOR_TYPE == 22) ? "DHT22" : "DHT11";
}

// TEST DHT SENSOR
void testDHTSensor() {
  const char* sensorName = getDhtSensorName();
  Serial.printf("\r\n=== TEST %s ===\r\n", sensorName);

  if (DHT_PIN < 0) {
    dhtTestResult = String(Texts::configuration_invalid);
    dhtAvailable = false;
    Serial.printf("%s: Configuration invalide\r\n", sensorName);
    return;
  }

  Serial.printf("Lecture %s - Pin:%d\r\n", sensorName, DHT_PIN);

  dht.begin();
  delay(2000); // Wait for sensor to stabilize

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    dhtTestResult = String(Texts::error_label);
    dhtAvailable = false;
    Serial.printf("%s: Failed to read from sensor!\r\n", sensorName);
  } else {
    dhtHumidity = h;
    dhtTemperature = t;
    dhtTestResult = String(Texts::ok);
    dhtAvailable = true;
    Serial.printf("%s: T=%.1f°C H=%.1f%%\r\n", sensorName, dhtTemperature, dhtHumidity);
  }
}

// TEST LIGHT SENSOR
void testLightSensor() {
  Serial.println("\r\n=== TEST LIGHT SENSOR ===");

  if (LIGHT_SENSOR_PIN < 0) {
    lightSensorTestResult = String(Texts::configuration_invalid);
    lightSensorAvailable = false;
    Serial.println("Light Sensor: Configuration invalide");
    return;
  }

  pinMode(LIGHT_SENSOR_PIN, INPUT);
  Serial.printf("Lecture Light Sensor - Pin:%d\r\n", LIGHT_SENSOR_PIN);

  int sum = 0;
  const int samples = 6;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(LIGHT_SENSOR_PIN);
    delay(8);
    yield();
  }
  lightSensorValue = sum / samples;

  lightSensorTestResult = String(Texts::ok);
  lightSensorAvailable = true;
  Serial.printf("Light Sensor: %d\r\n", lightSensorValue);
}

// TEST DISTANCE SENSOR
void testDistanceSensor() {
  Serial.println("\r\n=== TEST DISTANCE SENSOR (HC-SR04) ===");

  if (DISTANCE_TRIG_PIN < 0 || DISTANCE_ECHO_PIN < 0) {
    distanceSensorTestResult = String(Texts::configuration_invalid);
    distanceSensorAvailable = false;
    Serial.println("Distance Sensor: Configuration invalide");
    return;
  }

  pinMode(DISTANCE_TRIG_PIN, OUTPUT);
  pinMode(DISTANCE_ECHO_PIN, INPUT);

  Serial.printf("Distance Sensor - Trig:%d Echo:%d\r\n", DISTANCE_TRIG_PIN, DISTANCE_ECHO_PIN);

  digitalWrite(DISTANCE_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(DISTANCE_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(DISTANCE_TRIG_PIN, LOW);

  long duration = pulseIn(DISTANCE_ECHO_PIN, HIGH, 30000);

  if (duration > 0) {
    distanceValue = duration * 0.034 / 2;
    distanceSensorTestResult = String(Texts::ok);
    distanceSensorAvailable = true;
    Serial.printf("Distance: %.2f cm\r\n", distanceValue);
  } else {
    distanceSensorTestResult = String(Texts::error_label);
    distanceSensorAvailable = false;
    Serial.println("Distance Sensor: No echo");
  }
}

// TEST MOTION SENSOR
void testMotionSensor() {
  Serial.println("\r\n=== TEST MOTION SENSOR (PIR) ===");

  if (MOTION_SENSOR_PIN < 0) {
    motionSensorTestResult = String(Texts::configuration_invalid);
    motionSensorAvailable = false;
    Serial.println("Motion Sensor: Configuration invalide");
    return;
  }

  pinMode(MOTION_SENSOR_PIN, INPUT);
  Serial.printf("Motion Sensor - Pin:%d\r\n", MOTION_SENSOR_PIN);

  delay(30);
  yield();
  motionDetected = digitalRead(MOTION_SENSOR_PIN);

  motionSensorTestResult = String(Texts::ok);
  motionSensorAvailable = true;
  Serial.printf("Motion: %s\r\n", motionDetected ? "Detected" : "None");
}

// ========== TEST STRESS MÉMOIRE ==========
void memoryStressTest() {
  Serial.println("\r\n=== STRESS TEST MEMOIRE ===");
  
  unsigned long startMs = millis();
  stressAllocationCount = 0;
  stressDurationMs = 0;

  const int allocSize = 1024;
  int maxAllocs = 0;
  std::vector<void*> allocations;
  
  Serial.println("Allocation progressive...");
  while(true) {
    void* ptr = malloc(allocSize);
    if (!ptr) break;
    allocations.push_back(ptr);
    maxAllocs++;
    
    if (maxAllocs % 100 == 0) {
      Serial.printf("Alloue: %d KB\r\n", maxAllocs);
    }
  }
  
  Serial.printf("Max allocations: %d blocs (%d KB)\r\n", maxAllocs, maxAllocs);


  Serial.println("Liberation memoire...");
  for (auto ptr : allocations) {
    free(ptr);
  }

  stressAllocationCount = maxAllocs;
  stressDurationMs = millis() - startMs;

  stressTestResult = String(Texts::max_alloc) + ": " + String(maxAllocs) + " KB";
  Serial.println("Stress test: OK");
}

// ========== COLLECTE DONNÉES ==========
void collectDiagnosticInfo() {
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  
  diagnosticData.chipModel = detectChipModel();
  diagnosticData.chipRevision = String(chip_info.revision);
  diagnosticData.cpuCores = chip_info.cores;
  diagnosticData.cpuFreqMHz = ESP.getCpuFreqMHz();
  
  uint32_t flash_size;
  esp_flash_get_size(NULL, &flash_size);
  diagnosticData.flashSize = flash_size;
  diagnosticData.psramSize = ESP.getPsramSize();
  
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", 
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  diagnosticData.macAddress = String(macStr);
  
  diagnosticData.heapSize = ESP.getHeapSize();
  diagnosticData.freeHeap = ESP.getFreeHeap();
  diagnosticData.minFreeHeap = ESP.getMinFreeHeap();
  diagnosticData.maxAllocHeap = ESP.getMaxAllocHeap();
  
  diagnosticData.hasWiFi = (chip_info.features & CHIP_FEATURE_WIFI_BGN);
  diagnosticData.hasBT = (chip_info.features & CHIP_FEATURE_BT);
  bool featureBLE = (chip_info.features & CHIP_FEATURE_BLE);

#ifdef CHIP_ESP32S2
  if (chip_info.model == CHIP_ESP32S2) {
    runtimeBLE = false;
  }
#endif
#ifdef CHIP_ESP32S3
  if (chip_info.model == CHIP_ESP32S3) {
    runtimeBLE = true;
  }
#endif
#ifdef CHIP_ESP32C3
  if (chip_info.model == CHIP_ESP32C3) {
    runtimeBLE = true;
  }
#endif
#ifdef CHIP_ESP32C6
  if (chip_info.model == CHIP_ESP32C6) {
    runtimeBLE = true;
  }
#endif
#ifdef CHIP_ESP32H2
  if (chip_info.model == CHIP_ESP32H2) {
    runtimeBLE = true;
  }
#endif

  if (WiFi.status() == WL_CONNECTED) {
    diagnosticData.wifiSSID = WiFi.SSID();
    diagnosticData.wifiRSSI = WiFi.RSSI();
    diagnosticData.ipAddress = WiFi.localIP().toString();
#if DIAGNOSTIC_HAS_MDNS
    diagnosticData.mdnsAvailable = mdnsServiceActive;
#else
    diagnosticData.mdnsAvailable = false;
#endif
  } else {
    diagnosticData.wifiSSID = "";
    diagnosticData.wifiRSSI = -127;
    diagnosticData.ipAddress = "";
    diagnosticData.mdnsAvailable = false;
  }
  
  diagnosticData.gpioList = getGPIOList();
  diagnosticData.totalGPIO = countGPIO();
  
  diagnosticData.sdkVersion = ESP.getSdkVersion();
  diagnosticData.idfVersion = esp_get_idf_version();
  diagnosticData.uptime = millis();
  
  #ifdef SOC_TEMP_SENSOR_SUPPORTED
    diagnosticData.temperature = temperatureRead();
  #else
    diagnosticData.temperature = -999;
  #endif
  
  diagnosticData.neopixelTested = neopixelTested;
  diagnosticData.neopixelAvailable = neopixelAvailable;
  diagnosticData.neopixelResult = neopixelTestResult;
  
  diagnosticData.tftTested = tftTested;
  diagnosticData.tftAvailable = tftAvailable;
  diagnosticData.tftResult = tftTestResult;
  
  heapHistory[historyIndex] = (float)diagnosticData.freeHeap / 1024.0;
  if (diagnosticData.temperature != -999) {
    tempHistory[historyIndex] = diagnosticData.temperature;
  }
  historyIndex = (historyIndex + 1) % HISTORY_SIZE;
}

// Routines de tests en tâche de fond
static void runBuiltinLedTestTask() {
  resetBuiltinLEDTest();
  testBuiltinLED();
}

static void runNeopixelTestTask() {
  resetNeoPixelTest();
  testNeoPixel();
}

static void runTftTestTask() {
  resetTFTTest();
  testTFT();
}

static void runRgbLedTestTask() {
  testRGBLed();
}

static void runBuzzerTestTask() {
  testBuzzer();
}

// ========== HANDLERS API ==========
void handleTestGPIO() {
  testAllGPIOs();
  String json;
  json.reserve(gpioResults.size() * 40 + 20);  // Estimate size to avoid reallocations
  json = "{\"results\":[";
  for (size_t i = 0; i < gpioResults.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"pin\":" + String(gpioResults[i].pin) + ",\"working\":" + String(gpioResults[i].working ? "true" : "false") + "}";
  }
  json += "]}";
  server.send(200, "application/json", json);
}

void handleWiFiScan() {
  scanWiFiNetworks();
  String json;
  json.reserve(wifiNetworks.size() * 150 + 20);  // Estimate size to avoid reallocations
  json = "{\"networks\":[";
  for (size_t i = 0; i < wifiNetworks.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"ssid\":\"" + wifiNetworks[i].ssid + "\",\"rssi\":" + String(wifiNetworks[i].rssi) +
            ",\"channel\":" + String(wifiNetworks[i].channel) + ",\"encryption\":\"" + wifiNetworks[i].encryption +
            "\",\"bssid\":\"" + wifiNetworks[i].bssid + "\",\"band\":\"" + wifiNetworks[i].band +
            "\",\"bandwidth\":\"" + wifiNetworks[i].bandwidth + "\",\"phy\":\"" + wifiNetworks[i].phyModes +
            "\",\"freq\":" + String(wifiNetworks[i].freqMHz) + "}";
  }
  json += "]}";
  server.send(200, "application/json", json);
}

void handleI2CScan() {
  scanI2C();
  sendJsonResponse(200, {
    jsonNumberField("count", diagnosticData.i2cCount),
    jsonStringField("devices", diagnosticData.i2cDevices)
  });
}

void handleBuiltinLEDConfig() {
  if (server.hasArg("gpio")) {
    int newGPIO = server.arg("gpio").toInt();
    if (newGPIO >= 0 && newGPIO <= 48) {
      BUILTIN_LED_PIN = newGPIO;
      resetBuiltinLEDTest();
      String message = String(Texts::config) + " " + String(Texts::gpio) + " " + String(BUILTIN_LED_PIN);
      sendOperationSuccess(message);
      return;
    }
  }
  sendOperationError(400, Texts::gpio_invalid.str());
}

void handleBuiltinLEDTest() {
  bool alreadyRunning = false;
  bool started = startAsyncTest(builtinLedTestRunner, runBuiltinLedTestTask, alreadyRunning);

  if (started) {
    sendActionResponse(202, true, String(Texts::test_in_progress), {
      jsonBoolField("running", true),
      jsonStringField("result", builtinLedTestResult)
    });
    return;
  }

  if (alreadyRunning) {
    sendActionResponse(200, true, String(Texts::test_in_progress), {
      jsonBoolField("running", true),
      jsonStringField("result", builtinLedTestResult)
    });
    return;
  }

  resetBuiltinLEDTest();
  testBuiltinLED();
  sendActionResponse(200, builtinLedAvailable, builtinLedTestResult, {
    jsonBoolField("running", false),
    jsonStringField("result", builtinLedTestResult)
  });
}

void handleBuiltinLEDControl() {
  if (!server.hasArg("action")) {
    sendOperationError(400, Texts::configuration_invalid.str());
    return;
  }

  String action = server.arg("action");
  if (BUILTIN_LED_PIN == -1) {
    sendOperationError(400, Texts::gpio_invalid.str());
    return;
  }

  pinMode(BUILTIN_LED_PIN, OUTPUT);
  String message;

  if (action == "blink") {
    for (int i = 0; i < 5; i++) {
      digitalWrite(BUILTIN_LED_PIN, HIGH);
      delay(200);
      digitalWrite(BUILTIN_LED_PIN, LOW);
      delay(200);
    }
    message = String(Texts::blink) + " " + String(Texts::ok);
  } else if (action == "fade") {
    for (int i = 0; i <= 255; i += 5) {
      analogWrite(BUILTIN_LED_PIN, i);
      delay(10);
    }
    for (int i = 255; i >= 0; i -= 5) {
      analogWrite(BUILTIN_LED_PIN, i);
      delay(10);
    }
    digitalWrite(BUILTIN_LED_PIN, LOW);
    message = String(Texts::fade) + " " + String(Texts::ok);
  } else if (action == "off") {
    digitalWrite(BUILTIN_LED_PIN, LOW);
    builtinLedTested = false;
    message = String(Texts::off);
  } else {
    sendOperationError(400, Texts::configuration_invalid.str());
    return;
  }

  sendOperationSuccess(message);
}

void handleNeoPixelConfig() {
  if (server.hasArg("gpio") && server.hasArg("count")) {
    int newGPIO = server.arg("gpio").toInt();
    int newCount = server.arg("count").toInt();
    
    if (newGPIO >= 0 && newGPIO <= 48 && newCount > 0 && newCount <= 100) {
      LED_PIN = newGPIO;
      LED_COUNT = newCount;
      if (strip) delete strip;
      strip = new Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
      resetNeoPixelTest();
      String message = String(Texts::config) + " " + String(Texts::gpio) + " " + String(LED_PIN);
      sendOperationSuccess(message);
      return;
    }
  }
  sendOperationError(400, Texts::configuration_invalid.str());
}

void handleNeoPixelTest() {
  bool alreadyRunning = false;
  bool started = startAsyncTest(neopixelTestRunner, runNeopixelTestTask, alreadyRunning);

  if (started) {
    sendActionResponse(202, true, String(Texts::test_in_progress), {
      jsonBoolField("running", true),
      jsonBoolField("available", neopixelAvailable),
      jsonStringField("result", neopixelTestResult)
    });
    return;
  }

  if (alreadyRunning) {
    sendActionResponse(200, true, String(Texts::test_in_progress), {
      jsonBoolField("running", true),
      jsonBoolField("available", neopixelAvailable),
      jsonStringField("result", neopixelTestResult)
    });
    return;
  }

  resetNeoPixelTest();
  testNeoPixel();
  sendActionResponse(200, neopixelAvailable, neopixelTestResult, {
    jsonBoolField("running", false),
    jsonBoolField("available", neopixelAvailable),
    jsonStringField("result", neopixelTestResult)
  });
}

void handleNeoPixelPattern() {
  if (!server.hasArg("pattern")) {
    sendOperationError(400, Texts::configuration_invalid.str());
    return;
  }

  String pattern = server.arg("pattern");
  if (!strip) {
    String message = String(Texts::neopixel) + " - " + String(Texts::not_detected);
    sendOperationError(400, message);
    return;
  }

  String message;
  if (pattern == "rainbow") {
    neopixelRainbow();
    message = String(Texts::rainbow) + " " + String(Texts::ok);
  } else if (pattern == "blink") {
    neopixelBlink(strip->Color(255, 0, 0), 5);
    message = String(Texts::blink) + " " + String(Texts::ok);
  } else if (pattern == "fade") {
    neopixelFade(strip->Color(0, 0, 255));
    message = String(Texts::fade) + " " + String(Texts::ok);
  } else if (pattern == "chase") {
    neopixelChase();
    message = String(Texts::chase) + " " + String(Texts::ok);
  } else if (pattern == "off") {
    strip->clear();
    strip->show();
    neopixelTested = false;
    message = String(Texts::off);
  } else {
    sendOperationError(400, Texts::configuration_invalid.str());
    return;
  }

  sendOperationSuccess(message);
}

void handleNeoPixelColor() {
  if (!server.hasArg("r") || !server.hasArg("g") || !server.hasArg("b") || !strip) {
    sendOperationError(400, Texts::configuration_invalid.str());
    return;
  }

  int r = server.arg("r").toInt();
  int g = server.arg("g").toInt();
  int b = server.arg("b").toInt();

  strip->fill(strip->Color(r, g, b));
  strip->show();
  neopixelTested = false;

  String message = "RGB(" + String(r) + "," + String(g) + "," + String(b) + ")";
  sendOperationSuccess(message);
}

void handleTFTMessage() {
  if (!server.hasArg("message")) {
    sendOperationError(400, Texts::configuration_invalid.str());
    return;
  }

  String message = server.arg("message");
  tftShowMessage(message);
  sendOperationSuccess(Texts::message_displayed.str());
}

void handleTFTTest() {
  bool alreadyRunning = false;
  bool started = startAsyncTest(tftTestRunner, runTftTestTask, alreadyRunning, 4096, 1);

  if (started) {
    sendActionResponse(202, true, String(Texts::test_in_progress), {
      jsonBoolField("running", true),
      jsonBoolField("available", tftAvailable),
      jsonStringField("result", tftTestResult)
    });
    return;
  }

  if (alreadyRunning) {
    sendActionResponse(200, true, String(Texts::test_in_progress), {
      jsonBoolField("running", true),
      jsonBoolField("available", tftAvailable),
      jsonStringField("result", tftTestResult)
    });
    return;
  }

  resetTFTTest();
  testTFT();
  sendActionResponse(200, tftAvailable, tftTestResult, {
    jsonBoolField("running", false),
    jsonBoolField("available", tftAvailable),
    jsonStringField("result", tftTestResult)
  });
}

void handleTFTBacklight() {
  digitalWrite(TFT_BL, server.hasArg("on"));
}

void handleADCTest() {
  testADC();
  String json;
  json.reserve(adcReadings.size() * 60 + 50);  // Estimate size to avoid reallocations
  json = "{\"readings\":[";
  for (size_t i = 0; i < adcReadings.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"pin\":" + String(adcReadings[i].pin) + ",\"raw\":" + String(adcReadings[i].rawValue) +
            ",\"voltage\":" + String(adcReadings[i].voltage, 2) + "}";
  }
  json += "],\"result\":\"" + adcTestResult + "\"}";
  server.send(200, "application/json", json);
}

void handlePWMTest() {
  testPWM();
  sendJsonResponse(200, { jsonStringField("result", pwmTestResult) });
}

void handleSPIScan() {
  scanSPI();
  sendJsonResponse(200, { jsonStringField("info", spiInfo) });
}

void handlePartitionsList() {
  listPartitions();
  sendJsonResponse(200, { jsonStringField("partitions", partitionsInfo) });
}

void handleStressTest() {
  memoryStressTest();
  sendJsonResponse(200, {
    jsonStringField("result", stressTestResult),
    jsonNumberField("allocations", static_cast<unsigned long>(stressAllocationCount)),
    jsonNumberField("durationMs", stressDurationMs),
    jsonStringField("allocationsLabel", stressTestResult)
  });
}

// Handlers API pour les nouveaux capteurs
void handleRGBLedConfig() {
  if (server.hasArg("r") && server.hasArg("g") && server.hasArg("b")) {
    RGB_LED_PIN_R = server.arg("r").toInt();
    RGB_LED_PIN_G = server.arg("g").toInt();
    RGB_LED_PIN_B = server.arg("b").toInt();
    sendActionResponse(200, true, String(Texts::ok));
  } else {
    sendActionResponse(400, false, String(Texts::configuration_invalid));
  }
}

void handleRGBLedTest() {
  bool alreadyRunning = false;
  bool started = startAsyncTest(rgbLedTestRunner, runRgbLedTestTask, alreadyRunning, 3072, 1);

  if (started) {
    sendJsonResponse(202, {
      jsonBoolField("running", true),
      jsonBoolField("success", rgbLedAvailable),
      jsonStringField("result", rgbLedTestResult)
    });
    return;
  }

  if (alreadyRunning) {
    sendJsonResponse(200, {
      jsonBoolField("running", true),
      jsonBoolField("success", rgbLedAvailable),
      jsonStringField("result", rgbLedTestResult)
    });
    return;
  }

  testRGBLed();
  sendJsonResponse(200, {
    jsonBoolField("running", false),
    jsonBoolField("success", rgbLedAvailable),
    jsonStringField("result", rgbLedTestResult)
  });
}

void handleRGBLedColor() {
  if (server.hasArg("r") && server.hasArg("g") && server.hasArg("b")) {
    int r = server.arg("r").toInt();
    int g = server.arg("g").toInt();
    int b = server.arg("b").toInt();
    setRGBLedColor(r, g, b);
    sendActionResponse(200, true, "RGB(" + String(r) + "," + String(g) + "," + String(b) + ")");
  } else {
    sendActionResponse(400, false, String(Texts::configuration_invalid));
  }
}

void handleBuzzerConfig() {
  if (server.hasArg("pin")) {
    BUZZER_PIN = server.arg("pin").toInt();
    sendActionResponse(200, true, String(Texts::ok));
  } else {
    sendActionResponse(400, false, String(Texts::configuration_invalid));
  }
}

void handleBuzzerTest() {
  bool alreadyRunning = false;
  bool started = startAsyncTest(buzzerTestRunner, runBuzzerTestTask, alreadyRunning, 3072, 1);

  if (started) {
    sendJsonResponse(202, {
      jsonBoolField("running", true),
      jsonBoolField("success", buzzerAvailable),
      jsonStringField("result", buzzerTestResult)
    });
    return;
  }

  if (alreadyRunning) {
    sendJsonResponse(200, {
      jsonBoolField("running", true),
      jsonBoolField("success", buzzerAvailable),
      jsonStringField("result", buzzerTestResult)
    });
    return;
  }

  testBuzzer();
  sendJsonResponse(200, {
    jsonBoolField("running", false),
    jsonBoolField("success", buzzerAvailable),
    jsonStringField("result", buzzerTestResult)
  });
}

void handleBuzzerTone() {
  if (server.hasArg("freq") && server.hasArg("duration")) {
    int freq = server.arg("freq").toInt();
    int duration = server.arg("duration").toInt();
    playBuzzerTone(freq, duration);
    sendActionResponse(200, true, String(freq) + "Hz");
  } else {
    sendActionResponse(400, false, String(Texts::configuration_invalid));
  }
}

void handleDHTConfig() {
  bool updated = false;

  if (server.hasArg("pin")) {
    DHT_PIN = server.arg("pin").toInt();
    updated = true;
  }

  if (server.hasArg("type")) {
    String rawType = server.arg("type");
    rawType.trim();
    rawType.toUpperCase();

    uint8_t candidate = 0;
    if (rawType == "DHT22" || rawType == "22") {
      candidate = 22;
    } else if (rawType == "DHT11" || rawType == "11") {
      candidate = 11;
    }

    if (candidate == 11 || candidate == 22) {
      DHT_SENSOR_TYPE = candidate;
      updated = true;
    } else {
      sendActionResponse(400, false, String(Texts::configuration_invalid));
      return;
    }
  }

  if (updated) {
    sendActionResponse(200,
                       true,
                       String(Texts::ok),
                       {jsonNumberField("type", static_cast<int>(DHT_SENSOR_TYPE))});
  } else {
    sendActionResponse(400, false, String(Texts::configuration_invalid));
  }
}

void handleDHTTest() {
  testDHTSensor();
  sendJsonResponse(200, {
    jsonBoolField("success", dhtAvailable),
    jsonStringField("result", dhtTestResult),
    jsonFloatField("temperature", dhtTemperature, 1),
    jsonFloatField("humidity", dhtHumidity, 1),
    jsonNumberField("type", static_cast<int>(DHT_SENSOR_TYPE))
  });
}

void handleLightSensorConfig() {
  if (server.hasArg("pin")) {
    LIGHT_SENSOR_PIN = server.arg("pin").toInt();
    sendActionResponse(200, true, String(Texts::ok));
  } else {
    sendActionResponse(400, false, String(Texts::configuration_invalid));
  }
}

void handleLightSensorTest() {
  testLightSensor();
  sendJsonResponse(200, {
    jsonBoolField("success", lightSensorAvailable),
    jsonStringField("result", lightSensorTestResult),
    jsonNumberField("value", lightSensorValue)
  });
}

void handleDistanceSensorConfig() {
  if (server.hasArg("trig") && server.hasArg("echo")) {
    DISTANCE_TRIG_PIN = server.arg("trig").toInt();
    DISTANCE_ECHO_PIN = server.arg("echo").toInt();
    sendActionResponse(200, true, String(Texts::ok));
  } else {
    sendActionResponse(400, false, String(Texts::configuration_invalid));
  }
}

void handleDistanceSensorTest() {
  testDistanceSensor();
  sendJsonResponse(200, {
    jsonBoolField("success", distanceSensorAvailable),
    jsonStringField("result", distanceSensorTestResult),
    jsonFloatField("distance", distanceValue, 2)
  });
}

void handleMotionSensorConfig() {
  if (server.hasArg("pin")) {
    MOTION_SENSOR_PIN = server.arg("pin").toInt();
    sendActionResponse(200, true, String(Texts::ok));
  } else {
    sendActionResponse(400, false, String(Texts::configuration_invalid));
  }
}

void handleMotionSensorTest() {
  testMotionSensor();
  sendJsonResponse(200, {
    jsonBoolField("success", motionSensorAvailable),
    jsonStringField("result", motionSensorTestResult),
    jsonBoolField("motion", motionDetected)
  });
}

void handleBenchmark() {
  unsigned long cpuTime = benchmarkCPU();
  unsigned long memTime = benchmarkMemory();

  diagnosticData.cpuBenchmark = cpuTime;
  diagnosticData.memBenchmark = memTime;

  // Provide derived benchmark metrics for richer telemetry
  // Combined memory stress metrics for the benchmark API
  memoryStressTest();

  double cpuPerf = 100000.0 / static_cast<double>(cpuTime);
  double memSpeed = (10000.0 * sizeof(int) * 2.0) / static_cast<double>(memTime);
  sendJsonResponse(200, {
    jsonNumberField("cpu", cpuTime),
    jsonNumberField("memory", memTime),
    jsonFloatField("cpuPerf", cpuPerf, 2),
    jsonFloatField("memSpeed", memSpeed, 2),
    jsonNumberField("allocations", static_cast<unsigned long>(stressAllocationCount)),
    jsonNumberField("stressDuration", stressDurationMs),
    jsonStringField("stress", stressTestResult),
    jsonStringField("allocationsLabel", stressTestResult)
  });
}

void handleStatus() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  bool wifiConnected = (WiFi.status() == WL_CONNECTED);
  unsigned long currentUptime = millis();

  String json;
  json.reserve(400);
  json = "{";
  json += "\"connected\":" + String(wifiConnected ? "true" : "false") + ",";
  json += "\"uptime\":" + String(currentUptime) + ",";
  json += "\"temperature\":" + String(diagnosticData.temperature) + ",";
  json += "\"sram\":{\"total\":" + String(detailedMemory.sramTotal) +
          ",\"free\":" + String(detailedMemory.sramFree) +
          ",\"used\":" + String(detailedMemory.sramUsed) + "},";
  json += "\"psram\":{\"total\":" + String(detailedMemory.psramTotal) +
          ",\"free\":" + String(detailedMemory.psramFree) +
          ",\"used\":" + String(detailedMemory.psramUsed) + "},";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1) + "";
  json += "}";

  server.send(200, "application/json", json);
}

void handleSystemInfo() {
  collectDiagnosticInfo();
  String json;
  json.reserve(600);
  json = "{";
  json += "\"chipModel\":\"" + diagnosticData.chipModel + "\",";
  json += "\"chipRevision\":\"" + diagnosticData.chipRevision + "\",";
  json += "\"cpuCores\":" + String(diagnosticData.cpuCores) + ",";
  json += "\"cpuFreq\":" + String(diagnosticData.cpuFreqMHz) + ",";
  json += "\"macAddress\":\"" + diagnosticData.macAddress + "\",";
  json += "\"ipAddress\":\"" + diagnosticData.ipAddress + "\",";
  json += "\"mdnsReady\":" + String(diagnosticData.mdnsAvailable ? "true" : "false") + ",";
  json += "\"uptime\":" + String(diagnosticData.uptime);
  if (diagnosticData.temperature != -999) {
    json += ",\"temperature\":" + String(diagnosticData.temperature, 1);
  }
  json += "}";
  server.send(200, "application/json", json);
}

void handleMemory() {
  collectDetailedMemory();
  String json;
  json.reserve(500);
  json = "{";
  json += "\"heap\":{\"total\":" + String(diagnosticData.heapSize) +
          ",\"free\":" + String(diagnosticData.freeHeap) +
          ",\"used\":" + String(diagnosticData.heapSize - diagnosticData.freeHeap) + "},";
  json += "\"psram\":{\"total\":" + String(detailedMemory.psramTotal) +
          ",\"free\":" + String(detailedMemory.psramFree) +
          ",\"used\":" + String(detailedMemory.psramUsed) + "},";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1);
  json += "}";
  server.send(200, "application/json", json);
}

void handleWiFiInfo() {
  collectDiagnosticInfo();
  String json;
  json.reserve(400);
  json = "{";
  json += "\"connected\":" + String(WiFi.status() == WL_CONNECTED ? "true" : "false") + ",";
  json += "\"ssid\":\"" + diagnosticData.wifiSSID + "\",";
  json += "\"rssi\":" + String(diagnosticData.wifiRSSI) + ",";
  json += "\"quality_key\":\"" + String(getWiFiSignalQualityKey()) + "\",";
  json += "\"quality\":\"" + getWiFiSignalQuality() + "\",";
  json += "\"ip\":\"" + diagnosticData.ipAddress + "\",";
  json += "\"gateway\":\"" + WiFi.gatewayIP().toString() + "\",";
  json += "\"dns\":\"" + WiFi.dnsIP().toString() + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

void handlePeripherals() {
  scanI2C();
  String json;
  json.reserve(300);
  json = "{";
  json += "\"i2c\":{\"count\":" + String(diagnosticData.i2cCount) +
          ",\"devices\":\"" + diagnosticData.i2cDevices + "\"},";
  json += "\"gpio\":{\"total\":" + String(diagnosticData.totalGPIO) +
          ",\"list\":\"" + diagnosticData.gpioList + "\"}";
  json += "}";
  server.send(200, "application/json", json);
}

void handleLedsInfo() {
  String json;
  json.reserve(400);
  json = "{";
  json += "\"builtin\":{\"pin\":" + String(BUILTIN_LED_PIN) +
          ",\"status\":\"" + builtinLedTestResult + "\"},";
  json += "\"neopixel\":{\"pin\":" + String(LED_PIN) +
          ",\"count\":" + String(LED_COUNT) +
          ",\"status\":\"" + neopixelTestResult + "\"}";
  json += "}";
  server.send(200, "application/json", json);
}

void handleScreensInfo() {
  String json;
  json.reserve(300);
  json = "{";  
  #if ENABLE_TFT_DISPLAY
  json += "\"tft\":{\"available\":" + String(tftAvailable ? "true" : "false") +
          ",\"status\":\"" + tftTestResult + "\",";
  json += "\"pins\":{\"mosi\":" + String(TFT_MOSI) + ",\"sclk\":" + String(TFT_SCLK) +
          ",\"cs\":" + String(TFT_CS) + ",\"dc\":" + String(TFT_DC) +
          ",\"rst\":" + String(TFT_RST) + ",\"bl\":" + String(TFT_BL) + "},";
  json += "\"resolution\":\"" + String(TFT_WIDTH) + "x" + String(TFT_HEIGHT) + "\"}";
  #else
  json += "\"tft\":{\"available\":false, \"status\":\"disabled\"}";
  #endif

  json += "}";
  server.send(200, "application/json", json);
}

void handleOverview() {
  collectDiagnosticInfo();
  collectDetailedMemory();
  scanI2C();

  String json;
  json.reserve(2500);  // Reserve memory for the complete overview response
  json = "{";

  // Chip info
  json += "\"chip\":{";
  json += "\"model\":\"" + diagnosticData.chipModel + "\",";
  json += "\"revision\":\"" + diagnosticData.chipRevision + "\",";
  json += "\"cores\":" + String(diagnosticData.cpuCores) + ",";
  json += "\"freq\":" + String(diagnosticData.cpuFreqMHz) + ",";
  json += "\"mac\":\"" + diagnosticData.macAddress + "\",";
  json += "\"uptime\":" + String(diagnosticData.uptime);
  if (diagnosticData.temperature != -999) {
    json += ",\"temperature\":" + String(diagnosticData.temperature, 1);
  } else {
    json += ",\"temperature\":-999";
  }
  json += "},";

  // Memory info
  json += "\"memory\":{";
  json += "\"flash\":{\"real\":" + String(detailedMemory.flashSizeReal) +
          ",\"type\":\"" + getFlashType() + "\",\"speed\":\"" + getFlashSpeed() + "\"},";
  json += "\"sram\":{\"total\":" + String(detailedMemory.sramTotal) +
          ",\"free\":" + String(detailedMemory.sramFree) +
          ",\"used\":" + String(detailedMemory.sramUsed) + "},";
  json += "\"psram\":{\"total\":" + String(detailedMemory.psramTotal) +
          ",\"free\":" + String(detailedMemory.psramFree) +
          ",\"used\":" + String(detailedMemory.psramUsed) + "},";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1);
  json += "},";

  // WiFi info - Use translation key instead of translated string
  json += "\"wifi\":{";
  json += "\"ssid\":\"" + diagnosticData.wifiSSID + "\",";
  json += "\"rssi\":" + String(diagnosticData.wifiRSSI) + ",";
  json += "\"quality_key\":\"" + String(getWiFiSignalQualityKey()) + "\",";  // Return key, not translated string
  json += "\"quality\":\"" + getWiFiSignalQuality() + "\",";  // Keep for backward compatibility
  json += "\"ip\":\"" + diagnosticData.ipAddress + "\"";
  json += "},";

  // GPIO info
  json += "\"gpio\":{";
  json += "\"total\":" + String(diagnosticData.totalGPIO) + ",";
  json += "\"i2c_count\":" + String(diagnosticData.i2cCount) + ",";
  json += "\"i2c_devices\":\"" + diagnosticData.i2cDevices + "\"";
  json += "}";

  json += "}";
  server.send(200, "application/json", json);
}

void handleMemoryDetails() {
  collectDetailedMemory();

  String json;
  json.reserve(450);  // Reserve memory to avoid reallocations
  json = "{\"flash\":{\"real\":" + String(detailedMemory.flashSizeReal) + ",\"chip\":" + String(detailedMemory.flashSizeChip) + "},";
  json += "\"psram\":{\"available\":" + String(detailedMemory.psramAvailable ? "true" : "false") +
          ",\"configured\":" + String(detailedMemory.psramConfigured ? "true" : "false") +
          ",\"supported\":" + String(detailedMemory.psramBoardSupported ? "true" : "false") +
          ",\"type\":\"" + String(detailedMemory.psramType ? detailedMemory.psramType : Texts::unknown.str()) + "\"" +
          ",\"total\":" + String(detailedMemory.psramTotal) + ",\"free\":" + String(detailedMemory.psramFree) + "},";
  json += "\"sram\":{\"total\":" + String(detailedMemory.sramTotal) + ",\"free\":" + String(detailedMemory.sramFree) + "},";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1) + ",\"status\":\"" + detailedMemory.memoryStatus + "\"}";

  server.send(200, "application/json", json);
}

// ========== EXPORTS ==========
void handleExportTXT() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  String txt;
  txt.reserve(4500);  // Reserve memory to avoid reallocations during export
  txt = "========================================\r\n";
  txt += String(Texts::title) + " " + String(Texts::version) + String(PROJECT_VERSION) + "\r\n";
  txt += "========================================\r\n\r\n";
  
  txt += "=== CHIP ===\r\n";
  txt += String(Texts::model) + ": " + diagnosticData.chipModel + " " + String(Texts::revision) + diagnosticData.chipRevision + "\r\n";
  txt += "CPU: " + String(diagnosticData.cpuCores) + " " + String(Texts::cores) + " @ " + String(diagnosticData.cpuFreqMHz) + " MHz\r\n";
  txt += "MAC WiFi: " + diagnosticData.macAddress + "\r\n";
  txt += "SDK: " + diagnosticData.sdkVersion + "\r\n";
  txt += "ESP-IDF: " + diagnosticData.idfVersion + "\r\n";
  if (diagnosticData.temperature != -999) {
    txt += String(Texts::cpu_temp) + ": " + String(diagnosticData.temperature, 1) + " °C\r\n";
  }
  txt += "\r\n";
  
  txt += "=== " + String(Texts::memory_details) + " ===\r\n";
  txt += "Flash (" + String(Texts::board) + "): " + String(detailedMemory.flashSizeReal / 1048576.0, 2) + " MB\r\n";
  txt += "Flash (IDE): " + String(detailedMemory.flashSizeChip / 1048576.0, 2) + " MB\r\n";
  txt += String(Texts::flash_type) + ": " + getFlashType() + " @ " + getFlashSpeed() + "\r\n";
  txt += "PSRAM: " + String(detailedMemory.psramTotal / 1048576.0, 2) + " MB";
  if (detailedMemory.psramAvailable) {
    txt += " (" + String(Texts::free) + ": " + String(detailedMemory.psramFree / 1048576.0, 2) + " MB)\r\n";
  } else if (detailedMemory.psramBoardSupported) {
    String psramHint = String(Texts::enable_psram_hint);
    psramHint.replace("%TYPE%", detailedMemory.psramType ? detailedMemory.psramType : "PSRAM");
    txt += " (" + String(Texts::supported_not_enabled) + " - " + psramHint + ")\r\n";
  } else {
    txt += " (" + String(Texts::not_detected) + ")\r\n";
  }
  txt += "SRAM: " + String(detailedMemory.sramTotal / 1024.0, 2) + " KB";
  txt += " (" + String(Texts::free) + ": " + String(detailedMemory.sramFree / 1024.0, 2) + " KB)\r\n";
  txt += String(Texts::memory_fragmentation) + ": " + String(detailedMemory.fragmentationPercent, 1) + "%\r\n";
  txt += String(Texts::memory_status) + ": " + detailedMemory.memoryStatus + "\r\n";
  txt += "\r\n";
  
  txt += "=== WIFI ===\r\n";
  txt += "SSID: " + diagnosticData.wifiSSID + "\r\n";
  txt += "RSSI: " + String(diagnosticData.wifiRSSI) + " dBm (" + getWiFiSignalQuality() + ")\r\n";
  txt += "IP: " + diagnosticData.ipAddress + "\r\n";
  txt += "Lien constant: " + getStableAccessURL() + " (" + String(diagnosticData.mdnsAvailable ? "actif" : "en attente") + ")\r\n";
  txt += String(Texts::subnet_mask) + ": " + WiFi.subnetMask().toString() + "\r\n";
  txt += String(Texts::gateway) + ": " + WiFi.gatewayIP().toString() + "\r\n";
  txt += "DNS: " + WiFi.dnsIP().toString() + "\r\n";
  txt += "\r\n";

  txt += "=== GPIO ===\r\n";
  txt += String(Texts::total_gpio) + ": " + String(diagnosticData.totalGPIO) + " " + String(Texts::pins) + "\r\n";
  txt += String(Texts::gpio_list) + ": " + diagnosticData.gpioList + "\r\n";
  txt += "\r\n";
  
  txt += "=== " + String(Texts::i2c_peripherals) + " ===\r\n";
  txt += String(Texts::device_count) + ": " + String(diagnosticData.i2cCount) + " - " + diagnosticData.i2cDevices + "\r\n";
  txt += "SPI: " + spiInfo + "\r\n";
  txt += "\r\n";
  
  txt += "=== " + String(Texts::test) + " ===\r\n";
  txt += String(Texts::builtin_led) + ": " + builtinLedTestResult + "\r\n";
  txt += String(Texts::neopixel) + ": " + neopixelTestResult + "\r\n";
  txt += "TFT: " + tftTestResult + "\r\n";
  txt += "ADC: " + adcTestResult + "\r\n";
  txt += "PWM: " + pwmTestResult + "\r\n";
  txt += "\r\n";
  
  txt += "=== " + String(Texts::performance_bench) + " ===\r\n";
  if (diagnosticData.cpuBenchmark > 0) {
    txt += "CPU: " + String(diagnosticData.cpuBenchmark) + " us (" + String(100000.0 / diagnosticData.cpuBenchmark, 2) + " MFLOPS)\r\n";
    txt += String(Texts::memory_benchmark) + ": " + String(diagnosticData.memBenchmark) + " us\r\n";
  } else {
    txt += String(Texts::not_tested) + "\r\n";
  }
  txt += "Stress test: " + stressTestResult + "\r\n";
  txt += "\r\n";
  
  unsigned long seconds = diagnosticData.uptime / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  txt += "=== SYSTEM ===\r\n";
  txt += String(Texts::uptime) + ": " + String(days) + "d " + String(hours % 24) + "h " + String(minutes % 60) + "m\r\n";
  txt += String(Texts::last_reset) + ": " + getResetReason() + "\r\n";
  txt += "\r\n";
  txt += "========================================\r\n";
  txt += String(Texts::export_generated) + " " + String(millis()/1000) + "s " + String(Texts::export_after_boot) + "\r\n";
  txt += "========================================\r\n";

  server.sendHeader("Content-Disposition", "attachment; filename=esp32_diagnostic_v"+ String(PROJECT_VERSION) +".txt");
  server.send(200, "text/plain; charset=utf-8", txt);
}

void handleExportJSON() {
  collectDiagnosticInfo();
  collectDetailedMemory();
  String stableUrl = getStableAccessURL();

  String json;
  json.reserve(3500);  // Reserve memory to avoid reallocations during export
  json = "{";
  json += "\"chip\":{";
  json += "\"model\":\"" + diagnosticData.chipModel + "\",";
  json += "\"revision\":\"" + diagnosticData.chipRevision + "\",";
  json += "\"cores\":" + String(diagnosticData.cpuCores) + ",";
  json += "\"freq_mhz\":" + String(diagnosticData.cpuFreqMHz) + ",";
  json += "\"mac\":\"" + diagnosticData.macAddress + "\",";
  json += "\"sdk\":\"" + diagnosticData.sdkVersion + "\",";
  json += "\"idf\":\"" + diagnosticData.idfVersion + "\"";
  if (diagnosticData.temperature != -999) {
    json += ",\"temperature\":" + String(diagnosticData.temperature, 1);
  }
  json += "},";
  
  json += "\"memory\":{";
  json += "\"flash_real_mb\":" + String(detailedMemory.flashSizeReal / 1048576.0, 2) + ",";
  json += "\"flash_config_mb\":" + String(detailedMemory.flashSizeChip / 1048576.0, 2) + ",";
  json += "\"flash_type\":\"" + getFlashType() + "\",";
  json += "\"flash_speed\":\"" + getFlashSpeed() + "\",";
  json += "\"psram_mb\":" + String(detailedMemory.psramTotal / 1048576.0, 2) + ",";
  json += "\"psram_free_mb\":" + String(detailedMemory.psramFree / 1048576.0, 2) + ",";
  json += "\"psram_available\":" + String(detailedMemory.psramAvailable ? "true" : "false") + ",";
  json += "\"psram_supported\":" + String(detailedMemory.psramBoardSupported ? "true" : "false") + ",";
  json += "\"psram_type\":\"" + String(detailedMemory.psramType ? detailedMemory.psramType : Texts::unknown.str()) + "\",";
  json += "\"sram_kb\":" + String(detailedMemory.sramTotal / 1024.0, 2) + ",";
  json += "\"sram_free_kb\":" + String(detailedMemory.sramFree / 1024.0, 2) + ",";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1) + ",";
  json += "\"status\":\"" + detailedMemory.memoryStatus + "\"";
  json += "},";
  
  json += "\"wifi\":{";
  json += "\"ssid\":\"" + diagnosticData.wifiSSID + "\",";
  json += "\"rssi\":" + String(diagnosticData.wifiRSSI) + ",";
  json += "\"quality\":\"" + getWiFiSignalQuality() + "\",";
  json += "\"ip\":\"" + diagnosticData.ipAddress + "\",";
  json += "\"mdns_ready\":" + String(diagnosticData.mdnsAvailable ? "true" : "false") + ",";
  json += "\"stable_url\":\"" + jsonEscape(stableUrl.c_str()) + "\",";
  json += "\"subnet\":\"" + WiFi.subnetMask().toString() + "\",";
  json += "\"gateway\":\"" + WiFi.gatewayIP().toString() + "\",";
  json += "\"dns\":\"" + WiFi.dnsIP().toString() + "\"";
  json += "},";

  json += "\"gpio\":{";
  json += "\"total\":" + String(diagnosticData.totalGPIO) + ",";
  json += "\"list\":\"" + diagnosticData.gpioList + "\"";
  json += "},";
  
  json += "\"peripherals\":{";
  json += "\"i2c_count\":" + String(diagnosticData.i2cCount) + ",";
  json += "\"i2c_devices\":\"" + diagnosticData.i2cDevices + "\",";
  json += "\"spi\":\"" + spiInfo + "\"";
  json += "},";
  
  json += "\"hardware_tests\":{";
  json += "\"builtin_led\":\"" + builtinLedTestResult + "\",";
  json += "\"neopixel\":\"" + neopixelTestResult + "\",";
  json += "\"tft\":\"" + tftTestResult + "\",";
  json += "\"adc\":\"" + adcTestResult + "\",";
  json += "\"pwm\":\"" + pwmTestResult + "\"";
  json += "},";
  
  json += "\"performance\":{";
  if (diagnosticData.cpuBenchmark > 0) {
    json += "\"cpu_us\":" + String(diagnosticData.cpuBenchmark) + ",";
    json += "\"cpu_mflops\":" + String(100000.0 / diagnosticData.cpuBenchmark, 2) + ",";
    json += "\"memory_us\":" + String(diagnosticData.memBenchmark);
  } else {
    json += "\"benchmarks\":\"not_run\"";
  }
  json += ",\"stress_test\":\"" + stressTestResult + "\"";
  json += "},";
  
  json += "\"system\":{";
  json += "\"uptime_ms\":" + String(diagnosticData.uptime) + ",";
  json += "\"reset_reason\":\"" + getResetReason() + "\",";
  json += "\"language\":\"en\"";
  json += "}";
  
  json += "}";
  
  server.sendHeader("Content-Disposition", "attachment; filename=esp32_diagnostic_v" + String(PROJECT_VERSION) + ".json");
  server.send(200, "application/json", json);
}

void handleExportCSV() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  String csv;
  csv.reserve(4000);  // Reserve memory to avoid reallocations during export
  csv = String(Texts::category) + "," + String(Texts::parameter) + "," + String(Texts::value) + "\r\n";
  
  csv += "Chip," + String(Texts::model) + "," + diagnosticData.chipModel + "\r\n";
  csv += "Chip," + String(Texts::revision) + "," + diagnosticData.chipRevision + "\r\n";
  csv += "Chip,CPU " + String(Texts::cores) + "," + String(diagnosticData.cpuCores) + "\r\n";
  csv += "Chip," + String(Texts::frequency) + " MHz," + String(diagnosticData.cpuFreqMHz) + "\r\n";
  csv += "Chip,MAC," + diagnosticData.macAddress + "\r\n";
  if (diagnosticData.temperature != -999) {
    csv += "Chip," + String(Texts::cpu_temp) + " C," + String(diagnosticData.temperature, 1) + "\r\n";
  }
  
  csv += String(Texts::memory_details) + ",Flash MB (" + String(Texts::board) + ")," + String(detailedMemory.flashSizeReal / 1048576.0, 2) + "\r\n";
  csv += String(Texts::memory_details) + ",Flash MB (config)," + String(detailedMemory.flashSizeChip / 1048576.0, 2) + "\r\n";
  csv += String(Texts::memory_details) + "," + String(Texts::flash_type) + "," + getFlashType() + "\r\n";
  csv += String(Texts::memory_details) + ",PSRAM MB," + String(detailedMemory.psramTotal / 1048576.0, 2) + "\r\n";
  csv += String(Texts::memory_details) + ",PSRAM " + String(Texts::free) + " MB," + String(detailedMemory.psramFree / 1048576.0, 2) + "\r\n";
  String psramStatus = detailedMemory.psramAvailable ? String(Texts::detected_active)
                                                    : (detailedMemory.psramBoardSupported ? String(Texts::supported_not_enabled)
                                                                                        : String(Texts::not_detected));
  csv += String(Texts::memory_details) + ",PSRAM Statut,\"" + psramStatus + "\"\r\n";
  if (!detailedMemory.psramAvailable && detailedMemory.psramBoardSupported) {
    String psramHint = String(Texts::enable_psram_hint);
    psramHint.replace("%TYPE%", detailedMemory.psramType ? detailedMemory.psramType : "PSRAM");
    psramHint.replace("\"", "'");
    csv += String(Texts::memory_details) + ",PSRAM Conseils,\"" + psramHint + "\"\r\n";
  }
  csv += String(Texts::memory_details) + ",SRAM KB," + String(detailedMemory.sramTotal / 1024.0, 2) + "\r\n";
  csv += String(Texts::memory_details) + ",SRAM " + String(Texts::free) + " KB," + String(detailedMemory.sramFree / 1024.0, 2) + "\r\n";
  csv += String(Texts::memory_details) + "," + String(Texts::memory_fragmentation) + " %," + String(detailedMemory.fragmentationPercent, 1) + "\r\n";
  
  csv += "WiFi,SSID," + diagnosticData.wifiSSID + "\r\n";
  csv += "WiFi,RSSI dBm," + String(diagnosticData.wifiRSSI) + "\r\n";
  csv += "WiFi,IP," + diagnosticData.ipAddress + "\r\n";
  csv += "WiFi," + String(Texts::gateway) + "," + WiFi.gatewayIP().toString() + "\r\n";

  csv += "GPIO," + String(Texts::total_gpio) + "," + String(diagnosticData.totalGPIO) + "\r\n";
  
  csv += String(Texts::i2c_peripherals) + "," + String(Texts::device_count) + "," + String(diagnosticData.i2cCount) + "\r\n";
  csv += String(Texts::i2c_peripherals) + "," + String(Texts::devices) + "," + diagnosticData.i2cDevices + "\r\n";
  
  csv += String(Texts::test) + "," + String(Texts::builtin_led) + "," + builtinLedTestResult + "\r\n";
  csv += String(Texts::test) + "," + String(Texts::neopixel) + "," + neopixelTestResult + "\r\n";
  csv += String(Texts::test) + ",TFT," + tftTestResult + "\r\n";
  csv += String(Texts::test) + ",ADC," + adcTestResult + "\r\n";
  csv += String(Texts::test) + ",PWM," + pwmTestResult + "\r\n";
  
  if (diagnosticData.cpuBenchmark > 0) {
    csv += String(Texts::performance_bench) + ",CPU us," + String(diagnosticData.cpuBenchmark) + "\r\n";
    csv += String(Texts::performance_bench) + "," + String(Texts::memory_benchmark) + " us," + String(diagnosticData.memBenchmark) + "\r\n";
  }
  
  csv += "System," + String(Texts::uptime) + " ms," + String(diagnosticData.uptime) + "\r\n";
  csv += "System," + String(Texts::last_reset) + "," + getResetReason() + "\r\n";
  
  server.sendHeader("Content-Disposition", "attachment; filename=esp32_diagnostic_v" + String(PROJECT_VERSION) + ".csv");
  server.send(200, "text/csv; charset=utf-8", csv);
}

void handlePrintVersion() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>" + String(Texts::title) + " " + String(Texts::version) + String(PROJECT_VERSION) + "</title>";
  html += "<style>";
  html += "@page{size:A4;margin:10mm}";
  html += "body{font:11px Arial;margin:10px;color:#333}";
  html += "h1{font-size:18px;margin:0 0 5px;border-bottom:3px solid #667eea;color:#667eea;padding-bottom:5px}";
  html += "h2{font-size:14px;margin:15px 0 8px;color:#667eea;border-bottom:1px solid #ddd;padding-bottom:3px}";
  html += ".section{margin-bottom:20px}";
  html += ".grid{display:grid;grid-template-columns:1fr 1fr;gap:10px;margin:10px 0}";
  html += ".row{display:flex;margin:3px 0;padding:5px;background:#f8f9fa;border-radius:3px}";
  html += ".row b{min-width:150px;color:#667eea}";
  html += ".badge{display:inline-block;padding:2px 8px;border-radius:10px;font-size:10px;font-weight:bold}";
  html += ".badge-success{background:#d4edda;color:#155724}";
  html += ".badge-warning{background:#fff3cd;color:#856404}";
  html += ".badge-danger{background:#f8d7da;color:#721c24}";
  html += "table{width:100%;border-collapse:collapse;margin:10px 0;font-size:10px}";
  html += "th{background:#667eea;color:#fff;padding:5px;text-align:left}";
  html += "td{border:1px solid #ddd;padding:4px}";
  html += ".footer{margin-top:20px;padding-top:10px;border-top:1px solid #ddd;font-size:9px;color:#666;text-align:center}";
  html += "</style></head>";
  html += "<body onload='window.print()'>";
  
  // Header traduit
  html += "<h1>" + String(Texts::title) + " " + String(Texts::version) + String(PROJECT_VERSION) + "</h1>";
  html += "<div style='margin:10px 0;font-size:12px;color:#666'>";
  html += String(Texts::export_generated) + " " + String(millis()/1000) + "s " + String(Texts::export_after_boot) + " | IP: " + diagnosticData.ipAddress;
  html += "</div>";
  
  // Chip
  html += "<div class='section'>";
  html += "<h2>" + String(Texts::chip_info) + "</h2>";
  html += "<div class='grid'>";
  html += "<div class='row'><b>" + String(Texts::full_model) + ":</b><span>" + diagnosticData.chipModel + " Rev" + diagnosticData.chipRevision + "</span></div>";
  html += "<div class='row'><b>" + String(Texts::cpu_cores) + ":</b><span>" + String(diagnosticData.cpuCores) + " " + String(Texts::cores) + " @ " + String(diagnosticData.cpuFreqMHz) + " MHz</span></div>";
  html += "<div class='row'><b>" + String(Texts::mac_wifi) + ":</b><span>" + diagnosticData.macAddress + "</span></div>";
  html += "<div class='row'><b>" + String(Texts::sdk_version) + ":</b><span>" + diagnosticData.sdkVersion + "</span></div>";
  html += "<div class='row'><b>" + String(Texts::idf_version) + ":</b><span>" + diagnosticData.idfVersion + "</span></div>";
  if (diagnosticData.temperature != -999) {
    html += "<div class='row'><b>" + String(Texts::cpu_temp) + ":</b><span>" + String(diagnosticData.temperature, 1) + " °C</span></div>";
  }

  unsigned long seconds = diagnosticData.uptime / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  html += "<div class='row'><b>" + String(Texts::uptime) + ":</b><span>" + formatUptime(days, hours % 24, minutes % 60) + "</span></div>";
  html += "<div class='row'><b>" + String(Texts::last_reset) + ":</b><span>" + getResetReason() + "</span></div>";
  html += "</div></div>";

  // Mémoire
  html += "<div class='section'>";
  html += "<h2>" + String(Texts::memory_details) + "</h2>";
  html += "<table>";
  html += "<tr><th>" + String(Texts::category) + "</th><th>" + String(Texts::total_size) + "</th><th>" + String(Texts::free) + "</th><th>" + String(Texts::used) + "</th><th>" + String(Texts::status) + "</th></tr>";

  // Flash
  bool flashMatch = (detailedMemory.flashSizeReal == detailedMemory.flashSizeChip);
  html += "<tr><td><b>" + String(Texts::flash_memory) + "</b></td>";
  html += "<td>" + String(detailedMemory.flashSizeReal / 1048576.0, 1) + " MB</td>";
  html += "<td>-</td><td>-</td>";
  html += "<td><span class='badge " + String(flashMatch ? "badge-success'>" + String(Texts::ok) : "badge-warning'>" + String(Texts::ide_config)) + "</span></td></tr>";

  // PSRAM
  html += "<tr><td><b>" + String(Texts::psram_external) + "</b></td>";
  html += "<td>" + String(detailedMemory.psramTotal / 1048576.0, 1) + " MB</td>";
  if (detailedMemory.psramAvailable) {
    html += "<td>" + String(detailedMemory.psramFree / 1048576.0, 1) + " MB</td>";
    html += "<td>" + String(detailedMemory.psramUsed / 1048576.0, 1) + " MB</td>";
    html += "<td><span class='badge badge-success'>" + String(Texts::detected_active) + "</span></td>";
  } else if (detailedMemory.psramBoardSupported) {
    html += "<td>-</td><td>-</td>";
    String psramHint = String(Texts::enable_psram_hint);
    psramHint.replace("%TYPE%", detailedMemory.psramType ? detailedMemory.psramType : "PSRAM");
    html += "<td><span class='badge badge-warning'>" + String(Texts::supported_not_enabled) + "</span><br><small>" + psramHint + "</small></td>";
  } else {
    html += "<td>-</td><td>-</td>";
    html += "<td><span class='badge badge-danger'>" + String(Texts::not_detected) + "</span></td>";
  }
  html += "</tr>";

  // SRAM
  html += "<tr><td><b>" + String(Texts::internal_sram) + "</b></td>";
  html += "<td>" + String(detailedMemory.sramTotal / 1024.0, 1) + " KB</td>";
  html += "<td>" + String(detailedMemory.sramFree / 1024.0, 1) + " KB</td>";
  html += "<td>" + String(detailedMemory.sramUsed / 1024.0, 1) + " KB</td>";
  html += "<td><span class='badge badge-success'>" + detailedMemory.memoryStatus + "</span></td></tr>";
  html += "</table>";
  html += "<div class='row'><b>" + String(Texts::memory_fragmentation) + ":</b><span>" + String(detailedMemory.fragmentationPercent, 1) + "% - " + detailedMemory.memoryStatus + "</span></div>";
  html += "</div>";

  // WiFi
  html += "<div class='section'>";
  html += "<h2>" + String(Texts::wifi_connection) + "</h2>";
  html += "<div class='grid'>";
  html += "<div class='row'><b>" + String(Texts::connected_ssid) + ":</b><span>" + diagnosticData.wifiSSID + "</span></div>";
  html += "<div class='row'><b>" + String(Texts::signal_power) + ":</b><span>" + String(diagnosticData.wifiRSSI) + " dBm</span></div>";
  html += "<div class='row'><b>" + String(Texts::signal_quality) + ":</b><span>" + getWiFiSignalQuality() + "</span></div>";
  html += "<div class='row'><b>" + String(Texts::ip_address) + ":</b><span>" + diagnosticData.ipAddress + "</span></div>";
  html += "<div class='row'><b>" + String(Texts::subnet_mask) + ":</b><span>" + WiFi.subnetMask().toString() + "</span></div>";
  html += "<div class='row'><b>" + String(Texts::dns) + ":</b><span>" + WiFi.dnsIP().toString() + "</span></div>";
  html += "<div class='row'><b>" + String(Texts::gateway) + ":</b><span>" + WiFi.gatewayIP().toString() + "</span></div>";
  html += "</div></div>";

  // GPIO et Périphériques
  html += "<div class='section'>";
  html += "<h2>" + String(Texts::gpio_interfaces) + "</h2>";
  html += "<div class='grid'>";
  html += "<div class='row'><b>" + String(Texts::total_gpio) + ":</b><span>" + String(diagnosticData.totalGPIO) + " " + String(Texts::pins) + "</span></div>";
  html += "<div class='row'><b>" + String(Texts::i2c_peripherals) + ":</b><span>" + String(diagnosticData.i2cCount) + " " + String(Texts::devices) + " - " + diagnosticData.i2cDevices + "</span></div>";
  html += "<div class='row'><b>" + String(Texts::spi_bus) + ":</b><span>" + spiInfo + "</span></div>";
  html += "</div></div>";

  // Tests Matériels
  html += "<div class='section'>";
  html += "<h2>" + String(Texts::nav_tests) + "</h2>";
  html += "<table>";
  html += "<tr><th>" + String(Texts::parameter) + "</th><th>" + String(Texts::status) + "</th></tr>";
  html += "<tr><td>" + String(Texts::builtin_led) + "</td><td>" + builtinLedTestResult + "</td></tr>";
  html += "<tr><td>" + String(Texts::neopixel) + "</td><td>" + neopixelTestResult + "</td></tr>";
  html += "<tr><td>" + String(Texts::oled_screen) + "</td><td>" + tftTestResult + "</td></tr>";
  html += "<tr><td>" + String(Texts::adc_test) + "</td><td>" + adcTestResult + "</td></tr>";
  html += "<tr><td>" + String(Texts::pwm_test) + "</td><td>" + pwmTestResult + "</td></tr>";
  html += "</table></div>";

  // Performance
  if (diagnosticData.cpuBenchmark > 0) {
    html += "<div class='section'>";
    html += "<h2>" + String(Texts::performance_bench) + "</h2>";
    html += "<div class='grid'>";
    html += "<div class='row'><b>" + String(Texts::cpu_benchmark) + ":</b><span>" + String(diagnosticData.cpuBenchmark) + " µs (" + String(100000.0 / diagnosticData.cpuBenchmark, 2) + " MFLOPS)</span></div>";
    html += "<div class='row'><b>" + String(Texts::memory_benchmark) + ":</b><span>" + String(diagnosticData.memBenchmark) + " µs</span></div>";
    html += "<div class='row'><b>" + String(Texts::memory_stress) + ":</b><span>" + stressTestResult + "</span></div>";
    html += "</div></div>";
  }
  
  // Footer
  html += "<div class='footer'>";
  html += "ESP32 Diagnostic v"+ String(PROJECT_VERSION) + " | " + diagnosticData.chipModel + " | MAC: " + diagnosticData.macAddress;
  html += "</div>";
  
  html += "</body></html>";
  
  server.send(200, "text/html; charset=utf-8", html);
}

String htmlEscape(const String& raw) {
  String escaped;
  escaped.reserve(raw.length() * 6);
  for (size_t i = 0; i < raw.length(); ++i) {
    char c = raw[i];
    switch (c) {
      case '&':
        escaped += "&amp;";
        break;
      case '<':
        escaped += "&lt;";
        break;
      case '>':
        escaped += "&gt;";
        break;
      case '\"':
        escaped += "&quot;";
        break;
      case '\'':
        escaped += "&#39;";
        break;
      default:
        escaped += c;
        break;
    }
  }
  return escaped;
}

String jsonEscape(const char* raw) {
  if (raw == nullptr) {
    return "";
  }

  const size_t rawLength = strlen(raw);
  String escaped;
  escaped.reserve(rawLength * 2);
  while (*raw) {
    char c = *raw++;
    switch (c) {
      case '\\':
        escaped += "\\\\";
        break;
      case '"':
        escaped += "\\\"";
        break;
      case '\n':
        escaped += "\\n";
        break;
      case '\r':
        escaped += "\\r";
        break;
      case '\t':
        escaped += "\\t";
        break;
      default:
        escaped += c;
        break;
    }
  }
  return escaped;
}

static inline void appendJsonField(String& json, bool& first, const JsonFieldSpec& field) {
  if (!first) {
    json += ',';
  }
  first = false;
  json += '"';
  json += field.key;
  json += '"';
  json += ':';
  if (field.raw) {
    json += field.value;
  } else {
    json += '"';
    json += jsonEscape(field.value.c_str());
    json += '"';
  }
}

String buildJsonObject(std::initializer_list<JsonFieldSpec> fields) {
  String json;
  json.reserve(fields.size() * 50 + 10);  // Estimate size based on field count
  json = "{";
  bool first = true;
  for (const auto& field : fields) {
    appendJsonField(json, first, field);
  }
  json += '}';
  return json;
}

inline void sendJsonResponse(int statusCode, std::initializer_list<JsonFieldSpec> fields) {
  server.send(statusCode, "application/json", buildJsonObject(fields));
}

static String buildActionResponseJson(bool success,
                                      const String& message,
                                      std::initializer_list<JsonFieldSpec> extraFields) {
  String json;
  json.reserve(extraFields.size() * 50 + message.length() + 50);  // Estimate size
  json = "{";
  bool first = true;
  appendJsonField(json, first, jsonBoolField("success", success));
  if (message.length() > 0) {
    appendJsonField(json, first, jsonStringField("message", message));
  }
  for (const auto& field : extraFields) {
    appendJsonField(json, first, field);
  }
  json += '}';
  return json;
}

inline void sendActionResponse(int statusCode,
                               bool success,
                               const String& message,
                               std::initializer_list<JsonFieldSpec> extraFields) {
  server.send(statusCode, "application/json", buildActionResponseJson(success, message, extraFields));
}

inline void sendOperationSuccess(const String& message,
                                 std::initializer_list<JsonFieldSpec> extraFields) {
  sendActionResponse(200, true, message, extraFields);
}

inline void sendOperationError(int statusCode,
                               const String& message,
                               std::initializer_list<JsonFieldSpec> extraFields) {
  sendActionResponse(statusCode, false, message, extraFields);
}

static inline void appendInfoItem(String& chunk,
                                  const char* labelKey,
                                  const char* labelText,
                                  const String& valueText,
                                  const String& valueAttrs = String(),
                                  const String& wrapperAttrs = String()) {
  chunk += F("<div class='info-item'");
  if (wrapperAttrs.length() > 0) {
    chunk += ' ';
    chunk += wrapperAttrs;
  }
  chunk += F("><div class='info-label");
  if (labelKey != nullptr && labelKey[0] != '\0') {
    chunk += F(" data-i18n='");
    chunk += labelKey;
    chunk += F("'>");
  } else {
    chunk += F("'>");
  }
  chunk += labelText;
  chunk += F("</div><div class='info-value'");
  if (valueAttrs.length() > 0) {
    chunk += ' ';
    chunk += valueAttrs;
  }
  chunk += F(">");
  chunk += valueText;
  chunk += F("</div></div>");
}

// Dynamic bilingual string export for the web interface
String buildTranslationsJSON(Language lang) {
  String json;
  json.reserve(20000);
  json += '{';
  bool first = true;
  size_t count = 0;
  const Texts::ResourceEntry* const entries = Texts::getResourceEntries(count);
  for (size_t i = 0; i < count; ++i) {
    const Texts::ResourceEntry& entry = entries[i];
    if (entry.field == nullptr) {
      continue;
    }
    if (!first) {
      json += ',';
    }
    first = false;
    json += '"';
    json += entry.key;
    json += F("\":\"");
    const __FlashStringHelper* raw = entry.field->get(lang);
    String fieldValue;
    if (raw != nullptr) {
      fieldValue = String(raw);
    }
    json += jsonEscape(fieldValue.c_str());
    json += '"';
  }
  json += '}';
  return json;
}

String buildTranslationsJSON() {
  return buildTranslationsJSON(currentLanguage);
}





// Runtime language switching endpoint
void handleSetLanguage() {
  if (!server.hasArg("lang")) {
    sendJsonResponse(400, {
      jsonBoolField("success", false),
      jsonStringField("error", String(Texts::language_switch_error))
    });
    return;
  }

  String lang = server.arg("lang");
  lang.toLowerCase();
  Language target = LANG_EN;
  if (lang == "fr") {
    target = LANG_FR;
  } else if (lang == "en") {
    target = LANG_EN;
  } else {
    sendJsonResponse(400, {
      jsonBoolField("success", false),
      jsonStringField("error", String(Texts::language_switch_error))
    });
    return;
  }

  setLanguage(target);

  String response = F("{\"success\":true,\"language\":\"");
  response += (target == LANG_FR) ? "fr" : "en";
  response += F("\"}");
  server.send(200, "application/json; charset=utf-8", response);
}

// Language-aware translation extraction endpoint
void handleGetTranslations() {
  Language target = currentLanguage;
  if (server.hasArg("lang")) {
    String requested = server.arg("lang");
    requested.toLowerCase();
    if (requested == "fr") {
      target = LANG_FR;
    } else if (requested == "en") {
      target = LANG_EN;
    } else {
      sendJsonResponse(400, {
        jsonBoolField("success", false),
        jsonStringField("error", String(Texts::language_switch_error))
      });
      return;
    }
  }

  server.send(200, "application/json; charset=utf-8", buildTranslationsJSON(target));
}


// ========== INTERFACE WEB PRINCIPALE MULTILINGUE ==========
// Unique JavaScript handler defined in sketch (handleJavaScriptRoute)
void handleJavaScriptRoute() {
  server.send(200, "application/javascript; charset=utf-8", generateJavaScript());
}

// Modern web interface with dynamic tabs
void handleRoot() {
  server.send(200, "text/html; charset=utf-8", generateHTML());
}

void handleJavaScript() {
  server.send(200, "application/javascript; charset=utf-8", generateJavaScript());
}

// ========== SETUP COMPLET ==========
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\r\n===============================================");
  Serial.println("     DIAGNOSTIC ESP32 MULTILINGUE");
  Serial.printf("     Version %s - FR/EN\r\n", DIAGNOSTIC_VERSION_STR);
  Serial.printf("     Arduino Core %s\r\n", getArduinoCoreVersionString().c_str());
  Serial.println("===============================================\r\n");

  printPSRAMDiagnostic();

  #if ENABLE_TFT_DISPLAY
  tftSemaphore = xSemaphoreCreateMutex();
  detectTFT();
  #endif

  // WiFi
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);

  WiFi.setSleep(false);
  configureNetworkHostname();
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 3, 0)
  WiFi.setScanMethod(WIFI_ALL_CHANNEL_SCAN);
#ifdef WIFI_CONNECT_AP_BY_SIGNAL
  WiFi.setSortMethod(WIFI_CONNECT_AP_BY_SIGNAL);
#endif
#endif
  wifiMulti.addAP(WIFI_SSID_1, WIFI_PASS_1);
  wifiMulti.addAP(WIFI_SSID_2, WIFI_PASS_2);

  Serial.println("Connexion WiFi...");
  const int maxWiFiAttempts = 40;
  int attempt = 0;
  #if ENABLE_TFT_DISPLAY
  if (tftAvailable) {
    tftShowWiFiStatus(String(Texts::wifi_connection),
                       String(Texts::loading),
                       "",
                       0);
  }
  #endif
  while (wifiMulti.run() != WL_CONNECTED && attempt < maxWiFiAttempts) {
    delay(500);
    Serial.print(".");
    attempt++;
    #if ENABLE_TFT_DISPLAY
      int progress = (attempt * 100) / maxWiFiAttempts;
      if (progress > 100) {
        progress = 100;
      }
      String detail = String(Texts::loading) + " " + String(progress) + "%";
      tftShowWiFiStatus(String(Texts::wifi_connection),
                         detail,
                         "",
                         progress);
    #endif
  }

  bool wifiConnected = (WiFi.status() == WL_CONNECTED);
#if DIAGNOSTIC_HAS_MDNS
  wifiPreviouslyConnected = wifiConnected;
#endif

  if (wifiConnected) {
    Serial.println("\r\n\r\nWiFi OK!");
    Serial.printf("SSID: %s\r\n", WiFi.SSID().c_str());
    Serial.printf("IP: %s\r\n\r\n", WiFi.localIP().toString().c_str());
    #if ENABLE_TFT_DISPLAY
    if (tftAvailable) {
      String detail = String(Texts::connected) + ": " + WiFi.SSID();
      String footer = WiFi.localIP().toString();
      tftShowWiFiStatus(String(Texts::wifi_connection), detail, footer, 100);
    }
    #endif
    if (startMDNSService(true)) {
      Serial.printf("[Accès] Lien constant : %s\r\n", getStableAccessURL().c_str());
    } else {
      Serial.println("[mDNS] Initialisation différée - nouvel essai automatique");
    }
  } else {
    Serial.println("\r\n\r\nPas de WiFi\r\n");
    Serial.printf("[Accès] Lien constant disponible après connexion : %s\r\n", getStableAccessURL().c_str());
    #if ENABLE_TFT_DISPLAY
    if (tftAvailable) {
      tftShowWiFiStatus(String(Texts::wifi_connection),
                         String(Texts::disconnected),
                         getStableAccessURL(),
                         -1);
    }
    #endif
  }

  // Détections
  detectBuiltinLED();
  detectNeoPixelSupport();

  if (strip != nullptr) {
    strip->begin();
    strip->clear();
    strip->show();
  }
  
  if (ENABLE_I2C_SCAN) {
    scanI2C();
  }
  
  scanSPI();
  listPartitions();
  
  collectDiagnosticInfo();
  collectDetailedMemory();

  #if ENABLE_TFT_DISPLAY
  xTaskCreate(tftTask, "TFT_Task", TFT_TASK_STACK, NULL, 1, &tft_task_handle);
  #endif

  #if ENABLE_GPS
  xTaskCreate(gpsTask, "GPS_Task", GPS_TASK_STACK, NULL, GPS_TASK_PRIORITY, NULL);
  #endif


  // ========== ROUTES SERVEUR ==========
  server.on("/", handleRoot);
  server.on("/js/app.js", handleJavaScriptRoute);

  // **TRANSLATION API**
  server.on("/api/get-translations", handleGetTranslations);
  server.on("/api/set-language", handleSetLanguage);

  // Data endpoints
  server.on("/api/status", handleStatus);
  server.on("/api/overview", handleOverview);
  server.on("/api/system-info", handleSystemInfo);
  server.on("/api/memory", handleMemory);
  server.on("/api/wifi-info", handleWiFiInfo);
  server.on("/api/peripherals", handlePeripherals);
  server.on("/api/leds-info", handleLedsInfo);
  server.on("/api/screens-info", handleScreensInfo);

  // GPIO & WiFi
  server.on("/api/test-gpio", handleTestGPIO);
  server.on("/api/wifi-scan", handleWiFiScan);
  server.on("/api/i2c-scan", handleI2CScan);

  // LED intégrée
  server.on("/api/builtin-led-config", handleBuiltinLEDConfig);
  server.on("/api/builtin-led-test", handleBuiltinLEDTest);
  server.on("/api/builtin-led-control", handleBuiltinLEDControl);
  
  // NeoPixel
  server.on("/api/neopixel-config", handleNeoPixelConfig);
  server.on("/api/neopixel-test", handleNeoPixelTest);
  server.on("/api/neopixel-pattern", handleNeoPixelPattern);
  server.on("/api/neopixel-color", handleNeoPixelColor);
  
  // Écrans
  server.on("/api/tft-test", handleTFTTest);
  server.on("/api/tft-message", handleTFTMessage);
  server.on("/api/tft-backlight-on", []() { digitalWrite(TFT_BL, HIGH); server.send(200); });
  server.on("/api/tft-backlight-off", []() { digitalWrite(TFT_BL, LOW); server.send(200); });
  
  // Tests avancés
  server.on("/api/adc-test", handleADCTest);
  server.on("/api/pwm-test", handlePWMTest);
  server.on("/api/spi-scan", handleSPIScan);
  server.on("/api/partitions-list", handlePartitionsList);
  server.on("/api/stress-test", handleStressTest);

  // Routes API pour les nouveaux capteurs
  server.on("/api/rgb-led-config", handleRGBLedConfig);
  server.on("/api/rgb-led-test", handleRGBLedTest);
  server.on("/api/rgb-led-color", handleRGBLedColor);

  server.on("/api/buzzer-config", handleBuzzerConfig);
  server.on("/api/buzzer-test", handleBuzzerTest);
  server.on("/api/buzzer-tone", handleBuzzerTone);

  server.on("/api/dht-config", handleDHTConfig);
  server.on("/api/dht-test", handleDHTTest);

  server.on("/api/light-sensor-config", handleLightSensorConfig);
  server.on("/api/light-sensor-test", handleLightSensorTest);

  server.on("/api/distance-sensor-config", handleDistanceSensorConfig);
  server.on("/api/distance-sensor-test", handleDistanceSensorTest);

  server.on("/api/motion-sensor-config", handleMotionSensorConfig);
  server.on("/api/motion-sensor-test", handleMotionSensorTest);

  // Performance & Mémoire
  server.on("/api/benchmark", handleBenchmark);
  server.on("/api/memory-details", handleMemoryDetails);
  
  // Exports
  server.on("/export/txt", handleExportTXT);
  server.on("/export/json", handleExportJSON);
  server.on("/export/csv", handleExportCSV);
  server.on("/print", handlePrintVersion);
  
  server.begin();
  Serial.println("Serveur Web OK!");
  Serial.println("\r\n===============================================");
  Serial.println("            PRET - En attente");
  Serial.println("   Langue par defaut: FRANCAIS");
  Serial.println("   Changement dynamique via interface web");
  Serial.println("===============================================\r\n");
}

// ========== LOOP ==========
void loop() {
  server.handleClient();
  maintainNetworkServices();
  // --- [PLATFORMIO FIX] MDNS.update() not needed in ESP32 Arduino Core 3.x+ ---
  // mDNS service runs in background task, no manual update required

  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 30000) {
    lastUpdate = millis();
    collectDiagnosticInfo();
    
    Serial.println("\r\n=== UPDATE ===");
    Serial.printf("Heap: %.2f KB | Uptime: %.2f h\r\n", 
                  diagnosticData.freeHeap / 1024.0, 
                  diagnosticData.uptime / 3600000.0);
    if (diagnosticData.temperature != -999) {
      Serial.printf("Temp: %.1f°C\r\n", diagnosticData.temperature);
    }
  }

  delay(10);
}
