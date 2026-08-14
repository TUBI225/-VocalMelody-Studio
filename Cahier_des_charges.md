CAHIER DES CHARGES
PROJET : GÉNÉRATEUR DE MÉLODIE / ACCORDS / ARPÈGES À PARTIR D’UNE VOIX A CAPPELLA
Version de conception : 1.0
Plateforme cible : Windows

======================================================================
1. VISION DU PROJET
======================================================================

Créer un logiciel Windows de composition musicale assistée capable de recevoir une voix
a cappella, même imparfaitement chantée, puis de proposer plusieurs directions musicales
complètes et réellement différentes autour de cette voix.

Le logiciel ne doit pas fonctionner comme un simple Auto-Tune ni comme un convertisseur
audio-vers-MIDI classique.

Son objectif principal est de comprendre suffisamment l’idée musicale contenue dans la
performance vocale pour construire une musique cohérente autour de cette idée, tout en
conservant autant que possible :
- la manière de chanter ;
- le phrasé ;
- les respirations ;
- les silences ;
- les montées et descentes ;
- les notes longues ;
- les notes courtes ;
- les retards et anticipations ;
- les glissandos et ornements intentionnels ;
- le groove naturel ;
- la structure des phrases.

Le logiciel devra produire plusieurs propositions comprenant notamment :
- accords ;
- progressions harmoniques ;
- basses ;
- arpèges ;
- mélodies instrumentales complémentaires ;
- contre-mélodies ;
- variantes adaptées à différents genres ;
- sorties MIDI ;
- prévisualisations audio.

======================================================================
2. PROBLÈME CENTRAL À RÉSOUDRE
======================================================================

Une voix a cappella peut être :
- juste ;
- légèrement fausse ;
- très fausse ;
- instable ;
- enregistrée sans métronome ;
- irrégulière rythmiquement ;
- bruitée ;
- enregistrée au téléphone ;
- riche en vibrato ;
- riche en glissandos ;
- mélodiquement ambiguë.

Le problème principal est donc :

Ne pas confondre "ce qui a été physiquement chanté" avec
"ce que la personne voulait probablement chanter".

Le moteur doit maintenir plusieurs hypothèses plutôt que de prendre trop tôt
une décision unique.

======================================================================
3. PRINCIPES FONDAMENTAUX
======================================================================

3.1 Séparation performance / notes probables / intention

Le système doit conserver trois couches distinctes :

L0 — PERFORMANCE RÉELLE
La vérité acoustique :
- courbe F0 ;
- timing réel ;
- énergie ;
- onsets ;
- respirations ;
- vibrato ;
- glissandos ;
- bruit ;
- instabilité ;
- articulation.

L1 — NOTE LATTICE / HYPOTHÈSES DE NOTES
Pour chaque segment vocal :
- plusieurs notes candidates ;
- probabilité de chaque candidate ;
- stabilité ;
- confiance acoustique ;
- rôle probable ;
- alternatives ;
- importance structurelle.

L2 — INTENTION MÉLODIQUE
Représentation plus abstraite :
- contour montant/descendant ;
- répétitions ;
- sauts ;
- motifs ;
- rythme relatif ;
- points d’appui ;
- tension/résolution ;
- respiration ;
- début/fin de phrase ;
- relation entre phrases.

Aucune couche ne doit détruire la précédente.

3.2 Incertitude native

Le logiciel ne doit pas prétendre savoir lorsqu’il ne sait pas.

Exemple :
G4 : 0,55
G#4 : 0,28
F#4 : 0,17

Cette incertitude doit se propager jusqu’au moteur harmonique.

3.3 Analyse multi-niveaux

Une décision harmonique doit être évaluée à plusieurs niveaux :

NOTE
+
MESURE
+
PHRASE
+
SECTION
+
CHANSON ENTIÈRE

Un accord qui fonctionne pour une note isolée peut être mauvais dans la phrase ou
dans la chanson entière.

======================================================================
4. FONCTIONNALITÉS PRINCIPALES
======================================================================

4.1 Import audio

Formats visés :
- WAV ;
- MP3 ;
- M4A ;
- FLAC ;
- autres formats courants via bibliothèque compatible.

4.2 Analyse de qualité

Le logiciel doit évaluer :
- niveau sonore ;
- clipping ;
- bruit ;
- présence effective de voix ;
- qualité de détection possible ;
- sections inutilisables ;
- éventuelles voix multiples.

4.3 Prétraitement

Fonctions possibles :
- normalisation ;
- passe-haut ;
- réduction de bruit légère ;
- suppression de parasites ;
- conversion mono si nécessaire ;
- rééchantillonnage pour l’analyse.

Le prétraitement ne doit pas altérer inutilement la performance originale.

4.4 Détection de hauteur

Le moteur de pitch doit être modulaire.

Candidats à tester :
- RMVPE ;
- pYIN ;
- CREPE / CREPE Tiny ;
- YIN ;
- fusion de plusieurs estimateurs.

Le logiciel final ne doit pas dépendre d’un choix figé avant tests comparatifs.

4.5 Détection d’événements vocaux

Détecter :
- onsets ;
- offsets ;
- syllabes ;
- silences ;
- respirations ;
- vibrato ;
- glissandos ;
- notes parlées ;
- rap mélodique ;
- changements de stabilité.

4.6 Segmentation en notes

Transformer la performance continue en segments musicaux probables sans quantification
agressive.

Chaque segment doit contenir plusieurs hypothèses.

4.7 Détection de phrase

Identifier :
- frontières de phrases ;
- respirations ;
- répétitions ;
- motifs ;
- climax ;
- conclusions ;
- appels/réponses.

4.8 Temps vocal et temps musical

Le logiciel doit maintenir deux espaces temporels :

t_perf = temps réel de l’audio en secondes.
t_music = temps musical en mesures, temps et subdivisions.

Créer une fonction de correspondance :
TimeWarpMap / RubatoMap W(t)

Elle doit permettre de :
- conserver le timing original ;
- reconstruire une grille musicale ;
- gérer le rubato ;
- conserver les groove offsets ;
- générer un accompagnement stable sans écraser le phrasé.

4.9 Tonalité et mode

Le système ne doit pas choisir immédiatement une seule tonalité.

Il conserve plusieurs candidats, par exemple :
A minor : 0,72
C major : 0,67
D dorian : 0,38

Les modules suivants peuvent réévaluer ces probabilités.

4.10 Détection de modulation

À terme :
- analyser la tonalité par phrase et section ;
- détecter les changements persistants ;
- proposer des accords pivots ;
- conserver les modulations ambiguës si besoin.

======================================================================
5. MOTEUR HARMONIQUE
======================================================================

5.1 Objectif

Produire plusieurs progressions réellement différentes et musicalement crédibles.

5.2 Analyse multi-niveaux obligatoire

Chaque accord candidat doit être évalué selon :
- notes locales ;
- mesure ;
- phrase ;
- section ;
- chanson entière.

5.3 Importance structurelle des notes

Les notes ne doivent pas toutes avoir le même poids.

Importance possible selon :
- durée ;
- stabilité ;
- confiance F0 ;
- position métrique ;
- début/fin de phrase ;
- accent ;
- répétition ;
- rôle structurel.

5.4 Rôle des notes

Le moteur doit différencier :
- note d’accord ;
- note de passage ;
- approche ;
- suspension ;
- appoggiature ;
- ornement ;
- chromatisme intentionnel ;
- note incertaine.

5.5 Génération de candidats

Pour chaque zone harmonique :
- accords diatoniques ;
- accords de 7e ;
- extensions ;
- sus2/sus4 ;
- add9 ;
- accords empruntés ;
- dominantes secondaires ;
- substitutions ;
- renversements ;
- basses alternatives ;
- accords de passage.

Les possibilités autorisées dépendent du genre et du niveau de complexité.

5.6 Recherche globale

Les progressions ne doivent pas être générées accord par accord de façon gloutonne.

Méthodes envisagées :
- Viterbi ;
- N-best Viterbi ;
- Beam Search ;
- programmation dynamique ;
- contraintes symboliques ;
- moteur CSP/Z3 pour certaines contraintes dures.

5.7 Voice leading

Évaluer :
- mouvement de basse ;
- mouvements des voix internes ;
- notes communes ;
- renversements ;
- continuité ;
- collisions ;
- comportement spécifique au genre.

======================================================================
6. GÉNÉRATION DE 4 À 5 DIRECTIONS
======================================================================

Le logiciel doit produire plusieurs propositions réellement distinctes.

Exemples de familles :
1. Fidèle / simple ;
2. Émotionnelle ;
3. Moderne / riche ;
4. Minimaliste / aérée ;
5. Genre spécifique ou réharmonisation.

Le moteur ne doit pas forcer cinq propositions absurdes lorsque la mélodie ne permet
qu’un petit nombre de solutions fortes.

Il doit rechercher un équilibre :
QUALITÉ + DIVERSITÉ.

======================================================================
7. MOTEUR DE DIVERSITÉ
======================================================================

Mesurer la distance entre propositions avec notamment :
- progression d’accords ;
- rythme harmonique ;
- fonctions harmoniques ;
- tension ;
- ligne de basse ;
- renversements ;
- densité ;
- structure ;
- genre ;
- arpèges ;
- contre-mélodies.

Méthodes possibles :
- clustering K-Medoids ;
- sélection gloutonne qualité + distance ;
- DPP ;
- seuils de distance ;
- familles harmoniques forcées.

Le choix final devra être testé expérimentalement.

======================================================================
8. MOTEUR D’ARRANGEMENT
======================================================================

8.1 Basse

Générer :
- fondamentales ;
- inversions ;
- passages ;
- lignes syncopées ;
- walking simplifié ;
- ostinatos ;
- basses spécifiques au genre.

8.2 Arpèges

Les arpèges doivent dépendre de :
- accord ;
- renversement ;
- genre ;
- tempo ;
- densité vocale ;
- registre ;
- groove ;
- silences vocaux ;
- voice leading.

Ils ne doivent pas se résumer à 1-3-5-3 en boucle.

8.3 Mélodies complémentaires

Utiliser :
- call and response ;
- remplissage des silences ;
- prolongement de phrase ;
- imitation ;
- inversion de contour ;
- variation rythmique ;
- simplification ;
- contraste.

8.4 Masque d’occupation vocale

L’arrangement doit connaître les moments où :
- la voix est dense ;
- la voix est sparse ;
- la voix se tait ;
- la voix tient une note.

Il adapte automatiquement la densité instrumentale.

======================================================================
9. PROFILS DE GENRES
======================================================================

Genres initiaux possibles :
- Afro ;
- Afrobeats ;
- Coupé-décalé ;
- R&B ;
- Soul ;
- Gospel ;
- Pop ;
- Trap ;
- Ballade ;
- Variété.

Un genre doit influencer :
- groove ;
- tempo ;
- métrique ;
- rythme harmonique ;
- vocabulaire d’accords ;
- extensions ;
- voicings ;
- basse ;
- arpèges ;
- syncopes ;
- contre-mélodies ;
- densité ;
- espace laissé à la voix.

Les genres seront idéalement définis par profils JSON/YAML extensibles.

======================================================================
10. SCORING GLOBAL
======================================================================

Chaque proposition reçoit des scores séparés :

- compatibilité vocale ;
- cohérence harmonique ;
- conservation de l’intention ;
- voice leading ;
- cohérence rythmique ;
- structure de phrase ;
- cohérence de section ;
- cohérence chanson entière ;
- respect du genre ;
- respiration ;
- densité ;
- diversité ;
- musicalité.

Pénalités :
- collision voix/instrument ;
- surcharge ;
- correction excessive ;
- rupture harmonique ;
- monotonie ;
- contradiction avec une contrainte utilisateur.

Le système devra tester plusieurs méthodes :
- somme pondérée normalisée ;
- moyenne géométrique ;
- seuils de survie ;
- approche Pareto ;
- combinaison hiérarchique.

======================================================================
11. BOUCLE DE VALIDATION
======================================================================

Le pipeline ne doit pas fonctionner uniquement en sens unique.

Architecture :

ANALYSER
↓
PROPOSER
↓
TESTER
↓
CORRIGER
↓
RETESTER
↓
VALIDER

Après génération d’un accompagnement, un Compatibility Checker vérifie :
- collisions ;
- dissonances indésirables ;
- mauvaise densité ;
- voice leading ;
- mauvais registre ;
- mauvais groove ;
- conflit avec la phrase ;
- incohérence de section ;
- incohérence globale.

Une proposition trop faible est rejetée et remplacée.

======================================================================
12. INTERACTION UTILISATEUR
======================================================================

L’utilisateur doit pouvoir :
- choisir une proposition ;
- changer de genre ;
- choisir une émotion ;
- modifier une tonalité ;
- verrouiller une tonalité ;
- verrouiller une note ;
- verrouiller un accord ;
- signaler une zone bien chantée ;
- signaler une zone mal chantée ;
- choisir la densité ;
- choisir le niveau harmonique ;
- demander d’autres variantes.

Le système doit permettre un recalcul local.

======================================================================
13. RE-CALCUL INCRÉMENTAL
======================================================================

Exemple :
l’utilisateur change l’accord de la mesure 4.

Le moteur doit recalculer seulement :
- transition 3 → 4 ;
- transition 4 → 5 ;
- voicing autour de la mesure 4 ;
- basse concernée ;
- arpèges concernés ;
- contre-mélodies concernées.

Le reste reste en cache lorsque possible.

======================================================================
14. EXPLICABILITÉ
======================================================================

Chaque décision importante doit avoir une trace.

Exemple :
Accord sélectionné : Am7
Compatibilité vocale : 0,88
Voice leading : 0,92
Genre : R&B
Note vocale principale : E
Rôle de E : quinte de Am7
Alternative : Fmaj7
Motif du rejet : moins bonne transition de basse

Deux niveaux :
- logs techniques pour développement ;
- explication simple pour utilisateur.

======================================================================
15. SORTIES
======================================================================

Sorties visées :
- progression d’accords ;
- MIDI accords ;
- MIDI basse ;
- MIDI arpèges ;
- MIDI contre-mélodies ;
- MIDI mélodies ;
- tempo map ;
- time signature ;
- tonalité(s) ;
- structure ;
- fichier projet ;
- prévisualisation audio ;
- stems à terme.

======================================================================
16. PERFORMANCE ET MATÉRIEL
======================================================================

Cible :
Windows x64.

Le logiciel doit fonctionner sans GPU dédié obligatoire.

Architecture recommandée :
- cœur symbolique léger ;
- analyses lourdes hors temps réel ;
- cache des résultats ;
- modèles ONNX optionnels ;
- multithreading lorsque pertinent.

Le cœur musical doit rester utilisable après la première analyse sans recalculer le pitch.

======================================================================
17. TECHNOLOGIES À ÉVALUER
======================================================================

Application / Audio :
- C++20 ;
- JUCE ;
- éventuellement backend Python de prototypage.

Inférence IA :
- ONNX Runtime.

DSP / MIR :
- Essentia ;
- aubio ;
- librosa pour prototypage ;
- scipy/numpy côté prototype.

Pitch :
- RMVPE ;
- pYIN ;
- CREPE ;
- YIN.

Symbolique :
- music21 pour prototypage ;
- moteur théorie musicale maison pour version finale.

MIDI :
- JUCE MIDI ;
- pretty_midi / mido pour prototype.

Synthèse preview :
- SoundFont ;
- SF2/SFZ ;
- synthé léger intégré.

======================================================================
18. INSPIRATIONS ET SYSTÈMES À ÉTUDIER
======================================================================

- Microsoft MySong / Songsmith ;
- MIDI-SAG ;
- AccoMontage2 ;
- AutoHarmonizer ;
- AI Harmonizer ;
- BandLab Audio-to-MIDI / Layer ;
- Dubler 2 ;
- Band-in-a-Box ;
- outils de génération audio comme Suno/Udio uniquement comme comparaison produit.

Le logiciel ne doit pas copier ces systèmes.
Il doit étudier leurs forces et leurs limites.

======================================================================
19. CAS DIFFICILES
======================================================================

Prévoir explicitement :
- chant très faux ;
- chant légèrement faux ;
- erreurs d’octave ;
- vibrato ;
- glissando ;
- rap mélodique ;
- voix parlée ;
- absence de tempo ;
- rubato ;
- accélération ;
- ralentissement ;
- changement de tonalité ;
- modulation ;
- note hors gamme intentionnelle ;
- bruit ;
- téléphone ;
- respiration forte ;
- doublage ;
- harmonie vocale ;
- longue note ;
- silence long ;
- voix très grave ;
- voix très aiguë.

======================================================================
20. PRIORITÉS DE DÉVELOPPEMENT
======================================================================

PRIORITÉ 1
Comprendre la voix et construire L0/L1/L2.

PRIORITÉ 2
Construire une TimeWarpMap fiable.

PRIORITÉ 3
Construire le moteur tonal multi-hypothèses.

PRIORITÉ 4
Construire un Harmony Engine multi-niveaux.

PRIORITÉ 5
Produire plusieurs harmonisations de qualité.

PRIORITÉ 6
Construire basse, arpèges et contre-mélodies.

PRIORITÉ 7
Ajouter genres, UX, verrouillage, recalcul local et rendu.

======================================================================
21. CRITÈRES DE RÉUSSITE
======================================================================

Le projet sera considéré réussi lorsque :

1. Une voix correcte produit des accords cohérents.
2. Une voix légèrement fausse ne provoque pas une mauvaise tonalité.
3. Une voix très imparfaite produit plusieurs hypothèses plutôt qu’une fausse certitude.
4. Le phrasé et le rythme original restent reconnaissables.
5. Les 4-5 propositions sont musicalement différentes.
6. Les propositions restent éditables.
7. Le logiciel fonctionne sans GPU obligatoire.
8. Le moteur peut expliquer ses décisions.
9. Le changement d’un accord ne force pas l’analyse complète.
10. Les résultats MIDI respectent tempo, groove et structure.

FIN DU CAHIER DES CHARGES