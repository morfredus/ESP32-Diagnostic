/*
 * WEB_INTERFACE.H - Interface Web Dynamique v3.0.0
 * Interface complète avec mises à jour temps réel
 */

#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

void handleRoot() {
  String html = "<!DOCTYPE html><html lang='";
  html += (currentLanguage == LANG_FR) ? "fr" : "en";
  html += "'><head>";
  html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1.0'>";
  html += "<title>ESP32 Diagnostic v3.0.0</title>";
  html += "<style>";
  html += "*{margin:0;padding:0;box-sizing:border-box}";
  html += "body{font-family:'Segoe UI',sans-serif;background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);padding:20px}";
  html += ".container{max-width:1400px;margin:0 auto;background:#fff;border-radius:20px;box-shadow:0 20px 60px rgba(0,0,0,.3);overflow:hidden}";
  html += ".header{background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);color:#fff;padding:30px;text-align:center;position:relative}";
  html += ".header h1{font-size:2.5em;margin-bottom:10px;animation:fadeIn 0.5s}";
  html += "@keyframes fadeIn{from{opacity:0;transform:translateY(-20px)}to{opacity:1;transform:translateY(0)}}";
  html += ".lang-switcher{position:absolute;top:20px;right:20px;display:flex;gap:5px}";
  html += ".lang-btn{padding:8px 15px;background:rgba(255,255,255,.2);border:2px solid rgba(255,255,255,.3);border-radius:5px;color:#fff;cursor:pointer;font-weight:bold;transition:all .3s}";
  html += ".lang-btn:hover{background:rgba(255,255,255,.3)}";
  html += ".lang-btn.active{background:rgba(255,255,255,.4);border-color:rgba(255,255,255,.6)}";
  html += ".status-indicator{position:absolute;top:20px;left:20px;width:12px;height:12px;border-radius:50%;animation:pulse 2s infinite}";
  html += ".status-online{background:#0f0;box-shadow:0 0 10px #0f0}";
  html += ".status-offline{background:#f00;box-shadow:0 0 10px #f00}";
  html += "@keyframes pulse{0%,100%{opacity:1}50%{opacity:0.5}}";
  html += ".nav{display:flex;justify-content:center;gap:10px;margin-top:20px;flex-wrap:wrap}";
  html += ".nav-btn{padding:10px 20px;background:rgba(255,255,255,.2);border:none;border-radius:5px;color:#fff;cursor:pointer;font-weight:bold;transition:all .3s}";
  html += ".nav-btn:hover{background:rgba(255,255,255,.3);transform:translateY(-2px)}";
  html += ".nav-btn.active{background:rgba(255,255,255,.4)}";
  html += ".content{padding:30px}";
  html += ".tab-content{display:none;animation:fadeIn 0.3s}";
  html += ".tab-content.active{display:block}";
  html += ".section{background:#f8f9fa;border-radius:15px;padding:25px;margin-bottom:20px;border-left:5px solid #667eea;transition:transform .3s}";
  html += ".section:hover{transform:translateX(5px)}";
  html += ".section h2{color:#667eea;margin-bottom:20px;font-size:1.5em}";
  html += ".info-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(300px,1fr));gap:15px}";
  html += ".info-item{background:#fff;padding:15px;border-radius:10px;border:1px solid #e0e0e0;transition:all .3s}";
  html += ".info-item:hover{transform:translateY(-2px);box-shadow:0 5px 15px rgba(0,0,0,.1)}";
  html += ".info-label{font-weight:bold;color:#667eea;margin-bottom:5px;font-size:.9em}";
  html += ".info-value{font-size:1.1em;color:#333}";
  html += ".badge{display:inline-block;padding:5px 15px;border-radius:20px;font-size:.9em;font-weight:bold}";
  html += ".badge-success{background:#d4edda;color:#155724}";
  html += ".badge-warning{background:#fff3cd;color:#856404}";
  html += ".badge-danger{background:#f8d7da;color:#721c24}";
  html += ".btn{padding:12px 24px;border:none;border-radius:8px;font-size:1em;font-weight:bold;cursor:pointer;margin:5px;transition:all .3s}";
  html += ".btn:hover{opacity:.9;transform:translateY(-2px)}";
  html += ".btn-primary{background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);color:#fff}";
  html += ".btn-success{background:linear-gradient(135deg,#56ab2f 0%,#a8e063 100%);color:#fff}";
  html += ".btn-info{background:linear-gradient(135deg,#3a7bd5 0%,#00d2ff 100%);color:#fff}";
  html += ".btn-danger{background:linear-gradient(135deg,#eb3349 0%,#f45c43 100%);color:#fff}";
  html += ".progress-bar{background:#e0e0e0;border-radius:10px;height:25px;overflow:hidden;margin-top:10px;position:relative}";
  html += ".progress-fill{height:100%;border-radius:10px;transition:width .5s ease;background:linear-gradient(90deg,#667eea 0%,#764ba2 100%);display:flex;align-items:center;justify-content:center;color:#fff;font-weight:bold;font-size:0.85em}";
  html += ".update-badge{position:fixed;top:20px;right:20px;padding:10px 20px;background:#667eea;color:#fff;border-radius:5px;font-weight:bold;opacity:0;transition:opacity .3s;z-index:1000;pointer-events:none}";
  html += ".update-badge.show{opacity:1}";
  html += ".loading{display:inline-block;width:20px;height:20px;border:3px solid #f3f3f3;border-top:3px solid #667eea;border-radius:50%;animation:spin 1s linear infinite}";
  html += "@keyframes spin{0%{transform:rotate(0deg)}100%{transform:rotate(360deg)}}";
  html += "@media(max-width:768px){.header h1{font-size:1.8em}.info-grid{grid-template-columns:1fr}.nav{flex-direction:column}.nav-btn{width:100%}.lang-switcher{position:static;margin-top:10px;justify-content:center}}";
  html += "</style></head><body>";

  // Update Badge
  html += "<div class='update-badge' id='updateBadge'>Mise à jour...</div>";

  // Container
  html += "<div class='container'>";

  // Header
  html += "<div class='header'>";
  html += "<div class='status-indicator status-online' id='statusIndicator'></div>";
  html += "<div class='lang-switcher'>";
  html += "<button class='lang-btn " + String(currentLanguage == LANG_FR ? "active" : "") + "' onclick='changeLang(\"fr\")'>FR</button>";
  html += "<button class='lang-btn " + String(currentLanguage == LANG_EN ? "active" : "") + "' onclick='changeLang(\"en\")'>EN</button>";
  html += "</div>";
  html += "<h1 id='mainTitle'>ESP32 Diagnostic v3.0.0</h1>";
  html += "<div style='font-size:1.2em;margin:10px 0' id='chipModel'>" + diagnosticData.chipModel + "</div>";
  html += "<div style='font-size:.9em;opacity:.9;margin:10px 0'>";
  html += "Accès: <a href='http://" + String(MDNS_HOSTNAME) + ".local' style='color:#fff;text-decoration:underline'>http://" + String(MDNS_HOSTNAME) + ".local</a> ou <strong id='ipAddress'>" + diagnosticData.ipAddress + "</strong>";
  html += "</div>";

  // Navigation
  html += "<div class='nav'>";
  html += "<button class='nav-btn active' onclick='showTab(\"overview\")' data-i18n='nav_overview'>Vue d'ensemble</button>";
  html += "<button class='nav-btn' onclick='showTab(\"leds\")' data-i18n='nav_leds'>LEDs</button>";
  html += "<button class='nav-btn' onclick='showTab(\"screens\")' data-i18n='nav_screens'>Écrans</button>";
  html += "<button class='nav-btn' onclick='showTab(\"tests\")' data-i18n='nav_tests'>Tests</button>";
  html += "<button class='nav-btn' onclick='showTab(\"gpio\")' data-i18n='nav_gpio'>GPIO</button>";
  html += "<button class='nav-btn' onclick='showTab(\"wifi\")' data-i18n='nav_wifi'>WiFi</button>";
  html += "<button class='nav-btn' onclick='showTab(\"benchmark\")' data-i18n='nav_benchmark'>Performance</button>";
  html += "<button class='nav-btn' onclick='showTab(\"export\")' data-i18n='nav_export'>Export</button>";
  html += "</div></div>";

  // Content (tabs seront chargées dynamiquement)
  html += "<div class='content'><div id='tabContainer'></div></div>";
  html += "</div>";

  // JavaScript
  html += "<script>";
  html += "let currentLang='" + String(currentLanguage == LANG_FR ? "fr" : "en") + "';";
  html += "let updateTimer=null;";
  html += "let isConnected=true;";
  html += "const UPDATE_INTERVAL=5000;";

  // Initialisation
  html += "document.addEventListener('DOMContentLoaded',function(){";
  html += "console.log('ESP32 Diagnostic v3.0.0 - Interface dynamique');";
  html += "loadTab('overview');";
  html += "startAutoUpdate();";
  html += "});";

  // Auto-update
  html += "function startAutoUpdate(){";
  html += "if(updateTimer)clearInterval(updateTimer);";
  html += "updateTimer=setInterval(()=>{if(isConnected)updateLiveData();},UPDATE_INTERVAL);";
  html += "}";

  html += "async function updateLiveData(){";
  html += "try{";
  html += "const response=await fetch('/api/status');";
  html += "const data=await response.json();";
  html += "updateRealtimeValues(data);";
  html += "isConnected=true;";
  html += "updateStatusIndicator(true);";
  html += "}catch(error){";
  html += "console.error('Erreur:',error);";
  html += "isConnected=false;";
  html += "updateStatusIndicator(false);";
  html += "}}";

  html += "function updateRealtimeValues(d){";
  html += "const uptime=document.getElementById('uptime');";
  html += "if(uptime)uptime.textContent=formatUptime(d.uptime);";
  html += "const temp=document.getElementById('temperature');";
  html += "if(temp&&d.temperature!==-999)temp.textContent=d.temperature.toFixed(1)+' °C';";
  html += "const sramFree=document.getElementById('sram-free');";
  html += "if(sramFree)sramFree.textContent=(d.sram.free/1024).toFixed(2)+' KB';";
  html += "const sramUsed=document.getElementById('sram-used');";
  html += "if(sramUsed)sramUsed.textContent=(d.sram.used/1024).toFixed(2)+' KB';";
  html += "const sramProgress=document.getElementById('sram-progress');";
  html += "if(sramProgress&&d.sram.total>0){";
  html += "const pct=((d.sram.used/d.sram.total)*100).toFixed(1);";
  html += "sramProgress.style.width=pct+'%';";
  html += "sramProgress.textContent=pct+'%';";
  html += "}";
  html += "if(d.psram){";
  html += "const psramFree=document.getElementById('psram-free');";
  html += "if(psramFree)psramFree.textContent=(d.psram.free/1048576).toFixed(2)+' MB';";
  html += "const psramUsed=document.getElementById('psram-used');";
  html += "if(psramUsed)psramUsed.textContent=(d.psram.used/1048576).toFixed(2)+' MB';";
  html += "const psramProgress=document.getElementById('psram-progress');";
  html += "if(psramProgress&&d.psram.total>0){";
  html += "const pct=((d.psram.used/d.psram.total)*100).toFixed(1);";
  html += "psramProgress.style.width=pct+'%';";
  html += "psramProgress.textContent=pct+'%';";
  html += "}}";
  html += "const frag=document.getElementById('fragmentation');";
  html += "if(frag)frag.textContent=d.fragmentation.toFixed(1)+'%';";
  html += "}";

  html += "function updateStatusIndicator(connected){";
  html += "const indicator=document.getElementById('statusIndicator');";
  html += "if(connected){";
  html += "indicator.classList.remove('status-offline');";
  html += "indicator.classList.add('status-online');";
  html += "}else{";
  html += "indicator.classList.remove('status-online');";
  html += "indicator.classList.add('status-offline');";
  html += "}}";

  html += "function showUpdateBadge(text='Mise à jour...'){";
  html += "const badge=document.getElementById('updateBadge');";
  html += "badge.textContent=text;";
  html += "badge.classList.add('show');";
  html += "setTimeout(()=>badge.classList.remove('show'),1000);";
  html += "}";

  html += "function formatUptime(ms){";
  html += "const s=Math.floor(ms/1000);";
  html += "const m=Math.floor(s/60);";
  html += "const h=Math.floor(m/60);";
  html += "const d=Math.floor(h/24);";
  html += "return d+'j '+(h%24)+'h '+(m%60)+'m';";
  html += "}";

  // Changement de langue INSTANTANÉ
  html += "async function changeLang(lang){";
  html += "showUpdateBadge('Changement de langue...');";
  html += "try{";
  html += "const response=await fetch('/api/set-language?lang='+lang);";
  html += "const data=await response.json();";
  html += "if(data.success){";
  html += "currentLang=lang;";
  html += "document.documentElement.lang=lang;";
  html += "document.querySelectorAll('.lang-btn').forEach(b=>b.classList.remove('active'));";
  html += "event.target.classList.add('active');";
  html += "await updateAllTranslations();";
  html += "showUpdateBadge('Langue changée!');";
  html += "}";
  html += "}catch(e){console.error('Erreur:',e);}";
  html += "}";

  html += "async function updateAllTranslations(){";
  html += "const response=await fetch('/api/get-translations?lang='+currentLang+'&t='+Date.now());";
  html += "const tr=await response.json();";
  html += "document.getElementById('mainTitle').textContent=tr.title+' v3.0.0';";
  html += "document.querySelectorAll('[data-i18n]').forEach(el=>{";
  html += "const key=el.getAttribute('data-i18n');";
  html += "if(tr[key])el.textContent=tr[key];";
  html += "});";
  html += "const currentTab=document.querySelector('.nav-btn.active').onclick.toString().match(/showTab\\(\"(.*?)\"\\)/)[1];";
  html += "loadTab(currentTab);";
  html += "}";

  // Navigation tabs
  html += "function showTab(tabName){";
  html += "document.querySelectorAll('.tab-content').forEach(t=>t.classList.remove('active'));";
  html += "document.querySelectorAll('.nav-btn').forEach(b=>b.classList.remove('active'));";
  html += "event.target.classList.add('active');";
  html += "loadTab(tabName);";
  html += "}";

  html += "async function loadTab(tabName){";
  html += "const container=document.getElementById('tabContainer');";
  html += "let tab=document.getElementById(tabName);";
  html += "if(!tab){";
  html += "tab=document.createElement('div');";
  html += "tab.id=tabName;";
  html += "tab.className='tab-content';";
  html += "container.appendChild(tab);";
  html += "}";
  html += "document.querySelectorAll('.tab-content').forEach(t=>t.classList.remove('active'));";
  html += "tab.classList.add('active');";
  html += "tab.innerHTML='<div class=\"section\"><div class=\"loading\"></div><p style=\"text-align:center\">Chargement...</p></div>';";
  html += "try{";
  html += "if(tabName==='overview'){";
  html += "const r=await fetch('/api/overview');";
  html += "const d=await r.json();";
  html += "tab.innerHTML=buildOverview(d);";
  html += "}else if(tabName==='leds'){";
  html += "const r=await fetch('/api/leds-info');";
  html += "const d=await r.json();";
  html += "tab.innerHTML=buildLeds(d);";
  html += "}else if(tabName==='screens'){";
  html += "const r=await fetch('/api/screens-info');";
  html += "const d=await r.json();";
  html += "tab.innerHTML=buildScreens(d);";
  html += "}else if(tabName==='tests'){";
  html += "tab.innerHTML=buildTests();";
  html += "}else if(tabName==='gpio'){";
  html += "tab.innerHTML=buildGpio();";
  html += "}else if(tabName==='wifi'){";
  html += "tab.innerHTML=buildWifi();";
  html += "}else if(tabName==='benchmark'){";
  html += "tab.innerHTML=buildBenchmark();";
  html += "}else if(tabName==='export'){";
  html += "tab.innerHTML=buildExport();";
  html += "}";
  html += "}catch(e){";
  html += "tab.innerHTML='<div class=\"section\"><h2>⚠️ Erreur</h2><p>'+e+'</p></div>';";
  html += "}}";

  // Build Overview
  html += "function buildOverview(d){";
  html += "let h='<div class=\"section\"><h2>🔧 Informations Processeur</h2><div class=\"info-grid\">';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Modèle complet</div><div class=\"info-value\">'+d.chip.model+' Rev'+d.chip.revision+'</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">CPU</div><div class=\"info-value\">'+d.chip.cores+' cœurs @ '+d.chip.freq+' MHz</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">MAC WiFi</div><div class=\"info-value\">'+d.chip.mac+'</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Uptime</div><div class=\"info-value\" id=\"uptime\">'+formatUptime(d.chip.uptime)+'</div></div>';";
  html += "if(d.chip.temperature!==-999)h+='<div class=\"info-item\"><div class=\"info-label\">Température CPU</div><div class=\"info-value\" id=\"temperature\">'+d.chip.temperature.toFixed(1)+' °C</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Fonctionnalités</div><div class=\"info-value\">'+d.chip.features+'</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">SDK Version</div><div class=\"info-value\">'+d.chip.sdk+'</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">ESP-IDF</div><div class=\"info-value\">'+d.chip.idf+'</div></div>';";
  html += "h+='</div></div>';";

  html += "h+='<div class=\"section\"><h2>💾 Mémoire Détaillée</h2>';";
  html += "h+='<h3>Flash (Carte)</h3><div class=\"info-grid\">';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Taille réelle</div><div class=\"info-value\">'+(d.memory.flash.real/1048576).toFixed(2)+' MB</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Type Flash</div><div class=\"info-value\">'+d.memory.flash.type+'</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Vitesse Flash</div><div class=\"info-value\">'+d.memory.flash.speed+'</div></div>';";
  html += "h+='</div>';";

  html += "h+='<h3>SRAM (Interne)</h3><div class=\"info-grid\">';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Total</div><div class=\"info-value\" id=\"sram-total\">'+(d.memory.sram.total/1024).toFixed(2)+' KB</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Libre</div><div class=\"info-value\" id=\"sram-free\">'+(d.memory.sram.free/1024).toFixed(2)+' KB</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Utilisée</div><div class=\"info-value\" id=\"sram-used\">'+(d.memory.sram.used/1024).toFixed(2)+' KB</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Fragmentation</div><div class=\"info-value\" id=\"fragmentation\">'+d.memory.fragmentation.toFixed(1)+'%</div></div>';";
  html += "h+='</div>';";
  html += "const sramPct=((d.memory.sram.used/d.memory.sram.total)*100).toFixed(1);";
  html += "h+='<div class=\"progress-bar\"><div class=\"progress-fill\" id=\"sram-progress\" style=\"width:'+sramPct+'%\">'+sramPct+'%</div></div>';";

  html += "if(d.memory.psram&&d.memory.psram.total>0){";
  html += "h+='<h3>PSRAM (Externe)</h3><div class=\"info-grid\">';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Total</div><div class=\"info-value\" id=\"psram-total\">'+(d.memory.psram.total/1048576).toFixed(2)+' MB</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Libre</div><div class=\"info-value\" id=\"psram-free\">'+(d.memory.psram.free/1048576).toFixed(2)+' MB</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Utilisée</div><div class=\"info-value\" id=\"psram-used\">'+(d.memory.psram.used/1048576).toFixed(2)+' MB</div></div>';";
  html += "h+='</div>';";
  html += "const psramPct=((d.memory.psram.used/d.memory.psram.total)*100).toFixed(1);";
  html += "h+='<div class=\"progress-bar\"><div class=\"progress-fill\" id=\"psram-progress\" style=\"width:'+psramPct+'%\">'+psramPct+'%</div></div>';";
  html += "}";
  html += "h+='</div>';";

  html += "h+='<div class=\"section\"><h2>📡 Connexion WiFi</h2><div class=\"info-grid\">';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">SSID</div><div class=\"info-value\">'+d.wifi.ssid+'</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">RSSI</div><div class=\"info-value\">'+d.wifi.rssi+' dBm</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Qualité</div><div class=\"info-value\">'+d.wifi.quality+'</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Adresse IP</div><div class=\"info-value\">'+d.wifi.ip+'</div></div>';";
  html += "h+='</div></div>';";

  html += "h+='<div class=\"section\"><h2>🔌 GPIO et Interfaces</h2><div class=\"info-grid\">';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Total GPIO</div><div class=\"info-value\">'+d.gpio.total+'</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Périphériques I2C</div><div class=\"info-value\">'+d.gpio.i2c_count+'</div></div>';";
  html += "h+='<div class=\"info-item\" style=\"grid-column:1/-1\"><div class=\"info-label\">Adresses I2C</div><div class=\"info-value\">'+d.gpio.i2c_devices+'</div></div>';";
  html += "h+='</div></div>';";
  html += "return h;";
  html += "}";

  // Build LEDs (simplifié)
  html += "function buildLeds(d){";
  html += "let h='<div class=\"section\"><h2>💡 LED Intégrée</h2><div class=\"info-grid\">';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">GPIO</div><div class=\"info-value\">GPIO '+d.builtin.pin+'</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Statut</div><div class=\"info-value\" id=\"builtin-led-status\">'+d.builtin.status+'</div></div>';";
  html += "h+='<div class=\"info-item\" style=\"grid-column:1/-1;text-align:center\">';";
  html += "h+='<button class=\"btn btn-primary\" onclick=\"testBuiltinLED()\">🧪 Test complet</button> ';";
  html += "h+='<button class=\"btn btn-success\" onclick=\"ledControl(\\'on\\')\">💡 ON</button> ';";
  html += "h+='<button class=\"btn btn-success\" onclick=\"ledControl(\\'blink\\')\">⚡ Blink</button> ';";
  html += "h+='<button class=\"btn btn-info\" onclick=\"ledControl(\\'fade\\')\">🌊 Fade</button> ';";
  html += "h+='<button class=\"btn btn-danger\" onclick=\"ledControl(\\'off\\')\">⭕ OFF</button>';";
  html += "h+='</div></div></div>';";
  html += "h+='<div class=\"section\"><h2>🌈 NeoPixel</h2><div class=\"info-grid\">';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">GPIO</div><div class=\"info-value\">GPIO '+d.neopixel.pin+'</div></div>';";
  html += "h+='<div class=\"info-item\"><div class=\"info-label\">Statut</div><div class=\"info-value\" id=\"neopixel-status\">'+d.neopixel.status+'</div></div>';";
  html += "h+='<div class=\"info-item\" style=\"grid-column:1/-1;text-align:center\">';";
  html += "h+='<button class=\"btn btn-primary\" onclick=\"testNeoPixel()\">🧪 Test</button> ';";
  html += "h+='<button class=\"btn btn-primary\" onclick=\"neoPattern(\\'rainbow\\')\">🌈 Rainbow</button> ';";
  html += "h+='<input type=\"color\" id=\"neoColor\" value=\"#ff0000\" style=\"height:48px\"> ';";
  html += "h+='<button class=\"btn btn-primary\" onclick=\"neoCustomColor()\">🎨 Couleur</button> ';";
  html += "h+='<button class=\"btn btn-danger\" onclick=\"neoPattern(\\'off\\')\">⭕ OFF</button>';";
  html += "h+='</div></div></div>';";
  html += "return h;";
  html += "}";

  // Autres fonctions build (simplifié pour économiser de l'espace)
  html += "function buildScreens(d){return '<div class=\"section\"><h2>📺 Écrans</h2><p>TFT: '+d.tft.status+'</p><p>OLED: '+d.oled.status+'</p></div>';}";
  html += "function buildTests(){return '<div class=\"section\"><h2>🧪 Tests</h2><p>Utilisez les boutons ci-dessous</p></div>';}";
  html += "function buildGpio(){return '<div class=\"section\"><h2>🔌 GPIO</h2><button class=\"btn btn-primary\" onclick=\"testAllGPIO()\">Tester tous les GPIO</button><div id=\"gpio-results\"></div></div>';}";
  html += "function buildWifi(){return '<div class=\"section\"><h2>📡 WiFi</h2><button class=\"btn btn-primary\" onclick=\"scanWiFi()\">Scanner réseaux</button><div id=\"wifi-results\"></div></div>';}";
  html += "function buildBenchmark(){return '<div class=\"section\"><h2>⚡ Performance</h2><button class=\"btn btn-primary\" onclick=\"runBenchmarks()\">Lancer benchmarks</button><div id=\"bench-results\"></div></div>';}";
  html += "function buildExport(){return '<div class=\"section\"><h2>💾 Export</h2><a href=\"/export/txt\" class=\"btn btn-primary\">TXT</a><a href=\"/export/json\" class=\"btn btn-info\">JSON</a><a href=\"/export/csv\" class=\"btn btn-success\">CSV</a></div>';}";

  // Fonctions de contrôle
  html += "async function testBuiltinLED(){showUpdateBadge('Test LED...');const r=await fetch('/api/builtin-led-test');const d=await r.json();document.getElementById('builtin-led-status').textContent=d.result;showUpdateBadge('Test terminé!');}";
  html += "async function ledControl(action){showUpdateBadge('Contrôle LED...');const r=await fetch('/api/builtin-led-control?action='+action);const d=await r.json();document.getElementById('builtin-led-status').textContent=d.message;showUpdateBadge(d.message);}";
  html += "async function testNeoPixel(){showUpdateBadge('Test NeoPixel...');const r=await fetch('/api/neopixel-test');const d=await r.json();document.getElementById('neopixel-status').textContent=d.result;showUpdateBadge('Test terminé!');}";
  html += "async function neoPattern(p){showUpdateBadge('NeoPixel...');const r=await fetch('/api/neopixel-pattern?pattern='+p);const d=await r.json();document.getElementById('neopixel-status').textContent=d.message;showUpdateBadge(d.message);}";
  html += "async function neoCustomColor(){const c=document.getElementById('neoColor').value;const r=parseInt(c.substr(1,2),16),g=parseInt(c.substr(3,2),16),b=parseInt(c.substr(5,2),16);showUpdateBadge('Couleur RGB...');const resp=await fetch('/api/neopixel-color?r='+r+'&g='+g+'&b='+b);const d=await resp.json();document.getElementById('neopixel-status').textContent=d.message;showUpdateBadge(d.message);}";
  html += "async function testAllGPIO(){showUpdateBadge('Test GPIO...');const r=await fetch('/api/test-gpio');const d=await r.json();let h='';d.results.forEach(g=>{h+='<div style=\"padding:10px;margin:5px;background:'+(g.working?'#d4edda':'#f8d7da')+'\">GPIO '+g.pin+': '+(g.working?'✅':'❌')+'</div>'});document.getElementById('gpio-results').innerHTML=h;showUpdateBadge('Test terminé!');}";
  html += "async function scanWiFi(){showUpdateBadge('Scan WiFi...');const r=await fetch('/api/wifi-scan');const d=await r.json();let h='';d.networks.forEach(n=>{h+='<div style=\"padding:15px;margin:10px;background:#fff;border-left:4px solid #667eea\"><strong>'+n.ssid+'</strong><br>RSSI: '+n.rssi+' dBm | Canal: '+n.channel+'</div>'});document.getElementById('wifi-results').innerHTML=h;showUpdateBadge('Scan terminé!');}";
  html += "async function runBenchmarks(){showUpdateBadge('Benchmarks...');const r=await fetch('/api/benchmark');const d=await r.json();document.getElementById('bench-results').innerHTML='<div style=\"padding:15px;background:#fff;margin:10px\"><p>CPU: '+d.cpu+' µs ('+d.cpuPerf.toFixed(2)+' MFLOPS)</p><p>Mémoire: '+d.memory+' µs ('+d.memSpeed.toFixed(2)+' MB/s)</p></div>';showUpdateBadge('Terminé!');}";

  html += "</script></body></html>";

  server.send(200, "text/html; charset=utf-8", html);
}

#endif // WEB_INTERFACE_H
