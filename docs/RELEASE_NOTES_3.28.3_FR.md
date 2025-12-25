# Notes de Version – Version 3.28.3

**Date de sortie :** 2025-12-24
**Type :** Version de Correction (Patch)
**Focus :** Initialisation encodeur rotatif + API de monitoring des boutons

---

## 🐛 Corrections de Bugs

Cette version de correction résout deux problèmes critiques affectant les dispositifs d'entrée :

### 1. Encodeur Rotatif Non Fonctionnel Jusqu'au Test Manuel ✅

**Problème :**
- L'encodeur rotatif ne répondait pas à la rotation ou aux pressions du bouton après le démarrage
- Ne fonctionnait qu'après navigation vers la page "Input Devices" et clic sur "Test Rotary Encoder"
- Rendait l'encodeur rotatif inutilisable pour une opération immédiate après mise sous tension

**Cause Racine :**
- `initRotaryEncoder()` n'était JAMAIS appelée pendant le démarrage dans `setup()`
- La fonction n'était appelée que dans `testRotaryEncoder()` qui nécessite un déclenchement manuel via l'interface web
- Les broches GPIO n'étaient pas configurées et les interruptions n'étaient pas attachées au démarrage
- Laissait l'encodeur dans un état non fonctionnel jusqu'à une action explicite de l'utilisateur

**Solution :**
```cpp
// src/main.cpp:5757-5765 - Ajout de l'initialisation de l'encodeur rotatif dans setup()
// Initialize rotary encoder on startup (v3.28.3 fix)
Serial.println("Initialisation de l'encodeur rotatif...");
initRotaryEncoder();
if (rotaryAvailable) {
  Serial.printf("Encodeur rotatif OK: CLK=%d, DT=%d, SW=%d\r\n",
                rotary_clk_pin, rotary_dt_pin, rotary_sw_pin);
} else {
  Serial.println("Encodeur rotatif: non disponible ou configuration invalide");
}
```

**Impact :**
- ✅ L'encodeur rotatif s'initialise maintenant automatiquement au démarrage
- ✅ La détection de rotation fonctionne immédiatement sans test manuel
- ✅ Les pressions du bouton sont détectées dès la mise sous tension
- ✅ Le suivi de position en temps réel est disponible via `/api/rotary-position` immédiatement

---

### 2. Monitoring des Boutons Non Fonctionnel ✅

**Problème :**
- Les boutons "Monitor Button" dans l'interface web ne faisaient rien lors du clic
- Aucun moyen de voir l'état du bouton en temps réel (pressé/relâché)
- Les fonctions JavaScript de monitoring existaient mais les endpoints API backend manquaient
- Le polling frontend échouait silencieusement

**Cause Racine :**
- Le code frontend référençait des fonctions de monitoring (`toggleBootButtonMonitoring()`, etc.)
- MAIS aucun endpoint API backend n'existait pour lire les états des boutons en temps réel
- Handler de route `/api/button-states` manquant
- Le polling frontend retournait des erreurs 404

**Solution :**
```cpp
// src/main.cpp:3182-3196 - Ajout des fonctions de lecture d'état des boutons
int getButtonBootState() {
  if (buttonBootPin < 0 || buttonBootPin > 48) return -1;
  return digitalRead(buttonBootPin);
}

int getButton1State() {
  if (button1Pin < 0 || button1Pin > 48) return -1;
  return digitalRead(button1Pin);
}

int getButton2State() {
  if (button2Pin < 0 || button2Pin > 48) return -1;
  return digitalRead(button2Pin);
}

// src/main.cpp:4375-4393 - Ajout du handler HTTP pour tous les boutons
void handleButtonStates() {
  int bootState = getButtonBootState();
  int button1State = getButton1State();
  int button2State = getButton2State();

  // LOW = pressé (pull-up), HIGH = relâché
  sendJsonResponse(200, {
    jsonBoolField("boot_pressed", bootState == LOW && bootState != -1),
    jsonBoolField("boot_available", bootState != -1),
    jsonBoolField("button1_pressed", button1State == LOW && button1State != -1),
    jsonBoolField("button1_available", button1State != -1),
    jsonBoolField("button2_pressed", button2State == LOW && button2State != -1),
    jsonBoolField("button2_available", button2State != -1),
    jsonNumberField("boot_pin", buttonBootPin),
    jsonNumberField("button1_pin", button1Pin),
    jsonNumberField("button2_pin", button2Pin)
  });
}

// src/main.cpp:5759 - Enregistrement de la route
server.on("/api/button-states", handleButtonStates);
```

**Format de Réponse API :**
```json
GET /api/button-states
{
  "boot_pressed": false,
  "boot_available": true,
  "button1_pressed": false,
  "button1_available": true,
  "button2_pressed": false,
  "button2_available": true,
  "boot_pin": 0,
  "button1_pin": 38,
  "button2_pin": 39
}
```

**Impact :**
- ✅ Nouvel endpoint API `/api/button-states` retourne les états en temps réel pour TOUS les boutons
- ✅ Retourne JSON avec l'état pressé pour BOOT, Bouton 1, et Bouton 2
- ✅ Inclut les numéros de broches et le statut de disponibilité
- ✅ Le monitoring frontend peut maintenant interroger cet endpoint pour des mises à jour en direct (intervalle de 100ms)
- ✅ Retour en temps réel lorsque l'utilisateur presse les boutons physiques

---

## 📝 Fichiers Modifiés

**src/main.cpp :**
- Lignes 3182-3196 : Ajout des fonctions de lecture d'état des boutons (`getButtonBootState()`, `getButton1State()`, `getButton2State()`)
- Lignes 4375-4393 : Ajout du handler HTTP `handleButtonStates()` pour `/api/button-states`
- Lignes 5757-5765 : Initialisation de l'encodeur rotatif dans `setup()` pour fonctionnalité immédiate
- Ligne 5759 : Enregistrement de la route `/api/button-states`

**platformio.ini :**
- Version incrémentée de 3.28.2 → 3.28.3

---

## ✅ Tests

### Test Encodeur Rotatif :
1. Mise sous tension de l'ESP32
2. Rotation de l'encodeur immédiatement - la position devrait changer ✅
3. Pression du bouton de l'encodeur - devrait être enregistrée ✅
4. Navigation vers la page "Input Devices" - encodeur déjà fonctionnel ✅
5. Suivi de position précis sans initialisation manuelle ✅

### Test Monitoring des Boutons :
1. Navigation vers la page "Input Devices"
2. Pression du bouton BOOT (GPIO 0) - le retour LED/buzzer devrait fonctionner ✅
3. Vérification de l'endpoint `/api/button-states` - devrait retourner les états actuels ✅
4. Clic sur "Monitor Button" pour n'importe quel bouton - l'état en temps réel devrait se mettre à jour ✅
5. Pression/relâchement du bouton surveillé - l'interface devrait refléter les changements ✅

---

## 📚 Documentation Associée

- [CHANGELOG_FR.md](../CHANGELOG_FR.md) - Historique complet des versions
- [PIN_MAPPING_FR.md](PIN_MAPPING_FR.md) - Assignations des broches GPIO
- [API_REFERENCE_FR.md](API_REFERENCE_FR.md) - Documentation de l'API REST
- [WEB_INTERFACE_FR.md](WEB_INTERFACE_FR.md) - Guide de l'interface web

---

## ⬆️ Notes de Mise à Niveau

- **Aucun changement incompatible** - entièrement rétrocompatible avec v3.28.2
- **Aucun changement de configuration requis** - fonctionne avec le `board_config.h` existant
- **Amélioration automatique** - l'encodeur rotatif fonctionne maintenant immédiatement après le flash
- **Compatible avec tous les environnements :** `esp32s3_n16r8`, `esp32s3_n8r8`, `esp32devkitc`

---

## 🔗 Liens

- [Dépôt ESP32-Diagnostic](https://github.com/morfredus/ESP32-Diagnostic)
- [Guide d'Installation](INSTALL_FR.md)
- [Dépannage](TROUBLESHOOTING_FR.md)
