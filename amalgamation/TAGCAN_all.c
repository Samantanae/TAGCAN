/* TAGCAN amalgamated sources */
/* Generated on 2026-03-11 */


/* ===== BEGIN FILE: ./exemple_utilisation/com_utils.c ===== */

/*
 * com_utils.c
 *
 *  Created on: 4 Mar 2026
 *      Author: Samuel
 */
#include "com_utils.h"
#include "../../TAGCAN/include/sub_include/data_container.h"
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

CAN_RxHeaderTypeDef RxHeader;
uint8_t             RxData[8];

// valeurs
int8_t val_SEQ = 0;
int8_t val_HB = 0;
int8_t val_CI = 0;
int8_t val_AB = 0;
int8_t val_EDC = 0;
int16_t val_N2OPS = 0;
int16_t val_TLC = 0;
int8_t val_N2ODVS = 0;
int8_t val_N2OIV = 0;
int8_t val_N2OMV = 0;
int16_t val_TIME = 0;
int8_t out_Rx[8] = {0,0,0,0,0,0,0,0};


int datacheck = 0;    // permet d'update les valeurs uniquement lors de la reception de valeurs
// module pour CAN

#ifndef INC_COM_UTILS_H_
int8_t TxData_moteur[8] = {0,0,0,0,0,0,0,0};
int8_t RxData_moteur[8] = {0,0,0,0,0,0,0,0};
int32_t TxMailbox;
#endif
///* initialise tout les fonction de communication. (point d'appel unique: main) */
void init_com(void){
	// init module extene
	CAN_init_TAGCAN();
	CAN_setup_TAG();
	// init CAN (interne)
	CAN_setup_DF();

	// init msg
	if(HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING)!=HAL_OK)
	{
		Error_Handler();
	}
}


void CAN_init_TAGCAN(void){
	init_data_container();
	init_tag_manager();
}

void CAN_setup_DF(int32_t std_id){
	std_id &= 0x7FFU;
	TxHeader.IDE = CAN_ID_STD;		// Standard ID (11 bits)
	TxHeader.StdId = std_id;			// id
	TxHeader.RTR = CAN_RTR_DATA;	// sending data
	TxHeader.DLC = 8;				// nb of bytes
}



void CAN_setup_TAG(void){
	set_tag("SEQ", 3);		//*>Compteur de séquences 	*/
	set_tag("HB",1);		//*>Watchdog HeartBeat 		*/
	set_tag("CI",2);		//*>Continuité igniter 		*/
	set_tag("AB", 7);		//*>Alim. Batetterie (%) 	*/
	set_tag("EDC", 4);		//*>États des commandes 	*/
	set_tag("N2OPS",16);	//*>N2O Pressure Sensor 	*/
	set_tag("TLC",12);		//*>Thrust Load Cell 		*/
	set_tag("N2ODVS",1);	//*>N2O Dump Valve State 	*/
	set_tag("N2OIV",1);		//*>N2O Igniter Valve 		*/
	set_tag("N2OMV",1);		//*>N2O Main Valve 			*/
	set_tag("TIME",16);		//*>Timestamp (ms) 			*/
}


CAN_FilterTypeDef* CAN_setup_Filter(int32_t minimum_id, int32_t maximum_id, int filterBank){
	// understanding this https://youtu.be/JfWlIY0zAIc
	CAN_FilterTypeDef canfilterconfig;
	canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
	canfilterconfig.FilterBank = filterBank;		// Assign and Select Filter Banks
	canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	canfilterconfig.FilterIdHigh = maximum_id <<5;
	canfilterconfig.FilterMaskIdHigh = maximum_id << 5;
	canfilterconfig.FilterIdLow = minimum_id;
	canfilterconfig.FilterMaskIdLow = minimum_id;
	canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
	canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;    // 16 ou 32 bits register
	canfilterconfig.SlaveStartFilterBank = 20;    // how many filters to assign to the CAN1 (master can)

	HAL_CAN_ConfigFilter(&hcan, &canfilterconfig);
	return &canfilterconfig;
}



///////////////////////////////////////////////////////////////
//--------------!!!!-DANS LE MAIN LOOP!!!!!--------------------
///////////////////////////////////////////////////////////////





void main_loop(void){
	update_all_val_send();
	CAN_send_msg();
}

void update_TagData(void){
	if(datacheck == 1){
		change_val_to_TagData(&out_Rx);
	}
}

void CAN_send_msg(void){
	if(HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox)!=HAL_OK){
		Error_Handler();
	}
}

void CAN_change_val(void){

}

void update_val(const char* nom_du_tag, int32_t val){
	set_tag(nom_du_tag, val);
}

void update_all_val_send(void){	//TODO: changer toute les constantes ici par la source actuelle de donné (ou ne jamais appeller cette fonction si c'est le recever)
	set_value("SEQ", 3);		//*>Compteur de séquences 	*/
	set_value("HB", 1);		//*>Watchdog HeartBeat 		*/
	set_value("CI", 2);		//*>Continuité igniter 		*/
	set_value("AB", 7);		//*>Alim. Batetterie (%) 	*/
	set_value("EDC", 4);		//*>États des commandes 	*/
	set_value("N2OPS", 16);	//*>N2O Pressure Sensor 	*/
	set_value("TLC", 12);		//*>Thrust Load Cell 		*/
	set_value("N2ODVS", 1);	//*>N2O Dump Valve State 	*/
	set_value("N2OIV", 1);		//*>N2O Igniter Valve 		*/
	set_value("N2OMV", 1);		//*>N2O Main Valve 			*/
	set_value("TIME", 16);		//*>Timestamp (ms) 			*/
}


void update_all_val_receve(void){

}








////////////////////////////////////////////////////////
// événementielle
///////////////////////////////////////////////////////



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
  {
    Error_Handler();
  }
  if ((RxHeader.StdId == 0x103))    // suposé être l'id de l'autre CAN
  {
	  datacheck = 1;
	  for (int i = 0; i < N_BYTES; i++){
		  out_Rx[i] = RxData[i];
	  }

	  // update du conteneur de donnée.
	  //change_val_to_TagData(&RxData);
  }
}










/* ===== END FILE: ./exemple_utilisation/com_utils.c ===== */

/* ===== BEGIN FILE: ./scripts/CAN_utils.c ===== */




/* ===== END FILE: ./scripts/CAN_utils.c ===== */

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

/* ===== BEGIN FILE: ./scripts/main.c ===== */

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#ifndef CONFIG_VALUE_H
#include "../include/config_value.h"
#endif // CONFIG_VALUE_H
#ifndef DATA_CONTAINER_H
#include "../include/sub_include/data_container.h"
#endif // DATA_CONTAINER_H
#ifndef GESTION_TAG_H
#include "../include/gestion_tag.h"
#endif // GESTION_TAG_H
#ifndef PRINT_VAL_H
#include "../include/sub_include/print_val.h"
#endif // PRINT_VAL_H

int passed_tests = 0; /**< le nombre total de test réussi */
int total_tests = 0; /**< le nombre total de test effectué */

/** \brief Affiche un resultat de test avec format uniforme.
 *
 * \param name Le nom du test (peut aussi servir de description courte).
 * \param condition Resultat du test (true si reussi, false sinon).
 *
 */
void TEST(const char* name, const bool condition)
{
    if(condition)
    {
        printf("success\t\t\t%s\n", name);
        passed_tests++;
    }
    else
    {
        printf("fail\t\t\t%s\n", name);
    }
    total_tests++;
}
/** \brief Affiche un test d'egalite entre deux entiers.
 *
 * \param name Meme usage que TEST.
 * \param val_1 Premiere valeur a comparer.
 * \param val_2 Deuxieme valeur a comparer.
 *
 */
void TEST_EQ_INT(const char* name, const int val_1, const int val_2)
{
    bool condition = val_1 == val_2;
    if(condition)
    {
        printf("success\t\t\t%s\n", name);
        passed_tests++;
    }
    else
    {
        printf("fail\t\t\t%s", name);
        printf("\t%d != ", val_1);
        printf("%d\n", val_2);
    }
    total_tests++;
}







/** \brief Execute un scenario de demonstration des fonctions d'impression.
 * \details Initialise des tags/valeurs puis affiche metadonnees et repartition.
 */
void test_print(void){
printf("test des fonction d'impression.\n");
// prepa du gt
init_data_container();
init_tag_manager();

// set the tags
set_tag("a1",2);
set_tag("agh5",5);
set_tag("patap",16);
set_tag("patat",12);
set_tag("uni1",1);
set_tag("triot", 3);

// set the value
set_value("a1",3);
set_value("agh5",42);
set_value("patap",45);
set_value("patat",12);
set_value("uni1",1);
set_value("triot", 3);

print_all_tag_set();
print_all_data();
print_repartition_bit();
}




/** \brief Scenario de test manuel des operations de masque/shift.
 */
void somme_other_test(void){
printf("----------------------test mask ------------------------\n");
    printf("\t(0,8): ");
    print_bin_8(prep_mask(0, 8));
    printf("\n");
    printf("\t(4,4): ");
    print_bin_8(prep_mask(4, 4));
    printf("\n");
    printf("\t(4,3): ");
    print_bin_8(prep_mask(4, 3));
    printf("\n");
    printf("\t(1,4): ");
    print_bin_8(prep_mask(1, 4));
    printf("\n");
    printf("\t(5,1): ");
    print_bin_8(prep_mask(5, 1));
    printf("\n");
    printf("------------------------------test shift----------------------\n");
    uint8_t val1 = 0b01110000;
    uint8_t bi = 0b01110100;
    printf("val1: ");
    print_bin_8(val1);
    printf("\n");
    int p_bit = 4;
    int n_bit = 4;
    // eq logic pour la prise de la valeur
    uint8_t vrai_val = val1 >> (8 - p_bit - n_bit);
    // prepa du mask
    uint8_t mask_val = 0b1;
    uint8_t mask_temp = 0b1;
    // mask des bits de la valeurs
    for (int i = 0; i < (n_bit - 1); i++)
    {
        mask_val = mask_val << 1;
        mask_val |= mask_temp;
    }
    // mask pour conserver uniquement la valeurs dans le bytes
    uint8_t mask_val_p = mask_val << (8 - p_bit - n_bit);

    uint8_t byte_sans_val = bi;
    byte_sans_val &= ~mask_val_p;


    //mask_val = mask_val >> (8 - n_bit);
    printf("vrai_val: ");
    print_bin_8(vrai_val);
    printf("\n");
    printf("mask: ");
    print_bin_8(mask_val);
    printf("\n");
    printf("mask2: ");
    print_bin_8(mask_val_p);
    printf("\n");
    printf("byte avant: ");
    print_bin_8(bi);
    printf("\n");
    printf("byte sans val: ");
    print_bin_8(byte_sans_val);
    printf("\n");


    printf("test final\n");

    init_data_container();
    init_tag_manager();

}

/** \brief Execute les tests unitaires du module.
 *      Les resultats sont affiches dans le terminal.
 *
 */

void run_all_tests(void)
{
    uint32_t val = 0;

    printf("=== Lancement des tests unitaires ===\n");

    // ---------------------------------------------------------
    // TEST 1 : Cas d'erreurs de set_tag
    // ---------------------------------------------------------
    printf("Test 1: Erreurs de set_tag... ");
    init_tag_manager(); // On part d'un état propre

    // Tailles invalides (ex: 0 retourne CAN_TG_ERROR_SIZE_VALUE_INVALIDE)
    printf("\tTailles invalides\n");
    TEST_EQ_INT("0",set_tag("inv1", 0), CAN_TG_ERROR_SIZE_VALUE_INVALIDE);
    TEST_EQ_INT("2",set_tag("inv2", 2), CAN_TG_SUCCESS);
    TEST_EQ_INT("5",set_tag("inv5", 5), CAN_TG_SUCCESS);

    // Tag en double (retour attendu: CAN_TG_ERROR_NAME_ALREADY_USE)
    printf("\tTag en double\n");
    TEST_EQ_INT("première insertion",set_tag("TAG_A", 4), SUCCES_TO_SET); // Succès
    TEST_EQ_INT("duplica détection",set_tag("TAG_A", 3), CAN_TG_ERROR_NAME_ALREADY_USE); // Erreur : existe déjà
    printf("OK\n");

    // ---------------------------------------------------------
    // TEST 2 : Dépassement de l'espace mémoire (8 octets max)
    // ---------------------------------------------------------
    printf("Test 2: Remplissage memoire (8 bytes max)... ");
    init_tag_manager();

    // On ajoute 4 tags de 16 bits (4 * 2 = 8 octets complets)
    printf("\tpreparation du GT\n");
    TEST_EQ_INT("ajout de valeurs 16 bit (1/4)",set_tag("T1", 16), SUCCES_TO_SET);
    TEST_EQ_INT("ajout de valeurs 16 bit (2/4)",set_tag("T2", 16), SUCCES_TO_SET);
    TEST_EQ_INT("ajout de valeurs 16 bit (3/4)",set_tag("T3", 16), SUCCES_TO_SET);
    TEST_EQ_INT("ajout de valeurs 16 bit (4/4)",set_tag("T4", 16), SUCCES_TO_SET);

    // Le prochain tag doit etre refuse faute d'espace libre.
    printf("\ttest de Dépassement de l'espace mémoire (8 octets max)\n");
    TEST_EQ_INT("avec 1 bit",set_tag("T5", 1), CAN_TG_ERROR_NOT_ENOUNG_SPACE);
    TEST_EQ_INT("avec 16 bits",set_tag("T6", 16), CAN_TG_ERROR_NOT_ENOUNG_SPACE);
    printf("OK\n");

    // ---------------------------------------------------------
    // TEST 3 : Cas d'erreurs de set_value et get_value
    // ---------------------------------------------------------
    printf("Test 3: Erreurs et depassements de valeurs... ");
    init_tag_manager();
    init_data_container();
    printf("\tpreparation du GT\n");
    TEST_EQ_INT("prepa... ",set_tag("BIT_1", 1), SUCCES);
    TEST_EQ_INT("prepa... ",set_tag("BIT_3", 3), SUCCES);
    TEST_EQ_INT("prepa... ",set_tag("BIT_4", 4), SUCCES);

    // Tag inexistant (retour attendu: -3)
    printf("\tTag inexistant\n");
    TEST_EQ_INT("test set",set_value("FANTOME", 5), -3);
    TEST_EQ_INT("test get",get_value("FANTOME", &val), -3);

    // Depassement de capacite:
    // NOTE: l'implementation actuelle renvoie CAN_TG_ERROR_VALUE_TO_BIG (-10),
    // mais ces assertions historiques verifient encore -4.
    printf("\tDépassement de capacité\n");
    // - Un tag de 1 bit ne peut contenir que 0 ou 1
    TEST_EQ_INT("Un tag de 1 bits[0 ou 1]",set_value("BIT_1", 2), -4);
    // - Un tag de 3 bits ne peut contenir que de 0 à 7
    TEST_EQ_INT("Un tag de 3 bits [0 à 7]",set_value("BIT_3", 8), -4);
    // - Un tag de 4 bits ne peut contenir que de 0 à 15
    TEST_EQ_INT("Un tag de 4 bits [0 à 15]", set_value("BIT_4", 16), -4);
    printf("OK\n");

    // ---------------------------------------------------------
    // TEST 4 : Indépendance des écritures (Masquage binaire)
    // ---------------------------------------------------------
    printf("Test 4: Indépendance des bits contigus... ");
    init_tag_manager();
    init_data_container();

    // On crée une configuration qui remplit exactement 1 octet (1 + 3 + 4 = 8 bits)
    printf("\tpreparation du GT\n");
    TEST_EQ_INT("preparation [set_tag] (1/3)",set_tag("T_1", 1), SUCCES);
    TEST_EQ_INT("preparation [set_tag] (2/3)",set_tag("T_3", 3), SUCCES);
    TEST_EQ_INT("preparation [set_tag] (3/3)",set_tag("T_4", 4), SUCCES);

    // On assigne les valeurs maximales à chaque tag
    printf("\tpreparation du GT (partie 2)\n");
    TEST_EQ_INT("preparation [set_tag] (1) 1 bits (1/3)",set_value("T_1", 1), SUCCES);
    TEST_EQ_INT("preparation [set_tag] (7) 3 bits (2/3)",set_value("T_3", 7), SUCCES);
    TEST_EQ_INT("preparation [set_tag] (15) 4 bits (3/3)",set_value("T_4", 15), SUCCES);

    // On vérifie qu'elles sont bien mémorisées
    printf("\t\n");
    get_value("T_1", &val);
    TEST_EQ_INT("get value T_1",val, 1);
    get_value("T_3", &val);
    TEST_EQ_INT("get value T_3",val, 7);
    get_value("T_4", &val);
    TEST_EQ_INT("get value T_4",val, 15);

    // On modifie T_3 sans toucher aux autres
    printf("\tmodification de T_3 sans toucher aux autres\n");
    TEST_EQ_INT("",set_value("T_3", 2), 1);

    // On vérifie que T_1 et T_4 n'ont pas bougé
    printf("\t\n");
    get_value("T_1", &val);
    TEST_EQ_INT("verif valeur [encien]",val, 1);
    get_value("T_3", &val);
    TEST_EQ_INT("verif valeur [nouvel]",val, 2); // Nouvelle valeur
    get_value("T_4", &val);
    TEST_EQ_INT("verif valeur [encien]",val, 15);
    printf("OK\n");

    // ---------------------------------------------------------
    // TEST 5 : Variables de 8 bits
    // ---------------------------------------------------------
    printf("Test 5: Variables 8 bits... ");
    init_tag_manager();
    init_data_container();
    printf("\t\n");
    TEST_EQ_INT("prapa de la valeur",set_tag("V8", 8), SUCCES);

    // Test des bornes 0 et 255
    printf("\tTest des bornes 0 et 255\n");
    set_value("V8", 0);
    get_value("V8", &val);
    TEST_EQ_INT("val = 0",val, 0);
    set_value("V8", 255);
    get_value("V8", &val);
    TEST_EQ_INT("val = 255",val, 255);
    printf("\tErreur dépassement\n");
    TEST_EQ_INT("val: [256]",set_value("V8", 256), -4); // Assertion historique, a realigner avec -10
    printf("OK\n");

    //--------------------------------------
    // TEST 6: Tout les test de 4 bits
    //--------------------------------------
    printf("\ttest de tags à 4 bits (tout les type possible)\n");
    init_tag_manager();
    init_data_container();
    printf("test d'inserssion des tags\n");
    TEST_EQ_INT("ajout 1 tags à 4 bits ", set_tag("ma_val1", 4),SUCCES);
    TEST_EQ_INT("ajout 1 tags après 4 bits ", set_tag("ma_2", 4),SUCCES);
    printf("le dernier cas n'ais pas encore testé!!!!\n");
    printf("test  d'ajout de valeurs\n");
    TEST_EQ_INT("set val",set_value("ma_val1", 3),1);
    TEST_EQ_INT("set val 2",set_value("ma_2", 9),1);
    get_value("ma_val1", &val);
    TEST_EQ_INT("get val 1",val,3);;
    get_value("ma_2", &val);
    TEST_EQ_INT("get val 2",val,9);;
    //printf("=== TOUS LES TESTS SONT PASSES AVEC SUCCES ! ===\n\n");
}

int main(void)
{


    // Il suffit d'appeler la fonction de test au début de votre main
    run_all_tests();
    test_print();


    // ... suite de votre code (simulation CAN, etc.) ...
    return 0;
}


/* ===== END FILE: ./scripts/main.c ===== */

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

/* ===== BEGIN FILE: ./tests/test_file/test_print_repartition_bit.c ===== */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../include/gestion_tag.h"
#include "../../include/sub_include/data_container.h"
#include "../../include/sub_include/print_val.h"

static int contains_text(const char* haystack, const char* needle)
{
    return strstr(haystack, needle) != NULL;
}

static int capture_print_repartition_output(char* out_buf, size_t out_size)
{
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        return -1;
    }

    fflush(stdout);
    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout < 0) {
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }

    if (dup2(pipefd[1], STDOUT_FILENO) < 0) {
        close(saved_stdout);
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }

    close(pipefd[1]);

    init_data_container();
    init_tag_manager();

    if (set_tag("a1", 2) <= 0 ||
        set_tag("agh5", 5) <= 0 ||
        set_tag("patap", 16) <= 0 ||
        set_tag("patat", 12) <= 0 ||
        set_tag("uni1", 1) <= 0 ||
        set_tag("triot", 3) <= 0) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
        close(pipefd[0]);
        return -1;
    }

    print_repartition_bit();
    fflush(stdout);

    if (dup2(saved_stdout, STDOUT_FILENO) < 0) {
        close(saved_stdout);
        close(pipefd[0]);
        return -1;
    }
    close(saved_stdout);

    size_t total = 0;
    while (total + 1 < out_size) {
        ssize_t n = read(pipefd[0], out_buf + total, out_size - total - 1);
        if (n <= 0) {
            break;
        }
        total += (size_t)n;
    }
    close(pipefd[0]);
    out_buf[total] = '\0';

    return (int)total;
}

int main__(void)
{
    char output[8192];
    int n = capture_print_repartition_output(output, sizeof(output));
    if (n <= 0) {
        fprintf(stderr, "[FAIL] capture output failed\n");
        return 1;
    }

    if (!contains_text(output, "=== REPARTITION DES BITS DANS LA TRAME CAN ===")) {
        fprintf(stderr, "[FAIL] header missing\n");
        return 1;
    }

    if (!contains_text(output, "Legende des tags:")) {
        fprintf(stderr, "[FAIL] legend title missing\n");
        return 1;
    }

    if (!contains_text(output, "a1 : [a]") ||
        !contains_text(output, "agh5 : [b]") ||
        !contains_text(output, "patap : [c]") ||
        !contains_text(output, "patat : [d]") ||
        !contains_text(output, "uni1 : [e]") ||
        !contains_text(output, "triot : [f]")) {
        fprintf(stderr, "[FAIL] legend entries missing\n");
        return 1;
    }

    for (int i = 0; i < N_BYTES; i++) {
        char pattern[32];
        snprintf(pattern, sizeof(pattern), " [%d] | ", i);
        if (!contains_text(output, pattern)) {
            fprintf(stderr, "[FAIL] missing byte row: %d\n", i);
            return 1;
        }
    }

    printf("[PASS] print_repartition_bit: structure de sortie valide\n");
    return 0;
}


/* ===== END FILE: ./tests/test_file/test_print_repartition_bit.c ===== */

/* ===== BEGIN FILE: ./tests/test_file/test_safty_func.c ===== */



/* ===== END FILE: ./tests/test_file/test_safty_func.c ===== */

/* ===== BEGIN FILE: ./tests/test_file/test_send_receve.c ===== */



/* ===== END FILE: ./tests/test_file/test_send_receve.c ===== */

/* ===== BEGIN FILE: ./tests/test_file/test_set_tag.c ===== */



/* ===== END FILE: ./tests/test_file/test_set_tag.c ===== */

/* ===== BEGIN FILE: ./tests/test_utils.c ===== */

#include "test_utils.h"
#include <stdbool.h>
#include <stdio.h>
extern int total_tests;
extern int passed_tests;

void TEST_(const char* name, const bool condition){
    if(condition){printf("\tsuccess\t%s\n", name);}
    else{printf("\tfail\t%s\n", name);}
}
void TEST_EQ_INT_(const char* name, const int val_1, const int val_2){
    bool condition = val_1 == val_2;
    if(condition){
        printf("\tsuccess\t%s\n", name);
    passed_tests++;}
    else{
        printf("\tfail\t%s", name);
        printf("\t%d != ", val_1);
        printf("%d\n", val_2);
    }
    total_tests++;
}


/* ===== END FILE: ./tests/test_utils.c ===== */
