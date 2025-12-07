# Notes de Version - Version 3.20.3

**Date de Sortie** : 8 décembre 2025  
**Type** : Version Corrective - Optimisation du Code

## Vue d'ensemble

La version 3.20.3 est une version d'optimisation du code qui se concentre exclusivement sur les améliorations d'efficacité mémoire. Cette version applique 9 optimisations systématiques qui éliminent plus de 90 allocations de chaînes dans l'ensemble du code sans modifier aucune fonctionnalité, pin ou comportement matériel.

## Nouveautés

### Optimisation du Code ([OPT-001] à [OPT-009])

**Améliorations de l'Efficacité Mémoire :**
- **90+ allocations de chaînes éliminées** via des approches unifiées basées sur des buffers
- **13 messages de debug/statut** convertis de la concaténation String au formatage snprintf
- **30+ affectations de résultats de tests** utilisent maintenant des constantes pré-allouées
- **Construction de liste GPIO en O(1)** remplaçant le modèle d'allocation O(n)

**Optimisations Spécifiques :**

1. **[OPT-001] Formatage de Chaîne de Version**
   - Passage de 11 allocations à 1 en utilisant un buffer snprintf
   - Emplacement : `getArduinoCoreVersionString()`

2. **[OPT-002] Formatage du Temps de Fonctionnement**
   - Passage de 4-6 allocations à 1 en utilisant une approche basée sur buffer
   - Emplacement : `formatUptime()`

3. **[OPT-003] Déclarations Extern**
   - Réorganisées avec documentation inline pour plus de clarté
   - Emplacement : `include/web_interface.h`

4. **[OPT-004] Constante de Résultat de Test**
   - Création de `DEFAULT_TEST_RESULT_STR` utilisée dans 10+ emplacements d'initialisation
   - Élimine les allocations répétées de `String(Texts::not_tested)`

5. **[OPT-005] Construction de Liste GPIO**
   - Passage d'une boucle String += O(n) à un seul buffer snprintf
   - Emplacement : `getGPIOList()`

6. **[OPT-006] Fonctionnalités du Chip**
   - Élimination des opérations de sous-chaînes via une approche basée sur buffer
   - Emplacement : `getChipFeatures()`

7. **[OPT-007] Formatage de Messages**
   - Conversion de 13 messages de debug/statut en snprintf (2-9 allocations → 1 chacun)
   - Emplacements : messages de config, statut WiFi, titre HTML, étapes OLED

8. **[OPT-008] Utilisation de TextField dans formatUptime**
   - Appels directs `.str().c_str()` évitent les allocations String inutiles (3 instances)
   - Emplacement : formatage jours/heures/minutes dans `formatUptime()`

9. **[OPT-009] Constantes de Statut de Test**
   - Création des constantes `OK_STR` et `FAIL_STR`
   - Appliquées à 30+ emplacements : tests GPIO, résultats capteurs, démos LED, gestionnaires config

## Détails Techniques

### Évaluation de l'Impact
- **Aucun changement fonctionnel** : Tous les pins, tests et fonctionnalités restent identiques
- **Aucun changement d'API** : Le comportement externe est inchangé
- **Rétrocompatible** : Remplacement direct pour v3.20.2
- **Efficacité d'exécution** : Réduction des allocations heap améliore la stabilité mémoire

### Tests
- ✅ Compilé avec succès sur **ESP32-S3** (esp32s3_n16r8) - Temps de build : 52s
- ✅ Compilé avec succès sur **ESP32 CLASSIC** (esp32devkitc) - Temps de build : 25s
- ✅ Toutes les optimisations marquées avec des commentaires inline `[OPT-###]` pour traçabilité

### Fichiers Modifiés
- `src/main.cpp` - 224 insertions, 97 suppressions (cible principale d'optimisation)
- Toute la documentation mise à jour vers v3.20.3

## Instructions de Mise à Niveau

### De v3.20.2 vers v3.20.3
Il s'agit d'une mise à niveau transparente sans changement de configuration requis :

1. Récupérez le dernier code depuis la branche `dev/maint` ou `main`
2. Compilez et téléversez normalement
3. Aucun changement de fichier de configuration nécessaire
4. Aucun changement d'interface web requis

### Vérification
Après le téléversement, le firmware fonctionnera de manière identique à v3.20.2 mais avec une efficacité mémoire améliorée. Vous pouvez vérifier la version par :
- Vérification de l'en-tête de l'interface web
- Consultation de la sortie Serial au démarrage
- Accès au point de terminaison `/api/version`

## Changements Incompatibles
**Aucun** - Il s'agit d'une version corrective entièrement rétrocompatible.

## Problèmes Connus
Aucun introduit dans cette version. Tous les problèmes de v3.20.2 restent inchangés.

## Contributeurs
- Optimisation du code et implémentation : GitHub Copilot
- Tests et validation : morfredus

## Prochaines Étapes
La version 3.20.3 complète l'initiative d'optimisation mémoire. Les versions futures se concentreront sur :
- Améliorations de fonctionnalités
- Support de capteurs additionnels
- Améliorations de l'interface web

---

**Journal Complet des Modifications** : [CHANGELOG_FR.md](../CHANGELOG_FR.md)  
**Documentation** : [docs/](../docs/)  
**GitHub** : https://github.com/morfredus/ESP32-Diagnostic
