/**
 * @file osc_sine.h
 * @brief Oscillateur sinusoïdal
 */
#ifndef OSC_SINE_H
#define OSC_SINE_H

#include "osc.h"

/**
 * @brief Crée un oscillateur sinusoïdal
 * @return osc_t* l'oscillateur créé
 * @warning L'oscillateur doit être détruit avec sa méthode destroy pour libérer la mémoire
 */
osc_t *osc_sin_create(void);

#endif // OSC_SINE_H