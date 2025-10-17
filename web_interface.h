/*
 * ============================================================
 * ESP32 Diagnostic System - Web Interface
 * Version: 3.2.0
 * ============================================================
 * 
 * Modern, responsive web interface with:
 * - 8 thematic pages
 * - Multilingual support (FR/EN)
 * - Dark theme with gradients
 * - Real-time data updates
 * - Interactive controls
 * 
 * ============================================================
 */

#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

// ============================================================
// PAGE PRINCIPALE - HTML/CSS/JS
// ============================================================

const char MAIN_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Diagnostic v3.2.0</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        :root {
            --primary: #2563eb;
            --primary-dark: #1e40af;
            --secondary: #7c3aed;
            --bg-dark: #0f172a;
            --bg-card: #1e293b;
            --bg-card-hover: #334155;
            --text-primary: #f1f5f9;
            --text-secondary: #94a3b8;
            --border: #334155;
            --success: #10b981;
            --warning: #f59e0b;
            --error: #ef4444;
            --shadow: 0 10px 30px rgba(0, 0, 0, 0.3);
        }
        
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
            background: linear-gradient(135deg, var(--bg-dark) 0%, #1a2332 100%);
            color: var(--text-primary);
            min-height: 100vh;
            padding: 20px;
        }
        
        .container {
            max-width: 1400px;
            margin: 0 auto;
        }
        
        /* Header */
        .header {
            background: linear-gradient(135deg, var(--primary) 0%, var(--secondary) 100%);
            padding: 25px 30px;
            border-radius: 15px;
            margin-bottom: 30px;
            box-shadow: var(--shadow);
            display: flex;
            justify-content: space-between;
            align-items: center;
            flex-wrap: wrap;
        }
        
        .header h1 {
            font-size: 28px;
            font-weight: 700;
            display: flex;
            align-items: center;
            gap: 12px;
        }
        
        .version {
            background: rgba(255, 255, 255, 0.2);
            padding: 4px 12px;
            border-radius: 20px;
            font-size: 14px;
            font-weight: 600;
        }
        
        .lang-selector {
            display: flex;
            gap: 10px;
            background: rgba(255, 255, 255, 0.1);
            padding: 5px;
            border-radius: 10px;
        }
        
        .lang-btn {
            padding: 8px 16px;
            border: none;
            background: transparent;
            color: white;
            cursor: pointer;
            border-radius: 8px;
            font-weight: 600;
            transition: all 0.3s;
        }
        
        .lang-btn.active {
            background: rgba(255, 255, 255, 0.3);
        }
        
        .lang-btn:hover {
            background: rgba(255, 255, 255, 0.2);
        }
        
        /* Navigation */
        .nav-tabs {
            display: flex;
            gap: 10px;
            margin-bottom: 30px;
            overflow-x: auto;
            padding-bottom: 10px;
        }
        
        .nav-tabs::-webkit-scrollbar {
            height: 6px;
        }
        
        .nav-tabs::-webkit-scrollbar-track {
            background: var(--bg-card);
            border-radius: 3px;
        }
        
        .nav-tabs::-webkit-scrollbar-thumb {
            background: var(--primary);
            border-radius: 3px;
        }
        
        .tab-btn {
            padding: 12px 24px;
            border: none;
            background: var(--bg-card);
            color: var(--text-secondary);
            cursor: pointer;
            border-radius: 10px;
            font-weight: 600;
            transition: all 0.3s;
            white-space: nowrap;
            border: 2px solid transparent;
        }
        
        .tab-btn:hover {
            background: var(--bg-card-hover);
            color: var(--text-primary);
        }
        
        .tab-btn.active {
            background: linear-gradient(135deg, var(--primary) 0%, var(--secondary) 100%);
            color: white;
            border-color: var(--primary);
        }
        
        /* Content */
        .tab-content {
            display: none;
        }
        
        .tab-content.active {
            display: block;
            animation: fadeIn 0.3s;
        }
        
        @keyframes fadeIn {
            from {
                opacity: 0;
                transform: translateY(10px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }
        
        /* Cards */
        .card {
            background: var(--bg-card);
            padding: 25px;
            border-radius: 15px;
            margin-bottom: 20px;
            box-shadow: var(--shadow);
            border: 1px solid var(--border);
            transition: all 0.3s;
        }
        
        .card:hover {
            border-color: var(--primary);
            transform: translateY(-2px);
        }
        
        .card-title {
            font-size: 20px;
            font-weight: 700;
            margin-bottom: 20px;
            color: var(--text-primary);
            display: flex;
            align-items: center;
            gap: 10px;
        }
        
        .card-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
            gap: 20px;
        }
        
        /* Info Items */
        .info-item {
            display: flex;
            justify-content: space-between;
            padding: 12px 0;
            border-bottom: 1px solid var(--border);
        }
        
        .info-item:last-child {
            border-bottom: none;
        }
        
        .info-label {
            color: var(--text-secondary);
            font-weight: 500;
        }
        
        .info-value {
            color: var(--text-primary);
            font-weight: 600;
        }
        
        /* Buttons */
        .btn {
            padding: 12px 24px;
            border: none;
            border-radius: 10px;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s;
            display: inline-flex;
            align-items: center;
            gap: 8px;
        }
        
        .btn-primary {
            background: linear-gradient(135deg, var(--primary) 0%, var(--secondary) 100%);
            color: white;
        }
        
        .btn-primary:hover {
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(37, 99, 235, 0.4);
        }
        
        .btn-success {
            background: var(--success);
            color: white;
        }
        
        .btn-warning {
            background: var(--warning);
            color: white;
        }
        
        .btn-error {
            background: var(--error);
            color: white;
        }
        
        /* Status Badge */
        .status-badge {
            padding: 6px 12px;
            border-radius: 20px;
            font-size: 12px;
            font-weight: 700;
            text-transform: uppercase;
        }
        
        .status-success {
            background: var(--success);
            color: white;
        }
        
        .status-error {
            background: var(--error);
            color: white;
        }
        
        .status-warning {
            background: var(--warning);
            color: white;
        }
        
        /* Progress Bar */
        .progress-bar {
            width: 100%;
            height: 8px;
            background: var(--border);
            border-radius: 10px;
            overflow: hidden;
            margin-top: 10px;
        }
        
        .progress-fill {
            height: 100%;
            background: linear-gradient(90deg, var(--primary) 0%, var(--secondary) 100%);
            transition: width 0.3s;
            border-radius: 10px;
        }
        
        /* Table */
        .table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
        }
        
        .table th {
            background: var(--bg-dark);
            padding: 12px;
            text-align: left;
            font-weight: 600;
            border-bottom: 2px solid var(--primary);
        }
        
        .table td {
            padding: 12px;
            border-bottom: 1px solid var(--border);
        }
        
        .table tr:hover {
            background: var(--bg-card-hover);
        }
        
        /* Loading */
        .loading {
            display: inline-block;
            width: 20px;
            height: 20px;
            border: 3px solid var(--border);
            border-top-color: var(--primary);
            border-radius: 50%;
            animation: spin 1s linear infinite;
        }
        
        @keyframes spin {
            to { transform: rotate(360deg); }
        }
        
        /* Responsive */
        @media (max-width: 768px) {
            .header {
                flex-direction: column;
                gap: 15px;
            }
            
            .card-grid {
                grid-template-columns: 1fr;
            }
            
            .nav-tabs {
                justify-content: flex-start;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <!-- Header -->
        <div class="header">
            <h1>
                ⚡ ESP32 Diagnostic
                <span class="version">v3.2.0</span>
            </h1>
            <div class="lang-selector">
                <button class="lang-btn active" data-lang="fr">🇫🇷 FR</button>
                <button class="lang-btn" data-lang="en">🇬🇧 EN</button>
            </div>
        </div>
        
        <!-- Navigation Tabs -->
        <div class="nav-tabs">
            <button class="tab-btn active" data-tab="dashboard">📊 <span data-i18n="nav_dashboard">Tableau de bord</span></button>
            <button class="tab-btn" data-tab="tests">🧪 <span data-i18n="nav_tests">Tests</span></button>
            <button class="tab-btn" data-tab="memory">💾 <span data-i18n="nav_memory">Mémoire</span></button>
            <button class="tab-btn" data-tab="wifi">📡 <span data-i18n="nav_wifi">WiFi</span></button>
            <button class="tab-btn" data-tab="gpio">🔌 <span data-i18n="nav_gpio">GPIO</span></button>
            <button class="tab-btn" data-tab="benchmarks">⚡ <span data-i18n="nav_benchmarks">Benchmarks</span></button>
            <button class="tab-btn" data-tab="export">💾 <span data-i18n="nav_export">Export</span></button>
            <button class="tab-btn" data-tab="about">ℹ️ <span data-i18n="nav_about">À propos</span></button>
        </div>
        
        <!-- Tab Content: Dashboard -->
        <div id="dashboard" class="tab-content active">
            <div class="card-grid">
                <div class="card">
                    <div class="card-title">🖥️ <span data-i18n="dashboard_system_info">Informations Système</span></div>
                    <div id="systemInfo">
                        <div class="loading"></div>
                    </div>
                </div>
                
                <div class="card">
                    <div class="card-title">💾 <span data-i18n="dashboard_memory_status">État Mémoire</span></div>
                    <div id="memoryInfo">
                        <div class="loading"></div>
                    </div>
                </div>
                
                <div class="card">
                    <div class="card-title">📡 <span data-i18n="dashboard_wifi_status">État WiFi</span></div>
                    <div id="wifiInfo">
                        <div class="loading"></div>
                    </div>
                </div>
            </div>
        </div>
        
        <!-- Tab Content: Tests -->
        <div id="tests" class="tab-content">
            <div class="card">
                <div class="card-title">🧪 <span data-i18n="tests_title">Tests Matériels</span></div>
                <button class="btn btn-primary" onclick="runAllTests()">
                    ▶️ <span data-i18n="tests_run_all">Exécuter Tous les Tests</span>
                </button>
                <div id="testResults" style="margin-top: 20px;">
                </div>
            </div>
        </div>
        
        <!-- Tab Content: Memory -->
        <div id="memory" class="tab-content">
            <div class="card">
                <div class="card-title">💾 <span data-i18n="memory_title">Mémoire Détaillée</span></div>
                <div id="memoryDetails">
                    <div class="loading"></div>
                </div>
            </div>
        </div>
        
        <!-- Tab Content: WiFi -->
        <div id="wifi" class="tab-content">
            <div class="card">
                <div class="card-title">📡 <span data-i18n="wifi_title">Informations WiFi</span></div>
                <div id="wifiDetails">
                    <div class="loading"></div>
                </div>
                <button class="btn btn-primary" onclick="scanWiFi()" style="margin-top: 20px;">
                    🔍 <span data-i18n="wifi_scan">Scanner les réseaux</span>
                </button>
                <div id="wifiScanResults" style="margin-top: 20px;"></div>
            </div>
        </div>
        
        <!-- Tab Content: GPIO -->
        <div id="gpio" class="tab-content">
            <div class="card">
                <div class="card-title">🔌 <span data-i18n="gpio_title">Configuration GPIO</span></div>
                <button class="btn btn-primary" onclick="testGPIO()">
                    ▶️ <span data-i18n="gpio_test_all">Tester Toutes les GPIO</span>
                </button>
                <div id="gpioResults" style="margin-top: 20px;"></div>
            </div>
        </div>
        
        <!-- Tab Content: Benchmarks -->
        <div id="benchmarks" class="tab-content">
            <div class="card">
                <div class="card-title">⚡ <span data-i18n="bench_title">Benchmarks Performance</span></div>
                <button class="btn btn-primary" onclick="runBenchmarks()">
                    ▶️ <span data-i18n="bench_run">Lancer les Benchmarks</span>
                </button>
                <div id="benchResults" style="margin-top: 20px;"></div>
            </div>
        </div>
        
        <!-- Tab Content: Export -->
        <div id="export" class="tab-content">
            <div class="card">
                <div class="card-title">💾 <span data-i18n="export_title">Exporter les Données</span></div>
                <div style="display: flex; gap: 15px; flex-wrap: wrap;">
                    <button class="btn btn-success" onclick="exportJSON()">
                        📄 <span data-i18n="export_json">Exporter en JSON</span>
                    </button>
                    <button class="btn btn-warning" onclick="exportCSV()">
                        📊 <span data-i18n="export_csv">Exporter en CSV</span>
                    </button>
                </div>
            </div>
        </div>
        
        <!-- Tab Content: About -->
        <div id="about" class="tab-content">
            <div class="card">
                <div class="card-title">ℹ️ <span data-i18n="about_title">À Propos</span></div>
                <div class="info-item">
                    <span class="info-label" data-i18n="about_version">Version</span>
                    <span class="info-value">3.2.0</span>
                </div>
                <div class="info-item">
                    <span class="info-label" data-i18n="about_author">Auteur</span>
                    <span class="info-value">ESP32 Diagnostic Team</span>
                </div>
                <div class="info-item">
                    <span class="info-label" data-i18n="about_license">Licence</span>
                    <span class="info-value">MIT</span>
                </div>
                <div class="info-item">
                    <span class="info-label" data-i18n="about_description">Description</span>
                    <span class="info-value" data-i18n="about_desc_text">Système de diagnostic complet pour ESP32</span>
                </div>
            </div>
        </div>
    </div>
    
    <script>
        let currentLang = 'fr';
        let translations = {};
        
        // ============================================
        // GESTION DES ONGLETS
        // ============================================
        
        document.querySelectorAll('.tab-btn').forEach(btn => {
            btn.addEventListener('click', () => {
                const tab = btn.getAttribute('data-tab');
                
                // Désactiver tous les onglets et contenus
                document.querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));
                document.querySelectorAll('.tab-content').forEach(c => c.classList.remove('active'));
                
                // Activer l'onglet et le contenu sélectionné
                btn.classList.add('active');
                document.getElementById(tab).classList.add('active');
            });
        });
        
        // ============================================
        // GESTION DE LA LANGUE
        // ============================================
        
        document.querySelectorAll('.lang-btn').forEach(btn => {
            btn.addEventListener('click', async () => {
                const lang = btn.getAttribute('data-lang');
                
                // Mettre à jour la langue sur le serveur
                await fetch('/api/language', {
                    method: 'POST',
                    headers: {'Content-Type': 'application/json'},
                    body: JSON.stringify({language: lang})
                });
                
                // Mettre à jour l'interface
                currentLang = lang;
                document.querySelectorAll('.lang-btn').forEach(b => b.classList.remove('active'));
                btn.classList.add('active');
                
                // Recharger les traductions
                await loadTranslations();
                updateAllData();
            });
        });
        
        // ============================================
        // CHARGEMENT DES TRADUCTIONS
        // ============================================
        
        async function loadTranslations() {
            try {
                const response = await fetch('/api/language');
                const data = await response.json();
                currentLang = data.language || 'fr';
                
                // Mettre à jour tous les éléments avec data-i18n
                document.querySelectorAll('[data-i18n]').forEach(el => {
                    const key = el.getAttribute('data-i18n');
                    if (translations[key]) {
                        el.textContent = translations[key];
                    }
                });
            } catch (error) {
                console.error('Error loading translations:', error);
            }
        }
        
        // ============================================
        // DASHBOARD - INFORMATIONS SYSTÈME
        // ============================================
        
        async function loadSystemInfo() {
            try {
                const response = await fetch('/api/system/info');
                const data = await response.json();
                
                const html = `
                    <div class="info-item">
                        <span class="info-label" data-i18n="dashboard_chip_model">Modèle de Puce</span>
                        <span class="info-value">${data.chip}</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label" data-i18n="dashboard_cpu_cores">Cœurs CPU</span>
                        <span class="info-value">${data.cores}</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label" data-i18n="dashboard_cpu_freq">Fréquence CPU</span>
                        <span class="info-value">${data.frequency} MHz</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label" data-i18n="dashboard_flash_size">Taille Flash</span>
                        <span class="info-value">${data.memory.heap_size_formatted}</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label" data-i18n="dashboard_uptime">Temps de fonctionnement</span>
                        <span class="info-value">${data.uptime}</span>
                    </div>
                `;
                
                document.getElementById('systemInfo').innerHTML = html;
            } catch (error) {
                document.getElementById('systemInfo').innerHTML = '<p style="color: var(--error);">Error loading data</p>';
            }
        }
        
        // ============================================
        // DASHBOARD - MÉMOIRE
        // ============================================
        
        async function loadMemoryInfo() {
            try {
                const response = await fetch('/api/system/memory');
                const data = await response.json();
                
                const heapUsagePercent = ((data.heap.used / data.heap.size) * 100).toFixed(1);
                
                let html = `
                    <div class="info-item">
                        <span class="info-label" data-i18n="dashboard_heap_free">Heap Libre</span>
                        <span class="info-value">${data.heap.free_formatted}</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label" data-i18n="dashboard_heap_size">Taille Heap</span>
                        <span class="info-value">${data.heap.size_formatted}</span>
                    </div>
                    <div class="progress-bar">
                        <div class="progress-fill" style="width: ${heapUsagePercent}%"></div>
                    </div>
                    <p style="margin-top: 10px; color: var(--text-secondary);">Utilisation: ${heapUsagePercent}%</p>
                `;
                
                if (data.psram) {
                    const psramUsagePercent = ((data.psram.used / data.psram.size) * 100).toFixed(1);
                    html += `
                        <div class="info-item" style="margin-top: 20px;">
                            <span class="info-label">PSRAM</span>
                            <span class="info-value">${data.psram.free_formatted} / ${data.psram.size_formatted}</span>
                        </div>
                        <div class="progress-bar">
                            <div class="progress-fill" style="width: ${psramUsagePercent}%"></div>
                        </div>
                    `;
                }
                
                document.getElementById('memoryInfo').innerHTML = html;
            } catch (error) {
                document.getElementById('memoryInfo').innerHTML = '<p style="color: var(--error);">Error loading data</p>';
            }
        }
        
        // ============================================
        // DASHBOARD - WIFI
        // ============================================
        
        async function loadWiFiInfo() {
            try {
                const response = await fetch('/api/system/wifi');
                const data = await response.json();
                
                let html = '';
                
                if (data.connected) {
                    html = `
                        <div class="info-item">
                            <span class="info-label">Status</span>
                            <span class="status-badge status-success" data-i18n="dashboard_connected">Connecté</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label" data-i18n="dashboard_ssid">SSID</span>
                            <span class="info-value">${data.ssid}</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label" data-i18n="dashboard_ip">Adresse IP</span>
                            <span class="info-value">${data.ip}</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label" data-i18n="dashboard_rssi">Signal</span>
                            <span class="info-value">${data.rssi} dBm (${data.signal_quality})</span>
                        </div>
                    `;
                } else {
                    html = `
                        <div class="info-item">
                            <span class="info-label">Status</span>
                            <span class="status-badge status-error" data-i18n="dashboard_disconnected">Déconnecté</span>
                        </div>
                    `;
                }
                
                document.getElementById('wifiInfo').innerHTML = html;
            } catch (error) {
                document.getElementById('wifiInfo').innerHTML = '<p style="color: var(--error);">Error loading data</p>';
            }
        }
        
        // ============================================
        // TESTS MATÉRIELS
        // ============================================
        
        async function runAllTests() {
            const resultsDiv = document.getElementById('testResults');
            resultsDiv.innerHTML = '<div class="loading"></div> <span data-i18n="tests_running">Test en cours...</span>';
            
            try {
                const response = await fetch('/api/tests');
                const data = await response.json();
                
                let html = '<table class="table"><thead><tr><th data-i18n="tests_pin">Pin</th><th data-i18n="tests_status">État</th></tr></thead><tbody>';
                
                // Tests GPIO
                if (data.gpio_test) {
                    data.gpio_test.forEach(pin => {
                        const statusClass = pin.status === 'OK' ? 'status-success' : 'status-error';
                        html += `<tr><td>GPIO ${pin.pin}</td><td><span class="status-badge ${statusClass}">${pin.status}</span></td></tr>`;
                    });
                }
                
                html += '</tbody></table>';
                resultsDiv.innerHTML = html;
            } catch (error) {
                resultsDiv.innerHTML = '<p style="color: var(--error);">Error running tests</p>';
            }
        }
        
        // ============================================
        // MÉMOIRE DÉTAILLÉE
        // ============================================
        
        async function loadMemoryDetails() {
            try {
                const response = await fetch('/api/system/memory');
                const data = await response.json();
                
                const heapUsagePercent = ((data.heap.used / data.heap.size) * 100).toFixed(1);
                
                let html = `
                    <div class="card-grid">
                        <div>
                            <h3 style="margin-bottom: 15px;">💾 <span data-i18n="memory_heap">Mémoire Heap</span></h3>
                            <div class="info-item">
                                <span class="info-label" data-i18n="memory_free">Libre</span>
                                <span class="info-value">${data.heap.free_formatted}</span>
                            </div>
                            <div class="info-item">
                                <span class="info-label" data-i18n="memory_used">Utilisée</span>
                                <span class="info-value">${formatBytes(data.heap.used)}</span>
                            </div>
                            <div class="info-item">
                                <span class="info-label" data-i18n="memory_total">Total</span>
                                <span class="info-value">${data.heap.size_formatted}</span>
                            </div>
                            <div class="progress-bar" style="margin-top: 15px;">
                                <div class="progress-fill" style="width: ${heapUsagePercent}%"></div>
                            </div>
                            <p style="margin-top: 10px; color: var(--text-secondary);">
                                <span data-i18n="memory_usage">Utilisation</span>: ${heapUsagePercent}%
                            </p>
                        </div>
                `;
                
                if (data.psram) {
                    const psramUsagePercent = ((data.psram.used / data.psram.size) * 100).toFixed(1);
                    html += `
                        <div>
                            <h3 style="margin-bottom: 15px;">🧠 <span data-i18n="memory_psram">Mémoire PSRAM</span></h3>
                            <div class="info-item">
                                <span class="info-label" data-i18n="memory_free">Libre</span>
                                <span class="info-value">${data.psram.free_formatted}</span>
                            </div>
                            <div class="info-item">
                                <span class="info-label" data-i18n="memory_used">Utilisée</span>
                                <span class="info-value">${formatBytes(data.psram.used)}</span>
                            </div>
                            <div class="info-item">
                                <span class="info-label" data-i18n="memory_total">Total</span>
                                <span class="info-value">${data.psram.size_formatted}</span>
                            </div>
                            <div class="progress-bar" style="margin-top: 15px;">
                                <div class="progress-fill" style="width: ${psramUsagePercent}%"></div>
                            </div>
                            <p style="margin-top: 10px; color: var(--text-secondary);">
                                <span data-i18n="memory_usage">Utilisation</span>: ${psramUsagePercent}%
                            </p>
                        </div>
                    `;
                } else {
                    html += `
                        <div>
                            <h3 style="margin-bottom: 15px;">🧠 PSRAM</h3>
                            <p style="color: var(--text-secondary);" data-i18n="memory_not_available">Non disponible</p>
                        </div>
                    `;
                }
                
                html += '</div>';
                document.getElementById('memoryDetails').innerHTML = html;
            } catch (error) {
                document.getElementById('memoryDetails').innerHTML = '<p style="color: var(--error);">Error loading data</p>';
            }
        }
        
        // ============================================
        // WIFI DÉTAILLÉ
        // ============================================
        
        async function loadWiFiDetails() {
            try {
                const response = await fetch('/api/system/wifi');
                const data = await response.json();
                
                let html = '';
                
                if (data.connected) {
                    html = `
                        <div class="info-item">
                            <span class="info-label">Status</span>
                            <span class="status-badge status-success" data-i18n="dashboard_connected">Connecté</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label" data-i18n="wifi_network_name">Nom du réseau</span>
                            <span class="info-value">${data.ssid}</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label" data-i18n="wifi_ip_address">Adresse IP</span>
                            <span class="info-value">${data.ip}</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label" data-i18n="wifi_mac_address">Adresse MAC</span>
                            <span class="info-value">${data.mac}</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label" data-i18n="wifi_signal_strength">Force du signal</span>
                            <span class="info-value">${data.rssi} dBm (${data.signal_quality})</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label" data-i18n="wifi_channel">Canal</span>
                            <span class="info-value">${data.channel}</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label" data-i18n="wifi_gateway">Passerelle</span>
                            <span class="info-value">${data.gateway}</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label" data-i18n="wifi_subnet">Masque de sous-réseau</span>
                            <span class="info-value">${data.subnet}</span>
                        </div>
                    `;
                } else {
                    html = `
                        <div class="info-item">
                            <span class="info-label">Status</span>
                            <span class="status-badge status-error" data-i18n="dashboard_disconnected">Déconnecté</span>
                        </div>
                    `;
                }
                
                document.getElementById('wifiDetails').innerHTML = html;
            } catch (error) {
                document.getElementById('wifiDetails').innerHTML = '<p style="color: var(--error);">Error loading data</p>';
            }
        }
        
        // ============================================
        // SCAN WIFI
        // ============================================
        
        async function scanWiFi() {
            const resultsDiv = document.getElementById('wifiScanResults');
            resultsDiv.innerHTML = '<div class="loading"></div> <span data-i18n="wifi_scanning">Scan en cours...</span>';
            
            try {
                const response = await fetch('/api/wifi/scan', {method: 'POST'});
                const data = await response.json();
                
                let html = `<p style="margin-top: 20px;"><span data-i18n="wifi_networks_found">Réseaux trouvés</span>: ${data.count}</p>`;
                html += '<table class="table"><thead><tr><th>SSID</th><th data-i18n="wifi_signal_strength">Signal</th><th data-i18n="wifi_channel">Canal</th><th data-i18n="wifi_encryption">Chiffrement</th></tr></thead><tbody>';
                
                data.networks.forEach(network => {
                    html += `<tr>
                        <td>${network.ssid || '(hidden)'}</td>
                        <td>${network.rssi} dBm</td>
                        <td>${network.channel}</td>
                        <td>${network.encryption}</td>
                    </tr>`;
                });
                
                html += '</tbody></table>';
                resultsDiv.innerHTML = html;
            } catch (error) {
                resultsDiv.innerHTML = '<p style="color: var(--error);">Error scanning networks</p>';
            }
        }
        
        // ============================================
        // TEST GPIO
        // ============================================
        
        async function testGPIO() {
            const resultsDiv = document.getElementById('gpioResults');
            resultsDiv.innerHTML = '<div class="loading"></div> <span data-i18n="tests_running">Test en cours...</span>';
            
            try {
                const response = await fetch('/api/tests/gpio');
                const data = await response.json();
                
                let html = '<table class="table"><thead><tr><th data-i18n="tests_pin">Pin</th><th data-i18n="tests_status">État</th><th>HIGH</th><th>LOW</th></tr></thead><tbody>';
                
                data.results.forEach(pin => {
                    const statusClass = pin.status === 'OK' ? 'status-success' : 'status-error';
                    html += `<tr>
                        <td>GPIO ${pin.pin}</td>
                        <td><span class="status-badge ${statusClass}">${pin.status}</span></td>
                        <td>${pin.high_read}</td>
                        <td>${pin.low_read}</td>
                    </tr>`;
                });
                
                html += '</tbody></table>';
                resultsDiv.innerHTML = html;
            } catch (error) {
                resultsDiv.innerHTML = '<p style="color: var(--error);">Error running GPIO test</p>';
            }
        }
        
        // ============================================
        // BENCHMARKS
        // ============================================
        
        async function runBenchmarks() {
            const resultsDiv = document.getElementById('benchResults');
            resultsDiv.innerHTML = '<div class="loading"></div> <span data-i18n="bench_running">Benchmark en cours...</span>';
            
            try {
                const response = await fetch('/api/benchmark');
                const data = await response.json();
                
                let html = `
                    <div class="info-item">
                        <span class="info-label" data-i18n="bench_cpu">Benchmark CPU</span>
                        <span class="info-value">${data.cpu_benchmark_ms} ms</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label" data-i18n="bench_memory">Benchmark Mémoire</span>
                        <span class="info-value">${data.memory_benchmark_ms} ms</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label" data-i18n="wifi_signal_strength">Signal WiFi</span>
                        <span class="info-value">${data.wifi_rssi} dBm</span>
                    </div>
                `;
                
                resultsDiv.innerHTML = html;
            } catch (error) {
                resultsDiv.innerHTML = '<p style="color: var(--error);">Error running benchmarks</p>';
            }
        }
        
        // ============================================
        // EXPORT FONCTIONS
        // ============================================
        
        function exportJSON() {
            window.location.href = '/api/export/json';
        }
        
        function exportCSV() {
            window.location.href = '/api/export/csv';
        }
        
        // ============================================
        // UTILITAIRES
        // ============================================
        
        function formatBytes(bytes) {
            if (bytes < 1024) return bytes + ' B';
            else if (bytes < 1024 * 1024) return (bytes / 1024).toFixed(2) + ' KB';
            else if (bytes < 1024 * 1024 * 1024) return (bytes / 1024 / 1024).toFixed(2) + ' MB';
            else return (bytes / 1024 / 1024 / 1024).toFixed(2) + ' GB';
        }
        
        // ============================================
        // MISE À JOUR GLOBALE
        // ============================================
        
        function updateAllData() {
            loadSystemInfo();
            loadMemoryInfo();
            loadWiFiInfo();
            loadMemoryDetails();
            loadWiFiDetails();
        }
        
        // ============================================
        // INITIALISATION
        // ============================================
        
        window.addEventListener('load', () => {
            loadTranslations();
            updateAllData();
            
            // Auto-refresh toutes les 5 secondes
            setInterval(updateAllData, 5000);
        });
    </script>
</body>
</html>
)rawliteral";

// ============================================================
// HANDLER DE LA PAGE PRINCIPALE
// ============================================================

void handleRoot() {
  server.send(200, "text/html", MAIN_PAGE);
}

#endif // WEB_INTERFACE_H