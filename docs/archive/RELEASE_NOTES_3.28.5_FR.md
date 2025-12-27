# Notes de Version – Version 3.28.5

**Date de sortie :** 2025-12-24
**Type :** Version de Correction (Patch)
**Focus :** Corrections GPIO du monitoring des dispositifs d'entrée

---

## 🐛 Corrections de Bugs

Cette version de correction résout des problèmes critiques avec le monitoring des dispositifs d'entrée, spécifiquement l'état du bouton de l'encodeur rotatif et l'accès GPIO du monitoring des boutons.

### 1. Bouton Encodeur Rotatif Bloqué sur "Pressed" ✅

**Problème :**
- Après avoir pressé le bouton de l'encodeur rotatif, l'état restait "Pressed" même après relâchement
- L'interface web affichait toujours "Pressed" en rouge, ne montrait jamais "Released"
- Impossible de voir l'état réel du bouton en temps réel
- Le bouton apparaissait bloqué en permanence en état pressé

**Cause Racine :**
- `handleRotaryPosition()` retournait `rotaryButtonPressed` (variable volatile ISR)
- Variable volatile mise à `true` par l'ISR lors de la pression du bouton, mais JAMAIS automatiquement réinitialisée à `false`
- Pour le monitoring temps réel, nous devons lire l'état GPIO réel, pas la variable événementielle
- La variable ISR est pour la détection d'événements, pas pour le monitoring d'état

**Solution :**
```cpp
// src/main.cpp:3199-3203 - Nouvelle fonction pour lire le GPIO réel
int getRotaryButtonGPIOState() {
  if (rotary_sw_pin < 0 || rotary_sw_pin > 48) return -1;
  return digitalRead(rotary_sw_pin);
}

// src/main.cpp:4369-4379 - Utilisation dans handleRotaryPosition()
void handleRotaryPosition() {
  // v3.28.5 fix: Read REAL GPIO state for monitoring, not ISR variable
  int buttonGPIOState = getRotaryButtonGPIOState();
  bool buttonPressed = (buttonGPIOState == LOW && buttonGPIOState != -1);

  sendJsonResponse(200, {
    jsonNumberField("position", (int32_t)rotaryPosition),
    jsonBoolField("button_pressed", buttonPressed),  // Lit maintenant le GPIO réel
    jsonBoolField("available", rotaryAvailable)
  });
}
```

**Impact :**
- ✅ Le bouton de l'encodeur rotatif affiche maintenant l'état correct en temps réel
- ✅ "Pressed" (rouge) lorsque le bouton est physiquement pressé
- ✅ "Released" (vert) lorsque le bouton est physiquement relâché
- ✅ Mise à jour immédiate (polling de 100ms)
- ✅ Monitoring d'état précis pour les interactions utilisateur

---

### 2. Monitoring des Boutons Non Fonctionnel (BOOT/Bouton1/Bouton2) ✅

**Problème :**
- Le monitoring pour BOOT, Bouton 1, Bouton 2 ne fonctionnait pas
- Les états ne se mettaient jamais à jour malgré la correction v3.28.4
- Les boutons restaient bloqués sur l'état "Released"
- Le monitoring temps réel complètement non fonctionnel

**Cause Racine :**
- Les fonctions utilisaient des variables `static` (`buttonBootPin`, `button1Pin`, `button2Pin`)
- Problème potentiel de visibilité ou d'initialisation avec les variables statiques
- Le GPIO peut ne pas être correctement accessible via ces variables
- Problème de timing ou de portée d'accès aux variables

**Solution :**
```cpp
// src/main.cpp:3182-3199 - Accès direct aux constantes
// v3.28.5: Use constants directly to ensure correct pin access
int getButtonBootState() {
  // Utilise la constante directement au lieu de la variable statique
  if (BUTTON_BOOT < 0 || BUTTON_BOOT > 48) return -1;
  return digitalRead(BUTTON_BOOT);
}

int getButton1State() {
  if (BUTTON_1 < 0 || BUTTON_1 > 48) return -1;
  return digitalRead(BUTTON_1);
}

int getButton2State() {
  if (BUTTON_2 < 0 || BUTTON_2 > 48) return -1;
  return digitalRead(BUTTON_2);
}

// src/main.cpp:4420-4428 - handleButtonState() utilise les constantes
if (buttonParam == "boot") {
  state = getButtonBootState();
  pin = BUTTON_BOOT;  // v3.28.5: Utilise la constante directement
} else if (buttonParam == "1" || buttonParam == "button1") {
  state = getButton1State();
  pin = BUTTON_1;
} else if (buttonParam == "2" || buttonParam == "button2") {
  state = getButton2State();
  pin = BUTTON_2;
}
```

**Impact :**
- ✅ Le monitoring du bouton BOOT (GPIO 0) fonctionne maintenant
- ✅ Le monitoring du Bouton 1 (GPIO 38/5) fonctionne maintenant
- ✅ Le monitoring du Bouton 2 (GPIO 39/12) fonctionne maintenant
- ✅ Les états se mettent à jour en temps réel
- ✅ L'affichage "Pressed" (rouge gras) / "Released" (vert) est correct

---

## 📝 Fichiers Modifiés

**src/main.cpp :**
- Lignes 3182-3203 : Mise à jour des lecteurs d'état des boutons pour utiliser les constantes directement, ajout de `getRotaryButtonGPIOState()`
- Lignes 4369-4379 : `handleRotaryPosition()` lit maintenant l'état GPIO réel au lieu de la variable ISR
- Lignes 4389-4407 : `handleButtonStates()` utilise les constantes pour les numéros de broches
- Lignes 4420-4428 : `handleButtonState()` utilise les constantes pour l'accès aux broches

**platformio.ini :**
- Version incrémentée de 3.28.4 → 3.28.5

---

## 🧪 Tests

### Test Encodeur Rotatif :
1. Activer le monitoring du bouton de l'encodeur rotatif
2. Presser le bouton → "Pressed" (rouge) s'affiche immédiatement ✅
3. Relâcher → affiche immédiatement "Released" (vert) ✅
4. Répéter plusieurs fois → les états sont toujours corrects ✅
5. Aucun état bloqué ✅

### Test Monitoring des Boutons :
1. Activer le monitoring pour chaque bouton (BOOT, Bouton 1, Bouton 2)
2. Presser GPIO 0/38/39 → "Pressed" affiché immédiatement ✅
3. Relâcher → "Released" affiché immédiatement ✅
4. Aucun état bloqué sur aucun bouton ✅
5. Mises à jour en temps réel toutes les 100ms ✅

### Test API :
```bash
# Test position encodeur rotatif et bouton
curl http://esp32-diagnostic.local/api/rotary-position
# Devrait afficher: {"position":0,"button_pressed":false,"available":true}

# Test boutons individuels
curl http://esp32-diagnostic.local/api/button-state?button=boot
curl http://esp32-diagnostic.local/api/button-state?button=1
curl http://esp32-diagnostic.local/api/button-state?button=2
```

---

## 📚 Documentation Associée

- [CHANGELOG_FR.md](../CHANGELOG_FR.md) - Historique complet des versions
- [PIN_MAPPING_FR.md](PIN_MAPPING_FR.md) - Assignations des broches GPIO pour toutes les cartes
- [API_REFERENCE_FR.md](API_REFERENCE_FR.md) - Documentation de l'API REST
- [WEB_INTERFACE_FR.md](WEB_INTERFACE_FR.md) - Guide de l'interface web
- [TROUBLESHOOTING_FR.md](TROUBLESHOOTING_FR.md) - Problèmes courants et solutions

---

## ⬆️ Notes de Mise à Niveau

- **Aucun changement incompatible** - entièrement rétrocompatible avec v3.28.4
- **Aucun changement de configuration requis** - fonctionne avec le `board_config.h` existant
- **Correction automatique** - tout le monitoring des dispositifs d'entrée fonctionne immédiatement après la mise à niveau
- **Compatible avec tous les environnements :** `esp32s3_n16r8`, `esp32s3_n8r8`, `esp32devkitc`
- **Performance :** Aucun impact sur les performances, même intervalle de polling de 100ms

---

## 🔄 Progression des Versions

| Fonctionnalité | v3.28.3 | v3.28.4 | v3.28.5 |
|----------------|---------|---------|---------|
| Auto-init encodeur rotatif | ✅ | ✅ | ✅ |
| Endpoints API boutons | ✅ | ✅ | ✅ |
| Monitoring frontend | ❌ | ✅ | ✅ |
| État bouton encodeur correct | ❌ | ❌ | ✅ |
| Monitoring BOOT/BTN1/BTN2 | ❌ | ❌ | ✅ |
| Précision état temps réel | ❌ | ❌ | ✅ |

---

## 🎯 Problèmes Connus Corrigés

Cette version complète l'ensemble des fonctionnalités de monitoring des dispositifs d'entrée en corrigeant les derniers problèmes restants :
- ✅ v3.28.3 : Ajout de l'initialisation de l'encodeur rotatif et des endpoints de monitoring des boutons
- ✅ v3.28.4 : Correction de l'incompatibilité d'endpoint API entre frontend et backend
- ✅ v3.28.5 : Correction de l'accès GPIO pour une lecture d'état précise en temps réel

Toutes les fonctionnalités de monitoring des dispositifs d'entrée sont maintenant pleinement fonctionnelles.

---

## 🔗 Liens

- [Dépôt ESP32-Diagnostic](https://github.com/morfredus/ESP32-Diagnostic)
- [Guide d'Installation](INSTALL_FR.md)
- [Guide Build & Deploy](BUILD_AND_DEPLOY_FR.md)
- [Guide de Configuration](CONFIG_FR.md)
