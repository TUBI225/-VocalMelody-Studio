# Performances

## Objectifs initiaux

- Fonctionnement CPU-first sans GPU dédié obligatoire.
- Plateforme cible : Windows x64, PC cible de référence avec 16 Go de RAM.
- L'interface doit rester réactive pendant les analyses longues.
- Les modes FAST, BALANCED et QUALITY devront avoir des budgets mesurés distincts.
- Le recalcul local doit éviter de relancer les modules non affectés.

## Mesures obligatoires

| Mesure | Valeur actuelle | Objectif / seuil |
|---|---:|---|
| Temps de démarrage | Non mesuré | À définir après prototype UI |
| Mémoire au repos | Non mesurée | À définir |
| CPU au repos | Non mesuré | À définir |
| Temps d'import audio | Non mesuré | À définir par durée d'audio |
| Temps d'analyse pitch | Non mesuré | À définir par mode matériel |
| Temps de pipeline complet | Non mesuré | À définir |
| Latence de recalcul local | Non mesurée | Inférieure au recalcul complet, preuve requise |
| Taille du cache | Non mesurée | Limite et politique d'éviction à définir |

## Environnements de référence

- PC LOW : 8 Go RAM - CPU et stockage à préciser.
- PC TARGET : 16 Go RAM - CPU et stockage à préciser.
- PC HIGH : 32 Go RAM - CPU/GPU et stockage à préciser.

## Journal des mesures

Des mesures de build et de tests existent ci-dessous. Les performances d'import, de mémoire et d'interface restent non mesurées ; aucune amélioration applicative ne peut être déclarée sans protocole reproductible.

## Environnement observé le 2026-08-14

- Système : Microsoft Windows 11 Professionnel 10.0.26200.
- Processeur : Intel Core i5-7300U à 2.60 GHz.
- Processeurs logiques : 4.
- Mémoire visible : 15,8 Go.
- Profil approximatif : PC TARGET pour la mémoire, CPU à conserver comme référence basse/intermédiaire.

## Mesures T-001

- Temps de configuration CMake Debug (premier configure, hors ligne) : 233,9 s ; reconfiguration après modification : 4,4 s.
- Temps de configuration CMake Release (hors ligne) : 196,1 s.
- Temps de compilation Debug : non mesuré précisément (build lancé en arrière-plan, exécutables produits sans erreur).
- Temps de compilation Release : non mesuré précisément (build lancé en arrière-plan, exécutables produits sans erreur).
- Temps des tests CTest : Debug 1,86 s ; Release 0,85 s (1 test `common.strong_types`).
- Temps de démarrage de l'application : NON MESURÉ, binaire non exécuté interactivement.

Ces mesures sont indicatives : la configuration initiale de JUCE inclut la construction de l'outil `juceaide`. Les mesures de compilation devront être refaites sur un build propre et chronométré pour être comparables.

## Mesures T-101.4

- Machine : environnement observé ci-dessus, build incrémental après ajout de `juce_cryptography`.
- CTest Debug final : 4,25 s, 5/5 tests réussis.
- CTest Release final : 2,26 s, 5/5 tests réussis.
- Build Release incrémental final : 9,3 s avec `--parallel 2`, tests et contrôle de formatage inclus dans la commande complète ; le premier rebuild de JUCE cryptography a dépassé 5 minutes et n'est pas une mesure propre exploitable.
- Corpus automatisé le plus long : WAV mono 30 s à 8 kHz ; aucune mesure isolée du temps d'import ni du pic mémoire.

Interprétation : ces chiffres prouvent l'exécution des tests, pas la performance du produit. L'import charge actuellement le signal décodé complet puis une copie mono ; le plafond de 30 millions de trames limite le risque mais ne remplace pas une analyse par blocs et un benchmark mémoire.

## Mesures T-101.5

- Rééchantillonnage d'analyse : fréquence cible 16 kHz, interpolation linéaire, plafond interne 100 millions de trames de sortie.
- CTest Debug final (`analysisVersion=2`) : 5/5, 3,74 s.
- CTest Release final : 5/5, 1,69 s.
- Temps et mémoire du rééchantillonnage isolé : NON MESURÉS.

Ces résultats valident le comportement fonctionnel testé, pas la qualité spectrale. Un benchmark anti-repliement, précision et coût CPU est requis avant d'utiliser cette baseline pour le pitch.

## Mesures T-102.1

- CTest Debug après correction des bornes d'autocorrélation : 8/8 en 4,02 s.
- CTest Release : 8/8 en 9,78 s.
- Le tableau de corrélation est limité à la fenêtre de 2048 échantillons, même si le sample rate fourni annonce un lag maximal supérieur.
- Temps CPU et mémoire isolés de l'estimateur : NON MESURÉS ; le benchmark phase 2 reste requis.

## Mesures T-102.2 - rééchantillonnage anti-repliement

- Prototype sinc direct : rejeté, les fonctions trigonométriques par tap dépassaient 60 s sur les tests d'import longs.
- Table polyphasée fixe 1024 phases : tests frontend/import/métadonnées Debug en 12,84 s.
- Table polyphasée réduite par PGCD des sample rates : mêmes tests en 6,65 s en Debug.
- Validation complète : CTest Debug 8/8 en 6,43 s ; Release 8/8 en 11,62 s dans l'environnement observé.
- CI initiale de la PR #5, run `31952148170` : Windows Debug réussi en 3 min 59 s ; Windows Release réussi en 5 min 52 s, configuration, build et 8 tests inclus.
- Réponse fréquentielle automatisée 48→16 kHz : RMS du sinus 1 kHz exigée entre 0,33 et 0,37 ; RMS du sinus 12 kHz exigée sous 0,01 et sous 5 % de la sortie linéaire.

Ces durées couvrent des suites de tests et non le seul filtre. Le coût CPU/RAM isolé, les chirps et le corpus vocal restent NON MESURÉS.

## Mesures T-101.8

- CTest Debug final après intégration MP3 : 6/6 tests réussis en 8,29 s.
- CTest Release final : 6/6 tests réussis en 5,83 s.
- Le décodeur MP3 produit directement le mono par blocs et ne conserve plus simultanément le PCM brut, des copies par canal et le mono.
- Le pic mémoire et le temps d'import MP3 isolé ne sont pas encore mesurés ; aucune amélioration chiffrée ne doit donc être annoncée.

## Mesures T-102.3 - rééchantillonnage anti-repliement renforcé (rayon 32, marge 0.78)

- Noyau sinc/Blackman porté de 33 à 67 taps (rayon 16 -> 32) et marge de coupure 0.90 -> 0.78 ; `analysisVersion` passe de 3 à 4.
- Réponse mesurée le 2026-08-16 (sinus amplitude 0.5, 1 s, sortie 16 kHz ; dB relatifs au niveau d'entrée) :

| Fréquence d'entrée | 44,1 -> 16 kHz (avant -> après) | 48 -> 16 kHz (avant -> après) | 96 -> 16 kHz (avant -> après) |
|---|---|---|---|
| 6 kHz | -1,7 -> -3,8 dB | -1,9 -> -4,0 dB | -3,5 -> -4,9 dB |
| 7 kHz | -5,0 -> -18,6 dB | -5,1 -> -17,2 dB | -5,5 -> -10,6 dB |
| 7,5 kHz | -7,8 -> -34,2 dB | -7,6 -> -30,3 dB | -6,8 -> -14,6 dB |
| 8,5 kHz (repliée à 7,5 kHz) | -16,1 -> -80,6 dB | -15,0 -> -78,2 dB | -9,8 -> -26,3 dB |
| 10 kHz (repliée à 6 kHz) | -40,4 -> -91,9 dB | -35,3 -> -79,1 dB | -15,9 -> -60,5 dB |
| 12 kHz (repliée à 4 kHz) | -96,3 -> -96,5 dB | -79,4 -> -96,3 dB | -28,3 -> -77,0 dB |

- Contrepartie : la coupure -6 dB passe de ~7,3 kHz à ~6,3 kHz ; le contenu 6-7 kHz subit -4 à -18 dB selon la fréquence. Acceptable pour l'analyse vocale (fondamentale et harmoniques basses), tracé comme limite.
- Impact temps mesuré : CTest Debug 8/8 en 6,84 s et `audio.file_import` en 3,86 s (contre 3,89 s avant) ; le coût du noyau 67 taps est négligeable devant le décodage et le hachage sur le corpus actuel. Le coût CPU isolé sur un fichier de taille maximale (30 M trames) reste NON MESURÉ.

## Worker d'import et contrat renforcé - 2026-08-17

- CTest Debug final : 10/10 en 11,15 s ; `audio.file_import` 5,48 s.
- CTest Release final : 10/10 en 7,02 s ; `audio.file_import` 3,95 s.
- Le décodage JUCE utilise un tampon multicanal de 8192 trames, puis conserve le signal mono complet. Le pic RAM sur un fichier maximal et la latence d'annulation sur corpus réel restent NON MESURÉS.
- CI PR #9, run `32045569757` : Debug réussi en 4 min 03 s ; Release réussi en 5 min 40 s, téléchargements vérifiés, build et 10 tests inclus.
- Bande critique 8,5-9,5 kHz (auparavant non testée) couverte en permanence par `SignalAnalysisTests` (48 -> 16 kHz et 96 -> 16 kHz).
