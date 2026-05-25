/**
 * @file mixer.h
 * @brief Mixeur audio
 * @author Lukas Grando
 * @version 2.0
 */
#include <stddef.h>
#include <stdbool.h>

#include "sound/engine/mixer.h"

/**
 * @brief Créer un mixeur audio
 * @param config la structure de configuration du mixeur audio
 */
mixer_t *mixer_create(mixer_config_t *config) {
    if(!config || config->numChannels == 0 || config->maxFramesPerBuffer == 0) return NULL;

    mixer_t *mixer = malloc(sizeof(mixer_t));
    CHECK_ALLOC(mixer);

    mixer->config = *config;
    mixer->channels = calloc(config->numChannels, sizeof(voice_t *));
    if(!mixer->channels) {
        free(mixer);
        return NULL;
    }
    mixer->buffer = calloc(config->maxFramesPerBuffer, sizeof(double));
    if(!mixer->buffer) {
        free(mixer->channels);
        free(mixer);
        return NULL;
    }

    return mixer;
}

/**
 * @brief Libère la mémoire allué au mixeur audio
 * @param mixer
 */
void mixer_destroy(mixer_t *mixer) {
    if(mixer) {
        if(mixer->channels) {
            for(size_t i = 0; i < mixer->config.numChannels; i++) {
                if (mixer->channels[i]) voice_destroy(mixer->channels[i]);
            }
            free(mixer->channels);
        }
        if(mixer->buffer) free(mixer->buffer);
        free(mixer);
    }
}

/**
 * @brief Assigne un instrument et joue une note sur un channel précis
 * @details Si le channel jouait déjà une note, elle est coupée immédiatement
 */
void mixer_play_on_channel(mixer_t *mixer, size_t channel_id, instrument_t *instrument, double freq) {
    if(!mixer || !instrument || channel_id >= mixer->config.numChannels) return;

    if(mixer->channels[channel_id] != NULL) {
        voice_destroy(mixer->channels[channel_id]);
        mixer->channels[channel_id] = NULL;
    }

	mixer->channels[channel_id] = voice_create(instrument);
    
    if(mixer->channels[channel_id]) {
        voice_note_on(mixer->channels[channel_id], freq);
    }
}
/**
 * @brief Relâche la note du channel
 */
void mixer_stop_channel(mixer_t *mixer, size_t channel_id) {
    if(!mixer || channel_id >= mixer->config.numChannels) return;
    if(mixer->channels[channel_id] != NULL) {
        voice_note_off(mixer->channels[channel_id]);
    }
}
/**
 * @brief Génère le son des N channels et les additionne
 */
void mixer_process(mixer_t *mixer, double *outBuffer, size_t frames) {
    if (!mixer || !outBuffer) return;
    size_t processFrames = (frames > mixer->config.maxFramesPerBuffer) ? mixer->config.maxFramesPerBuffer : frames;

    for (size_t i = 0; i < processFrames; i++) {
        outBuffer[i] = 0.0;
    }

    for (size_t c = 0; c < mixer->config.numChannels; c++) {
        voice_t *voice = mixer->channels[c];
        
        if (voice != NULL && !voice_is_done(voice)) {
            voice_process(voice, mixer->config.sampleRate, mixer->buffer, processFrames);

            for (size_t i = 0; i < processFrames; i++) {
                outBuffer[i] += mixer->buffer[i];
            }
        }
    }
    for (size_t i = 0; i < processFrames; i++) {
        outBuffer[i] *=  mixer->config.masterVolume;;
        outBuffer[i] = tanh(outBuffer[i]);
    }
}