/*
 * ============================================================
 * WEB INTERFACE - ESP32 Diagnostic System
 * ============================================================
 *
 * Version: 3.0.1
 * Date: October 2025
 * Arduino Core: 3.3.2+
 *
 * Description:
 *   Modern web interface with glassmorphism design, real-time
 *   updates, and multilingual support. Optimized for Core 3.3.2.
 *
 * Features:
 *   - Responsive glassmorphism design
 *   - Real-time data updates (5s interval)
 *   - Tab-based navigation
 *   - French/English language toggle
 *   - Animated UI elements
 *   - JSON/CSV export
 *
 * ============================================================
 */

#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Diagnostic v3.0.1</title>
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }

    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      min-height: 100vh;
      padding: 20px;
      position: relative;
      overflow-x: hidden;
    }

    /* Animated background gradient - Core 3.3.2 optimized */
    body::before {
      content: '';
      position: fixed;
      top: 0;
      left: 0;
      width: 100%;
      height: 100%;
      background: linear-gradient(45deg,
        rgba(102, 126, 234, 0.8) 0%,
        rgba(118, 75, 162, 0.8) 50%,
        rgba(102, 126, 234, 0.8) 100%);
      background-size: 400% 400%;
      animation: gradientShift 15s ease infinite;
      z-index: -1;
    }

    @keyframes gradientShift {
      0% { background-position: 0% 50%; }
      50% { background-position: 100% 50%; }
      100% { background-position: 0% 50%; }
    }

    /* Glassmorphism container */
    .container {
      max-width: 1200px;
      margin: 0 auto;
      background: rgba(255, 255, 255, 0.1);
      backdrop-filter: blur(10px);
      border-radius: 20px;
      padding: 30px;
      box-shadow: 0 8px 32px 0 rgba(31, 38, 135, 0.37);
      border: 1px solid rgba(255, 255, 255, 0.18);
      animation: fadeIn 0.5s ease-in;
    }

    @keyframes fadeIn {
      from { opacity: 0; transform: translateY(20px); }
      to { opacity: 1; transform: translateY(0); }
    }

    /* Header */
    .header {
      display: flex;
      justify-content: space-between;
      align-items: center;
      margin-bottom: 30px;
      flex-wrap: wrap;
      gap: 15px;
    }

    h1 {
      color: white;
      font-size: 2em;
      text-shadow: 2px 2px 4px rgba(0,0,0,0.2);
      display: flex;
      align-items: center;
      gap: 10px;
    }

    .version-badge {
      background: rgba(255, 255, 255, 0.2);
      padding: 5px 15px;
      border-radius: 20px;
      font-size: 0.5em;
      font-weight: normal;
      backdrop-filter: blur(5px);
    }

    /* Language switcher */
    .lang-switcher {
      display: flex;
      gap: 10px;
    }

    .lang-btn {
      padding: 8px 16px;
      background: rgba(255, 255, 255, 0.2);
      border: 2px solid rgba(255, 255, 255, 0.3);
      border-radius: 10px;
      color: white;
      cursor: pointer;
      font-size: 14px;
      font-weight: bold;
      transition: all 0.3s ease;
      backdrop-filter: blur(5px);
    }

    .lang-btn:hover {
      background: rgba(255, 255, 255, 0.3);
      transform: translateY(-2px);
      box-shadow: 0 4px 12px rgba(0,0,0,0.2);
    }

    .lang-btn.active {
      background: rgba(255, 255, 255, 0.4);
      border-color: rgba(255, 255, 255, 0.6);
    }

    /* Tabs */
    .tabs {
      display: flex;
      gap: 10px;
      margin-bottom: 25px;
      border-bottom: 2px solid rgba(255, 255, 255, 0.2);
      padding-bottom: 10px;
      flex-wrap: wrap;
    }

    .tab {
      padding: 12px 24px;
      background: rgba(255, 255, 255, 0.1);
      border: none;
      border-radius: 10px 10px 0 0;
      color: white;
      cursor: pointer;
      font-size: 16px;
      font-weight: 600;
      transition: all 0.3s ease;
      backdrop-filter: blur(5px);
    }

    .tab:hover {
      background: rgba(255, 255, 255, 0.2);
      transform: translateY(-2px);
    }

    .tab.active {
      background: rgba(255, 255, 255, 0.3);
      border-bottom: 3px solid white;
    }

    /* Tab content */
    .tab-content {
      display: none;
      animation: fadeIn 0.3s ease-in;
    }

    .tab-content.active {
      display: block;
    }

    /* Cards */
    .card {
      background: rgba(255, 255, 255, 0.15);
      backdrop-filter: blur(10px);
      border-radius: 15px;
      padding: 20px;
      margin-bottom: 20px;
      border: 1px solid rgba(255, 255, 255, 0.2);
      box-shadow: 0 4px 16px rgba(0,0,0,0.1);
      transition: transform 0.3s ease, box-shadow 0.3s ease;
    }

    .card:hover {
      transform: translateY(-5px);
      box-shadow: 0 8px 24px rgba(0,0,0,0.2);
    }

    .card h2 {
      color: white;
      margin-bottom: 15px;
      font-size: 1.5em;
      display: flex;
      align-items: center;
      gap: 10px;
    }

    /* Info grid */
    .info-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
      gap: 15px;
    }

    .info-item {
      background: rgba(255, 255, 255, 0.1);
      padding: 15px;
      border-radius: 10px;
      border-left: 4px solid rgba(255, 255, 255, 0.5);
    }

    .info-label {
      color: rgba(255, 255, 255, 0.8);
      font-size: 0.9em;
      margin-bottom: 5px;
      font-weight: 600;
    }

    .info-value {
      color: white;
      font-size: 1.2em;
      font-weight: bold;
    }

    /* Tables */
    table {
      width: 100%;
      border-collapse: collapse;
      background: rgba(255, 255, 255, 0.1);
      border-radius: 10px;
      overflow: hidden;
    }

    th {
      background: rgba(255, 255, 255, 0.2);
      color: white;
      padding: 12px;
      text-align: left;
      font-weight: 600;
    }

    td {
      padding: 10px 12px;
      color: white;
      border-bottom: 1px solid rgba(255, 255, 255, 0.1);
    }

    tr:hover {
      background: rgba(255, 255, 255, 0.05);
    }

    /* Buttons */
    .btn {
      padding: 12px 24px;
      background: rgba(255, 255, 255, 0.2);
      border: 2px solid rgba(255, 255, 255, 0.3);
      border-radius: 10px;
      color: white;
      cursor: pointer;
      font-size: 16px;
      font-weight: bold;
      transition: all 0.3s ease;
      backdrop-filter: blur(5px);
      display: inline-flex;
      align-items: center;
      gap: 8px;
    }

    .btn:hover {
      background: rgba(255, 255, 255, 0.3);
      transform: translateY(-2px);
      box-shadow: 0 4px 12px rgba(0,0,0,0.2);
    }

    .btn:active {
      transform: translateY(0);
    }

    /* Update indicator */
    .update-indicator {
      display: flex;
      align-items: center;
      gap: 10px;
      padding: 15px;
      background: rgba(255, 255, 255, 0.15);
      border-radius: 10px;
      margin-bottom: 20px;
    }

    .pulse-dot {
      width: 12px;
      height: 12px;
      background: #4ade80;
      border-radius: 50%;
      animation: pulse 2s ease-in-out infinite;
    }

    @keyframes pulse {
      0%, 100% { opacity: 1; transform: scale(1); }
      50% { opacity: 0.5; transform: scale(1.2); }
    }

    .update-text {
      color: white;
      font-weight: 600;
    }

    .progress-bar {
      flex: 1;
      height: 8px;
      background: rgba(255, 255, 255, 0.2);
      border-radius: 10px;
      overflow: hidden;
    }

    .progress-fill {
      height: 100%;
      background: linear-gradient(90deg, #4ade80, #22c55e);
      border-radius: 10px;
      transition: width 1s linear;
      box-shadow: 0 0 10px rgba(74, 222, 128, 0.5);
    }

    /* Status badges */
    .status-ok {
      color: #4ade80;
      font-weight: bold;
    }

    .status-warn {
      color: #fbbf24;
      font-weight: bold;
    }

    .status-error {
      color: #f87171;
      font-weight: bold;
    }

    /* Loading animation */
    .loading {
      text-align: center;
      padding: 40px;
      color: white;
      font-size: 1.2em;
    }

    .spinner {
      border: 4px solid rgba(255, 255, 255, 0.3);
      border-top: 4px solid white;
      border-radius: 50%;
      width: 50px;
      height: 50px;
      animation: spin 1s linear infinite;
      margin: 20px auto;
    }

    @keyframes spin {
      0% { transform: rotate(0deg); }
      100% { transform: rotate(360deg); }
    }

    /* Responsive */
    @media (max-width: 768px) {
      .container {
        padding: 15px;
      }

      h1 {
        font-size: 1.5em;
      }

      .info-grid {
        grid-template-columns: 1fr;
      }

      .tabs {
        overflow-x: auto;
      }

      table {
        font-size: 0.9em;
      }
    }
  </style>
</head>
<body>
  <div class="container">
    <!-- Header -->
    <div class="header">
      <h1>
        📟 <span id="pageTitle">Diagnostic ESP32</span>
        <span class="version-badge">v3.0.1</span>
      </h1>
      <div class="lang-switcher">
        <button class="lang-btn active" onclick="setLanguage('fr')">🇫🇷 FR</button>
        <button class="lang-btn" onclick="setLanguage('en')">🇬🇧 EN</button>
      </div>
    </div>

    <!-- Update Indicator -->
    <div class="update-indicator">
      <div class="pulse-dot"></div>
      <span class="update-text" id="updateText">Mise à jour automatique dans <span id="countdown">5</span>s</span>
      <div class="progress-bar">
        <div class="progress-fill" id="progressBar"></div>
      </div>
    </div>

    <!-- Tabs -->
    <div class="tabs">
      <button class="tab active" onclick="showTab('overview')" id="tabOverview">📊 Vue générale</button>
      <button class="tab" onclick="showTab('tests')" id="tabTests">🧪 Tests</button>
      <button class="tab" onclick="showTab('exports')" id="tabExports">💾 Exports</button>
    </div>

    <!-- Tab Content: Overview -->
    <div id="overview" class="tab-content active">
      <div class="card">
        <h2>🖥️ <span id="sysInfoTitle">Informations Système</span></h2>
        <div class="info-grid" id="systemInfo">
          <div class="loading">
            <div class="spinner"></div>
            <p id="loadingText">Chargement...</p>
          </div>
        </div>
      </div>

      <div class="card">
        <h2>💾 <span id="memInfoTitle">Mémoire</span></h2>
        <div class="info-grid" id="memoryInfo">
          <div class="loading">
            <div class="spinner"></div>
          </div>
        </div>
      </div>

      <div class="card">
        <h2>📡 WiFi</h2>
        <div class="info-grid" id="wifiInfo">
          <div class="loading">
            <div class="spinner"></div>
          </div>
        </div>
      </div>
    </div>

    <!-- Tab Content: Tests -->
    <div id="tests" class="tab-content">
      <div class="card">
        <h2>🔌 <span id="gpioTestTitle">Test GPIO</span></h2>
        <div id="gpioTest">
          <div class="loading">
            <div class="spinner"></div>
          </div>
        </div>
      </div>

      <div class="card">
        <h2>🔗 <span id="i2cTestTitle">Test I2C</span></h2>
        <div id="i2cTest">
          <div class="loading">
            <div class="spinner"></div>
          </div>
        </div>
      </div>

      <div class="card">
        <h2>⚡ <span id="spiTestTitle">Test SPI</span></h2>
        <div id="spiTest">
          <div class="loading">
            <div class="spinner"></div>
          </div>
        </div>
      </div>

      <div class="card">
        <h2>💾 <span id="memTestTitle">Test Mémoire</span></h2>
        <div id="memoryTest">
          <div class="loading">
            <div class="spinner"></div>
          </div>
        </div>
      </div>

      <div class="card">
        <h2>📶 <span id="wifiTestTitle">Test WiFi</span></h2>
        <div id="wifiTest">
          <div class="loading">
            <div class="spinner"></div>
          </div>
        </div>
      </div>

      <div class="card">
        <h2>⚙️ <span id="sysTestTitle">Test Système</span></h2>
        <div id="systemTest">
          <div class="loading">
            <div class="spinner"></div>
          </div>
        </div>
      </div>
    </div>

    <!-- Tab Content: Exports -->
    <div id="exports" class="tab-content">
      <div class="card">
        <h2>💾 <span id="exportTitle">Export des Données</span></h2>
        <p style="color: rgba(255,255,255,0.8); margin-bottom: 20px;" id="exportDesc">
          Télécharger les données de diagnostic dans différents formats
        </p>
        <div style="display: flex; gap: 15px; flex-wrap: wrap;">
          <button class="btn" onclick="exportJSON()">
            📄 <span id="btnExportJSON">Exporter JSON</span>
          </button>
          <button class="btn" onclick="exportCSV()">
            📊 <span id="btnExportCSV">Exporter CSV</span>
          </button>
        </div>
      </div>
    </div>
  </div>

  <script>
    let currentLang = 'fr';
    let countdownValue = 5;
    let countdownInterval;

    // Translations
    const translations = {
      fr: {
        pageTitle: 'Diagnostic ESP32',
        tabOverview: '📊 Vue générale',
        tabTests: '🧪 Tests',
        tabExports: '💾 Exports',
        sysInfoTitle: 'Informations Système',
        memInfoTitle: 'Mémoire',
        gpioTestTitle: 'Test GPIO',
        i2cTestTitle: 'Test I2C',
        spiTestTitle: 'Test SPI',
        memTestTitle: 'Test Mémoire',
        wifiTestTitle: 'Test WiFi',
        sysTestTitle: 'Test Système',
        exportTitle: 'Export des Données',
        exportDesc: 'Télécharger les données de diagnostic dans différents formats',
        btnExportJSON: 'Exporter JSON',
        btnExportCSV: 'Exporter CSV',
        loadingText: 'Chargement...',
        updateText: 'Mise à jour automatique dans',
        chip: 'Puce',
        cores: 'Cœurs CPU',
        frequency: 'Fréquence',
        flash: 'Flash',
        psram: 'PSRAM',
        uptime: 'Durée',
        heapFree: 'Heap Libre',
        heapSize: 'Taille Heap',
        psramFree: 'PSRAM Libre',
        psramSize: 'Taille PSRAM',
        status: 'Statut',
        ssid: 'SSID',
        ip: 'Adresse IP',
        rssi: 'Signal',
        connected: 'Connecté',
        disconnected: 'Déconnecté',
        pin: 'Pin',
        details: 'Détails',
        address: 'Adresse',
        device: 'Périphérique'
      },
      en: {
        pageTitle: 'ESP32 Diagnostic',
        tabOverview: '📊 Overview',
        tabTests: '🧪 Tests',
        tabExports: '💾 Exports',
        sysInfoTitle: 'System Information',
        memInfoTitle: 'Memory',
        gpioTestTitle: 'GPIO Test',
        i2cTestTitle: 'I2C Test',
        spiTestTitle: 'SPI Test',
        memTestTitle: 'Memory Test',
        wifiTestTitle: 'WiFi Test',
        sysTestTitle: 'System Test',
        exportTitle: 'Data Export',
        exportDesc: 'Download diagnostic data in various formats',
        btnExportJSON: 'Export JSON',
        btnExportCSV: 'Export CSV',
        loadingText: 'Loading...',
        updateText: 'Auto-update in',
        chip: 'Chip',
        cores: 'CPU Cores',
        frequency: 'Frequency',
        flash: 'Flash',
        psram: 'PSRAM',
        uptime: 'Uptime',
        heapFree: 'Heap Free',
        heapSize: 'Heap Size',
        psramFree: 'PSRAM Free',
        psramSize: 'PSRAM Size',
        status: 'Status',
        ssid: 'SSID',
        ip: 'IP Address',
        rssi: 'Signal',
        connected: 'Connected',
        disconnected: 'Disconnected',
        pin: 'Pin',
        details: 'Details',
        address: 'Address',
        device: 'Device'
      }
    };

    function t(key) {
      return translations[currentLang][key] || key;
    }

    function updateUIText() {
      document.getElementById('pageTitle').textContent = t('pageTitle');
      document.getElementById('tabOverview').innerHTML = t('tabOverview');
      document.getElementById('tabTests').innerHTML = t('tabTests');
      document.getElementById('tabExports').innerHTML = t('tabExports');
      document.getElementById('sysInfoTitle').textContent = t('sysInfoTitle');
      document.getElementById('memInfoTitle').textContent = t('memInfoTitle');
      document.getElementById('gpioTestTitle').textContent = t('gpioTestTitle');
      document.getElementById('i2cTestTitle').textContent = t('i2cTestTitle');
      document.getElementById('spiTestTitle').textContent = t('spiTestTitle');
      document.getElementById('memTestTitle').textContent = t('memTestTitle');
      document.getElementById('wifiTestTitle').textContent = t('wifiTestTitle');
      document.getElementById('sysTestTitle').textContent = t('sysTestTitle');
      document.getElementById('exportTitle').textContent = t('exportTitle');
      document.getElementById('exportDesc').textContent = t('exportDesc');
      document.getElementById('btnExportJSON').textContent = t('btnExportJSON');
      document.getElementById('btnExportCSV').textContent = t('btnExportCSV');
      document.getElementById('loadingText').textContent = t('loadingText');
      document.getElementById('updateText').innerHTML = t('updateText') + ' <span id="countdown">' + countdownValue + '</span>s';
    }

    // Language switcher
    function setLanguage(lang) {
      currentLang = lang;

      // Update button states
      document.querySelectorAll('.lang-btn').forEach(btn => {
        btn.classList.remove('active');
      });
      event.target.classList.add('active');

      // Update UI text
      updateUIText();

      // Reload data with new language
      loadData();

      // Send to server
      fetch('/api/language', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({language: lang})
      });
    }

    // Tab switching
    function showTab(tabName) {
      // Hide all tabs
      document.querySelectorAll('.tab-content').forEach(content => {
        content.classList.remove('active');
      });
      document.querySelectorAll('.tab').forEach(tab => {
        tab.classList.remove('active');
      });

      // Show selected tab
      document.getElementById(tabName).classList.add('active');
      event.target.classList.add('active');
    }

    // Load system data
    async function loadData() {
      try {
        // Load system info
        const sysResponse = await fetch('/api/system');
        const sysData = await sysResponse.json();

        // Display system info
        document.getElementById('systemInfo').innerHTML = `
          <div class="info-item">
            <div class="info-label">${t('chip')}</div>
            <div class="info-value">${sysData.chip}</div>
          </div>
          <div class="info-item">
            <div class="info-label">${t('cores')}</div>
            <div class="info-value">${sysData.cores}</div>
          </div>
          <div class="info-item">
            <div class="info-label">${t('frequency')}</div>
            <div class="info-value">${sysData.frequency} MHz</div>
          </div>
          <div class="info-item">
            <div class="info-label">${t('flash')}</div>
            <div class="info-value">${sysData.flash_size}</div>
          </div>
          <div class="info-item">
            <div class="info-label">${t('psram')}</div>
            <div class="info-value">${sysData.psram_size}</div>
          </div>
          <div class="info-item">
            <div class="info-label">${t('uptime')}</div>
            <div class="info-value">${sysData.uptime}</div>
          </div>
          <div class="info-item">
            <div class="info-label">Version</div>
            <div class="info-value">${sysData.version}</div>
          </div>
          <div class="info-item">
            <div class="info-label">Arduino Core</div>
            <div class="info-value">${sysData.core_version}</div>
          </div>
        `;

        // Display memory info
        const mem = sysData.memory;
        let memHtml = `
          <div class="info-item">
            <div class="info-label">${t('heapFree')}</div>
            <div class="info-value">${mem.heap_free_formatted}</div>
          </div>
          <div class="info-item">
            <div class="info-label">${t('heapSize')}</div>
            <div class="info-value">${mem.heap_size_formatted}</div>
          </div>
        `;

        if (mem.psram_free !== undefined) {
          memHtml += `
            <div class="info-item">
              <div class="info-label">${t('psramFree')}</div>
              <div class="info-value">${mem.psram_free_formatted}</div>
            </div>
            <div class="info-item">
              <div class="info-label">${t('psramSize')}</div>
              <div class="info-value">${mem.psram_size_formatted}</div>
            </div>
          `;
        }

        document.getElementById('memoryInfo').innerHTML = memHtml;

        // Display WiFi info
        const wifi = sysData.wifi;
        document.getElementById('wifiInfo').innerHTML = `
          <div class="info-item">
            <div class="info-label">${t('status')}</div>
            <div class="info-value ${wifi.connected ? 'status-ok' : 'status-error'}">
              ${wifi.connected ? t('connected') : t('disconnected')}
            </div>
          </div>
          ${wifi.connected ? `
            <div class="info-item">
              <div class="info-label">${t('ssid')}</div>
              <div class="info-value">${wifi.ssid}</div>
            </div>
            <div class="info-item">
              <div class="info-label">${t('ip')}</div>
              <div class="info-value">${wifi.ip}</div>
            </div>
            <div class="info-item">
              <div class="info-label">${t('rssi')}</div>
              <div class="info-value">${wifi.rssi} dBm</div>
            </div>
          ` : ''}
        `;

        // Load tests
        const testsResponse = await fetch('/api/tests');
        const testsData = await testsResponse.json();

        // GPIO Test
        let gpioHtml = '<table><thead><tr><th>' + t('pin') + '</th><th>' + t('status') + '</th><th>' + t('details') + '</th></tr></thead><tbody>';
        testsData.gpio_test.forEach(pin => {
          const statusClass = pin.status === 'OK' ? 'status-ok' : 'status-error';
          gpioHtml += `<tr><td>GPIO ${pin.pin}</td><td class="${statusClass}">${pin.status}</td><td>${pin.details}</td></tr>`;
        });
        gpioHtml += '</tbody></table>';
        document.getElementById('gpioTest').innerHTML = gpioHtml;

        // I2C Test
        let i2cHtml = '<table><thead><tr><th>' + t('address') + '</th><th>' + t('device') + '</th><th>' + t('status') + '</th></tr></thead><tbody>';
        testsData.i2c_test.forEach(device => {
          i2cHtml += `<tr><td>${device.address}</td><td>${device.device}</td><td class="status-ok">${device.status}</td></tr>`;
        });
        i2cHtml += '</tbody></table>';
        document.getElementById('i2cTest').innerHTML = i2cHtml;

        // SPI Test
        let spiHtml = '<table><thead><tr><th>Bus</th><th>MOSI</th><th>MISO</th><th>SCLK</th><th>' + t('status') + '</th></tr></thead><tbody>';
        testsData.spi_test.forEach(spi => {
          spiHtml += `<tr><td>${spi.bus}</td><td>${spi.mosi}</td><td>${spi.miso}</td><td>${spi.sclk}</td><td class="status-ok">${spi.status}</td></tr>`;
        });
        spiHtml += '</tbody></table>';
        document.getElementById('spiTest').innerHTML = spiHtml;

        // Memory Test
        const memTest = testsData.memory_test;
        let memTestHtml = `
          <div class="info-grid">
            <div class="info-item">
              <div class="info-label">${t('heapFree')}</div>
              <div class="info-value">${memTest.heap_free}</div>
            </div>
            <div class="info-item">
              <div class="info-label">${t('heapSize')}</div>
              <div class="info-value">${memTest.heap_size}</div>
            </div>
            <div class="info-item">
              <div class="info-label">Min Free</div>
              <div class="info-value">${memTest.heap_min_free}</div>
            </div>
            <div class="info-item">
              <div class="info-label">Usage</div>
              <div class="info-value">${memTest.heap_usage_percent}%</div>
            </div>
            <div class="info-item">
              <div class="info-label">Largest Block</div>
              <div class="info-value">${memTest.largest_free_block}</div>
            </div>
            <div class="info-item">
              <div class="info-label">Fragmentation</div>
              <div class="info-value">${memTest.fragmentation_percent}%</div>
            </div>
            <div class="info-item">
              <div class="info-label">${t('status')}</div>
              <div class="info-value status-${memTest.status === 'OK' ? 'ok' : memTest.status === 'MEDIUM' ? 'warn' : 'error'}">${memTest.status}</div>
            </div>
            <div class="info-item">
              <div class="info-label">Alert</div>
              <div class="info-value" style="font-size: 0.9em;">${memTest.alert}</div>
            </div>
          </div>
        `;
        document.getElementById('memoryTest').innerHTML = memTestHtml;

        // WiFi Test
        const wifiTest = testsData.wifi_test;
        let wifiTestHtml = '<div class="info-grid">';

        if (wifiTest.connected) {
          wifiTestHtml += `
            <div class="info-item">
              <div class="info-label">${t('status')}</div>
              <div class="info-value status-ok">${wifiTest.status}</div>
            </div>
            <div class="info-item">
              <div class="info-label">${t('ssid')}</div>
              <div class="info-value">${wifiTest.ssid}</div>
            </div>
            <div class="info-item">
              <div class="info-label">${t('ip')}</div>
              <div class="info-value">${wifiTest.ip}</div>
            </div>
            <div class="info-item">
              <div class="info-label">MAC</div>
              <div class="info-value">${wifiTest.mac}</div>
            </div>
            <div class="info-item">
              <div class="info-label">${t('rssi')}</div>
              <div class="info-value">${wifiTest.rssi} dBm</div>
            </div>
            <div class="info-item">
              <div class="info-label">Signal Quality</div>
              <div class="info-value">${wifiTest.signal_quality}</div>
            </div>
            <div class="info-item">
              <div class="info-label">Channel</div>
              <div class="info-value">${wifiTest.channel}</div>
            </div>
            <div class="info-item">
              <div class="info-label">Gateway</div>
              <div class="info-value">${wifiTest.gateway}</div>
            </div>
            <div class="info-item">
              <div class="info-label">DNS</div>
              <div class="info-value">${wifiTest.dns}</div>
            </div>
            <div class="info-item">
              <div class="info-label">Subnet</div>
              <div class="info-value">${wifiTest.subnet}</div>
            </div>
          `;
        } else {
          wifiTestHtml += `
            <div class="info-item">
              <div class="info-label">${t('status')}</div>
              <div class="info-value status-error">${wifiTest.status}</div>
            </div>
            <div class="info-item">
              <div class="info-label">Error</div>
              <div class="info-value">${wifiTest.error}</div>
            </div>
          `;
        }

        wifiTestHtml += '</div>';
        document.getElementById('wifiTest').innerHTML = wifiTestHtml;

        // System Test
        const sysTest = testsData.system_test;
        let sysTestHtml = `
          <div class="info-grid">
            <div class="info-item">
              <div class="info-label">Chip Model</div>
              <div class="info-value">${sysTest.chip_model}</div>
            </div>
            <div class="info-item">
              <div class="info-label">Chip Revision</div>
              <div class="info-value">${sysTest.chip_revision}</div>
            </div>
            <div class="info-item">
              <div class="info-label">CPU Cores</div>
              <div class="info-value">${sysTest.cpu_cores}</div>
            </div>
            <div class="info-item">
              <div class="info-label">CPU Frequency</div>
              <div class="info-value">${sysTest.cpu_frequency} MHz</div>
            </div>
            <div class="info-item">
              <div class="info-label">Flash Size</div>
              <div class="info-value">${sysTest.flash_size}</div>
            </div>
            <div class="info-item">
              <div class="info-label">Flash Speed</div>
              <div class="info-value">${sysTest.flash_speed} MHz</div>
            </div>
            <div class="info-item">
              <div class="info-label">${t('uptime')}</div>
              <div class="info-value">${sysTest.uptime}</div>
            </div>
            <div class="info-item">
              <div class="info-label">Uptime (ms)</div>
              <div class="info-value">${sysTest.uptime_ms}</div>
            </div>
            <div class="info-item">
              <div class="info-label">Last Reset</div>
              <div class="info-value">${sysTest.last_reset_reason}</div>
            </div>
            <div class="info-item">
              <div class="info-label">Arduino Core</div>
              <div class="info-value">${sysTest.arduino_core}</div>
            </div>
            <div class="info-item">
              <div class="info-label">IDF Version</div>
              <div class="info-value">${sysTest.idf_version}</div>
            </div>
            <div class="info-item">
              <div class="info-label">Overall Status</div>
              <div class="info-value status-${sysTest.overall_status === 'HEALTHY' ? 'ok' : sysTest.overall_status === 'WARNING' ? 'warn' : 'error'}">${sysTest.overall_status}</div>
            </div>
          </div>
        `;
        document.getElementById('systemTest').innerHTML = sysTestHtml;

      } catch (error) {
        console.error('Error loading data:', error);
      }
    }

    // Export functions
    function exportJSON() {
      window.location.href = '/api/export/json';
    }

    function exportCSV() {
      window.location.href = '/api/export/csv';
    }

    // Countdown and progress bar
    function startCountdown() {
      clearInterval(countdownInterval);
      countdownValue = 5;

      countdownInterval = setInterval(() => {
        countdownValue--;
        document.getElementById('countdown').textContent = countdownValue;

        // Update progress bar
        const progress = ((5 - countdownValue) / 5) * 100;
        document.getElementById('progressBar').style.width = progress + '%';

        if (countdownValue <= 0) {
          countdownValue = 5;
          document.getElementById('progressBar').style.width = '0%';
          loadData();
        }
      }, 1000);
    }

    // Initialize on page load
    window.addEventListener('load', () => {
      loadData();
      startCountdown();
    });
  </script>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

#endif // WEB_INTERFACE_H
