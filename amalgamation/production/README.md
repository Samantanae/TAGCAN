# Amalgamation TAGCAN - Variante Production (Stricte)

Cette variante regroupe le coeur de la librairie dans deux fichiers :

- `TAGCAN_core_all.h`
- `TAGCAN_core_all.c`

## Exclusions appliquées

- `tests/`
- `exemple_utilisation/`
- `scripts/main.c` (fichier de tests/demonstration)
- fichiers `.c` vides ou uniquement blancs (ex: `scripts/CAN_utils.c`)
- `obj/`
- `doxygen/`
- `amalgamation/` (pour éviter l'auto-inclusion)

## Notes

- Les blocs sont encadrés par des marqueurs `BEGIN FILE` / `END FILE`.
- Cette version est une concaténation brute ; selon votre environnement, certains `#include` relatifs peuvent nécessiter une adaptation pour une compilation directe.
