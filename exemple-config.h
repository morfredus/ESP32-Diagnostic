/*
 * ============================================================
 * CONFIGURATION FILE EXAMPLE - ESP32 Diagnostic System
 * ============================================================
 *
 * Version: 3.0.1
 * Date: October 2025
 *
 * Description:
 *   WiFi configuration example file with Multi-WiFi support.
 *
 * IMPORTANT - INSTALLATION STEPS:
 *   1. Copy this file and rename it to "config.h"
 *   2. Edit "config.h" with your WiFi credentials
 *   3. DO NOT commit "config.h" to version control (already in .gitignore)
 *   4. Keep this "exemple-config.h" as template
 *
 * Usage:
 *   1. cp exemple-config.h config.h
 *   2. Edit config.h with your WiFi network details
 *   3. Configure single or multiple networks
 *   4. Save and upload to your ESP32
 *
 * Security:
 *   - config.h is in .gitignore (won't be committed)
 *   - exemple-config.h can be safely committed (no credentials)
 *   - Never share your config.h file
 *
 * ============================================================
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
// WiFi CONFIGURATION MODE
// ============================================================

// CHOOSE YOUR MODE:
// Uncomment ONE of the following:

// Option A: Multi-WiFi (RECOMMENDED) - ESP32 tries multiple networks
#define MULTI_WIFI_ENABLED

// Option B: Single WiFi - Only one network
// #define SINGLE_WIFI_ENABLED

// ============================================================
// MULTI-WiFi CONFIGURATION (Recommended)
// ============================================================
#ifdef MULTI_WIFI_ENABLED

// Number of WiFi networks to try
#define NUM_SSIDS 2

// List of SSIDs (network names)
const char* ssid_list[] = {
  "VotreSSID1",       // First network to try
  "VotreSSID2"        // Second network if first fails
};

// List of passwords (same order as SSIDs)
const char* password_list[] = {
  "MotDePasse1",      // Password for VotreSSID1
  "MotDePasse2"       // Password for VotreSSID2
};

// You can add more networks by:
// 1. Increasing NUM_SSIDS
// 2. Adding entries to both arrays
// Example for 3 networks:
/*
#define NUM_SSIDS 3
const char* ssid_list[] = {"Home", "Office", "Mobile"};
const char* password_list[] = {"pass1", "pass2", "pass3"};
*/

#endif // MULTI_WIFI_ENABLED

// ============================================================
// SINGLE WiFi CONFIGURATION (Simple mode)
// ============================================================
#ifdef SINGLE_WIFI_ENABLED

// Single WiFi credentials
const char* ssid = "VotreSSID";              // ← Your WiFi SSID
const char* password = "VotreMotDePasse";    // ← Your WiFi password

#endif // SINGLE_WIFI_ENABLED

// ============================================================
// ADVANCED WiFi SETTINGS (Optional)
// ============================================================

// Uncomment to use static IP instead of DHCP
// #define USE_STATIC_IP

#ifdef USE_STATIC_IP
  IPAddress staticIP(192, 168, 1, 184);     // Your desired IP
  IPAddress gateway(192, 168, 1, 1);        // Your router IP
  IPAddress subnet(255, 255, 255, 0);       // Subnet mask
  IPAddress dns1(8, 8, 8, 8);               // Primary DNS (Google)
  IPAddress dns2(8, 8, 4, 4);               // Secondary DNS (Google)
#endif

// WiFi connection timeout (seconds)
#define WIFI_TIMEOUT 20

// WiFi power settings
// Options: WIFI_POWER_19_5dBm, WIFI_POWER_19dBm, WIFI_POWER_18_5dBm,
//          WIFI_POWER_17dBm, WIFI_POWER_15dBm, WIFI_POWER_13dBm,
//          WIFI_POWER_11dBm, WIFI_POWER_8_5dBm, WIFI_POWER_7dBm,
//          WIFI_POWER_5dBm, WIFI_POWER_2dBm, WIFI_POWER_MINUS_1dBm
#define WIFI_TX_POWER WIFI_POWER_19_5dBm  // Maximum power

// ============================================================
// AP MODE CONFIGURATION (Fallback when WiFi fails)
// ============================================================

const char* ap_ssid = "ESP32-Diagnostic";
const char* ap_password = "12345678";  // Minimum 8 characters

// AP IP configuration
IPAddress ap_local_IP(192, 168, 4, 1);
IPAddress ap_gateway(192, 168, 4, 1);
IPAddress ap_subnet(255, 255, 255, 0);

// ============================================================
// INSTALLATION REMINDER
// ============================================================

/*
 * ⚠️ REMINDER: This is the EXAMPLE file
 *
 * To use this project:
 * 1. Copy this file: cp exemple-config.h config.h
 * 2. Edit config.h with your credentials
 * 3. config.h will NOT be tracked by Git (.gitignore)
 * 4. You can safely commit exemple-config.h
 *
 * Don't modify this exemple-config.h with real credentials!
 */

#endif // CONFIG_H
