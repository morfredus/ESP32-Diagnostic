# Notes de version - Version 3.18.2

**Date de sortie** : 6 décembre 2025  
**Type** : Version correctif (patch)  
**Branche** : `dev/maint`

---

## Vue d'ensemble

La version 3.18.2 est une version correctif mineure qui ajoute des clés de traduction manquantes pour l'affichage du GPS et des capteurs environnementaux dans l'interface web. Cela garantit que tous les éléments de l'interface utilisateur disposent de traductions françaises et anglaises appropriées.

---

## Corrections

### Clés de traduction manquantes
Ajout de 4 clés de traduction manquantes pour une meilleure internationalisation :

1. **`gps_status`** (EN: "GPS Status" / FR: "Statut GPS")
   - Utilisée pour afficher le statut de connexion GPS dans l'interface sans fil
   
2. **`temperature_avg`** (EN: "Average Temperature" / FR: "Température moyenne")
   - Label pour la température moyenne lorsque plusieurs capteurs de température sont présents
   
3. **`pressure_hpa`** (EN: "Pressure (hPa)" / FR: "Pression (hPa)")
   - Label explicite pour la pression barométrique avec affichage de l'unité
   
4. **`altitude_calculated`** (EN: "Calculated Altitude" / FR: "Altitude calculée")
   - Label pour l'altitude calculée depuis la pression barométrique (capteur BMP280)

---

## Détails techniques

### Système de traduction
Toutes les clés de traduction sont définies dans `include/languages.h` en utilisant le système de macro `TEXT_RESOURCE_MAP` :

```cpp
X(gps_status, "GPS Status", "Statut GPS") \
X(temperature_avg, "Average Temperature", "Température moyenne") \
X(pressure_hpa, "Pressure (hPa)", "Pression (hPa)") \
X(altitude_calculated, "Calculated Altitude", "Altitude calculée") \
```

Ces clés sont accessibles via le namespace `Texts::` dans le code C++ et rendues dynamiquement dans l'interface web JavaScript en fonction de la langue sélectionnée.

---

## Notes de mise à jour

### De 3.18.1 vers 3.18.2
- **Aucune modification de code requise** - il s'agit uniquement d'une mise à jour de traduction
- **Aucune modification de configuration** - toutes les configurations existantes restent valides
- **Aucune modification d'API** - tous les points de terminaison API existants restent inchangés
- Recompilez et téléversez simplement le nouveau firmware

### Compatibilité
- ✅ Entièrement rétrocompatible avec 3.18.1 et 3.18.0
- ✅ Toutes les fonctionnalités et configurations existantes fonctionnent sans modification
- ✅ Aucune modification matérielle requise

---

## Mises à jour de la documentation

- Mise à jour de `CHANGELOG.md` avec l'entrée version 3.18.2
- Mise à jour de `CHANGELOG_FR.md` avec l'entrée version française 3.18.2
- Création de `RELEASE_NOTES_3.18.2.md` (version anglaise)
- Création de `RELEASE_NOTES_3.18.2_FR.md` (ce document)

---

## Informations de compilation

- **Version du firmware** : `3.18.2`
- **Version PlatformIO** : `6.12.0`
- **Framework** : Arduino (ESP32 v3.20017.241212)
- **Compilateur** : GCC 8.4.0 (xtensa-esp32s3)
- **Flag de compilation** : `-D PROJECT_VERSION='"3.18.2"'`

---

## Problèmes connus

Aucun problème identifié dans cette version. Tous les problèmes des versions précédentes restent tels que documentés dans leurs notes de version respectives.

---

## Crédits

- **Développement** : Équipe ESP32-Diagnostic
- **Tests** : Retours de la communauté
- **Traduction** : Révision bilingue complétée

---

## Prochaines étapes

Après la mise à jour vers 3.18.2 :
1. Vérifiez que le GPS et les capteurs environnementaux s'affichent correctement dans l'interface web
2. Testez le changement de langue (EN ↔ FR) pour confirmer que tous les labels sont traduits
3. Signalez toute chaîne non traduite restante pour inclusion dans les futurs correctifs

Pour des informations détaillées sur les fonctionnalités GPS et capteurs environnementaux, consultez :
- `RELEASE_NOTES_3.18.0_FR.md` - Implémentation initiale du GPS et des capteurs environnementaux
- `RELEASE_NOTES_3.18.1_FR.md` - Corrections de bugs du capteur AHT20
