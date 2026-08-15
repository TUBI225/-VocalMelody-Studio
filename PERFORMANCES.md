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
