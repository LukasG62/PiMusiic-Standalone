/**
 * @file osc_resampler.c
 * @brief Oscillateur de sample utilisant la méthode de resampling linéaire
 * @author Lukas Grando
 * @version 1.0
 */

#include "sound/osc/osc_resampler.h"

#include <stdlib.h>

typedef struct {
	double currentIndex; /*!< Position actuelle dans le sample (en échantillons, peut être fractionnaire) */
	osc_resampler_config_t *config; /*!< Configuration de l'oscillateur de resampling */
} osc_resampler_state_t;

/**
 * @private static inline bool osc_resampler_is_playing(const osc_resampler_state_t *state, const osc_resampler_config_t *cfg) {
 * @brief Vérifie si l'oscillateur de resampling est encore en train de jouer
 * @param state État de l'oscillateur de resampling
 * @param cfg Configuration de l'oscillateur de resampling
 * @return true si l'oscillateur est en train de jouer, false sinon
 */
static inline bool osc_resampler_is_playing(const osc_resampler_state_t *state, const osc_resampler_config_t *cfg) {
    if (!cfg->loopEnabled && state->currentIndex >= cfg->sampleLength - 1) {
        return false;
    }
    return true;
}

/**
 * @private static inline void osc_resampler_wrap_index(osc_resampler_state_t *state, const osc_resampler_config_t *cfg) {
 * @brief Gère le wrapping de l'index de lecture en fonction de la configuration de la boucle
 * @param state État de l'oscillateur de resampling
 * @param cfg Configuration de l'oscillateur de resampling
 */
static inline void osc_resampler_wrap_index(osc_resampler_state_t *state, const osc_resampler_config_t *cfg) {
    if (cfg->loopEnabled && state->currentIndex >= cfg->loopEnd) {
        state->currentIndex -= (cfg->loopEnd - cfg->loopStart);
    }
}

/**
 * @private static inline double osc_resampler_read_interpolated(const osc_resampler_config_t *cfg, double currentIndex) {
 * @brief lit la valeur du sample à la position actuelle avec interpolation linéaire
 */
static inline double osc_resampler_read_interpolated(const osc_resampler_config_t *cfg, double currentIndex) {
    size_t idx1 = (size_t)currentIndex;
    size_t idx2 = idx1 + 1;

    if (idx2 >= cfg->sampleLength) {
        idx2 = cfg->loopEnabled ? cfg->loopStart : idx1;
    }

    double frac = currentIndex - (double)idx1;
    
    double val1 = cfg->sampleData[idx1] / NORMALIZATION_FACTOR_16B;
    double val2 = cfg->sampleData[idx2] / NORMALIZATION_FACTOR_16B;

    return val1 + frac * (val2 - val1);
}

static void osc_resampler_generate(osc_t *self, double freq, double sampleRate, double *buffer, size_t size) {
    osc_resampler_state_t *state = (osc_resampler_state_t *)self->state;
    osc_resampler_config_t *cfg = state->config;
    
    if (!cfg || !cfg->sampleData || cfg->baseFrequency <= 0.0) {
        for (size_t i = 0; i < size; i++) buffer[i] = 0.0;
        return;
    }

    double phaseIncrement = freq / cfg->baseFrequency;
    
    for (size_t i = 0; i < size; i++) {
        if (!osc_resampler_is_playing(state, cfg)) {
            buffer[i] = 0.0;
            continue;
        }
        osc_resampler_wrap_index(state, cfg);

        buffer[i] = osc_resampler_read_interpolated(cfg, state->currentIndex);
        state->currentIndex += phaseIncrement;
    }
}

/**
 * @private
 * @fn static void osc_resampler_destroy(osc_t *self)
 * @brief Libère la mémoire allouée pour l'oscillateur de resampling
 * @param self L'oscillateur à détruire
 */
static void osc_resampler_destroy(osc_t *self) {
    if(self) {
        if(self->state) {
            free(self->state);
        }
        free(self); 
    }
}

/**
 * @brief Crée un oscillateur de sample avec resampling linéaire
 * @param config Configuration de l'oscillateur (doit être pré-remplie)
 * @return osc_t* l'oscillateur créé
 * @warning L'oscillateur doit être détruit avec sa méthode destroy pour libérer la mémoire
 */
osc_t *osc_resampler_create(osc_resampler_config_t *config) {
    osc_t *osc = (osc_t *)malloc(sizeof(osc_t));
    CHECK_ALLOC(osc);
    
    osc_resampler_state_t *state = (osc_resampler_state_t *)malloc(sizeof(osc_resampler_state_t));
    CHECK_ALLOC(state);
    
    state->currentIndex = 0.0;
    state->config = config;

    osc->state = (void *)state;
    osc->generate = osc_resampler_generate;
    osc->destroy = osc_resampler_destroy;
    
    return osc;
}