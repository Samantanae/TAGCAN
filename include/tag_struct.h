#ifndef TAG_STRUCT_H
#define TAG_STRUCT_H


#include <stdint.h>
#include <stdbool.h>

#define MAX_TAGS 32
#define MAX_TAG_NAME_LEN 16
#define N_BYTES 8

typedef struct {
    char name[MAX_TAG_NAME_LEN];
    uint8_t n_bit;   // 1, 3, 4, 8 ou 16
    int8_t byte_idx_a;    // Index du byte (0 à 7), -1 si non utilisé
    int8_t byte_idx_b;    // Deuxième index pour les variables 16 bits, sinon -1
    uint8_t bit_pos;      // Position du premier bit (de gauche à droite)
} TagDef;

// gestionnaire global des tags
extern TagDef g_tags[MAX_TAGS];
extern uint8_t g_tag_count;

// alocation des bits dans chaque byte
extern uint8_t g_bytes_used[N_BYTES];

// fonction qui gère les tags
void init_tag_manager(void);
int set_tag(const char* tag_name, uint8_t size);
const TagDef* get_tag_def(const char* tag_name);
#endif /* TAG_STRUCT_H */
