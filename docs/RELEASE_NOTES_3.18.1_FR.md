# Notes de version - Version 3.18.1

**Date de sortie** : 6 décembre 2025  
**Type** : Version corrective (Correctif de bugs)

## Vue d'ensemble

La version 3.18.1 est une version corrective critique qui corrige les problèmes de lecture des données des capteurs introduits dans la version 3.18.0. Cette mise à jour résout les lectures incorrectes de température et d'humidité du capteur AHT20 et améliore l'API des capteurs environnementaux pour une meilleure compatibilité avec l'interface web.

## Corrections apportées

### Lecture des données du capteur AHT20 (Correction critique)
- **Problème** : Algorithme d'extraction de bits incorrect causant des valeurs de température et d'humidité erronées
- **Cause racine** : Les valeurs 20 bits n'étaient pas correctement extraites de la réponse 6 octets du capteur
- **Solution** : Implémentation de la manipulation correcte des bits pour extraire l'humidité des octets 0-2 et la température des octets 2-4
- **Impact** : Les lectures de température et d'humidité affichent maintenant des valeurs précises

### Structure de l'API des capteurs environnementaux
- **Problème** : L'interface web ne pouvait pas afficher correctement les données des capteurs en raison d'une structure JSON imbriquée
- **Solution** : Changement de la réponse API d'objets imbriqués vers une structure plate
- **Avant** : `{"aht20": {"available": true, ...}, "bmp280": {...}}`
- **Après** : `{"aht20_available": true, "bmp280_available": true, "temperature_avg": 23.5, ...}`

### Rapport de statut des capteurs
- **Problème** : Messages de statut de capteur peu clairs
- **Solution** : Implémentation de valeurs de statut explicites : "OK", "Erreur de lecture", "Non détecté"
- **Bénéfice** : Les utilisateurs peuvent maintenant identifier clairement les problèmes de capteurs

## Ajouts

### Clés de traduction
Ajout des clés de traduction FR/EN manquantes pour les éléments d'interface :
- `refresh_gps` - "Refresh GPS" / "Rafraîchir GPS"
- `gps_module` - "GPS Module" / "Module GPS"  
- `gps_module_desc` - Description du module GPS
- `refresh_env_sensors` - "Refresh Sensors" / "Rafraîchir capteurs"
- `test_env_sensors` - "Test Sensors" / "Tester capteurs"

## Notes de mise à niveau

### Depuis la version 3.18.0
- **Recommandé** : Ceci est un correctif de bug critique pour les lectures des capteurs environnementaux
- **Changements cassants** : Aucun - entièrement rétrocompatible
- **Action requise** : Téléverser le nouveau firmware pour corriger les lectures incorrectes des capteurs

### Depuis les versions antérieures
- Consultez les [notes de version 3.18.0](RELEASE_NOTES_3.18.0_FR.md) pour les informations sur les fonctionnalités GPS et capteurs environnementaux

## Détails techniques

### Format de données AHT20
Le capteur AHT20 retourne 6 octets :
```
Octet 0 : Octet de statut
Octets 1-2 : Bits d'humidité [19:4]
Octet 3 : Bits d'humidité [3:0] + Bits de température [19:16]
Octets 4-5 : Bits de température [15:0]
```

L'algorithme d'extraction corrigé :
```cpp
uint32_t humidity_raw = ((uint32_t)data[0] << 12) | ((uint32_t)data[1] << 4) | ((uint32_t)data[2] >> 4);
uint32_t temp_raw = (((uint32_t)data[2] & 0x0F) << 16) | ((uint32_t)data[3] << 8) | (uint32_t)data[4];
```

## Problèmes connus

Aucun spécifique à cette version. Consultez la [FAQ](FAQ_FR.md) pour le dépannage général.

## Prochaines étapes

Après la mise à niveau :
1. Naviguez vers la page "Capteurs" dans l'interface web
2. Vérifiez que les capteurs AHT20 et BMP280 affichent le statut "OK"
3. Vérifiez que les valeurs de température et d'humidité sont maintenant précises
4. Testez les boutons "Rafraîchir capteurs" et "Tester capteurs"

## Support

- Documentation : Voir le dossier [docs/](../docs/)
- Problèmes : Signaler sur le dépôt GitHub
- Compatibilité : Cartes ESP32-S3 et ESP32 Classic

---

**Version** : 3.18.1  
**Build** : 6 décembre 2025  
**Versioning sémantique** : MAJEUR.MINEUR.PATCH (3.18.1)
