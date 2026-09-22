/**
 * @file channel.h
 * @brief Définition d'un channel de musique
 * @details Un channel est une séquence de steps (notes, pauses, commandes) qui seront joués simultanément avec les autres channels. Un morceau de musique est composé de plusieurs channels.
 * @version 2.0
 */
#ifndef MUSIC_CHANNEL_H
#define MUSIC_CHANNEL_H

#include "music/step.h"

#include <stddef.h>
#include <stdbool.h>

#define MUSIC_CHANNEL_MAX_STEPS 4096

/**
 * @brief Structure pour représenter un channel de musique
 */
typedef struct {
    uint8_t id; /*!< Identifiant du channel (0-255) */
    music_step_t steps[MUSIC_CHANNEL_MAX_STEPS]; /*!< Tableau de steps composant le channel */
    size_t nbSteps; /*!< Nombre de steps actuellement utilisés dans le channel (doit être inférieur ou égal à MUSIC_CHANNEL_MAX_STEPS) */
} music_channel_t;

/**
 * @brief Initialise un channel (le remplit de steps vides et définit son id)
 * @param channel Pointeur vers le channel à initialiser
 * @param id Identifiant du channel
 */
void music_channel_init(music_channel_t *channel, uint8_t id);

/**
 * @brief Écrit un step à une ligne précise du tracker
 * @param channel Pointeur vers le channel
 * @param lineIndex Index de la ligne où écrire le step
 * @param step Step à écrire
 * @return true si l'index est valide
 */
bool music_channel_write_step(music_channel_t *channel, size_t lineIndex, music_step_t step);

/**
 * @brief Récupère un pointeur vers un step (pour l'affichage ou la modification in-place)
 * @param channel Pointeur vers le channel
 * @param lineIndex Index de la ligne du step à récupérer
 * @return Pointeur vers le step (ou NULL si l'index est invalide)
 */
music_step_t* music_channel_get_step(music_channel_t *channel, size_t lineIndex);

/**
 * @brief Efface un step spécifique (le remplace par un EMPTY step en gardant sa durée)
 */
void music_channel_delete_step(music_channel_t *channel, size_t lineIndex);

#endif // MUSIC_CHANNEL_H