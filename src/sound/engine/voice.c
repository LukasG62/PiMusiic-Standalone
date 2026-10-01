/**
 * @file voice.c
 * @brief Gestion d'une voix
 * @author Lukas Grando
 * @version 1.0
 */

#include <stddef.h>
#include <stdbool.h>

#include "sound/engine/voice.h"

#include "sound/osc/osc_sin.h"
#include "sound/osc/osc_square.h"
#include "sound/osc/osc_sawtooth.h"
#include "sound/osc/osc_triangle.h"
#include "sound/osc/osc_resampler.h"
#include "sound/osc/osc_noise.h"

#include "sound/fx/fx_compressor.h"
#include "sound/fx/fx_fuzz.h"

/**
 * @brief Crée une voix en lisant l'instrument à utiliser
 * @param instrument L'instrument à jouer
 * @return voice_t* La voix prête à être déclenchée
 */
voice_t *voice_create(instrument_t *instrument) {
    if(!instrument) return NULL;

    voice_t *voice = malloc(sizeof(voice_t));
    if(!voice) return NULL;

    voice->instrument = instrument;
    voice->currentFreq = 0.0;
    voice->isActive = false;

    switch(instrument->oscType) {
		case OSC_TYPE_SIN: 
			voice->oscillator = osc_sin_create(); 
			break;
		case OSC_TYPE_SQUARE:
			voice->oscillator = osc_square_create(); 
			break;
		case OSC_TYPE_SAWTOOTH:
			voice->oscillator = osc_sawtooth_create();
			break;
		case OSC_TYPE_TRIANGLE:
			voice->oscillator = osc_triangle_create();
			break;
		case OSC_TYPE_NOISE:
			voice->oscillator = osc_noise_create();
			break;
		case OSC_TYPE_RESAMPLER:
            voice->oscillator = osc_resampler_create(&instrument->oscConfig.resampler);
            break;
        default:
            voice->oscillator = NULL;
            break;
    }

    voice->envelope = adsr_create(&instrument->adsrConfig);
    
	for(int i = 0; i < MAX_FX_PER_INSTRUMENT; i++) {
        voice->effects[i] = NULL;
    }

    for(int i = 0; i < instrument->fxCount; i++) {
        switch (instrument->fxTypes[i]) {
            case FX_TYPE_FUZZ:
                voice->effects[i] = fx_fuzz_create(&instrument->fxConfigs[i].fuzz);
                break;
            case FX_TYPE_COMPRESSOR:
                voice->effects[i] = fx_compressor_create(&instrument->fxConfigs[i].compressor);
                break;
            case FX_TYPE_NONE:
            default:
                break;
        }
    }

    return voice;
}

/**
 * @brief Fonction de nettoyage de la voix
 * @param voice La voix à détruire
 */
void voice_destroy(voice_t *voice) {
    if(voice) {
        if (voice->oscillator && voice->oscillator->destroy) {
            voice->oscillator->destroy(voice->oscillator);
        }

        if (voice->envelope) adsr_destroy(voice->envelope);

        for (int i = 0; i < MAX_FX_PER_INSTRUMENT; i++) {
            if (voice->effects[i] && voice->effects[i]->destroy) {
                voice->effects[i]->destroy(voice->effects[i]);
            }
        }
        free(voice);
    }
}

/**
 * @brief Déclenche la note
 * @param voice La voix à déclencher
 * @param freq La fréquence cible
 */
void voice_note_on(voice_t *voice, double freq) {
    if (!voice) return;
    
    voice->currentFreq = freq;
    voice->isActive = true;
    
    if (voice->envelope) adsr_note_on(voice->envelope);
}

/**
 * @brief Relâche la note 
 * @param voice La voix à relâcher
 */
void voice_note_off(voice_t *voice) {
    if(!voice) return;
    if(voice->envelope) adsr_note_off(voice->envelope);
}

/**
 * @brief Demande à la voix de générer son signal et d'appliquer ses effets
 * @param voice La voix
 * @param sampleRate Le taux d'échantillonnage global
 * @param buffer Le buffer de destination à remplir et traiter
 * @param size Le nombre d'échantillons à traiter
 */
void voice_process(voice_t *voice, double sampleRate, double *buffer, size_t size) {
    if (!voice || !voice->isActive) {
        for (size_t i = 0; i < size; i++) buffer[i] = 0.0;
        return;
    }

    if (voice->oscillator && voice->oscillator->generate) {
        voice->oscillator->generate(voice->oscillator, voice->currentFreq, sampleRate, buffer, size);
    } else {
        for (size_t i = 0; i < size; i++) buffer[i] = 0.0;
    }

    if (voice->envelope) adsr_process(voice->envelope, sampleRate, buffer, size);

    for (int i = 0; i < MAX_FX_PER_INSTRUMENT; i++) {
        if (voice->effects[i] && voice->effects[i]->process) {
            voice->effects[i]->process(voice->effects[i], buffer, size);
        }
    }
}

/**
 * @brief Vérifie si la voix a totalement terminé son cycle
 * @param voice La voix
 * @return true si la voix est terminée et peut être nettoyée, false sinon
 */
bool voice_is_done(voice_t *voice) {
    if (!voice || !voice->envelope) return true;
    
    if (voice->envelope->currentPhase == ADSR_PHASE_OFF) {
        voice->isActive = false;
        return true;
    }
    
    return false;
}