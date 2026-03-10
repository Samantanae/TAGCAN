#ifndef PRINT_VAL_H
#include "../../include/sub_include/print_val.h"
#endif // PRINT_VAL_H
#ifndef TAG_MANAGER_H
#include "../../include/sub_include/tag_manager.h"
#endif // TAG_MANAGER_H
#ifndef DATA_CONTAINER_H
#include "../../include/sub_include/data_container.h"
#endif // DATA_CONTAINER_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>

tsfp gbc2[9];

/** \brief Affiche la liste complete des tags et leurs metadonnees.
 */
void print_all_tag_set(void){
    printf("-------------------------------------------------\n");
    printf("legende\n\tnb:\tn_bits\t (le nombre de bites attribuer ـ la valeurs)\n");
    printf("\tbia:\tbyte_idx_a\t (le numﻯro du bytes de la partie A de la valeur \n");
    printf("\tbib:\tbyte_idx_b\t (le numﻯro de bytes de la partie B de la valeur (-1: pas de partie B))\n");
    printf("\tbpa:\tbit_pos_a (La position du premier bits dans le byte en question de la partie A.)\n");
    printf("\tbpb:\tbit_pos_b (idem, mais pour la partie B.\n");
    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    printf("\tid\tname\tnb\tbia\tbib\tbpa\tbpb\n");
    for(int i = 0; i <= g_tag_count - 1; i++){
        printf("\t%d\t",i);
        printf("%s\t",g_tags[i].name);
        printf("%d\t",g_tags[i].n_bits);
        printf("%d\t",g_tags[i].byte_idx_a);
        printf("%d\t",g_tags[i].byte_idx_b);
        printf("%d\t",g_tags[i].bit_pos_a);
        printf("%d\t",g_tags[i].bit_pos_b);
        printf("\n");
    }
    printf("---------------------------------------------------------------------\n");
}

/** \brief Affiche toutes les valeurs logiques des tags.
 * \details Chaque valeur est relue via get_value puis imprimee.
 */
void print_all_data(void){
    printf("---------------------[view of all data]----------------------------\n");
    printf("format: tag[n.bits]:\tvaleur\n");
    int32_t result_val;
    for(int i = 0; i <= g_tag_count - 1; i++){
        printf("\t%d",i);
        printf("%s\t",g_tags[i].name);
        get_value(g_tags[i].name,&result_val);
        printf("[%d]:\t",g_tags[i].n_bits);
        printf("%d", result_val);
        printf("\n");
    }
    printf("-------------------[END view of all data]---------------------------\n");
}

/** \brief Affiche la repartition des tags dans les bytes de la trame CAN.
 *
 */


/** \brief [INTERNE] Point d'extension pour la construction de vues de repartition.
 * \warning Fonction actuellement inachevee/conservee pour evolution future.
 */
void modif_vt(int8_t bia, int8_t bib, uint8_t bpa, uint8_t bpb, uint8_t nb,
               char* v1,char* v2,char* v3,char* v4,char* v5,char* v6,char* v7,char* v8 ){
               if(nb>8){

               }

               }



/** \brief Affiche une vue textuelle de l'occupation des bits de la trame.
 */
void print_repartition_bit(void){
    // Grille de remplissage: bit_grid[byte_idx][bit_pos] = caractere du tag
    char bit_grid[N_BYTES][8];
    memset(bit_grid, '.', sizeof(bit_grid));

    // Affichage de la legende
    printf("=== REPARTITION DES BITS DANS LA TRAME CAN ===\n\n");
    printf("Legende des tags:\n");
    char possible_id[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Remplir la grille avec les tags
    for(int i = 0; i <= g_tag_count - 1; i++){
        printf("  %s : [%c]\n", g_tags[i].name, possible_id[i]);

        if(g_tags[i].n_bits > 8){
            // Le tag s'etend sur deux bytes
            // Partie A: de byte_idx_a, bits bit_pos_a a 7
            for(int j = g_tags[i].bit_pos_a; j < 8; j++){
                bit_grid[g_tags[i].byte_idx_a][j] = possible_id[i];
            }
            // Partie B: de byte_idx_b, bits 0 a (remaining_bits - 1)
            int remaining_bits = g_tags[i].n_bits - (8 - g_tags[i].bit_pos_a);
            for(int j = 0; j < remaining_bits && j < 8; j++){
                bit_grid[g_tags[i].byte_idx_b][j] = possible_id[i];
            }
        }
        else {
            // Le tag tient dans un seul byte: de bit_pos_a a (bit_pos_a + n_bits - 1)
            for(int j = 0; j < g_tags[i].n_bits; j++){
                if(g_tags[i].bit_pos_a + j < 8){
                    bit_grid[g_tags[i].byte_idx_a][g_tags[i].bit_pos_a + j] = possible_id[i];
                }
            }
        }
    }

    // Affichage de la grille
    printf("\n");
    printf("========================================\n");
    printf("Byte | Bit 7 6 5 4 3 2 1 0\n");
    printf("-----|---------+--------\n");

    for(int byte = 0; byte < N_BYTES; byte++){
        printf(" [%d] | ", byte);
        for(int bit = 7; bit >= 0; bit--){
            printf("%c ", bit_grid[byte][bit]);
        }
        printf("\n");
    }
    printf("========================================\n");
}

/**
 * @brief Prints the binary representation of an 8-bit unsigned integer.
 *
 * @param val The `val` [from 0 to 255]. The function prints the binary
 * representation of this value by iterating over each bit.
 */
void print_bin_8(uint8_t val)
{
    for(int i = 7; i >= 0 ; i--)
    {
        printf("%c", (val & (1 << i)) ? '1' : '0');
    }
}

/**
 * @brief Prints the binary representation of each byte in `TxData`.
 *
 * Prints the binary representation of each byte in the `TxData`
 * array.
 */
void print_all_data_bin(void)
{
    printf("DATA: \t");
    for (int i = 0; i < N_BYTES; i++)
    {
        print_bin_8(TxData[i]);
        printf("\t");
    }
    printf("\n");
}

/** \brief Affiche TxData en binaire avec un titre.
 * \param title Libelle affiche avant les octets.
 */
void print_all_with_title(const char* title){
    printf("%s\t",title);
    int i;
    for (int i = 0; i < N_BYTES; i++)
    {
        print_bin_8(TxData[i]);
        printf("\t");
    }
    printf("\n");
}



