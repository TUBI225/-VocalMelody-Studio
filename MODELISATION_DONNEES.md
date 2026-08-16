MODELISATION DES DONNEES MUSICALES
PROJET : GENERATEUR DE MELODIE / ACCORDS / ARPEGES A PARTIR D'UNE VOIX A CAPPELLA
MOTEUR CENTRAL : VIRE — Vocal Intent Reconstruction Engine
Version : 1.0
Plateforme cible : Windows x64

======================================================================
1. OBJECTIF DU DOCUMENT
======================================================================

Ce document définit la manière dont le logiciel représente les informations musicales
en mémoire, dans les fichiers projet et dans les échanges entre moteurs.

Il complète :
- le Cahier des charges ;
- le Cahier de logique du moteur VIRE ;
- l'Architecture technique.

L'objectif est d'éviter que chaque moteur invente ses propres formats.

Tous les moteurs doivent travailler avec des structures cohérentes, versionnées et
explicites.

======================================================================
2. PRINCIPES DE MODELISATION
======================================================================

Les données doivent respecter les principes suivants :

1. Ne jamais perdre l'information d'origine.
2. Ne jamais réduire trop tôt une distribution à une valeur unique.
3. Séparer clairement acoustique, musique symbolique et intention.
4. Conserver les incertitudes.
5. Conserver les alternatives.
6. Permettre le recalcul local.
7. Permettre l'explicabilité.
8. Être sérialisables.
9. Être versionnées.
10. Pouvoir évoluer sans rendre les anciens projets illisibles.

======================================================================
3. TYPES DE TEMPS
======================================================================

Le projet utilise deux espaces temporels.

3.1 PerformanceTime

Temps réel de l'audio en secondes.

Exemple :
0.000
0.527
1.843

Type :
float64 seconds

3.2 MusicalTime

Temps musical abstrait.

Structure :

MusicalTime {
  bar: int
  beat: int
  subdivision: float
  absoluteBeats: float
}

Exemple :
Mesure 4
Temps 3
Subdivision 0.5

3.3 TickTime

Temps MIDI.

TickTime {
  ticks: int64
  ppq: int
}

======================================================================
4. IDENTIFIANTS
======================================================================

Chaque entité importante possède un identifiant stable.

Types :
- ProjectId
- AudioSourceId
- SegmentId
- NoteNodeId
- PhraseId
- SectionId
- HarmonyCandidateId
- ArrangementId
- ConstraintId
- DecisionTraceId

Recommandation :
UUID ou identifiant 128 bits.

======================================================================
5. AUDIO SOURCE
======================================================================

AudioSource {
  id
  originalPath
  importedAt
  originalFormat
  sampleRate
  channelCount
  bitDepth
  durationSeconds
  fileHash
}

L'AudioSource représente le fichier original.

Il est immutable.

======================================================================
6. AUDIO ANALYSIS RESULT
======================================================================

AudioAnalysisResult {
  audioSourceId
  analysisVersion
  durationSeconds
  analysisSampleRate
  monoAnalysisPath
  clippingScore
  noiseScore
  voicePresenceScore
  silenceMap
  qualityScore
  warnings
}

======================================================================
7. TIME RANGE
======================================================================

Structure de base :

PerformanceRange {
  startSeconds
  endSeconds
}

MusicalRange {
  startBeats
  endBeats
}

Ces structures servent partout :
- segment ;
- phrase ;
- section ;
- accord ;
- contrainte.

======================================================================
8. PITCH FRAME
======================================================================

PitchFrame {
  timeSeconds
  frequencyHz
  midiFloat
  confidence
  voicedProbability
  estimatorId
}

midiFloat permet de conserver les micro-déviations.

Exemple :
69.37

======================================================================
9. PITCH CANDIDATE
======================================================================

PitchCandidate {
  midiFloat
  frequencyHz
  probability
  sourceEstimators
  octaveAmbiguity
}

======================================================================
10. PITCH DISTRIBUTION FRAME
======================================================================

PitchDistributionFrame {
  timeSeconds
  candidates[]
  fusedConfidence
  voicedProbability
}

Cette structure est utilisée avant quantification musicale.

======================================================================
11. ENERGY FRAME
======================================================================

EnergyFrame {
  timeSeconds
  rms
  peak
  normalizedEnergy
}

======================================================================
12. ONSET EVENT
======================================================================

OnsetEvent {
  id
  timeSeconds
  probability
  typeCandidates
}

typeCandidates possibles :
- syllabic
- note_attack
- breath
- consonant
- unknown

======================================================================
13. BREATH EVENT
======================================================================

BreathEvent {
  id
  startSeconds
  endSeconds
  confidence
  phraseBoundaryProbability
}

======================================================================
14. VIBRATO EVENT
======================================================================

VibratoEvent {
  startSeconds
  endSeconds
  centerPitchMidi
  rateHz
  depthCents
  regularity
  confidence
}

======================================================================
15. GLISSANDO EVENT
======================================================================

GlissandoEvent {
  startSeconds
  endSeconds
  startPitchMidi
  endPitchMidi
  slopeSemitonesPerSecond
  regularity
  confidence
  structuralStart
  structuralEnd
}

======================================================================
16. VOCAL SEGMENT
======================================================================

VocalSegment {
  id
  range
  pitchFrames[]
  energyFrames[]
  onsetRefs[]
  breathRefs[]
  vibratoRefs[]
  glissandoRefs[]
  voicedRatio
  stabilityScore
  segmentType
}

segmentType :
- stable_note
- transition
- glissando
- breath
- silence
- spoken
- melodic_spoken
- uncertain

======================================================================
17. L0 — PERFORMANCE LAYER
======================================================================

PerformanceLayerL0 {
  version
  audioSourceId
  pitchDistributionFrames[]
  energyFrames[]
  onsets[]
  breaths[]
  vibratos[]
  glissandos[]
  vocalSegments[]
  globalVoiceRange
  acousticWarnings[]
}

L0 est immutable.

======================================================================
18. NOTE NAME
======================================================================

NoteName {
  midiNumber
  pitchClass
  octave
  accidentalPreference
}

Le système doit stocker le MIDI number comme référence interne.
L'affichage Do/Ré/Mi ou C/D/E est une couche de présentation.

======================================================================
19. NOTE ROLE
======================================================================

Enum NoteRole :

- chord_tone
- root
- third
- fifth
- seventh
- extension
- suspension
- passing_tone
- neighbor_tone
- approach_tone
- appoggiatura
- anticipation
- escape_tone
- chromatic_tone
- ornament
- unknown

Une note peut avoir plusieurs rôles candidats avec probabilités.

======================================================================
20. NOTE ROLE HYPOTHESIS
======================================================================

NoteRoleHypothesis {
  role
  probability
}

======================================================================
21. NOTE NODE — L1
======================================================================

NoteNode {
  id
  segmentId
  targetMidi
  targetPitchClass
  probability
  centsDeviationFromObserved
  acousticConfidence
  stabilityScore
  structuralImportance
  roleHypotheses[]
  performanceRange
  musicalRangeCandidate
  isLocked
}

======================================================================
22. NOTE EDGE — L1
======================================================================

NoteEdge {
  fromNodeId
  toNodeId
  intervalSemitones
  melodicTransitionProbability
  contourCompatibility
  phraseCompatibility
  tonalCompatibility
  transitionCost
}

======================================================================
23. NOTE LATTICE L1
======================================================================

NoteLatticeL1 {
  version
  nodes[]
  edges[]
  segmentNodeIndex
  bestPathCandidate
  nBestPaths[]
  globalConfidence
}

Le Lattice est un graphe orienté acyclique.

======================================================================
24. NOTE PATH
======================================================================

NotePath {
  nodeIds[]
  logProbability
  melodicScore
  tonalScore
  confidence
}

======================================================================
25. STRUCTURAL IMPORTANCE
======================================================================

StructuralImportanceComponents {
  durationScore
  stabilityScore
  metricalAccentScore
  phraseBoundaryScore
  repetitionScore
  energyScore
  confidenceScore
}

StructuralImportance {
  components
  totalScore
}

Le total ne doit pas être une simple moyenne fixe.
La formule peut évoluer.

======================================================================
26. CONTOUR STEP
======================================================================

Enum ContourStep :

- strong_up
- up
- same
- down
- strong_down
- leap_up
- leap_down
- hold

======================================================================
27. RHYTHM TOKEN
======================================================================

RhythmToken {
  relativeDuration
  onsetStrength
  isSyncopated
  isRestBefore
  isRestAfter
}

======================================================================
28. MELODIC MOTIF
======================================================================

MelodicMotif {
  id
  contour[]
  relativeIntervals[]
  rhythmTokens[]
  lengthBeats
  repetitions[]
  similarityClusterId
}

======================================================================
29. MELODIC INTENTION PHRASE — L2
======================================================================

MelodicIntentPhrase {
  phraseId
  contour[]
  relativeIntervals[]
  rhythmPattern[]
  anchorNoteRefs[]
  climaxPosition
  tensionCurve[]
  breathPoints[]
  motifRefs[]
  phraseRole
  confidence
}

phraseRole :
- opening
- continuation
- response
- development
- climax
- resolution
- unknown

======================================================================
30. MELODIC INTENTION L2
======================================================================

MelodicIntentionL2 {
  version
  phrases[]
  globalMotifs[]
  globalContour
  globalTensionCurve
  confidence
}

======================================================================
31. BEAT ANCHOR
======================================================================

BeatAnchor {
  performanceTime
  musicalTime
  confidence
  anchorType
}

anchorType :
- strong_onset
- phrase_start
- phrase_end
- repeated_motif
- inferred

======================================================================
32. TEMPO HYPOTHESIS
======================================================================

TempoHypothesis {
  bpm
  probability
  meterCandidates[]
  sourceEvidence[]
}

======================================================================
33. METER HYPOTHESIS
======================================================================

MeterHypothesis {
  numerator
  denominator
  probability
}

======================================================================
34. TEMPO POINT
======================================================================

TempoPoint {
  musicalBeat
  bpm
  confidence
}

======================================================================
35. GROOVE OFFSET
======================================================================

GrooveOffset {
  eventId
  theoreticalTime
  performanceTime
  offsetMilliseconds
  confidence
}

======================================================================
36. TIME WARP MAP
======================================================================

TimeWarpMap {
  version
  tempoHypotheses[]
  selectedTempoMap[]
  meterHypotheses[]
  selectedMeter
  beatAnchors[]
  grooveOffsets[]
  mode
  confidence
}

mode :
- fixed_tempo
- elastic_tempo
- rubato
- free_time

Fonctions logiques :
performanceToMusicalTime()
musicalToPerformanceTime()

======================================================================
37. MEASURE
======================================================================

Measure {
  index
  musicalRange
  phraseRefs[]
  sectionRef
  vocalNoteRefs[]
  accentProfile
  vocalDensity
}

======================================================================
38. PHRASE
======================================================================

Phrase {
  id
  performanceRange
  musicalRange
  noteRefs[]
  motifRefs[]
  breathBefore
  breathAfter
  contour
  density
  registerLow
  registerHigh
  climax
  tensionProfile
  confidence
}

======================================================================
39. SECTION
======================================================================

Section {
  id
  label
  performanceRange
  musicalRange
  phraseRefs[]
  sectionRoleHypotheses[]
  energyProfile
  vocalDensity
  registerProfile
  repetitionGroup
  tensionProfile
}

sectionRoleHypotheses :
- intro
- verse
- pre_chorus
- chorus
- bridge
- outro
- A
- B
- C
- unknown

======================================================================
40. SONG STRUCTURE
======================================================================

SongStructure {
  measures[]
  phrases[]
  sections[]
  repetitionMap
  globalEnergyCurve
  globalTensionCurve
  confidence
}

======================================================================
41. PITCH CLASS DISTRIBUTION
======================================================================

PitchClassDistribution {
  probabilities[12]
  confidence
}

La somme vaut 1.

======================================================================
42. TONAL HYPOTHESIS
======================================================================

TonalHypothesis {
  tonicPitchClass
  mode
  probability
  musicalRange
  supportingEvidence[]
  contradictingEvidence[]
}

Modes possibles :
- major
- natural_minor
- harmonic_minor
- melodic_minor
- dorian
- phrygian
- lydian
- mixolydian
- locrian
- custom

======================================================================
43. TONAL HYPOTHESIS SET
======================================================================

TonalHypothesisSet {
  version
  globalCandidates[]
  localCandidatesBySection[]
  modulationCandidates[]
}

======================================================================
44. CHORD QUALITY
======================================================================

Enum ChordQuality :

- major
- minor
- diminished
- augmented
- sus2
- sus4
- power
- custom

======================================================================
45. CHORD EXTENSION
======================================================================

ChordExtension {
  degree
  alteration
}

Exemples :
7
maj7
b9
#11
13

======================================================================
46. CHORD SYMBOL
======================================================================

ChordSymbol {
  rootPitchClass
  quality
  extensions[]
  addedTones[]
  omittedTones[]
  bassPitchClassOptional
}

======================================================================
47. VOICING
======================================================================

Voicing {
  midiNotes[]
  registerLow
  registerHigh
  spacingProfile
  inversionIndex
}

======================================================================
48. CHORD EVENT
======================================================================

ChordEvent {
  id
  chord
  musicalRange
  voicing
  functionHypotheses[]
  localScore
  transitionScore
  phraseScoreContribution
  locked
}

======================================================================
49. HARMONIC FUNCTION
======================================================================

Enum HarmonicFunction :

- tonic
- predominant
- dominant
- tonic_substitute
- dominant_substitute
- modal
- chromatic
- pedal
- ambiguous

======================================================================
50. HARMONIC FUNCTION HYPOTHESIS
======================================================================

HarmonicFunctionHypothesis {
  function
  probability
}

======================================================================
51. HARMONIC RHYTHM
======================================================================

HarmonicRhythm {
  chordChangesPerMeasure
  changePositions[]
  regularity
}

======================================================================
52. SCORE BREAKDOWN
======================================================================

ScoreBreakdown {
  vocalCompatibility
  tonalCompatibility
  harmonicCoherence
  voiceLeading
  bassMotion
  rhythmicCoherence
  phraseSupport
  sectionSupport
  songStructure
  genreFit
  intentionPreservation
  densityBalance
  diversity
  penalties
  finalScore
}

======================================================================
53. PENALTY BREAKDOWN
======================================================================

PenaltyBreakdown {
  vocalCollision
  registerCollision
  excessiveCorrection
  harmonicBreak
  overload
  monotony
  constraintViolation
  unresolvedTension
}

======================================================================
54. HARMONY CANDIDATE
======================================================================

HarmonyCandidate {
  id
  tonalHypothesisRef
  chordEvents[]
  harmonicRhythm
  scoreBreakdown
  phraseScores[]
  sectionScores[]
  globalScore
  diversityFeatures
  decisionTraceRef
  family
}

family :
- faithful
- emotional
- modern
- minimal
- reharmonized
- genre_specific

======================================================================
55. PHRASE SCORE
======================================================================

PhraseScore {
  phraseId
  vocalFit
  cadenceQuality
  tensionShape
  intentionSupport
  rhythmSupport
  finalScore
}

======================================================================
56. SECTION SCORE
======================================================================

SectionScore {
  sectionId
  phraseCoherence
  energyDirection
  harmonicVariety
  repetitionBalance
  genreCoherence
  finalScore
}

======================================================================
57. DIVERSITY FEATURES
======================================================================

DiversityFeatures {
  chordFingerprint
  functionalFingerprint
  bassContour
  tensionProfile
  harmonicRhythmProfile
  borrowedChordRatio
  extensionDensity
  inversionProfile
  complexityScore
}

======================================================================
58. GENRE PROFILE
======================================================================

GenreProfile {
  id
  name
  version
  tempoRange
  meters[]
  chordVocabularyRules
  harmonicTransitionWeights
  harmonicRhythmRules
  bassRules
  arpeggioRules
  grooveRules
  counterMelodyRules
  densityRules
  voiceSpaceRules
  scoringModifiers
}

======================================================================
59. VOICE OCCUPATION FRAME
======================================================================

VoiceOccupationFrame {
  musicalRange
  vocalActivity
  density
  registerLow
  registerHigh
  energy
  phraseImportance
}

======================================================================
60. VOICE OCCUPATION MAP
======================================================================

VoiceOccupationMap {
  frames[]
}

======================================================================
61. MIDI NOTE EVENT
======================================================================

MidiNoteEvent {
  pitch
  velocity
  startBeat
  durationBeats
  channel
  articulation
  sourceRole
}

sourceRole :
- bass
- chord
- arpeggio
- counter_melody
- melody
- vocal_guide

======================================================================
62. MIDI CONTROL EVENT
======================================================================

MidiControlEvent {
  controller
  value
  beat
  channel
}

======================================================================
63. MIDI TRACK DATA
======================================================================

MidiTrackData {
  id
  name
  role
  noteEvents[]
  controlEvents[]
  program
}

======================================================================
64. BASS RESULT
======================================================================

BassResult {
  harmonyCandidateId
  midiTrack
  style
  score
  decisionTraceRef
}

======================================================================
65. ARPEGGIO RESULT
======================================================================

ArpeggioResult {
  harmonyCandidateId
  midiTrack
  grammarId
  densityProfile
  score
  decisionTraceRef
}

======================================================================
66. COUNTER MELODY RESULT
======================================================================

CounterMelodyResult {
  harmonyCandidateId
  midiTrack
  motifSources[]
  responseWindows[]
  score
  decisionTraceRef
}

======================================================================
67. ARRANGEMENT RESULT
======================================================================

ArrangementResult {
  id
  harmonyCandidateId
  bass
  chordsTrack
  arpeggio
  counterMelody
  optionalMelody
  voiceOccupationMap
  validationStatus
  globalScore
}

======================================================================
68. VALIDATION ISSUE
======================================================================

ValidationIssue {
  id
  type
  severity
  musicalRange
  involvedObjects[]
  message
  suggestedRepairScope
}

type possibles :
- vocal_harmony_collision
- register_collision
- bass_harmony_conflict
- rhythmic_conflict
- excessive_density
- weak_cadence
- broken_voice_leading
- global_monotony
- constraint_violation

======================================================================
69. VALIDATION RESULT
======================================================================

ValidationResult {
  arrangementId
  status
  issues[]
  score
}

status :
- VALID
- REPAIR
- REJECT

======================================================================
70. USER CONSTRAINT
======================================================================

UserConstraint {
  id
  type
  performanceRangeOptional
  musicalRangeOptional
  targetObjectIdOptional
  value
  strength
  locked
  createdAt
}

======================================================================
71. CONSTRAINT TYPE
======================================================================

Enum ConstraintType :

- lock_note
- lock_chord
- lock_key
- lock_tempo
- lock_meter
- prefer_genre
- avoid_chord
- mark_good_vocal
- mark_bad_vocal
- set_complexity
- set_density
- set_emotion

======================================================================
72. DECISION FACTOR
======================================================================

DecisionFactor {
  name
  value
  weight
  contribution
  explanationCode
}

======================================================================
73. DECISION ALTERNATIVE
======================================================================

DecisionAlternative {
  candidate
  score
  rejectionReason
}

======================================================================
74. DECISION TRACE
======================================================================

DecisionTrace {
  id
  module
  moduleVersion
  timestamp
  inputRefs[]
  selected
  confidence
  factors[]
  alternatives[]
  constraintsApplied[]
  warnings[]
}

======================================================================
75. MODEL DESCRIPTOR
======================================================================

ModelDescriptor {
  id
  name
  version
  path
  checksum
  backend
  precision
  license
  source
}

======================================================================
76. MODULE VERSION
======================================================================

ModuleVersion {
  moduleName
  semanticVersion
  algorithmId
  configurationHash
}

======================================================================
77. CACHE RECORD
======================================================================

CacheRecord {
  key
  moduleVersion
  inputHashes[]
  outputPath
  createdAt
  valid
}

======================================================================
78. PROJECT SETTINGS
======================================================================

ProjectSettings {
  analysisMode
  selectedGenre
  selectedEmotion
  harmonicComplexity
  density
  preferredPitchEstimators[]
  maxHarmonyCandidates
  outputProposalCount
}

analysisMode :
- fast
- balanced
- high_quality

======================================================================
79. APPLICATION SETTINGS
======================================================================

ApplicationSettings {
  cpuThreads
  cacheDirectory
  maxCacheSize
  audioDevice
  previewSoundfont
  logLevel
  defaultAnalysisMode
}

======================================================================
80. PROJECT STATE
======================================================================

ProjectState {
  projectId
  schemaVersion
  audioSource
  audioAnalysis
  l0
  l1
  l2
  timeWarpMap
  songStructure
  tonalHypotheses
  harmonyCandidates
  selectedHarmonyIds[]
  arrangements[]
  constraints[]
  settings
  activeVersions
}

======================================================================
81. SERIALISATION
======================================================================

JSON recommandé pour :
- settings ;
- genre profiles ;
- tonal hypotheses ;
- harmony candidates ;
- constraints ;
- decision traces ;
- song structure.

Binaire recommandé pour :
- pitch frames ;
- energy frames ;
- grands tableaux L0.

MIDI pour :
- tracks musicales.

SQLite possible pour :
- index des projets ;
- cache ;
- historique.

======================================================================
82. VERSION DU SCHEMA
======================================================================

Tous les fichiers structurés doivent contenir :

schemaVersion

Exemple :
"1.0.0"

Une migration doit être prévue si le schéma évolue.

======================================================================
83. HASH ET INVALIDATION
======================================================================

Chaque résultat doit pouvoir savoir si ses entrées ont changé.

Exemple :

L1 dépend de :
- L0 hash ;
- segmentation version ;
- lattice algorithm version ;
- paramètres.

Si un de ces éléments change :
L1 devient invalide.

======================================================================
84. IMMUTABILITE ET VERSIONNAGE
======================================================================

AudioSource :
immutable.

L0 :
immutable pour un couple audio+analyseVersion.

L1/L2 :
versionnés.

HarmonyCandidate :
immutable après création.

Une modification utilisateur crée :
- nouvelle contrainte ;
- nouvelle version dérivée ;
- nouvelle trace.

======================================================================
85. RELATIONS ENTRE OBJETS
======================================================================

AudioSource
→ AudioAnalysisResult
→ PerformanceLayerL0
→ NoteLatticeL1
→ MelodicIntentionL2
→ TimeWarpMap
→ SongStructure
→ TonalHypothesisSet
→ HarmonyCandidate[]
→ ArrangementResult[]
→ ValidationResult

======================================================================
86. PRINCIPES DE CONCEPTION DES SCORES
======================================================================

Tous les scores normalisés sont en [0,1], sauf indication contraire.

Les probabilités doivent être distinguées des scores heuristiques.

Exemple :
probability = interprétation statistique.
score = valeur de qualité.

Ne jamais nommer "probability" une heuristique arbitraire.

======================================================================
87. CONFIDENCE VS PROBABILITY
======================================================================

Probability :
probabilité relative entre hypothèses.

Confidence :
niveau de fiabilité du système sur cette distribution.

Exemple :
G = 0.55
G# = 0.45
confidence = 0.30

Cela signifie :
le système hésite presque à égalité et n'est pas très fiable.

======================================================================
88. GESTION DES VALEURS MANQUANTES
======================================================================

Ne jamais inventer une valeur.

Utiliser :
- optional ;
- unknown ;
- NaN contrôlé ;
- absence explicite.

Exemple :
selectedMeter = null
mode = free_time

======================================================================
89. REPRESENTATION DES ALTERNATIVES
======================================================================

Toute décision critique peut stocker :
- selected ;
- alternatives ;
- confidence.

Cela vaut pour :
- pitch ;
- note ;
- tempo ;
- meter ;
- key ;
- chord ;
- section role.

======================================================================
90. RECOMMANDATION POUR LE CODE C++
======================================================================

Types de base recommandés :

struct PerformanceTime;
struct MusicalTime;
struct PitchFrame;
struct NoteNode;
struct NoteEdge;
struct Phrase;
struct Section;
struct TonalHypothesis;
struct ChordEvent;
struct HarmonyCandidate;
struct MidiNoteEvent;
struct DecisionTrace;

Collections :
std::vector
std::unordered_map
std::optional

Identifiants :
UUID wrapper ou StrongType.

======================================================================
91. STRONG TYPES
======================================================================

Ne pas utiliser un simple double pour toutes les unités.

Créer des types distincts :

Seconds
Beats
BPM
MidiPitch
Cents
Probability
Score01

Cela évite des erreurs de conversion.

======================================================================
92. EXEMPLE COMPLET SIMPLIFIE
======================================================================

Un segment vocal observé :

L0 :
F0 = 386 Hz
confidence = 0.91
stability = 0.88
duration = 0.7s

L1 :
G4 = 0.62
F#4 = 0.23
G#4 = 0.15

L2 :
contour = montée
phraseRole = climax
structuralImportance = 0.92

Time :
mesure 8
temps 3
grooveOffset = +47ms

Tonal :
C major = 0.66
A minor = 0.61

Harmony candidate A :
Am7
vocalCompatibility = 0.86

Harmony candidate B :
Cmaj7
vocalCompatibility = 0.82

Le moteur ne choisit pas uniquement sur ce chiffre.
Il regarde aussi :
- phrase ;
- section ;
- voice leading ;
- chanson entière.

======================================================================
93. EXEMPLE DE TRACE
======================================================================

DecisionTrace :

module = HarmonyEngine

selected = Am7

factors :
- VocalFit = 0.86
- VoiceLeading = 0.94
- PhraseSupport = 0.90
- GenreFit = 0.81

alternative :
Cmaj7
score = 0.84
rejection = "moins bonne préparation de la résolution suivante"

======================================================================
94. TESTS DE MODELISATION
======================================================================

Tester :
- sérialisation / désérialisation ;
- compatibilité anciennes versions ;
- valeurs optionnelles ;
- graphes L1 ;
- top N paths ;
- références entre objets ;
- invalidation cache ;
- contraintes ;
- traces.

======================================================================
95. REGLE D'OR
======================================================================

Les structures de données doivent représenter la complexité réelle du problème,
pas la cacher.

Le moteur ne doit jamais être forcé par son modèle de données à transformer trop tôt :

"plusieurs hypothèses plausibles"

en :

"une seule vérité".

======================================================================
96. CONCLUSION
======================================================================

Cette modélisation permet de faire circuler la même information cohérente entre tous
les moteurs du projet.

Elle protège les principes fondamentaux de VIRE :

- performance originale préservée ;
- incertitude conservée ;
- intention séparée de la justesse ;
- temps réel séparé du temps musical ;
- raisonnement multi-niveaux ;
- alternatives harmoniques ;
- recalcul local ;
- explicabilité ;
- évolution des algorithmes.

FIN DU DOCUMENT

======================================================================
97. ETAT D'IMPLEMENTATION DES TYPES DE BASE
======================================================================

Date : 2026-08-14
Version du code : 0.1.0

Types effectivement introduits dans `StrongTypes.h` :
- `Seconds` : valeur finie supérieure ou égale à zéro ;
- `Beats` : valeur finie supérieure ou égale à zéro ;
- `Probability` : valeur finie comprise dans [0, 1] ;
- `Score01` : valeur finie comprise dans [0, 1] ;
- `MidiPitch` : valeur finie supérieure ou égale à zéro (ajouté le 2026-08-15, socle phase 2) ;
- `Cents` : valeur finie, négative autorisée pour les déviations (ajouté le 2026-08-15, socle phase 2).

Règles d'implémentation :
- construction contrôlée par `fromValue` ;
- une valeur invalide produit `std::nullopt` ;
- aucune conversion implicite depuis `double` ;
- aucune donnée persistante ni migration de schéma n'est introduite ;
- `Probability` et `Score01` restent des types distincts malgré le même domaine numérique.

Types non encore implémentés :
- `BPM` ;
- identifiants stables ;
- structures L0, L1 et L2.

Validation :
Les tests unitaires source couvrent les bornes, les valeurs négatives, NaN et l'infini. Ils sont compilés et exécutés (Debug et Release, CI verte) ; le test CTest `common.strong_types` passe à 100 %.

======================================================================
98. ETAT D'IMPLEMENTATION - STRUCTURES AUDIO (PHASE 1)
======================================================================

Date : 2026-08-14
Version du code : 0.1.0

Structures effectivement introduites (phase 1 - Audio Frontend) :

- `AudioFormat` (enum : Unknown, Wav, Mp3, M4a) avec `audioFormatFromExtension` et `audioFormatToString`.
- `AudioSource` : immuable, créée via `AudioSource::create` (validation : id/chemin/date/hash non vides, sample rate/canaux/bit depth positifs, format connu). Champs : id, originalPath, importedAt, originalFormat, sampleRate, channelCount, bitDepth, durationSeconds (`Seconds`), fileHash.
- `SilenceSegment` : plage `Seconds` début/fin, créée via `SilenceSegment::create` (validation : start < end).
- `AudioAnalysisResult` : créée via `AudioAnalysisResult::create` (validation : audioSourceId non vide, version >= 0, sample rate > 0). Champs : audioSourceId, analysisVersion, durationSeconds, analysisSampleRate, monoAnalysisPath, clippingScore (`Score01`), noiseScore (`Score01`), voicePresenceScore (`Score01`), silenceMap, qualityScore (`Score01`), warnings.

Module d'analyse (indépendant de JUCE) :
- `vocalmelody_frontend` (`src/frontend`) : analyse de trames mono — RMS, peak, score de clipping, ratio de silence, détection de segments de silence, plancher de bruit approximatif, downmix stéréo vers mono.

Validation :
- Test CTest `frontend.signal_analysis` : RÉUSSI (signal vide, sample rate invalide, constant, clipping, silence, segments, bruit, downmix).
- Les structures `AudioSource` et `AudioAnalysisResult` correspondent aux sections 5 et 6 du présent document.

Reste à implémenter (phase 1) :
- Décodage et import réels de fichiers WAV/MP3/M4A (adossé à JUCE `juce_audio_formats`).
- Lecture audio et production de la version mono d'analyse.
- Alimentation réelle de `AudioAnalysisResult` (durée, sample rate, canaux, diagnostics).

======================================================================
99. ETAT D'IMPLEMENTATION - IMPORT ET METADONNEES T-101.4
======================================================================

Date : 2026-08-14

La présente section actualise la section 98 :

- `AudioSource` est alimentée par l'import WAV réel ; `id` vaut `audio-<sha256>` et `fileHash` contient les 64 chiffres hexadécimaux SHA-256 du contenu.
- `importedAt` est un horodatage ISO 8601 généré au moment de l'import, et non une date fixe.
- `AudioAnalysisResult` reçoit durée, sample rate, clipping, bruit approximatif, présence vocale, qualité et carte de silence.
- Les métadonnées sont sérialisables en JSON ; les nombres non finis sont écrits `null` pour préserver un JSON valide.
- Le fichier est haché avant et après décodage ; l'import échoue si le contenu change entre les deux lectures.

Limites de modèle encore ouvertes :

- `monoAnalysisPath` reste vide car aucun artefact mono persistant n'est encore produit ;
- `analysisSampleRate` vaut 16 000 Hz ; `analysisVersion=2` identifie l'ancienne interpolation linéaire et `analysisVersion=3` le sinc/Blackman polyphasé anti-repliement ;
- `warnings` n'est pas encore alimenté ;
- `AudioFormat::M4a` exprime un format visé, pas une preuve que le codec est disponible sous Windows ;
- le schéma JSON n'a pas encore de numéro de version racine ni de migration ; il ne doit donc pas être considéré comme format projet stable.

## 100. Adaptateur de décodage MP3 - T-101.8

`DecodedAudioData` est une structure interne et non persistante. Elle transporte uniquement `sampleRate`, `channelCount`, `frameCount` et le signal mono nécessaire à l'analyse, afin de borner les copies mémoire. Après un décodage MP3 réussi, l'importeur crée une `AudioSource` avec `format=Mp3`. Cette valeur est attribuée seulement au chemin d'extension MP3 validé ; elle ne doit pas être inférée d'un repli de codec appliqué à une autre extension.

`AudioFormat::M4a` reste une intention de schéma. Aucun objet M4A ne doit être créé tant qu'un décodeur M4A réel, testé et audité n'est pas intégré.

## 101. Etat d'implémentation - socle pitch phase 2 (T-102)

Date : 2026-08-15

Structures pitch conformes aux sections 8, 9 et 10, introduites dans `src/common` (`Pitch.h` / `Pitch.cpp`, domaine commun sans JUCE) :

- `PitchFrame` (section 8) : timeSeconds (`Seconds`), frequencyHz (strictement positive), midiFloat (`MidiPitch`), confidence (`Score01`), voicedProbability (`Probability`), estimatorId (non vide). Création via `PitchFrame::create` qui rejette les valeurs hors bornes. Helper `frequencyHzToMidi` : `69 + 12 * log2(f / 440)`.
- `PitchCandidate` (section 9) : midiFloat (`MidiPitch`), frequencyHz (strictement positive), probability (`Probability`), sourceEstimators, octaveAmbiguity.
- `PitchDistributionFrame` (section 10) : timeSeconds (`Seconds`), candidates, fusedConfidence (`Score01`), voicedProbability (`Probability`).

Interface unique (architecture §10), module `src/pitch` indépendant de JUCE :

- `IPitchEstimator` : `estimate(MonoSignal) -> std::vector<PitchFrame>` et `id()` ;
- `MonoSignal` : `{ samples mono, sampleRate }` ;
- baseline `AutocorrelationPitchEstimator` (fenêtre 2048, hop 1024, plage inclusive 80-2000 Hz) : premier pic local de l'autocorrélation normalisée au-dessus du seuil de voicing 0,8, pour éviter le biais sous-harmonique du pic global. T-102.1 calcule un voisin de comparaison de part et d'autre de la plage et borne le stockage des corrélations à la fenêtre.

Validation :

- Test CTest `pitch.structures` : RÉUSSI (bornes des structures, `frequencyHzToMidi`, `MidiPitch`, `Cents`).
- Test CTest `pitch.autocorrelation` : RÉUSSI (sinus 440 Hz à 16 kHz détecté dans ±10 Hz, bornes 80/2000 Hz, signal vide, sample rate invalide, usage via l'interface).
- Build Debug : réussi, aucun avertissement ; CTest global 8/8 ; clang-format 35/35.

Limites du socle (le benchmark reste à faire, feuille de route phase 2) :

- l'estimateur autocorrélation est une baseline de référence, pas un candidat final ;
- aucune fusion (`PitchDistributionFrame` produite par fusion de plusieurs estimateurs) ni métrique de benchmark n'est encore implémentée.
