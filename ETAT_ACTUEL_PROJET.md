# État actuel du projet

## Identification

- Projet : VocalMelody Studio
- Moteur : VIRE - Vocal Intent Reconstruction Engine
- Version : 0.1.0 (phase 0 terminée ; phase 1 Audio Frontend en cours)
- Plateforme cible : Windows x64
- Branche Git : `phase1/audio-frontend` (Pull Request #2 ouverte vers `main`)
- Dernier commit poussé : `22a8e99` - docs: trace la ci checkout node 24
- Dernière mise à jour : 2026-08-15 (T-101.8 MP3 en validation locale ; GitHub CLI 2.97.0 installé et authentifié)

## État général

La phase 0 est terminée (T-000 et T-001, PR #1 fusionnée). La **phase 1 Audio Frontend** est PARTIELLE : import WAV réel, analyse mono, diagnostics, métadonnées JSON, transport de lecture et décodage MP3 sont implémentés. L'import est borné (1 Gio sur disque, 30 millions de trames décodées), vérifie le succès du décodage, calcule un SHA-256 avant/après et rejette toute modification concurrente. Le code local T-101.8 passe 6/6 tests en Debug et Release ainsi que le contrôle de formatage 28/28. La CI verte du commit poussé `22a8e99` ne couvre pas encore ces changements locaux.

## Tâches par statut

| Statut | Nombre |
|---|---:|
| TERMINÉ | 2 |
| EN COURS | 0 |
| PARTIEL | 1 |
| À VÉRIFIER | 0 |
| BLOQUÉ | 0 |
| À FAIRE | 0 |

## Fonctions terminées

- Infrastructure documentaire T-000 terminée.
- T-001 : socle de build C++20/JUCE/CMake, application minimale, types forts `Seconds`, `Beats`, `Probability`, `Score01`, test CTest et CI Windows — builds/tests Debug/Release réussis et CI verte.
- Import/analyse WAV et sérialisation JSON validés automatiquement sur corpus synthétique.

## Fonctions partielles

- T-101 : import/analyse WAV, rééchantillonnage mono linéaire à 16 kHz et métadonnées JSON validés ; décodeur MP3 `minimp3` intégré et validé localement sur un vecteur Layer III réel ; M4A non implémenté et lecture interactive à finaliser.

## Tâches en cours ou bloquées

- T-101 : PARTIEL dans la Pull Request #2. La CI verte actuelle couvre le commit poussé précédent ; le nouveau code MP3 attend encore sa CI, un corpus musical/utilisateur et une validation interactive. M4A reste à concevoir.

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

Terminer la validation locale Debug/Release du décodeur MP3, pousser le changement et vérifier sa CI. Ensuite, tester un corpus musical/utilisateur, effectuer une validation manuelle de la lecture et choisir une stratégie M4A juridiquement et techniquement compatible. Avant la phase pitch, comparer le rééchantillonneur linéaire à une méthode avec filtrage anti-repliement.
