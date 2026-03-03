#include "../../include/sub_include/tag_manager.h"
#include "../../include/config_value.h"
#include "../../include/gestion_tag.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// initialisation des variable global
TagDef g_tags[MAX_TAGS];
uint8_t g_tag_count = 0;
uint8_t g_bytes_used[N_BYTES] = {0};

void init_tag_manager(void){
    g_tag_count = 0;
    //tout les bite sont à 0 (comme ont les initialise/réinitialise.)
    // évite les valeurs poubelle.
    memset(g_bytes_used, 0, sizeof(g_bytes_used));
    // initialisation de l'ensemble des tags avec des valeurs vide
    for (int i = 0; i < MAX_TAGS; i++){
        g_tags[i].name[0] = '\0';   // fait en sorte que le nom soit concidérer comme 'vide', même avec des donnée poubelle après ce caractère.
        g_tags[i].n_bits = 0;
        g_tags[i].byte_idx_a = -1;
        g_tags[i].byte_idx_b = -1;
        g_tags[i].bit_pos_a = 0;
        g_tags[i].bit_pos_b = 0;
    }
}

int get_first_tags_vide(void){
    for (int i = 0; i < MAX_TAGS; i++){
        // juste au cas où il n'as jamais été initialisé.
        if(g_tags[i].name == NULL){return i;}
        // sinon, le premier vide est déterminer par le premier dont le nom est vide '\0'
        if(g_tags[i].name[0] == '\0'){return i;}
    }
    // ne devrais jamais être appeller ici, mais juste au cas où...
    return CAN_TG_ERROR_TOO_MAY_TAGS;
}

/** \brief [FONCTION INTERNE] Fais les manipulation nécécaire à la définition d'un nom et le place dans le tag # n_tag.
 * \warning comme c'est une fonction interne, les donnée entrer sont suposé être sanétiser avant de passer dans cette dernière.
 * \param n_tag:    une valeur entre 0 et MAX_TAGS désignant l'id numérique du tags.
 * \param tag_name: le nom du tags qui serras utilisé pour référer à la valeur.
 *
 */

void set_name_tag(int n_tag, const char* tag_name){
    // netoie l'espace pour préparer l'inssertion du nom dans l'espace mémoire.
    for (int i = 0; i < MAX_TAG_NAME_LEN - 2; i++){g_tags[n_tag].name[i]=' ';}
    // copie du nom à l'espace mémoire.
    strncpy(g_tags[n_tag].name, tag_name, MAX_TAG_NAME_LEN - 1);
    // copie the value
    g_tags[n_tag].name[MAX_TAG_NAME_LEN - 1] = '\0';
    // voire le nom (juste pour débugé)
    //printf("le nom du tag est: [%s]\n", g_tags[n_tag].name);
}

// set the ellement how is set to any tags (redusing the redundance of code)
void general_set_tag(int n_tag,const char* tag_name,
                      uint8_t n_bits, int8_t byte_idx_a){
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
//  n_bits2:    la taille dans le deuxième bytes (en bits).     entre 0 et 8
void set_new_tag_9_to_16(const char* tag_name, int8_t byte_idx_a, int8_t byte_idx_b, uint8_t n_bits2){
    // détermination d'une espace de tag vierge
    int n_tag = get_first_tags_vide();
    uint8_t n_bits = 8;
    general_set_tag(n_tag, tag_name, n_bits, byte_idx_a);    // définir les éléments de base
    // au cas où 0 est passé, nous n'avons pas besoins d'utiliser un deuxième bytes.
    if(n_bits2 > 0){
        g_tags[n_tag].byte_idx_b = byte_idx_b;
        g_bytes_used[byte_idx_b] += n_bits2;
    }
}

/** \brief trouve le premier bytes qui contien suffissament d'espace libre pour contenir la taille demander.
 *
 * \param siz:  la taille de la valeurs.
 * \return
 *
 */
uint8_t find_byte_with_enoung_space(uint8_t siz){
    // verification de si c'est une taille impaire
    if(siz%2 > 0){
        // dans un telle cas, priorisation de ceux aussi impaire (pour former des paires)
        for (uint8_t i = 0; i < N_BYTES; i++){{
            if(((8 - g_bytes_used[i])%2 > 0) && ((8 - g_bytes_used[i]) >= siz)){
                return i;
            }
        }
    }
    // si aucun trouver ou si c'est déjà une valeur paire, faire une recherche normal.
    for (uint8_t i = 0; i < N_BYTES; i++){
        if((8 - g_bytes_used[i]) >= siz){
            return i;
        }
    }
}


// permet la définition des tags
int set_tag(const char* tag_name, uint8_t siz){
    // évite d'avoir plus de 32 tags
    if(g_tag_count >= MAX_TAGS){return CAN_TG_ERROR_TOO_MAY_TAGS;}
    // évite des noms trops long
    if(sizeof(tag_name) >= MAX_TAG_NAME_LEN){return CAN_TG_ERROR_TAG_NAME_TO_BIGS;}
    // évite de redéfinir un tags déjà définie
    if(get_tag_def(tag_name) != NULL){return CAN_TG_ERROR_NAME_ALREADY_USE;}
    // vérifie si le nom est invalide
    if(tag_name[0]=='\0'){return CAN_TG_ERROR_TAG_NAME_INVALIDE;}
    //


    //------------------------- v2 de l'algo pour trouver des tailles valides.

    if(siz>8){
        // vérification de si c'est une taille plus grand que 16 bits.
        if(siz>16){

        }
    }


    // -------------------------fin nouvelle algo
    if(siz == 16){
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
        // set the tags
        set_new_tag_9_to_16(tag_name, bytea, byteb,8);
        return SUCCES_TO_SET;
    }
    else if(siz == 8){    // cas de 8 bits
         for (int i = 0; i < N_BYTES; i++){
            if(g_bytes_used[i]==0){
                int n_tag = get_first_tags_vide();
                general_set_tag(n_tag, tag_name, 8, i);
                return SUCCES_TO_SET;
            }
         }
    }
    else if(siz == 4){
        // priorité au endroi où il y a déjà un autre 4 (pour formé un byte complet.
        for (int i = 0; i < N_BYTES; i++){
           int n_tag = get_first_tags_vide();
            general_set_tag(n_tag, tag_name, 4, i);
            return SUCCES_TO_SET;
        }
        // sinon, priorisé ceux avec rien.
        for (int i = 0; i < N_BYTES; i++){
            if(g_bytes_used[i] == 0){
                int n_tag = get_first_tags_vide();
                general_set_tag(n_tag, tag_name, 4, i);
                return SUCCES_TO_SET;
            }
        }
        // sinon, là, permettre d'ajouté la valeurs à un endrois incomplet.
        for (int i = 0; i < N_BYTES; i++){
            if(g_bytes_used[i] <= 4){
                int n_tag = get_first_tags_vide();
                general_set_tag(n_tag, tag_name, 4, i);
                return SUCCES_TO_SET;
            }
        }
    }
    else if(siz == 3 || siz == 1){
        // vérification de s'il y a des bytes impaire (affin de le compler)
        for (int i = 0; i < N_BYTES; i++){
            if(g_bytes_used[i]%2 == 0){}
            else{
                if((8 - g_bytes_used[i]) >= siz){
                    int n_tag = get_first_tags_vide();
                    general_set_tag(n_tag, tag_name, siz, i);
                    return SUCCES_TO_SET;
                }
            }
        }
        // sinon, essayer avec les autres
        for (int i = 0; i < N_BYTES; i++){
            if((8 - g_bytes_used[i]) >= siz){
                int n_tag = get_first_tags_vide();
                general_set_tag(n_tag, tag_name, siz, i);
                return SUCCES_TO_SET;
            }
         }
         // sinon, pas asser de place.
         return CAN_TG_ERROR_NOT_ENOUNG_SPACE;
    }
    // taille invalide
    return CAN_TG_ERROR_SIZE_VALUE_INVALIDE;
}
const TagDef* get_tag_def(const char* tag_name) {
    for (int i = 0; i < g_tag_count; i++) {
        if (strncmp(g_tags[i].name, tag_name, MAX_TAG_NAME_LEN) == 0) {
            return &g_tags[i];
        }
    }
    return NULL; // Tag non trouvé
}

