/**
 * @file adsr.c
 * @brief Enveloppe ADSR
 * @author Lukas Grando
 * @version 1.0
 */
#include "sound/env/adsr.h"


/**
 * @brief Crée une instance d'ADSR avec la configuration donnée
 * @param config La configuration de l'enveloppe (doit être pré-remplie
 * @return adsr_t* L'instance créée
 */
adsr_t *adsr_create(adsr_config_t *config) {
	if(!config) return NULL;
	adsr_t *adsr = malloc(sizeof(adsr_t));
	CHECK_ALLOC(adsr);
	
	adsr->config = config;
	adsr->currentPhase = ADSR_PHASE_OFF;
	adsr->currentVolume = 0.0;
	return adsr;
}

/**
 * @brief Libère la mémoire allouée pour l'instance de l'ADSR
 * @param self L'instance de l'ADSR à détruire
 * @warning Ne libère pas la configuration pointée par self->config (gérée par ailleurs)
 */
void adsr_destroy(adsr_t *self) {
	if(!self) return;
	free(self);
}

/**
 * @brief Déclenche le début d'une note
 * @param self L'instance de l'ADSR
 */
void adsr_note_on(adsr_t *self) {
	if (!self) return;

	if(self->config->enabled) {
		self->currentPhase = ADSR_PHASE_ATTACK;
	} else {
		self->currentPhase = ADSR_PHASE_SUSTAIN;
		self->currentVolume = ADSR_DISABLE_VOLUME;
	}
}

/**
 * @brief Déclenche la fin d'une note
 * @param self L'instance de l'ADSR
 */
void adsr_note_off(adsr_t *self) {
	if (!self) return;

	if(self->config->enabled) {
		self->currentPhase = ADSR_PHASE_RELEASE;
	} else {
		self->currentPhase = ADSR_PHASE_OFF;
		self->currentVolume = 0.0;
	}
}

/**
 * @brief Applique l'enveloppe de volume au buffer audio in-place
 * @param self L'instance de l'ADSR
 * @param sampleRate Taux d'échantillonnage
 * @param buffer Le buffer généré par l'oscillateur
 * @param size Nombre de samples dans le buffer
 */
void adsr_process(adsr_t *self, double sampleRate, double *buffer, size_t size) {
	if(!self) return;

    if(!self->config->enabled) {
        for(size_t i = 0; i < size; i++) {
            buffer[i] *= self->currentVolume; 
        }
        return;
    }

	adsr_config_t *cfg = self->config;
	for(size_t i = 0; i < size; i++) {
		switch (self->currentPhase) {
			case ADSR_PHASE_OFF:
				self->currentVolume = 0.0;
				break;

			case ADSR_PHASE_ATTACK:
				if(cfg->attackTime <= 0.0) self->currentVolume = 1.0;
				else self->currentVolume += (1.0 / (cfg->attackTime * sampleRate));

				if(self->currentVolume >= 1.0) {
					self->currentVolume = 1.0;
					self->currentPhase = ADSR_PHASE_DECAY;
				}
				break;
			case ADSR_PHASE_DECAY:
				if (cfg->decayTime <= 0.0) self->currentVolume = cfg->sustainLevel;
				else self->currentVolume -= (1.0 - cfg->sustainLevel) / (cfg->decayTime * sampleRate);
				
				if (self->currentVolume <= cfg->sustainLevel) {
					self->currentVolume = cfg->sustainLevel;
					self->currentPhase = ADSR_PHASE_SUSTAIN;
				}
				break;

			case ADSR_PHASE_SUSTAIN:
				// Le volume reste à sustainLevel, on attend juste que note_off soit appelé
				self->currentVolume = cfg->sustainLevel;
				break;

			case ADSR_PHASE_RELEASE:
				if (cfg->releaseTime <= 0.0) self->currentVolume = 0.0;
				else self->currentVolume -= 1.0 / (cfg->releaseTime * sampleRate);
				
				if (self->currentVolume <= 0.0) {
					self->currentVolume = 0.0;
					self->currentPhase = ADSR_PHASE_OFF;
				}
				break;
		}
		buffer[i] *= self->currentVolume;
	}
}