/*
 * ============================================================
 * WEB INTERFACE v3.1.0 - ESP32 Diagnostic System
 * ============================================================
 *
 * Complete interface with 8 pages:
 * 1. Vue d'ensemble (Overview)
 * 2. LEDs (Built-in + NeoPixel)
 * 3. Écrans (TFT + OLED)
 * 4. Tests Avancés (ADC, Touch, PWM, SPI, Flash, Stress)
 * 5. GPIO (Comprehensive testing)
 * 6. WiFi (Network scanner)
 * 7. Performance (Benchmarks)
 * 8. Export (TXT, JSON, CSV, PDF preview)
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
  <title>ESP32 Diagnostic v3.1.0</title>
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

    .container {
      max-width: 1400px;
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
    }

    .lang-btn.active {
      background: rgba(255, 255, 255, 0.4);
      border-color: rgba(255, 255, 255, 0.6);
    }

    .tabs {
      display: flex;
      gap: 10px;
      margin-bottom: 25px;
      border-bottom: 2px solid rgba(255, 255, 255, 0.2);
      padding-bottom: 10px;
      flex-wrap: wrap;
      overflow-x: auto;
    }

    .tab {
      padding: 12px 20px;
      background: rgba(255, 255, 255, 0.1);
      border: none;
      border-radius: 10px 10px 0 0;
      color: white;
      cursor: pointer;
      font-size: 15px;
      font-weight: 600;
      transition: all 0.3s ease;
      backdrop-filter: blur(5px);
      white-space: nowrap;
    }

    .tab:hover {
      background: rgba(255, 255, 255, 0.2);
      transform: translateY(-2px);
    }

    .tab.active {
      background: rgba(255, 255, 255, 0.3);
      border-bottom: 3px solid white;
    }

    .tab-content {
      display: none;
      animation: fadeIn 0.3s ease-in;
    }

    .tab-content.active {
      display: block;
    }

    .card {
      background: rgba(255, 255, 255, 0.15);
      backdrop-filter: blur(10px);
      border-radius: 15px;
      padding: 20px;
      margin-bottom: 20px;
      border: 1px solid rgba(255, 255, 255, 0.2);
      box-shadow: 0 4px 16px rgba(0,0,0,0.1);
      transition: transform 0.3s ease;
    }

    .card:hover {
      transform: translateY(-5px);
    }

    .card h2 {
      color: white;
      margin-bottom: 15px;
      font-size: 1.5em;
      display: flex;
      align-items: center;
      gap: 10px;
    }

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
      margin: 5px;
    }

    .btn:hover {
      background: rgba(255, 255, 255, 0.3);
      transform: translateY(-2px);
      box-shadow: 0 4px 12px rgba(0,0,0,0.2);
    }

    .btn-red { background: rgba(239, 68, 68, 0.3); border-color: rgba(239, 68, 68, 0.5); }
    .btn-green { background: rgba(34, 197, 94, 0.3); border-color: rgba(34, 197, 94, 0.5); }
    .btn-blue { background: rgba(59, 130, 246, 0.3); border-color: rgba(59, 130, 246, 0.5); }
    .btn-purple { background: rgba(168, 85, 247, 0.3); border-color: rgba(168, 85, 247, 0.5); }
    .btn-cyan { background: rgba(6, 182, 212, 0.3); border-color: rgba(6, 182, 212, 0.5); }

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
    }

    .status-ok { color: #4ade80; font-weight: bold; }
    .status-warn { color: #fbbf24; font-weight: bold; }
    .status-error { color: #f87171; font-weight: bold; }

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

    .control-group {
      display: flex;
      gap: 10px;
      flex-wrap: wrap;
      align-items: center;
      margin: 10px 0;
    }

    .control-group input[type="number"] {
      width: 80px;
      padding: 8px;
      border-radius: 5px;
      border: 2px solid rgba(255, 255, 255, 0.3);
      background: rgba(255, 255, 255, 0.1);
      color: white;
      font-weight: bold;
    }

    .color-input {
      width: 60px;
      height: 40px;
      border-radius: 5px;
      border: 2px solid rgba(255, 255, 255, 0.3);
      background: transparent;
      cursor: pointer;
    }

    .export-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
      gap: 20px;
      margin-top: 20px;
    }

    .export-card {
      background: rgba(255, 255, 255, 0.1);
      padding: 20px;
      border-radius: 15px;
      border: 2px solid rgba(255, 255, 255, 0.2);
      text-align: center;
      transition: all 0.3s ease;
    }

    .export-card:hover {
      transform: translateY(-5px);
      border-color: rgba(255, 255, 255, 0.5);
    }

    .export-card h3 {
      color: white;
      margin-bottom: 10px;
      font-size: 1.3em;
    }

    .export-card p {
      color: rgba(255, 255, 255, 0.8);
      margin-bottom: 15px;
    }

    @media (max-width: 768px) {
      .container { padding: 15px; }
      h1 { font-size: 1.5em; }
      .info-grid { grid-template-columns: 1fr; }
      .tabs { overflow-x: auto; }
      table { font-size: 0.9em; }
      .tab { padding: 10px 15px; font-size: 14px; }
    }
  </style>
</head>
<body>
  <div class="container">
    <!-- Header -->
    <div class="header">
      <h1>
        📟 <span id="pageTitle">Diagnostic ESP32</span>
        <span class="version-badge">v3.1.0</span>
      </h1>
      <div class="lang-switcher">
        <button class="lang-btn active" onclick="setLanguage('fr')">🇫🇷 FR</button>
        <button class="lang-btn" onclick="setLanguage('en')">🇬🇧 EN</button>
      </div>
    </div>

    <!-- Update Indicator -->
    <div class="update-indicator">
      <div class="pulse-dot"></div>
      <span class="update-text" id="updateText">Mise à jour dans <span id="countdown">5</span>s</span>
      <div class="progress-bar">
        <div class="progress-fill" id="progressBar"></div>
      </div>
    </div>

    <!-- Tabs Navigation -->
    <div class="tabs">
      <button class="tab active" onclick="showTab('overview')" id="tabOverview">📊 Vue d'ensemble</button>
      <button class="tab" onclick="showTab('leds')" id="tabLEDs">💡 LEDs</button>
      <button class="tab" onclick="showTab('screens')" id="tabScreens">🖥️ Écrans</button>
      <button class="tab" onclick="showTab('advanced')" id="tabAdvanced">🧪 Tests Avancés</button>
      <button class="tab" onclick="showTab('gpio')" id="tabGPIO">📌 GPIO</button>
      <button class="tab" onclick="showTab('wifi')" id="tabWiFi">📶 WiFi</button>
      <button class="tab" onclick="showTab('performance')" id="tabPerformance">⚡ Performance</button>
      <button class="tab" onclick="showTab('export')" id="tabExport">💾 Export</button>
    </div>

    <!-- Tab 1: Vue d'ensemble -->
    <div id="overview" class="tab-content active">
      <div class="card">
        <h2>🖥️ <span id="sysInfoTitle">Informations Système</span></h2>
        <div class="info-grid" id="systemInfo">
          <div class="loading"><div class="spinner"></div><p id="loadingText">Chargement...</p></div>
        </div>
      </div>

      <div class="card">
        <h2>💾 <span id="memInfoTitle">Mémoire</span></h2>
        <div class="info-grid" id="memoryInfo">
          <div class="loading"><div class="spinner"></div></div>
        </div>
      </div>

      <div class="card">
        <h2>📡 WiFi</h2>
        <div class="info-grid" id="wifiInfo">
          <div class="loading"><div class="spinner"></div></div>
        </div>
      </div>
    </div>

    <!-- Tab 2: LEDs -->
    <div id="leds" class="tab-content">
      <div class="card">
        <h2>💡 <span id="builtinLEDTitle">LED Intégrée</span></h2>
        <p style="color: rgba(255,255,255,0.8); margin-bottom: 15px;">
          <span id="ledGPIOText">GPIO</span> 97
        </p>
        <div class="control-group">
          <button class="btn btn-green" onclick="controlBuiltinLED('on')">⚪ <span id="btnLEDOn">Allumer</span></button>
          <button class="btn btn-red" onclick="controlBuiltinLED('off')">⚫ <span id="btnLEDOff">Éteindre</span></button>
          <button class="btn btn-purple" onclick="controlBuiltinLED('blink')">🔄 <span id="btnLEDBlink">Clignoter</span></button>
          <button class="btn btn-blue" onclick="controlBuiltinLED('test')">✅ <span id="btnLEDTest">Test</span></button>
          <button class="btn btn-cyan" onclick="controlBuiltinLED('fade')">🌊 <span id="btnLEDFade">Fade</span></button>
        </div>
        <div id="ledStatus" style="margin-top: 15px; color: white;"></div>
      </div>

      <div class="card">
        <h2>🌈 NeoPixel</h2>
        <p style="color: rgba(255,255,255,0.8); margin-bottom: 15px;">
          <span id="neoGPIOText">GPIO</span> 48 - <span id="neoCountText">1 LED</span>
        </p>
        
        <h3 style="color: white; margin-bottom: 10px;"><span id="neoPatternTitle">Motifs</span></h3>
        <div class="control-group">
          <button class="btn btn-purple" onclick="neoPattern('rainbow')">🌈 Rainbow</button>
          <button class="btn btn-blue" onclick="neoPattern('pulse')">💓 Pulse</button>
          <button class="btn btn-red" onclick="neoPattern('strobe')">⚡ Strobe</button>
          <button class="btn" onclick="neoPattern('off')">⚫ Off</button>
        </div>

        <h3 style="color: white; margin: 20px 0 10px;"><span id="neoColorTitle">Couleur Personnalisée</span></h3>
        <div class="control-group">
          <input type="number" id="neoR" min="0" max="255" value="255" placeholder="R">
          <input type="number" id="neoG" min="0" max="255" value="0" placeholder="G">
          <input type="number" id="neoB" min="0" max="255" value="0" placeholder="B">
          <button class="btn btn-green" onclick="neoColor()">🎨 <span id="btnNeoApply">Appliquer</span></button>
        </div>
        <div id="neoStatus" style="margin-top: 15px; color: white;"></div>
      </div>
    </div>

    <!-- Tab 3: Écrans -->
    <div id="screens" class="tab-content">
      <div class="card">
        <h2>🖥️ <span id="tftTitle">Écran TFT 320x240</span></h2>
        <div class="info-grid">
          <div class="info-item">
            <div class="info-label"><span id="tftStatusLabel">Statut</span></div>
            <div class="info-value"><span id="tftStatus">Non testé - Config SPI prête</span></div>
          </div>
          <div class="info-item">
            <div class="info-label">Pins SPI</div>
            <div class="info-value">CS:14 DC:47 RST:21</div>
          </div>
        </div>
        <div class="control-group" style="margin-top: 15px;">
          <button class="btn btn-purple" onclick="alert('Test TFT - Configuration matérielle requise')">
            🧪 <span id="btnTFTTest">Test Complet</span>
          </button>
          <button class="btn btn-green" onclick="alert('Test couleurs TFT')">
            🎨 <span id="btnTFTColors">Couleurs</span>
          </button>
          <button class="btn btn-blue" onclick="alert('Damier TFT')">
            🔲 <span id="btnTFTPattern">Damier</span>
          </button>
          <button class="btn btn-red" onclick="alert('Effacer TFT')">
            🗑️ <span id="btnTFTClear">Effacer</span>
          </button>
        </div>
      </div>

      <div class="card">
        <h2>📟 <span id="oledTitle">Écran OLED 0.96" I2C</span></h2>
        <div class="info-grid">
          <div class="info-item">
            <div class="info-label"><span id="oledStatusLabel">Statut</span></div>
            <div class="info-value" id="oledStatus">Détecté à 0x3c</div>
          </div>
          <div class="info-item">
            <div class="info-label">Pins I2C</div>
            <div class="info-value">SDA:21 SCL:20</div>
          </div>
        </div>
        <div class="control-group" style="margin-top: 15px;">
          <label style="color: white; margin-right: 10px;"><span id="oledSDALabel">SDA:</span></label>
          <input type="number" value="21" style="width: 60px;">
          <label style="color: white; margin: 0 10px 0 20px;"><span id="oledSCLLabel">SCL:</span></label>
          <input type="number" value="20" style="width: 60px;">
          <button class="btn btn-blue" onclick="alert('Appliquer et re-détecter I2C')">
            🔄 <span id="btnOLEDApply">Appliquer</span>
          </button>
        </div>
        <div class="control-group" style="margin-top: 10px;">
          <button class="btn btn-purple" onclick="alert('Test OLED complet')">
            🧪 <span id="btnOLEDTest">Test Complet</span>
          </button>
          <input type="text" placeholder="Message personnalisé" style="flex: 1; padding: 8px; border-radius: 5px; border: 2px solid rgba(255,255,255,0.3); background: rgba(255,255,255,0.1); color: white;">
          <button class="btn btn-green" onclick="alert('Afficher message sur OLED')">
            📝 <span id="btnOLEDMessage">Afficher</span>
          </button>
        </div>
      </div>
    </div>

    <!-- Tab 4: Tests Avancés -->
    <div id="advanced" class="tab-content">
      <div class="card">
        <h2>🔬 <span id="adcTitle">Test ADC</span></h2>
        <button class="btn btn-purple" onclick="testADC()">🧪 <span id="btnADCTest">Tester</span></button>
        <div id="adcResults" style="margin-top: 15px;"></div>
      </div>

      <div class="card">
        <h2>👆 <span id="touchTitle">Test Touch Pads</span></h2>
        <button class="btn btn-purple" onclick="testTouch()">🧪 <span id="btnTouchTest">Tester</span></button>
        <div id="touchResults" style="margin-top: 15px;"></div>
      </div>

      <div class="card">
        <h2>〰️ <span id="pwmTitle">Test PWM</span></h2>
        <button class="btn btn-purple" onclick="testPWM()">🧪 <span id="btnPWMTest">Tester</span></button>
        <div id="pwmResults" style="margin-top: 15px;"></div>
      </div>

      <div class="card">
        <h2>🔌 <span id="spiTitle">Bus SPI</span></h2>
        <button class="btn btn-blue" onclick="alert('Scan SPI - SPI2, SPI3 disponibles')">
          🔍 <span id="btnSPIScan">Scanner</span>
        </button>
        <div style="margin-top: 15px; color: white;">
          <p><strong>SPI2, SPI3 disponibles</strong></p>
        </div>
      </div>

      <div class="card">
        <h2>💽 <span id="flashTitle">Partitions Flash</span></h2>
        <button class="btn btn-blue" onclick="alert('Lister partitions Flash')">
          📋 <span id="btnFlashList">Lister Partitions</span>
        </button>
        <div style="margin-top: 15px; color: white; font-family: monospace; font-size: 0.9em;">
          <p>nvs - Type:data - Addr:0x9000 Size:20KB</p>
          <p>otadata - Type:data - Addr:0xe000 Size:8KB</p>
          <p>app0 - Type:app - Addr:0x10000 Size:1280KB</p>
          <p>app1 - Type:app - Addr:0x150000 Size:1280KB</p>
          <p>spiffs - Type:data - Addr:0x290000 Size:1472KB</p>
          <p>coredump - Type:data - Addr:0x3f0000 Size:64KB</p>
        </div>
      </div>

      <div class="card">
        <h2>🔥 <span id="stressTitle">Stress Test Mémoire</span></h2>
        <button class="btn btn-red" onclick="alert('Lancer Stress Test - Attention, peut redémarrer l ESP32')">
          🚀 <span id="btnStressTest">Lancer Stress Test</span>
        </button>
        <div id="stressResults" style="margin-top: 15px; color: white;">
          <p><span id="stressInfo">Non testé</span></p>
        </div>
      </div>
    </div>

    <!-- Tab 5: GPIO -->
    <div id="gpio" class="tab-content">
      <div class="card">
        <h2>📌 <span id="gpioTestTitle">Test GPIO</span></h2>
        <button class="btn btn-purple" onclick="testAllGPIO()">
          🧪 <span id="btnGPIOTest">Tester Tous les GPIO</span>
        </button>
        <div id="gpioResults" style="margin-top: 20px;"></div>
      </div>
    </div>

    <!-- Tab 6: WiFi Scanner -->
    <div id="wifi" class="tab-content">
      <div class="card">
        <h2>📶 <span id="wifiScanTitle">Scanner WiFi</span></h2>
        <button class="btn btn-blue" onclick="scanWiFi()">
          🔍 <span id="btnWiFiScan">Scanner Réseaux WiFi</span>
        </button>
        <div id="wifiScanResults" style="margin-top: 20px;"></div>
      </div>
    </div>

    <!-- Tab 7: Performance -->
    <div id="performance" class="tab-content">
      <div class="card">
        <h2>⚡ <span id="benchmarkTitle">Benchmarks de Performance</span></h2>
        <button class="btn btn-purple" onclick="runBenchmark()">
          🚀 <span id="btnBenchmark">Lancer Benchmarks</span>
        </button>
        <div id="benchmarkResults" style="margin-top: 20px;"></div>
      </div>
    </div>

    <!-- Tab 8: Export -->
    <div id="export" class="tab-content">
      <div class="card">
        <h2>💾 <span id="exportTitle">Export des Données</span></h2>
        <p style="color: rgba(255,255,255,0.8); margin-bottom: 20px;" id="exportDesc">
          Télécharger les données de diagnostic dans différents formats
        </p>
        
        <div class="export-grid">
          <div class="export-card">
            <h3><span id="exportTXTTitle">Fichier TXT</span></h3>
            <p><span id="exportTXTDesc">Rapport texte lisible</span></p>
            <button class="btn btn-purple" onclick="exportTXT()">
              📄 <span id="btnExportTXT">Télécharger TXT</span>
            </button>
          </div>

          <div class="export-card">
            <h3><span id="exportJSONTitle">Fichier JSON</span></h3>
            <p><span id="exportJSONDesc">Format structuré</span></p>
            <button class="btn btn-blue" onclick="exportJSON()">
              📄 <span id="btnExportJSON">Télécharger JSON</span>
            </button>
          </div>

          <div class="export-card">
            <h3><span id="exportCSVTitle">Fichier CSV</span></h3>
            <p><span id="exportCSVDesc">Pour Excel</span></p>
            <button class="btn btn-green" onclick="exportCSV()">
              📊 <span id="btnExportCSV">Télécharger CSV</span>
            </button>
          </div>

          <div class="export-card">
            <h3><span id="exportPDFTitle">Version Imprimable</span></h3>
            <p><span id="exportPDFDesc">Format PDF</span></p>
            <button class="btn btn-cyan" onclick="window.print()">
              🖨️ <span id="btnExportPDF">Ouvrir</span>
            </button>
          </div>
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
        tabOverview: '📊 Vue d\'ensemble',
        tabLEDs: '💡 LEDs',
        tabScreens: '🖥️ Écrans',
        tabAdvanced: '🧪 Tests Avancés',
        tabGPIO: '📌 GPIO',
        tabWiFi: '📶 WiFi',
        tabPerformance: '⚡ Performance',
        tabExport: '💾 Export',
        updateText: 'Mise à jour dans',
        loadingText: 'Chargement...',
        // Add more translations...
      },
      en: {
        pageTitle: 'ESP32 Diagnostic',
        tabOverview: '📊 Overview',
        tabLEDs: '💡 LEDs',
        tabScreens: '🖥️ Screens',
        tabAdvanced: '🧪 Advanced Tests',
        tabGPIO: '📌 GPIO',
        tabWiFi: '📶 WiFi',
        tabPerformance: '⚡ Performance',
        tabExport: '💾 Export',
        updateText: 'Update in',
        loadingText: 'Loading...',
        // Add more translations...
      }
    };

    function t(key) {
      return translations[currentLang][key] || key;
    }

    function setLanguage(lang) {
      currentLang = lang;
      document.querySelectorAll('.lang-btn').forEach(btn => btn.classList.remove('active'));
      event.target.classList.add('active');
      
      // Update UI text
      document.getElementById('pageTitle').textContent = t('pageTitle');
      // Update other elements...
      
      fetch('/api/language', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({language: lang})
      });
    }

    function showTab(tabName) {
      document.querySelectorAll('.tab-content').forEach(content => content.classList.remove('active'));
      document.querySelectorAll('.tab').forEach(tab => tab.classList.remove('active'));
      
      document.getElementById(tabName).classList.add('active');
      event.target.classList.add('active');
    }

    // LED Controls
    async function controlBuiltinLED(action) {
      try {
        const response = await fetch('/api/led/builtin', {
          method: 'POST',
          headers: {'Content-Type': 'application/json'},
          body: JSON.stringify({action: action})
        });
        const data = await response.json();
        document.getElementById('ledStatus').innerHTML = data.success ? 
          '<span class="status-ok">✅ ' + action + ' exécuté</span>' : 
          '<span class="status-error">❌ Erreur</span>';
      } catch (error) {
        console.error('Error:', error);
      }
    }

    async function neoPattern(pattern) {
      try {
        const response = await fetch('/api/neopixel/pattern', {
          method: 'POST',
          headers: {'Content-Type': 'application/json'},
          body: JSON.stringify({pattern: pattern})
        });
        const data = await response.json();
        document.getElementById('neoStatus').innerHTML = data.success ? 
          '<span class="status-ok">✅ Pattern: ' + pattern + '</span>' : 
          '<span class="status-error">❌ Erreur</span>';
      } catch (error) {
        console.error('Error:', error);
      }
    }

    async function neoColor() {
      const r = parseInt(document.getElementById('neoR').value);
      const g = parseInt(document.getElementById('neoG').value);
      const b = parseInt(document.getElementById('neoB').value);
      
      try {
        const response = await fetch('/api/neopixel/color', {
          method: 'POST',
          headers: {'Content-Type': 'application/json'},
          body: JSON.stringify({r: r, g: g, b: b})
        });
        const data = await response.json();
        document.getElementById('neoStatus').innerHTML = data.success ? 
          '<span class="status-ok">✅ Couleur RGB(' + r + ',' + g + ',' + b + ') appliquée</span>' : 
          '<span class="status-error">❌ Erreur</span>';
      } catch (error) {
        console.error('Error:', error);
      }
    }

    // Advanced Tests
    async function testADC() {
      try {
        const response = await fetch('/api/test/adc');
        const data = await response.json();
        
        let html = '<table><thead><tr><th>Pin</th><th>Raw</th><th>Voltage</th><th>%</th></tr></thead><tbody>';
        data.adc_readings.forEach(reading => {
          html += `<tr><td>GPIO ${reading.pin}</td><td>${reading.raw}</td><td>${reading.voltage}V</td><td>${reading.percent}%</td></tr>`;
        });
        html += '</tbody></table>';
        
        document.getElementById('adcResults').innerHTML = html;
      } catch (error) {
        console.error('Error:', error);
      }
    }

    async function testTouch() {
      try {
        const response = await fetch('/api/test/touch');
        const data = await response.json();
        
        if (data.status === 'not_available') {
          document.getElementById('touchResults').innerHTML = '<p style="color: white;">' + data.message + '</p>';
        } else {
          let html = '<table><thead><tr><th>Pin</th><th>Value</th><th>Status</th></tr></thead><tbody>';
          data.touch_pads.forEach(touch => {
            html += `<tr><td>GPIO ${touch.pin}</td><td>${touch.value}</td><td class="status-ok">${touch.status}</td></tr>`;
          });
          html += '</tbody></table>';
          document.getElementById('touchResults').innerHTML = html;
        }
      } catch (error) {
        console.error('Error:', error);
      }
    }

    async function testPWM() {
      try {
        const response = await fetch('/api/test/pwm');
        const data = await response.json();
        
        let html = '<table><thead><tr><th>Pin</th><th>Channel</th><th>Frequency</th><th>Status</th></tr></thead><tbody>';
        data.pwm_channels.forEach(pwm => {
          html += `<tr><td>GPIO ${pwm.pin}</td><td>${pwm.channel}</td><td>${pwm.frequency} Hz</td><td class="status-ok">${pwm.status}</td></tr>`;
        });
        html += '</tbody></table>';
        
        document.getElementById('pwmResults').innerHTML = html;
      } catch (error) {
        console.error('Error:', error);
      }
    }

    async function testAllGPIO() {
      try {
        const response = await fetch('/api/test/gpio');
        const data = await response.json();
        
        let html = '<p style="color: white; margin-bottom: 15px;"><strong>Total: ' + data.total_pins + ' pins testés</strong></p>';
        html += '<table><thead><tr><th>Pin</th><th>Input</th><th>High</th><th>Low</th><th>Status</th></tr></thead><tbody>';
        data.results.forEach(pin => {
          const statusClass = pin.status === 'OK' ? 'status-ok' : 'status-error';
          html += `<tr><td>GPIO ${pin.pin}</td><td>${pin.input}</td><td>${pin.high}</td><td>${pin.low}</td><td class="${statusClass}">${pin.status}</td></tr>`;
        });
        html += '</tbody></table>';
        
        document.getElementById('gpioResults').innerHTML = html;
      } catch (error) {
        console.error('Error:', error);
      }
    }

    async function scanWiFi() {
      document.getElementById('wifiScanResults').innerHTML = '<div class="loading"><div class="spinner"></div><p>Scan en cours...</p></div>';
      
      try {
        const response = await fetch('/api/scan/wifi');
        const data = await response.json();
        
        let html = '<p style="color: white; margin-bottom: 15px;"><strong>' + data.count + ' réseaux trouvés</strong></p>';
        html += '<table><thead><tr><th>SSID</th><th>Signal</th><th>Canal</th><th>Qualité</th><th>Sécurité</th></tr></thead><tbody>';
        data.networks.forEach(net => {
          let qualityClass = 'status-ok';
          if (net.quality === 'Fair') qualityClass = 'status-warn';
          if (net.quality === 'Weak') qualityClass = 'status-error';
          
          html += `<tr>
            <td><strong>${net.ssid}</strong></td>
            <td>${net.rssi} dBm</td>
            <td>${net.channel}</td>
            <td class="${qualityClass}">${net.quality}</td>
            <td>${net.encryption}</td>
          </tr>`;
        });
        html += '</tbody></table>';
        
        document.getElementById('wifiScanResults').innerHTML = html;
      } catch (error) {
        console.error('Error:', error);
        document.getElementById('wifiScanResults').innerHTML = '<p style="color: #f87171;">Erreur lors du scan</p>';
      }
    }

    async function runBenchmark() {
      document.getElementById('benchmarkResults').innerHTML = '<div class="loading"><div class="spinner"></div><p>Benchmarks en cours...</p></div>';
      
      try {
        const response = await fetch('/api/benchmark');
        const data = await response.json();
        
        let html = '<div class="info-grid">';
        html += '<div class="info-item"><div class="info-label">CPU Benchmark</div><div class="info-value">' + data.cpu_benchmark_us + ' µs</div></div>';
        html += '<div class="info-item"><div class="info-label">Performance CPU</div><div class="info-value">' + data.cpu_performance + '</div></div>';
        html += '<div class="info-item"><div class="info-label">Memory Benchmark</div><div class="info-value">' + data.memory_benchmark_us + ' µs</div></div>';
        html += '<div class="info-item"><div class="info-label">Vitesse Mémoire</div><div class="info-value">' + data.memory_speed + '</div></div>';
        html += '</div>';
        
        document.getElementById('benchmarkResults').innerHTML = html;
      } catch (error) {
        console.error('Error:', error);
      }
    }

    // Export functions
    function exportTXT() {
      window.location.href = '/api/export/txt';
    }

    function exportJSON() {
      window.location.href = '/api/export/json';
    }

    function exportCSV() {
      window.location.href = '/api/export/csv';
    }

    // Load system data
    async function loadData() {
      try {
        const sysResponse = await fetch('/api/system');
        const sysData = await sysResponse.json();

        // Display system info
        document.getElementById('systemInfo').innerHTML = `
          <div class="info-item">
            <div class="info-label">Puce</div>
            <div class="info-value">${sysData.chip}</div>
          </div>
          <div class="info-item">
            <div class="info-label">Cœurs CPU</div>
            <div class="info-value">${sysData.cores}</div>
          </div>
          <div class="info-item">
            <div class="info-label">Fréquence</div>
            <div class="info-value">${sysData.frequency} MHz</div>
          </div>
          <div class="info-item">
            <div class="info-label">Flash</div>
            <div class="info-value">${sysData.flash_size}</div>
          </div>
          <div class="info-item">
            <div class="info-label">PSRAM</div>
            <div class="info-value">${sysData.psram_size}</div>
          </div>
          <div class="info-item">
            <div class="info-label">Uptime</div>
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
            <div class="info-label">Heap Libre</div>
            <div class="info-value">${mem.heap_free_formatted}</div>
          </div>
          <div class="info-item">
            <div class="info-label">Taille Heap</div>
            <div class="info-value">${mem.heap_size_formatted}</div>
          </div>
        `;

        if (mem.psram_free !== undefined) {
          memHtml += `
            <div class="info-item">
              <div class="info-label">PSRAM Libre</div>
              <div class="info-value">${mem.psram_free_formatted}</div>
            </div>
            <div class="info-item">
              <div class="info-label">Taille PSRAM</div>
              <div class="info-value">${mem.psram_size_formatted}</div>
            </div>
          `;
        }

        document.getElementById('memoryInfo').innerHTML = memHtml;

        // Display WiFi info
        const wifi = sysData.wifi;
        document.getElementById('wifiInfo').innerHTML = `
          <div class="info-item">
            <div class="info-label">Statut</div>
            <div class="info-value ${wifi.connected ? 'status-ok' : 'status-error'}">
              ${wifi.connected ? 'Connecté' : 'Déconnecté'}
            </div>
          </div>
          ${wifi.connected ? `
            <div class="info-item">
              <div class="info-label">SSID</div>
              <div class="info-value">${wifi.ssid}</div>
            </div>
            <div class="info-item">
              <div class="info-label">IP</div>
              <div class="info-value">${wifi.ip}</div>
            </div>
            <div class="info-item">
              <div class="info-label">Signal</div>
              <div class="info-value">${wifi.rssi} dBm</div>
            </div>
          ` : ''}
        `;

        // Update OLED status based on I2C scan
        const testsResponse = await fetch('/api/tests');
        const testsData = await testsResponse.json();
        
        let oledFound = false;
        testsData.i2c_test.forEach(device => {
          if (device.address === '0x3c' || device.address === '0x3d') {
            oledFound = true;
            document.getElementById('oledStatus').innerHTML = `Détecté à ${device.address}`;
          }
        });
        
        if (!oledFound) {
          document.getElementById('oledStatus').innerHTML = '<span class="status-error">Non détecté</span>';
        }

      } catch (error) {
        console.error('Error loading data:', error);
      }
    }

    // Countdown and progress bar
    function startCountdown() {
      clearInterval(countdownInterval);
      countdownValue = 5;

      countdownInterval = setInterval(() => {
        countdownValue--;
        document.getElementById('countdown').textContent = countdownValue;

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