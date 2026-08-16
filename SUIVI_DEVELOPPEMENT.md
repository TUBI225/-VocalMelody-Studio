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

# 2026-08-14 - T-101.3 - Corpus de tests d'intégration et métadonnées JSON

## Objectif

Étendre les tests d'intégration à un corpus de signaux variés et sérialiser les métadonnées d'import (AudioSource + AudioAnalysisResult) en JSON.

## Travail effectué

- Sérialiseur JSON dans le domaine commun (`JsonWriter`) : objets, tableaux, clés/valeurs, échappement des chaînes, nombres, booléens, null — sans dépendance externe, testé.
- Sérialisation des métadonnées audio (`src/audio`) : `audioMetadataToJson(AudioImportResult)` et `saveAudioMetadata(result, path)` (source + analyse, segments de silence, avertissements).
- Tests d'intégration `audio.metadata` (corpus) :
  - WAV mono 44,1 kHz et stéréo 22,05 kHz, signaux constant, silencieux et écrêté (clipping) ;
  - diagnostics fiables (clipping détecté, silence détecté) ;
  - sérialisation JSON vérifiée (champs attendus) ;
  - sauvegarde du fichier de métadonnées et relecture ;
  - **fichier audio original jamais modifié** (hash FNV-1a avant/après import).
- Refactor : helpers de test partagés `WavTestHelpers.h` (écriture WAV, chemins temporaires, hash FNV-1a), utilisés par les tests d'import et de métadonnées.

## Corrections apportées

- `JsonWriter` : la virgule est gérée par la clé (et non la valeur) et supprimée après `:` ; ajout d'une surcharge `value(const char*)` pour éviter la conversion implicite des littéraux en `bool`.

## Fichiers créés

- `src/common/include/vocalmelody/common/JsonWriter.h`
- `src/common/JsonWriter.cpp`
- `src/audio/include/vocalmelody/audio/AudioMetadataSerializer.h`
- `src/audio/AudioMetadataSerializer.cpp`
- `tests/unit/JsonWriterTests.cpp`
- `tests/unit/AudioMetadataTests.cpp`
- `tests/unit/WavTestHelpers.h`

## Fichiers modifiés

- `src/common/CMakeLists.txt`, `src/audio/CMakeLists.txt`, `tests/CMakeLists.txt`
- `tests/unit/AudioImporterTests.cpp` (helpers partagés)

## Tests exécutés

- Build Debug : RÉUSSI (`/W4 /WX`, aucune erreur ni avertissement applicatif).
- CTest Debug : RÉUSSI - `100% tests passed out of 5` (strong_types, json_writer, signal_analysis, file_import, metadata).
- Build Release : lancé (résultat à confirmer).
- Conformité `clang-format` : RÉUSSIE.

## État final de la tâche

PARTIEL - corpus de tests d'intégration et métadonnées JSON en place ; validation manuelle de la lecture et corpus réel (MP3/M4A) restants.

## Travail restant

- Validation manuelle de la lecture (périphérique audio).
- Corpus réel (MP3/M4A, fichiers longs, corrompus).
- Validation Release et CI (run à suivre).

# 2026-08-14 - T-101.4 - Audit, durcissement de l'import et validation Debug/Release

## Objectif

Vérifier l'état réel de la branche et de la CI, corriger les défauts observables de l'import audio, étendre le corpus demandé par la feuille de route et remettre les documents permanents en cohérence sans présenter l'audit comme exhaustif ou irréfutable.

## État vérifié avant modification

- Branche `phase1/audio-frontend`, commit poussé `58e96e7`, Pull Request #2 ouverte et fusionnable.
- CI du commit `58e96e7` : RÉUSSIE, run `31809979210`, événement `pull_request`.
- Outils présents : CMake/CTest 4.4.2, MSVC 19.44, clang-format et clang-tidy 19.1.5 dans Visual Studio, Git 2.55.0.
- Outils absents : GitHub CLI (`gh`) et FFmpeg ; ils ne sont pas requis pour le build WAV actuel.

## Défauts et limites trouvés

- Import complet en mémoire sans plafond ; conversion vers `int` avant contrôle ; retour du décodeur ignoré.
- Downmix limité aux deux premiers canaux ; date d'import fixe ; hash FNV-1a trop faible pour l'intégrité.
- Fonctions allouantes marquées `noexcept`, callback asynchrone capturant `this`, erreur du périphérique audio ignorée.
- JSON pouvant émettre NaN/Infinity.
- M4A non garanti sous Windows, MP3 non testé sur corpus réel, rééchantillonnage absent, lecture interactive non exécutée et erreur d'import générique.

## Corrections effectuées

- Plafonds de 1 Gio et 30 millions de trames ; validation avant allocation et contrôle du retour de lecture.
- Downmix de tous les canaux.
- SHA-256 avant/après décodage avec rejet d'une modification concurrente ; identifiant `audio-<sha256>` ; horodatage ISO 8601 runtime.
- Exceptions converties en échec d'import ; retrait des `noexcept` incorrects.
- `SafePointer` dans le sélecteur et lecture désactivée sans périphérique audio.
- NaN/infini JSON sérialisés en `null`.
- Ajout de `juce_cryptography`, issu du même JUCE 8.0.15 épinglé.

## Tests ajoutés et exécutés

- WAV vide, une trame, mono 30 secondes, trois canaux et WAV tronqué.
- SHA-256, identifiant contenu, timestamp runtime, NaN/infini JSON.
- Build Debug : RÉUSSI ; CTest Debug final : 5/5, 4,25 s.
- Build Release : RÉUSSI ; CTest Release final : 5/5, 2,26 s.
- `clang-format --dry-run --Werror` : RÉUSSI, 24/24 fichiers C++.
- `git diff --check` : RÉUSSI.
- CI du code T-101.4 : NON ENCORE EXÉCUTÉE avant commit/push.

## Contrôle documentaire

- Les 13 documents permanents ont été relus dans l'ordre imposé.
- Cahier des charges : préservé ; documents d'état, route, architecture, décisions, règles, dépendances, modèle, sécurité, risques, performances et reprise : actualisés selon leur but.
- README : actualisé avec les commandes et les limites codec.

## État final

T-101 reste PARTIEL. Les preuves couvrent le WAV automatisé mais pas le rééchantillonnage, M4A, le corpus MP3 réel, la lecture interactive ni la reprise après interruption.

## Prochaine action

Committer et pousser T-101.4, vérifier la CI de la Pull Request #2, puis traiter ADR-006 et le rééchantillonnage.

# 2026-08-14 - T-101.5 - Rééchantillonnage canonique d'analyse

## Décision sur le code Python fourni

Les deux parties reçues constituent un installateur Python de phase 0. Elles ne sont pas intégrées : elles créeraient un second projet `VIRE/`, dupliqueraient les documents permanents et réintroduiraient des états faux (`Git non initialisé`, CI reportée, phase 0 en cours). L'idée d'un prototype Python reste pertinente uniquement pour les futurs benchmarks isolés autorisés par ADR-002.

## Travail effectué

- Ajout de `frontend::resampleLinear`, rééchantillonneur pur, déterministe et borné.
- Sample rate canonique d'analyse fixé à 16 kHz.
- Diagnostics de silence, bruit et présence vocale calculés sur le flux rééchantillonné ; clipping conservé sur les trames source.
- Cas d'une seule trame garanti par une sortie minimale d'une trame.
- `analysisVersion` incrémentée de 1 à 2 et test JSON adapté.
- ADR-007 et risque R-012 ajoutés : l'interpolation linéaire reste une baseline sans anti-repliement, non validée pour le pitch.

## Tests

- Premier passage Debug : ÉCHEC utile sur le WAV d'une trame ; cause identifiée (taille calculée inférieure à une trame).
- Correction : sortie minimale d'une trame.
- Validation finale après `analysisVersion=2` : build et CTest Debug RÉUSSIS, 5/5, 3,74 s ; build et CTest Release RÉUSSIS, 5/5, 1,69 s.
- Formatage : RÉUSSI, 24/24 fichiers ; `git diff --check` : RÉUSSI.

## État final

Le critère « resampling » de phase 1 est couvert comme baseline fonctionnelle. Il n'est pas validé pour la phase pitch tant qu'un benchmark anti-repliement/qualité/CPU n'a pas comparé une méthode de meilleure qualité.

## Prochaine action

Relancer la validation finale après `analysisVersion=2`, puis publier lorsque GitHub CLI sera installé et authentifié. Ensuite : codec MP3/M4A ou lecture interactive selon la disponibilité du corpus et du périphérique.

# 2026-08-15 - T-101.6 - Publication et CI de l'audit Audio Frontend

## Travail effectué

- GitHub CLI 2.97.0 installé avec `winget` et authentifié pour `TUBI225`.
- Commit technique `7b24b76` créé puis poussé sur `phase1/audio-frontend`.
- Pull Request #2 mise à jour automatiquement.
- CI `31854004303` suivie jusqu'à sa conclusion réelle.

## Résultats CI

- Conclusion globale : RÉUSSIE.
- Windows x64 Debug : RÉUSSI en 3 min 36 s (formatage, configure, build, 5 tests).
- Windows x64 Release : RÉUSSI en 5 min 20 s (formatage, configure, build, 5 tests).
- Avertissement non bloquant : `actions/checkout` v4 ciblait Node.js 20 déprécié et était forcé sur Node.js 24.

## Correction de maintenance

- Version officielle la plus récente vérifiée : `actions/checkout` v7.0.1, publiée le 2026-07-20.
- Workflow mis à jour vers le commit immuable `3d3c42e5aac5ba805825da76410c181273ba90b1`.
- `action.yml` de ce commit vérifié : runtime `node24`.
- CI de validation du workflow : RÉUSSIE, run `31854334410` sur `4d81f3b`.
- Windows x64 Debug : RÉUSSI en 3 min 14 s.
- Windows x64 Release : RÉUSSI en 4 min 14 s.
- Aucun avertissement Node.js 20 observé sur ce run.

## État

Le code T-101.4/T-101.5 est validé localement et par la CI. T-101 reste PARTIEL pour les limites déjà consignées : lecture interactive, corpus MP3, codec M4A et qualité du rééchantillonnage avant pitch.

# 2026-08-15 - T-101.7 - Formalisation de la stratégie MP3 / M4A (ADR-006)

## Travail effectué

- Audit complet de la base de code, des tests unitaires/intégration et de la CI.
- Formalisation et validation de l'ADR-006 dans `DECISIONS_ARCHITECTURE.md` :
  - **MP3** : décodeur autonome C/C++ header-only léger (`minimp3` / `dr_mp3`, licence CC0/MIT) dans `vocalmelody::audio` avec repli WMF.
  - **M4A / AAC** : activation du décodeur Windows Media Foundation (`juce::WindowsMediaAudioFormat`) natif sur Windows x64.
- Mise à jour du registre des dépendances `DEPENDANCES.md`.
- Préparation de l'intégration et du corpus de test pour les formats compressés.

## Prochaine action

Exécuter la validation audio interactive dans l'IHM et intégrer les décodeurs MP3/M4A avec leurs tests de non-régression.

# 2026-08-15 - T-101.8 - Audit et intégration réelle du décodeur MP3

## Correction de l'audit précédent

- L'entrée T-101.7 est conservée comme historique, mais sa conclusion M4A était trop affirmative : dans JUCE 8.0.15, `juce::WindowsMediaAudioFormat` n'annonce pas `.m4a`. L'ADR-006 est donc acceptée pour MP3 et reste proposée pour M4A.
- Les premiers tests MP3 fabriquaient seulement des en-têtes et ne prouvaient pas un décodage utile. Ils ont été remplacés par le vrai vecteur Layer III `l3-sin1k0db.bit` de la dépendance épinglée, avec contrôle d'un signal non silencieux.
- Les formulations « exhaustive », « irréfutable » ou équivalentes ne doivent pas être utilisées : les preuves sont bornées à la plateforme, aux fichiers et aux commandes réellement testés.

## Travail effectué

- Intégration de `minimp3` au commit immuable `ea99364f61c14656440e8d77e9c233ccf3124633`, archive SHA-256 vérifiée et licence CC0 documentée.
- Ajout de `Mp3Decoder` pour fichier et mémoire, lecture par trames, production mono directe, validations de bornes et fermeture RAII.
- Repli JUCE limité à l'extension `.mp3`; suppression du repli MP3 pour les autres extensions afin d'empêcher un format mal étiqueté.
- Ajout de tests de décodage réel, données corrompues et MP3 renommé en WAV.

## Validation locale

- Debug : build réussi, CTest 6/6 réussi en 8,29 s.
- Release : build réussi, CTest 6/6 réussi en 5,83 s.
- Formatage : 28/28 fichiers C++ conformes ; `git diff --check` réussi.
- CI : à confirmer après publication du nouveau commit.

## État

MP3 est implémenté et validé localement en Debug/Release sur un vecteur réel, mais attend encore la CI et un corpus musical/utilisateur. M4A n'est pas implémenté. T-101 reste PARTIELLE.

## Résultat CI après publication

- Commit technique : `b89c053` (`feat(audio): ajoute le décodage MP3 vérifié`).
- GitHub Actions : run `31886309639`, conclusion RÉUSSIE.
- Windows x64 Debug : formatage, configuration, build et 6 tests réussis.
- Windows x64 Release : formatage, configuration, build et 6 tests réussis.

La preuve MP3 couvre désormais les builds locaux et la CI sur le vecteur Layer III épinglé. Le corpus musical/utilisateur, la lecture interactive et le M4A restent ouverts ; T-101 reste PARTIELLE.

# 2026-08-15 - T-101.9 - Fusionner la Pull Request #2 (phase 1 Audio Frontend)

## Objectif

Fusionner le socle de la phase 1 (Audio Frontend) dans `main`, après corrections issues de l'audit.

## Travail effectué

- Corrections mineures post-audit : retrait de `.m4a` du sélecteur de fichiers (M4A non encore supporté) et documentation de `minimp3` dans le README (prérequis réseau).
- Recompilation locale de l'application : RÉUSSIE (`/W4 /WX`, aucune erreur).
- CTest local : RÉUSSI - 6/6.
- CI (run #15) : RÉUSSIE - jobs Debug et Release `success` sur le commit `11aba83`.
- Fusion de la Pull Request #2 via l'API GitHub (méthode `merge`, historique préservé) : merge commit `8d0b715d9136dc3142126f42f4955a7c5810b197`.
- Synchronisation du dépôt local : `fetch`, `switch main`, `merge --ff-only origin/main` (45 fichiers, +2796/−53).
- Suppression de la branche `phase1/audio-frontend` (locale et distante).

## Résultats

- PR #2 : `merged=true`, merge commit `8d0b715`.
- `main` : 8d0b715 (Merge pull request #2) sur 11aba83.
- Working tree local : propre, sur `main`, synchronisé avec `origin/main`.

## État final de la tâche

T-101 (socle phase 1) : PARTIEL - import WAV/MP3, diagnostics, resampling 16 kHz, lecture et métadonnées fusionnés dans `main` ; reste la validation manuelle de la lecture, le corpus musical réel et M4A.

## Travail restant

- Validation manuelle de la lecture (périphérique audio).
- Corpus musical réel (MP3/M4A, fichiers longs, corrompus).
- Rééchantillonneur validé (filtrage anti-repliement) avant la phase pitch (R-012).
- Benchmark phase 2 : estimateurs RMVPE/CREPE/pYIN/YIN sur le corpus vocal, mesures (cents, octave error, voicing accuracy, CPU, RAM) puis sélection FAST/BALANCED/HIGH QUALITY.

# 2026-08-15 - T-102 - Socle phase 2 : interface IPitchEstimator et structures pitch

## Objectif

Poser le socle de la phase 2 (Pitch Benchmark) sur une branche dédiée : les structures pitch conformes au modèle (§8-10) et l'interface unique `IPitchEstimator` exigée par l'architecture (§10), sans dépendance à JUCE.

## Travail effectué

- Types forts pitch ajoutés dans `StrongTypes.h` : `MidiPitch` (valeur finie ≥ 0) et `Cents` (valeur finie, négative autorisée pour les déviations), via le nouveau gabarit `FiniteValue`.
- Structures pitch dans le domaine commun (`src/common`, `Pitch.h` / `Pitch.cpp`) :
  - `PitchFrame` (timeSeconds, frequencyHz, midiFloat, confidence, voicedProbability, estimatorId) ;
  - `PitchCandidate` (midiFloat, frequencyHz, probability, sourceEstimators, octaveAmbiguity) ;
  - `PitchDistributionFrame` (timeSeconds, candidates, fusedConfidence, voicedProbability) ;
  - fabriques validantes `create` (rejet des valeurs hors bornes) et helper `frequencyHzToMidi`.
- Nouveau module `src/pitch` (statique, lié à `VocalMelody::Common`, sans JUCE) :
  - interface `IPitchEstimator` : `estimate(MonoSignal) → std::vector<PitchFrame>` et `id()` ;
  - type `MonoSignal` (samples mono + sampleRate) ;
  - estimateur de référence `AutocorrelationPitchEstimator` (fenêtre 2048, hop 1024, plage 80-2000 Hz) : premier pic local de l'autocorrélation normalisée au-dessus du seuil de voicing 0,8, corrigeant le biais sous-harmonique observé avec le pic global.
- Nouveaux tests CTest : `pitch.structures` (bornes des structures, `frequencyHzToMidi`, types forts) et `pitch.autocorrelation` (sinus 440 Hz détecté dans ±10 Hz, signal vide, usage via l'interface).

## Validation locale

- Build Debug : RÉUSSI, aucun avertissement (`/W4 /WX`).
- CTest Debug : RÉUSSI - 8/8 (4,99 s) - 6 suites existantes + 2 nouvelles suites pitch.
- clang-format : RÉUSSI - 35/35 fichiers conformes.

## État

T-102 : EN COURS - le socle (structures pitch + interface + baseline autocorrélation) est validé localement sur la branche `phase2/pitch-benchmark`. Le benchmark proprement dit (estimateurs RMVPE/CREPE/pYIN/YIN, corpus vocal, mesures) reste à réaliser, de préférence après le rééchantillonneur anti-repliement (R-012).

## Résultat CI et fusion

- Commit technique : `e71eb8b` (`feat(pitch): ajoute l'interface IPitchEstimator et les structures pitch (socle phase 2)`).
- GitHub Actions : run `31890705017`, conclusion RÉUSSIE.
- Windows x64 Debug : formatage, configuration, build et 8 tests réussis.
- Windows x64 Release : formatage, configuration, build et 8 tests réussis.
- Pull Request #3 fusionnée le 2026-08-15 (méthode `merge`, historique préservé) : merge commit `466f4cb`.
- Branche `phase2/pitch-benchmark` supprimée (locale et distante) ; `main` local synchronisé.

Le socle phase 2 est donc intégré à `main` et validé localement + CI. Le benchmark (estimateurs cibles, corpus vocal, mesures FAST/BALANCED/HIGH QUALITY) reste à réaliser.

# 2026-08-15 - T-102.1 - Correction des bornes de la baseline pitch

## Problème reproduit

- La recherche du premier pic local excluait `minLag` et `maxLag`, donc les fréquences annoncées aux bornes 2000 Hz et 80 Hz n'étaient pas réellement couvertes.
- Une première correction naïve sans voisins extérieurs classait un sinus de 80 Hz à 2000 Hz, car son autocorrélation reste élevée aux petits lags.
- `frequencyHzToMidi` laissait passer `NaN` et `+infini` comme résultats non finis.

## Correction

- Calcul d'un lag voisin immédiatement avant et après la plage sélectionnable ; seuls les lags correspondant à 80-2000 Hz peuvent être retenus.
- Plafonnement des lags calculés à la fenêtre de 2048 échantillons, empêchant une allocation proportionnelle à un sample rate anormalement grand.
- Retour de zéro par `frequencyHzToMidi` pour toute fréquence non finie ou non positive, conformément au comportement existant des entrées invalides.
- Tests de non-régression sur sinus 80 Hz, 2000 Hz, `NaN` et `+infini`.

## Validation locale

- Debug : build réussi, CTest 8/8 en 4,02 s.
- Release : build réussi, CTest 8/8 en 9,78 s.
- CI : à exécuter après publication de la branche corrective.

## État

La baseline couvre désormais localement sa plage annoncée aux deux bornes. Cela ne valide pas sa robustesse sur voix, bruit, vibrato ou glissando et ne remplace pas le benchmark T-102.

## Résultat CI de T-102.1

- Commit technique : `dd3fff0` (`fix(pitch): corrige les bornes de l autocorrelation`).
- GitHub Actions : run `31893018165`, conclusion RÉUSSIE.
- Windows x64 Debug : formatage, configuration, build et 8 tests réussis.
- Windows x64 Release : formatage, configuration, build et 8 tests réussis.
- Pull Request #4 ouverte en brouillon vers `main` ; fusion non effectuée.

La correction est validée localement et en CI. Les limites fonctionnelles du benchmark consignées ci-dessus restent inchangées.

## Fusion de T-102.1

- Pull Request #4 passée de brouillon à prête pour révision puis fusionnée le 2026-08-15 avec historique préservé.
- Merge commit : `41463c9` (`Merge pull request #4 from TUBI225/fix/pitch-autocorrelation-bounds`).
- CI finale de la tête documentaire `e38dedf` : run `31893329023`, Debug et Release RÉUSSIS, 8/8 tests.
- Branche `fix/pitch-autocorrelation-bounds` supprimée localement et sur l'origine ; `main` local synchronisé avec `origin/main`.

T-102.1 est fusionnée et close. T-102 reste EN COURS pour le corpus vocal, les estimateurs cibles, les mesures et la sélection FAST/BALANCED/HIGH QUALITY.

# 2026-08-16 - T-102.2 - Rééchantillonnage anti-repliement 16 kHz

## Objectif

Réduire R-012 avant le benchmark pitch sans introduire JUCE dans `src/frontend`, tout en conservant une preuve comparative contre l'interpolation linéaire.

## Implémentation

- Ajout de `resampleWindowedSinc`, sinc fenêtré Blackman de rayon 16 (33 taps).
- Downsampling avec marge de coupure à 90 % pour créer une bande de transition avant la nouvelle fréquence de Nyquist.
- Table polyphasée pré-calculée ; nombre de phases dérivé du PGCD source/cible et plafonné à 1024.
- Taille de sortie toujours plafonnée à 100 millions de trames ; normalisation des poids aux frontières.
- `AudioFileImporter` utilise exclusivement le chemin filtré ; `resampleLinear` reste une référence de comparaison dans les tests.
- `analysisVersion` passe de 2 à 3 et le test JSON est mis à jour.

## Preuves fonctionnelles

- Upsampling d'un signal constant : durée et amplitude conservées.
- Sample rates identiques : copie exacte.
- Downsampling 48→16 kHz : sinus 1 kHz conservé dans une plage RMS 0,33-0,37.
- Même conversion : sinus 12 kHz atténué sous 0,01 RMS et sous 5 % du niveau produit par l'interpolation linéaire.

## Performance et validation locale

- Prototype trigonométrique direct rejeté après dépassement de 60 s sur les tests longs.
- Optimisation finale par table polyphasée/PGCD : suite ciblée en 6,65 s Debug.
- CTest complet Debug : 8/8 en 6,43 s.
- CTest complet Release : 8/8 en 11,62 s.
- CI initiale de la PR #5 : run `31952148170` RÉUSSI ; Windows Debug en 3 min 59 s et Windows Release en 5 min 52 s, formatage, configuration, build et 8 tests inclus.
- Corpus vocal : à exécuter.

## État

R-012 est RÉDUIT mais reste OUVERT. La preuve actuelle porte sur des sinusoïdes ciblées ; elle ne démontre pas encore la précision pitch sur chirps, vibrato, glissando ou voix réelle.

## Fusion de T-102.2

- Pull Request #5 passée de brouillon à prête pour révision puis fusionnée le 2026-08-16 avec historique préservé.
- Merge commit : `70b801f` (`Merge pull request #5 from TUBI225/phase2/antialias-resampler`).
- CI finale de la tête documentaire `ef91831` : run `31952485810`, Windows Debug et Release RÉUSSIS, formatage, configuration, build et 8 tests inclus.
- Branche `phase2/antialias-resampler` supprimée localement et sur l'origine ; `main` local synchronisé avec `origin/main`.

T-102.2 est fusionnée et close. T-102 reste EN COURS pour les chirps, le corpus vocal, les estimateurs cibles, les mesures CPU/RAM et la sélection FAST/BALANCED/HIGH QUALITY.
