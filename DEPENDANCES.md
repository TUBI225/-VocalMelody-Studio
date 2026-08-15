# Dépendances

## Dépendances actives du socle

| Nom | Version / référence | Rôle | Statut | Licence | Source officielle | Installation / mise à jour | Dernière vérification |
|---|---|---|---|---|---|---|---|
| C++ | C++20 | Langage du cœur et de l'application | Obligatoire | Standard ISO | https://isocpp.org/ | Compilateur MSVC 2022 recommandé ; mise à niveau après validation CI | 2026-08-14 |
| CMake | >= 3.24 | Configuration et orchestration du build | Obligatoire | BSD-3-Clause | https://cmake.org/ | Installer localement ou utiliser l'image GitHub Actions ; version minimale dans `CMakeLists.txt` | 2026-08-14 |
| JUCE | 8.0.15, commit `91ad83ae34a81e0833b1a2b0866f54846370ae53` | Application Windows, UI, audio et MIDI futurs | Obligatoire pour l'application, pas pour le domaine commun | Double licence AGPLv3 ou JUCE commerciale | https://github.com/juce-framework/JUCE | `FetchContent` avec SHA-256 ; toute mise à jour exige revue de licence, changelog et tests | 2026-08-14 |
| minimp3 | commit `ea99364f61c14656440e8d77e9c233ccf3124633`, SHA-256 `5628166eb82a9bb581317918a334c317a2c0a30278bb14a20381307976768f34` | Décodage MP3 isolé dans `vocalmelody::audio` | Actif pour MP3 | CC0 | https://github.com/lieff/minimp3 | `FetchContent` épinglé ; mise à jour après revue licence, vecteurs et tests | 2026-08-15 |
| GitHub Actions Checkout | v7.0.1, commit `3d3c42e5aac5ba805825da76410c181273ba90b1` | Lecture du dépôt dans la CI | Obligatoire pour la CI | MIT | https://github.com/actions/checkout | Action épinglée au commit ; runtime Node.js 24 vérifié | 2026-08-15 |

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
| Codec M4A/AAC | À déterminer | Décodage M4A/AAC sous Windows 10/11 | Non choisi / non ajouté | À auditer | Évaluer un adaptateur Media Foundation dédié ou un codec tiers ; vérifier licence, redistribution et corpus |

## Capacités de codec et stratégie (ADR-006)

- WAV PCM 16/24/32 bits : vérifié par tests automatiques.
- MP3 : `minimp3` intégré et validé localement et en CI avec un vrai vecteur Layer III non silencieux ; corpus musical/utilisateur encore requis.
- M4A/AAC : non implémenté. `juce::WindowsMediaAudioFormat` dans JUCE 8.0.15 n'annonce pas `.m4a`, il ne suffit donc pas à établir ce support.
- Le module `juce_cryptography` est lié pour calculer SHA-256 ; il fait partie de la même distribution JUCE 8.0.15 déjà épinglée.

## Décision de licence à obtenir

JUCE 8 est disponible sous AGPLv3 ou licence JUCE commerciale. La préparation et la CI du code ne constituent pas une autorisation de distribuer un binaire fermé. Avant test externe ou distribution, le propriétaire doit documenter le régime choisi et vérifier les obligations correspondantes.

## Règle d'ajout

Avant toute dépendance : confirmer le besoin, la maintenance, la licence, la compatibilité Windows x64, l'utilisation commerciale, la redistribution, la méthode d'installation, la stratégie de mise à jour, le contrôle d'intégrité et une alternative raisonnable.
