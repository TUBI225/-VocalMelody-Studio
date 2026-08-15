# Décisions d'architecture

Les décisions sont conservées dans l'ordre chronologique. Une décision remplacée n'est jamais supprimée.

## ADR-001 - Architecture modulaire CPU-first

- Date : 2026-08-14
- Statut : ACCEPTÉE (issue des documents de conception version 1.0)
- Problème : organiser un moteur musical complexe sans coupler l'analyse acoustique, l'intention musicale, l'harmonie et l'export.
- Contexte : l'application cible Windows x64 et doit fonctionner sans GPU dédié obligatoire.
- Options étudiées : monolithe ; pipeline modulaire ; services réseau séparés.
- Choix retenu : pipeline modulaire local, interfaces de données explicites et versionnées, cache et recalcul incrémental.
- Justification : testabilité, remplaçabilité des estimateurs, explicabilité et fonctionnement hors ligne.
- Conséquences : chaque moteur possède une responsabilité et des contrats d'entrée/sortie ; les incertitudes ne doivent pas être réduites prématurément.
- Risques : complexité des contrats et de l'invalidation du cache.
- Condition de révision : impossibilité mesurée d'atteindre les objectifs de performance ou de maintenance.

## ADR-002 - Socle applicatif C++20 et JUCE

- Date : 2026-08-14
- Statut : ACCEPTÉE TECHNIQUEMENT - LICENCE DE DISTRIBUTION À VALIDER
- Problème : choisir le socle de l'application Windows audio/MIDI.
- Contexte : l'architecture fournie recommande C++20 + JUCE et autorise Python pour les prototypes scientifiques.
- Options étudiées : C++20/JUCE ; application Python ; autre framework natif.
- Choix retenu : C++20 + JUCE pour l'application finale, Python isolé pour les benchmarks et prototypes.
- Avantages : audio et MIDI natifs, performances, multithreading, possibilité de VST future.
- Inconvénients : chaîne de compilation plus lourde et complexité C++.
- Conséquences : structure CMake, application JUCE et CI Windows créées ; le code de domaine reste indépendant de JUCE.
- Risques : JUCE est proposé sous AGPLv3 ou licence commerciale. Le propriétaire doit choisir un régime compatible avant toute distribution. La compilation locale et la CI sont validées ; la disponibilité des codecs MP3/M4A reste distincte de cette validation.
- Condition de révision : benchmark ou contrainte de distribution défavorable.

## ADR-004 - Acquisition reproductible de JUCE

- Date : 2026-08-14
- Statut : ACCEPTÉE
- Problème : obtenir JUCE sans commettre une copie volumineuse ni dépendre d'une branche mobile.
- Contexte : le build et la CI doivent utiliser exactement les mêmes sources.
- Options étudiées : sous-module Git ; archive vendored ; `FetchContent` sur tag ; `FetchContent` sur commit avec empreinte.
- Choix retenu : archive du commit JUCE 8.0.15 `91ad83ae34a81e0833b1a2b0866f54846370ae53`, téléchargée par CMake et vérifiée avec SHA-256 `04f8d5055382582c757be9da069ea98338005f98248facd9c2804435ac853e70`.
- Avantages : version immuable, contrôle d'intégrité, dépôt applicatif léger.
- Inconvénients : configuration initiale dépendante du réseau et d'une disponibilité de GitHub.
- Conséquences : un cache ou miroir sera nécessaire pour les builds entièrement hors ligne.
- Risques : indisponibilité réseau ; changement de régime de licence lors d'une future mise à niveau majeure.
- Condition de révision : exigence de build hors ligne ou indisponibilité de la source.

## ADR-003 - Formats persistants initiaux

- Date : 2026-08-14
- Statut : PROPOSÉE
- Problème : stocker projets, configurations, traces, cache et exports.
- Options étudiées : JSON ; SQLite ; format binaire ; combinaison de formats.
- Choix proposé : JSON pour profils/configurations/traces, SQLite pour index/cache/projets, binaire versionné pour courbes F0 volumineuses, MIDI pour l'export musical.
- Justification : formats adaptés à leurs usages et compatibilité avec le recalcul local.
- Conséquences : tout format persistant devra être versionné et migrable.
- Risques : divergence de schéma et incompatibilité descendante.
- Condition de révision : prototypes de volume, performance et migration.

## ADR-005 - Séparer décodage JUCE et analyse de signal pure

- Date : 2026-08-14
- Statut : ACCEPTÉE
- Problème : tester les diagnostics audio sans coupler le domaine aux codecs et à l'interface JUCE.
- Options étudiées : analyse directement dans l'interface ; module audio JUCE unique ; séparation `audio`/`frontend`/`common`.
- Choix retenu : `src/audio` décode et adapte les fichiers, `src/frontend` analyse des trames mono sans JUCE, `src/common` porte les structures et la sérialisation générique.
- Justification : tests rapides du domaine, codecs remplaçables et dépendances visibles.
- Conséquences : le décodeur peut être remplacé ou complété pour M4A sans réécrire l'analyse ; les tests de signal restent indépendants de JUCE.
- Risques : copie actuelle du signal complet en mémoire et double lecture pour le SHA-256 ; une API par blocs sera nécessaire pour les fichiers longs.
- Condition de révision : mesures montrant que les copies ou frontières de modules empêchent d'atteindre les budgets de performance.

## ADR-006 - Stratégie de décodage MP3 et M4A sous Windows

- Date : 2026-08-15
- Statut : ACCEPTÉE POUR MP3 / PROPOSÉE POUR M4A
- Problème : garantir un décodage robuste, déterministe et juridiquement sûr des fichiers compressés MP3 et M4A/AAC sous Windows x64, sans alourdir le binaire avec FFmpeg complet.
- Options étudiées :
  1. FFmpeg / libavcodec : complet mais lourd (plusieurs dizaines de Mo), complexe à intégrer sous Windows et risques de licences (LGPL/GPL avec brevets).
  2. Décodeur dédié pour MP3 (`minimp3`) et adaptateur Windows Media Foundation à étudier pour M4A/AAC.
  3. Dépendance exclusive aux codecs système Windows Media Foundation.
- Choix retenu :
  - **MP3** : `minimp3` au commit `ea99364f61c14656440e8d77e9c233ccf3124633`, archive contrôlée par SHA-256, licence CC0, isolé dans `vocalmelody::audio`, avec repli vers le lecteur JUCE pour les fichiers `.mp3` seulement.
  - **M4A / AAC** : décision non finalisée. Dans JUCE 8.0.15, `juce::WindowsMediaAudioFormat` n'annonce pas `.m4a`; un adaptateur Media Foundation dédié ou un autre codec audité reste nécessaire.
- Justification MP3 : dépendance légère, version immuable, licence identifiée et tests automatisables sans dépendre d'un codec système. Ceci ne constitue pas une garantie juridique exhaustive.
- Conséquences : WAV et MP3 disposent d'un chemin d'import ; M4A reste explicitement non implémenté. Les tests MP3 utilisent un vecteur Layer III réel fourni par la dépendance épinglée.
- Risques : corpus musical/utilisateur MP3 et CI du nouveau code encore à valider ; stratégie M4A, redistribution et environnements Windows N/KN à auditer avant toute annonce de support.
- Condition de révision : exigence de portabilité macOS/Linux nécessitant une abstraction décodeur AAC multi-plateforme.

## ADR-007 - Sample rate canonique d'analyse à 16 kHz

- Date : 2026-08-14
- Statut : ACCEPTÉE COMME BASELINE, À RÉÉVALUER AVANT LA PHASE PITCH
- Problème : les diagnostics doivent recevoir une fréquence d'échantillonnage stable malgré des sources WAV différentes.
- Choix provisoire : downmix de tous les canaux puis interpolation linéaire bornée vers 16 kHz ; le clipping reste mesuré sur les trames source pour ne pas masquer les pics.
- Justification : implémentation pure, déterministe, testable sans JUCE et suffisante pour stabiliser les diagnostics de phase 1.
- Limites : absence de filtre anti-repliement lors du sous-échantillonnage ; qualité non démontrée pour l'estimation de pitch.
- Conséquences : `analysisSampleRate=16000` et `analysisVersion=2` ; toute métadonnée version 1 reste distinguable.
- Condition de révision : benchmark avant phase 2 contre un rééchantillonneur à filtrage polyphasé/sinc ou une solution JUCE validée.
