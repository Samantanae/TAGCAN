#include "../include/tag_manager.h"
#include "../include/config_value.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// initialisation des variable global
TagDef g_tags[MAX_TAGS];
uint8_t g_tag_count = 0;
uint8_t g_bytes_used[N_BYTES] = {0};

void init_tag_manager(void){
    g_tag_count = 0;
    //all bytes to 0 (no bytes used for now)
    // avoid garbage value
    memset(g_bytes_used, 0, sizeof(g_bytes_used));

    for (int i = 0; i < MAX_TAGS; i++){
        g_tags[i].name[0] = '\0';   // make sur the name is not set by garbage value
        g_tags[i].n_bits = 0;
        g_tags[i].byte_idx_a = -1;
        g_tags[i].byte_idx_b = -1;

    }
}

int get_first_tags_vide(void){
    for (int i = 0; i < MAX_TAGS; i++){
        // juste au cas où il n'as jamais été initialisé.
        if(g_tags[i].name == NULL){return i;}
        if(g_tags[i].name[0] == '\0'){return i;}
    }
    // ne devrais jamais être appeller ici, mais juste au cas où...
    return CAN_TG_ERROR_TOO_MAY_TAGS;
}

void set_name_tag(int n_tag, const char* tag_name){
    // clean the name
    for (int i = 0; i < MAX_TAG_NAME_LEN - 2; i++){g_tags[n_tag].name[i]=' ';}
    strncpy(g_tags[n_tag].name, tag_name, MAX_TAG_NAME_LEN - 1);
    // copie the value
    g_tags[n_tag].name[MAX_TAG_NAME_LEN - 1] = '\0';
    // voire le nom (juste pour débugé)
    printf("le nom du tag est: [%s]\n", g_tags[n_tag].name);
}

// set the ellement how is set to any tags (redusing the redundance of code)
void general_set_tag(int n_tag,const char* tag_name, uint8_t n_bits, int8_t byte_idx_a){
    set_name_tag(n_tag, tag_name);
    g_tags[n_tag].n_bits = n_bits;
    g_tags[n_tag].byte_idx_a = byte_idx_a;
    // signal l'augmentation du nombre de tags
    // comme de base, le nombre de tags est défini pour 32 tags de 1 bits,
    // nous pouvons utiliser g_tag_count comme un vrai compteur d'espace disponible
    // mais, pour le moment, ce n'ais pas le cas.
    g_tag_count++;

    // augmentation du g_bytes_used.
    // -- prise en compte de si c'est une valeurs de plus de 8 bits--
    if(n_bits >= 8){g_bytes_used[byte_idx_a] = 8;}
    else{g_bytes_used[byte_idx_a] += n_bits;}

}

// pas suposé d'être utilisé avant les vérification.
void set_new_tag_16(const char* tag_name, int8_t byte_idx_a, int8_t byte_idxb){
    // détermination d'une espace de tag vierge
    int n_tag = get_first_tags_vide();
    uint8_t n_bits = 16;
    general_set_tag(n_tag, tag_name, n_bits, byte_idx_a);
    g_tags[n_tag].byte_idx_b = byte_idxb;

}

// permet la définition des tags
int set_tag(const char* tag_name, uint8_t size){
    // évite d'avoir plus de 32 tags
    if(g_tag_count >= MAX_TAGS){return CAN_TG_ERROR_TOO_MAY_TAGS;}
    // évite des noms trops long
    if(sizeof(tag_name) >= MAX_TAG_NAME_LEN){return CAN_TG_ERROR_TAG_NAME_TO_BIGS;}
    // évite de redéfinir un tags déjà définie
    if(get_tag_def(tag_name) != NULL){return CAN_TG_ERROR_NAME_ALREADY_USE;}
    // vérifie si le nom est invalide
    if(tag_name[0]=='\0'){return CAN_TG_ERROR_TAG_NAME_INVALIDE;}
    //


    if(size == 16){
        // détermine s'il y a des bits avec asser d'espace (pour les 16 bites)
        // et en même temps, déterminer quel bytes possiblement prendre
        int bytea, byteb;    // l'id des futures bytes pris (s'il en reste asser)
        bytea=-1;
        int n_byte_nesessaire = 2;
        // itération sur chaque quantité d'espace mémoire de chaque bytes disponible
        for (int i = 0; i < N_BYTES; i++){
            if(g_bytes_used[i]==0){
                    n_byte_nesessaire--;
                    if(bytea == -1){bytea=i;}
                    else{byteb=i;}
            }
            // pas besoins de chercher plus s'il y a déjà les 2 de trouver.
            if(n_byte_nesessaire == 0){break;}
        };
        // vérification de s'il y a asser d'espace (après analyse)
        if(n_byte_nesessaire > 0){return CAN_TG_ERROR_NOT_ENOUNG_SPACE;}


    }
}
const TagDef* get_tag_def(const char* tag_name){

}

