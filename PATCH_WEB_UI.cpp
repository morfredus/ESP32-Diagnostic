/*
 * PATCH_WEB_UI.cpp - Correctifs pour l'interface web UI
 *
 * Instructions d'application :
 * 1. Copier les fonctions ci-dessous dans src/main.cpp
 * 2. Remplacer la fonction handleJavaScriptRoute() existante
 * 3. Ajouter setupDebugRoutes() dans setup() après server.begin()
 * 4. Recompiler et flasher
 */

// ================================================================================
// CORRECTIF #1 : handleJavaScriptRoute avec débogage
// ================================================================================
// Remplacer dans src/main.cpp ligne ~3790

void handleJavaScriptRoute() {
  unsigned long startTime = millis();

  String js = generateJavaScript();
  unsigned long generateTime = millis() - startTime;

  // ===== LOGS DE DÉBOGAGE =====
  Serial.println("\n========== JAVASCRIPT DEBUG ==========");
  Serial.printf("Temps de génération: %lu ms\n", generateTime);
  Serial.printf("Taille JavaScript: %d octets\n", js.length());
  Serial.printf("Mémoire libre avant: %d octets\n", ESP.getFreeHeap());

  // Vérifier la présence des fonctions critiques
  bool hasShowTab = (js.indexOf("function showTab") != -1);
  bool hasChangeLang = (js.indexOf("function changeLang") != -1);
  bool hasLoadTab = (js.indexOf("function loadTab") != -1);
  bool hasInitNav = (js.indexOf("function initNavigation") != -1);

  Serial.printf("✓ function showTab: %s\n", hasShowTab ? "OUI" : "NON ❌");
  Serial.printf("✓ function changeLang: %s\n", hasChangeLang ? "OUI" : "NON ❌");
  Serial.printf("✓ function loadTab: %s\n", hasLoadTab ? "OUI" : "NON ❌");
  Serial.printf("✓ function initNavigation: %s\n", hasInitNav ? "OUI" : "NON ❌");

  // Afficher le début et la fin du JS pour vérifier la troncature
  Serial.println("\n--- Début du JavaScript (100 premiers caractères) ---");
  Serial.println(js.substring(0, min(100, (int)js.length())));
  Serial.println("\n--- Fin du JavaScript (100 derniers caractères) ---");
  int endStart = max(0, (int)js.length() - 100);
  Serial.println(js.substring(endStart));
  Serial.println("======================================\n");

  // Vérifier si le JavaScript est tronqué
  if (!hasShowTab || !hasChangeLang) {
    Serial.println("⚠️ ERREUR CRITIQUE : Fonctions JavaScript manquantes!");
    Serial.println("   Cause probable : JavaScript trop volumineux ou erreur de génération");
    Serial.println("   Action : Augmenter la réservation mémoire dans web_interface.h");
  }

  // Envoyer le JavaScript au client
  server.send(200, "application/javascript; charset=utf-8", js);

  Serial.printf("Mémoire libre après: %d octets\n", ESP.getFreeHeap());
}


// ================================================================================
// CORRECTIF #2 : Routes de débogage
// ================================================================================
// Ajouter cette fonction avant setup()

void setupDebugRoutes() {
  // Route de test JavaScript minimaliste
  server.on("/js/test.js", []() {
    String js = "";
    js += "console.log('✓ Test JS OK - Chargé avec succès');\n";
    js += "function showTab(tabName, btn) {\n";
    js += "  console.log('showTab appelé:', tabName);\n";
    js += "  alert('Test showTab: ' + tabName);\n";
    js += "}\n";
    js += "function changeLang(lang, btn) {\n";
    js += "  console.log('changeLang appelé:', lang);\n";
    js += "  alert('Test changeLang: ' + lang);\n";
    js += "}\n";
    js += "console.log('Fonctions de test définies');\n";
    js += "console.log('typeof showTab:', typeof showTab);\n";
    js += "console.log('typeof changeLang:', typeof changeLang);\n";

    server.send(200, "application/javascript; charset=utf-8", js);
    Serial.println("[DEBUG] Route /js/test.js servie");
  });

  // Route de diagnostic complète
  server.on("/debug/status", []() {
    String json = "{";
    json += "\"freeHeap\":" + String(ESP.getFreeHeap()) + ",";
    json += "\"jsSize\":" + String(generateJavaScript().length()) + ",";
    json += "\"version\":\"" + String(DIAGNOSTIC_VERSION_STR) + "\",";
    json += "\"uptime\":" + String(millis()) + ",";
    json += "\"chipModel\":\"" + String(ESP.getChipModel()) + "\"";
    json += "}";

    server.send(200, "application/json", json);
    Serial.println("[DEBUG] Route /debug/status servie");
  });

  // Page HTML de test
  server.on("/debug/test", []() {
    String html = R"HTML(<!DOCTYPE html>
<html lang='fr'>
<head>
  <meta charset='UTF-8'>
  <title>ESP32 Debug</title>
  <style>
    body { font-family: monospace; padding: 20px; background: #1e1e1e; color: #d4d4d4; }
    .section { background: #2d2d2d; padding: 15px; margin: 10px 0; border-radius: 5px; }
    .ok { color: #4ec9b0; }
    .error { color: #f48771; }
    button { padding: 10px 20px; margin: 5px; cursor: pointer; }
  </style>
</head>
<body>
  <h1>ESP32 Diagnostic - Test Interface</h1>

  <div class='section'>
    <h2>1. Test JavaScript Minimaliste</h2>
    <button onclick="testMinimalJS()">Charger test.js</button>
    <div id='test1'></div>
  </div>

  <div class='section'>
    <h2>2. Test JavaScript Principal</h2>
    <button onclick="testMainJS()">Charger app.js</button>
    <div id='test2'></div>
  </div>

  <div class='section'>
    <h2>3. Test Fonctions</h2>
    <button onclick="testFunctions()">Vérifier fonctions</button>
    <div id='test3'></div>
  </div>

  <div class='section'>
    <h2>4. Test API</h2>
    <button onclick="testAPI()">Tester /api/overview</button>
    <div id='test4'></div>
  </div>

  <script>
    function log(id, msg, isError = false) {
      const el = document.getElementById(id);
      const cls = isError ? 'error' : 'ok';
      el.innerHTML += `<p class='${cls}'>${msg}</p>`;
    }

    function testMinimalJS() {
      document.getElementById('test1').innerHTML = '';
      const script = document.createElement('script');
      script.src = '/js/test.js?t=' + Date.now();
      script.onload = () => log('test1', '✓ test.js chargé avec succès');
      script.onerror = () => log('test1', '✗ Erreur de chargement test.js', true);
      document.body.appendChild(script);

      setTimeout(() => {
        if (typeof showTab !== 'undefined') {
          log('test1', '✓ Fonction showTab définie');
          showTab('test', null);
        } else {
          log('test1', '✗ Fonction showTab non définie', true);
        }
      }, 500);
    }

    function testMainJS() {
      document.getElementById('test2').innerHTML = '';
      fetch('/js/app.js?t=' + Date.now())
        .then(r => {
          log('test2', '✓ Réponse reçue, status: ' + r.status);
          return r.text();
        })
        .then(text => {
          log('test2', '✓ Taille du JS: ' + text.length + ' octets');

          // Vérifier la présence des fonctions
          const hasShowTab = text.includes('function showTab');
          const hasChangeLang = text.includes('function changeLang');
          const hasLoadTab = text.includes('function loadTab');

          log('test2', hasShowTab ? '✓ showTab trouvée' : '✗ showTab manquante', !hasShowTab);
          log('test2', hasChangeLang ? '✓ changeLang trouvée' : '✗ changeLang manquante', !hasChangeLang);
          log('test2', hasLoadTab ? '✓ loadTab trouvée' : '✗ loadTab manquante', !hasLoadTab);

          // Afficher le début
          log('test2', 'Début: ' + text.substring(0, 100) + '...');
        })
        .catch(err => log('test2', '✗ Erreur: ' + err, true));
    }

    function testFunctions() {
      document.getElementById('test3').innerHTML = '';
      log('test3', 'typeof showTab: ' + typeof showTab);
      log('test3', 'typeof changeLang: ' + typeof changeLang);
      log('test3', 'typeof loadTab: ' + typeof loadTab);
      log('test3', 'typeof initNavigation: ' + typeof initNavigation);
    }

    function testAPI() {
      document.getElementById('test4').innerHTML = '';
      fetch('/api/overview')
        .then(r => r.json())
        .then(data => {
          log('test4', '✓ API overview OK');
          log('test4', 'Chip: ' + data.chip.model);
          log('test4', 'Mémoire SRAM libre: ' + data.memory.sram.free);
        })
        .catch(err => log('test4', '✗ Erreur API: ' + err, true));
    }
  </script>
</body>
</html>)HTML";

    server.send(200, "text/html; charset=utf-8", html);
    Serial.println("[DEBUG] Route /debug/test servie");
  });

  Serial.println("[DEBUG] Routes de débogage installées:");
  Serial.println("  - /js/test.js (JavaScript minimaliste)");
  Serial.println("  - /debug/status (JSON statut système)");
  Serial.println("  - /debug/test (Page HTML de test)");
}


// ================================================================================
// CORRECTIF #3 : Augmentation de la réservation mémoire
// ================================================================================
// Dans include/web_interface.h ligne ~380, modifier :

/*
// AVANT :
js.reserve(45000);

// APRÈS :
js.reserve(65000);  // Augmenté de 45000 à 65000 octets
*/


// ================================================================================
// CORRECTIF #4 : Modification du setup()
// ================================================================================
// Dans src/main.cpp, dans la fonction setup(), après server.begin(), ajouter :

/*
void setup() {
  // ... code existant ...

  server.begin();

  // AJOUTER CETTE LIGNE :
  setupDebugRoutes();

  Serial.println("Serveur démarré");
  // ... reste du code ...
}
*/


// ================================================================================
// INSTRUCTIONS D'UTILISATION
// ================================================================================

/*
ÉTAPE 1 : Appliquer les correctifs
-------------------------------
1. Copier la nouvelle fonction handleJavaScriptRoute() dans src/main.cpp
2. Ajouter la fonction setupDebugRoutes() avant setup()
3. Appeler setupDebugRoutes() dans setup() après server.begin()
4. Modifier web_interface.h ligne 380 : js.reserve(65000);

ÉTAPE 2 : Recompiler et flasher
-------------------------------
1. Ouvrir PlatformIO dans VS Code
2. Compiler le projet (Ctrl+Alt+B)
3. Flasher sur l'ESP32 (Ctrl+Alt+U)
4. Ouvrir le moniteur série (115200 baud)

ÉTAPE 3 : Tests de diagnostic
-------------------------------
1. Connecter au WiFi (attendre l'IP dans les logs)
2. Accéder à http://ESP32-IP/debug/test
3. Cliquer sur tous les boutons de test
4. Noter les résultats affichés
5. Vérifier les logs série pour voir les détails

ÉTAPE 4 : Analyser les résultats
-------------------------------
Si test.js fonctionne mais app.js ne fonctionne pas :
  → Le problème est dans la génération du JavaScript principal
  → Vérifier les logs série pour voir la taille et les fonctions manquantes
  → Augmenter encore js.reserve() si nécessaire

Si aucun des deux ne fonctionne :
  → Problème de serveur web ou de mémoire
  → Vérifier la mémoire libre dans les logs
  → Vérifier que le serveur répond aux requêtes

Si tout fonctionne sur /debug/test mais pas sur / :
  → Problème dans le HTML généré par generateHTML()
  → Vérifier l'ordre de chargement du script
  → Tester avec defer : <script src='/js/app.js' defer></script>

ÉTAPE 5 : Correction finale
-------------------------------
Une fois le problème identifié :
1. Appliquer la correction appropriée
2. Retirer les routes de débogage (commenter setupDebugRoutes())
3. Retirer les logs de débogage de handleJavaScriptRoute()
4. Recompiler en version de production
5. Tester l'interface principale sur http://ESP32-IP/

========================
Support et Documentation
========================
- Document de débogage complet : DEBUGGING_WEB_UI.md
- Changelog : CHANGELOG.md / CHANGELOG_FR.md
- README : README.md / README_FR.md
*/
