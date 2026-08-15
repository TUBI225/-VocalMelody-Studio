ARCHITECTURE TECHNIQUE
PROJET : GÉNÉRATEUR DE MÉLODIE / ACCORDS / ARPÈGES À PARTIR D’UNE VOIX A CAPPELLA
MOTEUR CENTRAL : VIRE — Vocal Intent Reconstruction Engine
Version : 1.0
Plateforme cible : Windows x64

======================================================================
1. OBJECTIF DU DOCUMENT
======================================================================

Ce document décrit l’architecture logicielle du projet.

Il complète :
- le Cahier des charges ;
- le Cahier de logique du moteur VIRE.

Le cahier des charges définit CE QUE le logiciel doit faire.
Le cahier de logique définit COMMENT le moteur musical doit raisonner.
Le présent document définit COMMENT LE LOGICIEL DOIT ÊTRE ORGANISÉ pour permettre
l’implémentation, les tests, l’évolution et le remplacement de certains algorithmes sans
casser l’ensemble du projet.

======================================================================
2. PRINCIPES D’ARCHITECTURE
======================================================================

L’architecture doit respecter les principes suivants :

1. Modularité.
Chaque moteur doit avoir une responsabilité claire.

2. Faible couplage.
Le Pitch Engine ne doit pas connaître les détails du Harmony Engine.

3. Données explicites.
Les moteurs échangent des structures de données définies et versionnées.

4. Incertitude conservée.
Les interfaces ne doivent pas forcer la réduction prématurée des distributions.

5. Recalcul incrémental.
Une modification locale ne doit pas provoquer un recalcul complet.

6. Cache.
Les analyses lourdes doivent être sauvegardées.

7. Explicabilité.
Chaque décision importante doit pouvoir être retracée.

8. Testabilité.
Chaque moteur doit pouvoir être testé séparément.

9. Remplaçabilité.
RMVPE doit pouvoir être remplacé par CREPE, pYIN ou un futur modèle sans réécrire
le moteur musical.

10. CPU-first.
Le logiciel doit fonctionner sans GPU dédié obligatoire.

======================================================================
3. ARCHITECTURE GLOBALE
======================================================================

APPLICATION WINDOWS
│
├── UI / PROJECT MANAGER
│
├── AUDIO FRONTEND
│
├── VIRE
│   ├── Pitch Fusion
│   ├── Segmentation
│   ├── L0 Performance
│   ├── L1 Note Lattice
│   └── L2 Melodic Intention
│
├── TIME & STRUCTURE ENGINE
│
├── TONAL ENGINE
│
├── HARMONY ENGINE
│
├── DIVERSITY ENGINE
│
├── ARRANGEMENT ENGINE
│   ├── Bass
│   ├── Arpeggio
│   └── Counter-Melody
│
├── COMPATIBILITY / VALIDATION ENGINE
│
├── MIDI ENGINE
│
├── AUDIO PREVIEW ENGINE
│
├── CACHE & PROJECT DATABASE
│
└── LOGGING / DECISION TRACE

======================================================================
4. COUCHES LOGICIELLES
======================================================================

L’architecture est organisée en six couches.

COUCHE A — PRESENTATION
- interface utilisateur ;
- piano roll ;
- waveform ;
- vue accords ;
- sélection des propositions ;
- contrôles utilisateur.

COUCHE B — ORCHESTRATION
- Project Manager ;
- Pipeline Controller ;
- Task Scheduler ;
- Dependency Graph ;
- Cache Manager.

COUCHE C — PERCEPTION AUDIO
- Audio Frontend ;
- Pitch Estimators ;
- VAD ;
- Onset Detection ;
- Breath Detection ;
- Segmentation.

COUCHE D — COMPRÉHENSION MUSICALE
- VIRE L1/L2 ;
- Phrase Engine ;
- Time Engine ;
- Tonal Engine ;
- Structure Engine.

COUCHE E — GENERATION
- Harmony Engine ;
- Diversity Engine ;
- Bass Engine ;
- Arpeggio Engine ;
- Counter-Melody Engine.

COUCHE F — SORTIES
- MIDI ;
- Audio Preview ;
- Exports ;
- Decision Traces ;
- Project Save.

======================================================================
5. TECHNOLOGIES PROPOSÉES
======================================================================

5.1 Application Windows

Option principale :
C++20 + JUCE.

Raisons :
- audio natif ;
- MIDI ;
- interface ;
- Windows ;
- performances ;
- multithreading ;
- plugins / VST possibles plus tard.

5.2 Prototypage scientifique

Python peut être utilisé pour :
- tester des algorithmes ;
- comparer RMVPE / CREPE / pYIN ;
- analyser les datasets ;
- entraîner ou convertir des modèles ;
- expérimenter avec music21, librosa, scipy, numpy.

Le prototype Python ne doit pas obligatoirement devenir le cœur de la version finale.

5.3 Machine Learning

ONNX Runtime est privilégié pour l’inférence CPU dans l’application finale.

5.4 Données persistantes

Formats possibles :
- JSON pour profils, traces et configurations ;
- SQLite pour index/cache/projets ;
- format binaire compact pour courbes F0 très volumineuses ;
- MIDI pour données musicales exportables.

======================================================================
6. PROJECT MANAGER
======================================================================

Responsabilité :
gérer un projet musical complet.

Il contient :
- chemin audio original ;
- version d’analyse ;
- paramètres ;
- résultats L0/L1/L2 ;
- TimeWarpMap ;
- tonalités ;
- propositions harmoniques ;
- arrangements ;
- contraintes utilisateur ;
- historique ;
- cache.

Le Project Manager ne doit pas faire de calcul musical.

Il orchestre et stocke.

======================================================================
7. PIPELINE CONTROLLER
======================================================================

Le Pipeline Controller décide quelles étapes doivent être exécutées.

Exemple :

Nouvel audio
→ exécuter Audio Frontend
→ Pitch
→ VIRE
→ Time
→ Tonal
→ Harmony
→ Arrangement
→ Validation

Mais :

Utilisateur change l’accord mesure 4
→ ne PAS relancer Audio
→ ne PAS relancer Pitch
→ ne PAS relancer VIRE
→ relancer Harmony local
→ Arrangement local
→ Validation locale.

======================================================================
8. DEPENDENCY GRAPH
======================================================================

Construire un graphe de dépendances explicite.

Exemple :

AudioSource
↓
AudioAnalysis
↓
PitchFrames
↓
L0
↓
L1
↓
L2
├──→ TimeMap
├──→ TonalHypotheses
└──→ PhraseStructure
       ↓
    Harmony
       ↓
  Arrangement
       ↓
  Validation
       ↓
     MIDI

Chaque nœud possède :
- version ;
- hash des entrées ;
- statut ;
- timestamp ;
- dépendances.

Si une entrée ne change pas, le résultat peut être réutilisé.

======================================================================
9. AUDIO FRONTEND
======================================================================

Responsabilités :
- décoder le fichier ;
- vérifier le format ;
- convertir pour analyse ;
- normaliser si nécessaire ;
- calculer qualité ;
- produire une version mono d’analyse ;
- conserver l’original.

Interface logique :

AudioFrontend.analyze(AudioSource)
→ AudioAnalysisResult

AudioAnalysisResult contient :
- duration ;
- sampleRate ;
- channelCount ;
- clippingScore ;
- noiseScore ;
- silenceMap ;
- qualityScore.

======================================================================
10. PITCH ESTIMATOR INTERFACE
======================================================================

Tous les estimateurs doivent respecter la même interface.

IPitchEstimator :

estimate(audioChunk)
→ PitchFrameSequence

PitchFrame :

{
 time,
 frequencyHz,
 midiFloat,
 confidence,
 voicedProbability
}

Implémentations possibles :
- RMVPEPitchEstimator ;
- PYINPitchEstimator ;
- CREPEPitchEstimator ;
- YINPitchEstimator.

Le reste du logiciel ne doit pas connaître le modèle utilisé.

======================================================================
11. PITCH FUSION ENGINE
======================================================================

Responsabilité :
fusionner plusieurs PitchFrameSequence.

Entrées :
- estimateur A ;
- estimateur B ;
- estimateur C.

Sortie :
PitchDistributionSequence.

Chaque frame peut contenir plusieurs candidats :

{
 time,
 candidates: [
   {pitch, probability},
   {pitch, probability}
 ],
 octaveAmbiguity,
 confidence
}

======================================================================
12. PERFORMANCE LAYER L0
======================================================================

L0 est immutable.

Cela signifie :
une fois calculé pour un audio donné, L0 ne doit pas être modifié par les moteurs
musicaux.

L0 contient :
- pitch brut ;
- énergie ;
- timing ;
- vibrato ;
- glissando ;
- VAD ;
- onsets ;
- breaths ;
- caractéristiques acoustiques.

Si une correction est faite, elle appartient à L1 ou à une couche supérieure.

======================================================================
13. NOTE LATTICE L1
======================================================================

Structure centrale du projet.

Un NoteLattice est un DAG.

NoteNode :
- id ;
- segmentId ;
- midiCandidate ;
- probability ;
- centsDeviation ;
- stability ;
- acousticConfidence ;
- structuralImportance ;
- possibleRoles.

NoteEdge :
- from ;
- to ;
- interval ;
- melodicCost ;
- transitionProbability ;
- phraseCompatibility.

Le moteur doit supporter :
- best path ;
- top N paths ;
- pruning ;
- recalcul local.

======================================================================
14. MELODIC INTENTION L2
======================================================================

L2 est une représentation abstraite de phrase.

MelodicIntentPhrase :
- phraseId ;
- contour ;
- relativeIntervals ;
- rhythmPattern ;
- anchors ;
- climax ;
- tensionCurve ;
- breathPoints ;
- motifId ;
- confidence.

L2 ne doit pas dépendre exclusivement des notes MIDI absolues.

======================================================================
15. TIME ENGINE
======================================================================

Responsabilité :
construire la correspondance performance ↔ grille musicale.

Structures :

TempoHypothesis
TimeSignatureHypothesis
BeatAnchor
GrooveOffset
TimeWarpMap

TimeWarpMap doit fournir :

performanceToMusicalTime(seconds)
musicalToPerformanceTime(beats)

Le moteur doit pouvoir fonctionner :
- tempo fixe ;
- tempo variable ;
- rubato ;
- free time.

======================================================================
16. STRUCTURE ENGINE
======================================================================

Responsabilité :
comprendre la structure hiérarchique.

Niveaux :
- note ;
- measure ;
- phrase ;
- section ;
- song.

Structures :

Measure
Phrase
Section
SongStructure

Une Section ne doit pas être obligatoirement nommée "couplet" ou "refrain".

Si incertitude :
Section A
Section B
Section C.

======================================================================
17. TONAL ENGINE
======================================================================

Entrées :
- L1 ;
- L2 ;
- TimeMap ;
- PhraseStructure.

Sortie :
TonalHypothesisSet.

TonalHypothesis :
- tonic ;
- mode ;
- probability ;
- timeRange ;
- supportingNotes ;
- contradictingNotes.

Le moteur doit conserver plusieurs hypothèses.

======================================================================
18. HARMONY ENGINE
======================================================================

Le Harmony Engine doit être séparé en sous-modules.

18.1 Chord Vocabulary

Définit :
- triades ;
- sevenths ;
- extensions ;
- sus ;
- add ;
- inversions ;
- borrowed chords ;
- secondary dominants.

18.2 Candidate Generator

Génère les accords possibles.

18.3 Local Compatibility Scorer

Évalue note/mesure.

18.4 Transition Scorer

Évalue :
- fonctions ;
- voice leading ;
- basse ;
- genre.

18.5 Phrase Scorer

Évalue la phrase.

18.6 Section Scorer

Évalue la section.

18.7 Song Scorer

Évalue la chanson entière.

18.8 Search Engine

Implémentations possibles :
- Beam Search ;
- Viterbi ;
- Dynamic Programming.

18.9 Constraint Adapter

Applique contraintes utilisateur / CSP.

======================================================================
19. HARMONY CANDIDATE STRUCTURE
======================================================================

HarmonyCandidate :

{
 id,
 chords,
 harmonicRhythm,
 keyHypothesis,
 scoreBreakdown,
 phraseScores,
 sectionScores,
 globalScore,
 diversityFeatures,
 decisionTrace
}

Chaque accord doit conserver :
- position ;
- duration ;
- root ;
- quality ;
- extensions ;
- inversion ;
- bassNote ;
- voicingSuggestion.

======================================================================
20. GENRE ENGINE
======================================================================

Les profils de genres sont externes au code.

Exemple :

genres/
  afrobeats.yaml
  coupe_decale.yaml
  rnb.yaml
  soul.yaml
  gospel.yaml
  pop.yaml
  trap.yaml

Un profil contient :
- tempo range ;
- meters ;
- harmonic vocabulary ;
- transition weights ;
- rhythm density ;
- bass rules ;
- groove rules ;
- arpeggio grammars ;
- counter melody density ;
- voice space.

Le moteur doit charger ces profils dynamiquement.

======================================================================
21. DIVERSITY ENGINE
======================================================================

Entrée :
liste de HarmonyCandidate.

Sortie :
4-5 propositions.

Pipeline :
- filtrage qualité minimum ;
- extraction features ;
- distance ;
- clustering/sélection ;
- contrôle final.

DiversityFeatures :
- chordSequenceFingerprint ;
- functionalHarmonyFingerprint ;
- harmonicRhythm ;
- tensionProfile ;
- bassContour ;
- complexity ;
- borrowedChordRatio.

======================================================================
22. ARRANGEMENT ENGINE
======================================================================

Le Arrangement Engine reçoit une harmonie déjà sélectionnée.

Sous-moteurs :
- BassEngine ;
- ArpeggioEngine ;
- CounterMelodyEngine ;
- VoicingEngine ;
- DensityEngine.

Ils partagent :
- VoiceOccupationMap ;
- GenreProfile ;
- TimeMap ;
- Harmony.

======================================================================
23. VOICE OCCUPATION MAP
======================================================================

Structure importante.

Pour chaque fenêtre temporelle :

{
 vocalActivity,
 vocalDensity,
 vocalRegisterLow,
 vocalRegisterHigh,
 energy,
 phraseImportance
}

Utilisée pour décider quand :
- jouer ;
- simplifier ;
- se taire ;
- remplir ;
- déplacer le registre.

======================================================================
24. BASS ENGINE
======================================================================

Entrées :
HarmonyCandidate
GenreProfile
TimeMap
VoiceOccupationMap

Sortie :
MidiTrackData.

Le Bass Engine doit être capable de générer plusieurs styles :
- root ;
- inversion ;
- passing ;
- syncopated ;
- ostinato ;
- melodic.

======================================================================
25. ARPEGGIO ENGINE
======================================================================

Arpège basé sur une grammaire.

ArpeggioGrammar définit :
- subdivisions ;
- note-order ;
- probability rules ;
- octave rules ;
- syncopation ;
- rests ;
- variations ;
- maximum density.

Les grammaires sont genre-spécifiques.

======================================================================
26. COUNTER-MELODY ENGINE
======================================================================

Entrées :
- vocal phrase ;
- L2 intention ;
- harmony ;
- silence windows ;
- genre.

Étapes :
- chercher opportunité ;
- créer motif ;
- appliquer transformations ;
- vérifier tonalité ;
- vérifier harmonie ;
- vérifier collision ;
- scorer.

======================================================================
27. COMPATIBILITY ENGINE
======================================================================

Responsabilité :
tester une proposition complète.

Checks :
- vocal/harmony ;
- vocal/bass ;
- vocal/arpeggio ;
- vocal/counter melody ;
- bass/chord ;
- register collision ;
- density ;
- timing ;
- section energy ;
- global coherence.

Résultat :

ValidationResult {
 status: VALID | REPAIR | REJECT,
 issues,
 severity,
 suggestedRepairScope
}

======================================================================
28. REPAIR ENGINE
======================================================================

Si status = REPAIR :

Issue local
→ réparer mesure.

Issue phrase
→ réparer phrase.

Issue section
→ réparer section.

Issue global
→ renvoyer au Harmony Search.

Cette architecture évite de recommencer l’analyse audio.

======================================================================
29. USER CONSTRAINT ENGINE
======================================================================

UserConstraint :

{
 id,
 type,
 scope,
 target,
 value,
 strength,
 locked
}

Types :
- LOCK_NOTE ;
- LOCK_CHORD ;
- LOCK_KEY ;
- LOCK_TEMPO ;
- PREFER_GENRE ;
- AVOID_CHORD ;
- MARK_GOOD_VOCAL ;
- MARK_BAD_VOCAL.

Le moteur de contraintes transforme ces objets en :
- hard constraints ;
- soft constraints ;
- scoring modifiers.

======================================================================
30. DECISION TRACE SYSTEM
======================================================================

Chaque moteur peut produire une trace.

DecisionTrace :

{
 module,
 timestamp,
 inputRefs,
 selected,
 alternatives,
 scores,
 confidence,
 rulesTriggered,
 constraintsApplied,
 rejectionReasons
}

Ces traces sont conservées séparément des données musicales.

======================================================================
31. CACHE MANAGER
======================================================================

Le cache est essentiel.

Cache keys :
hash(audio)
+
moduleVersion
+
parameters
+
modelVersion

Exemple :
si RMVPE change de version,
seul le cache Pitch et ses descendants deviennent invalides.

======================================================================
32. PROJECT FILE
======================================================================

Format projet proposé :

project/
  project.json
  source/
    vocal_original.*
  cache/
    audio_analysis.bin
    pitch.bin
    l0.bin
    l1.bin
    l2.json
    time_map.json
    tonal.json
  harmony/
    candidates.json
    selected.json
  arrangement/
    bass.mid
    arpeggio.mid
    counter.mid
  exports/
  traces/
  logs/

Option future :
empaqueter le dossier dans un fichier unique .vireproject ou équivalent.

======================================================================
33. THREADING
======================================================================

UI Thread :
jamais bloqué.

Worker Threads :
- Audio Analysis ;
- Pitch ;
- VIRE ;
- Harmony Search ;
- Arrangement ;
- Rendering.

Les résultats sont renvoyés à l’UI par messages/events.

======================================================================
34. JOB SYSTEM
======================================================================

Chaque opération lourde est un Job.

Job {
 id,
 type,
 dependencies,
 progress,
 status,
 cancelable
}

Exemple :
AnalyzePitchJob.

L’utilisateur peut annuler une génération sans fermer le logiciel.

======================================================================
35. PROGRESS REPORTING
======================================================================

Chaque moteur doit pouvoir rapporter :
0-100 %.

Exemple :
Pitch 35 %
VIRE 60 %
Harmony 82 %

Le Pipeline Controller transforme ces valeurs en progression globale.

======================================================================
36. GESTION DES ERREURS
======================================================================

Erreur ne doit pas faire planter le projet.

Catégories :
- InvalidAudio ;
- ModelLoadFailure ;
- PitchFailure ;
- NoVoiceDetected ;
- TempoUncertain ;
- HarmonyNoCandidate ;
- ExportFailure.

Chaque erreur contient :
- code ;
- message utilisateur ;
- message technique ;
- recoverable ;
- suggestedAction.

======================================================================
37. MODÈLES IA
======================================================================

Tous les modèles doivent être référencés par :

ModelDescriptor {
 name,
 version,
 path,
 checksum,
 backend,
 precision,
 license,
 source
}

Ne jamais charger un modèle uniquement par un nom codé en dur.

======================================================================
38. LICENCES
======================================================================

Avant intégration de toute bibliothèque/modèle :
- vérifier licence ;
- usage commercial ;
- redistribution ;
- attribution ;
- poids téléchargés ;
- compatibilité Windows.

Conserver un registre de dépendances.

======================================================================
39. TESTS UNITAIRES
======================================================================

Chaque moteur doit avoir ses tests.

Exemples :

PitchFusion :
- accord des estimateurs ;
- désaccord ;
- octave error.

Lattice :
- plusieurs chemins ;
- pruning ;
- probabilities.

Harmony :
- note longue ;
- passing note ;
- cadence ;
- voice leading.

Time :
- tempo fixe ;
- rubato ;
- free time.

======================================================================
40. TESTS D’INTÉGRATION
======================================================================

Scénarios :

Audio → L0.
Audio → L1.
Audio → L2.
Audio → Harmony.
Audio → 5 candidates.
Audio → MIDI.

Tester aussi :
- changer un accord ;
- changer genre ;
- verrouiller tonalité ;
- réexporter sans recalcul Pitch.

======================================================================
41. GOLDEN TESTS MUSICAUX
======================================================================

Créer un dossier de références.

Chaque exemple possède :
- audio ;
- annotations ;
- tonalité attendue ;
- notes principales ;
- phrases ;
- acceptables harmonies ;
- cas interdits.

Le but n’est pas d’imposer une seule bonne réponse,
mais de vérifier que les résultats restent dans une zone musicalement acceptable.

======================================================================
42. BENCHMARK PITCH
======================================================================

Comparer :
- RMVPE ;
- pYIN ;
- CREPE ;
- YIN ;
- fusion.

Mesures :
- cent error ;
- octave error ;
- voicing accuracy ;
- stabilité ;
- CPU ;
- RAM ;
- temps de calcul.

======================================================================
43. BENCHMARK HARMONIE
======================================================================

Comparer :
- Beam Search widths ;
- Viterbi ;
- constraints ;
- scoring versions.

Mesures :
- vocal compatibility ;
- cadence quality ;
- diversity ;
- human rating ;
- runtime.

======================================================================
44. VERSIONNAGE DES ALGORITHMES
======================================================================

Chaque moteur possède une version.

Exemple :
VIRE_LATTICE_V1
HARMONY_SCORE_V3
TIMEWARP_V2

Les fichiers de projet stockent la version utilisée.

Cela permet de comparer les résultats après amélioration.

======================================================================
45. LOGS
======================================================================

Logs séparés :

app.log
audio.log
vire.log
harmony.log
performance.log
errors.log

Niveaux :
DEBUG
INFO
WARNING
ERROR

======================================================================
46. PERFORMANCE
======================================================================

Les opérations lourdes :
- débruitage ML ;
- pitch ML ;
- transcription ;
- gros beam search.

Doivent être :
- offline ;
- cacheables ;
- parallélisables si possible.

Les opérations utilisateur :
- changer accord ;
- changer inversion ;
- choisir proposition ;
- modifier densité.

Doivent être quasi instantanées.

======================================================================
47. GESTION MÉMOIRE
======================================================================

Éviter de garder plusieurs copies audio en RAM.

Utiliser :
- streaming ;
- memory mapping si nécessaire ;
- buffers ;
- données compactes.

Les pitch curves doivent être stockées en format compact.

======================================================================
48. EXPORT MIDI
======================================================================

MIDI Engine doit produire :
- tempo track ;
- time signature ;
- chords ;
- bass ;
- arpeggio ;
- counter melody ;
- optional vocal guide.

Le groove offset peut être converti en ticks.

======================================================================
49. AUDIO PREVIEW
======================================================================

Le rendu preview doit être séparé de la logique musicale.

Le moteur musical produit MIDI/events.

Preview Engine transforme cela en audio.

Ainsi :
changer de synthé ne modifie pas la composition.

======================================================================
50. PLUGINS / VST FUTUR
======================================================================

La version 1 peut fonctionner avec synthé interne/SoundFont.

Architecture future possible :
- VST3 host ;
- instruments externes ;
- presets utilisateur.

Cette fonction ne doit pas être nécessaire au MVP.

======================================================================
51. API INTERNE
======================================================================

Chaque moteur expose une API claire.

Exemple :

VireEngine.process(L0, parameters)
→ VireResult

HarmonyEngine.generate(VireResult, TonalSet, GenreProfile, Constraints)
→ HarmonyCandidateSet

ArrangementEngine.generate(HarmonyCandidate, ...)
→ ArrangementResult

Cette clarté facilite les tests et le remplacement des moteurs.

======================================================================
52. IMMUTABILITÉ DES DONNÉES DE BASE
======================================================================

Audio original :
immutable.

L0 :
immutable.

L1 :
versionné.

L2 :
versionné.

Une correction utilisateur ne remplace pas l’historique.
Elle crée une nouvelle version logique.

======================================================================
53. UNDO / REDO
======================================================================

Toute action utilisateur éditable doit être une Command.

Command pattern :

ApplyChordChange
ApplyKeyLock
ApplyGenreChange

Chaque Command possède :
execute()
undo()

Cela facilite Undo/Redo et historique.

======================================================================
54. CONFIGURATION
======================================================================

settings.json :

- audio device ;
- cache size ;
- analysis quality ;
- pitch estimator ;
- CPU threads ;
- preview soundfont ;
- log level.

project settings séparés des préférences globales.

======================================================================
55. MODES D’ANALYSE
======================================================================

FAST
- estimateur léger ;
- beam réduit ;
- preview rapide.

BALANCED
- mode normal.

HIGH QUALITY
- plusieurs estimateurs ;
- plus de candidats ;
- scoring complet.

======================================================================
56. MODE DEBUG MUSICAL
======================================================================

Afficher :
- L0 pitch ;
- L1 candidates ;
- L2 contour ;
- key hypotheses ;
- chord scores ;
- rejected chords ;
- phrase score ;
- global score.

Essentiel pendant le développement.

======================================================================
57. MODE UTILISATEUR SIMPLE
======================================================================

Masquer la complexité.

Afficher :
- Importer ma voix ;
- Genre ;
- Générer ;
- Proposition 1-5 ;
- Accords ;
- Mélodie ;
- Arpège ;
- Export.

La sophistication interne ne doit pas rendre le logiciel difficile à utiliser.

======================================================================
58. DOSSIERS SOURCE PROPOSÉS
======================================================================

src/
  app/
  ui/
  project/
  audio/
    io/
    preprocessing/
    pitch/
    segmentation/
  vire/
    l0/
    lattice/
    intention/
  time/
  structure/
  tonal/
  harmony/
    vocabulary/
    candidate/
    scoring/
    search/
    constraints/
  genre/
  diversity/
  arrangement/
    bass/
    arpeggio/
    counter_melody/
    voicing/
  validation/
  midi/
  preview/
  cache/
  tracing/
  common/

tests/
  unit/
  integration/
  golden/
  benchmarks/

assets/
  genres/
  models/
  soundfonts/

docs/

======================================================================
59. ORDRE TECHNIQUE DE DÉVELOPPEMENT
======================================================================

PHASE A
Project Manager + Audio Frontend.

PHASE B
Pitch interface + un premier estimateur.

PHASE C
L0.

PHASE D
Segmentation + L1.

PHASE E
Phrase + L2.

PHASE F
Time Engine.

PHASE G
Tonal Engine.

PHASE H
Harmony Engine simple.

PHASE I
Multi-candidates + diversity.

PHASE J
Bass + Arpeggio.

PHASE K
Counter melody.

PHASE L
Validation loop.

PHASE M
UI avancée + locks + recalcul local.

======================================================================
60. RÈGLE D’OR DE L’ARCHITECTURE
======================================================================

Aucun moteur ne doit devenir le "cerveau géant" du logiciel.

La puissance du système vient de la coopération de plusieurs moteurs spécialisés,
chacun produisant :
- une information ;
- une confiance ;
- des alternatives ;
- des traces.

Le Pipeline Controller orchestre ces moteurs,
mais il ne décide pas musicalement à leur place.

======================================================================
61. CONCLUSION
======================================================================

L’architecture finale doit permettre ceci :

Un utilisateur importe une voix.

Le système analyse l’audio une seule fois.

Il conserve la performance réelle.

Il reconstruit plusieurs intentions possibles.

Il reconstruit une grille musicale sans écraser le groove.

Il maintient plusieurs tonalités.

Il explore plusieurs harmonisations à l’échelle :
note → mesure → phrase → section → chanson.

Il sélectionne plusieurs propositions réellement différentes.

Il génère basse, arpèges et contre-mélodies.

Il vérifie le résultat complet.

Il corrige localement si nécessaire.

Il permet à l’utilisateur de modifier le morceau sans relancer toute l’analyse.

C’est cette séparation claire des responsabilités qui doit rendre le projet robuste,
testable, évolutif et capable d’intégrer de futurs algorithmes sans reconstruction complète.

FIN DU DOCUMENT

======================================================================
62. ETAT D'IMPLEMENTATION - SOCLE PHASE 0
======================================================================

Version de l'implémentation : 0.1.0
Date : 2026-08-14
Statut : PARTIEL - sources présentes, compilation locale non exécutée.

Arborescence réellement créée :

src/
  app/
    Main.cpp
    MainComponent.h
    MainComponent.cpp
  common/
    include/vocalmelody/common/
      StrongTypes.h
      Version.h

tests/
  unit/
    StrongTypesTests.cpp

cmake/
  VocalMelodyWarnings.cmake

.github/workflows/
  ci.yml

Responsabilités actuelles :
- `vocalmelody_common` : contrats de base indépendants de JUCE et réutilisables par tous les moteurs ;
- `VocalMelodyStudio` : coquille de présentation JUCE, sans décision musicale ;
- `VocalMelodyCommonTests` : validation des invariants des types forts ;
- CMake : orchestration du build uniquement ;
- GitHub Actions : compilation et tests Windows x64 Debug/Release.

Flux initial :

CMake
→ vérification SHA-256 et extraction de JUCE
→ construction de `vocalmelody_common`
→ construction de l'application JUCE
→ construction puis exécution des tests CTest.

Séparations respectées :
- les types du domaine ne dépendent pas de JUCE ;
- l'interface ne contient aucune logique audio ou musicale ;
- aucune couche L0/L1/L2 n'est encore implémentée ;
- aucun Audio Frontend n'est commencé avant validation du socle.

Limite actuelle :
La machine de développement inspectée ne contient ni CMake ni compilateur C++ ; la
correspondance entre cette architecture et des binaires Debug/Release reste donc à valider.

======================================================================
63. ETAT D'IMPLEMENTATION - AUDIO FRONTEND T-101.4
======================================================================

Date : 2026-08-14
Statut : PARTIEL - cette section remplace l'état opérationnel obsolète de la section 62 sans supprimer son historique.

Modules effectivement présents :

- `vocalmelody_common` : types forts, `AudioSource`, `AudioAnalysisResult`, `JsonWriter` ; aucune dépendance JUCE.
- `vocalmelody_frontend` : analyse de signal pure (RMS, peak, silence, clipping, bruit approximatif, downmix).
- `vocalmelody_audio` : décodage JUCE, import borné, SHA-256, production des structures du domaine et métadonnées JSON.
- `VocalMelodyStudio` : sélection de fichier, diagnostics et transport de lecture JUCE.

Flux implémenté :

fichier audio
→ validation taille/format/durée
→ SHA-256 avant décodage
→ décodage JUCE en mémoire
→ moyenne de tous les canaux vers mono
→ rééchantillonnage linéaire borné à 16 kHz
→ diagnostics
→ SHA-256 après décodage
→ `AudioSource` + `AudioAnalysisResult`
→ JSON optionnel.

Garde-fous actuels :

- taille sur disque limitée à 1 Gio ;
- longueur décodée limitée à 30 millions de trames ;
- rejet d'un fichier vide, d'une lecture incomplète ou d'un changement de contenu pendant l'import ;
- exceptions d'allocation ou de codec converties en échec d'import, sans terminaison du processus ;
- durée d'import générée à l'exécution et identifiant stable dérivé du SHA-256 ;
- sélecteur asynchrone protégé contre la destruction du composant ;
- lecture désactivée si aucun périphérique audio n'est disponible.

Limites non masquées :

- l'import et l'analyse s'exécutent encore sur le thread d'interface et chargent le signal en mémoire ;
- le rééchantillonnage linéaire à 16 kHz est une baseline déterministe sans filtre anti-repliement ; il convient aux diagnostics initiaux mais ne constitue pas encore le rééchantillonneur validé pour le pitch ;
- WAV est couvert par tests synthétiques, MP3 n'est pas couvert par un corpus réel et M4A n'est pas garanti par les formats JUCE enregistrés sous Windows ;
- la lecture n'a pas encore été validée manuellement sur un périphérique réel ;
- l'API d'import retourne encore `std::nullopt` sans code d'erreur détaillé.
