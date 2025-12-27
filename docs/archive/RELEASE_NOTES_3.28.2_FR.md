# ESP32 Diagnostic Suite – Notes de Version v3.28.2

**Date de sortie :** 24 décembre 2025
**Type :** Patch d'urgence
**Sévérité :** Correction de bug critique

---

## 📋 Vue d'ensemble

La version 3.28.2 est un **patch d'urgence** qui corrige l'erreur JavaScript BUTTON_BOOT persistante dans les versions 3.28.0 et 3.28.1 malgré les tentatives de correction précédentes. Cette version identifie et résout correctement la cause racine en injectant les constantes GPIO au bon endroit.

**Qui doit mettre à jour :**
- ✅ TOUS les utilisateurs sur v3.28.0 ou v3.28.1 (correctif incomplet)
- ✅ Utilisateurs rencontrant "ReferenceError: BUTTON_BOOT is not defined" dans la console navigateur
- ✅ Utilisateurs incapables d'accéder à la page Dispositifs d'Entrée de l'interface web

---

## 🐛 Problèmes corrigés

### ReferenceError JavaScript BUTTON_BOOT ✅ (VRAIMENT CORRIGÉ MAINTENANT)

#### Description du problème
Lors de la navigation vers la page "Dispositifs d'Entrée" de l'interface web, la console du navigateur affichait :
```
ReferenceError: BUTTON_BOOT is not defined
```

Cette erreur :
- Empêchait le chargement correct de la page Dispositifs d'Entrée
- Rendait les contrôles BUTTON_BOOT, BUTTON_1 et BUTTON_2 non fonctionnels
- Persistait malgré les tentatives de correction en v3.28.0 et v3.28.1

#### Cause racine
Les constantes GPIO (`BUTTON_BOOT`, `BUTTON_1`, `BUTTON_2`, `TFT_MISO_PIN`) étaient injectées au **mauvais endroit** :

1. **Approche v3.28.0/3.28.1 (INCORRECTE) :**
   - Les constantes étaient ajoutées à `web_interface.h::generateJavaScript()`
   - **MAIS** cette fonction est uniquement appelée pour calculer la taille JavaScript pour les statistiques
   - Le JavaScript réellement servi au navigateur provient de `main.cpp::handleJavaScriptRoute()`
   - Par conséquent, les constantes injectées **n'atteignaient jamais le navigateur**

2. **Solution v3.28.2 (CORRECTE) :**
   - Les constantes sont maintenant injectées dans `main.cpp::handleJavaScriptRoute()`
   - C'est la fonction qui génère réellement le JavaScript pour le navigateur
   - Les constantes sont maintenant correctement disponibles avant toute exécution de fonctions JavaScript

#### Solution technique
**Fichier :** `src/main.cpp`
**Lignes :** 5397-5405

```cpp
// Button pins (v3.28.2 fix - ajouté à handleJavaScriptRoute)
pinVars += ";const BUTTON_BOOT=";
pinVars += String(BUTTON_BOOT);
pinVars += ";const BUTTON_1=";
pinVars += String(BUTTON_1);
pinVars += ";const BUTTON_2=";
pinVars += String(BUTTON_2);

// TFT MISO pin
pinVars += ";const TFT_MISO_PIN=";
pinVars += String(TFT_MISO);
```

#### Impact et bénéfices
- ✅ La page Dispositifs d'Entrée se charge maintenant **SANS** erreurs JavaScript
- ✅ BUTTON_BOOT s'affiche correctement en **lecture seule GPIO 0**
- ✅ BUTTON_1 et BUTTON_2 sont entièrement fonctionnels
- ✅ Toutes les constantes GPIO correctement injectées **avant** l'exécution JavaScript
- ✅ La console du navigateur n'affiche **aucune ReferenceError**

---

## 📦 Fichiers modifiés

| Fichier | Changements | Lignes |
|---------|-------------|--------|
| `src/main.cpp` | Ajout des constantes boutons/TFT MISO à pinVars dans handleJavaScriptRoute() | 5397-5415 |
| `platformio.ini` | Changement de version : 3.28.1 → 3.28.2 | 27 |

---

## ✅ Tests et vérification

### Comment vérifier le correctif

1. **Flasher le firmware v3.28.2** sur votre carte ESP32
2. **Se connecter à l'interface web** via http://esp32.local ou l'adresse IP
3. **Naviguer** vers la page "Dispositifs d'Entrée" via le menu
4. **Vérifier** que la page se charge sans erreurs
5. **Ouvrir la console du navigateur** (F12) et vérifier qu'il n'y a AUCUN message ReferenceError
6. **Vérifier** que BUTTON_BOOT affiche "GPIO 0 (non configurable)"
7. **Tester** les fonctions de monitoring BUTTON_1 et BUTTON_2

### Résultats attendus
✅ La page Dispositifs d'Entrée se charge complètement
✅ Le champ BUTTON_BOOT affiche "GPIO 0 (non configurable)"
✅ La console du navigateur n'affiche AUCUNE erreur JavaScript
✅ Le monitoring des boutons fonctionne en cliquant sur "Monitor Button"

---

## 📌 Compatibilité

### Cartes supportées
- ✅ ESP32-S3 DevKitC-1 N16R8 (16MB Flash, 8MB PSRAM)
- ✅ ESP32-S3 DevKitC-1 N8R8 (8MB Flash, 8MB PSRAM)
- ✅ ESP32 Classic DevKitC (4MB Flash)

### Prérequis
- **PlatformIO :** espressif32@^6
- **ESP32 Arduino Core :** 3.0.0+
- **Navigateur :** Chrome 90+, Firefox 88+, Edge 90+, Safari 14+

### Changements incompatibles
❌ Aucun – Ceci est une version de correction de bug, entièrement rétrocompatible

---

## 🔄 Guide de migration

### Mise à niveau depuis v3.28.0 ou v3.28.1

**Étape 1 :** Récupérer le dernier code
```bash
git pull origin dev/addfeatures
```

**Étape 2 :** Compilation propre (recommandé)
```bash
pio run -t clean
pio run -e esp32s3_n16r8
```

**Étape 3 :** Téléverser vers l'ESP32
```bash
pio run -e esp32s3_n16r8 --target upload
```

**Étape 4 :** Vérifier l'interface web
- Naviguer vers http://esp32.local
- Aller à la page "Dispositifs d'Entrée"
- Confirmer qu'il n'y a pas d'erreurs JavaScript dans la console

### Mise à niveau depuis v3.27.x ou antérieure

Suivre la procédure de mise à niveau standard :
1. Consulter `docs/RELEASE_NOTES_3.28.0_FR.md` pour les changements v3.28.0
2. Consulter `docs/RELEASE_NOTES_3.28.1_FR.md` pour les changements v3.28.1
3. Consulter ce document pour les changements v3.28.2
4. Flasher le firmware avec PlatformIO

---

## 🔗 Documentation associée

- [CHANGELOG_FR.md](../CHANGELOG_FR.md) – Historique complet des versions
- [RELEASE_NOTES_3.28.0_FR.md](RELEASE_NOTES_3.28.0_FR.md) – v3.28.0 Tentative initiale de correction BUTTON_BOOT
- [RELEASE_NOTES_3.28.1_FR.md](RELEASE_NOTES_3.28.1_FR.md) – v3.28.1 Corrections MISO & carte SD
- [TROUBLESHOOTING_FR.md](TROUBLESHOOTING_FR.md) – Problèmes courants et solutions
- [PIN_MAPPING_FR.md](PIN_MAPPING_FR.md) – Assignation des broches GPIO

---

## ❓ FAQ

### Q : Dois-je sauter les v3.28.0 et v3.28.1 ?
**R :** Oui, mettez à niveau directement vers v3.28.2. Les versions 3.28.0 et 3.28.1 contiennent un correctif incomplet pour l'erreur BUTTON_BOOT.

### Q : Ce correctif affectera-t-il mon câblage matériel existant ?
**R :** Non, il s'agit d'un correctif logiciel uniquement. Aucun changement matériel requis.

### Q : Que faire si je vois toujours des erreurs JavaScript après la mise à niveau ?
**R :** Videz le cache de votre navigateur (Ctrl+Maj+Suppr) et rechargez la page. Si les erreurs persistent, vérifiez la console du navigateur pour des messages d'erreur spécifiques et signalez-les via GitHub Issues.

### Q : Ce correctif s'applique-t-il aux ESP32-S3 et ESP32 Classic ?
**R :** Oui, le correctif s'applique à toutes les variantes de cartes supportées.

---

## 📝 Crédits

**Signalé par :** Tests communautaires
**Corrigé par :** Équipe de développement ESP32-Diagnostic
**Testé sur :** ESP32-S3 DevKitC-1 N16R8, ESP32-S3 N8R8, ESP32 Classic DevKitC

---

## 🔖 Informations de version

- **Version actuelle :** 3.28.2
- **Version précédente :** 3.28.1
- **Branche de release :** `dev/addfeatures`
- **Tag :** `v3.28.2`

---

**Pour le journal des modifications complet, voir [CHANGELOG_FR.md](../CHANGELOG_FR.md)**
