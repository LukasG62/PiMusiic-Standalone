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

#include "music/step.h"

#include <stdint.h>
#include <stdbool.h>


/**
 * @brief Crée un step contenant une note
 * @param noteId Identifiant de la note
 * @param octave Octave de la note
 * @param instrumentId Identifiant de l'instrument
 * @param duration Durée du step
 * @return Le step créé
 */
music_step_t music_step_create_note(uint8_t noteId, int8_t octave, uint32_t instrumentId, music_time_duration_t duration) {
	music_step_t step;
	step.type = MUSIC_STEP_TYPE_NOTE;
	step.duration = duration;
	step.data.note.noteId = noteId;
	step.data.note.octave = octave;
	step.data.note.instrumentId = instrumentId;
	step.data.note.volumePercent = 100;
	return step;
}

/**
 * @brief Crée un step contenant une pause
 * @param duration Durée du step
 * @return Le step créé
 */
music_step_t music_step_create_rest(music_time_duration_t duration) {
	music_step_t step;
	step.type = MUSIC_STEP_TYPE_REST;
	step.duration = duration;
	return step;
}

/**
 * @brief Crée un step contenant une commande de changement de BPM
 * @param newBpm Nouveau BPM à appliquer à partir de ce step
 * @param duration Durée du step (généralement on mettra une durée nulle pour les commandes, mais on laisse la possibilité de mettre une durée pour des effets de transition par exemple)
 * @return Le step créé
 */
music_step_t music_step_create_cmd_bpm(uint16_t newBpm, music_time_duration_t duration) {
	music_step_t step;
	step.type = MUSIC_STEP_TYPE_COMMAND;
	step.duration = duration;
	step.data.cmd.type = MUSIC_CMD_SET_BPM;
	step.data.cmd.param.bpm.bpm = newBpm;
	return step;
}

/**
 * @brief Crée un step contenant une commande de changement de volume
 * @param newVolume Nouveau volume en pourcentage (0-100)
 * @param duration Durée du step
 * @return Le step créé
 */
music_step_t music_step_create_cmd_volume(uint8_t newVolume, music_time_duration_t duration) {
	music_step_t step;
	step.type = MUSIC_STEP_TYPE_COMMAND;
	step.duration = duration;
	step.data.cmd.type = MUSIC_CMD_SET_VOLUME;
	step.data.cmd.param.volume.volumePercent = newVolume;
	return step;
}

/**
 * @brief Crée un step contenant une commande de début de boucle
 * @param loopId Identifiant de la boucle (doit être unique pour chaque boucle imbriquée)
 * @param duration Durée du step
 * @return Le step créé
 */
music_step_t music_step_create_cmd_loop_start(uint8_t loopId, music_time_duration_t duration) {
	music_step_t step;
	step.type = MUSIC_STEP_TYPE_COMMAND;
	step.duration = duration;
	step.data.cmd.type = MUSIC_CMD_LOOP_START;
	step.data.cmd.param.loopStart.id = loopId;
	return step;
}

/**
 * @brief Crée un step contenant une commande de fin de boucle
 * @param loopId Identifiant de la boucle (doit correspondre à un loopId de début de boucle)
 * @param repeatCount Nombre de répétitions restantes (0 pour une boucle infinie)
 * @param duration Durée du step
 * @return Le step créé
 */
music_step_t music_step_create_cmd_loop_end(uint8_t loopId, uint16_t repeatCount, music_time_duration_t duration) {
	music_step_t step;
	step.type = MUSIC_STEP_TYPE_COMMAND;
	step.duration = duration;
	step.data.cmd.type = MUSIC_CMD_LOOP_END;
	step.data.cmd.param.loopEnd.id = loopId;
	step.data.cmd.param.loopEnd.count = repeatCount;
	return step;
}