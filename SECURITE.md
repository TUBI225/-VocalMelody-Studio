# Sécurité

## Principes

- Priorité à l'intégrité des données et à l'arrêt propre face à une entrée douteuse.
- Traitement local et hors ligne privilégié ; aucune communication réseau applicative n'est actuellement prévue.
- Moindre privilège : ne demander aucune permission non nécessaire.
- Aucun secret ne doit être stocké dans le dépôt, les journaux ou les fichiers projet.

## Menaces principales

- Fichier audio, MIDI, JSON ou projet malformé, démesuré ou malveillant.
- Traversée de chemin ou écrasement de fichier lors d'un import/export.
- Corruption de projet ou cache lors d'une interruption.
- Dépendance compromise, vulnérable ou non redistribuable.
- Modèle ML téléchargé altéré.
- Exposition de données vocales privées dans les journaux ou diagnostics.

## Mesures requises

- Valider extension, signature, taille, durée, structure et limites avant décodage.
- Canonicaliser les chemins et limiter les écritures aux emplacements choisis par l'utilisateur ou au stockage applicatif prévu.
- Utiliser des fichiers temporaires uniques et des remplacements atomiques quand possible.
- Vérifier origine, version, licence et empreinte des dépendances et modèles.
- Éviter de journaliser l'audio ou des chemins complets lorsqu'ils ne sont pas nécessaires.
- Prévoir des limites CPU, mémoire, disque et temps de traitement.
- Documenter toute future communication réseau, authentification, permission ou collecte de télémétrie avant implémentation.

## État des vérifications

- Analyse statique : avertissements stricts configurés ; aucun compilateur local pour les exécuter.
- Audit de dépendances : JUCE 8.0.15 et `actions/checkout` enregistrés et épinglés ; choix de licence JUCE non tranché pour la distribution.
- Tests de fichiers hostiles : non exécutés.
- Gestion de secrets : aucun secret applicatif identifié.

## Sécurité du build - phase 0

- L'archive JUCE est liée à un commit immuable et vérifiée par SHA-256 avant extraction.
- La CI possède uniquement la permission `contents: read`.
- `actions/checkout` est épinglée par SHA de commit et non par simple branche.
- Les modules JUCE `curl` et navigateur embarqué sont désactivés dans le socle afin de ne pas ajouter de communication réseau à l'application.
- Le téléchargement de JUCE a lieu uniquement lors de la configuration du build ; l'application créée ne télécharge rien.
- Aucun secret, jeton, télémétrie ou permission système supplémentaire n'est introduit.

Limite : le téléchargement de dépendance rend le premier build dépendant du réseau. Un miroir ou paquet vérifié devra être prévu avant d'exiger des builds totalement hors ligne.
