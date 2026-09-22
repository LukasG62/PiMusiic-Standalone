/**
 * @file music.c
 * @brief Fichier d'implémentation des structures et fonctions de base pour la gestion de la musique (music.c)
 * @version 2.0
 */

#include "music/music.h"

#include <time.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

/**
 * @brief Initialise la musique (Nom par défaut, BPM, nettoie les channels)
 */
void music_init(music_t *music, uint16_t baseBpm, const char *name) {
	if (!music) return;
	music->id = (uint32_t)time(NULL);
	strncpy(music->name, name ? name : "Untitled", MUSIC_NAME_MAX_LENGTH - 1);
	music->name[MUSIC_NAME_MAX_LENGTH - 1] = '\0'; // Sécurité
	gettimeofday(&music->date, NULL);
	music->baseBpm = baseBpm > 0 ? baseBpm : 120; // BPM par défaut à 120 si invalide

	for (size_t i = 0; i < MUSIC_MAX_CHANNELS; i++) {
		music_channel_init(&music->channels[i], (uint8_t)i);
	}
}

/**
 * @brief Raccourci global pour écrire un step sur un channel précis
 */
bool music_write_step(music_t *music, uint8_t channelId, size_t lineIndex, music_step_t step) {
	if (!music || channelId >= MUSIC_MAX_CHANNELS) {
		return false;
	}
	return music_channel_write_step(&music->channels[channelId], lineIndex, step);
}

/**
 * @brief Raccourci pour lire un step (pour la vue Ncurses)
 */
music_step_t* music_get_step(music_t *music, uint8_t channelId, size_t lineIndex) {
	if (!music || channelId >= MUSIC_MAX_CHANNELS) {
		return NULL;
	}
	return music_channel_get_step(&music->channels[channelId], lineIndex);
}