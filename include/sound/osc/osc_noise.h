/**
 * @file osc_noise.h
 * @brief Oscillateur de bruit utilisant la méthode LFSR
 * @author Lukas Grando
 * @version 1.0
 */
#ifndef OSC_NOISE_H
#define OSC_NOISE_H

#include "osc.h"

// todo: créer une config pour cet osc sur le lfsr :
// - taille du registre (8, 16, 32 bits)
// - taps (bits à xorer pour le feedback)
// - état initial (doit être non nul)

/**
 * @brief Crée un oscillateur de bruit
 * @return osc_t* l'oscillateur créé
 * @warning L'oscillateur doit être détruit avec osc_destroy() pour libérer
 */
osc_t *osc_noise_create();

#endif