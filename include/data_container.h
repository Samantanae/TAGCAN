#ifndef DATA_CONTAINER_H
#define DATA_CONTAINER_H

#include <stdint.h>
#include "tag_manager.h"

// Représente le buffer de données (le payload CAN de 8 octets)
extern uint8_t g_data[N_BYTES];

void init_data_container(void);

// Fonctions principales (getters / setters)
int set_value(const char* tag_name, uint32_t value); // Accepte jusqu'à 16 bits sans problème
int get_value(const char* tag_name, uint32_t* out_value);

// Utilitaires d'affichage pour le débogage
void print_bin_8(uint8_t val);
void print_all_data_bin(void);

// Simulation de transmission (remplace send/receve python)
void can_simulate_send_receive(uint8_t* tx_buffer, uint8_t* rx_buffer);

#endif // DATA_CONTAINER_H
