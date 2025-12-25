# Notes de Version – Version 3.28.4

**Date de sortie :** 2025-12-24
**Type :** Version de Correction (Patch)
**Focus :** Correction de l'endpoint API de monitoring des boutons

---

## 🐛 Correction de Bug

Cette version de correction résout une incompatibilité critique d'endpoint API qui empêchait le monitoring des boutons de fonctionner.

### Monitoring des Boutons Non Fonctionnel - Incompatibilité d'Endpoint API ✅

**Problème :**
- Les boutons de monitoring (BOOT, Bouton 1, Bouton 2) affichaient toujours l'état "Released"
- Cliquer sur "Monitor Button" n'avait aucun effet - l'état ne se mettait jamais à jour dans l'interface web
- Le JavaScript frontend appelait le mauvais endpoint API
- Les utilisateurs ne pouvaient pas surveiller les états des boutons en temps réel

**Cause Racine :**
- Le JavaScript frontend appelle `/api/button-state` (singulier) pour les requêtes de boutons individuels
- Le backend n'avait que l'endpoint `/api/button-states` (pluriel) qui retourne TOUS les boutons
- Incompatibilité d'endpoint : le frontend s'attendait à une requête de bouton individuel, le backend fournissait seulement une requête par lot
- Aucun handler de route enregistré pour `/api/button-state` (singulier)
- Résultat : erreurs 404 à chaque tentative de monitoring, échecs silencieux dans l'interface

**Solution :**
```cpp
// src/main.cpp:4395-4431 - Ajout du handler d'état de bouton individuel
void handleButtonState() {
  if (!server.hasArg("button")) {
    sendActionResponse(400, false, "Missing 'button' parameter");
    return;
  }

  String buttonParam = server.arg("button");
  int state = -1;
  int pin = -1;

  if (buttonParam == "boot") {
    state = getButtonBootState();
    pin = buttonBootPin;
  } else if (buttonParam == "1" || buttonParam == "button1") {
    state = getButton1State();
    pin = button1Pin;
  } else if (buttonParam == "2" || buttonParam == "button2") {
    state = getButton2State();
    pin = button2Pin;
  } else {
    sendActionResponse(400, false, "Invalid button parameter");
    return;
  }

  // LOW = pressé (pull-up), HIGH = relâché
  bool pressed = (state == LOW && state != -1);
  bool available = (state != -1);

  sendJsonResponse(200, {
    jsonBoolField("pressed", pressed),
    jsonBoolField("released", !pressed && available),
    jsonBoolField("available", available),
    jsonNumberField("pin", pin),
    jsonNumberField("raw_state", state)
  });
}

// src/main.cpp:5798 - Enregistrement de la route
server.on("/api/button-state", handleButtonState);
```

**Format de Réponse API :**
```json
GET /api/button-state?button=boot
{
  "pressed": false,
  "released": true,
  "available": true,
  "pin": 0,
  "raw_state": 1
}

GET /api/button-state?button=1
{
  "pressed": true,
  "released": false,
  "available": true,
  "pin": 38,
  "raw_state": 0
}
```

**Impact :**
- ✅ Le monitoring des boutons fonctionne maintenant correctement
- ✅ L'état se met à jour en temps réel (polling de 100ms) lorsque le monitoring est activé
- ✅ "Pressed" affiché en rouge gras lorsque le bouton est pressé
- ✅ "Released" affiché en vert lorsque le bouton est relâché
- ✅ Fonctionne pour BOOT (GPIO 0), Bouton 1, et Bouton 2
- ✅ Gestion d'erreurs appropriée pour les paramètres de bouton invalides

---

## 📝 Fichiers Modifiés

**src/main.cpp :**
- Lignes 4395-4431 : Ajout du handler `handleButtonState()` pour les requêtes de boutons individuels
- Ligne 5798 : Enregistrement de la route `/api/button-state`

**platformio.ini :**
- Version incrémentée de 3.28.3 → 3.28.4

---

## 🧪 Tests

### Test de Monitoring des Boutons :
1. Navigation vers la page "Input Devices" dans l'interface web
2. Clic sur "Monitor Button" pour le bouton BOOT
3. Pression du bouton GPIO 0 (BOOT) sur l'ESP32 - l'état devrait changer en "Pressed" (rouge gras) ✅
4. Relâchement du bouton - l'état devrait revenir à "Released" (vert) ✅
5. Répétition pour Bouton 1 (GPIO 38/5) et Bouton 2 (GPIO 39/12) ✅
6. Vérification des mises à jour en temps réel toutes les 100ms ✅
7. Vérification de la console du navigateur - aucune erreur 404 ✅

### Test de l'Endpoint API :
```bash
# Test bouton BOOT
curl http://esp32-diagnostic.local/api/button-state?button=boot

# Test Bouton 1
curl http://esp32-diagnostic.local/api/button-state?button=1

# Test Bouton 2
curl http://esp32-diagnostic.local/api/button-state?button=2

# Test paramètre invalide (devrait retourner une erreur)
curl http://esp32-diagnostic.local/api/button-state?button=invalid
```

---

## 📚 Documentation Associée

- [CHANGELOG_FR.md](../CHANGELOG_FR.md) - Historique complet des versions
- [API_REFERENCE_FR.md](API_REFERENCE_FR.md) - Documentation de l'API REST
- [WEB_INTERFACE_FR.md](WEB_INTERFACE_FR.md) - Guide de l'interface web
- [TROUBLESHOOTING_FR.md](TROUBLESHOOTING_FR.md) - Problèmes courants et solutions

---

## ⬆️ Notes de Mise à Niveau

- **Aucun changement incompatible** - entièrement rétrocompatible avec v3.28.3
- **Aucun changement de configuration requis** - fonctionne avec le `board_config.h` existant
- **Correction automatique** - le monitoring des boutons fonctionne immédiatement après la mise à niveau
- **Compatible avec tous les environnements :** `esp32s3_n16r8`, `esp32s3_n8r8`, `esp32devkitc`
- **Les deux endpoints disponibles :** `/api/button-states` (tous les boutons) et `/api/button-state` (bouton individuel)

---

## 🔄 Comparaison avec v3.28.3

| Fonctionnalité | v3.28.3 | v3.28.4 |
|----------------|---------|---------|
| Auto-init encodeur rotatif | ✅ | ✅ |
| `/api/button-states` (tous) | ✅ | ✅ |
| `/api/button-state` (individuel) | ❌ | ✅ |
| Monitoring frontend fonctionne | ❌ | ✅ |
| Affichage état bouton temps réel | ❌ | ✅ |

---

## 🔗 Liens

- [Dépôt ESP32-Diagnostic](https://github.com/morfredus/ESP32-Diagnostic)
- [Guide d'Installation](INSTALL_FR.md)
- [Mappage des Broches](PIN_MAPPING_FR.md)
