/*
 * ============================================================
 * TRANSLATIONS - ESP32 Diagnostic System
 * ============================================================
 *
 * Version: 3.0.1
 * Date: October 2025
 * Arduino Core: 3.3.2+
 *
 * Description:
 *   Bilingual support (French/English) for the web interface.
 *   All UI text strings are defined here for easy maintenance.
 *
 * Supported Languages:
 *   - French (fr) - Default
 *   - English (en)
 *
 * ============================================================
 */

#ifndef TRANSLATIONS_H
#define TRANSLATIONS_H

// ============================================================
// FRENCH TRANSLATIONS
// ============================================================

const char* TEXT_FR_TITLE = "Diagnostic ESP32";
const char* TEXT_FR_VERSION = "Version";
const char* TEXT_FR_CORE_VERSION = "Arduino Core";

// Tabs
const char* TEXT_FR_TAB_OVERVIEW = "Vue générale";
const char* TEXT_FR_TAB_TESTS = "Tests";
const char* TEXT_FR_TAB_EXPORTS = "Exports";

// System Info
const char* TEXT_FR_SYSTEM_INFO = "Informations Système";
const char* TEXT_FR_CHIP = "Puce";
const char* TEXT_FR_CORES = "Cœurs CPU";
const char* TEXT_FR_FREQUENCY = "Fréquence";
const char* TEXT_FR_FLASH = "Mémoire Flash";
const char* TEXT_FR_PSRAM = "PSRAM";
const char* TEXT_FR_UPTIME = "Durée de fonctionnement";

// Memory
const char* TEXT_FR_MEMORY = "Mémoire";
const char* TEXT_FR_HEAP_FREE = "Heap Libre";
const char* TEXT_FR_HEAP_SIZE = "Taille Heap";
const char* TEXT_FR_PSRAM_FREE = "PSRAM Libre";
const char* TEXT_FR_PSRAM_SIZE = "Taille PSRAM";

// WiFi
const char* TEXT_FR_WIFI = "WiFi";
const char* TEXT_FR_WIFI_STATUS = "Statut";
const char* TEXT_FR_WIFI_SSID = "SSID";
const char* TEXT_FR_WIFI_IP = "Adresse IP";
const char* TEXT_FR_WIFI_RSSI = "Signal (RSSI)";
const char* TEXT_FR_WIFI_CONNECTED = "Connecté";
const char* TEXT_FR_WIFI_DISCONNECTED = "Déconnecté";

// Tests
const char* TEXT_FR_GPIO_TEST = "Test GPIO";
const char* TEXT_FR_I2C_TEST = "Test I2C";
const char* TEXT_FR_SPI_TEST = "Test SPI";
const char* TEXT_FR_MEMORY_TEST = "Test Mémoire";
const char* TEXT_FR_WIFI_TEST = "Test WiFi";
const char* TEXT_FR_SYSTEM_TEST = "Test Système";

const char* TEXT_FR_PIN = "Pin";
const char* TEXT_FR_STATUS = "Statut";
const char* TEXT_FR_DETAILS = "Détails";
const char* TEXT_FR_ADDRESS = "Adresse";
const char* TEXT_FR_DEVICE = "Périphérique";

// Exports
const char* TEXT_FR_EXPORT_JSON = "Exporter JSON";
const char* TEXT_FR_EXPORT_CSV = "Exporter CSV";
const char* TEXT_FR_EXPORT_DESC = "Télécharger les données de diagnostic dans différents formats";

// NeoPixel
const char* TEXT_FR_NEOPIXEL = "Contrôle NeoPixel";
const char* TEXT_FR_NEOPIXEL_PATTERN = "Motif";
const char* TEXT_FR_NEOPIXEL_COLOR = "Couleur";

// Status
const char* TEXT_FR_LOADING = "Chargement...";
const char* TEXT_FR_ERROR = "Erreur";
const char* TEXT_FR_SUCCESS = "Succès";
const char* TEXT_FR_UPDATING = "Mise à jour dans";
const char* TEXT_FR_SECONDS = "secondes";

// ============================================================
// ENGLISH TRANSLATIONS
// ============================================================

const char* TEXT_EN_TITLE = "ESP32 Diagnostic";
const char* TEXT_EN_VERSION = "Version";
const char* TEXT_EN_CORE_VERSION = "Arduino Core";

// Tabs
const char* TEXT_EN_TAB_OVERVIEW = "Overview";
const char* TEXT_EN_TAB_TESTS = "Tests";
const char* TEXT_EN_TAB_EXPORTS = "Exports";

// System Info
const char* TEXT_EN_SYSTEM_INFO = "System Information";
const char* TEXT_EN_CHIP = "Chip";
const char* TEXT_EN_CORES = "CPU Cores";
const char* TEXT_EN_FREQUENCY = "Frequency";
const char* TEXT_EN_FLASH = "Flash Memory";
const char* TEXT_EN_PSRAM = "PSRAM";
const char* TEXT_EN_UPTIME = "Uptime";

// Memory
const char* TEXT_EN_MEMORY = "Memory";
const char* TEXT_EN_HEAP_FREE = "Heap Free";
const char* TEXT_EN_HEAP_SIZE = "Heap Size";
const char* TEXT_EN_PSRAM_FREE = "PSRAM Free";
const char* TEXT_EN_PSRAM_SIZE = "PSRAM Size";

// WiFi
const char* TEXT_EN_WIFI = "WiFi";
const char* TEXT_EN_WIFI_STATUS = "Status";
const char* TEXT_EN_WIFI_SSID = "SSID";
const char* TEXT_EN_WIFI_IP = "IP Address";
const char* TEXT_EN_WIFI_RSSI = "Signal (RSSI)";
const char* TEXT_EN_WIFI_CONNECTED = "Connected";
const char* TEXT_EN_WIFI_DISCONNECTED = "Disconnected";

// Tests
const char* TEXT_EN_GPIO_TEST = "GPIO Test";
const char* TEXT_EN_I2C_TEST = "I2C Test";
const char* TEXT_EN_SPI_TEST = "SPI Test";
const char* TEXT_EN_MEMORY_TEST = "Memory Test";
const char* TEXT_EN_WIFI_TEST = "WiFi Test";
const char* TEXT_EN_SYSTEM_TEST = "System Test";

const char* TEXT_EN_PIN = "Pin";
const char* TEXT_EN_STATUS = "Status";
const char* TEXT_EN_DETAILS = "Details";
const char* TEXT_EN_ADDRESS = "Address";
const char* TEXT_EN_DEVICE = "Device";

// Exports
const char* TEXT_EN_EXPORT_JSON = "Export JSON";
const char* TEXT_EN_EXPORT_CSV = "Export CSV";
const char* TEXT_EN_EXPORT_DESC = "Download diagnostic data in various formats";

// NeoPixel
const char* TEXT_EN_NEOPIXEL = "NeoPixel Control";
const char* TEXT_EN_NEOPIXEL_PATTERN = "Pattern";
const char* TEXT_EN_NEOPIXEL_COLOR = "Color";

// Status
const char* TEXT_EN_LOADING = "Loading...";
const char* TEXT_EN_ERROR = "Error";
const char* TEXT_EN_SUCCESS = "Success";
const char* TEXT_EN_UPDATING = "Updating in";
const char* TEXT_EN_SECONDS = "seconds";

// ============================================================
// TRANSLATION HELPER FUNCTIONS
// ============================================================

String t(const char* textFr, const char* textEn) {
  if (currentLanguage == "en") {
    return String(textEn);
  }
  return String(textFr);
}

// Convenience macros for common translations
#define T_TITLE t(TEXT_FR_TITLE, TEXT_EN_TITLE)
#define T_VERSION t(TEXT_FR_VERSION, TEXT_EN_VERSION)
#define T_CORE_VERSION t(TEXT_FR_CORE_VERSION, TEXT_EN_CORE_VERSION)

#define T_TAB_OVERVIEW t(TEXT_FR_TAB_OVERVIEW, TEXT_EN_TAB_OVERVIEW)
#define T_TAB_TESTS t(TEXT_FR_TAB_TESTS, TEXT_EN_TAB_TESTS)
#define T_TAB_EXPORTS t(TEXT_FR_TAB_EXPORTS, TEXT_EN_TAB_EXPORTS)

#define T_SYSTEM_INFO t(TEXT_FR_SYSTEM_INFO, TEXT_EN_SYSTEM_INFO)
#define T_CHIP t(TEXT_FR_CHIP, TEXT_EN_CHIP)
#define T_CORES t(TEXT_FR_CORES, TEXT_EN_CORES)
#define T_FREQUENCY t(TEXT_FR_FREQUENCY, TEXT_EN_FREQUENCY)
#define T_FLASH t(TEXT_FR_FLASH, TEXT_EN_FLASH)
#define T_PSRAM t(TEXT_FR_PSRAM, TEXT_EN_PSRAM)
#define T_UPTIME t(TEXT_FR_UPTIME, TEXT_EN_UPTIME)

#define T_MEMORY t(TEXT_FR_MEMORY, TEXT_EN_MEMORY)
#define T_HEAP_FREE t(TEXT_FR_HEAP_FREE, TEXT_EN_HEAP_FREE)
#define T_HEAP_SIZE t(TEXT_FR_HEAP_SIZE, TEXT_EN_HEAP_SIZE)
#define T_PSRAM_FREE t(TEXT_FR_PSRAM_FREE, TEXT_EN_PSRAM_FREE)
#define T_PSRAM_SIZE t(TEXT_FR_PSRAM_SIZE, TEXT_EN_PSRAM_SIZE)

#define T_WIFI t(TEXT_FR_WIFI, TEXT_EN_WIFI)
#define T_WIFI_STATUS t(TEXT_FR_WIFI_STATUS, TEXT_EN_WIFI_STATUS)
#define T_WIFI_SSID t(TEXT_FR_WIFI_SSID, TEXT_EN_WIFI_SSID)
#define T_WIFI_IP t(TEXT_FR_WIFI_IP, TEXT_EN_WIFI_IP)
#define T_WIFI_RSSI t(TEXT_FR_WIFI_RSSI, TEXT_EN_WIFI_RSSI)

#define T_GPIO_TEST t(TEXT_FR_GPIO_TEST, TEXT_EN_GPIO_TEST)
#define T_I2C_TEST t(TEXT_FR_I2C_TEST, TEXT_EN_I2C_TEST)
#define T_SPI_TEST t(TEXT_FR_SPI_TEST, TEXT_EN_SPI_TEST)
#define T_MEMORY_TEST t(TEXT_FR_MEMORY_TEST, TEXT_EN_MEMORY_TEST)
#define T_WIFI_TEST t(TEXT_FR_WIFI_TEST, TEXT_EN_WIFI_TEST)
#define T_SYSTEM_TEST t(TEXT_FR_SYSTEM_TEST, TEXT_EN_SYSTEM_TEST)

#define T_PIN t(TEXT_FR_PIN, TEXT_EN_PIN)
#define T_STATUS t(TEXT_FR_STATUS, TEXT_EN_STATUS)
#define T_DETAILS t(TEXT_FR_DETAILS, TEXT_EN_DETAILS)
#define T_ADDRESS t(TEXT_FR_ADDRESS, TEXT_EN_ADDRESS)
#define T_DEVICE t(TEXT_FR_DEVICE, TEXT_EN_DEVICE)

#define T_EXPORT_JSON t(TEXT_FR_EXPORT_JSON, TEXT_EN_EXPORT_JSON)
#define T_EXPORT_CSV t(TEXT_FR_EXPORT_CSV, TEXT_EN_EXPORT_CSV)

#define T_LOADING t(TEXT_FR_LOADING, TEXT_EN_LOADING)
#define T_ERROR t(TEXT_FR_ERROR, TEXT_EN_ERROR)
#define T_SUCCESS t(TEXT_FR_SUCCESS, TEXT_EN_SUCCESS)

#endif // TRANSLATIONS_H
