/*
 * ============================================================
 * ESP32 Diagnostic System - Translations
 * Version: 3.2.0
 * ============================================================
 * 
 * Multilingual support for French and English.
 * Used by web interface for dynamic language switching.
 * 
 * Languages: FR (Français), EN (English)
 * ============================================================
 */

#ifndef TRANSLATIONS_H
#define TRANSLATIONS_H

// ============================================================
// STRUCTURE DE TRADUCTION
// ============================================================

struct Translation {
  const char* key;
  const char* fr;
  const char* en;
};

// ============================================================
// TRADUCTIONS - Interface Web
// ============================================================

const Translation translations[] = {
  // Navigation
  {"nav_dashboard", "Tableau de bord", "Dashboard"},
  {"nav_tests", "Tests", "Tests"},
  {"nav_memory", "Mémoire", "Memory"},
  {"nav_wifi", "WiFi", "WiFi"},
  {"nav_gpio", "GPIO", "GPIO"},
  {"nav_benchmarks", "Benchmarks", "Benchmarks"},
  {"nav_export", "Exportation", "Export"},
  {"nav_about", "À propos", "About"},
  
  // Dashboard
  {"dashboard_title", "Tableau de Bord", "Dashboard"},
  {"dashboard_system_info", "Informations Système", "System Information"},
  {"dashboard_chip_model", "Modèle de Puce", "Chip Model"},
  {"dashboard_cpu_cores", "Cœurs CPU", "CPU Cores"},
  {"dashboard_cpu_freq", "Fréquence CPU", "CPU Frequency"},
  {"dashboard_flash_size", "Taille Flash", "Flash Size"},
  {"dashboard_psram_size", "Taille PSRAM", "PSRAM Size"},
  {"dashboard_uptime", "Temps de fonctionnement", "Uptime"},
  {"dashboard_memory_status", "État de la Mémoire", "Memory Status"},
  {"dashboard_heap_free", "Heap Libre", "Free Heap"},
  {"dashboard_heap_size", "Taille Heap", "Heap Size"},
  {"dashboard_wifi_status", "État WiFi", "WiFi Status"},
  {"dashboard_connected", "Connecté", "Connected"},
  {"dashboard_disconnected", "Déconnecté", "Disconnected"},
  {"dashboard_ssid", "SSID", "SSID"},
  {"dashboard_ip", "Adresse IP", "IP Address"},
  {"dashboard_rssi", "Puissance Signal", "Signal Strength"},
  
  // Tests
  {"tests_title", "Tests Matériels", "Hardware Tests"},
  {"tests_run_all", "Exécuter Tous les Tests", "Run All Tests"},
  {"tests_gpio", "Test GPIO", "GPIO Test"},
  {"tests_pwm", "Test PWM", "PWM Test"},
  {"tests_i2c", "Test I2C", "I2C Test"},
  {"tests_wifi", "Test WiFi", "WiFi Test"},
  {"tests_memory", "Test Mémoire", "Memory Test"},
  {"tests_running", "Test en cours...", "Running test..."},
  {"tests_completed", "Test terminé", "Test completed"},
  {"tests_failed", "Test échoué", "Test failed"},
  {"tests_success", "Succès", "Success"},
  {"tests_pin", "Pin", "Pin"},
  {"tests_status", "État", "Status"},
  {"tests_result", "Résultat", "Result"},
  
  // Memory
  {"memory_title", "Mémoire Détaillée", "Memory Details"},
  {"memory_heap", "Mémoire Heap", "Heap Memory"},
  {"memory_psram", "Mémoire PSRAM", "PSRAM Memory"},
  {"memory_free", "Libre", "Free"},
  {"memory_used", "Utilisée", "Used"},
  {"memory_total", "Total", "Total"},
  {"memory_usage", "Utilisation", "Usage"},
  {"memory_available", "Disponible", "Available"},
  {"memory_not_available", "Non disponible", "Not available"},
  {"memory_refresh", "Actualiser", "Refresh"},
  
  // WiFi
  {"wifi_title", "Informations WiFi", "WiFi Information"},
  {"wifi_connection", "Connexion", "Connection"},
  {"wifi_network_name", "Nom du réseau", "Network name"},
  {"wifi_ip_address", "Adresse IP", "IP Address"},
  {"wifi_mac_address", "Adresse MAC", "MAC Address"},
  {"wifi_signal_strength", "Force du signal", "Signal Strength"},
  {"wifi_channel", "Canal", "Channel"},
  {"wifi_gateway", "Passerelle", "Gateway"},
  {"wifi_subnet", "Masque de sous-réseau", "Subnet Mask"},
  {"wifi_dns", "DNS", "DNS"},
  {"wifi_scan", "Scanner les réseaux", "Scan Networks"},
  {"wifi_scanning", "Scan en cours...", "Scanning..."},
  {"wifi_networks_found", "Réseaux trouvés", "Networks found"},
  {"wifi_encryption", "Chiffrement", "Encryption"},
  {"wifi_open", "Ouvert", "Open"},
  {"wifi_encrypted", "Chiffré", "Encrypted"},
  
  // GPIO
  {"gpio_title", "Configuration GPIO", "GPIO Configuration"},
  {"gpio_pin_number", "Numéro de Pin", "Pin Number"},
  {"gpio_mode", "Mode", "Mode"},
  {"gpio_state", "État", "State"},
  {"gpio_input", "Entrée", "Input"},
  {"gpio_output", "Sortie", "Output"},
  {"gpio_high", "HAUT", "HIGH"},
  {"gpio_low", "BAS", "LOW"},
  {"gpio_test_all", "Tester Toutes", "Test All"},
  {"gpio_available", "Disponible", "Available"},
  {"gpio_used", "Utilisée", "Used"},
  
  // Benchmarks
  {"bench_title", "Benchmarks Performance", "Performance Benchmarks"},
  {"bench_run", "Lancer les Benchmarks", "Run Benchmarks"},
  {"bench_cpu", "Benchmark CPU", "CPU Benchmark"},
  {"bench_memory", "Benchmark Mémoire", "Memory Benchmark"},
  {"bench_wifi", "Benchmark WiFi", "WiFi Benchmark"},
  {"bench_running", "Benchmark en cours...", "Running benchmark..."},
  {"bench_time", "Temps d'exécution", "Execution time"},
  {"bench_score", "Score", "Score"},
  {"bench_operations", "Opérations/sec", "Operations/sec"},
  
  // Export
  {"export_title", "Exporter les Données", "Export Data"},
  {"export_format", "Format d'exportation", "Export Format"},
  {"export_json", "Exporter en JSON", "Export as JSON"},
  {"export_csv", "Exporter en CSV", "Export as CSV"},
  {"export_success", "Exportation réussie", "Export successful"},
  {"export_failed", "Échec de l'exportation", "Export failed"},
  {"export_downloading", "Téléchargement...", "Downloading..."},
  {"export_include", "Inclure dans l'exportation", "Include in export"},
  {"export_system_info", "Informations système", "System information"},
  {"export_test_results", "Résultats des tests", "Test results"},
  {"export_memory_info", "Informations mémoire", "Memory information"},
  {"export_wifi_info", "Informations WiFi", "WiFi information"},
  
  // About
  {"about_title", "À Propos", "About"},
  {"about_version", "Version", "Version"},
  {"about_author", "Auteur", "Author"},
  {"about_license", "Licence", "License"},
  {"about_description", "Description", "Description"},
  {"about_desc_text", "Système de diagnostic complet pour ESP32 avec interface web multilingue", 
                      "Complete diagnostic system for ESP32 with multilingual web interface"},
  {"about_features", "Fonctionnalités", "Features"},
  {"about_support", "Support", "Support"},
  {"about_documentation", "Documentation", "Documentation"},
  {"about_github", "Dépôt GitHub", "GitHub Repository"},
  
  // NeoPixel
  {"neo_title", "Contrôle NeoPixel", "NeoPixel Control"},
  {"neo_color", "Couleur", "Color"},
  {"neo_pattern", "Pattern", "Pattern"},
  {"neo_on", "Allumer", "Turn On"},
  {"neo_off", "Éteindre", "Turn Off"},
  {"neo_blink", "Clignoter", "Blink"},
  {"neo_fade", "Fondu", "Fade"},
  {"neo_rainbow", "Arc-en-ciel", "Rainbow"},
  {"neo_test", "Test", "Test"},
  {"neo_not_available", "NeoPixel non disponible", "NeoPixel not available"},
  
  // Common
  {"common_loading", "Chargement...", "Loading..."},
  {"common_error", "Erreur", "Error"},
  {"common_success", "Succès", "Success"},
  {"common_warning", "Attention", "Warning"},
  {"common_info", "Information", "Information"},
  {"common_ok", "OK", "OK"},
  {"common_cancel", "Annuler", "Cancel"},
  {"common_yes", "Oui", "Yes"},
  {"common_no", "Non", "No"},
  {"common_refresh", "Actualiser", "Refresh"},
  {"common_close", "Fermer", "Close"},
  {"common_save", "Enregistrer", "Save"},
  {"common_delete", "Supprimer", "Delete"},
  {"common_download", "Télécharger", "Download"},
  {"common_upload", "Téléverser", "Upload"},
  {"common_search", "Rechercher", "Search"},
  {"common_filter", "Filtrer", "Filter"},
  {"common_sort", "Trier", "Sort"},
  {"common_settings", "Paramètres", "Settings"},
  {"common_help", "Aide", "Help"},
  
  // Messages
  {"msg_connection_lost", "Connexion perdue", "Connection lost"},
  {"msg_reconnecting", "Reconnexion...", "Reconnecting..."},
  {"msg_connected", "Connecté", "Connected"},
  {"msg_no_data", "Aucune donnée disponible", "No data available"},
  {"msg_test_in_progress", "Test en cours, veuillez patienter", "Test in progress, please wait"},
  {"msg_operation_failed", "L'opération a échoué", "Operation failed"},
  {"msg_operation_success", "Opération réussie", "Operation successful"},
  
  // Units
  {"unit_bytes", "octets", "bytes"},
  {"unit_kb", "Ko", "KB"},
  {"unit_mb", "Mo", "MB"},
  {"unit_gb", "Go", "GB"},
  {"unit_mhz", "MHz", "MHz"},
  {"unit_ms", "ms", "ms"},
  {"unit_seconds", "secondes", "seconds"},
  {"unit_percent", "%", "%"},
  {"unit_dbm", "dBm", "dBm"}
};

// ============================================================
// FONCTION D'OBTENTION DE TRADUCTION
// ============================================================

String getTranslation(String key, String lang = "fr") {
  int numTranslations = sizeof(translations) / sizeof(Translation);
  
  for (int i = 0; i < numTranslations; i++) {
    if (String(translations[i].key) == key) {
      if (lang == "en") {
        return String(translations[i].en);
      } else {
        return String(translations[i].fr);
      }
    }
  }
  
  // Si la clé n'est pas trouvée, retourner la clé elle-même
  return key;
}

// ============================================================
// FONCTION DE GÉNÉRATION JSON DES TRADUCTIONS
// ============================================================

String getTranslationsJSON(String lang = "fr") {
  String json = "{";
  int numTranslations = sizeof(translations) / sizeof(Translation);
  
  for (int i = 0; i < numTranslations; i++) {
    json += "\"" + String(translations[i].key) + "\":\"";
    
    if (lang == "en") {
      json += String(translations[i].en);
    } else {
      json += String(translations[i].fr);
    }
    
    json += "\"";
    
    if (i < numTranslations - 1) {
      json += ",";
    }
  }
  
  json += "}";
  return json;
}

#endif // TRANSLATIONS_H