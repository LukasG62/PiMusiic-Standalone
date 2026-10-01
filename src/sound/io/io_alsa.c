/**
 * @file io_alsa.c
 * @brief Gestionnaire de la carte son via ALSA et Thread Audio
 * @author Lukas Grando
 * @version 1.0
 */

#include "sound/io/io_alsa.h"

/**
 * @brief La boucle infinie qui tourne en tâche de fond
 */
static void *audio_thread_func(void *arg) {
    io_alsa_t *io = (io_alsa_t *)arg;
    size_t frames = io->frames_per_period;

    while (io->is_running) {
        mixer_process(io->mixer, io->double_buffer, frames);

        for (size_t i = 0; i < frames; i++) {
            double sample = io->double_buffer[i];
            if (sample > 1.0) sample = 1.0;
            if (sample < -1.0) sample = -1.0;
            
            io->pcm_buffer[i] = (int16_t)(sample * 32767.0);
        }
        int err = snd_pcm_writei(io->pcm_handle, io->pcm_buffer, frames);
        
        if(err == -EPIPE) {
            snd_pcm_prepare(io->pcm_handle);
        } else if(err < 0) {
        }
    }

    return NULL;
}

/**
 * @brief Initialise la carte son et prépare le thread audio
 * @param mixer Le mixeur qui va fournir le son
 * @param device_name Le nom de la carte
 * @return io_alsa_t* Le gestionnaire alloué
 */
io_alsa_t *io_alsa_create(mixer_t *mixer, const char *device_name) {
	if(!mixer) return NULL;

    io_alsa_t *io = malloc(sizeof(io_alsa_t));
    if(!io) return NULL;

    io->mixer = mixer;
    io->is_running = false;
    io->frames_per_period = mixer->config.maxFramesPerBuffer;

    io->double_buffer = calloc(io->frames_per_period, sizeof(double));
    io->pcm_buffer = calloc(io->frames_per_period, sizeof(int16_t));

    if(!io->double_buffer || !io->pcm_buffer) {
        io_alsa_destroy(io);
        return NULL;
    }

    int err;
    if((err = snd_pcm_open(&io->pcm_handle, device_name, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        io_alsa_destroy(io);
        return NULL;
    }

    unsigned int rate = (unsigned int)mixer->config.sampleRate;
    err = snd_pcm_set_params(io->pcm_handle,SND_PCM_FORMAT_S16_LE,SND_PCM_ACCESS_RW_INTERLEAVED,1,rate,1,20000); 

    if (err < 0) {
        io_alsa_destroy(io);
        return NULL;
    }

    return io;
}

/**
 * @brief Démarre le thread
 * @param io Le gestionnaire initialisé
 * @return true si le thread a bien démarré
 */
bool io_alsa_start(io_alsa_t *io) {
	if (!io || io->is_running) return false;
    io->is_running = true;
    
    if (pthread_create(&io->audio_thread, NULL, audio_thread_func, io) != 0) {
        io->is_running = false;
        return false;
    }
    return true;	
}

/**
 * @brief Arrête proprement le thread audio
 * @param io Le gestionnaire initialisé
 */
void io_alsa_stop(io_alsa_t *io) {
	if(!io || !io->is_running) return;

    io->is_running = false;
    pthread_join(io->audio_thread, NULL);	
}

/**
 * @brief Ferme la carte son et libère la mémoire
 * @param io Le gestionnaire initialisé
 */
void io_alsa_destroy(io_alsa_t *io) {
	if(io) {
        if(io->is_running) {
            io_alsa_stop(io);
        }

        if(io->pcm_handle) {
            snd_pcm_drain(io->pcm_handle);
            snd_pcm_close(io->pcm_handle);
        }

        if(io->double_buffer) free(io->double_buffer);
        if(io->pcm_buffer) free(io->pcm_buffer);

        free(io);
    }
}