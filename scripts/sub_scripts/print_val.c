#include "../../include/sub_include/print_val.h"
#include "../../include/sub_include/tag_manager.h"
#include "../../include/sub_include/data_container.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

tsfp gbc2[9];
void print_all_tag_set(void){
    printf("-------------------------------------------------\n");
    printf("\tid\tname\t\tn_bits\tbyte_idx_a\tbyte_idx_b\tbit_pos_a\tbit_pos_b\n");
    for(int i = 0; i <= g_tag_count ; i++){
        printf("\t%d",i);
        printf("%s\t",g_tags[i].name);
        printf("%d\t",g_tags[i].n_bits);
        printf("%d\t",g_tags[i].byte_idx_a);
        printf("%d\t",g_tags[i].byte_idx_b);
        printf("%d\t",g_tags[i].bit_pos_a);
        printf("%d\t",g_tags[i].bit_pos_b);
        printf("\n");
    }
    printf("-------------------------------------------------\n");
}

void print_all_data(void){
    printf("---------------------[view of all data]----------------------------\n");
    printf("format: tag[nb bits pour la valeur]:\tactuel valeur");
    int32_t result_val;
    for(int i = 0; i <= g_tag_count ; i++){
        printf("\t%d",i);
        printf("%s\t",g_tags[i].name);
        get_value(g_tags[i].name,&result_val);
        printf("[%d]:\t",g_tags[i].n_bits);
        printf("%d", result_val);
        printf("\n");
    }
    printf("-------------------[END view of all data]---------------------------\n");
}

/** \brief print in the terminal the place where the module place the value in the bytes of the CAN frame.
 *
 */


void modif_vt(int8_t bia, int8_t bib, uint8_t bpa, uint8_t bpb, uint8_t nb,
               char* v1,char* v2,char* v3,char* v4,char* v5,char* v6,char* v7,char* v8 ){
               if(nb>8){

               }

               }



void print_repartition_bit(void){
    int max_byte_toucher = 0;
    // reset the struc
    for(int i = 0; i < 8 ; i++){
        for(int i2 = 0; i2 < 8 ; i2++){gbc2[i].bi[i2]=' ';}
    gbc2[i].bi[8] = '\0';
    }


    printf("legend:\n");
    char possible_id[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    //char* vt1[8],vt2[8],vt3[8],vt4[8],vt5[8],vt6[8],vt7[8], vt8[8];
    uint8_t gbu[N_BYTES] = {0};
    memset(gbu, 0, sizeof(gbu));
    for(int i = 0; i <= g_tag_count ; i++){
        printf("%s :\t",g_tags[i].name);
        printf("[%c]\n", possible_id[i]);
        if(g_tags[i].n_bits > 8){
            // suposon 'v' le bon 'byte_idx_a'
            // 1) pour toute les valeurs présent dans le a, les 8 bites vont être attribuer à celui-ci.
            for(int i2=0; i2<8;i2++){
                gbc2[g_tags[i].bit_pos_a].bi[i2] = possible_id[i];
            }
            gbu[g_tags[i].bit_pos_a] += 8;
            int i2=gbu[g_tags[i].bit_pos_b];
            int condi_fin = gbu[g_tags[i].bit_pos_b]+(g_tags[i].n_bits - 8);
            for(i2; i2<condi_fin;i2++){
                gbc2[g_tags[i].bit_pos_b].bi[i2] = possible_id[i];
            }
            gbu[g_tags[i].bit_pos_b] += (g_tags[i].n_bits - 8);
            if(g_tags[i].bit_pos_b>max_byte_toucher){max_byte_toucher=g_tags[i].bit_pos_b;}
            if(g_tags[i].bit_pos_a>max_byte_toucher){max_byte_toucher=g_tags[i].bit_pos_a;}
        }
        else{
            gbu[g_tags[i].bit_pos_a] += 8;
            int i2=gbu[g_tags[i].bit_pos_a];
            int condi_fin = gbu[g_tags[i].bit_pos_a]+(g_tags[i].n_bits - 8);
            for(i2; i2<condi_fin;i2++){
                gbc2[g_tags[i].bit_pos_a].bi[i2] = possible_id[i];
            }
            gbu[g_tags[i].bit_pos_b] += (g_tags[i].n_bits - 8);
            if(g_tags[i].bit_pos_a>max_byte_toucher){max_byte_toucher=g_tags[i].bit_pos_a;}
        }
    }
    printf("---------------------------------------------\n\t");
    for(int i = 0; i < max_byte_toucher ; i++){
        printf("%s\t",gbc2[i]);
    }
    printf("\n----------------------------------------------\n");
}

/**
 * @bref prints the binary representation of an 8-bit unsigned integer.
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
 * prints the binary representation of each byte in the `TxData`
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

void print_all_with_title(const char* title){
    printf("%s\t",title);
    int i;
    for (int i = 0; i < N_BYTES; i++);
    {
        print_bin_8(TxData[i]);
        printf("\t");
    };
    printf("\n");
}



