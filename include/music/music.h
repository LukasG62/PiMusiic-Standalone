/**
 * @file music.h
 * @brief Fichier d'en-tête des structures et fonctions de base pour la gestion de la musique (music.c)
 * @version 2.0
 */
#ifndef MUSIC_H
#define MUSIC_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include "music/channel.h"

#define MUSIC_MAX_CHANNELS 3
#define MUSIC_NAME_MAX_LENGTH 32

/**
 * @brief Structure pour représenter une musique
 */
typedef struct {
    uint32_t id; /*!< Identifiant unique de la musique (généré à la création) */                            
    char name[MUSIC_NAME_MAX_LENGTH]; /*!< Nom de la musique (pour l'affichage) */
    struct timeval date; /*!< Date de création ou de dernière modification */                    
    uint16_t baseBpm; /*!< BPM de base de la musique (peut être modifié en cours de lecture par des commandes) */

    music_channel_t channels[MUSIC_MAX_CHANNELS]; /*!< Tableau de channels composant la musique */
} music_t;


/**
 * @brief Initialise la musique (Nom par défaut, BPM, nettoie les channels)
 */
void music_init(music_t *music, uint16_t baseBpm, const char *name);

/**
 * @brief Raccourci global pour écrire un step sur un channel précis
 */
bool music_write_step(music_t *music, uint8_t channelId, size_t lineIndex, music_step_t step);

/**
 * @brief Raccourci pour lire un step (pour la vue Ncurses)
 */
music_step_t* music_get_step(music_t *music, uint8_t channelId, size_t lineIndex);

#endif // MUSIC_H