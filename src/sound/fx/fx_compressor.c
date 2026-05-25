/**
 * @file fx_fuzz.c
 * @brief Implémentation de l'effet compresseur avec suiveur d'enveloppe
 * @author Lukas Grando
 * @version 1.0
 */
#include "sound/fx/fx_compressor.h"
#include <stdlib.h>
#include <math.h>


typedef struct {
    fx_compressor_config_t *config;
    double currentEnvelope;
} fx_compressor_state_t;

/**
 * @brief Calcule le coefficient de lissage
 */
static inline double calculate_coeff(double timeInSeconds, double sampleRate) {
    if (timeInSeconds <= 0.0) return 0.0;
    return exp(-1.0 / (timeInSeconds * sampleRate));
}


static void fx_compressor_process(fx_t *self, double *buffer, size_t size) {
    fx_compressor_state_t *state = (fx_compressor_state_t *)self->state;
    fx_compressor_config_t *cfg = state->config;

    if (!cfg) return;

    double attackCoeff = calculate_coeff(cfg->attackTime, cfg->sampleRate);
    double releaseCoeff = calculate_coeff(cfg->releaseTime, cfg->sampleRate);

    for (size_t i = 0; i < size; i++) {
        double absInput = fabs(buffer[i]);

        if (absInput > state->currentEnvelope) {
            state->currentEnvelope = attackCoeff * state->currentEnvelope + (1.0 - attackCoeff) * absInput;
        } else {
            state->currentEnvelope = releaseCoeff * state->currentEnvelope + (1.0 - releaseCoeff) * absInput;
        }

        double gain = 1.0;
        
        if (state->currentEnvelope > cfg->threshold) {
            double overshoot = state->currentEnvelope - cfg->threshold;
        	double targetEnvelope = cfg->threshold + (overshoot / cfg->ratio);
            gain = targetEnvelope / state->currentEnvelope;
        }
        buffer[i] = buffer[i] * gain * cfg->makeupGain;
    }
}

static void fx_compressor_destroy(fx_t *self) {
    if (self) {
        if (self->state) {
            free(self->state);
        }
        free(self);
    }
}

/**
 * @brief Crée une instance du compresseur
 * @param config pointeur vers la configuration
 * @param sampleRate Le taux d'échantillonnage
 * @return fx_t* L'effet créé
 */
fx_t *fx_compressor_create(fx_compressor_config_t *config) {
    if (!config || config->sampleRate <= 0.0) return NULL;

    fx_t *fx = malloc(sizeof(fx_t));
    CHECK_ALLOC(fx);
    
    fx_compressor_state_t *state = malloc(sizeof(fx_compressor_state_t));
    if (!state) {
        free(fx);
        return NULL;
    }

    state->config = config;
    state->currentEnvelope = 0.0;

    fx->state = (void *)state;
    fx->process = fx_compressor_process;
    fx->destroy = fx_compressor_destroy;

    return fx;
}