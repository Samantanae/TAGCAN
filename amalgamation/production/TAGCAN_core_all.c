/* TAGCAN core amalgamated sources (strict) */
/* Generated on 2026-03-11 */


/* ===== BEGIN FILE: ./scripts/config_value.c ===== */

#ifndef CONFIG_VALUE_H
#include "../include/config_value.h"
#endif // CONFIG_VALUE_H
// Definitions des constantes globales historiques de codes de retour.
const int CAN_TG_ERROR_NOT_ENOUNG_SPACE = -2;
const int CAN_TG_ERROR_TAG_NOT_FOUND = -3;
const int CAN_TG_ERROR_TAG_NAME_TO_BIGS = -4;
const int CAN_TG_ERROR_TOO_MAY_TAGS = -5;
const int CAN_TG_ERROR_SIZE_VALUE_INVALIDE = -6;
const int CAN_TG_ERROR_NAME_ALREADY_USE = -7;
const int CAN_TG_ERROR_SUBDIVISION_FAIL = -8;
const int CAN_TG_ERROR_TAG_NAME_INVALIDE=-9;
const int CAN_TG_ERROR_NULL_POINTOR = -20;
const int CAN_TG_ERROR_TRY_TO_FIND_END_CARACTER = -21;
const int CAN_TG_ERROR_INTERNAL_CORUPTION = -30;
const int SUCCES_TO_SET = 1;
const int SUCCES = 1;




/* ===== END FILE: ./scripts/config_value.c ===== */

/* ===== BEGIN FILE: ./scripts/sub_scripts/data_container.c ===== */

#ifndef DATA_CONTAINER_H
#include "../../include/sub_include/data_container.h"
#endif // DATA_CONTAINER_H
#ifndef CONFIG_VALUE_H
#include "../../include/config_value.h"
#endif // CONFIG_VALUE_H
#ifndef PRINT_VAL_H
#include "../../include/sub_include/print_val.h"
#endif // PRINT_VAL_H
#include <stdio.h>

uint8_t TxData[N_BYTES];

void init_data_container(void)
{
    for(int i = 0; i < N_BYTES; i++)
    {
        TxData[i] = 0;
    }
}



void set_val_TxData(uint8_t *TxDataArray){
for (int i = 0; i < N_BYTES; i++){
    TxData[i] = TxDataArray[i];
}
}
uint8_t prep_mask(uint8_t p_bit, uint8_t n_bit)
{
// Preparation du masque
    uint8_t mask_val = 0b1;
    uint8_t mask_temp = 0b1;
    // Construit n_bit bits a 1
    for (int i = 0; i < (n_bit - 1); i++)
    {
        mask_val = mask_val << 1;
        mask_val |= mask_temp;
    }
    // Aligne le masque sur la position cible dans le byte
    return mask_val << (8 - p_bit - n_bit);
}

CAN_TG_STATUE set_value(const char* tag_name, uint32_t value)
{
    const TagDef* tag = get_tag_def(tag_name);
    if (!tag) return -3; // Tag non trouve

    const uint8_t n_bits = tag->n_bits;
    const uint8_t bit_pos_a = tag->bit_pos_a;
    const uint8_t bit_pos_b = tag->bit_pos_b;
    const int8_t byte_idx_a = tag->byte_idx_a;
    const int8_t byte_idx_b = tag->byte_idx_b;

    // Verification du depassement de la valeur selon n_bits
    if (tag->n_bits < 32 && value >= (1ULL << tag->n_bits))
    {
        printf("[ERREUR] Valeur ٪u trop grande pour le tag ٪s (٪u bits)\n", value, tag_name, tag->n_bits);
        return CAN_TG_ERROR_VALUE_TO_BIG;
    }

    // Cas des valeurs sur plus de 8 bits: repartition entre partie A et partie B.
    if(n_bits > 8){
        uint8_t data_b = (value & 0xFF00) >> 8;
        uint8_t data_a = (value & 0x00FF);
        TxData[byte_idx_a] = data_a;
        // Cas 16 bits: la partie B occupe tout le byte.
        if(n_bits == 16){TxData[tag->byte_idx_b] = data_b;}
        else{
            // Cas 9..15 bits: insertion partielle dans le byte B.
            // Creation du masque binaire
            uint8_t mask = prep_mask(bit_pos_b, n_bits - 8);
            // Alignement de la deuxieme partie
            data_b = data_b << (8 - bit_pos_b - n_bits);
            // Mise a jour selective des bits de la partie B.
            TxData[byte_idx_b] &= ~mask;
            TxData[byte_idx_b] |= data_b;
            return CAN_TG_SUCCESS;
        }
    }
    else if(tag->n_bits == 8)
    {
        uint8_t data_a = (value & 0x00FF);
        TxData[tag->byte_idx_a] = data_a;
        return CAN_TG_SUCCESS;
    }
    else{
        // Valeur <= 8 bits (entre 1 et 7 bits inclusivement)
        // Creation du masque binaire aligne sur la zone cible
        uint8_t mask = prep_mask(bit_pos_a, n_bits);
        uint8_t vrai_val = value;
        // Alignement de la valeur dans le byte
        vrai_val = vrai_val << (8 - bit_pos_a - n_bits);
        TxData[byte_idx_a] &= ~mask;
        TxData[byte_idx_a] |= vrai_val;
    }
    return CAN_TG_SUCCESS;
}

uint16_t convert_8_to_16bit(uint8_t ba, uint8_t bb){
    uint16_t r;
    r = ba | (bb << 8);   // Recompose un entier 16 bits depuis 2 octets.
    return r;
}

int get_value(const char* tag_name, uint32_t* out_value)
{
    const TagDef* tag = get_tag_def(tag_name);
    if (!tag) return CAN_TG_ERROR_TAG_NOT_FOUND_; // Tag non trouve
    // Copie locale des metadonnees du tag pour simplifier les calculs.
    const uint8_t n_bits = tag->n_bits;
    const uint8_t bpa = tag->bit_pos_a;
    const uint8_t bpb = tag->bit_pos_b;
    const int8_t byta = tag->byte_idx_a;
    const int8_t bytb = tag->byte_idx_b;
    if(tag->byte_idx_b > -1){
            // Declaration conditionnelle conservee pour debug/historique.
            const int8_t byte_indx_b = tag->byte_idx_b;
    }
    if (tag->n_bits == 16)    // cas de 16 bits exactement
    {
        uint8_t ba = TxData[byta];
        uint8_t bb = TxData[bytb];
        *out_value = ba | (bb << 8);   // Recomposition 16 bits complete.
    }
    else if((n_bits < 16) && (n_bits > 8)){// cas d'entre 8 et 16 bits.
        uint8_t ba = TxData[byta];
        uint8_t bb;
        // Creation d'un masque pour extraire la partie B.
        uint8_t mask = prep_mask(bpb, n_bits - 8);
        uint8_t data_temp = TxData[bytb] & mask;
        bb = data_temp >> (8 - bpa - n_bits);
        *out_value = ba | (bb << 8);   // Recomposition avec partie B partielle.

    }
    else
    {
    /** \brief Copie un buffer externe dans TxData.
     * \param TxDataArray Tableau source de N_BYTES octets.
     */
        // Creation du masque d'extraction (<= 8 bits)
        uint8_t mask = prep_mask(bpa, n_bits);


        //uint8_t mask = ((1 << n_bits) - 1) << shift;

    /** \brief Recompose un entier 16 bits depuis deux octets.
     * \param ba Octet de poids faible.
     * \param bb Octet de poids fort.
     * \return Valeur 16 bits combinee.
     */

        uint8_t data_temp = TxData[byta] & mask;
        *out_value = data_temp >> (8 - bpa - n_bits);
    }
    return CAN_TG_SUCCESS;
}


/** \brief Simule une transmission CAN locale.
 * \details Copie simplement tx_buffer dans rx_buffer.
 * \param tx_buffer Buffer d'emission.
 * \param rx_buffer Buffer de reception.
 */
void can_simulate_send_receive(uint8_t* tx_buffer, uint8_t* rx_buffer)
{
    for (int i = 0; i < N_BYTES; i++)
    {
        rx_buffer[i] = tx_buffer[i];
    }
}


/* ===== END FILE: ./scripts/sub_scripts/data_container.c ===== */

/* ===== BEGIN FILE: ./scripts/sub_scripts/print_val.c ===== */

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





/* ===== END FILE: ./scripts/sub_scripts/print_val.c ===== */

/* ===== BEGIN FILE: ./scripts/sub_scripts/tag_manager.c ===== */


#include "../../include/sub_include/tag_manager.h"
#ifndef CONFIG_VALUE_H
#include "../../include/config_value.h"
#endif // CONFIG_VALUE_H
#ifndef GESTION_TAG_H
#include "../../include/gestion_tag.h"
#endif // GESTION_TAG_H
#ifndef PRINT_VAL_H
#include "../../include/sub_include/print_val.h"
#endif // PRINT_VAL_H
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Etat global du gestionnaire de tags
TagDef g_tags[MAX_TAGS];
uint8_t g_tag_count = 0;
uint8_t g_bytes_used[N_BYTES] = {0};

void init_tag_manager(void)
{
    g_tag_count = 0;
    // Remise a zero de l'occupation des bytes.
    memset(g_bytes_used, 0, sizeof(g_bytes_used));
    // Reinitialisation complete de tous les slots de tags.
    for (int i = 0; i < MAX_TAGS; i++)
    {
        g_tags[i].name[0] = '\0';   // Slot considere vide quand name[0] == '\0'.
        g_tags[i].n_bits = 0;
        g_tags[i].byte_idx_a = -1;
        g_tags[i].byte_idx_b = -1;
        g_tags[i].bit_pos_a = 0;
        g_tags[i].bit_pos_b = 0;
    }
}

/** \brief [INTERNE] Retourne l'index du premier slot tag libre.
 * \details Un slot est considere libre si son nom est NULL ou vide ('\\0').
 * \return Index du slot libre, ou CAN_TG_ERROR_TOO_MAY_TAGS si aucun slot disponible.
 */
int get_first_tags_vide(void)
{
    for (int i = 0; i < MAX_TAGS; i++)
    {
        // Cas defensif: un nom NULL est traite comme slot libre.
        if(g_tags[i].name == NULL)
        {
            return i;
        }
        // Premier slot libre: nom vide.
        if(g_tags[i].name[0] == '\0')
        {
            return i;
        }
    }
    // Cas de garde: aucun slot libre detecte.
    return CAN_TG_ERROR_TOO_MAY_TAGS;
}

/** \brief [INTERNE] Enregistre un nom de tag dans un slot.
 * \warning Les verifications d'entree sont censees etre faites en amont.
 * \param n_tag Index du slot (0..MAX_TAGS-1).
 * \param tag_name Nom logique du tag.
 *
 */

void set_name_tag(const int n_tag, const char* tag_name)
{
    // Nettoie le buffer avant copie du nom.
    for (int i = 0; i < MAX_TAG_NAME_LEN - 2; i++)
    {
        g_tags[n_tag].name[i]=' ';
    }
    // Copie du nom dans la structure.
    strncpy(g_tags[n_tag].name, tag_name, MAX_TAG_NAME_LEN - 1);
    // Termine explicitement la chaine.
    g_tags[n_tag].name[MAX_TAG_NAME_LEN - 1] = '\0';
    // Debug optionnel:
    //printf("le nom du tag est: [%s]\n", g_tags[n_tag].name);
}

// Initialise les champs communs d'un tag et met a jour l'occupation memoire.
/** \brief [INTERNE] Initialise les champs communs d'un tag.
 * \param n_tag Index du slot tag cible.
 * \param tag_name Nom logique a enregistrer.
 * \param n_bits Taille de la valeur en bits.
 * \param byte_idx_a Byte principal de stockage (partie A).
 */
void general_set_tag(const int n_tag,const char* tag_name,
                     uint8_t n_bits, int8_t byte_idx_a)
{
    set_name_tag(n_tag, tag_name);
    g_tags[n_tag].n_bits = n_bits;

    g_tags[n_tag].byte_idx_a = byte_idx_a;
    // Le tag est desormais actif.
    g_tag_count++;

    // Mise a jour de l'occupation des bits dans le byte principal.
    if(n_bits >= 8)
    {
        g_bytes_used[byte_idx_a] = 8;
    }
    else
    {
        g_bytes_used[byte_idx_a] += n_bits;
    }
}

// [INTERNE] Creation d'un tag sur 9..16 bits (2 bytes potentiels).
// n_bits_total: taille totale de la valeur (8..16).
/** \brief [INTERNE] Cree un tag avec repartition potentielle sur 2 bytes.
 * \param tag_name Nom logique du tag.
 * \param byte_idx_a Byte de la partie A.
 * \param byte_idx_b Byte de la partie B.
 * \param n_bits_total Taille totale en bits.
 */
void set_new_tag_9_to_16(const char* tag_name, const int8_t byte_idx_a, const int8_t byte_idx_b, uint8_t n_bits_total)
{
    // Recherche d'un slot libre.
    int n_tag = get_first_tags_vide();
    general_set_tag(n_tag, tag_name, n_bits_total, byte_idx_a);    // Initialisation commune.
    // Pour > 8 bits, une partie B est utilisee.
    if(n_bits_total > 8)
    {

        g_tags[n_tag].byte_idx_b = byte_idx_b;
        g_tags[n_tag].bit_pos_b = g_bytes_used[byte_idx_b];
        g_bytes_used[byte_idx_b] += n_bits_total - 8;
        g_tags[n_tag].bit_pos_a = 0;
        /* A ce stade, la repartition est validee: on peut finaliser
           l'occupation et les positions des bits pour A et B. */
    }
}

/** \brief Trouve le premier byte ayant assez de place libre.
 *
 * \param siz Taille demandee (en bits).
 * \param retur_var Sortie: index du byte selectionne.
 * \return CAN_TG_SUCCESS ou CAN_TG_ERROR_NOT_ENOUNG_SPACE_.
 *
 */
CAN_TG_STATUE find_byte_with_enoung_space(uint8_t siz, int8_t* retur_var)
{
    // Pour > 8 bits, la 1re recherche se limite a un bloc de 8 bits.
    if(siz > 8)
    {
        siz = 8;
    }
    // Pour les tailles impaires: priorite aux bytes a place impaire restante.
    if(siz%2 > 0)
    {
        // Heuristique de placement.
        for (int8_t i = 0; i < N_BYTES; i++)
        {
            {
                if(((8 - g_bytes_used[i])%2 > 0) && ((8 - g_bytes_used[i]) >= siz))
                {
                    *retur_var=i;
                    return CAN_TG_SUCCESS;
                }
            }
        }
    }
    // Sinon, recherche sequentielle standard.
    for (int8_t i = 0; i < N_BYTES; i++)
    {
        if((8 - g_bytes_used[i]) >= siz)
        {
            *retur_var=i;
            return CAN_TG_SUCCESS;
        }
    }
    return CAN_TG_ERROR_NOT_ENOUNG_SPACE_;
}


/** \brief Variante de recherche en excluant le byte deja utilise par la partie A.
 *
 * \param siz_total Taille totale (9..16).
 * \param posi_premier_bit Byte deja retenu pour la partie A.
 * \param retur_var Sortie: index du byte pour la partie B.
 * \return CAN_TG_SUCCESS ou CAN_TG_ERROR_NOT_ENOUNG_SPACE_.
 *
 */

CAN_TG_STATUE find_byte2_with_enoung_space(uint8_t siz_total, int8_t posi_premier_bit,int8_t* retur_var)
{
    uint8_t siz = siz_total - 8;
    // Meme heuristique que find_byte_with_enoung_space.
    if(siz%2 > 0)
    {
        // Priorite aux bytes dont l'espace restant est impair.
        for (int8_t i = 0; i < N_BYTES; i++)
        {
            if(posi_premier_bit == i)
            {
                continue;
            }
            if(((8 - g_bytes_used[i])%2 > 0) && ((8 - g_bytes_used[i]) >= siz))
            {
                *retur_var=i;
                return CAN_TG_SUCCESS;
            }
        }
    }
    // Sinon, recherche sequentielle standard.
    for (uint8_t i = 0; i < N_BYTES; i++)
    {
        if(posi_premier_bit == i)
        {
            continue;
        }
        if((8 - g_bytes_used[i]) >= siz)
        {
            *retur_var=i;
            return CAN_TG_SUCCESS;
        }
    }
    return CAN_TG_ERROR_NOT_ENOUNG_SPACE_;
}



// API publique: declaration de tag.
/** \brief Declare un nouveau tag et reserve son espace dans TxData.
 * \param tag_name Nom logique du tag.
 * \param siz Taille de la valeur en bits (implementation actuelle: 1..16).
 * \return Code de succes ou code d'erreur CAN_TG_STATUE.
 */
CAN_TG_STATUE set_tag(const char* tag_name, uint8_t siz){
    CAN_TG_STATUE statue_actuel;
    // Limite du nombre de tags.
    if(g_tag_count >= MAX_TAGS){
        return CAN_TG_ERROR_TOO_MAY_TAGS;
    }
    // Validation longueur nom (implementation actuelle basee sur sizeof(pointer)).
    if(sizeof(tag_name) >= MAX_TAG_NAME_LEN){
        return CAN_TG_ERROR_TAG_NAME_TO_BIGS;
    }
    // Interdit les doublons de nom.
    if(get_tag_def(tag_name) != NULL){
        return CAN_TG_ERROR_NAME_ALREADY_USE;
    }
    // Nom vide invalide.
    if(tag_name[0]=='\0'){
        return CAN_TG_ERROR_TAG_NAME_INVALIDE;
    }
    // Tailles acceptees: 1..16.
    if(siz <= 0 || siz > 16){return CAN_TG_ERROR_SIZE_VALUE_INVALIDE;}

    //------------------------- algo de placement memoire.

    if(siz>8){
        // Garde defensive (deja couverte plus haut).
        if(siz>16){
            return CAN_TG_ERROR_SIZE_VALUE_INVALIDE_;
        }
        else{
            uint8_t bytea, byteb;    // Index des bytes reserves.
            statue_actuel = find_byte_with_enoung_space(siz, &bytea);
            if(statue_actuel != CAN_TG_SUCCESS){return statue_actuel;}// propagation du statut
            statue_actuel = find_byte2_with_enoung_space(siz, bytea, &byteb);
            if(statue_actuel != CAN_TG_SUCCESS){
                return statue_actuel;   // propagation du statut
            }
            set_new_tag_9_to_16(tag_name, bytea, byteb, siz);
            return CAN_TG_SUCCESS;
        }
    }
    else{
        uint8_t bytea;
        statue_actuel = find_byte_with_enoung_space(siz, &bytea);
        if(statue_actuel != CAN_TG_SUCCESS){return statue_actuel;}// propagation du statut
        int n_tag = get_first_tags_vide();
        general_set_tag(n_tag, tag_name, siz, bytea);
        return CAN_TG_SUCCESS;
    }


};

/** \brief Recherche un tag actif par son nom.
 * \param tag_name Nom du tag recherche.
 * \return Pointeur sur TagDef si trouve, sinon NULL.
 */
const TagDef* get_tag_def(const char* tag_name)
{
    for (int i = 0; i < g_tag_count; i++)
    {
        if (strncmp(g_tags[i].name, tag_name, MAX_TAG_NAME_LEN) == 0)
        {
            return &g_tags[i];
        }
    }
    return NULL; // Tag non trouve
};



/* ===== END FILE: ./scripts/sub_scripts/tag_manager.c ===== */
