# Protocole de test de reprise

## Portée actuelle

Aucune opération de reprise n'est implémentée. Tous les scénarios ci-dessous sont définis mais NON EXÉCUTÉS. Résultat inconnu.

Contrôle du 2026-08-14 pour T-001 : VÉRIFIÉ - NON CONCERNÉ. Le socle créé ne réalise encore aucune analyse, sauvegarde, conversion ou export interruptible. Les scénarios PR-001 à PR-003 restent inchangés et NON EXÉCUTÉS.

## PR-001 - Interruption d'une analyse audio

- Objectif : reprendre ou recommencer proprement une analyse interrompue sans résultat partiel présenté comme valide.
- Conditions initiales : projet valide, audio de test long, cache vide.
- Préparation : activer les traces et identifier le répertoire temporaire.
- Étapes : démarrer l'analyse ; interrompre le processus au milieu ; relancer l'application ; rouvrir le projet ; demander la reprise.
- Résultat attendu : aucun artefact corrompu ; reprise au dernier point sûr ou redémarrage explicite ; état utilisateur cohérent.
- Résultat obtenu : NON EXÉCUTÉ.
- Preuves : aucune.
- Version testée : aucune.
- Date : non exécuté.
- Statut : NON EXÉCUTÉ.

## PR-002 - Interruption d'un export MIDI

- Objectif : empêcher qu'un fichier incomplet remplace un export valide.
- Conditions initiales : projet prêt à exporter et fichier de destination existant sauvegardé.
- Préparation : utiliser une destination de test.
- Étapes : démarrer l'export ; interrompre l'écriture ; relancer ; inspecter destination et temporaires ; réessayer.
- Résultat attendu : ancien fichier intact ou absence de fichier final ; temporaire identifiable et nettoyable ; nouvel export valide.
- Résultat obtenu : NON EXÉCUTÉ.
- Preuves : aucune.
- Version testée : aucune.
- Date : non exécuté.
- Statut : NON EXÉCUTÉ.

## PR-003 - Disque plein pendant une sauvegarde

- Objectif : préserver la dernière version valide du projet.
- Conditions initiales : projet sauvegardé et environnement de test avec quota contrôlé.
- Préparation : simuler une écriture insuffisante sans affecter les données réelles.
- Étapes : modifier le projet ; déclencher la sauvegarde ; provoquer l'échec ; redémarrer ; rouvrir la dernière sauvegarde.
- Résultat attendu : erreur claire, aucune corruption, dernière sauvegarde récupérable.
- Résultat obtenu : NON EXÉCUTÉ.
- Preuves : aucune.
- Version testée : aucune.
- Date : non exécuté.
- Statut : NON EXÉCUTÉ.
