# État actuel du projet

## Identification

- Projet : VocalMelody Studio
- Moteur : VIRE - Vocal Intent Reconstruction Engine
- Version : 0.1.0 (phase 0 terminée ; phase 1 Audio Frontend PARTIELLE ; phase 2 Pitch Benchmark en cours — socle fusionné)
- Plateforme cible : Windows x64
- Branche Git : `main`, propre et synchronisée avec `origin/main` (Pull Requests #1 à #8 fusionnées)
- Dernier commit poussé sur `main` : `e21e52d` - Merge pull request #8 from TUBI225/feat/import-error-diagnostics
- Dernière mise à jour : 2026-08-17 (audit du 2026-08-16 : PR #6 plafonds/Unicode et PR #7 anti-repliement renforcé fusionnées)

## État général

La phase 0 est terminée (T-000 et T-001, PR #1 fusionnée). La **phase 1 Audio Frontend** est PARTIELLE : import WAV/MP3 réel, analyse mono, diagnostics, métadonnées JSON, resampling 16 kHz, transport de lecture et décodage MP3 sont implémentés et **fusionnés dans `main` (PR #2, merge commit `8d0b715`)**. L'import est borné (1 Gio sur disque, 30 millions de trames décodées), vérifie le succès du décodage, calcule un SHA-256 avant/après et rejette toute modification concurrente. La suite de tests passe 6/6 en Debug et Release, le contrôle de formatage 28/28 et la CI Windows du dernier commit (run #15) est verte. Restent : validation manuelle de la lecture, corpus musical réel et M4A.

Le **socle de la phase 2 (Pitch Benchmark)** est implémenté, validé et **fusionné dans `main` (PR #3, merge commit `466f4cb`)** : structures `PitchFrame` / `PitchCandidate` / `PitchDistributionFrame` et types forts `MidiPitch` / `Cents` dans `src/common`, interface `IPitchEstimator` et baseline `AutocorrelationPitchEstimator` dans le nouveau module `src/pitch` (sans JUCE). La correction T-102.1 est fusionnée par la PR #4 (`41463c9`). T-102.2 est fusionnée par la PR #5 (`70b801f`) : le sinc/Blackman polyphasé remplace le chemin d'analyse linéaire et `analysisVersion` passe à 3 ; Debug/Release 8/8, formatage 35/35 et CI finale sont verts. Le benchmark proprement dit reste à réaliser.

L'**audit du 2026-08-16** (`AUDIT_2026-08-16.md`, non exhaustif) a été consigné (`da2397d`) et ses constats confirmés sont intégrés : la **PR #6** (`06b653e`) borne le décodage (≤ 64 canaux, ≤ 100 M échantillons décodés avant allocation) et rend les chemins MP3/métadonnées compatibles Unicode Windows ; la **PR #7** (`df6eefa`) renforce l'anti-repliement du rééchantillonneur (noyau 67 taps, marge 0.78, `analysisVersion` 4) avec la bande critique 8,5-9,5 kHz mesurée et testée, et documente l'invariant MP3 CBR tronqué. Debug/Release 8/8 et CI verte sur les trois PR. La **PR #8** (`e21e52d`) ajoute les diagnostics d'échec d'import (`ImportError`/`AudioImportOutcome`, rejet des formats inconnus avant décodage, raison affichée dans l'UI) — constat C-04 résolu. Reste : corpus musical réel, validation manuelle de la lecture et M4A.

## Tâches par statut

| Statut | Nombre |
|---|---:|
| TERMINÉ | 2 |
| EN COURS | 1 |
| PARTIEL | 1 |
| À VÉRIFIER | 0 |
| BLOQUÉ | 0 |
| À FAIRE | 0 |

## Fonctions terminées

- Infrastructure documentaire T-000 terminée.
- T-001 : socle de build C++20/JUCE/CMake, application minimale, types forts `Seconds`, `Beats`, `Probability`, `Score01`, test CTest et CI Windows — builds/tests Debug/Release réussis et CI verte.
- Import/analyse WAV et sérialisation JSON validés automatiquement sur corpus synthétique.

## Fonctions partielles

- T-101 : import/analyse WAV, rééchantillonnage mono sinc/Blackman polyphasé à 16 kHz et métadonnées JSON version 3 validés localement ; décodeur MP3 `minimp3` validé sur un vecteur Layer III réel ; M4A non implémenté et lecture interactive à finaliser.
- T-102 : socle phase 2 validé et fusionné dans `main` — structures pitch, types forts `MidiPitch`/`Cents`, interface `IPitchEstimator` et baseline autocorrélation ; le benchmark (estimateurs cibles, corpus vocal, mesures) reste à réaliser.

## Tâches en cours ou bloquées

- T-101 : PARTIEL dans la Pull Request #2. Le nouveau code MP3 est vert en CI ; un corpus musical/utilisateur et une validation interactive restent nécessaires. M4A reste à concevoir.
- T-102 : EN COURS, socle fusionné dans `main` via la Pull Request #3 (merge commit `466f4cb`, CI verte) ; le benchmark des estimateurs et le corpus vocal restent à réaliser.

## Erreurs et risques critiques

- Aucun crash reproduit sur le corpus automatisé. Cela ne constitue pas une garantie d'absence d'erreur.
- R-003 : encore OUVERT mais réduit par les plafonds d'import et les tests vide/court/long/corrompu ; l'import reste synchrone et charge le signal décodé en mémoire.
- R-001 : RÉDUIT - builds locaux et CI réussis (MSVC 19.44, CMake 4.4.2).
- R-007 : RÉSOLU - toolchain installée, téléchargement JUCE vérifié (SHA-256) puis configuration hors ligne, Git installé et socle commité/poussé.
- R-008 : régime de licence JUCE à décider avant distribution.
- R-002 : conservation de l'incertitude toujours prioritaire pour les futures structures L0/L1/L2.

## Derniers tests exécutés

- Validation JSON de `CMakePresets.json` : RÉUSSIE (1/1) après passage du schéma 6 au schéma 5.
- Correction des blocages de configuration CMake : RÉUSSIE - `LANGUAGES C CXX` (exigence compilateur C de JUCE 8.0.15) et schéma presets 5 (compatible CMake 3.24).
- Contrôle de l'arborescence initiale : RÉUSSI (14/14 fichiers attendus).
- Vérification SHA-256 de l'archive JUCE 8.0.15 : RÉUSSIE (1/1) lors de T-001.
- Vérification dans les en-têtes JUCE des API utilisées par le socle : RÉUSSIE (4/4).
- `git diff --check` : RÉUSSI, avec avertissement LF/CRLF informatif sur README.
- Contrôle documentaire : RÉUSSI, 13/13 documents valides et 5/5 cahiers sources préservés.
- Validation YAML automatisée : NON EXÉCUTÉE - PyYAML puis le module Node `yaml` étaient absents.
- Détection des compilateurs C et C++ par CMake : RÉUSSIE - MSVC 19.44.35228 identifié et ABI testé pour les deux langages.
- Presets CMake : RÉUSSIE - schéma version 5 accepté par CMake 4.4.2.
- Conformité `clang-format` : CORRIGÉE - 6/6 fichiers sources reformatés avec `clang-format -i` puis conformes au `--dry-run --Werror`.
- Vérification SHA-256 de l'archive JUCE (téléchargement complet via curl) : RÉUSSIE - empreinte `04f8d505...` conforme, 22 896 965 octets.
- Configuration Debug : RÉUSSIE - `-- Configuring done (233.9s)` ; compilateurs C/C++ (MSVC 19.44) identifiés.
- Build Debug : RÉUSSI - `VocalMelodyCommonTests.exe` et `VocalMelody Studio.exe` produits (`/W4 /WX`, aucune erreur ni avertissement applicatif).
- CTest Debug T-101.4 : RÉUSSI - `100% tests passed out of 5` (4,25 s, validation finale).
- Configuration Release : RÉUSSIE - `-- Configuring done (196.1s)` ; avertissement JUCE BUNDLE_ID corrigé.
- Build Release : RÉUSSI - les deux exécutables Release produits.
- CTest Release T-101.4 : RÉUSSI - `100% tests passed out of 5` (2,26 s, validation finale).
- Conformité `clang-format` T-101.4 : RÉUSSIE - 24/24 fichiers C++ contrôlés.
- Validation finale T-101.5 (`analysisVersion=2`) : Debug 5/5 en 3,74 s ; Release 5/5 en 1,69 s ; formatage 24/24 et `git diff --check` réussis.
- CI distante de T-101.4/T-101.5 : RÉUSSIE sur `7b24b76` - run `31854004303`, Debug 3 min 36 s et Release 5 min 20 s. Mise à jour `actions/checkout` v7.0.1/Node.js 24 validée sur `4d81f3b` - run `31854334410`, Debug 3 min 14 s et Release 4 min 14 s, sans l'avertissement Node.js 20.

## Prochaine action recommandée

Tester un corpus musical/utilisateur MP3, effectuer une validation manuelle de la lecture et choisir une stratégie M4A juridiquement et techniquement compatible. Compléter la preuve du rééchantillonneur filtré sur chirps et corpus vocal. Réaliser ensuite le benchmark des estimateurs puis sélectionner FAST/BALANCED/HIGH QUALITY.
