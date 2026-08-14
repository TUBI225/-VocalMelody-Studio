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

Aucune mesure exécutée. Une amélioration ne pourra être déclarée qu'avec une comparaison avant/après reproductible.

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
