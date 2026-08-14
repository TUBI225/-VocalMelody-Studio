# Suivi du développement

Ce journal fonctionne en mode « ajouter sans effacer ».

# 2026-08-14 - 02:32 UTC - T-000 - Initialiser la documentation permanente

## Objectif

Créer dans le dépôt les 13 fichiers Markdown imposés par les instructions permanentes et intégrer les documents de conception fournis.

## État avant intervention

Le dépôt distant ne contenait qu'un `README.md` minimal et `.gitignore`. Aucun des 13 documents permanents n'existait.

## Travail effectué

- Clonage du dépôt `TUBI225/-VocalMelody-Studio` dans le workspace.
- Lecture textuelle et contrôle visuel du PDF de 28 pages.
- Création des 13 documents permanents à la racine.
- Intégration sans réduction des cahiers fournis dans les documents correspondants.
- Conservation du cahier logique VIRE dans `docs/`.
- Initialisation des décisions, règles, dépendances candidates, risques, sécurité, reprise, performances et état courant.
- Mise à jour du README avec l'ordre de lecture documentaire.

## Fichiers créés

- `Cahier_des_charges.md`
- `FEUILLE_DE_ROUTE.md`
- `SUIVI_DEVELOPPEMENT.md`
- `ARCHITECTURE_TECHNIQUE.md`
- `REGISTRE_DES_RISQUES.md`
- `PROTOCOLE_TEST_REPRISE.md`
- `ETAT_ACTUEL_PROJET.md`
- `DECISIONS_ARCHITECTURE.md`
- `REGLES_DE_CODAGE.md`
- `DEPENDANCES.md`
- `MODELISATION_DONNEES.md`
- `SECURITE.md`
- `PERFORMANCES.md`
- `docs/CAHIER_LOGIQUE_MOTEUR_VIRE.md`

## Fichiers modifiés

- `README.md`

## Fichiers supprimés

- Aucun.

## Décisions prises

- Les quatre cahiers ayant un rôle permanent ont été utilisés comme contenu initial intégral de leur document Markdown correspondant.
- Le cahier logique VIRE est conservé comme référence spécialisée sans remplacer l'architecture ou la modélisation.
- Les choix techniques non encore prouvés sont marqués PROPOSÉS et non installés.

## Problèmes rencontrés

- Git n'était pas disponible dans le PATH système.
- Une première assertion de clôture a échoué parce qu'un caractère accentué injecté dans le script PowerShell a été remplacé par `?` avant comparaison ; le contenu du fichier était correct.

## Solutions appliquées

- Utilisation de l'exécutable Git fourni par l'environnement Codex.
- Nouvelle vérification fondée sur une lecture UTF-8 du fichier et un prédicat ASCII indépendant de l'encodage de la console ; vérification réussie.

## Tests exécutés

- Clonage Git et lecture du commit courant : RÉUSSI.
- Extraction du texte des 28 pages du PDF : RÉUSSI.
- Rendu PNG des 28 pages et contrôle visuel par planches : RÉUSSI.
- Contrôle automatisé de présence/non-vacuité et encodage UTF-8 des 13 documents : RÉUSSI (13/13).
- Contrôle d'intégrité SHA-256 des quatre cahiers copiés sans modification : RÉUSSI (4/4 identiques).
- Vérification `git diff --check` : RÉUSSIE (aucune erreur ; avertissement informatif LF/CRLF sur README).
- Tests applicatifs : NON EXÉCUTÉS. Aucun code applicatif n'existe.

## Résultats

Les 13 documents demandés existent, sont non vides et décodables en UTF-8. Les quatre cahiers permanents copiés sans adaptation sont identiques octet par octet à leurs sources ; le plan de développement a été conservé puis complété avec T-000 et T-001. L'état Git ne contient que les créations attendues et la modification du README.

## Risques découverts

Les risques initiaux R-001 à R-006 ont été enregistrés.

## État final de la tâche

TERMINÉ.

## Travail restant

- Aucun travail restant pour T-000.

## Prochaine action

Démarrer T-001 : valider et créer le socle C++20/JUCE/CMake.

## Commit associé

Aucun commit créé.

## Contrôle documentaire

| Document | État | Action |
|---|---|---|
| Cahier_des_charges.md | MIS À JOUR | Source fournie intégrée |
| FEUILLE_DE_ROUTE.md | MIS À JOUR | Plan fourni intégré ; T-000 et T-001 ajoutées |
| SUIVI_DEVELOPPEMENT.md | MIS À JOUR | Entrée T-000 ajoutée |
| ARCHITECTURE_TECHNIQUE.md | MIS À JOUR | Source fournie intégrée |
| REGISTRE_DES_RISQUES.md | MIS À JOUR | Registre initial créé |
| PROTOCOLE_TEST_REPRISE.md | MIS À JOUR | Scénarios initiaux non exécutés créés |
| ETAT_ACTUEL_PROJET.md | MIS À JOUR | État initial créé |
| DECISIONS_ARCHITECTURE.md | MIS À JOUR | ADR initiales consignées |
| REGLES_DE_CODAGE.md | MIS À JOUR | Règles initiales créées |
| DEPENDANCES.md | MIS À JOUR | Candidats distingués des dépendances installées |
| MODELISATION_DONNEES.md | MIS À JOUR | Source fournie intégrée |
| SECURITE.md | MIS À JOUR | Menaces et règles initiales créées |
| PERFORMANCES.md | MIS À JOUR | Objectifs initiaux et mesures inconnues consignés |

# 2026-08-14 - 04:13 UTC - T-001 - Créer le socle C++20 / JUCE / CMake

## Objectif

Lancer la première étape technique définie par les cinq cahiers : créer une arborescence stable, un build minimal C++20/JUCE/CMake, un premier test automatisé et une CI Windows, sans commencer prématurément l'Audio Frontend.

## État avant intervention

Le dépôt contenait la documentation permanente mais aucun fichier source, aucune configuration de build, aucun test et aucune CI. ADR-002 était proposée. La disponibilité locale de CMake et du compilateur n'avait pas été vérifiée.

## Travail effectué

- Lecture des documents permanents dans l'ordre imposé et extraction des exigences de phase 0.
- Vérification des versions et licences officielles de JUCE.
- Sélection de JUCE 8.0.15 et épinglage du commit et de l'empreinte SHA-256.
- Création du build CMake 3.24+ et des presets Windows x64 Debug/Release.
- Création de la bibliothèque commune indépendante de JUCE.
- Implémentation des types forts `Seconds`, `Beats`, `Probability` et `Score01`.
- Création d'une coquille d'application JUCE avec fenêtre principale minimale.
- Création d'un test unitaire CTest sans framework externe.
- Configuration des avertissements MSVC/Clang/GCC et de clang-format.
- Création d'une CI GitHub Actions Windows Debug/Release avec permissions minimales.
- Épinglage de `actions/checkout` par SHA de commit.
- Mise à jour des documents concernés selon leur rôle.

## Fichiers créés

- `CMakeLists.txt`
- `CMakePresets.json`
- `cmake/VocalMelodyWarnings.cmake`
- `src/common/CMakeLists.txt`
- `src/common/include/vocalmelody/common/StrongTypes.h`
- `src/common/include/vocalmelody/common/Version.h`
- `src/app/CMakeLists.txt`
- `src/app/Main.cpp`
- `src/app/MainComponent.h`
- `src/app/MainComponent.cpp`
- `tests/CMakeLists.txt`
- `tests/unit/StrongTypesTests.cpp`
- `.github/workflows/ci.yml`
- `.clang-format`
- `.editorconfig`

## Fichiers modifiés

- `README.md`
- `FEUILLE_DE_ROUTE.md`
- `SUIVI_DEVELOPPEMENT.md`
- `ARCHITECTURE_TECHNIQUE.md`
- `REGISTRE_DES_RISQUES.md`
- `PROTOCOLE_TEST_REPRISE.md`
- `ETAT_ACTUEL_PROJET.md`
- `DECISIONS_ARCHITECTURE.md`
- `REGLES_DE_CODAGE.md`
- `DEPENDANCES.md`
- `MODELISATION_DONNEES.md`
- `SECURITE.md`
- `PERFORMANCES.md`

## Fichiers supprimés

- Aucun.

## Décisions prises

- ADR-002 est acceptée techniquement pour C++20/JUCE, mais ne vaut pas choix de licence de distribution.
- JUCE reste isolé dans la couche application ; le domaine commun n'en dépend pas.
- JUCE est acquis par archive immuable contrôlée par SHA-256, conformément à ADR-004.
- Les premiers tests utilisent uniquement CTest et la bibliothèque standard afin d'éviter une dépendance de test inutile.
- L'Audio Frontend n'est pas commencé tant que le socle n'est pas réellement compilé et testé.

## Problèmes rencontrés

- CMake, CTest, MSVC, Clang, GCC, Ninja, clang-format et clang-tidy sont absents de la machine locale.
- PyYAML était absent ; la validation YAML Python a été ignorée.
- La tentative de validation YAML avec Node a échoué car le module `yaml` était absent.
- Une première recherche textuelle de signature JUCE a échoué car le test attendait le nom de paramètre `fontStyleFlags` alors que JUCE utilise `styleFlags` ; le constructeur lui-même existait.
- `git diff --check` signale que Git convertira potentiellement README de LF vers CRLF ; aucune erreur de whitespace n'est signalée.

## Solutions appliquées

- Création d'une CI Windows Debug/Release destinée à fournir une validation reproductible après commit/push.
- Conservation du statut PARTIEL tant que les builds et tests réels ne sont pas exécutés.
- Validation statique limitée au JSON, à l'arborescence, aux empreintes de dépendance et à `git diff --check`.
- Documentation explicite de la toolchain manquante et de la prochaine commande à exécuter.
- Inspection de l'en-tête officiel JUCE 8.0.15 puis correction du contrôle de signature sans modifier l'API utilisée par l'application.

## Tests exécutés

- Recherche de toolchain locale : EXÉCUTÉE - 8 outils recherchés, 8 absents.
- Validation JSON de `CMakePresets.json` : RÉUSSIE - 1 fichier, 1 réussite, 0 échec.
- Contrôle de l'arborescence : RÉUSSI - 14 fichiers attendus, 14 présents, 0 manquant.
- Vérification de l'archive JUCE : RÉUSSIE - 22 896 965 octets, SHA-256 conforme.
- Contrôle initial des signatures JUCE : ÉCHEC DE CONTRÔLE - 1 signature trouvée, 1 recherche trop stricte échouée.
- Nouveau contrôle des signatures JUCE utilisées : RÉUSSI - 4 signatures sur 4 trouvées dans les en-têtes du commit épinglé.
- Vérification `git diff --check` : RÉUSSIE - 0 erreur, 1 avertissement LF/CRLF.
- Contrôle final des documents : RÉUSSI - 13/13 présents, non vides et UTF-8 valides.
- Conservation des cinq cahiers sources : RÉUSSIE - 5/5 contenus originaux préservés comme préfixe exact.
- Contrôle de la table documentaire : RÉUSSI - 13/13 documents référencés.
- Cohérence du statut T-001 : RÉUSSIE - PARTIEL dans la feuille de route et le suivi.
- Validation YAML avec PyYAML : NON EXÉCUTÉE - dépendance de validation absente.
- Validation YAML avec Node : ÉCHEC - module `yaml` introuvable ; le workflow n'a pas été exécuté.
- Configuration CMake : NON EXÉCUTÉE - CMake absent. Résultat inconnu.
- Build Debug : NON EXÉCUTÉ - compilateur absent. Résultat inconnu.
- CTest Debug : NON EXÉCUTÉ - aucun binaire produit. Résultat inconnu.
- Build Release : NON EXÉCUTÉ - compilateur absent. Résultat inconnu.
- CTest Release : NON EXÉCUTÉ - aucun binaire produit. Résultat inconnu.
- GitHub Actions : NON EXÉCUTÉE - changements non commités et non poussés. Résultat inconnu.

Environnement : Windows 11 Professionnel 10.0.26200, Intel Core i5-7300U, 4 processeurs logiques, 15,8 Go RAM, 2026-08-14.

## Résultats

Le socle source, les contrats minimaux, le test source et la CI sont présents. Aucune affirmation de compilation, d'exécution ou de CI verte n'est possible. T-001 reste PARTIEL.

## Risques découverts

- R-007 : toolchain locale absente, validation locale bloquée.
- R-008 : régime de licence JUCE à décider avant distribution.
- R-009 : dépendance réseau du premier build, réduite par l'épinglage et le SHA-256.

## État final de la tâche

PARTIEL.

## Travail restant

- Installer ou fournir Visual Studio 2022 Build Tools avec C++ et CMake.
- Exécuter configuration, builds et tests Debug/Release.
- Exécuter la CI et corriger tout échec constaté.
- Choisir AGPLv3 ou licence commerciale JUCE avant toute distribution.

## Prochaine action

Fournir la toolchain, exécuter `cmake --preset windows-debug`, puis construire et lancer `ctest --preset windows-debug` avant toute phase Audio Frontend.

## Commit associé

Aucun commit créé.

## Contrôle documentaire

| Document | État | Action |
|---|---|---|
| Cahier_des_charges.md | VÉRIFIÉ - NON CONCERNÉ | Aucun comportement fonctionnel ajouté ou modifié |
| FEUILLE_DE_ROUTE.md | MIS À JOUR | T-001 passée à PARTIEL avec preuves et reste à faire |
| SUIVI_DEVELOPPEMENT.md | MIS À JOUR | Entrée T-001 ajoutée sans effacer T-000 |
| ARCHITECTURE_TECHNIQUE.md | MIS À JOUR | Arborescence et responsabilités réelles ajoutées |
| REGISTRE_DES_RISQUES.md | MIS À JOUR | R-007 à R-009 ajoutés |
| PROTOCOLE_TEST_REPRISE.md | MIS À JOUR | Contrôle T-001 non concerné consigné |
| ETAT_ACTUEL_PROJET.md | MIS À JOUR | Version 0.1.0 et état PARTIEL consignés |
| DECISIONS_ARCHITECTURE.md | MIS À JOUR | ADR-002 précisée et ADR-004 ajoutée |
| REGLES_DE_CODAGE.md | MIS À JOUR | CMake, CTest, formatage et avertissements documentés |
| DEPENDANCES.md | MIS À JOUR | Dépendances actives, versions, licences et outils absents enregistrés |
| MODELISATION_DONNEES.md | MIS À JOUR | Types forts implémentés et types restants documentés |
| SECURITE.md | MIS À JOUR | Chaîne d'approvisionnement et permissions CI documentées |
| PERFORMANCES.md | MIS À JOUR | Environnement et mesures non exécutées consignés |

# 2026-08-14 - T-001.1 - Corriger les deux blocages de configuration CMake

## Objectif

Lever les deux blocages de build identifiés par un audit interne du socle, afin que `cmake --preset windows-debug` soit configurable dès que la toolchain sera installée.

## État avant intervention

- `CMakeLists.txt` déclarait `LANGUAGES CXX` sans le langage `C`, alors que JUCE 8.0.15 (commit épinglé) émet `FATAL_ERROR` si `"C"` est absent des langages globaux initiaux (`A C compiler is required to build targets that depend on JUCE. Add 'C' to your project's LANGUAGES.`). Toute configuration avec `VOCALMELODY_BUILD_APP=ON` (presets et CI) était donc vouée à l'échec.
- `CMakePresets.json` déclarait le schéma `"version": 6`, qui exige CMake >= 3.25, alors que `cmakeMinimumRequired` et le README annoncent CMake 3.24.

## Travail effectué

- `CMakeLists.txt` : `LANGUAGES CXX` remplacé par `LANGUAGES C CXX`.
- `CMakePresets.json` : `"version": 6` remplacé par `"version": 5` (schéma compatible CMake 3.24, conformément au tableau officiel des versions de schéma).

## Fichiers modifiés

- `CMakeLists.txt`
- `CMakePresets.json`

## Décisions prises

- Le schéma 5 est retenu car le contenu du fichier n'utilise aucune fonctionnalité du schéma 6 et la cible annoncée reste CMake >= 3.24.
- Aucune autre modification du socle n'est apportée dans cette passe.

## Tests exécutés

- Validation JSON de `CMakePresets.json` : RÉUSSIE - version=5, cmakeMinimumRequired=3.24.0.
- Lecture de contrôle des deux fichiers : RÉUSSIE - `LANGUAGES C CXX` et `"version": 5` confirmés.
- Configuration CMake, builds et CTest : NON EXÉCUTÉS - toolchain toujours absente localement.

## Résultats

Les deux blocages de configuration sont corrigés et validés statiquement. Aucun build n'a encore pu être exécuté faute de CMake, de compilateur C++ et de Git sur la machine.

## État final de la tâche

T-001 reste PARTIEL.

## Travail restant

- Installer Visual Studio 2022 Build Tools (charge « Développement Desktop en C++ »), CMake et Git.
- Exécuter configuration, builds et tests Debug/Release.
- Exécuter la CI (branche de travail puis Pull Request).
- Marquer T-001 TERMINÉ uniquement après réussite des builds, des tests et de la CI.

## Prochaine action

Installer la toolchain, puis exécuter dans l'ordre : `cmake --preset windows-debug`, `cmake --build --preset windows-debug --parallel`, `ctest --preset windows-debug`, puis l'équivalent Release.


# 2026-08-14 - T-001.2 - Valider les blocages corrigés et reformater le code

## Objectif

Vérifier réellement les corrections de configuration (T-001.1) dès que la toolchain est disponible, et appliquer la règle de formatage `clang-format`.

## État avant intervention

- La toolchain a été installée entre-temps : CMake 4.4.2, CTest 4.4.2, Visual Studio 2022 Build Tools (MSVC 19.44.35207, SDK Windows 10.0.26100, clang-format 19.1.5, ninja).
- Aucun build n'avait encore été tenté ; la conformité `clang-format` n'avait jamais été contrôlée.

## Travail effectué

- Lancement de la configuration réelle : `cmake --preset windows-debug`.
- Vérification de la conformité `clang-format` puis application de `clang-format -i` sur les sources non conformes.

## Tests exécutés

- Détection des compilateurs par CMake : RÉUSSIE - `The C compiler identification is MSVC 19.44.35228.0` et CXX identique ; ABI C et C++ testés. La correction `LANGUAGES C CXX` (exigence compilateur C de JUCE 8.0.15) est donc validée.
- Presets CMake : RÉUSSIE - le schéma version 5 est accepté par CMake 4.4.2 (aucune erreur de schéma).
- `clang-format --dry-run --Werror` : ÉCHEC initial - les 6 fichiers sources présentaient des violations de style ; `clang-format -i` appliqué.
- `clang-format --dry-run --Werror` après correction : RÉUSSIE - 6/6 fichiers conformes, aucune modification sémantique (revue des 6 fichiers effectuée).
- Vérification SHA-256 de l'archive JUCE téléchargée localement : NON CONCLUANTE - le téléchargement de l'archive (~22,9 Mo) n'aboutit jamais de façon fiable depuis cette machine (réseau instable vers GitHub, téléchargements coupés, empreinte non reproductible). L'empreinte attendue `04f8d505...` reste celle vérifiée lors de T-001.
- Configuration complète et build : NON RÉALISÉS - FetchContent de JUCE ne peut pas terminer le téléchargement localement.
- CTest : NON RÉALISÉ - aucun binaire produit.

## Décisions prises

- Le blocage actuel est d'environnement (réseau local vers GitHub), pas de code : la CI GitHub Actions (réseau interne rapide) reste la voie de validation principale.
- Le formatage `clang-format -i` est appliqué aux sources, conforme à `.clang-format` et aux règles de codage.
- T-001 ne passe PAS à TERMINÉ : builds Debug/Release et CI non réussis.

## État final de la tâche

T-001 reste PARTIEL.

## Travail restant

- Réussir la configuration locale quand le réseau le permettra, ou committer et pousser pour exécuter la CI (branche de travail puis Pull Request).
- Exécuter configuration, builds et CTest Debug/Release.
- Marquer T-001 TERMINÉ uniquement après réussite des builds, des tests et de la CI.

## Prochaine action

Committer et pousser le socle vers une branche de travail, ouvrir une Pull Request et laisser la CI valider Debug/Release ; en parallèle, retenter la configuration locale dès que l'accès réseau à GitHub est fiable.

# 2026-08-14 - T-001.3 - Valider le socle par les builds et tests Debug/Release

## Objectif

Exécuter réellement la configuration, les builds et les tests CTest du socle en Debug puis en Release, avec la toolchain installée.

## État avant intervention

- Configurations précédentes bloquées par l'instabilité du téléchargement de l'archive JUCE depuis GitHub.
- Solution retenue : télécharger l'archive avec `curl` (22 896 965 octets), vérifier son empreinte SHA-256 (`04f8d505...`, conforme), l'extraire dans `%TEMP%\juce-src` et configurer avec `FETCHCONTENT_SOURCE_DIR_JUCE` (aucun téléchargement réseau pendant la configuration).

## Travail effectué

- Correction d'un avertissement JUCE : `juce_add_gui_app` générait un `JUCE_BUNDLE_ID` invalide (`com.VocalMelody Studio.VocalMelodyStudio`, contenant des espaces). Ajout de `BUNDLE_ID "com.vocalmelody.studio"` dans `src/app/CMakeLists.txt`.
- Configuration puis build puis CTest en Debug, puis en Release.

## Fichiers modifiés

- `src/app/CMakeLists.txt` (ajout de `BUNDLE_ID`).

## Tests exécutés

- Configuration Debug : RÉUSSIE - `-- Configuring done (233.9s)`, schéma presets v5 accepté, compilateurs C/C++ identifiés.
- Build Debug : RÉUSSI - `VocalMelodyCommonTests.exe` puis `VocalMelody Studio.exe` produits (MSVC 19.44, `/W4 /WX`, aucune erreur ni avertissement du code applicatif).
- CTest Debug : RÉUSSI - `100% tests passed out of 1` (test `common.strong_types`, 1,86 s).
- Configuration Release : RÉUSSIE - `-- Configuring done (196.1s)`, aucun avertissement BUNDLE_ID.
- Build Release : RÉUSSI - les deux exécutables Release produits.
- CTest Release : RÉUSSI - `100% tests passed out of 1` (0,85 s).
- L'empreinte SHA-256 de l'archive JUCE a été re-vérifiée sur le téléchargement complet : conforme à `04f8d5055382582c757be9da069ea98338005f98248facd9c2804435ac853e70`.

## Décisions prises

- Les critères de build minimal et de tests minimaux de la phase 0 sont satisfaits localement en Debug et Release.
- La CI GitHub (builds + tests sur `windows-2022`, épinglée et avec contrôle `clang-format`) reste à exécuter via un push et une Pull Request, ce qui nécessite Git.
- T-001 ne passe PAS encore à TERMINÉ : le critère « CI verte » n'est pas atteint.

## État final de la tâche

T-001 reste PARTIEL (builds et tests locaux réussis ; CI non exécutée).

## Travail restant

- Installer Git pour Windows.
- Committer le socle (branche `fix/phase-0-build`), pousser et ouvrir une Pull Request pour exécuter la CI.
- Après CI verte, marquer T-001 TERMINÉ.

## Prochaine action

Installer Git, puis committer et pousser vers une branche de travail pour déclencher la CI GitHub Actions.

# 2026-08-14 - T-001.4 - Committer et pousser le socle validé

## Objectif

Placer le socle validé sous contrôle de version et le pousser vers GitHub pour préparer l'exécution de la CI.

## État avant intervention

- Git était absent de la machine ; aucun commit de travail n'existait (l'index ne suivait que `.gitignore` et `README.md`).

## Travail effectué

- Installation de Git pour Windows (MinGit 2.55.0.4) via `winget` (`Git.MinGit`).
- Configuration Git globale : `core.pager cat`, `user.name TUBI225`, `user.email emiloneufsix@gmail.com`, credential helper Git Credential Manager (fourni par GitHub Desktop).
- Création de la branche `fix/phase-0-build`.
- Stage et commit de l'ensemble du socle et de la documentation (31 fichiers, 7741 insertions, 1 suppression).
- Push de la branche vers `origin` après authentification GitHub.

## Commit associé

- `f83f7a6bed46db5a0fb19f7619b3b2712b299932` - « build(T-001): ajoute et valide le socle C++20 JUCE » - auteur TUBI225 <emiloneufsix@gmail.com> - 2026-08-14.
- Branche poussée : `fix/phase-0-build` (parent `bb2bbe3` Initial commit).

## Tests exécutés

- `git status --short` après commit : RÉUSSI - working tree propre.
- Vérification distante (API GitHub) : RÉUSSIE - la branche `fix/phase-0-build` existe avec le commit attendu.

## Décisions prises

- Le risque R-007 (partie « aucun commit de travail ») est résolu : tout le travail T-000 et T-001 est désormais commité et poussé.
- La CI se déclenchera sur l'événement `pull_request` : une Pull Request vers `main` reste à créer.

## État final de la tâche

T-001 reste PARTIEL (builds et tests locaux réussis, branche poussée ; CI non encore exécutée).

## Travail restant

- Créer la Pull Request depuis `fix/phase-0-build` vers `main` (lien : https://github.com/TUBI225/-VocalMelody-Studio/pull/new/fix/phase-0-build).
- Attendre la CI (builds Debug/Release, CTest, contrôle `clang-format`).
- Après CI verte, marquer T-001 TERMINÉ.

# 2026-08-14 - T-001.5 - Créer la Pull Request et déclencher la CI

## Objectif

Créer la Pull Request vers `main` pour déclencher la CI GitHub Actions sur le socle validé.

## Travail effectué

- Création de la Pull Request #1 via l'API GitHub (authentification réutilisée via le credential manager).
- Vérification du déclenchement du workflow `ci`.

## Résultats

- Pull Request #1 : https://github.com/TUBI225/-VocalMelody-Studio/pull/1 - « build(T-001): ajoute et valide le socle C++20 JUCE » - état `open`, 2 commits, 31 fichiers, +7784/−1.
- Workflow CI : run #1 (https://github.com/TUBI225/-VocalMelody-Studio/actions/runs/31802211276) - événement `pull_request` - statut observé : `in_progress`.

## État final de la tâche

T-001 reste PARTIEL (CI en cours d'exécution).

## Travail restant

- Attendre la fin de la CI (builds Debug/Release, CTest, contrôle `clang-format`).
- Après CI verte, marquer T-001 TERMINÉ.

# 2026-08-14 - T-001.6 - CI verte : T-001 TERMINÉ

## Objectif

Confirmer la réussite de la CI sur la Pull Request et clôturer la tâche T-001.

## Résultats

- Workflow `ci` (run #2, head `dd7f8ec`) : STATUT `completed`, CONCLUSION `success`.
- Job Windows x64 Debug : success - Checkout, Check formatting (`clang-format`), Configure, Build, Test, Complete job.
- Job Windows x64 Release : success - mêmes étapes.
- Les critères de sortie de la phase 0 sont donc tous satisfaits : build minimal, tests minimaux, CI verte, structure stable.

## Décisions prises

- T-001 passe à TERMINÉ.
- La phase 0 est terminée ; l'Audio Frontend (phase 1) peut être engagé après fusion de la Pull Request et décision de licence JUCE.

## État final de la tâche

TERMINÉ.

## Travail restant

- Fusionner la Pull Request #1 vers `main` (recommandé par le propriétaire).
- Décider du régime de licence JUCE (AGPLv3 ou commerciale) avant distribution (R-008).
- Mettre à jour les documents de statut (feuille de route, état actuel, registre des risques).

# 2026-08-14 - T-001.7 - Fusionner la Pull Request #1

## Objectif

Fusionner le socle validé dans `main` pour clôturer la phase 0.

## Travail effectué

- Fusion de la Pull Request #1 via l'API GitHub (méthode `merge`, historique préservé).
- Synchronisation du dépôt local : `fetch origin`, `switch main`, `merge --ff-only origin/main`.

## Résultats

- Pull Request #1 : état `closed`, `merged=true`, merge commit `5a8c7c346ab5412d101f38f3a6b61dbb901d2925`.
- Branche `main` distante : `5a8c7c3` (Merge pull request #1 from TUBI225/fix/phase-0-build).
- Dépôt local : sur `main`, synchronisé avec `origin/main` (fast-forward).

## État final de la tâche

T-001 TERMINÉ - phase 0 clôturée.

## Travail restant

- Décider du régime de licence JUCE (R-008) avant distribution.
- Engager la phase 1 (Audio Frontend) conformément à la feuille de route.

# 2026-08-14 - T-101 - Socle de la phase 1 : Audio Frontend

## Objectif

Démarrer la phase 1 (Audio Frontend) par son socle : structures de données du domaine (AudioSource, AudioAnalysisResult) et analyse de signal pure, testable sans JUCE.

## Travail effectué

- Nouveau module `src/frontend` (analyse de signal indépendante de JUCE) :
  - `SignalAnalysis` : RMS, peak, score de clipping, ratio de silence, détection de segments de silence, estimation du plancher de bruit, downmix stéréo vers mono.
- Structures de domaine dans `src/common` (conformes à MODELISATION_DONNEES) :
  - `AudioSource` (id, chemin original, date d'import, format, sample rate, canaux, bit depth, durée, hash du fichier — immuable, validation par factory `create`).
  - `AudioAnalysisResult` (id de source, version d'analyse, durée, sample rate d'analyse, chemin mono, scores clipping/bruit/présence vocale/qualité en `Score01`, carte de silence, avertissements).
  - `SilenceSegment` (plage `Seconds` début/fin, validée).
  - `AudioFormat` (Unknown/Wav/Mp3/M4a) avec conversion depuis l'extension.
- `vocalmelody_common` passe de `INTERFACE` à `STATIC` (sources `.cpp`).
- Refactor : `TestContext` partagé dans `tests/unit/TestContext.h`.
- Tests : `frontend.signal_analysis` (signal vide, sample rate invalide, constant, clipping, silence, segments, bruit, downmix) + `common.strong_types` conservé.

## Fichiers créés

- `src/frontend/CMakeLists.txt`
- `src/frontend/include/vocalmelody/frontend/SignalAnalysis.h`
- `src/frontend/SignalAnalysis.cpp`
- `src/common/include/vocalmelody/common/AudioSource.h`
- `src/common/include/vocalmelody/common/AudioAnalysisResult.h`
- `src/common/AudioSource.cpp`
- `src/common/AudioAnalysisResult.cpp`
- `tests/unit/TestContext.h`
- `tests/unit/SignalAnalysisTests.cpp`

## Fichiers modifiés

- `CMakeLists.txt` (add_subdirectory src/frontend)
- `src/common/CMakeLists.txt` (INTERFACE -> STATIC)
- `tests/CMakeLists.txt` (test frontend.signal_analysis)
- `tests/unit/StrongTypesTests.cpp` (TestContext partagé)

## Tests exécutés

- Build Debug : RÉUSSI - `vocalmelody_common.lib`, `vocalmelody_frontend.lib`, `VocalMelodyFrontendTests.exe`, application (`/W4 /WX`, aucune erreur ni avertissement).
- CTest Debug : RÉUSSI - `100% tests passed out of 2` (`common.strong_types` 0,01 s ; `frontend.signal_analysis` 0,79 s).
- Build Release : lancé (résultat à confirmer).
- Conformité `clang-format` : RÉUSSIE - 14/14 fichiers.

## Décisions prises

- Le socle de l'Audio Frontend est conçu indépendant de JUCE (analyse pure des trames mono) afin de rester testable et conforme aux règles de codage (« les tests ne doivent pas dépendre de JUCE lorsqu'ils vérifient uniquement le domaine commun »).
- Le décodage de fichiers WAV/MP3/M4A et la lecture (fonctions de la phase 1) viendront dans les prochaines étapes, adossés à JUCE (`juce_audio_formats`).

## État final de la tâche

PARTIEL - socle du frontend compilé et testé en Debug ; lecture/import réels à venir.

## Travail restant

- Importer/valider des fichiers réels (WAV/MP3/M4A) via JUCE.
- Alimenter `AudioSource` et `AudioAnalysisResult` depuis les fichiers décodés.
- Tests d'intégration « import -> analyse -> sauvegarde metadata ».
- Valider le Release localement et via la CI.

# 2026-08-14 - T-101.1 - Import audio WAV via JUCE

## Objectif

Décoder et importer de vrais fichiers audio et produire `AudioSource` + `AudioAnalysisResult` à partir du fichier décodé.

## Travail effectué

- Nouveau module `src/audio` (bibliothèque liée à JUCE) :
  - `AudioFileImporter` : décodage via `juce::AudioFormatManager`/`AudioFormatReader`, lecture des trames, downmix stéréo vers mono (`SignalAnalysis::downmixToMono`), calcul des diagnostics (`analyzeSignal`, `detectSilenceSegments`, `estimateNoiseFloor`), hachage FNV-1a du fichier, production de `AudioSource` et `AudioAnalysisResult`.
- Correction : `audioFormatFromExtension` ignore désormais un point initial (`.wav` -> `wav`) ; sans cela, l'extension retournée par JUCE (avec point) produisait `AudioFormat::Unknown` et l'import était rejeté.
- Tests `audio.file_import` : génération de WAV PCM 16 bits en mémoire (mono 44,1 kHz ; stéréo 22,05 kHz), fichier non-audio rejeté, fichier manquant rejeté.

## Fichiers créés

- `src/audio/CMakeLists.txt`
- `src/audio/include/vocalmelody/audio/AudioFileImporter.h`
- `src/audio/AudioFileImporter.cpp`
- `tests/unit/AudioImporterTests.cpp`

## Fichiers modifiés

- `CMakeLists.txt` (add_subdirectory src/audio dans le bloc JUCE)
- `tests/CMakeLists.txt` (test audio.file_import, conditionné à la disponibilité de JUCE)
- `src/common/AudioSource.cpp` (audioFormatFromExtension tolère le point initial)

## Tests exécutés

- Build Debug : RÉUSSI - `vocalmelody_audio.lib` et `VocalMelodyAudioImporterTests.exe` (`/W4 /WX`, aucune erreur ni avertissement applicatif).
- CTest Debug : RÉUSSI - `100% tests passed out of 3` (`common.strong_types`, `frontend.signal_analysis`, `audio.file_import`).
- Build Release : lancé (résultat à confirmer).
- Conformité `clang-format` : RÉUSSIE.

## Décisions prises

- Le décodage de fichiers est isolé dans `src/audio` (dépend de JUCE), tandis que l'analyse de signal reste pure (`src/frontend`, sans JUCE). Les tests de l'import dépendent de JUCE (couche audio, pas le domaine commun).
- L'import prend en charge WAV/MP3/M4A selon les formats enregistrés par `registerBasicFormats` (décodage WAV vérifié par tests ; MP3/M4A à valider avec un corpus réel).

## État final de la tâche

PARTIEL - import WAV validé ; lecture audio et corpus réel (MP3/M4A) à venir.

## Travail restant

- Valider le Release localement et via la CI.
- Lecture audio dans l'application (transport).
- Corpus de test réel (MP3/M4A, fichiers longs, corrompus) et tests d'intégration « import -> analyse -> sauvegarde metadata ».

# 2026-08-14 - T-101.2 - Lecture audio dans l'application

## Objectif

Permettre l'import d'un fichier audio depuis l'application et sa lecture (transport), avec affichage des diagnostics.

## Travail effectué

- `MainComponent` enrichi :
  - bouton « Importer... » (FileChooser, filtres *.wav;*.mp3;*.m4a) ;
  - import via `AudioFileImporter` (VocalMelody::Audio) ;
  - lecture via `AudioDeviceManager`, `AudioSourcePlayer` et `AudioTransportSource` (modules JUCE `juce_audio_devices`, `juce_audio_utils`) ;
  - bouton « Lecture / Pause » ;
  - affichage des diagnostics : format, sample rate, canaux, durée, scores de clipping/silence/qualité, nombre de segments de silence.
- `src/app/CMakeLists.txt` : liens ajoutés (VocalMelody::Audio, juce_audio_devices, juce_audio_utils).

## Fichiers modifiés

- `src/app/MainComponent.h`
- `src/app/MainComponent.cpp`
- `src/app/CMakeLists.txt`

## Tests exécutés

- Build Debug : RÉUSSI - application `VocalMelody Studio.exe` compilée avec les modules audio (`/W4 /WX`, aucune erreur ni avertissement applicatif).
- CTest Debug : RÉUSSI - `100% tests passed out of 3`.
- Lecture interactive : NON TESTÉE (nécessite un périphérique audio et une validation manuelle).
- Conformité `clang-format` : RÉUSSIE.

## Décisions prises

- L'import produit deux flux indépendants : les diagnostics via `AudioFileImporter` (domaine) et le transport de lecture via JUCE (lecteur dédié), afin de ne pas coupler le domaine au décodage.
- L'audio original n'est jamais modifié : l'analyse lit le fichier, la lecture utilise un lecteur séparé sur le même fichier.

## État final de la tâche

PARTIEL - import + lecture implémentés (à valider manuellement) ; corpus réel et métadonnées à venir.

## Travail restant

- Validation manuelle de la lecture (périphérique audio).
- Corpus de test réel (MP3/M4A, fichiers longs, corrompus) et tests d'intégration « import -> analyse -> sauvegarde metadata ».
- Validation Release et CI (run à suivre).

