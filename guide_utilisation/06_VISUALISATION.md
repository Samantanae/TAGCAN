# 06 - Fonctions de visualisation

## Objectif

Ce chapitre presente les fonctions utilitaires de visualisation disponibles pour debugger rapidement le contenu des tags et du buffer CAN.

Ces fonctions sont principalement definies dans [include/sub_include/print_val.h](../include/sub_include/print_val.h) et implementees dans [scripts/sub_scripts/print_val.c](../scripts/sub_scripts/print_val.c).

## Fonctions disponibles

### print_bin_8

Prototype:

```c
void print_bin_8(uint8_t val);
```

Usage:
- Affiche un octet en binaire sur 8 bits.
- Utile pour verifier rapidement une valeur brute.
- Afficher les bits utiliser et quel est rataché à quoi.
### print_all_data_bin

Prototype:

```c
void print_all_data_bin(void);
```

Usage:
- Affiche tous les octets de TxData en binaire.
- A utiliser pour une vue brute de la trame complete.

### print_all_with_title

Prototype:

```c
void print_all_with_title(const char* title);
```

Usage:
- Meme principe que print_all_data_bin(), avec un titre en prefixe.
- Pratique pour comparer plusieurs etats successifs.

### print_all_tag_set

Prototype:

```c
void print_all_tag_set(void);
```

Usage:
- Affiche la table des tags declares et leurs metadonnees de mapping.
- Permet de verifier la repartition logique des tags (indices de bytes, positions de bits, etc.).

### print_all_data

Prototype:

```c
void print_all_data(void);
```

Usage:
- Relit les tags via get_value() et affiche leurs valeurs logiques.
- Tres utile pour verifier le comportement applicatif, au-dela du binaire brut.

### print_repartition_bit

Prototype:

```c
void print_repartition_bit(void);
```

Usage:
- Affiche une vue textuelle bit-a-bit de la trame CAN.
- Recommande pour controler le layout exact des tags et eviter les erreurs de superposition.

## Exemple rapide d'utilisation

```c
print_all_tag_set();
print_repartition_bit();
print_all_data();
print_all_data_bin();
```

## Fonctions en developpement

Certaines fonctions de visualisation sont encore en cours de developpement.

- `modif_vt(...)` (dans `scripts/sub_scripts/print_val.c`) est une fonction interne marquee comme point d'evolution future.
- Son comportement n'est pas finalise et ne doit pas etre utilise comme API stable.

## Recommandations

- Prioriser `print_repartition_bit()` pour valider le layout des tags.
- Utiliser `print_all_data()` pour verifier les valeurs metier.
- Considerer les fonctions marquees "en developpement" comme experimentales.

Voir aussi:
- [02_GESTION_TAGS.md](02_GESTION_TAGS.md)
- [03_DONNEES_CAN.md](03_DONNEES_CAN.md)
- [05_EXEMPLES_COMPLETS.md](05_EXEMPLES_COMPLETS.md)
