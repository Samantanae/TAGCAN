# API Reference (Etat actuel)

## Vue d'ensemble

TAGCAN gere un payload CAN de 8 octets (`TxData`) et permet:

- de declarer des tags (`set_tag`) avec une taille en bits,
- d'ecrire une valeur dans `TxData` via un tag (`set_value`),
- de relire une valeur via un tag (`get_value`).

## Modules

- `include/config_value.h` + `scripts/config_value.c`: constantes et codes de retour.
- `include/gestion_tag.h` + `scripts/sub_scripts/tag_manager.c`: creation/gestion des tags.
- `include/sub_include/data_container.h` + `scripts/sub_scripts/data_container.c`: stockage et acces aux valeurs.
- `include/sub_include/tag_manager.h`: structure `TagDef` et variables globales du gestionnaire.
- `include/sub_include/print_val.h` + `scripts/sub_scripts/print_val.c`: affichage/debug.

## Structures et etat global

### `TagDef` (`include/sub_include/tag_manager.h`)

- `name[MAX_TAG_NAME_LEN]`: nom logique du tag.
- `n_bits`: taille du tag en bits.
- `byte_idx_a`, `byte_idx_b`: index des octets utilises dans `TxData`.
- `bit_pos_a`, `bit_pos_b`: position des bits dans les octets.

### Variables globales

- `TagDef g_tags[MAX_TAGS]`: table des tags.
- `uint8_t g_tag_count`: nombre de tags actifs.
- `uint8_t g_bytes_used[N_BYTES]`: occupation des octets.
- `uint8_t TxData[N_BYTES]`: buffer CAN (8 octets).

## Fonctions principales

### Gestion des tags

- `void init_tag_manager(void)`
  - Reinitialise `g_tags`, `g_tag_count` et `g_bytes_used`.

- `int set_tag(const char* tag_name, uint8_t n_bit)`
  - Declare un tag et lui alloue une zone dans `TxData`.
  - Implementation observee: accepte les tailles `1..16` (pas seulement `1, 3, 4, 8, 16`).
  - Retourne un code `CAN_TG_STATUE`.

- `const TagDef* get_tag_def(const char* tag_name)`
  - Recherche un tag par nom.
  - Retourne `NULL` si absent.

### Valeurs (payload)

- `void init_data_container(void)`
  - Met `TxData` a zero.

- `int set_value(const char* tag_name, uint32_t value)`
  - Ecrit la valeur dans `TxData` selon la definition du tag.
  - Debordement detecte via `value >= (1ULL << tag->n_bits)`.

- `int get_value(const char* tag_name, uint32_t* out_value)`
  - Lit la valeur d'un tag depuis `TxData`.

- `uint8_t prep_mask(uint8_t p_bit, uint8_t n_bit)`
  - Fonction utilitaire de masque binaire.

- `void can_simulate_send_receive(uint8_t* tx_buffer, uint8_t* rx_buffer)`
  - Copie `tx_buffer` vers `rx_buffer` (simulation simple).

- `void set_val_TxData(uint8_t *TxDataArray)`
  - Copie un tableau externe vers `TxData`.

## Debug / affichage

- `print_bin_8`, `print_all_data_bin`, `print_all_with_title`.
- `print_all_tag_set`, `print_all_data`, `print_repartition_bit`.

## Codes de retour

### Enum `CAN_TG_STATUE` (`include/config_value.h`)

- `CAN_TG_SUCCESS = 1`
- `CAN_TG_ERROR_NOT_ENOUNG_SPACE_ = -2`
- `CAN_TG_ERROR_TAG_NOT_FOUND_ = -3`
- `CAN_TG_ERROR_TAG_NAME_TO_BIGS_ = -4`
- `CAN_TG_ERROR_TOO_MAY_TAGS_ = -5`
- `CAN_TG_ERROR_SIZE_VALUE_INVALIDE_ = -6`
- `CAN_TG_ERROR_NAME_ALREADY_USE_ = -7`
- `CAN_TG_ERROR_SUBDIVISION_FAIL_ = -8`
- `CAN_TG_ERROR_TAG_NAME_INVALIDE_ = -9`
- `CAN_TG_ERROR_VALUE_TO_BIG = -10`
- `CAN_TG_ERROR_TRY_TO_FIND_END_CARACTER_ = -21`
- `CAN_TG_ERROR_INTERNAL_CORUPTION_ = -30`

### Constantes globales (`scripts/config_value.c`)

- Les constantes `CAN_TG_ERROR_*` existent aussi en variables globales.
- Attention: `CAN_TG_ERROR_NULL_POINTOR` vaut `-20` (constante), alors que `CAN_TG_ERROR_NULL_POINTOR_` vaut `-2` (enum).

## Notes importantes de comportement

- Validation longueur nom de tag: implementation actuelle utilise `sizeof(tag_name)` (taille pointeur), pas `strlen(tag_name)`.
- Gestion debordement `set_value`: retourne `CAN_TG_ERROR_VALUE_TO_BIG` (`-10`).
- Les tests de `scripts/main.c` attendent parfois `-4` pour des debordements de valeur, ce qui diverge du code courant.
