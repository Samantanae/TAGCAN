# Amalgamation TAGCAN

Cette version regroupe le code C du projet dans deux fichiers :

- `TAGCAN_all.h` : concaténation de tous les fichiers `.h`
- `TAGCAN_all.c` : concaténation de tous les fichiers `.c`

## Règles utilisées

- Inclus : tous les `.h` et `.c` du dépôt
- Exclus : `obj/`, `doxygen/`, et le dossier de sortie `amalgamation/`
- Les sections sont délimitées par des marqueurs `BEGIN FILE` / `END FILE` pour conserver la traçabilité.

## Remarque

Cette fusion est une concaténation brute pour faciliter la lecture/portage. Selon votre chaîne de build, des ajustements d'`#include` peuvent être nécessaires pour une compilation directe du fichier `TAGCAN_all.c`.
