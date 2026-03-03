#include "../../include/sub_include/data_container.h"
#include "../../include/config_value.h"
#include <stdio.h>

uint8_t TxData[N_BYTES];

void init_data_container(void){
    for(int i = 0; i < N_BYTES; i++){TxData[i] = 0;}
}




uint8_t prep_mask(uint8_t p_bit, uint8_t n_bit){
// prepa du mask
    uint8_t mask_val = 0b1;
    uint8_t mask_temp = 0b1;
    // mask des bits de la valeurs
    for (int i = 0; i < (n_bit - 1); i++){
        mask_val = mask_val << 1;
    mask_val |= mask_temp;
    }
    // mask pour conserver uniquement la valeurs dans le bytes
    return mask_val << (8 - p_bit - n_bit);
}

int set_value(const char* tag_name, uint32_t value) {
    const TagDef* tag = get_tag_def(tag_name);
    if (!tag) return -3; // Tag non trouvé

    // Vérification de débordement de valeur
    if (tag->n_bits < 32 && value >= (1ULL << tag->n_bits)) {
        printf("[ERREUR] Valeur %u trop grande pour le tag %s (%u bits)\n", value, tag_name, tag->n_bits);
        return -4;
    }

    if (tag->n_bits == 16) {
        // Séparation de la data en 2 bytes (Logique Python set_value16)
        uint8_t data_b = (value & 0xFF00) >> 8;
        uint8_t data_a = (value & 0x00FF);

        TxData[tag->byte_idx_a] = data_a;
        TxData[tag->byte_idx_b] = data_b;
    }
    else if(tag->n_bits == 8){
            uint8_t data_a = (value & 0x00FF);
            TxData[tag->byte_idx_a] = data_a;
    }

     else {
        // Valeur <= 8 bits
        // Création du masque binaire aligné à droite puis décalé (simule make_binary_mask python)
        uint8_t n_bits = tag->n_bits;
        uint8_t bit_pos_a = tag->bit_pos_a;
        int8_t byte_indx_a = tag->byte_idx_a;

        uint8_t mask = prep_mask(bit_pos_a, n_bits);

        uint8_t vrai_val = value;
        // déplacement de la vrai valeur
        vrai_val = vrai_val << (8 - bit_pos_a - n_bits);
        TxData[byte_indx_a] &= ~mask;
        TxData[byte_indx_a] |= vrai_val;
    }
    return 1;
}

int get_value(const char* tag_name, uint32_t* out_value) {
    const TagDef* tag = get_tag_def(tag_name);
    if (!tag) return CAN_TG_ERROR_TAG_NOT_FOUND; // Tag non trouvé

    if (tag->n_bits == 16) {
        uint8_t ba = TxData[tag->byte_idx_a];
        uint8_t bb = TxData[tag->byte_idx_b];
        *out_value = ba | (bb << 8);
    } else {

        // la sortie temporaire des valeurs ici sont juste pour voire les val. lors du débugages.
        uint8_t n_bits = tag->n_bits;
        uint8_t bit_pos_a = tag->bit_pos_a;
        int8_t byte_indx_a = tag->byte_idx_a;
        // création du mask
        uint8_t mask = prep_mask(bit_pos_a, n_bits);


        //uint8_t mask = ((1 << n_bits) - 1) << shift;

        uint8_t data_temp = TxData[byte_indx_a] & mask;
        *out_value = data_temp >> (8 - bit_pos_a - n_bits);;
    }
    return 1;
}


void can_simulate_send_receive(uint8_t* tx_buffer, uint8_t* rx_buffer) {
    for (int i = 0; i < N_BYTES; i++) {
        rx_buffer[i] = tx_buffer[i];
    }
}
