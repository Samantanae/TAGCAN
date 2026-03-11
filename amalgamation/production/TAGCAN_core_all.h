/* TAGCAN core amalgamated headers */
/* Generated on 2026-03-11 */


/* ===== BEGIN FILE: ./TAGCAN.h ===== */

#ifndef TAGCAN_H
#define TAGCAN_H

#include "include/config_value.h"
#include "include/gestion_tag.h"
#include "include/sub_include/data_container.h"
#include "include/sub_include/tag_manager.h"

#endif /* TAGCAN_H */


/* ===== END FILE: ./TAGCAN.h ===== */

/* ===== BEGIN FILE: ./include/config_value.h ===== */

#ifndef CONFIG_VALUE_H
#define CONFIG_VALUE_H

/** Nombre maximal de tags geres simultanement. */
#define MAX_TAGS 32
/** Longueur maximale d'un nom de tag (incluant le '\0'). */
#define MAX_TAG_NAME_LEN 16
/** Taille du payload CAN en octets. */
#define N_BYTES 8
/** Nombre maximal de gestionnaires (reserve/extension). */
#define MAX_N_GT 3

/** Code d'erreur: corruption interne detectee. */
extern const int CAN_TG_ERROR_INTERNAL_CORUPTION;
/** Code d'erreur: nom de tag trop long. */
extern const int CAN_TG_ERROR_TAG_NAME_TO_BIGS;
/** Code d'erreur: nom de tag invalide. */
extern const int CAN_TG_ERROR_TAG_NAME_INVALIDE;
/** Code d'erreur: tag introuvable. */
extern const int CAN_TG_ERROR_TAG_NOT_FOUND;
/** Code d'erreur: espace memoire insuffisant. */
extern const int CAN_TG_ERROR_NOT_ENOUNG_SPACE;
/** Code d'erreur: pointeur NULL recu. */
extern const int CAN_TG_ERROR_NULL_POINTOR;
/** Code d'erreur: fin de chaine non trouvee. */
extern const int CAN_TG_ERROR_TRY_TO_FIND_END_CARACTER;
/** Code d'erreur: taille de valeur invalide. */
extern const int CAN_TG_ERROR_SIZE_VALUE_INVALIDE;
/** Code d'erreur: echec de subdivision/allocation. */
extern const int CAN_TG_ERROR_SUBDIVISION_FAIL;
/** Code d'erreur: trop de tags declares. */
extern const int CAN_TG_ERROR_TOO_MAY_TAGS;
/** Code d'erreur: nom de tag deja utilise. */
extern const int CAN_TG_ERROR_NAME_ALREADY_USE;
/** Code de succes historique pour set_tag/set_value. */
extern const int SUCCES_TO_SET;
/** Code de succes generique historique. */
extern const int SUCCES;
// Codes de retour utilises par l'API TAGCAN
/** \brief Codes de retour du module TAGCAN. */
typedef enum {
    CAN_TG_ERROR_NOT_ENOUNG_SPACE_ = -2,
    CAN_TG_SUCCESS = 1,
    CAN_TG_ERROR_TAG_NOT_FOUND_ = -3,
    CAN_TG_ERROR_TAG_NAME_TO_BIGS_ = -4,
    CAN_TG_ERROR_TOO_MAY_TAGS_ = -5,
    CAN_TG_ERROR_SIZE_VALUE_INVALIDE_ = -6,
    CAN_TG_ERROR_NAME_ALREADY_USE_ = -7,
    CAN_TG_ERROR_SUBDIVISION_FAIL_ = -8,
    CAN_TG_ERROR_TAG_NAME_INVALIDE_=-9,
    CAN_TG_ERROR_NULL_POINTOR_ = -2,
    CAN_TG_ERROR_TRY_TO_FIND_END_CARACTER_ = -21,
    CAN_TG_ERROR_INTERNAL_CORUPTION_ = -30,
    CAN_TG_ERROR_VALUE_TO_BIG = -10,
} CAN_TG_STATUE;

/** \brief modes d'utilisation CAN (envoi/reception). */
typedef enum {
    RECEVE_ONLY,    /**< Réception seule: les mises à jour entrantes peuvent modifier la valeur; l'écriture locale est bloquée. */
    SEND_ONLY,    /**< Émission seule: la valeur locale est mise, les mises à jour entrantes ne la modifient pas. */
    SEND_AND_RECEVE,    /**< Emission + reception: écriture locale et mises à jour entrantes toutes deux autorisées. */
} CAN_MODE;

#endif // CONFIG_VALUE_H


/* ===== END FILE: ./include/config_value.h ===== */

/* ===== BEGIN FILE: ./include/gestion_tag.h ===== */

#ifndef GESTION_TAG_H
#define GESTION_TAG_H

// fonction qui gere les tags
/** \brief Initialise/reinitialise le gestionnaire de tags.
 * \note Apres cet appel, l'ensemble des tags doit etre redefini via `set_tag`.
 */

void init_tag_manager(void);

/** \brief créé un nouveau tag.
 *
 * \param tag_name le nom (id) relié a la valeur.
 * \param n_bit La taille de la valeur (implémentation actuelle: 1 à 16 bits).
 * \return un code de succès ou d'erreur.
 */
int set_tag(const char* tag_name, uint8_t n_bit);

#endif /* GESTION_TAG_H */

#ifndef GESTION_TAG_H
#define GESTION_TAG_H
#include <stdint.h>
// fonction qui gère les tags
/** \brief initialise/réinitialise le gestionnaire de tags.
 * \note après cet appel, l'ensemble des tags doit être redéfini via `set_tag`.
 */

void init_tag_manager(void);

/** \brief créé un nouveau tag.
 *
 * \param tag_name Le nom (id) relié a la valeur.
 * \param n_bit La taille de la valeur (implémentation actuelle: 1. 16 bits).
 * \return Un code de succès ou d'erreur.
 */
int set_tag(const char* tag_name, uint8_t n_bit);

#endif /* GESTION_TAG_H */


/* ===== END FILE: ./include/gestion_tag.h ===== */

/* ===== BEGIN FILE: ./include/sub_include/data_container.h ===== */

#ifndef DATA_CONTAINER_H

#include <stdint.h>

#include "tag_manager.h"

#define DATA_CONTAINER_H

// Buffer de donnees CAN (payload de 8 octets)
extern uint8_t TxData[N_BYTES];    /**< Buffer de donnees CAN (payload de 8 octets). */

/** \brief Reinitialise le payload TxData a zero.
 * \details Si reexecutee, la fonction efface les donnees precedemment ecrites.
 */

void init_data_container(void);

// Fonctions principales (set/get)
/** \brief Ecrit une valeur dans TxData a partir d'un tag.
 * \warning La valeur doit tenir dans la taille du tag.
 * \details En cas de depassement, l'implementation retourne CAN_TG_ERROR_VALUE_TO_BIG.
 * \param tag_name Nom du tag cible.
 * \param value Nouvelle valeur a ecrire.
 * \return Code de succes ou code d'erreur.
 */

int set_value(const char* tag_name, uint32_t value);

/** \brief Lit la valeur associee a un tag.
 *
 * \param tag_name Nom du tag a lire.
 * \param out_value Adresse de sortie qui recoit la valeur extraite.
 * \return CAN_TG_SUCCESS en cas de succes, sinon un code d'erreur negatif.
 *
 */
int get_value(const char* tag_name, uint32_t* out_value);


/** \brief [FONCTION INTERNE] Prepare un masque binaire pour set/get.
 * \details Utilise principalement pour les valeurs strictement inferieures a 8 bits.
 * \param p_bit Position du premier bit (de gauche a droite).
 * \param n_bit Nombre de bits de la valeur.
 * \return Masque binaire aligne sur la position cible.
 *
 */
uint8_t prep_mask(uint8_t p_bit, uint8_t n_bit);
/** \brief Simule un envoi/reception CAN par copie buffer-a-buffer.
 * \param tx_buffer Buffer source (emission).
 * \param rx_buffer Buffer destination (reception).
 */
void can_simulate_send_receive(uint8_t* tx_buffer, uint8_t* rx_buffer);

//int CAN_RECEVE

/** \brief Copie un tableau externe dans le buffer global TxData.
 * \param TxDataArray Tableau source de N_BYTES octets.
 */
void set_val_TxData(uint8_t *TxDataArray);


#endif // DATA_CONTAINER_H


/* ===== END FILE: ./include/sub_include/data_container.h ===== */

/* ===== BEGIN FILE: ./include/sub_include/print_val.h ===== */

#ifndef PRINT_VAL_H

#include <stdio.h>
#include <stdint.h>

#define PRINT_VAL_H

// Utilitaires d'affichage pour le debogage
/** \brief Affiche la representation binaire (8 bits) d'une valeur.
 *
 * \param val Valeur a afficher en binaire sur 8 bits.
 */

void print_bin_8(uint8_t val);

/** \brief Affiche tout le payload TxData en binaire.
 * \details Chaque octet est affiche sur 8 bits, dans l'ordre du buffer.
 */
void print_all_data_bin(void);

/** \brief Affiche TxData en binaire avec un titre en prefixe.
 * \param title Texte affiche avant la ligne binaire.
 */
void print_all_with_title(const char* title);

/** \brief Affiche la table des tags declares et leurs metadonnees.
 */
void print_all_tag_set(void);

/** \brief Affiche toutes les valeurs logiques relues via get_value.
 */
void print_all_data(void);

/** \brief Affiche la repartition des bits utilises dans la trame CAN.
 */
void print_repartition_bit(void);

/** \brief Buffer de rendu pour l'affichage de repartition bit-a-bit. */
typedef struct{
    char bi[8];

} tsfp;

/** Espace global de rendu utilise par les fonctions d'impression de repartition. */
extern tsfp gbc2[9];

#endif /* PRINT_VAL_H */


/* ===== END FILE: ./include/sub_include/print_val.h ===== */

/* ===== BEGIN FILE: ./include/sub_include/tag_manager.h ===== */

#ifndef TAG_MANAGER_H
#define TAG_MANAGER_H

#ifndef CONFIG_VALUE_H
#include "../config_value.h"
#endif // CONFIG_VALUE_H
#include <stdint.h>
#include <stdbool.h>


/**
 * \brief Structure de definition d'un tag.
 */
typedef struct
{
    char name[MAX_TAG_NAME_LEN];    /**< Nom logique du tag. Doit etre unique. */
    uint8_t n_bits;    /**< Taille en bits (implementation actuelle: 1..16, 0 si slot inutilise). */
    int8_t byte_idx_a;    /**< Index du byte de la partie A (0..7), -1 si non utilise. */
    int8_t byte_idx_b;    /**< Index du byte de la partie B (tags > 8 bits), sinon -1. */
    uint8_t bit_pos_a;      /**< Position de debut dans le byte A (de gauche a droite). */
    uint8_t bit_pos_b;      /**< Position de debut dans le byte B pour les tags > 8 bits, 0 sinon. */
} TagDef;

// Etat global du gestionnaire de tags

extern TagDef g_tags[MAX_TAGS];    /**< Table des tags (actifs et inactifs). */
extern uint8_t g_tag_count;    /**< Nombre de tags actifs (max: MAX_TAGS). */
// Nombre de bits deja utilises dans chaque byte (0..7)
extern uint8_t g_bytes_used[N_BYTES];    /**< Occupation courante en bits pour chaque byte du payload. */

// structure de multi gestionnaire de tags (en dévelopement)

/*typedef struct {


}*/

//--------------------------------fonctions d'acces generales ------------------------
/** \brief Recherche un tag et retourne sa definition.
 * \warning API surtout utilisee en interne par le module de valeurs.
 * \param tag_name Nom du tag a chercher.
 * \return Pointeur vers le tag si trouve, sinon NULL.
 */
const TagDef* get_tag_def(const char* tag_name);

#endif // TAG_MANAGER_H


/* ===== END FILE: ./include/sub_include/tag_manager.h ===== */

/* ===== BEGIN FILE: ./version.h ===== */

#ifndef VERSION_H
#define VERSION_H

	//Date Version Types
	static const char DATE[] = "03";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2026";
	static const char UBUNTU_VERSION_STYLE[] =  "26.03";

	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";

	//Standard Version Type
	static const long MAJOR  = 9;
	static const long MINOR  = 0;
	static const long BUILD  = 24;
	static const long REVISION  = 114;

	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 135;
	#define RC_FILEVERSION 9,0,24,114
	#define RC_FILEVERSION_STRING "9, 0, 24, 114\0"
	static const char FULLVERSION_STRING [] = "9.0.24.114";

	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 24;

#endif //VERSION_H


/* ===== END FILE: ./version.h ===== */
