/**
 * @file osc.h
 * @brief Interface générique pour les oscillateurs audio
 * @author Lukas Grando
 * @version 2.0
 */

#ifndef OSC_H
#define OSC_H

#include "common.h"

#include <stddef.h>
#include <stdbool.h>

// note :
// - tous les oscillateurs doivent implémenter les fonctions generate et destroy
// - La fonction generate doit remplir le buffer avec des valeurs normalisées entre -1.0 et 1.0
// - La fonction destroy doit libérer toute la mémoire allouée pour l'oscillateur et son state (si il y en a un)
typedef struct __osc_t osc_t;

/**
 * @typedef osc_generate_func_t
 * @brief Remplit un buffer avec le signal généré
 * @param self la structure de l'oscillateur
 * @param freq La fréquence en Hz
 * @param sampleRate Le taux d'échantillonnage en Hz
 * @param buffer le buffer à remplir avec les samples générés
 * @param size Le nombre de samples à générer
 * @warning le buffer doit être alloué et de taille minimale size * sizeof(double)
 */
typedef void (*osc_generate_func_t)(osc_t *self, double freq, double sample_rate, double *buffer, size_t size);

/**
 * @typedef osc_destroy_func_t
 * @brief Libère la mémoire allouée pour l'oscillateur et son state
 * @param self L'instance de l'oscillateur
 */
typedef void (*osc_destroy_func_t)(osc_t *self);

/**
 * @struct osc_t
 * @brief Structure représentant un oscillateur audio
 * @details Cette structure encapsule les données necessaires pour générer un signal audio
 */
struct __osc_t {
    void *state; /*!< Pointe vers les données internes propres à l'oscillateur */

	osc_generate_func_t generate; /*!< Fonction pour générer le signal audio */
    osc_destroy_func_t destroy; /*!< Fonction pour libérer la mémoire */
};

#endif // OSC_H