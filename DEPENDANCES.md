# Dépendances

## Dépendances actives du socle

| Nom | Version / référence | Rôle | Statut | Licence | Source officielle | Installation / mise à jour | Dernière vérification |
|---|---|---|---|---|---|---|---|
| C++ | C++20 | Langage du cœur et de l'application | Obligatoire | Standard ISO | https://isocpp.org/ | Compilateur MSVC 2022 recommandé ; mise à niveau après validation CI | 2026-08-14 |
| CMake | >= 3.24 | Configuration et orchestration du build | Obligatoire | BSD-3-Clause | https://cmake.org/ | Installer localement ou utiliser l'image GitHub Actions ; version minimale dans `CMakeLists.txt` | 2026-08-14 |
| JUCE | 8.0.15, commit `91ad83ae34a81e0833b1a2b0866f54846370ae53` | Application Windows, UI, audio et MIDI futurs | Obligatoire pour l'application, pas pour le domaine commun | Double licence AGPLv3 ou JUCE commerciale | https://github.com/juce-framework/JUCE | `FetchContent` avec SHA-256 ; toute mise à jour exige revue de licence, changelog et tests | 2026-08-14 |
| GitHub Actions Checkout | v4, commit `11d5960a326750d5838078e36cf38b85af677262` | Lecture du dépôt dans la CI | Obligatoire pour la CI | MIT | https://github.com/actions/checkout | Action épinglée au commit ; vérifier avant mise à jour | 2026-08-14 |

## Outils de qualité configurés

| Nom | Version | Rôle | Présence locale | Licence / source |
|---|---|---|---|---|
| CMake | 4.4.2 (>= 3.24 requis) | Configuration et orchestration du build | Présent | BSD-3-Clause / https://cmake.org/ |
| CTest | Fourni avec CMake | Exécution des tests | Présent | BSD-3-Clause / https://cmake.org/ |
| clang-format | 19.1.5 (LLVM fourni par VS 2022) | Formatage selon `.clang-format` | Présent | Apache-2.0 with LLVM exceptions / https://clang.llvm.org/ |
| MSVC Build Tools | Visual Studio 2022, MSVC 19.44.35207 | Compilation Windows x64 | Présent | Microsoft / https://visualstudio.microsoft.com/visual-cpp-build-tools/ |
| Ninja | Fourni par VS 2022 | Générateur de build optionnel | Présent | Apache-2.0 / https://ninja-build.org/ |
| Git | 2.55.0.windows.4 | Versionnement, branches et push | Présent dans le PATH | GPLv2 / https://git-scm.com/ |
| clang-tidy | 19.1.5 (LLVM fourni par VS 2022) | Analyse statique future | Présent dans Visual Studio, absent du PATH | Apache-2.0 with LLVM exceptions / https://clang.llvm.org/extra/clang-tidy/ |
| GitHub CLI (`gh`) | 2.97.0 | Gestion locale des Pull Requests, authentification et Actions | Présent dans `C:\Program Files\GitHub CLI`, compte `TUBI225` authentifié | MIT / https://cli.github.com/ |

## Dépendances futures non installées

| Nom | Version | Rôle futur | Statut | Licence | Vérification requise |
|---|---|---|---|---|---|
| ONNX Runtime | À déterminer | Inférence ML CPU | Facultatif / non ajouté | MIT | Taille, redistribution, backend CPU et performances |
| SQLite | À déterminer | Index, cache et projets | Facultatif / non ajouté | Domaine public | Schéma, migrations et stratégie d'écriture atomique |
| Python | À déterminer | Prototypes et benchmarks isolés | Facultatif / non ajouté au produit | PSF | Isolation stricte du cœur final |
| Codec M4A/MP3 Windows | À déterminer (Media Foundation, FFmpeg ou alternative) | Décodage garanti des formats phase 1 | Manquant / décision ADR-006 | À auditer selon option | Licence, redistribution, sécurité, maintenance et corpus réel |

## Capacités de codec réellement vérifiées

- WAV PCM 16 bits : vérifié par tests automatiques.
- MP3 : format potentiellement fourni par Windows Media dans JUCE sous Windows, mais aucun fichier réel n'a été testé.
- M4A/AAC : non garanti par `juce::AudioFormatManager::registerBasicFormats()` sous Windows dans la configuration actuelle ; FFmpeg n'est pas installé.
- Le module `juce_cryptography` est maintenant lié pour calculer SHA-256 ; il fait partie de la même distribution JUCE 8.0.15 déjà épinglée.

## Décision de licence à obtenir

JUCE 8 est disponible sous AGPLv3 ou licence JUCE commerciale. La préparation et la CI du code ne constituent pas une autorisation de distribuer un binaire fermé. Avant test externe ou distribution, le propriétaire doit documenter le régime choisi et vérifier les obligations correspondantes.

## Règle d'ajout

Avant toute dépendance : confirmer le besoin, la maintenance, la licence, la compatibilité Windows x64, l'utilisation commerciale, la redistribution, la méthode d'installation, la stratégie de mise à jour, le contrôle d'intégrité et une alternative raisonnable.
