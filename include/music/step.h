/**
 * @file step.h
 * @brief Définition de l'unité de base d'un channel
 * @details Un step représente une action à effectuer à un moment donné.
 * Historiquement un channel était une simple liste de notes, mais pour permettre plus de flexibilités
 * On créer un structure plus générique qui peut aussi contenir des commandes pour par exemple :
 * - Changer le BPM sur une portion d'un channel
 * - Créer des boucles sans devoir dupliquer les notes
 * @version 1.0
 */

#ifndef MUSIC_STEP_H
#define MUSIC_STEP_H

#include "music/note.h"

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Enumeration des durées des notes
*/
typedef enum {
    MUSIC_TIME_ZERO = 0,
    MUSIC_TIME_CROCHE_DOUBLE = 1, /*!< elle vaut ¼ temps*/
    MUSIC_TIME_CROCHE = 2, /*!< elle vaut ½ temps*/
    MUSIC_TIME_NOIRE = 4, /*!< elle vaut 1 temps*/
    MUSIC_TIME_BLANCHE = 8, /*!< elle vaut 2 temps*/
    MUSIC_TIME_RONDE = 16 /*!< elle vaut 4 temps*/
} music_time_duration_t;

/**
 * @brief Enumeration des types de steps
*/
typedef enum {
    MUSIC_STEP_TYPE_REST, /*!<  Ne fait rien, attend juste la durée définie */
    MUSIC_STEP_TYPE_NOTE, /*!< Joue une note */
    MUSIC_STEP_TYPE_COMMAND /*!< Instruction spéciale (Boucle, BPM...) */
} music_step_type_t;

/**
 * @brief Enumeration des types de commandes musicales
 */
typedef enum {
    MUSIC_CMD_LOOP_START, /*!< Début d'une boucle */
    MUSIC_CMD_LOOP_END, /*!< Fin d'une boucle */
    MUSIC_CMD_SET_BPM, /*!< Change le BPM à partir de ce step */  
	MUSIC_CMD_RESET_BPM, /*!< Remet le BPM par celui défini dans la musique */
    MUSIC_CMD_SET_VOLUME /*!< Change le volume à partir de ce step */
} music_command_type_t;

/**
 * @brief Structure pour représenter le début d'une boucle
 */
typedef struct {
    uint8_t id; /*!< Identifiant de la boucle */ 
} music_step_cmd_loop_start_t;

/**
 * @brief Structure pour représenter la fin d'une boucle
 */
typedef struct {
    uint8_t id; /*!< Identifiant de la boucle */
    uint16_t count; /*!< Nombre de répétitions restantes (0 pour une boucle infinie) */
} music_step_cmd_loop_end_t;

/**
 * @brief Structure pour représenter un changement de BPM
 */
typedef struct {
    uint16_t bpm; /*!< Nouveau BPM à appliquer à partir de ce step (cappé logiquement à 300) */
} music_step_cmd_bpm_t;

/**
 * @brief Structure pour représenter un changement de volume
 */
typedef struct {
    uint8_t volumePercent; /*!< Volume en pourcentage (0-100) */
} music_step_cmd_volume_t;

typedef struct {
    uint8_t noteId; /*!< Identifiant de la note */
    int8_t octave; /*!< Octave de la note */
    uint32_t instrumentId; /*!< Identifiant de l'instrument */
    uint8_t volumePercent; /*!< Volume en pourcentage (0-100) */
} music_step_note_t;

typedef struct {
    music_command_type_t type; /*!< Type de la commande */
    union {
        music_step_cmd_loop_start_t loopStart; 
        music_step_cmd_loop_end_t loopEnd;
        music_step_cmd_bpm_t bpm;
        music_step_cmd_volume_t volume;
    } param; /*!< Paramètres spécifiques à la commande */
} music_step_cmd_t;

typedef struct {
    music_step_type_t type; /*!< Type du step (note, rest, command) */
    music_time_duration_t duration; /*!< Durée du step (en valeur rythmique) */
    
    union {
        music_step_note_t note; /*!< Détails de la note à jouer (valide si type == MUSIC_STEP_TYPE_NOTE) */
        music_step_cmd_t cmd; /*!< Détails de la commande à exécuter (valide si type == MUSIC_STEP_TYPE_COMMAND) */
    } data;
} music_step_t;

/**
 * @brief Crée un step contenant une note
 * @param noteId Identifiant de la note
 * @param octave Octave de la note
 * @param instrumentId Identifiant de l'instrument
 * @param duration Durée du step
 * @return Le step créé
 */
music_step_t music_step_create_note(uint8_t noteId, int8_t octave, uint32_t instrumentId, music_time_duration_t duration);

/**
 * @brief Crée un step contenant une pause
 * @param duration Durée du step
 * @return Le step créé
 */
music_step_t music_step_create_rest(music_time_duration_t duration);

/**
 * @brief Crée un step contenant une commande de changement de BPM
 * @param newBpm Nouveau BPM à appliquer à partir de ce step
 * @param duration Durée du step (généralement on mettra une durée nulle pour les commandes, mais on laisse la possibilité de mettre une durée pour des effets de transition par exemple)
 * @return Le step créé
 */
music_step_t music_step_create_cmd_bpm(uint16_t newBpm, music_time_duration_t duration);

/**
 * @brief Crée un step contenant une commande de changement de volume
 * @param newVolume Nouveau volume en pourcentage (0-100)
 * @param duration Durée du step
 * @return Le step créé
 */
music_step_t music_step_create_cmd_volume(uint8_t newVolume, music_time_duration_t duration);

/**
 * @brief Crée un step contenant une commande de début de boucle
 * @param loopId Identifiant de la boucle (doit être unique pour chaque boucle imbriquée)
 * @param duration Durée du step
 * @return Le step créé
 */
music_step_t music_step_create_cmd_loop_start(uint8_t loopId, music_time_duration_t duration);

/**
 * @brief Crée un step contenant une commande de fin de boucle
 * @param loopId Identifiant de la boucle (doit correspondre à un loopId de début de boucle)
 * @param repeatCount Nombre de répétitions restantes (0 pour une boucle infinie)
 * @param duration Durée du step
 * @return Le step créé
 */
music_step_t music_step_create_cmd_loop_end(uint8_t loopId, uint16_t repeatCount, music_time_duration_t duration);

#endif // MUSIC_STEP_H