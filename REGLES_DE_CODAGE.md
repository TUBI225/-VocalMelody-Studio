# Règles de codage

## Principes

- Respecter C++20 pour le code applicatif une fois ADR-002 acceptée.
- Garder les modules faiblement couplés et faire circuler des structures de données explicites et versionnées.
- Préserver les distributions, alternatives et niveaux de confiance ; ne pas réduire prématurément l'incertitude.
- Éviter les états globaux mutables et rendre les dépendances visibles.
- Une fonction ou classe doit avoir une responsabilité claire.
- Ne pas dupliquer une fonction existante ; rechercher avant d'ajouter.
- Limiter chaque modification à la tâche active.

## Nommage et organisation

- Fichiers C++ : `PascalCase.h` et `PascalCase.cpp`.
- Classes et structures : `PascalCase`.
- Fonctions, méthodes et variables : `camelCase`.
- Constantes : `kPascalCase`.
- Tests : nommer le comportement et le résultat attendu.
- Les interfaces publiques sont placées séparément des détails d'implémentation.

## Erreurs, sécurité et journalisation

- Valider toute entrée externe avant traitement, notamment formats audio, chemins et données persistantes.
- Retourner une erreur explicite ; ne jamais continuer avec des données possiblement corrompues.
- Ne pas marquer `noexcept` une fonction qui alloue sans intercepter les exceptions ; une frontière d'import doit convertir les échecs de codec ou d'allocation en erreur contrôlée.
- Ne pas journaliser de secrets ni de contenu utilisateur inutile.
- Les opérations longues et fichiers temporaires doivent prévoir nettoyage, interruption et reprise.

## Qualité

- Aucun statut de réussite sans commande et résultat de test enregistrés.
- Ajouter des tests unitaires pour la logique et des tests d'intégration aux frontières de modules.
- Formatage C++ : `clang-format` selon `.clang-format`, style LLVM, indentation 4 espaces, largeur 100.
- Build : CMake 3.24 ou supérieur et presets Windows x64.
- Tests : CTest ; un exécutable de test doit retourner un code non nul dès qu'une assertion échoue.
- Avertissements : `/W4 /permissive- /Zc:__cplusplus` avec MSVC ; `-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion` avec Clang/GCC.
- La CI traite les avertissements comme des erreurs ; ce mode reste optionnel localement.
- Les tests ne doivent pas dépendre de JUCE lorsqu'ils vérifient uniquement le domaine commun.
- Toute nouvelle convention doit faire l'objet d'une mise à jour de ce document.
- Un sérialiseur JSON doit produire des jetons valides pour toute entrée ; NaN et les infinis sont rejetés ou sérialisés en `null`.
- Un format audio est « pris en charge » uniquement après test du codec réellement activé sur la plateforme cible ; une extension dans l'interface ne constitue pas une preuve.
- Toute modification de l'algorithme ou du sample rate qui change la signification d'un résultat persistant doit incrémenter `analysisVersion` et ajouter un test de sérialisation.

## Règles CMake et dépendances

- Épingler les dépendances à une version ou un commit immuable.
- Vérifier l'empreinte des archives téléchargées.
- Documenter la licence et la source exacte de tout codec tiers ; une garantie juridique absolue ne doit pas être déduite de cette seule vérification.
- Les tests de codec doivent décoder au moins un flux réel et contrôler le signal produit ; un simple en-tête fabriqué ne prouve pas le support du format.
- Ne pas ajouter une dépendance de test lorsque CTest et la bibliothèque standard suffisent.
- Les options de build du projet portent le préfixe `VOCALMELODY_`.
- Les cibles internes sont exposées avec l'espace de noms `VocalMelody::` lorsque pertinent.
