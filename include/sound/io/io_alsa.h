/**
 * @file alsa_backend.h
 * @brief Gestionnaire de la carte son via ALSA et Thread Audio
 * @author Lukas Grando
 * @version 1.0
 */
#ifndef ALSA_BACKEND_H
#define ALSA_BACKEND_H

#include <stdbool.h>
#include <alsa/asoundlib.h>

#include "sound/engine/mixer.h"

/**
 * @struct io_alsa_t
 * @brief Structure de gestion de l'interface ALSA et du thread audio
 */
typedef struct {
    snd_pcm_t *pcm_handle; /*!< Handle de la carte son ALSA */
    mixer_t *mixer; /*!< Mixeur audio*/

    pthread_t audio_thread; /*!< Thread audio */
    volatile bool is_running; /*!< Flag pour contrôler le thread audio */
    
	double *double_buffer; /*!< Buffer contenant les échantillons audio entre -1.0 et 1.0 */
    int16_t *pcm_buffer; /*!< Buffer final pour la carte son*/
    
	size_t frames_per_period; /*!< Taille d'un bloc audio */
} io_alsa_t;

/**
 * @brief Initialise la carte son et prépare le thread audio
 * @param mixer Le mixeur qui va fournir le son
 * @param device_name Le nom de la carte
 * @return io_alsa_t* Le gestionnaire alloué
 */
io_alsa_t *io_alsa_create(mixer_t *mixer, const char *device_name);

/**
 * @brief Démarre le thread
 * @param io Le gestionnaire initialisé
 * @return true si le thread a bien démarré
 */
bool io_alsa_start(io_alsa_t *io);

/**
 * @brief Arrête proprement le thread audio
 * @param io Le gestionnaire initialisé
 */
void io_alsa_stop(io_alsa_t *io);

/**
 * @brief Ferme la carte son et libère la mémoire
 * @param io Le gestionnaire initialisé
 */
void io_alsa_destroy(io_alsa_t *io);

#endif // ALSA_BACKEND_H