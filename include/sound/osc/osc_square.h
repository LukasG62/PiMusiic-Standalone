/**
 * @file osc_square.c
 * @brief Oscillateur d'onde carrée
 * @author Lukas Grando
 * @version 1.0
 */

#ifndef OSC_SQUARE_H
#define OSC_SQUARE_H

#include "osc.h"

/**
 * @brief Crée un oscillateur carré
 * @return osc_t* l'oscillateur créé
 * @warning L'oscillateur doit être détruit avec sa méthode destroy pour libérer la mémoire
 */
osc_t *osc_square_create();

#endif // OSC_SQUARE_H