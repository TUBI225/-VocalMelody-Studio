# Registre des risques

| ID | Risque | Probabilité | Impact | Priorité | Prévention / détection | Solution de secours | Tests liés | Statut |
|---|---|---|---|---|---|---|---|---|
| R-001 | La stack C++20/JUCE ne peut pas être construite de façon reproductible sur Windows x64 | Moyenne | Élevé | Haute | Prototype minimal, build propre et CI dès T-001 | Réévaluer le framework via ADR | Build Debug/Release, CI | OUVERT |
| R-002 | Réduction trop précoce de l'incertitude acoustique, entraînant une perte de l'intention vocale | Moyenne | Critique | Critique | Contrats L0/L1/L2 versionnés, top-k et distributions conservés | Revenir aux données acoustiques et recalculer | Tests lattice top-k et intention | OUVERT |
| R-003 | Fichiers audio invalides, énormes ou malveillants | Moyenne | Élevé | Haute | Limites de taille/durée, validation de format, erreurs fermées | Rejet propre sans modification du projet | Corpus fichiers vides/corrompus | OUVERT |
| R-004 | Modèle ou dépendance incompatible avec la redistribution commerciale | Moyenne | Élevé | Haute | Audit de licence avant intégration et registre des versions | Remplacement par une alternative compatible | Revue de dépendances | OUVERT |
| R-005 | Cache ou projet persistant incompatible après changement de schéma | Moyenne | Élevé | Haute | Versionnage, migrations, écritures atomiques et sauvegarde | Invalidaton contrôlée du cache ; restauration projet | Tests migration/reprise | OUVERT |
| R-006 | Performances insuffisantes sans GPU dédié | Moyenne | Élevé | Haute | Budgets CPU/RAM/latence et benchmarks sur PC cible | Modes FAST/BALANCED/QUALITY et moteurs remplaçables | Benchmarks phase 17 | OUVERT |
| R-007 | Environnement local sans CMake, CTest ni compilateur C++ | Confirmée | Élevé | Critique | Détection préalable de la toolchain et documentation d'installation | Exécuter la CI Windows ou installer Visual Studio 2022 Build Tools + CMake | Configure, builds Debug/Release, CTest | OUVERT - BLOQUE LA VALIDATION LOCALE |
| R-008 | Distribution incompatible avec la licence JUCE choisie | Moyenne | Critique | Critique | Décision propriétaire explicite AGPLv3/commerciale avant test externe ou distribution | Suspendre la distribution ou remplacer JUCE | Revue licence avant release | OUVERT |
| R-009 | Compromission ou dérive d'une dépendance téléchargée pendant le build | Faible | Élevé | Haute | Commit et SHA-256 épinglés ; action CI épinglée ; permissions minimales | Miroir interne vérifié ou vendoring contrôlé | Revue reproductibilité du build | RÉDUIT, À SURVEILLER |

Un incident réellement rencontré doit également être ajouté à `SUIVI_DEVELOPPEMENT.md`.
