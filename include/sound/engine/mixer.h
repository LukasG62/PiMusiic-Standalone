/**
 * @file mixer.h
 * @brief Mixeur audio
 * @author Lukas Grando
 * @version 2.0
 */
#ifndef MIXER_H
#define MIXER_H

#include <stddef.h>
#include <stdbool.h>
#include <math.h>

#include "common.h"
#include "sound/engine/voice.h"

/**
 * @struct mixer_config_t
 * @brief Configuration initiale du mixeur
 */
typedef struct {
    double sampleRate; /*!< Taux d'échantillonnage global */
    double masterVolume; /*!< Volume global*/
    size_t numChannels; /*!< Nombre de pistes indépendantes */
    size_t maxFramesPerBuffer; /*!< Taille max d'un buffer */
} mixer_config_t;

/**
 * @struct mixer_t
 * @brief Structure du mixeur audio
 */
typedef struct {
    mixer_config_t config;
    voice_t **channels; /*!< Tableau des canaux */
    double *buffer; /*!< Buffer temporaire pour le mixage */
} mixer_t;

/**
 * @brief Créer un mixeur audio
 * @param config la structure de configuration du mixeur audio
 */
mixer_t *mixer_create(mixer_config_t *config);

/**
 * @brief Libère la mémoire allué au mixeur audio
 * @param mixer
 */
void mixer_destroy(mixer_t *mixer);

/**
 * @brief Assigne un instrument et joue une note sur un channel précis
 * @details Si le channel jouait déjà une note, elle est coupée immédiatement
 */
void mixer_play_on_channel(mixer_t *mixer, size_t channel_id, instrument_t *instrument, double freq);

/**
 * @brief Relâche la note du channel
 */
void mixer_stop_channel(mixer_t *mixer, size_t channel_id);

/**
 * @brief Génère le son des N channels et les additionne
 */
void mixer_process(mixer_t *mixer, double *outBuffer, size_t frames);

#endif // MIXER_H