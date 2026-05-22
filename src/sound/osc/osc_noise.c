/**
 * @file osc_noise.c
 * @brief Oscillateur de bruit utilisant la méthode LFSR
 * @author Lukas Grando
 * @version 1.0
 */
#include "sound/osc/osc_noise.h"

#define LFSR_INITIAL_STATE 0xACE1U /*!< État initial du LFSR (non nul) */

#include <stdlib.h>
#include <stdint.h>

typedef struct {
	double currentPhase; /*!< La position actuelle dans le cycle de l'onde */
	double currentValue; /*!< La valeur actuelle du signal de bruit */
	uint16_t lfsr; /*!< État actuel du registre à décalage linéaire */
} osc_noise_state_t;

/**
 * @private
 * @fn static void osc_noise_lsfr(osc_noise_state_t *state)
 * @brief Met à jour le LFSR et calcule la nouvelle valeur de bruit
 * @param state La structure d'état de l'oscillateur de bruit
 */
static void osc_noise_lsfr(osc_noise_state_t *state) {
	uint16_t feedback = ((state->lfsr >> 0) ^ (state->lfsr >> 1) ^ (state->lfsr >> 3) ^ (state->lfsr >> 12)) & 1;
	state->lfsr = (state->lfsr >> 1) | (feedback << 15);
	state->currentValue = (state->lfsr & 1) ? 1.0 : -1.0;
}

static void osc_noise_generate(osc_t *self, double freq, double sampleRate, double *buffer, size_t size) {
	osc_noise_state_t *state = (osc_noise_state_t *)self->state;
	
	double phaseIncrement = freq / sampleRate;
	
	for (size_t i = 0; i < size; i++) {
		state->currentPhase += phaseIncrement;
		if (state->currentPhase >= 1.0) {
			state->currentPhase -= 1.0;
			osc_noise_lsfr(state);
		}
		buffer[i] = state->currentValue;
	}

}

static void osc_noise_destroy(osc_t *self) {
    if (self) {
        if (self->state) {
            free(self->state);
        }
        free(self);
    }
}

/**
 * @brief Crée un oscillateur de bruit
 * @return osc_t* l'oscillateur créé
 * @warning L'oscillateur doit être détruit avec sa méthode destroy pour libérer la mémoire
 */
osc_t *osc_noise_create(void) {
    osc_t *osc = malloc(sizeof(osc_t));
    if (!osc) return NULL;
    
    osc_noise_state_t *state = malloc(sizeof(osc_noise_state_t));
    if (!state) {
        free(osc);
        return NULL;
    }

    state->currentPhase = 0.0;
    state->currentValue = 0.0;
    
    state->lfsr = LFSR_INITIAL_STATE;

    osc->state = (void *)state;
    osc->generate = osc_noise_generate;
    osc->destroy = osc_noise_destroy;

    return osc;
}

/**
 * @brief Crée un oscillateur de bruit
 * @return osc_t* l'oscillateur créé
 * @warning L'oscillateur doit être détruit avec osc_destroy() pour libérer
 */
osc_t *osc_noise_create();
