/**
 * @file osc_triangle.c
 * @brief Oscillateur d'onde triangulaire
 * @author Lukas Grando
 * @version 1.0
 */

#include "sound/osc/osc_triangle.h"

#include <math.h>
#include <stdlib.h>

/**
 * @struct osc_triangle_state_t
 * @brief Structure interne pour stocker l'état de l'oscillateur triangulaire
 */
typedef struct {
    double currentPhase; /*!< La position actuelle dans le cycle de l'onde */
} osc_triangle_state_t;


/**
 * @private
 * @fn static void osc_triangle_generate(osc_t *self, double freq, double sample_rate, double *buffer, size_t size)
 * @brief Génère un signal triangulaire dans le buffer
 * @param self La structure de l'oscillateur
 * @param freq La fréquence en Hz
 * @param sampleRate Le taux d'échantillonnage en Hz
 */
static void osc_triangle_generate(osc_t *self, double freq, double sampleRate, double *buffer, size_t size) {
	osc_triangle_state_t *state = (osc_triangle_state_t *)self->state;
    double phaseIncrement = (2.0 * M_PI * freq) / sampleRate;

    for (size_t i = 0; i < size; i++) {
        buffer[i] = 2.0 * fabs((state->currentPhase / M_PI) - 1.0) - 1.0;
        
        state->currentPhase += phaseIncrement;
        if (state->currentPhase >= 2.0 * M_PI) {
            state->currentPhase -= 2.0 * M_PI;
        }
    }
}

static void osc_triangle_destroy(osc_t *self) {
    if(self) {
        if(self->state) {
            free(self->state);
        }
        free(self); 
    }
}

/**
 * @brief Crée un oscillateur triangulaire
 * @return osc_t* l'oscillateur créé
 * @warning L'oscillateur doit être détruit avec osc_destroy() pour libérer
 */
osc_t *osc_triangle_create() {
    osc_t *osc = malloc(sizeof(osc_t));
    osc_triangle_state_t *state = malloc(sizeof(osc_triangle_state_t));
    
    state->currentPhase = 0.0;
    
    osc->state = (void *)state;
    osc->generate = osc_triangle_generate;
    osc->destroy = osc_triangle_destroy;
    
    return osc;
}

