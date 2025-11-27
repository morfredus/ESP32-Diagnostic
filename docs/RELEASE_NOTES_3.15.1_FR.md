# Notes de Version v3.15.1 - Correction Mémoire Critique

**Date de sortie :** 27 novembre 2025  
**Type :** Release Patch (Correction de Bug)  
**Sévérité :** Critique pour les utilisateurs ESP32 Classic

---

## Vue d'ensemble

La version 3.15.1 corrige un problème mémoire critique qui empêchait le chargement de l'interface web sur les cartes ESP32 Classic (environnement `esp32devkitc`) sans PSRAM. Ce patch implémente un streaming par morceaux du contenu JavaScript depuis PROGMEM, éliminant les grosses allocations heap qui causaient des plantages sur les appareils à mémoire limitée.

---

## Correction de Bug Critique

### Échec de Chargement de l'Interface Web sur ESP32 Classic

**Problème :**
- Les pages web ne se chargeaient pas ou affichaient un contenu vide sur ESP32 Classic (4Mo Flash, sans PSRAM)
- Cause racine : Gros fichier JavaScript (`DIAGNOSTIC_JS_STATIC`) converti de PROGMEM en un seul objet String, causant épuisement du heap
- Symptôme : Erreurs HTTP 500, timeouts de connexion, ou rendu de page incomplet

**Solution :**
- Implémentation du transfert par morceaux (chunked transfer) pour le contenu JavaScript
- Modification de `handleJavaScriptRoute()` pour streamer les données PROGMEM en blocs de 1Ko via `memcpy_P()` et `server.sendContent()`
- Élimination de l'unique grosse allocation String (précédemment ~50Ko+)

**Impact :**
- ✅ ESP32 Classic (`esp32devkitc`) : Interface web se charge désormais de manière fiable
- ✅ ESP32-S3 N16R8 : Efficacité mémoire améliorée, aucune régression
- ✅ ESP32-S3 N8R8 : Efficacité mémoire améliorée, aucune régression

---

## Détails Techniques

### Fichiers Modifiés
- `src/main.cpp` : Fonction `handleJavaScriptRoute()` modifiée

### Avant (v3.15.0)
```cpp
// Grosse allocation - échoue sur ESP32 Classic
const char* staticJsPtr = DIAGNOSTIC_JS_STATIC;
size_t staticJsLen = strlen(staticJsPtr);
server.sendContent(String(FPSTR(DIAGNOSTIC_JS_STATIC)));
```

### Après (v3.15.1)
```cpp
// Streaming par morceaux - économe en mémoire
const char* staticJsPtr = DIAGNOSTIC_JS_STATIC;
size_t staticJsLen = strlen_P(staticJsPtr);
const size_t CHUNK_SIZE = 1024;
char chunkBuf[CHUNK_SIZE + 1];
size_t sent = 0;
while (sent < staticJsLen) {
  size_t n = (staticJsLen - sent) > CHUNK_SIZE ? CHUNK_SIZE : (staticJsLen - sent);
  memcpy_P(chunkBuf, staticJsPtr + sent, n);
  chunkBuf[n] = '\0';
  server.sendContent(chunkBuf);
  sent += n;
}
```

### Économies Mémoire
- **Réduction du pic d'allocation heap de ~50Ko** pendant le service des pages web
- Transfert par morceaux utilise seulement un buffer stack de 1Ko par itération
- Aucun changement fonctionnel de l'UI ou de l'expérience utilisateur

---

## Tests & Validation

### Environnements Testés
| Environnement | Carte | Flash | PSRAM | Statut |
|---------------|-------|-------|-------|--------|
| `esp32s3_n16r8` | ESP32-S3 DevKitC-1 N16R8 | 16Mo | 8Mo OPI | ✅ Validé |
| `esp32s3_n8r8` | ESP32-S3 DevKitC-1 N8R8 | 8Mo | 8Mo | ✅ Validé |
| `esp32devkitc` | ESP32 Classic DevKitC | 4Mo | Aucune | ✅ **Corrigé** |

### Liste de Validation
- [x] Interface web se charge sur ESP32 Classic
- [x] Tous les onglets accessibles (Vue d'ensemble, Affichage & Signaux, Capteurs, Tests Matériels, Sans-fil, Benchmark, Export)
- [x] Changement de langue (FR/EN) fonctionne
- [x] Endpoints REST API répondent correctement
- [x] Aucune régression sur variantes ESP32-S3
- [x] Fragmentation mémoire réduite sur toutes les cartes

---

## Instructions de Mise à Jour

### Depuis v3.15.0
1. Récupérez le dernier code du dépôt
2. Vérifiez que `platformio.ini` affiche `PROJECT_VERSION="3.15.1"`
3. Nettoyez l'environnement de build :
   ```bash
   pio run -t clean
   ```
4. Compilez pour votre cible :
   ```bash
   # ESP32 Classic (correction critique s'applique ici)
   pio run -e esp32devkitc
   
   # Ou variantes ESP32-S3 (bénéficie de l'optimisation)
   pio run -e esp32s3_n16r8
   pio run -e esp32s3_n8r8
   ```
5. Téléversez le firmware :
   ```bash
   pio run --target upload -e <votre_environnement>
   ```

### Depuis Versions Antérieures
Suivez la procédure de mise à jour standard documentée dans [BUILD_AND_DEPLOY_FR.md](BUILD_AND_DEPLOY_FR.md).

---

## Changements Incompatibles
**Aucun.** Il s'agit d'une release patch rétrocompatible.

---

## Problèmes Connus
Aucun introduit par cette version. Voir [TROUBLESHOOTING_FR.md](TROUBLESHOOTING_FR.md) pour les problèmes connus généraux.

---

## Recommandations

### Pour Utilisateurs ESP32 Classic
- **Mettez à jour immédiatement** si vous rencontrez des problèmes de chargement de l'interface web
- Ce patch est critique pour un fonctionnement fiable sur cartes sans PSRAM

### Pour Utilisateurs ESP32-S3
- Mise à jour recommandée pour efficacité mémoire améliorée
- Aucun problème urgent adressé pour variantes S3, mais bénéfices d'optimisation s'appliquent

---

## Travaux Futurs

Bien que ce patch résolve la crise mémoire immédiate, des améliorations futures peuvent inclure :
- Compression gzip optionnelle pour assets JavaScript
- Service d'assets basé sur système de fichiers (LittleFS/SPIFFS)
- Capacités d'application web progressive (PWA) pour cache hors ligne
- Minification CSS/HTML supplémentaire

---

## Documentation Associée
- [CHANGELOG_FR.md](../CHANGELOG_FR.md) - Historique complet des versions
- [TROUBLESHOOTING_FR.md](TROUBLESHOOTING_FR.md) - Guides de diagnostic
- [ARCHITECTURE_FR.md](ARCHITECTURE_FR.md) - Internals du firmware

---

## Support
Si vous rencontrez des problèmes après mise à jour :
1. Vérifiez la sortie du moniteur série au démarrage
2. Vérifiez le heap libre via l'endpoint `/debug/status`
3. Signalez les problèmes à : https://github.com/morfredus/ESP32-Diagnostic/issues

---

**Version :** 3.15.1  
**Tag Git :** `v3.15.1`  
**Commit :** (à taguer après merge vers main)
