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

- Analyse statique : MSVC 19.44 exécute `/W4 /WX` en Debug/Release ; `clang-tidy` 19.1.5 est présent dans Visual Studio mais n'est pas encore intégré au build ou à la CI.
- Audit de dépendances : JUCE 8.0.15 et `actions/checkout` enregistrés et épinglés ; choix de licence JUCE non tranché pour la distribution.
- Tests de fichiers hostiles : partiels - fichier non audio, WAV vide et WAV tronqué rejetés sans crash ; fuzzing et corpus malveillant réel non exécutés.
- Gestion de secrets : aucun secret applicatif identifié.

## Sécurité du build - phase 0

- L'archive JUCE est liée à un commit immuable et vérifiée par SHA-256 avant extraction.
- La CI possède uniquement la permission `contents: read`.
- `actions/checkout` est épinglée par SHA de commit et non par simple branche.
- Les modules JUCE `curl` et navigateur embarqué sont désactivés dans le socle afin de ne pas ajouter de communication réseau à l'application.
- Le téléchargement de JUCE a lieu uniquement lors de la configuration du build ; l'application créée ne télécharge rien.
- Aucun secret, jeton, télémétrie ou permission système supplémentaire n'est introduit.

Limite : le téléchargement de dépendance rend le premier build dépendant du réseau. Un miroir ou paquet vérifié devra être prévu avant d'exiger des builds totalement hors ligne.

## Sécurité de l'import audio - T-101.4

- Rejet avant décodage des fichiers absents, vides, supérieurs à 1 Gio ou annonçant plus de 30 millions de trames.
- Vérification du retour de lecture du codec et des bornes canaux/sample rate/durée avant allocation.
- SHA-256 calculé avant et après décodage ; un fichier modifié pendant l'opération est rejeté.
- Toutes les exceptions à la frontière d'import sont converties en échec contrôlé.
- Le fichier audio original est ouvert en lecture et n'est jamais réécrit par l'importeur.

Limites restantes : l'import est synchrone sur le thread UI, le signal complet est chargé en mémoire sous le plafond, l'erreur affichée reste générique et aucun fuzzing des codecs n'a été exécuté. Le JSON de métadonnées est écrit directement et non encore par remplacement atomique ; il ne doit pas être utilisé comme sauvegarde projet critique.
