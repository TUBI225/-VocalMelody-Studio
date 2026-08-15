# VocalMelody Studio

Application Windows de génération de mélodies, d'accords et d'arrangements à partir d'une voix a cappella. Le moteur central VIRE (Vocal Intent Reconstruction Engine) vise à préserver l'intention mélodique, rythmique et expressive de l'interprétation.

## État

La phase 0 est terminée et la phase 1 Audio Frontend est en cours sur `phase1/audio-frontend` (Pull Request #2). L'application importe et analyse des WAV, rééchantillonne le flux mono à 16 kHz, produit des métadonnées JSON et contient un transport de lecture JUCE. Les builds Debug/Release et les 5 tests CTest locaux réussissent ; la CI Windows est verte sur le dernier commit poussé `58e96e7`. Cette validation n'est pas exhaustive : le rééchantillonneur linéaire est une baseline pour les diagnostics, pas encore un choix validé pour le pitch ; la lecture réelle et un corpus MP3/M4A restent également à valider.

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
- Git 2.55 ou supérieur recommandé
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

Contrôle du formatage avec l'outil LLVM fourni par Visual Studio Build Tools (adapter le chemin si nécessaire) :

```powershell
$clangFormat = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\Llvm\x64\bin\clang-format.exe"
Get-ChildItem src,tests -Recurse -File -Include *.cpp,*.h |
    ForEach-Object { & $clangFormat --dry-run --Werror $_.FullName }
```

Le premier `cmake --preset` télécharge l'archive JUCE vérifiée (~23 Mo). Sur un poste sans accès réseau fiable à GitHub, télécharger et extraire l'archive puis configurer avec `-DFETCHCONTENT_SOURCE_DIR_JUCE=<dossier>`. La CI s'exécute à chaque Pull Request et à chaque push sur `main`.

Limite actuelle importante : sous Windows, la configuration JUCE retenue décode le WAV et peut s'appuyer sur Windows Media pour le MP3, mais elle ne fournit pas de décodeur M4A garanti. Aucun support MP3/M4A ne doit être annoncé comme validé avant un corpus réel et une décision de codec (par exemple Media Foundation adapté ou FFmpeg audité juridiquement).
