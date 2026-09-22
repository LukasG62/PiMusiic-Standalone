/**
 * @file channel.c
 * @brief Implémentation d'un channel de musique
 * @details Un channel est une séquence de steps (notes, pauses, commandes) qui seront joués simultanément avec les autres channels. Un morceau de musique est composé de plusieurs channels.
 * @version 2.0
 */
#include "music/channel.h"


/**
 * @brief Initialise un channel (le remplit de steps vides et définit son id)
 * @param channel Pointeur vers le channel à initialiser
 * @param id Identifiant du channel
 */
void music_channel_init(music_channel_t *channel, uint8_t id) {
    if (!channel) return;
    channel->id = id;
    channel->nbSteps = 0;
    for (size_t i = 0; i < MUSIC_CHANNEL_MAX_STEPS; i++) {
        channel->steps[i] = (music_step_t) { .type = MUSIC_STEP_TYPE_REST, .duration = MUSIC_TIME_ZERO };
    }
}

/**
 * @brief Écrit un step à une ligne précise du tracker
 * @param channel Pointeur vers le channel
 * @param lineIndex Index de la ligne où écrire le step
 * @param step Step à écrire
 * @return true si l'index est valide
 */
bool music_channel_write_step(music_channel_t *channel, size_t lineIndex, music_step_t step) {
    if (!channel || lineIndex >= MUSIC_CHANNEL_MAX_STEPS) {
        return false;
    }
    channel->steps[lineIndex] = step;
    if (lineIndex >= channel->nbSteps) {
        channel->nbSteps = lineIndex + 1;
    }
    return true;
}

/**
 * @brief Récupère un pointeur vers un step (pour l'affichage ou la modification in-place)
 * @param channel Pointeur vers le channel
 * @param lineIndex Index de la ligne du step à récupérer
 * @return Pointeur vers le step (ou NULL si l'index est invalide)
 */
music_step_t* music_channel_get_step(music_channel_t *channel, size_t lineIndex) {
    if (!channel || lineIndex >= MUSIC_CHANNEL_MAX_STEPS) {
        return NULL;
    }
    return &channel->steps[lineIndex];
}

/**
 * @brief Efface un step spécifique (le remplace par un EMPTY step en gardant sa durée)
 */
void music_channel_delete_step(music_channel_t *channel, size_t lineIndex) {
    if (!channel || lineIndex >= MUSIC_CHANNEL_MAX_STEPS) {
        return;
    }
    channel->steps[lineIndex] = (music_step_t) { .type = MUSIC_STEP_TYPE_REST, .duration = channel->steps[lineIndex].duration };
}