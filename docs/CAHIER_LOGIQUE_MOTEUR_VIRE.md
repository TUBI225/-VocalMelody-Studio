CAHIER DE LOGIQUE DU MOTEUR
PROJET : GÉNÉRATEUR DE MÉLODIE / ACCORDS / ARPÈGES
MOTEUR CENTRAL : VIRE — Vocal Intent Reconstruction Engine
Version de conception : 1.0

======================================================================
1. OBJECTIF DU MOTEUR
======================================================================

VIRE n’a pas pour mission de "corriger une voix".

Sa mission est :

Construire plusieurs interprétations musicales plausibles de ce que la personne a voulu
exprimer, tout en conservant intégralement la trace de ce qu’elle a réellement chanté.

Le moteur doit pouvoir fonctionner avec une performance :
- bonne ;
- approximative ;
- fausse ;
- irrégulière ;
- non métronomique ;
- bruitée ;
- expressive.

======================================================================
2. ARCHITECTURE GÉNÉRALE
======================================================================

A CAPPELLA
↓
AUDIO FRONTEND
↓
PERFORMANCE LAYER — L0
↓
NOTE LATTICE — L1
↓
MELODIC INTENTION — L2
↓
TIME & STRUCTURE ENGINE
↓
TONAL HYPOTHESIS ENGINE
↓
HARMONY REASONING ENGINE
↓
MULTI-CANDIDATE GENERATION
↓
GLOBAL SCORING
↓
DIVERSITY ENGINE
↓
5 PROPOSITIONS
↓
ARRANGEMENT ENGINE
↓
COMPATIBILITY CHECKER
↓
VALIDATION / REJET / RE-GÉNÉRATION
↓
MIDI + PREVIEW AUDIO

======================================================================
3. ÉTAGE 0 — AUDIO FRONTEND
======================================================================

Entrée :
fichier vocal brut.

Objectifs :
- charger l’audio ;
- mesurer sa qualité ;
- produire une version d’analyse propre ;
- conserver l’original intact.

Données produites :
- sample rate ;
- canaux ;
- durée ;
- SNR estimé ;
- clipping ;
- zones silencieuses ;
- zones vocales probables ;
- qualité générale.

Le moteur ne doit pas appliquer de débruitage agressif avant de vérifier s’il détruit
les harmoniques utiles à la détection F0.

======================================================================
4. ÉTAGE 1 — PERFORMANCE LAYER L0
======================================================================

L0 représente la vérité acoustique.

Pour chaque trame temporelle :

L0(t) = {
  time,
  F0_candidates,
  F0_confidence,
  energy,
  spectral_features,
  voicing_probability,
  onset_probability
}

On conserve :
- fréquence réelle ;
- micro-déviations ;
- vibrato ;
- glissando ;
- transitions ;
- bruit ;
- respirations ;
- instabilité.

RÈGLE :
L0 n’est jamais écrasé par une correction.

======================================================================
5. ÉTAGE 2 — FUSION DE PITCH
======================================================================

Le système doit permettre plusieurs estimateurs.

Exemple :
RMVPE
pYIN
CREPE
YIN

Pour chaque frame :
1. récupérer les candidats ;
2. comparer les estimateurs ;
3. détecter les erreurs d’octave ;
4. calculer une confiance fusionnée ;
5. conserver les désaccords entre estimateurs.

Exemple :

RMVPE : G4 0,88
pYIN : G4 0,71
CREPE : G#4 0,52

Sortie :
G4 : 0,74
G#4 : 0,19
autre : 0,07

Les coefficients exacts devront être calibrés sur un dataset de voix.

======================================================================
6. ÉTAGE 3 — SEGMENTATION VOCALE
======================================================================

Découper la performance en événements.

Événements :
- note stable ;
- transition ;
- glissando ;
- vibrato ;
- consonne ;
- respiration ;
- silence ;
- parlé ;
- chanté ;
- onset ;
- offset.

La segmentation utilise :
- changements de F0 ;
- énergie ;
- onsets ;
- stabilité ;
- durée ;
- changements spectraux.

Ne pas couper un vibrato en plusieurs notes.

Ne pas considérer toutes les hauteurs traversées par un glissando comme des notes
structurelles.

======================================================================
7. ÉTAGE 4 — NOTE LATTICE L1
======================================================================

Chaque segment reçoit plusieurs notes candidates.

Exemple :

Segment 12
Durée : 430 ms
F0 médian : 388 Hz

Hypothèses :
G4 : 0,55
G#4 : 0,28
F#4 : 0,17

Structure :

NoteNode {
  segment_id
  pitch_candidate
  probability
  start_perf
  end_perf
  cents_deviation
  stability_score
  acoustic_confidence
  structural_weight
  role_candidates
}

Les nodes sont reliés par des transitions.

Une transition contient :
- intervalle ;
- probabilité ;
- coût mélodique ;
- coût de rupture ;
- compatibilité avec le contour ;
- compatibilité avec la phrase.

Le Lattice doit pouvoir conserver N chemins.

======================================================================
8. STABILITÉ, CONFIANCE, JUSTESSE ET IMPORTANCE
======================================================================

Ne jamais confondre :

1. confiance du détecteur ;
2. stabilité acoustique ;
3. justesse ;
4. importance structurelle.

Exemple :

F0 confidence = 0,93
Stability = 0,91
Pitch deviation = -65 cents
Structural importance = 0,85

Cette note est acoustiquement fiable et importante,
même si elle est fausse.

Le moteur doit donc envisager qu’il s’agit d’une note cible mal atteinte.

======================================================================
9. ÉTAGE 5 — MELODIC INTENTION L2
======================================================================

L2 ne doit pas dépendre uniquement des hauteurs absolues.

Il décrit :
- montée ;
- descente ;
- répétition ;
- saut ;
- arche ;
- note tenue ;
- motif ;
- rythme relatif ;
- densité ;
- respiration ;
- climax ;
- tension ;
- résolution.

Exemple :

Phrase 4 :
[montée légère]
→ [répétition]
→ [grand saut]
→ [note longue]
→ [descente]
→ [respiration]

Même si les hauteurs sont fausses, cette structure peut rester très fiable.

L2 doit servir à empêcher le moteur de réécrire totalement l’identité de la chanson.

======================================================================
10. ÉTAGE 6 — PHRASE ANALYSIS
======================================================================

Créer les frontières de phrase à partir de :
- respirations ;
- silences ;
- changement d’énergie ;
- fin de motif ;
- notes longues ;
- répétitions ;
- contour.

Chaque phrase reçoit :
- phrase_id ;
- start/end ;
- contour ;
- densité ;
- registre ;
- climax ;
- terminaison ;
- motifs ;
- poids structurel.

======================================================================
11. ÉTAGE 7 — TIME & STRUCTURE ENGINE
======================================================================

Deux horloges :

t_perf = temps original.
t_music = temps musical.

Créer :
W(t_perf) -> t_music

W doit être monotone.

Elle doit permettre :
- tempo moyen ;
- variations locales ;
- rubato ;
- accelerando ;
- ritardando ;
- groove offset ;
- barlines ;
- beats ;
- subdivisions.

Le système ne doit pas quantifier brutalement la voix.

Chaque événement conserve :
- position théorique ;
- position réelle ;
- écart de groove.

======================================================================
12. DÉTECTION DU TEMPO
======================================================================

Entrées :
- onsets syllabiques ;
- accents ;
- respirations ;
- durées ;
- motifs répétés.

Étapes :
1. calcul IOI ;
2. histogramme ;
3. autocorrélation ;
4. hypothèses de tempo ;
5. hypothèses de métrique ;
6. programmation dynamique ;
7. TimeWarpMap ;
8. validation au niveau phrase.

Si aucune grille fiable :
mode FREE TIME.

Dans ce mode :
- pads ;
- accords longs ;
- accompagnement moins métrique ;
- utilisateur peut imposer un tempo.

======================================================================
13. ÉTAGE 8 — TONAL HYPOTHESIS ENGINE
======================================================================

Le moteur tonal reçoit le Lattice, pas seulement une suite MIDI figée.

Pour chaque tonalité/mode :
calculer une probabilité.

Les notes sont pondérées selon :
- stabilité ;
- rôle ;
- durée utile ;
- accent ;
- fin de phrase ;
- importance structurelle.

Les notes incertaines pèsent moins.

Top-K conservé.

Exemple :
C major 0,69
A minor 0,65
G major 0,31
D dorian 0,22

Une tonalité relative ambiguë peut rester ambiguë.

======================================================================
14. INTERACTION TONALITÉ ↔ NOTES
======================================================================

Le raisonnement doit être bidirectionnel.

Exemple :
Le Lattice hésite :
G ou G#.

Le Tonal Engine hésite :
C major ou A minor.

Si plusieurs notes stables renforcent C major,
la branche G gagne du poids.

Mais aucune correction définitive n’est appliquée à L0.

Le moteur doit permettre plusieurs passes :

L1 initial
↓
Tonal hypotheses
↓
réévaluation de L1
↓
harmonie
↓
nouvelle cohérence globale

======================================================================
15. ÉTAGE 9 — HARMONY REASONING ENGINE
======================================================================

Le moteur harmonique travaille sur cinq niveaux :

NIVEAU A — NOTE
NIVEAU B — MESURE
NIVEAU C — PHRASE
NIVEAU D — SECTION
NIVEAU E — CHANSON ENTIÈRE

======================================================================
16. NIVEAU NOTE
======================================================================

Pour une note vocale donnée :
tester son rôle possible dans un accord :

- fondamentale ;
- tierce ;
- quinte ;
- septième ;
- neuvième ;
- onzième ;
- treizième ;
- suspension ;
- note de passage ;
- appoggiature ;
- chromatisme.

RÈGLE :
Une note vocale ne doit jamais être forcée à être la fondamentale.

Compatibilité locale :
expected_compatibility(chord, note_distribution)

Exemple :
Note :
E 0,54
F 0,41
D# 0,05

Score(chord) =
0,54 * compat(chord,E)
+
0,41 * compat(chord,F)
+
0,05 * compat(chord,D#)

======================================================================
17. NIVEAU MESURE
======================================================================

Agréger toutes les notes de la mesure.

Pondération :
- structural_weight ;
- position métrique ;
- stabilité ;
- confiance ;
- rôle.

Une note de passage courte ne doit pas pouvoir rejeter un accord très cohérent.

Le moteur doit aussi tester plusieurs rythmes harmoniques :
- 1 accord / 2 mesures ;
- 1 accord / mesure ;
- 2 accords / mesure ;
- changements irréguliers.

======================================================================
18. NIVEAU PHRASE
======================================================================

La phrase doit posséder une direction harmonique.

Détecter :
- stabilité initiale ;
- montée de tension ;
- climax ;
- préparation ;
- résolution.

Le moteur doit évaluer une progression sur la phrase entière.

Une progression localement correcte mais qui ne résout pas la phrase doit être pénalisée.

======================================================================
19. NIVEAU SECTION
======================================================================

Identifier ou estimer :
- couplet ;
- pré-refrain ;
- refrain ;
- pont ;
- intro ;
- outro.

Si la classification est incertaine :
utiliser des labels neutres :
Section A / B / C.

Chaque section reçoit :
- énergie ;
- densité ;
- registre vocal ;
- répétitivité ;
- tension ;
- rôle structurel.

L’harmonie doit pouvoir devenir plus forte au refrain sans changer nécessairement de
tonalité.

======================================================================
20. NIVEAU CHANSON
======================================================================

Évaluer :
- répétition ;
- cohérence ;
- architecture globale ;
- tensions ;
- résolutions ;
- contraste entre sections ;
- continuité de basse ;
- modulation ;
- monotonie.

Une progression excellente mesure par mesure peut être rejetée si elle n’a aucune
direction globale.

======================================================================
21. GÉNÉRATION DE CANDIDATS HARMONIQUES
======================================================================

Pour chaque position harmonique :
produire un ensemble de candidats.

Selon niveau de complexité :

Niveau 1 :
triades diatoniques.

Niveau 2 :
7e + inversions.

Niveau 3 :
add9 / sus / extensions simples.

Niveau 4 :
emprunts modaux / dominantes secondaires.

Niveau 5 :
réharmonisation avancée.

======================================================================
22. BEAM SEARCH / VITERBI / CSP
======================================================================

Répartition proposée :

HMM :
modèle probabiliste note/performance.

Viterbi :
meilleur chemin lorsque nécessaire.

N-best Viterbi / Beam Search :
conserver plusieurs solutions.

Beam Search harmonique :
explorer les progressions.

CSP/Z3 :
contraintes dures :
- accord verrouillé ;
- tonalité verrouillée ;
- note interdite ;
- cadence imposée ;
- limites utilisateur.

Ces technologies sont complémentaires.

======================================================================
23. SCORE D’UN ACCORD
======================================================================

Un score local peut contenir :

S_local =
VocalCompatibility
+ MetricFit
+ StructuralNoteFit
+ GenreFit
+ TonalFit

Mais ce score n’est jamais suffisant pour valider l’accord.

======================================================================
24. SCORE D’UNE TRANSITION
======================================================================

S_transition =
FunctionalHarmony
+ VoiceLeading
+ BassMotion
+ CommonTones
+ GenreTransition
- AbruptJump
- UnwantedParallelMotion

Les règles varient selon le genre.

======================================================================
25. SCORE D’UNE PHRASE
======================================================================

S_phrase =
LocalCompatibility
+ TensionShape
+ CadenceQuality
+ MelodicIntentionSupport
+ RhythmSupport
+ BreathingSupport

======================================================================
26. SCORE D’UNE SECTION
======================================================================

S_section =
PhraseCoherence
+ EnergyDirection
+ HarmonicVariety
+ RepetitionBalance
+ GenreCoherence

======================================================================
27. SCORE GLOBAL
======================================================================

Le score global doit éviter qu’un critère excellent cache un défaut grave.

Approche à tester :

1. normalisation [0,1] ;
2. seuils minimum obligatoires ;
3. moyenne géométrique pour les critères essentiels ;
4. pénalités séparées ;
5. Pareto pour diversité.

Exemple :

CoreScore =
geometric_mean(
  VocalFit,
  HarmonicCoherence,
  IntentionPreservation,
  RhythmCoherence,
  VoiceLeading,
  GlobalStructure
)

FinalScore =
CoreScore
+ GenreBonus
+ DiversityBonus
- CollisionPenalty
- OverCorrectionPenalty
- OverloadPenalty
- MonotonyPenalty

======================================================================
28. GÉNÉRATION DE PLUSIEURS PROPOSITIONS
======================================================================

Générer 50 à 500 candidats internes selon complexité.

Filtrer :
1. invalides ;
2. trop faibles ;
3. trop similaires ;
4. incohérents globalement.

Créer ensuite des familles :

A — fidèle/simple
B — émotionnelle
C — moderne
D — minimaliste
E — genre/réharmonisation

Ne pas forcer E si aucun candidat de qualité suffisante.

======================================================================
29. DIVERSITY ENGINE
======================================================================

Distance(P1,P2) basée sur :

- EditDistance accords ;
- fonctions harmoniques ;
- basse ;
- densité ;
- rythme harmonique ;
- tension ;
- renversements ;
- extensions ;
- structure ;
- couleur de genre.

Méthodes à tester :
- K-Medoids ;
- DPP ;
- greedy max-distance ;
- clustering + meilleur score de chaque cluster.

Le moteur final choisit la méthode qui produit le meilleur compromis qualité/diversité.

======================================================================
30. ARRANGEMENT ENGINE — BASSE
======================================================================

Entrées :
- progression ;
- genre ;
- tempo ;
- voix ;
- groove ;
- section.

La basse est générée après les accords, mais peut provoquer un retour sur le voicing.

Règles :
- fondamentale non obligatoire ;
- inversions ;
- notes de passage ;
- anticipation ;
- syncopes ;
- silence ;
- répétition ;
- ligne mélodique.

======================================================================
31. ARRANGEMENT ENGINE — ARPÈGES
======================================================================

Arpège = grammaire musicale, pas simple pattern fixe.

Entrées :
- accord ;
- voicing ;
- basse ;
- genre ;
- densité vocale ;
- registre ;
- groove.

VoiceAwareMask :
si voix dense -> arpège réduit.
si voix sparse -> arpège plus riche.
si silence -> fill possible.
si note longue -> mouvement complémentaire possible.

======================================================================
32. ARRANGEMENT ENGINE — CONTRE-MÉLODIE
======================================================================

Étapes :
1. détecter fenêtre d’opportunité ;
2. analyser phrase vocale précédente ;
3. générer réponse ;
4. contraindre aux accords ;
5. éviter collision ;
6. évaluer musicalité.

Transformations possibles :
- imitation ;
- inversion ;
- simplification ;
- augmentation ;
- diminution ;
- variation rythmique ;
- réponse contrastante.

======================================================================
33. GENRE ENGINE
======================================================================

Le genre est une lentille de paramètres.

Il modifie :
- vocabulaire harmonique ;
- poids de scoring ;
- rythme harmonique ;
- basse ;
- swing ;
- syncope ;
- arpèges ;
- espace vocal ;
- densité.

Il ne doit pas être seulement un choix d’instruments.

======================================================================
34. COMPATIBILITY CHECKER
======================================================================

Après arrangement :
tester l’ensemble.

Vérifier :
- accord/voix ;
- basse/accord ;
- basse/voix ;
- arpège/voix ;
- contre-mélodie/voix ;
- saturation de registre ;
- densité ;
- espace ;
- groove ;
- structure.

Une proposition peut être :
VALIDÉE
À CORRIGER
REJETÉE

======================================================================
35. BOUCLE DE CORRECTION
======================================================================

Si erreur locale :
recalcul local.

Si erreur phrase :
recalcul de la phrase.

Si erreur section :
recalcul de la section.

Si incohérence globale :
nouvelle progression.

Le moteur ne doit pas relancer la détection F0 sauf si la source audio change.

======================================================================
36. CONTRAINTES UTILISATEUR
======================================================================

Chaque objet peut avoir :

locked = true/false
strength = 0..1

Exemples :
- note verrouillée ;
- accord verrouillé ;
- tonalité verrouillée ;
- genre verrouillé ;
- tempo verrouillé.

Contrainte dure :
doit être respectée.

Contrainte souple :
entre dans le scoring.

======================================================================
37. CACHE ET DÉPENDANCES
======================================================================

Construire un graphe de dépendances.

Exemple :
Audio
→ L0
→ L1
→ L2
→ Key
→ Harmony
→ Arrangement

Si changement accord mesure 4 :
invalider uniquement les descendants concernés.

======================================================================
38. TRACES DE DÉCISION
======================================================================

Chaque décision doit stocker :

DecisionTrace {
  selected
  alternatives
  scores
  constraints
  confidence
  reasons
  rejection_reasons
}

But :
- débogage ;
- explicabilité ;
- comparaison versions ;
- apprentissage futur.

======================================================================
39. CAS DU CHANT TRÈS FAUX
======================================================================

Si confiance acoustique bonne mais cohérence tonale faible :
suspecter mauvaise justesse systématique.

Si confiance acoustique faible :
élargir le Lattice.

Si tout est incertain :
basculer progressivement vers :
- contour ;
- rythme ;
- respirations ;
- points d’appui ;
- tonalité globale.

Ne jamais inventer avec une confiance artificielle élevée.

======================================================================
40. CAS D’UNE NOTE HORS GAMME
======================================================================

Ne pas corriger automatiquement.

Tester :
- note stable ?
- longue ?
- accentuée ?
- répétée ?
- résout par demi-ton ?
- cohérente avec genre ?
- compatible avec accord emprunté ?

Si oui :
chromatisme intentionnel probable.

======================================================================
41. CAS DU VIBRATO
======================================================================

Détecter périodicité.

Extraire centre du vibrato.

L0 conserve oscillation.

L1 représente note cible.

L2 conserve expressivité.

======================================================================
42. CAS DU GLISSANDO
======================================================================

Détecter :
- pente ;
- durée ;
- régularité.

Ne garder comme notes structurelles que :
- départ ;
- arrivée ;
- éventuels points réellement tenus.

Le chemin intermédiaire reste dans L0.

======================================================================
43. CAS DU RAP MÉLODIQUE
======================================================================

Classifier zones :
- parlé ;
- pitch vague ;
- chanté.

Sous une zone parlée :
réduire poids du pitch.
Augmenter poids :
- rythme ;
- accent ;
- phrase ;
- section.

======================================================================
44. CAS SANS TEMPO CLAIR
======================================================================

Conserver plusieurs tempo hypotheses.

Si aucune suffisante :
Free Time.

L’arrangement devient :
- pads ;
- drones ;
- accords longs ;
- motifs libres.

======================================================================
45. VALIDATION EXPÉRIMENTALE
======================================================================

Ne pas choisir définitivement les algorithmes par théorie.

Construire un dataset de test avec :
- voix juste ;
- voix fausse ;
- voix très fausse ;
- hommes ;
- femmes ;
- graves ;
- aigus ;
- téléphone ;
- bruit ;
- vibrato ;
- glissando ;
- Afro ;
- R&B ;
- Pop ;
- Gospel ;
- rap mélodique.

Comparer :
RMVPE
pYIN
CREPE
fusion.

Comparer aussi :
K-Medoids
DPP
greedy diversity.

Comparer :
Beam Search
Viterbi
CSP.

======================================================================
46. PRINCIPE FINAL
======================================================================

Le logiciel doit fonctionner comme un assistant compositeur.

Il ne doit pas dire :
"tu as chanté cette note, donc je t’impose cette harmonie."

Il doit raisonner :

"voici ce que j’ai réellement entendu ;
voici plusieurs choses que tu as probablement voulu faire ;
voici comment ces hypothèses s’intègrent dans le rythme, la phrase, la section et
la chanson entière ;
voici plusieurs harmonisations possibles ;
voici celles qui donnent les meilleurs résultats sans détruire ton identité vocale."

FIN DU CAHIER DE LOGIQUE DU MOTEUR VIRE