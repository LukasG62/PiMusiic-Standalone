/**
 * @file fx_fuzz.c
 * @brief Implémentation de l'effet Fuzz
 * @author Lukas Grando
 * @version 1.0
 */

#include "sound/fx/fx_fuzz.h"

#include <stdlib.h>
#include <math.h>

static void fx_fuzz_process(fx_t *self, double *buffer, size_t size) {
    fx_fuzz_config_t *cfg = (fx_fuzz_config_t *) self->state;
    if (!cfg) return;

    double drive = (cfg->drive < 1.0) ? 1.0 : cfg->drive; 
    for (size_t i = 0; i < size; i++) {
        buffer[i] = tanh(buffer[i] * drive);
    }
}

static void fx_fuzz_destroy(fx_t *self) {
    if (self) free(self);
}

/**
 * @brief Crée un générateur d'effet fuzz
 * @param config structure de configuration du fuzz
 * @return fx_t* le générateur d'effet créé
 * @warning Le générateur doit être détruit à avec la méthode destroy
 */
fx_t *fx_fuzz_create(fx_fuzz_config_t *config) {
    if (!config) return NULL;

    fx_t *fx = malloc(sizeof(fx_t));
    CHECK_ALLOC(fx);
    
    fx->state = (void *)config; 
    fx->process = fx_fuzz_process;
    fx->destroy = fx_fuzz_destroy;

    return fx;
}