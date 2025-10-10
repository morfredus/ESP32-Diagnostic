/*
 * WEB_INTERFACE.H - Interface Web Dynamique v3.0-dev
 * Interface moderne avec mise à jour temps réel
 */

#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#include <WebServer.h>

// Déclarations externes (définies dans le fichier principal)
extern const char* DIAGNOSTIC_VERSION_STR;
extern const char* MDNS_HOSTNAME_STR;
extern WebServer server;
extern DiagnosticInfo diagnosticData;
extern Language currentLanguage;

// Déclarations forward des fonctions (pour éviter les erreurs d'ordre)
String generateHTML();
String generateJavaScript();

// Implémentation de handleRoot()
void handleRoot() {
  server.send(200, "text/html; charset=utf-8", generateHTML());
}

// Implémentation de handleJavaScript()
void handleJavaScript() {
  server.send(200, "application/javascript; charset=utf-8", generateJavaScript());
}

// Génère le HTML principal
String generateHTML() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang='fr'>
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <title>ESP32 Diagnostic v)rawliteral";

  html += DIAGNOSTIC_VERSION_STR;

  html += R"rawliteral(</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }

    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      min-height: 100vh;
      padding: 20px;
    }

    .container {
      max-width: 1400px;
      margin: 0 auto;
      background: #fff;
      border-radius: 20px;
      box-shadow: 0 20px 60px rgba(0,0,0,.3);
      overflow: hidden;
    }

    .header {
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      color: #fff;
      padding: 30px;
      text-align: center;
      position: relative;
    }

    .header h1 {
      font-size: 2.5em;
      margin-bottom: 10px;
      animation: fadeIn 0.5s;
    }

    .lang-switcher {
      position: absolute;
      top: 20px;
      right: 20px;
      display: flex;
      gap: 5px;
    }

    .lang-btn {
      padding: 8px 15px;
      background: rgba(255,255,255,.2);
      border: 2px solid rgba(255,255,255,.3);
      border-radius: 5px;
      color: #fff;
      cursor: pointer;
      font-weight: bold;
      transition: all .3s;
    }

    .lang-btn:hover { background: rgba(255,255,255,.3); }
    .lang-btn.active { background: rgba(255,255,255,.4); border-color: rgba(255,255,255,.6); }

    .status-indicator {
      display: inline-block;
      width: 12px;
      height: 12px;
      border-radius: 50%;
      margin-right: 8px;
      animation: pulse 2s infinite;
    }

    .status-online { background: #00ff00; box-shadow: 0 0 10px #00ff00; }
    .status-offline { background: #ff0000; box-shadow: 0 0 10px #ff0000; }

    @keyframes pulse {
      0%, 100% { opacity: 1; }
      50% { opacity: 0.5; }
    }

    @keyframes fadeIn {
      from { opacity: 0; transform: translateY(-20px); }
      to { opacity: 1; transform: translateY(0); }
    }

    .nav {
      display: flex;
      justify-content: center;
      gap: 10px;
      margin-top: 20px;
      flex-wrap: wrap;
    }

    .nav-btn {
      padding: 10px 20px;
      background: rgba(255,255,255,.2);
      border: none;
      border-radius: 5px;
      color: #fff;
      cursor: pointer;
      font-weight: bold;
      transition: all .3s;
    }

    .nav-btn:hover { background: rgba(255,255,255,.3); transform: translateY(-2px); }
    .nav-btn.active { background: rgba(255,255,255,.4); }

    .content {
      padding: 30px;
      animation: fadeIn 0.5s;
    }

    .tab-content {
      display: none;
      animation: fadeIn 0.3s;
    }

    .tab-content.active { display: block; }

    .section {
      background: #f8f9fa;
      border-radius: 15px;
      padding: 25px;
      margin-bottom: 20px;
      border-left: 5px solid #667eea;
      transition: transform .3s;
    }

    .section:hover { transform: translateX(5px); }

    .section h2 {
      color: #667eea;
      margin-bottom: 20px;
      font-size: 1.5em;
      display: flex;
      align-items: center;
      gap: 10px;
    }

    .section h3 {
      color: #667eea;
      margin: 15px 0 10px;
      font-size: 1.2em;
    }

    .info-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
      gap: 15px;
    }

    .info-item {
      background: #fff;
      padding: 15px;
      border-radius: 10px;
      border: 1px solid #e0e0e0;
      transition: all .3s;
    }

    .info-item:hover {
      transform: translateY(-2px);
      box-shadow: 0 5px 15px rgba(0,0,0,.1);
    }

    .info-label {
      font-weight: bold;
      color: #667eea;
      margin-bottom: 5px;
      font-size: .9em;
      text-transform: uppercase;
      letter-spacing: 0.5px;
    }

    .info-value {
      font-size: 1.1em;
      color: #333;
      font-weight: 500;
    }

    .badge {
      display: inline-block;
      padding: 5px 15px;
      border-radius: 20px;
      font-size: .9em;
      font-weight: bold;
      animation: fadeIn 0.5s;
    }

    .badge-success { background: #d4edda; color: #155724; }
    .badge-warning { background: #fff3cd; color: #856404; }
    .badge-danger { background: #f8d7da; color: #721c24; }
    .badge-info { background: #d1ecf1; color: #0c5460; }

    .btn {
      padding: 12px 24px;
      border: none;
      border-radius: 8px;
      font-size: 1em;
      font-weight: bold;
      cursor: pointer;
      margin: 5px;
      transition: all .3s;
      text-decoration: none;
      display: inline-block;
    }

    .btn:hover {
      opacity: .9;
      transform: translateY(-2px);
      box-shadow: 0 5px 15px rgba(0,0,0,.2);
    }

    .btn-primary { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: #fff; }
    .btn-success { background: linear-gradient(135deg, #56ab2f 0%, #a8e063 100%); color: #fff; }
    .btn-info { background: linear-gradient(135deg, #3a7bd5 0%, #00d2ff 100%); color: #fff; }
    .btn-danger { background: linear-gradient(135deg, #eb3349 0%, #f45c43 100%); color: #fff; }
    .btn-warning { background: linear-gradient(135deg, #f2994a 0%, #f2c94c 100%); color: #fff; }

    .progress-bar {
      background: #e0e0e0;
      border-radius: 10px;
      height: 25px;
      overflow: hidden;
      margin-top: 10px;
      position: relative;
    }

    .progress-fill {
      height: 100%;
      border-radius: 10px;
      transition: width .5s ease;
      background: linear-gradient(90deg, #667eea 0%, #764ba2 100%);
      display: flex;
      align-items: center;
      justify-content: center;
      color: #fff;
      font-weight: bold;
      font-size: 0.85em;
    }

    .card {
      background: #fff;
      border-radius: 10px;
      padding: 20px;
      margin: 10px 0;
      border: 2px solid #e0e0e0;
      transition: all .3s;
    }

    .card:hover {
      border-color: #667eea;
      box-shadow: 0 5px 15px rgba(102, 126, 234, .2);
    }

    .loading {
      display: inline-block;
      width: 20px;
      height: 20px;
      border: 3px solid #f3f3f3;
      border-top: 3px solid #667eea;
      border-radius: 50%;
      animation: spin 1s linear infinite;
    }

    @keyframes spin {
      0% { transform: rotate(0deg); }
      100% { transform: rotate(360deg); }
    }

    .status-live {
      padding: 15px;
      background: #f0f0f0;
      border-radius: 10px;
      text-align: center;
      font-weight: bold;
      margin: 15px 0;
      border-left: 4px solid #667eea;
    }

    .gpio-grid {
      display: grid;
      grid-template-columns: repeat(auto-fill, minmax(80px, 1fr));
      gap: 10px;
      margin-top: 15px;
    }

    .gpio-item {
      padding: 15px;
      background: #fff;
      border: 2px solid #ddd;
      border-radius: 8px;
      text-align: center;
      font-weight: bold;
      transition: all .3s;
    }

    .gpio-item:hover { transform: scale(1.05); }
    .gpio-ok { border-color: #28a745; background: #d4edda; }
    .gpio-fail { border-color: #dc3545; background: #f8d7da; }

    .wifi-list { max-height: 500px; overflow-y: auto; }

    .wifi-item {
      background: #fff;
      padding: 15px;
      margin: 10px 0;
      border-radius: 10px;
      border-left: 4px solid #667eea;
      transition: all .3s;
    }

    .wifi-item:hover {
      transform: translateX(5px);
      box-shadow: 0 5px 15px rgba(0,0,0,.1);
    }

    input[type='number'],
    input[type='color'],
    input[type='text'] {
      padding: 10px;
      border: 2px solid #ddd;
      border-radius: 5px;
      font-size: 1em;
      transition: border .3s;
    }

    input:focus {
      outline: none;
      border-color: #667eea;
    }

    .update-indicator {
      position: fixed;
      top: 20px;
      right: 20px;
      padding: 10px 20px;
      background: #667eea;
      color: #fff;
      border-radius: 5px;
      font-weight: bold;
      opacity: 0;
      transition: opacity .3s;
      z-index: 1000;
    }

    .update-indicator.show { opacity: 1; }

    @media (max-width: 768px) {
      .header h1 { font-size: 1.8em; }
      .info-grid { grid-template-columns: 1fr; }
      .nav { flex-direction: column; }
      .nav-btn { width: 100%; }
      .lang-switcher { position: static; margin-top: 10px; justify-content: center; }
    }

    @media print {
      .nav, .btn, .lang-switcher { display: none; }
      .container { box-shadow: none; }
    }
  </style>
</head>
<body>
  <div class='update-indicator' id='updateIndicator'>Mise à jour...</div>

  <div class='container'>
    <div class='header'>
      <div class='lang-switcher'>
        <button class='lang-btn active' onclick='changeLang("fr")'>FR</button>
        <button class='lang-btn' onclick='changeLang("en")'>EN</button>
      </div>

      <h1 id='main-title'>
        <span class='status-indicator status-online' id='statusIndicator'></span>
        Diagnostic ESP32 v)rawliteral";

  html += DIAGNOSTIC_VERSION_STR;

  html += R"rawliteral(
      </h1>

      <div style='font-size:1.2em;margin:10px 0' id='chipModel'>)rawliteral";

  html += diagnosticData.chipModel;

  html += R"rawliteral(</div>

      <div style='font-size:.9em;opacity:.9;margin:10px 0'>
        Accès: <a href='http://)rawliteral";

  html += MDNS_HOSTNAME_STR;

  html += R"rawliteral(.local' style='color:#fff;text-decoration:underline'><strong>http://)rawliteral";

  html += MDNS_HOSTNAME_STR;

  html += R"rawliteral(.local</strong></a> ou <strong id='ipAddress'>)rawliteral";

  html += diagnosticData.ipAddress;

  html += R"rawliteral(</strong>
      </div>

      <div class='nav'>
        <button class='nav-btn active' onclick='showTab("overview")'>Vue d'ensemble</button>
        <button class='nav-btn' onclick='showTab("leds")'>LEDs</button>
        <button class='nav-btn' onclick='showTab("screens")'>Écrans</button>
        <button class='nav-btn' onclick='showTab("tests")'>Tests</button>
        <button class='nav-btn' onclick='showTab("gpio")'>GPIO</button>
        <button class='nav-btn' onclick='showTab("wifi")'>WiFi</button>
        <button class='nav-btn' onclick='showTab("benchmark")'>Performance</button>
        <button class='nav-btn' onclick='showTab("export")'>Export</button>
      </div>
    </div>

    <div class='content'>
      <div id='tabContainer'></div>
    </div>
  </div>

  <script src='/js/app.js'></script>
</body>
</html>
)rawliteral";

  return html;
}
// Génère le JavaScript principal
String generateJavaScript() {
  String js = R"rawliteral(
// Configuration
const UPDATE_INTERVAL = 5000;
let currentLang = 'fr';
let updateTimer = null;
let isConnected = true;

console.log('ESP32 Diagnostic v)rawliteral";

  js += DIAGNOSTIC_VERSION_STR;

  js += R"rawliteral( - Initialisation');

// Initialisation au chargement
document.addEventListener('DOMContentLoaded', function() {
  console.log('Interface chargée - Démarrage');
  loadAllData();
  startAutoUpdate();
  showTab('overview');
});

// Démarrage de la mise à jour automatique
function startAutoUpdate() {
  if (updateTimer) clearInterval(updateTimer);
  updateTimer = setInterval(() => {
    if (isConnected) updateLiveData();
  }, UPDATE_INTERVAL);
}

// Chargement initial de toutes les données
async function loadAllData() {
  showUpdateIndicator();
  try {
    await Promise.all([
      updateSystemInfo(),
      updateMemoryInfo(),
      updateWiFiInfo(),
      updatePeripheralsInfo()
    ]);
    isConnected = true;
    updateStatusIndicator(true);
  } catch (error) {
    console.error('Erreur chargement:', error);
    isConnected = false;
    updateStatusIndicator(false);
  }
  hideUpdateIndicator();
}

// Mise à jour des données en temps réel
async function updateLiveData() {
  try {
    const response = await fetch('/api/status');
    const data = await response.json();
    updateRealtimeValues(data);
    isConnected = true;
    updateStatusIndicator(true);
  } catch (error) {
    console.error('Erreur:', error);
    isConnected = false;
    updateStatusIndicator(false);
  }
}

// Mise à jour des informations système
async function updateSystemInfo() {
  const response = await fetch('/api/system-info');
  const data = await response.json();
  document.getElementById('chipModel').textContent = data.chipModel;
  document.getElementById('ipAddress').textContent = data.ipAddress;
}

async function updateMemoryInfo() {
  const response = await fetch('/api/memory');
  const data = await response.json();
}

async function updateWiFiInfo() {
  const response = await fetch('/api/wifi-info');
  const data = await response.json();
}

async function updatePeripheralsInfo() {
  const response = await fetch('/api/peripherals');
  const data = await response.json();
}

// Gestion des onglets
function showTab(tabName) {
  document.querySelectorAll('.tab-content').forEach(tab => {
    tab.classList.remove('active');
  });
  document.querySelectorAll('.nav-btn').forEach(btn => {
    btn.classList.remove('active');
  });
  const tab = document.getElementById(tabName);
  if (tab) {
    tab.classList.add('active');
  } else {
    loadTab(tabName);
  }
  event.target.classList.add('active');
}

// Chargement dynamique des onglets
async function loadTab(tabName) {
  const container = document.getElementById('tabContainer');
  let tab = document.getElementById(tabName);
  if (!tab) {
    tab = document.createElement('div');
    tab.id = tabName;
    tab.className = 'tab-content';
    container.appendChild(tab);
  }

  switch(tabName) {
    case 'overview':
      tab.innerHTML = await generateOverviewContent();
      break;
    case 'leds':
      tab.innerHTML = await generateLedsContent();
      break;
    case 'screens':
      tab.innerHTML = await generateScreensContent();
      break;
    case 'tests':
      tab.innerHTML = await generateTestsContent();
      break;
    case 'gpio':
      tab.innerHTML = await generateGpioContent();
      break;
    case 'wifi':
      tab.innerHTML = await generateWiFiContent();
      break;
    case 'benchmark':
      tab.innerHTML = await generateBenchmarkContent();
      break;
    case 'export':
      tab.innerHTML = await generateExportContent();
      break;
  }
  tab.classList.add('active');
}

// Génération du contenu Overview
async function generateOverviewContent() {
  const response = await fetch('/api/overview');
  const data = await response.json();
  return `
    <div class='section'>
      <h2>🔧 Informations Processeur</h2>
      <div class='info-grid'>
        <div class='info-item'>
          <div class='info-label'>Modèle</div>
          <div class='info-value'>${data.chip.model} Rev${data.chip.revision}</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>CPU</div>
          <div class='info-value'>${data.chip.cores} cœurs @ ${data.chip.freq} MHz</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>MAC</div>
          <div class='info-value'>${data.chip.mac}</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>Uptime</div>
          <div class='info-value' id='uptime'>${formatUptime(data.chip.uptime)}</div>
        </div>
        ${data.chip.temperature !== -999 ? `
        <div class='info-item'>
          <div class='info-label'>Température</div>
          <div class='info-value' id='temperature'>${data.chip.temperature.toFixed(1)} °C</div>
        </div>` : ''}
      </div>
    </div>
    <div class='section'>
      <h2>💾 Mémoire</h2>
      <h3>SRAM (Interne)</h3>
      <div class='info-grid'>
        <div class='info-item'>
          <div class='info-label'>Total</div>
          <div class='info-value' id='sram-total'>${(data.memory.sram.total / 1024).toFixed(2)} KB</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>Libre</div>
          <div class='info-value' id='sram-free'>${(data.memory.sram.free / 1024).toFixed(2)} KB</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>Utilisée</div>
          <div class='info-value' id='sram-used'>${(data.memory.sram.used / 1024).toFixed(2)} KB</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>Fragmentation</div>
          <div class='info-value' id='fragmentation'>${data.memory.fragmentation.toFixed(1)}%</div>
        </div>
      </div>
      <div class='progress-bar'>
        <div class='progress-fill' style='width: ${((data.memory.sram.used / data.memory.sram.total) * 100).toFixed(1)}%' id='sram-progress'>
          ${((data.memory.sram.used / data.memory.sram.total) * 100).toFixed(1)}%
        </div>
      </div>
      ${data.memory.psram.total > 0 ? `
      <h3>PSRAM (Externe)</h3>
      <div class='info-grid'>
        <div class='info-item'>
          <div class='info-label'>Total</div>
          <div class='info-value' id='psram-total'>${(data.memory.psram.total / 1048576).toFixed(2)} MB</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>Libre</div>
          <div class='info-value' id='psram-free'>${(data.memory.psram.free / 1048576).toFixed(2)} MB</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>Utilisée</div>
          <div class='info-value' id='psram-used'>${(data.memory.psram.used / 1048576).toFixed(2)} MB</div>
        </div>
      </div>
      <div class='progress-bar'>
        <div class='progress-fill' style='width: ${((data.memory.psram.used / data.memory.psram.total) * 100).toFixed(1)}%' id='psram-progress'>
          ${((data.memory.psram.used / data.memory.psram.total) * 100).toFixed(1)}%
        </div>
      </div>` : '<p>PSRAM non détectée</p>'}
    </div>
    <div class='section'>
      <h2>📡 WiFi</h2>
      <div class='info-grid'>
        <div class='info-item'>
          <div class='info-label'>SSID</div>
          <div class='info-value'>${data.wifi.ssid}</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>RSSI</div>
          <div class='info-value'>${data.wifi.rssi} dBm</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>Qualité</div>
          <div class='info-value'>${data.wifi.quality}</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>IP</div>
          <div class='info-value'>${data.wifi.ip}</div>
        </div>
      </div>
    </div>
    <div class='section'>
      <h2>🔌 GPIO</h2>
      <div class='info-grid'>
        <div class='info-item'>
          <div class='info-label'>Total</div>
          <div class='info-value'>${data.gpio.total}</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>I2C</div>
          <div class='info-value'>${data.gpio.i2c_count}</div>
        </div>
      </div>
    </div>
  `;
}

// Génération du contenu LEDs
async function generateLedsContent() {
  const response = await fetch('/api/leds-info');
  const data = await response.json();
  return `
    <div class='section'>
      <h2>💡 LED Intégrée</h2>
      <div class='info-grid'>
        <div class='info-item'>
          <div class='info-label'>GPIO</div>
          <div class='info-value'>GPIO ${data.builtin.pin}</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>Statut</div>
          <div class='info-value' id='builtin-led-status'>${data.builtin.status}</div>
        </div>
        <div class='info-item' style='grid-column: 1/-1; text-align: center'>
          <input type='number' id='ledGPIO' value='${data.builtin.pin}' min='0' max='48' style='width:80px'>
          <button class='btn btn-info' onclick='configBuiltinLED()'>⚙️ Config</button>
          <button class='btn btn-primary' onclick='testBuiltinLED()'>🧪 Test</button>
          <button class='btn btn-success' onclick='ledBlink()'>⚡ Blink</button>
          <button class='btn btn-info' onclick='ledFade()'>🌊 Fade</button>
          <button class='btn btn-danger' onclick='ledOff()'>⭕ Off</button>
        </div>
      </div>
    </div>
    <div class='section'>
      <h2>🌈 NeoPixel</h2>
      <div class='info-grid'>
        <div class='info-item'>
          <div class='info-label'>GPIO</div>
          <div class='info-value'>GPIO ${data.neopixel.pin}</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>LEDs</div>
          <div class='info-value'>${data.neopixel.count}</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>Statut</div>
          <div class='info-value' id='neopixel-status'>${data.neopixel.status}</div>
        </div>
        <div class='info-item' style='grid-column: 1/-1; text-align: center'>
          GPIO: <input type='number' id='neoGPIO' value='${data.neopixel.pin}' min='0' max='48' style='width:80px'>
          LEDs: <input type='number' id='neoCount' value='${data.neopixel.count}' min='1' max='100' style='width:80px'>
          <button class='btn btn-info' onclick='configNeoPixel()'>⚙️ Config</button>
          <br><br>
          <button class='btn btn-primary' onclick='testNeoPixel()'>🧪 Test</button>
          <button class='btn btn-primary' onclick='neoPattern("rainbow")'>🌈 Rainbow</button>
          <button class='btn btn-success' onclick='neoPattern("blink")'>⚡ Blink</button>
          <button class='btn btn-info' onclick='neoPattern("fade")'>🌊 Fade</button>
          <br><br>
          <input type='color' id='neoColor' value='#ff0000' style='height:48px;width:100px'>
          <button class='btn btn-primary' onclick='neoCustomColor()'>🎨 Custom</button>
          <button class='btn btn-danger' onclick='neoPattern("off")'>⭕ Off</button>
        </div>
      </div>
    </div>
  `;
}

// Génération du contenu Écrans
async function generateScreensContent() {
  const response = await fetch('/api/screens-info');
  const data = await response.json();
  return `
    <div class='section'>
      <h2>📺 TFT</h2>
      <div class='info-grid'>
        <div class='info-item'>
          <div class='info-label'>Statut</div>
          <div class='info-value' id='tft-status'>${data.tft.status}</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>Résolution</div>
          <div class='info-value'>${data.tft.width}x${data.tft.height}</div>
        </div>
        <div class='info-item' style='grid-column: 1/-1; text-align: center'>
          <button class='btn btn-primary' onclick='testTFT()'>🧪 Test (15s)</button>
          <button class='btn btn-success' onclick='tftPattern("colors")'>🎨 Couleurs</button>
          <button class='btn btn-info' onclick='tftPattern("checkerboard")'>⬛ Damier</button>
          <button class='btn btn-danger' onclick='tftPattern("clear")'>🗑️ Clear</button>
        </div>
      </div>
    </div>
    <div class='section'>
      <h2>🖥️ OLED</h2>
      <div class='info-grid'>
        <div class='info-item'>
          <div class='info-label'>Statut</div>
          <div class='info-value' id='oled-status'>${data.oled.status}</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>I2C</div>
          <div class='info-value'>SDA:${data.oled.pins.sda} SCL:${data.oled.pins.scl}</div>
        </div>
        <div class='info-item' style='grid-column: 1/-1; text-align: center'>
          SDA: <input type='number' id='oledSDA' value='${data.oled.pins.sda}' min='0' max='48' style='width:70px'>
          SCL: <input type='number' id='oledSCL' value='${data.oled.pins.scl}' min='0' max='48' style='width:70px'>
          <button class='btn btn-info' onclick='configOLED()'>⚙️ Config</button>
          ${data.oled.available ? `
          <br><br>
          <button class='btn btn-primary' onclick='testOLED()'>🧪 Test (25s)</button>
          <br><br>
          <input type='text' id='oledMsg' placeholder='Message' style='width:300px'>
          <button class='btn btn-success' onclick='oledMessage()'>📤 Afficher</button>
          ` : ''}
        </div>
      </div>
    </div>
  `;
}

// Génération du contenu Tests
async function generateTestsContent() {
  return `
    <div class='section'>
      <h2>📊 ADC</h2>
      <div style='text-align:center;margin:20px 0'>
        <button class='btn btn-primary' onclick='testADC()'>🧪 Test ADC</button>
        <div id='adc-status' class='status-live'>Cliquez pour tester</div>
      </div>
      <div id='adc-results' class='info-grid'></div>
    </div>
    <div class='section'>
      <h2>👆 Touch Pads</h2>
      <div style='text-align:center;margin:20px 0'>
        <button class='btn btn-primary' onclick='testTouch()'>🧪 Test Touch</button>
        <div id='touch-status' class='status-live'>Cliquez pour tester</div>
      </div>
      <div id='touch-results' class='info-grid'></div>
    </div>
    <div class='section'>
      <h2>🔊 PWM</h2>
      <div style='text-align:center;margin:20px 0'>
        <button class='btn btn-primary' onclick='testPWM()'>🧪 Test PWM</button>
        <div id='pwm-status' class='status-live'>Cliquez pour tester</div>
      </div>
    </div>
    <div class='section'>
      <h2>🔌 SPI</h2>
      <div style='text-align:center;margin:20px 0'>
        <button class='btn btn-primary' onclick='scanSPI()'>🔍 Scanner SPI</button>
        <div id='spi-status' class='status-live'>Cliquez pour scanner</div>
      </div>
    </div>
    <div class='section'>
      <h2>💾 Partitions</h2>
      <div style='text-align:center;margin:20px 0'>
        <button class='btn btn-primary' onclick='listPartitions()'>📋 Lister</button>
      </div>
      <div id='partitions-results' class='card' style='white-space:pre-wrap;font-family:monospace;font-size:0.85em'>Cliquez pour afficher</div>
    </div>
    <div class='section'>
      <h2>🔥 Stress Test</h2>
      <div style='text-align:center;margin:20px 0'>
        <p style='color:#dc3545;font-weight:bold'>⚠️ Peut ralentir l'ESP32</p>
        <button class='btn btn-danger' onclick='stressTest()'>🚀 Démarrer</button>
        <div id='stress-status' class='status-live'>Non testé</div>
      </div>
    </div>
  `;
}

// Génération du contenu GPIO
async function generateGpioContent() {
  return `
    <div class='section'>
      <h2>🔌 Test GPIO</h2>
      <div style='text-align:center;margin:20px 0'>
        <button class='btn btn-primary' onclick='testAllGPIO()'>🧪 Tester tous</button>
        <div id='gpio-status' class='status-live'>Cliquez pour tester</div>
      </div>
      <div id='gpio-results' class='gpio-grid'></div>
    </div>
  `;
}

// Génération du contenu WiFi
async function generateWiFiContent() {
  return `
    <div class='section'>
      <h2>📡 Scanner WiFi</h2>
      <div style='text-align:center;margin:20px 0'>
        <button class='btn btn-primary' onclick='scanWiFi()'>🔍 Scanner</button>
        <div id='wifi-status' class='status-live'>Cliquez pour scanner</div>
      </div>
      <div id='wifi-results' class='wifi-list'></div>
    </div>
    <div class='section'>
      <h2>🔍 Scanner I2C</h2>
      <div style='text-align:center;margin:20px 0'>
        <button class='btn btn-primary' onclick='scanI2C()'>🔍 Scanner I2C</button>
      </div>
    </div>
  `;
}

// Génération du contenu Benchmark
async function generateBenchmarkContent() {
  return `
    <div class='section'>
      <h2>⚡ Benchmarks</h2>
      <div style='text-align:center;margin:20px 0'>
        <button class='btn btn-primary' onclick='runBenchmarks()'>🚀 Lancer</button>
      </div>
      <div id='benchmark-results' class='info-grid'>
        <div class='info-item'>
          <div class='info-label'>CPU</div>
          <div class='info-value' id='cpu-bench'>Non testé</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>Mémoire</div>
          <div class='info-value' id='mem-bench'>Non testé</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>Perf CPU</div>
          <div class='info-value' id='cpu-perf'>-</div>
        </div>
        <div class='info-item'>
          <div class='info-label'>Vitesse Mem</div>
          <div class='info-value' id='mem-speed'>-</div>
        </div>
      </div>
    </div>
  `;
}

// Génération du contenu Export
async function generateExportContent() {
  return `
    <div class='section'>
      <h2>💾 Exporter</h2>
      <div style='display:grid;grid-template-columns:repeat(auto-fit,minmax(250px,1fr));gap:20px;margin-top:20px'>
        <div class='card' style='text-align:center;padding:30px'>
          <h3 style='color:#667eea;margin-bottom:15px'>📄 TXT</h3>
          <p style='font-size:0.9em;color:#666;margin-bottom:15px'>Rapport lisible</p>
          <a href='/export/txt' class='btn btn-primary'>📥 TXT</a>
        </div>
        <div class='card' style='text-align:center;padding:30px'>
          <h3 style='color:#3a7bd5;margin-bottom:15px'>📋 JSON</h3>
          <p style='font-size:0.9em;color:#666;margin-bottom:15px'>Format structuré</p>
          <a href='/export/json' class='btn btn-info'>📥 JSON</a>
        </div>
        <div class='card' style='text-align:center;padding:30px'>
          <h3 style='color:#56ab2f;margin-bottom:15px'>📊 CSV</h3>
          <p style='font-size:0.9em;color:#666;margin-bottom:15px'>Pour Excel</p>
          <a href='/export/csv' class='btn btn-success'>📥 CSV</a>
        </div>
        <div class='card' style='text-align:center;padding:30px'>
          <h3 style='color:#667eea;margin-bottom:15px'>🖨️ Print</h3>
          <p style='font-size:0.9em;color:#666;margin-bottom:15px'>Version PDF</p>
          <a href='/print' target='_blank' class='btn btn-primary'>🖨️ Ouvrir</a>
        </div>
      </div>
    </div>
  `;
}
// Fonctions API - LED Intégrée
async function configBuiltinLED() {
  const gpio = document.getElementById('ledGPIO').value;
  const response = await fetch('/api/builtin-led-config?gpio=' + gpio);
  const data = await response.json();
  document.getElementById('builtin-led-status').textContent = data.message;
  alert(data.message);
}

async function testBuiltinLED() {
  document.getElementById('builtin-led-status').textContent = 'Test en cours...';
  const response = await fetch('/api/builtin-led-test');
  const data = await response.json();
  document.getElementById('builtin-led-status').textContent = data.result;
  alert(data.result);
}

async function ledBlink() {
  const response = await fetch('/api/builtin-led-control?action=blink');
  const data = await response.json();
  document.getElementById('builtin-led-status').textContent = data.message;
}

async function ledFade() {
  const response = await fetch('/api/builtin-led-control?action=fade');
  const data = await response.json();
  document.getElementById('builtin-led-status').textContent = data.message;
}

async function ledOff() {
  const response = await fetch('/api/builtin-led-control?action=off');
  const data = await response.json();
  document.getElementById('builtin-led-status').textContent = data.message;
}

// Fonctions API - NeoPixel
async function configNeoPixel() {
  const gpio = document.getElementById('neoGPIO').value;
  const count = document.getElementById('neoCount').value;
  const response = await fetch('/api/neopixel-config?gpio=' + gpio + '&count=' + count);
  const data = await response.json();
  document.getElementById('neopixel-status').textContent = data.message;
  alert(data.message);
}

async function testNeoPixel() {
  document.getElementById('neopixel-status').textContent = 'Test en cours...';
  const response = await fetch('/api/neopixel-test');
  const data = await response.json();
  document.getElementById('neopixel-status').textContent = data.result;
}

async function neoPattern(pattern) {
  const response = await fetch('/api/neopixel-pattern?pattern=' + pattern);
  const data = await response.json();
  document.getElementById('neopixel-status').textContent = data.message;
}

async function neoCustomColor() {
  const color = document.getElementById('neoColor').value;
  const r = parseInt(color.substr(1, 2), 16);
  const g = parseInt(color.substr(3, 2), 16);
  const b = parseInt(color.substr(5, 2), 16);
  const response = await fetch('/api/neopixel-color?r=' + r + '&g=' + g + '&b=' + b);
  const data = await response.json();
  document.getElementById('neopixel-status').textContent = data.message;
}

// Fonctions API - TFT
async function testTFT() {
  document.getElementById('tft-status').textContent = 'Test en cours (15s)...';
  const response = await fetch('/api/tft-test');
  const data = await response.json();
  document.getElementById('tft-status').textContent = data.result;
}

async function tftPattern(pattern) {
  const response = await fetch('/api/tft-pattern?pattern=' + pattern);
  const data = await response.json();
  document.getElementById('tft-status').textContent = data.message;
}

// Fonctions API - OLED
async function configOLED() {
  document.getElementById('oled-status').textContent = 'Reconfiguration...';
  const sda = document.getElementById('oledSDA').value;
  const scl = document.getElementById('oledSCL').value;
  const response = await fetch('/api/oled-config?sda=' + sda + '&scl=' + scl);
  const data = await response.json();
  if (data.success) {
    alert(data.message);
    location.reload();
  } else {
    alert('Erreur: ' + data.message);
  }
}

async function testOLED() {
  document.getElementById('oled-status').textContent = 'Test en cours (25s)...';
  const response = await fetch('/api/oled-test');
  const data = await response.json();
  document.getElementById('oled-status').textContent = data.result;
}

async function oledMessage() {
  const message = document.getElementById('oledMsg').value;
  const response = await fetch('/api/oled-message?message=' + encodeURIComponent(message));
  const data = await response.json();
  document.getElementById('oled-status').textContent = data.message;
}

// Fonctions API - Tests
async function testADC() {
  document.getElementById('adc-status').textContent = 'Test en cours...';
  const response = await fetch('/api/adc-test');
  const data = await response.json();
  let html = '';
  data.readings.forEach(reading => {
    html += '<div class="info-item">';
    html += '<div class="info-label">GPIO ' + reading.pin + '</div>';
    html += '<div class="info-value">' + reading.raw + ' (' + reading.voltage.toFixed(2) + 'V)</div>';
    html += '</div>';
  });
  document.getElementById('adc-results').innerHTML = html;
  document.getElementById('adc-status').textContent = data.result;
}

async function testTouch() {
  document.getElementById('touch-status').textContent = 'Test en cours...';
  const response = await fetch('/api/touch-test');
  const data = await response.json();
  let html = '';
  data.readings.forEach(reading => {
    html += '<div class="info-item">';
    html += '<div class="info-label">Touch ' + reading.pin + '</div>';
    html += '<div class="info-value">' + reading.value + '</div>';
    html += '</div>';
  });
  document.getElementById('touch-results').innerHTML = html;
  document.getElementById('touch-status').textContent = data.result;
}

async function testPWM() {
  document.getElementById('pwm-status').textContent = 'Test en cours...';
  const response = await fetch('/api/pwm-test');
  const data = await response.json();
  document.getElementById('pwm-status').textContent = data.result;
}

async function scanSPI() {
  document.getElementById('spi-status').textContent = 'Scan en cours...';
  const response = await fetch('/api/spi-scan');
  const data = await response.json();
  document.getElementById('spi-status').textContent = data.info;
}

async function listPartitions() {
  document.getElementById('partitions-results').textContent = 'Scan en cours...';
  const response = await fetch('/api/partitions-list');
  const data = await response.json();
  document.getElementById('partitions-results').textContent = data.partitions;
}

async function stressTest() {
  document.getElementById('stress-status').textContent = 'Test en cours...';
  const response = await fetch('/api/stress-test');
  const data = await response.json();
  document.getElementById('stress-status').textContent = data.result;
}

// Fonctions API - GPIO
async function testAllGPIO() {
  document.getElementById('gpio-status').textContent = 'Test en cours...';
  const response = await fetch('/api/test-gpio');
  const data = await response.json();
  let html = '';
  data.results.forEach(gpio => {
    html += '<div class="gpio-item ' + (gpio.working ? 'gpio-ok' : 'gpio-fail') + '">';
    html += 'GPIO ' + gpio.pin + '<br>';
    html += (gpio.working ? '✅ OK' : '❌ FAIL');
    html += '</div>';
  });
  document.getElementById('gpio-results').innerHTML = html;
  document.getElementById('gpio-status').textContent = 'Terminé - ' + data.results.length + ' GPIO testés';
}

// Fonctions API - WiFi
async function scanWiFi() {
  document.getElementById('wifi-status').textContent = 'Scan en cours...';
  const response = await fetch('/api/wifi-scan');
  const data = await response.json();
  let html = '';
  data.networks.forEach(network => {
    const signalIcon = network.rssi >= -60 ? '🟢' : network.rssi >= -70 ? '🟡' : '🔴';
    const signalColor = network.rssi >= -60 ? '#28a745' : network.rssi >= -70 ? '#ffc107' : '#dc3545';
    html += '<div class="wifi-item">';
    html += '<div style="display:flex;justify-content:space-between;align-items:center">';
    html += '<div>';
    html += '<strong>' + signalIcon + ' ' + network.ssid + '</strong><br>';
    html += '<small>' + network.bssid + ' | Canal ' + network.channel + ' | ' + network.encryption + '</small>';
    html += '</div>';
    html += '<div style="font-size:1.3em;font-weight:bold;color:' + signalColor + '">';
    html += network.rssi + ' dBm';
    html += '</div>';
    html += '</div>';
    html += '</div>';
  });
  document.getElementById('wifi-results').innerHTML = html;
  document.getElementById('wifi-status').textContent = data.networks.length + ' réseaux détectés';
}

async function scanI2C() {
  const response = await fetch('/api/i2c-scan');
  const data = await response.json();
  alert('Périphériques I2C:\\n' + data.count + ' périphérique(s)\\n\\nAdresses: ' + data.devices);
  location.reload();
}

// Fonctions API - Benchmarks
async function runBenchmarks() {
  document.getElementById('cpu-bench').textContent = 'Test en cours...';
  document.getElementById('mem-bench').textContent = 'Test en cours...';
  const response = await fetch('/api/benchmark');
  const data = await response.json();
  document.getElementById('cpu-bench').textContent = data.cpu + ' µs';
  document.getElementById('mem-bench').textContent = data.memory + ' µs';
  document.getElementById('cpu-perf').textContent = data.cpuPerf.toFixed(2) + ' MFLOPS';
  document.getElementById('mem-speed').textContent = data.memSpeed.toFixed(2) + ' MB/s';
}

// Changement de langue
function changeLang(lang) {
  fetch('/api/set-language?lang=' + lang)
    .then(r => r.json())
    .then(data => {
      if (data.success) {
        currentLang = lang;
        document.querySelectorAll('.lang-btn').forEach(btn => btn.classList.remove('active'));
        event.target.classList.add('active');
        location.reload();
      }
    });
}

// Utilitaires
function formatUptime(ms) {
  const seconds = Math.floor(ms / 1000);
  const minutes = Math.floor(seconds / 60);
  const hours = Math.floor(minutes / 60);
  const days = Math.floor(hours / 24);
  return days + 'j ' + (hours % 24) + 'h ' + (minutes % 60) + 'm';
}

function showUpdateIndicator() {
  document.getElementById('updateIndicator').classList.add('show');
}

function hideUpdateIndicator() {
  setTimeout(() => {
    document.getElementById('updateIndicator').classList.remove('show');
  }, 500);
}

function updateStatusIndicator(connected) {
  const indicator = document.getElementById('statusIndicator');
  if (connected) {
    indicator.classList.remove('status-offline');
    indicator.classList.add('status-online');
  } else {
    indicator.classList.remove('status-online');
    indicator.classList.add('status-offline');
  }
}

// Mise à jour des valeurs en temps réel
function updateRealtimeValues(data) {
  const uptimeEl = document.getElementById('uptime');
  if (uptimeEl) uptimeEl.textContent = formatUptime(data.uptime);

  const tempEl = document.getElementById('temperature');
  if (tempEl && data.temperature !== -999) {
    tempEl.textContent = data.temperature.toFixed(1) + ' °C';
  }

  const sramFreeEl = document.getElementById('sram-free');
  if (sramFreeEl) {
    sramFreeEl.textContent = (data.sram.free / 1024).toFixed(2) + ' KB';
  }

  const sramUsedEl = document.getElementById('sram-used');
  if (sramUsedEl) {
    sramUsedEl.textContent = (data.sram.used / 1024).toFixed(2) + ' KB';
  }

  const sramProgressEl = document.getElementById('sram-progress');
  if (sramProgressEl && data.sram.total > 0) {
    const percent = ((data.sram.used / data.sram.total) * 100).toFixed(1);
    sramProgressEl.style.width = percent + '%';
    sramProgressEl.textContent = percent + '%';
  }

  if (data.psram && data.psram.total > 0) {
    const psramFreeEl = document.getElementById('psram-free');
    if (psramFreeEl) {
      psramFreeEl.textContent = (data.psram.free / 1048576).toFixed(2) + ' MB';
    }

    const psramUsedEl = document.getElementById('psram-used');
    if (psramUsedEl) {
      psramUsedEl.textContent = (data.psram.used / 1048576).toFixed(2) + ' MB';
    }

    const psramProgressEl = document.getElementById('psram-progress');
    if (psramProgressEl) {
      const percent = ((data.psram.used / data.psram.total) * 100).toFixed(1);
      psramProgressEl.style.width = percent + '%';
      psramProgressEl.textContent = percent + '%';
    }
  }

  const fragEl = document.getElementById('fragmentation');
  if (fragEl) {
    fragEl.textContent = data.fragmentation.toFixed(1) + '%';
  }
}
)rawliteral";

  return js;
}

#endif // WEB_INTERFACE_H
