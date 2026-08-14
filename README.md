# VocalMelody Studio

Application Windows de génération de mélodies, d'accords et d'arrangements à partir d'une voix a cappella. Le moteur central VIRE (Vocal Intent Reconstruction Engine) vise à préserver l'intention mélodique, rythmique et expressive de l'interprétation.

## État

Le dépôt est en phase 0. Le socle C++20/CMake/JUCE, une application Windows minimale, les premiers types du domaine, un test CTest et la CI Windows sont présents. La toolchain locale est installée (CMake 4.4.2, MSVC 19.44) et les configurations, builds et tests CTest **réussissent en Debug et en Release** (test `common.strong_types` à 100 %). La CI GitHub (bâtir puis tester sur `windows-2022`) n'a pas encore été exécutée : Git n'est pas installé sur la machine et aucun commit de travail n'existe. T-001 reste donc PARTIEL.

## Documentation permanente

Avant toute modification, lire les documents dans l'ordre défini par les instructions du projet :

1. `Cahier_des_charges.md`
2. `ETAT_ACTUEL_PROJET.md`
3. `FEUILLE_DE_ROUTE.md`
4. dernières entrées de `SUIVI_DEVELOPPEMENT.md`
5. `ARCHITECTURE_TECHNIQUE.md`
6. `DECISIONS_ARCHITECTURE.md`
7. `REGLES_DE_CODAGE.md`
8. `DEPENDANCES.md`
9. `MODELISATION_DONNEES.md`
10. `SECURITE.md`
11. `REGISTRE_DES_RISQUES.md`
12. `PERFORMANCES.md`
13. `PROTOCOLE_TEST_REPRISE.md` lorsqu'une opération interruptible est concernée

Le cahier détaillé du moteur VIRE est conservé dans `docs/CAHIER_LOGIQUE_MOTEUR_VIRE.md`.

## Prérequis de développement

- Windows x64
- Visual Studio 2022 Build Tools avec le composant C++
- CMake 3.24 ou supérieur
- Accès réseau au premier configure pour récupérer l'archive JUCE vérifiée

JUCE 8.0.15 est épinglé par commit et empreinte SHA-256. Avant toute distribution, choisir et documenter l'utilisation sous AGPLv3 ou sous licence JUCE commerciale.

## Construire et tester

```powershell
cmake --preset windows-debug
cmake --build --preset windows-debug --parallel
ctest --preset windows-debug

cmake --preset windows-release
cmake --build --preset windows-release --parallel
ctest --preset windows-release
```

Le premier `cmake --preset` télécharge l'archive JUCE vérifiée (~23 Mo). Sur un poste sans accès réseau fiable à GitHub, télécharger et extraire l'archive puis configurer avec `-DFETCHCONTENT_SOURCE_DIR_JUCE=<dossier>`. La prochaine étape consiste à committer le socle et à exécuter la CI. L'Audio Frontend ne doit commencer qu'après validation du socle.
