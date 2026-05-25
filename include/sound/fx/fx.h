/**
 * @file fx.h
 * @brief Interface générique pour les effets audio
 * @author Lukas Grando
 * @version 1.0
 */
#ifndef FX_H
#define FX_H

#include <stddef.h>
#include "common.h"

typedef struct __fx_t fx_t;

/**
 * @typedef fx_process_func_t
 * @brief Traite un buffer audio in-place
 * @param self L'instance de l'effet
 * @param buffer Le buffer audio contenant les samples
 * @param size Le nombre de samples dans le buffer
 */
typedef void (*fx_process_func_t)(fx_t *self, double *buffer, size_t size);

/**
 * @typedef fx_destroy_func_t
 * @brief Libère la mémoire de l'effet et de son état interne
 * @param self L'instance de l'effet
 */
typedef void (*fx_destroy_func_t)(fx_t *self);

/**
 * @struct fx_t
 * @brief Structure représentant un effet audio
 */
typedef struct __fx_t {
    void *state; /*!< Pointe vers les données internes de l'effet */

    fx_process_func_t process; /*!< Fonction d'application de l'effet */
    fx_destroy_func_t destroy; /*!< Fonction de nettoyage */
} fx_t;

#endif // FX_H