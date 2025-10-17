/*
 * ============================================================
 * ESP32 Diagnostic System - WiFi Configuration Template
 * Version: 3.2.0
 * ============================================================
 * 
 * INSTRUCTIONS:
 * 1. Copier ce fichier vers "wifi-config.h"
 * 2. Modifier les identifiants WiFi ci-dessous
 * 3. Ajouter autant de réseaux que nécessaire
 * 4. NE PAS commiter wifi-config.h dans Git (ajouté au .gitignore)
 * 
 * SÉCURITÉ:
 * - Ce fichier contient des informations sensibles
 * - Il est automatiquement ignoré par Git
 * - Ne jamais le partager publiquement
 * 
 * ============================================================
 */

#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

// ============================================================
// CONFIGURATION WIFI MULTI-SSID
// ============================================================

/*
 * Le système tentera de se connecter à chaque réseau dans l'ordre
 * jusqu'à ce qu'une connexion réussisse.
 * 
 * Ajoutez autant de réseaux que nécessaire.
 */

struct WiFiCredentials {
  const char* ssid;
  const char* password;
};

// Liste des réseaux WiFi (ordre de priorité)
const WiFiCredentials wifiNetworks[] = {
  // Réseau principal
  {"VotreSsidPrincipal", "VotreMotDePassePrincipal"},
  
  // Réseau secondaire
  {"VotreSsidSecondaire", "VotreMotDePasseSecondaire"},
  
  // Réseau de secours
  {"VotreSsidSecours", "VotreMotDePasseSecours"},
  
  // Ajoutez d'autres réseaux ici si nécessaire
  // {"AutreSSID", "AutreMotDePasse"},
};

// Nombre de réseaux configurés
const int WIFI_NETWORKS_COUNT = sizeof(wifiNetworks) / sizeof(WiFiCredentials);

// ============================================================
// CONFIGURATION POINT D'ACCÈS (Mode Secours)
// ============================================================

/*
 * Si aucune connexion WiFi n'aboutit, l'ESP32 créera
 * son propre point d'accès WiFi.
 */

// SSID du point d'accès
#define AP_SSID "ESP32-Diagnostic-Setup"

// Mot de passe du point d'accès (minimum 8 caractères)
#define AP_PASSWORD "diagnostic123"

// Canal WiFi (1-13)
#define AP_CHANNEL 1

// Nombre maximum de clients connectés
#define AP_MAX_CONNECTIONS 4

// Masquer le SSID du point d'accès (false = visible)
#define AP_HIDDEN false

// ============================================================
// PARAMÈTRES DE CONNEXION
// ============================================================

// Timeout pour chaque tentative de connexion (secondes)
#define WIFI_CONNECT_TIMEOUT 10

// Nombre de tentatives avant de passer au réseau suivant
#define WIFI_MAX_RETRIES 2

// Délai entre les tentatives (millisecondes)
#define WIFI_RETRY_DELAY 500

// Activer le mode économie d'énergie WiFi
#define WIFI_POWER_SAVE false

// ============================================================
// CONFIGURATION MDNS
// ============================================================

// Nom mDNS (accès via http://nom.local)
#define MDNS_HOSTNAME "esp32-diagnostic"

// ============================================================
// EXEMPLES DE CONFIGURATION
// ============================================================

/*
 * EXEMPLE 1 - Maison + Bureau:
 * 
 * const WiFiCredentials wifiNetworks[] = {
 *   {"WiFi-Maison", "motdepasse123"},
 *   {"WiFi-Bureau", "autremdp456"},
 * };
 * 
 * EXEMPLE 2 - Avec hotspot mobile:
 * 
 * const WiFiCredentials wifiNetworks[] = {
 *   {"MonWiFi", "mdp123"},
 *   {"iPhone-de-Jean", "hotspot456"},
 *   {"AndroidAP", "mobile789"},
 * };
 * 
 * EXEMPLE 3 - Plusieurs étages/zones:
 * 
 * const WiFiCredentials wifiNetworks[] = {
 *   {"WiFi-RDC", "mdp-rdc"},
 *   {"WiFi-Etage1", "mdp-etage1"},
 *   {"WiFi-Etage2", "mdp-etage2"},
 *   {"WiFi-Garage", "mdp-garage"},
 * };
 */

// ============================================================
// NOTES IMPORTANTES
// ============================================================

/*
 * SÉCURITÉ:
 * - Utilisez des mots de passe forts (12+ caractères)
 * - Mélangez majuscules, minuscules, chiffres et symboles
 * - Ne réutilisez pas le même mot de passe partout
 * 
 * PERFORMANCE:
 * - Placez le réseau le plus utilisé en premier
 * - Les réseaux 2.4GHz ont une meilleure portée que le 5GHz
 * - L'ESP32 ne supporte QUE le 2.4GHz
 * 
 * DÉPANNAGE:
 * - Vérifiez que le SSID est exact (sensible à la casse)
 * - Assurez-vous d'être à portée du réseau
 * - Certains réseaux d'entreprise peuvent bloquer l'ESP32
 * - Le filtrage MAC peut empêcher la connexion
 * 
 * COMPATIBILITÉ:
 * - WPA/WPA2 Personal: ✅ Supporté
 * - WPA2/WPA3 Enterprise: ❌ Non supporté
 * - Réseau ouvert (pas de mot de passe): ✅ Supporté (déconseillé)
 * - WEP: ⚠️ Déprécié (très peu sécurisé)
 */

#endif // WIFI_CONFIG_H