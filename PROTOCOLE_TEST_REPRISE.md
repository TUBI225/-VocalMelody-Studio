# Protocole de test de reprise

## Portée actuelle

L'import/analyse audio est désormais exécuté sur un worker annulable. L'annulation est testée et ne publie aucun résultat partiel ; une nouvelle demande recommence depuis le début. La reprise à un point intermédiaire n'est pas implémentée. L'écriture de métadonnées reste synchrone et sans remplacement atomique.

Contrôle initial du 2026-08-14 pour T-101.4 : CONCERNÉ. Mise à jour du 2026-08-17 : le chemin d'annulation coopérative de PR-001 est automatisé, mais l'arrêt brutal et la reprise après redémarrage ne le sont pas. PR-003 reste non exécuté. `saveAudioMetadata` écrit directement la destination et n'offre pas encore de remplacement atomique : ces métadonnées ne doivent pas être traitées comme sauvegarde projet fiable.

## PR-001 - Interruption d'une analyse audio

- Objectif : reprendre ou recommencer proprement une analyse interrompue sans résultat partiel présenté comme valide.
- Conditions initiales : projet valide, audio de test long, cache vide.
- Préparation : activer les traces et identifier le répertoire temporaire.
- Étapes : démarrer l'analyse ; interrompre le processus au milieu ; relancer l'application ; rouvrir le projet ; demander la reprise.
- Résultat attendu : aucun artefact corrompu ; reprise au dernier point sûr ou redémarrage explicite ; état utilisateur cohérent.
- Résultat obtenu : annulation coopérative observée ; retour `ImportError::Cancelled`, aucun `AudioImportResult` accessible, puis nouvel import possible.
- Preuves : test CTest `audio.file_import` (`AudioImportWorker` hors thread appelant et annulation pendant le rééchantillonnage).
- Version testée : branche `fix/audit-hardening-async-import` avant publication.
- Date : 2026-08-17.
- Statut : PARTIELLEMENT EXÉCUTÉ — arrêt applicatif brutal et reprise après redémarrage restent à tester.

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
