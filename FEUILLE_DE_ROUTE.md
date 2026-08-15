PLAN DE DEVELOPPEMENT ET DE TESTS
PROJET : GENERATEUR DE MELODIE / ACCORDS / ARPEGES A PARTIR D'UNE VOIX A CAPPELLA
MOTEUR CENTRAL : VIRE — Vocal Intent Reconstruction Engine
Version : 1.0
Plateforme cible : Windows x64

======================================================================
1. OBJECTIF DU DOCUMENT
======================================================================

Ce document définit :
- l'ordre de développement ;
- les jalons ;
- les dépendances entre étapes ;
- les critères de validation ;
- les jeux de tests ;
- les benchmarks ;
- les tests musicaux ;
- les règles de passage d'une phase à la suivante.

Le principe est simple :

NE PAS construire tout le logiciel d'un seul bloc.

Chaque moteur doit être :
1. développé ;
2. testé seul ;
3. mesuré ;
4. validé ;
5. intégré ;
6. re-testé dans le pipeline complet.

======================================================================
2. REGLE PRINCIPALE DE DEVELOPPEMENT
======================================================================

Aucune phase ne doit être considérée terminée uniquement parce que :
"le code fonctionne".

Une phase est terminée lorsque :
- les tests unitaires passent ;
- les tests d'intégration passent ;
- les cas difficiles ont été essayés ;
- les performances sont acceptables ;
- les résultats musicaux atteignent un seuil minimal ;
- les limites sont documentées.

======================================================================
3. ORDRE GENERAL
======================================================================

PHASE 0 — Préparation du projet
PHASE 1 — Audio Frontend
PHASE 2 — Pitch Benchmark
PHASE 3 — L0 Performance Layer
PHASE 4 — Segmentation
PHASE 5 — L1 Note Lattice
PHASE 6 — L2 Melodic Intention
PHASE 7 — Time & Structure Engine
PHASE 8 — Tonal Engine
PHASE 9 — Harmony Engine MVP
PHASE 10 — Harmony Multi-Hypothèses
PHASE 11 — Diversity Engine
PHASE 12 — Arrangement Engine
PHASE 13 — Compatibility / Repair Loop
PHASE 14 — Contraintes utilisateur
PHASE 15 — MIDI / Preview
PHASE 16 — UI Windows
PHASE 17 — Optimisation
PHASE 18 — Validation musicale globale

======================================================================
4. PHASE 0 — PREPARATION DU PROJET
======================================================================

Objectifs :
- créer le dépôt ;
- créer l'arborescence ;
- installer CI ;
- définir les conventions ;
- préparer les docs ;
- préparer les fichiers de suivi.

Documents obligatoires :
- CAHIER_DES_CHARGES
- CAHIER_LOGIQUE_MOTEUR_VIRE
- ARCHITECTURE_TECHNIQUE
- MODELISATION_DONNEES_MUSICALES
- PLAN_DEVELOPPEMENT_ET_TESTS
- SUIVI_DEVELOPPEMENT
- DECISIONS_ARCHITECTURE
- DEPENDANCES
- ERREURS_CONNNUES
- PERFORMANCES

Critère de sortie :
- build minimal ;
- tests minimal ;
- CI verte ;
- structure stable.

======================================================================
5. PHASE 1 — AUDIO FRONTEND
======================================================================

Fonctions :
- import WAV ;
- import MP3 ;
- import M4A ;
- lecture ;
- mono analysis ;
- resampling ;
- détection durée ;
- détection silence ;
- clipping ;
- niveau ;
- bruit approximatif.

Tests unitaires :
- fichier vide ;
- fichier court ;
- fichier long ;
- stéréo ;
- mono ;
- sample rates différents ;
- fichiers corrompus.

Tests intégration :
- import → analyse → sauvegarde metadata.

Critère de sortie :
- aucun crash sur corpus test ;
- diagnostics fiables ;
- audio original jamais modifié.

======================================================================
6. PHASE 2 — PITCH BENCHMARK
======================================================================

Objectif :
ne PAS choisir RMVPE/CREPE/pYIN/YIN par intuition.

Tester :
- RMVPE ;
- CREPE ;
- CREPE Tiny ;
- pYIN ;
- YIN ;
- fusion de 2 estimateurs ;
- fusion de 3 estimateurs.

Dataset pitch :
- voix juste ;
- voix légèrement fausse ;
- voix très fausse ;
- voix grave ;
- voix aiguë ;
- vibrato léger ;
- vibrato fort ;
- glissando ;
- rap mélodique ;
- téléphone ;
- bruit ;
- souffle ;
- voix faible ;
- voix forte.

Mesures :
- erreur moyenne en cents ;
- median cents error ;
- gross pitch error ;
- octave error rate ;
- voicing accuracy ;
- stabilité ;
- CPU time ;
- RAM ;
- modèle size.

Critère de sortie :
sélectionner :
- estimateur FAST ;
- estimateur BALANCED ;
- estimateur HIGH QUALITY.

======================================================================
7. CREATION DU DATASET DE TEST VOCAL
======================================================================

Créer une bibliothèque interne structurée.

Chaque exemple possède :

TestCase {
  id
  audio
  singerProfile
  recordingType
  expectedMainNotes
  acceptableAlternatives
  phraseBoundaries
  tempoInfo
  keyCandidates
  annotations
}

Profils :
- homme grave ;
- homme medium ;
- femme medium ;
- femme aiguë ;
- amateur ;
- confirmé.

Ne pas limiter le dataset à des chanteurs justes.

======================================================================
8. PHASE 3 — L0 PERFORMANCE LAYER
======================================================================

Objectif :
construire la vérité acoustique.

Contenu :
- pitch distributions ;
- énergie ;
- voicing ;
- onsets ;
- breaths ;
- vibrato ;
- glissando ;
- segments préliminaires.

Tests :
- sérialisation ;
- immutabilité ;
- reproductibilité ;
- précision temporelle.

Critère de sortie :
L0 stable, versionné et cacheable.

======================================================================
9. PHASE 4 — SEGMENTATION
======================================================================

Objectif :
ne pas découper les mauvais événements.

Cas tests :
- note tenue ;
- note tenue avec vibrato ;
- glissando ;
- deux notes liées ;
- consonne forte ;
- respiration ;
- silence court ;
- silence long ;
- parlé ;
- chanté.

Mesures :
- onset precision ;
- onset recall ;
- F1 ;
- segmentation over-split ;
- segmentation under-split.

Critère :
le système ne transforme pas le vibrato en succession de notes.

======================================================================
10. PHASE 5 — L1 NOTE LATTICE
======================================================================

Objectif :
conserver plusieurs notes possibles.

Tests synthétiques :
- 1 note certaine ;
- 2 notes ambiguës ;
- erreur d'octave ;
- note entre 2 demi-tons ;
- note longue fausse ;
- notes courtes de passage.

Vérifier :
- somme des probabilités ;
- cohérence graphe ;
- top-N paths ;
- pruning ;
- aucun cycle ;
- recalcul local.

Critère de sortie :
sur voix imparfaite, le bon chemin doit rester présent dans le top-N même s'il n'est
pas systématiquement top-1.

======================================================================
11. METRIQUE CRITIQUE DU LATTICE
======================================================================

Top-K Recall.

Exemple :
si la note cible réelle est E,
et L1 propose :
F 0,41
E 0,36
Eb 0,23

Top-1 = faux.
Top-3 = correct.

Le moteur doit mesurer :
Top1 accuracy
Top2 recall
Top3 recall
Top5 recall

Pour VIRE, Top-K Recall est souvent plus important que Top-1.

======================================================================
12. PHASE 6 — L2 MELODIC INTENTION
======================================================================

Objectifs :
- contour ;
- répétition ;
- rythme relatif ;
- motifs ;
- anchors ;
- climax ;
- respiration ;
- phrase role.

Tests :
prendre la même mélodie :
- bien chantée ;
- 30 cents trop bas ;
- 60 cents trop haut ;
- avec vibrato ;
- avec rubato.

L2 devrait rester similaire.

Mesure :
Intention Invariance Score.

Critère :
une variation de justesse ne doit pas détruire l'identité L2.

======================================================================
13. PHASE 7 — TIME ENGINE
======================================================================

Tests :
- métronome parfait ;
- tempo légèrement fluctuant ;
- rubato ;
- accélération ;
- ralentissement ;
- 4/4 ;
- 3/4 ;
- 6/8 ;
- aucune pulsation claire.

Mesures :
- beat alignment error ;
- bar alignment error ;
- tempo error ;
- groove preservation.

Critère :
le TimeWarpMap doit être monotone et stable.

======================================================================
14. TEST DU GROOVE
======================================================================

Comparer :
A. quantification stricte ;
B. TimeWarpMap + groove offset.

Faire écouter à des musiciens.

Question :
quelle version conserve mieux la manière de chanter ?

Le résultat doit être documenté.

======================================================================
15. PHASE 8 — TONAL ENGINE
======================================================================

Tests :
- C major évident ;
- A minor évident ;
- C major / A minor ambigu ;
- modal ;
- chromatique ;
- modulation ;
- chant faux.

Mesures :
- Top1 key accuracy ;
- Top3 key recall ;
- calibration des probabilités.

Critère :
en cas d'ambiguïté, le moteur doit conserver plusieurs tonalités au lieu de forcer.

======================================================================
16. PHASE 9 — HARMONY ENGINE MVP
======================================================================

Version initiale :
- triades diatoniques ;
- 1 accord par mesure ;
- tonalité unique ;
- Beam Search simple ;
- vocal compatibility ;
- transition score ;
- voice leading basique.

Tests :
mélodies synthétiques connues.

Critère :
produire au moins une progression musicalement correcte et stable.

======================================================================
17. PHASE 10 — HARMONY MULTI-HYPOTHESES
======================================================================

Ajouter :
- tonalités multiples ;
- note distributions ;
- rythme harmonique variable ;
- 7e ;
- inversions ;
- extensions simples ;
- borrowed chords ;
- dominantes secondaires.

Recherche :
- Beam Search widths 5/10/20/50/100 ;
- N-best Viterbi ;
- contraintes.

Mesures :
- qualité musicale ;
- runtime ;
- mémoire ;
- taux de propositions invalides.

======================================================================
18. TEST NOTE → MESURE → PHRASE → SECTION → CHANSON
======================================================================

Créer volontairement une progression :
- bonne localement ;
- mauvaise globalement.

Le moteur global doit la rejeter.

Créer une progression :
- accord local légèrement inférieur ;
- meilleure cadence globale.

Le moteur doit pouvoir la préférer.

======================================================================
19. HUMAN HARMONY EVALUATION
======================================================================

Chaque candidat évalué par plusieurs personnes.

Critères notés 1-5 :
- accords justes ;
- naturel ;
- émotion ;
- soutien de la voix ;
- cohérence ;
- originalité ;
- envie de garder la proposition.

Ne jamais utiliser uniquement le score algorithmique.

======================================================================
20. PHASE 11 — DIVERSITY ENGINE
======================================================================

Comparer :
- greedy max-distance ;
- K-Medoids ;
- DPP ;
- clustering + best-of-cluster.

Tests :
générer 100 candidats.

Mesures :
- average pairwise distance ;
- quality loss ;
- duplicate rate ;
- human perceived diversity.

Critère :
5 propositions perçues comme différentes sans sacrifier fortement la qualité.

======================================================================
21. REGLE DE DIVERSITE
======================================================================

La diversité ne doit jamais permettre une mauvaise proposition uniquement pour
atteindre le nombre 5.

Définir :
MIN_QUALITY_THRESHOLD

Si seulement 3 propositions dépassent ce seuil :
afficher 3.

======================================================================
22. PHASE 12 — BASS ENGINE
======================================================================

Niveaux :
1. fondamentale ;
2. inversions ;
3. passing notes ;
4. syncopes ;
5. bassline mélodique.

Tests :
- collision avec voix ;
- collision avec chord voicing ;
- genre ;
- mouvement.

Critère :
la basse doit améliorer la proposition, jamais la rendre moins stable.

======================================================================
23. PHASE 12B — ARPEGGIO ENGINE
======================================================================

Tester :
- patterns fixes ;
- grammaires ;
- voice-aware density ;
- variations.

Mesures :
- répétitivité ;
- collisions ;
- densité ;
- adéquation genre.

Critère :
pas de boucle 1-3-5-3 systématique.

======================================================================
24. PHASE 12C — COUNTER-MELODY
======================================================================

Tester :
- silence response ;
- note-longue response ;
- imitation ;
- inversion ;
- contraste.

Mesures :
- collision rate ;
- vocal masking ;
- phrase support ;
- human rating.

======================================================================
25. PHASE 13 — COMPATIBILITY CHECKER
======================================================================

Créer des erreurs volontairement :
- arpège dans même registre ;
- basse hors harmonie ;
- contre-mélodie en conflit ;
- densité excessive ;
- mauvaise cadence.

Le checker doit :
- détecter ;
- classer severity ;
- proposer scope de réparation.

======================================================================
26. PHASE 13B — REPAIR LOOP
======================================================================

Tester :
Issue locale
→ réparation locale.

Issue phrase
→ réparation phrase.

Issue globale
→ nouvelle progression.

Mesure :
pourcentage de repairs réussis sans toucher aux zones correctes.

======================================================================
27. PHASE 14 — CONTRAINTES UTILISATEUR
======================================================================

Tests :
- lock chord ;
- lock key ;
- lock tempo ;
- mark bad vocal ;
- mark good vocal ;
- avoid chord ;
- prefer genre.

Critère :
aucune contrainte dure ne doit être violée silencieusement.

======================================================================
28. PHASE 15 — MIDI ENGINE
======================================================================

Tests :
- tempo map ;
- time signature ;
- chord track ;
- bass ;
- arpeggio ;
- counter melody ;
- groove offsets.

Ouvrir exports dans :
- FL Studio ;
- Reaper ;
- Ableton si disponible ;
- autre DAW.

Critère :
timing identique et reproductible.

======================================================================
29. PHASE 15B — AUDIO PREVIEW
======================================================================

Le Preview Engine ne doit pas être utilisé pour juger la logique si le SoundFont est mauvais.

Tester séparément :
- timing ;
- note ;
- rendu ;
- latence.

======================================================================
30. PHASE 16 — UI WINDOWS
======================================================================

Mode simple :
- importer ;
- choisir genre ;
- générer ;
- écouter propositions ;
- modifier ;
- exporter.

Mode avancé :
- waveform ;
- pitch ;
- L1 ;
- L2 ;
- tonal hypotheses ;
- chord reasons ;
- locks.

Tests UX :
utilisateurs non musiciens.

======================================================================
31. TESTS DE RE-CALCUL LOCAL
======================================================================

Scénario :
changer accord mesure 8.

Mesurer :
- modules relancés ;
- durée ;
- modules non relancés.

Le Pitch Engine ne doit pas être relancé.

======================================================================
32. PHASE 17 — PERFORMANCE
======================================================================

Profiler :
- CPU ;
- RAM ;
- disk ;
- cache ;
- startup ;
- models ;
- beam search.

Objectifs :
- UI toujours responsive ;
- génération annulable ;
- cache fiable.

======================================================================
33. PROFILS MATERIELS DE TEST
======================================================================

Tester au minimum :

PC LOW
- 2-4 cores
- 8 GB RAM
- pas GPU dédié

PC TARGET
- 4 cores
- 16 GB RAM
- iGPU

PC HIGH
- 8+ cores
- 32 GB RAM

Mesurer tous les benchmarks sur ces profils.

======================================================================
34. PHASE 18 — VALIDATION MUSICALE GLOBALE
======================================================================

Corpus final :
- 100+ a cappella si possible ;
- plusieurs genres ;
- plusieurs niveaux ;
- plusieurs chanteurs.

Pour chaque morceau :
- analyser ;
- générer ;
- noter ;
- comparer.

======================================================================
35. CATEGORIES DE CHANT
======================================================================

A. juste
B. légèrement faux
C. faux mais contour clair
D. très faux
E. rap mélodique
F. parlé
G. vibrato
H. glissando
I. sans tempo
J. bruité

Les performances doivent être rapportées séparément.

======================================================================
36. NE PAS CACHER LES ECHECS
======================================================================

Pour chaque test :
- réussi ;
- partiel ;
- échoué.

Les échecs doivent entrer dans ERREURS_CONNNUES.

Aucune moyenne globale ne doit masquer une catégorie catastrophique.

======================================================================
37. BENCHMARK COMPARATIF PRODUITS EXISTANTS
======================================================================

Comparer fonctionnellement :
- MySong / Songsmith ;
- BandLab Audio-to-MIDI ;
- Dubler ;
- MIDI-SAG ;
- AccoMontage2 ;
- AutoHarmonizer.

Comparer :
- entrée ;
- chant faux ;
- tempo libre ;
- accords ;
- diversité ;
- MIDI ;
- contrôle ;
- explicabilité.

======================================================================
38. MESURE "INTENTION PRESERVATION"
======================================================================

Créer une métrique interne combinant :
- contour similarity ;
- phrase timing similarity ;
- breath preservation ;
- relative rhythm similarity ;
- anchor preservation.

Cette métrique n'est pas une vérité musicale.
Elle sert à comparer les versions du moteur.

======================================================================
39. MESURE "OVER-CORRECTION"
======================================================================

Calculer :
- % notes changées ;
- distance moyenne en cents ;
- modifications d'onsets ;
- modifications de durées ;
- modification de contour.

Un moteur qui produit de beaux accords mais réécrit la chanson doit être pénalisé.

======================================================================
40. MESURE "HARMONY DIVERSITY"
======================================================================

Inclure :
- chord edit distance ;
- function distance ;
- bass contour distance ;
- tension distance ;
- harmonic rhythm distance.

Rapporter :
- min ;
- mean ;
- max.

======================================================================
41. MESURE "VOICE SPACE"
======================================================================

Calculer :
- temps où voix et instrument partagent même registre ;
- densité instrumentale pendant voix active ;
- collisions exactes ;
- masking proxy.

======================================================================
42. TESTS DE REGRESSION
======================================================================

Tout bug musical corrigé devient un test.

Exemple :
Bug :
vibrato découpé en 7 notes.

Créer test :
test_vibrato_not_split.

Ce test ne doit plus jamais échouer.

======================================================================
43. GOLDEN FILES
======================================================================

Pour certains cas :
sauvegarder :
- L0 attendu ;
- L1 attendu ;
- tonal top-K ;
- harmonies acceptables ;
- exports.

Comparer après chaque changement d'algorithme.

======================================================================
44. A/B TESTS
======================================================================

Quand deux algorithmes sont plausibles :

A vs B.

Exemples :
RMVPE vs fusion.
Beam 20 vs Beam 50.
K-Medoids vs DPP.
Scoring arithmétique vs géométrique.

Ne pas décider sans données.

======================================================================
45. ABLATION TESTS
======================================================================

Retirer volontairement un composant.

Exemple :
sans L2.
sans TimeWarp.
sans structural weights.
sans diversity.

Mesurer la perte.

Cela permet de savoir quels modules apportent réellement de la valeur.

======================================================================
46. CALIBRATION DES PROBABILITES
======================================================================

Si le moteur dit :
"0,80"

Cela doit signifier quelque chose.

Tester calibration :
- reliability diagrams ;
- Brier score si applicable.

Eviter les probabilités arbitraires non calibrées.

======================================================================
47. DATASET D'ANNOTATION
======================================================================

À terme, construire un dataset propriétaire annoté.

Annotations :
- F0 target ;
- intended note ;
- phrase boundaries ;
- ornaments ;
- key ;
- chords acceptables ;
- rhythm anchors.

Important :
certaines annotations doivent accepter plusieurs réponses valides.

======================================================================
48. TESTS DE GENRE
======================================================================

Pour chaque profil :
- Pop ;
- R&B ;
- Soul ;
- Gospel ;
- Afrobeats ;
- Coupé-décalé ;
- Trap ;
- Ballade.

Tester :
- harmonie ;
- groove ;
- basse ;
- arpège ;
- densité.

Un genre ne doit pas être validé seulement parce que les instruments changent.

======================================================================
49. VALIDATION PAR MUSICIENS
======================================================================

Créer une grille.

1 à 5 :
- compréhension de la voix ;
- respect du phrasé ;
- qualité des accords ;
- diversité ;
- groove ;
- arpèges ;
- basse ;
- contre-mélodie ;
- utilité en production.

Recueillir commentaires libres.

======================================================================
50. VALIDATION PAR NON-MUSICIENS
======================================================================

Question principale :
"Est-ce que le logiciel a compris ce que vous vouliez chanter ?"

Autres :
- proposition préférée ;
- simplicité ;
- besoin de correction ;
- compréhension UI.

======================================================================
51. CRITERES DE PASSAGE MVP
======================================================================

MVP accepté si :

1. Import stable.
2. Pitch fiable sur voix propre.
3. L1 conserve bonnes alternatives.
4. Tempo correct sur chant simple.
5. Tonalité top-3 correcte.
6. Une harmonisation correcte.
7. Export MIDI correct.
8. Pas de crash.

======================================================================
52. CRITERES DE PASSAGE BETA
======================================================================

Beta accepté si :

1. chant légèrement faux géré ;
2. rubato géré ;
3. 3+ propositions ;
4. diversité mesurable ;
5. bass + arpège ;
6. recalcul local ;
7. locks utilisateur ;
8. Windows stable.

======================================================================
53. CRITERES DE PASSAGE V1
======================================================================

V1 acceptée si :

1. chant faux raisonnable géré ;
2. L0/L1/L2 stables ;
3. 4-5 propositions si qualité suffisante ;
4. plusieurs genres ;
5. counter-melody ;
6. validation loop ;
7. export MIDI complet ;
8. preview ;
9. logs ;
10. docs ;
11. performances acceptables sur PC cible.

======================================================================
54. ETAT DES TACHES
======================================================================

Chaque tâche :
- TODO
- IN_PROGRESS
- BLOCKED
- PARTIAL
- DONE
- REJECTED

DONE exige :
- code ;
- tests ;
- documentation ;
- résultat.

======================================================================
55. JOURNAL DE DEVELOPPEMENT
======================================================================

SUIVI_DEVELOPPEMENT doit être append-only.

Chaque entrée :
- date ;
- tâche ;
- changement ;
- tests ;
- résultats ;
- problèmes ;
- commit.

======================================================================
56. DECISIONS ARCHITECTURE
======================================================================

Chaque grande décision doit être enregistrée.

Exemple :
"RMVPE choisi pour BALANCED après benchmark V03."

Inclure :
- alternatives ;
- données ;
- raison ;
- date ;
- possibilité de révision.

======================================================================
57. REGISTRE DES DEPENDANCES
======================================================================

Pour chaque dépendance :
- version ;
- licence ;
- URL source ;
- rôle ;
- checksum ;
- usage commercial ;
- redistribution.

======================================================================
58. REGISTRE DES MODELES
======================================================================

Pour chaque modèle :
- nom ;
- version ;
- taille ;
- format ;
- licence ;
- benchmark ;
- checksum.

======================================================================
59. PERFORMANCE LOG
======================================================================

Après chaque optimisation importante :
- test case ;
- machine ;
- runtime ;
- RAM ;
- CPU ;
- version.

======================================================================
60. STOP CONDITIONS
======================================================================

Arrêter une approche si :

- aucune amélioration mesurable ;
- complexité excessive ;
- CPU incompatible ;
- licence incompatible ;
- résultats moins bons ;
- maintenance trop lourde.

Ne pas conserver une technologie uniquement parce qu'elle paraît sophistiquée.

======================================================================
61. PRIORITE ABSOLUE
======================================================================

Les tests doivent toujours privilégier :

1. conservation de l'intention ;
2. compatibilité musicale ;
3. qualité des harmonies ;
4. diversité ;
5. vitesse.

Une solution rapide mais musicalement mauvaise est un échec.

======================================================================
62. CONCLUSION
======================================================================

Le développement doit être guidé par les résultats, pas par les promesses des algorithmes.

La stratégie finale est :

CONSTRUIRE
↓
MESURER
↓
ECOUTER
↓
COMPARER
↓
CORRIGER
↓
VALIDER

Chaque moteur doit prouver sa valeur avant de devenir une dépendance permanente du projet.

FIN DU DOCUMENT

======================================================================
63. INITIALISATION DU DEPOT
======================================================================

T-000 - Initialiser les 13 documents permanents
Statut : TERMINÉ
Priorité : CRITIQUE
Dépendances : aucune
Critères de validation :
- les 13 fichiers imposés par le PDF existent à la racine ;
- aucun fichier n'est vide ;
- les cinq documents de conception fournis sont conservés intégralement ;
- le suivi, l'état actuel et le contrôle documentaire sont renseignés ;
- l'état Git et l'encodage sont vérifiés.

T-001 - Créer le socle C++20 / JUCE / CMake
Statut : TERMINÉ
Priorité : CRITIQUE
Dépendances : validation de ADR-002, compilateur C++20, CMake et version/licence JUCE
Critères de validation :
- [VALIDÉ] build minimal Debug et Release sur Windows x64 : configurations (233,9 s / 196,1 s) et builds réussis (MSVC 19.44, /W4 /WX) ;
- [VALIDÉ] premier test automatisé exécuté : CTest `common.strong_types` à 100 % en Debug et Release ;
- [VALIDÉ] formatage et avertissements stricts configurés et contrôlés (clang-format dans la CI) ;
- [VALIDÉ] CI verte : workflow Windows Debug/Release exécuté sur windows-2022 (run `ci` conclusion success) ;
- [VALIDÉ] architecture, dépendances, sécurité, risques, suivi et état actuel mis à jour.

Travail réalisé :
- CMake 3.24+ et presets Windows x64 (schéma v5) ;
- application JUCE minimale compilée et testée en Debug/Release ;
- bibliothèque commune avec types forts `Seconds`, `Beats`, `Probability` et `Score01` ;
- tests unitaires étendus (bornes, non finis, égalité/ordre, Score01, Beats) ;
- CI GitHub Actions Debug/Release validée (windows-2022, timeout, contrôle clang-format) ;
- règles de formatage, avertissements et sécurité de téléchargement ;
- .gitattributes pour la normalisation des fins de ligne.

Travail restant :
- obtenir la décision du propriétaire sur AGPLv3 ou licence commerciale JUCE avant distribution (R-008).

T-101 - Implémenter l'Audio Frontend
Statut : PARTIEL
Priorité : CRITIQUE
Dépendances : T-001 terminée, JUCE 8.0.15, minimp3 épinglé pour MP3 ; décision de codec M4A encore ouverte
Critères de validation :
- [VALIDÉ] import WAV, mono et stéréo, sample rates différents ;
- [VALIDÉ] détection durée, silence, clipping, niveau et bruit approximatif ;
- [VALIDÉ] métadonnées JSON et preuve que l'original n'est pas modifié pendant les tests ;
- [VALIDÉ] fichiers WAV vide, une trame, 30 secondes, multicanal et tronqué sans crash ;
- [VALIDÉ] builds et CTest locaux Debug/Release, 6/6 tests après intégration MP3 ;
- [VALIDÉ SUR `58e96e7`] CI Pull Request #2 verte ;
- [VALIDÉ] CI des corrections T-101.4/T-101.5 : run `31854004303`, Debug et Release réussis ;
- [VALIDÉ LOCALEMENT ET EN CI] décodeur MP3 sur un vecteur Layer III réel et non silencieux fourni par la dépendance épinglée ; corpus musical/utilisateur encore requis ;
- [NON IMPLÉMENTÉ/GARANTI] décodage M4A Windows ;
- [VALIDÉ COMME BASELINE] rééchantillonnage mono linéaire à 16 kHz, borné et testé ; qualité pitch/anti-repliement non validée ;
- [NON EXÉCUTÉ] lecture interactive sur périphérique audio réel.

Travail restant :
- compléter la preuve MP3 par un corpus musical/utilisateur et la CI ; choisir et auditer séparément la stratégie M4A (Media Foundation adapté, FFmpeg ou autre) ;
- remplacer ou valider la baseline de rééchantillonnage par benchmark avec anti-repliement et implémenter l'analyse par blocs pour les fichiers longs ;
- exposer des erreurs d'import détaillées à l'interface ;
- exécuter le corpus réel et la validation manuelle de lecture ;
- compléter la validation interactive et le corpus avant de déclarer la phase 1 terminée.
