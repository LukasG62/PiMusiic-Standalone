/**
 * @file osc_triangle.c
 * @brief Oscillateur d'onde triangulaire
 * @author Lukas Grando
 * @version 1.0
 */

#include "sound/osc/osc.h"

/**
 * @brief Crée un oscillateur triangulaire
 * @return osc_t* l'oscillateur créé
 * @warning L'oscillateur doit être détruit avec sa méthode destroy pour libérer la mémoire
 */
osc_t *osc_triangle_create();
