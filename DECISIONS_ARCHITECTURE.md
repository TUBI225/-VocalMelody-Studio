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
- Risques : JUCE est proposé sous AGPLv3 ou licence commerciale. Le propriétaire doit choisir un régime compatible avant toute distribution ; la compilation locale reste à valider.
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
