/**
 * @file osc_sawtooth.c
 * @brief Oscillateur d'onde en dents de scie
 * @author Lukas Grando
 * @version 1.0
 */

#ifndef OSC_SAWTOOTH_H
#define OSC_SAWTOOTH_H

#include "osc.h"

/**
 * @brief Crée un oscillateur en dents de scie
 * @return osc_t* l'oscillateur créé
 * @warning L'oscillateur doit être détruit avec sa méthode destroy pour libérer la mémoire
 */
osc_t *osc_sawtooth_create();

#endif // OSC_SAWTOOTH_H