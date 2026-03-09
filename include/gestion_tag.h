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
