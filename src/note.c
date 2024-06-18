/**
 * @file note.c
 * @brief Fichier source pour la bibliothèque note.
 * @version 1.0
 * @author Tomas Salvado Robalo & Lukas Grando
 */

/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */
#include "note.h"


/* ------------------------------------------------------------------------ */
/*                  C O D E    D E S    F O N C T I O N S                   */
/* ------------------------------------------------------------------------ */
/**
 * \fn note_t create_note(short id, double frequency, short octave,instrument_t instrument, time_t time);
 * \brief Créé une note avec les param donnés
 * \param id Nom de la note
 * \param frequency Fréquence en Hz à l’octave de référence
 * \param octave octave de la note
 * \param instrument Istrument sur lequel la jouer
 * \param time Durée de la note
 */
note_t create_note(short id, double frequency, short octave,instrument_t instrument, time_duration_t time){
	note_t  maNote ;
	maNote.id = id;
	maNote.frequency = frequency ;
	maNote.octave = octave;
	maNote.instrument =instrument;
	maNote.time =time;
	return maNote;
}

/**
 * \fn void mod_note(note_t noteModif, short id, double frequency, short octave,instrument_t instrument, time_t time);
 * \brief modifie la note avec les param donnés
 * \param noteModif note à modifier
 * \param id numéro de la note dans la gamme
 * \param frequency Fréquence en Hz à l’octave de référence
 * \param octave octave de la note
 * \param instrument Istrument sur lequel la jouer
 * \param time Durée de la note
 */
note_t* mod_note(note_t * noteModif,short id, double frequency, short octave,instrument_t instrument, time_duration_t time){
	noteModif->id = noteModif->id;
	noteModif->frequency = frequency ;
	noteModif->octave = octave;
	noteModif->instrument =instrument;
	noteModif->time =time;
	return noteModif;
}


/**
 * \fn scale_t init_scale()
 * \brief Initialiser la gamme
 * \return la gamme initialisée
 * \details Cette fonction initialise la gamme avec le nom des notes
*/
scale_t init_scale() {
	// TODO : changer l'id par des constantes
	scale_t scale;

	// Initialisation de la gamme avec les fréquences
	scale.freqScale[0] = NOTE_NA_FQ;
	scale.freqScale[1] = NOTE_C_FQ;
	scale.freqScale[2] = NOTE_CS_FQ;
	scale.freqScale[3] = NOTE_D_FQ;
	scale.freqScale[4] = NOTE_DS_FQ;
	scale.freqScale[5] = NOTE_E_FQ;
	scale.freqScale[6] = NOTE_F_FQ;
	scale.freqScale[7] = NOTE_FS_FQ;
	scale.freqScale[8] = NOTE_G_FQ;
	scale.freqScale[9] = NOTE_GS_FQ;
	scale.freqScale[10] = NOTE_A_FQ;
	scale.freqScale[11] = NOTE_AS_FQ;
	scale.freqScale[12] = NOTE_B_FQ;
	return scale;
}

/**
 * \fn void get_next_note(note_t *note, scale_t *scale);
 * \brief récupérer la note suivante
 * \param note note de référence
 * \param scale la gamme
 * \return la note suivante
 * \note Elle met à jour la position dans la gamme
 */
void get_next_note(note_t *note, scale_t *scale) {
	note->id = (note->id + 1) % NB_NOTES;
	note->frequency = scale->freqScale[note->id];
}

/**
 * \fn void get_previous_note(note_t *note, scale_t *scale);
 * \brief récupérer la note précédente avec les paramètres donnés
 * \param note note de référence 
 * \param scale la gamme
 */
void get_previous_note(note_t *note, scale_t *scale) {
	note->id = ((note->id - 1) == -1) ? NB_NOTES - 1 : note->id - 1;
	note->frequency = scale->freqScale[note->id];
}

/**
 * \fn void get_note_freq(note_t *note, scale_t *scale);
 * \brief récupérer la fréquence de la note pointée par la position courante
 * \param note la note
 * \param scale la gamme
 * \return la fréquence de la note pointée par la position courante
 */ 
double get_note_freq(note_t *note, scale_t *scale) {
	return scale->freqScale[note->id];
}

/**
 * \fn note_t * cp_note(note_t * dest, note_t src);
 * \brief copier les données d'une note vers une autre
 * \param dest note qui va etre modifiée
 * \param src note qui va etre copiée 
 */
note_t * cp_note(note_t * dest, note_t src){
	dest->id = src.id;
	dest->frequency = src.frequency ;
	dest->octave = src.octave;
	dest->instrument =src.instrument;
	dest->time =src.time;
	return dest;
}

/**
 * \fn void init_channel(channel_t *channel);
 * \brief Initialiser un channel avec des notes vides
 * \param channel le channel à initialiser
 * \param id l'identifiant du channel
 * \see channel_t
 */
void init_channel(channel_t *channel, int id) {
	note_t note = create_note(0, NOTE_NA_FQ, REF_OCTAVE, INSTRUMENT_NA, TIME_NOIRE);
	int i;
	for (i = 0; i < CHANNEL_MAX_NOTES; i++) channel->notes[i] = note;
	channel->nbNotes = 0; // Aucune note non vide // TODO : voir si on peut sans passer
	channel->id  = id;
}

/**
 * \fn init_music(music_t *music, short bpm);
 * \brief Initialiser une musique avec des channels vides
 * \param music la musique à initialiser
 * \param bpm le bpm de la musique
*/
void init_music(music_t *music, short bpm) {
	int i;
	music->bpm = bpm;
	for (i = 0; i < MUSIC_MAX_CHANNELS; i++) init_channel(&music->channels[i], i);
}

/**
 * \fn void instrument2str(instrument_t instrument, char *str);
 * \brief Convertir un instrument en chaine de caractère
 * \param instrument l'instrument à convertir
 * \param str la chaine de caractère qui va contenir le nom de l'instrument
 */
void instrument2str(instrument_t instrument, char *str) {
	switch (instrument) {
		case INSTRUMENT_NA:
			strcpy(str, INSTRUMENT_NA_NAME);
			break;
		case INSTRUMENT_SIN:
			strcpy(str, INSTRUMENT_SIN_NAME);
			break;
		case INSTRUMENT_SAWTOOTH:
			strcpy(str, INSTRUMENT_SAWTOOTH_NAME);
			break;
		case INSTRUMENT_TRIANGLE:
			strcpy(str, INSTRUMENT_TRIANGLE_NAME);
			break;
		case INSTRUMENT_SQUARE:
			strcpy(str, INSTRUMENT_SQUARE_NAME);
			break;
		case INSTRUMENT_ORGAN:
			strcpy(str, INSTRUMENT_ORGAN_NAME);
			break;
		case INSTRUMENT_PIANO:
			strcpy(str, INSTRUMENT_PIANO_NAME);
			break;
		case INSTRUMENT_SINPHASER:
			strcpy(str, INSTRUMENT_SINPHASER_NAME);
			break;
		
		default:
			strcpy(str, INSTRUMENT_NA_NAME);
			break;
	}
}

/**
 * \fn note2str(note_t note, char *str);
 * \brief Convertir une note en chaine de caractère
 * \param note la note à convertir
 * \param scale la gamme
 * 
*/
void note2str(note_t note, char *str) {
	switch (note.id) {
		case 0:
			strcpy(str, NOTE_NA_NAME);
			break;
		case 1:
			strcpy(str, NOTE_C_NAME);
			break;
		case 2:
			strcpy(str, NOTE_CS_NAME);
			break;
		case 3:
			strcpy(str, NOTE_D_NAME);
			break;
		case 4:
			strcpy(str, NOTE_DS_NAME);
			break;
		case 5:
			strcpy(str, NOTE_E_NAME);
			break;
		case 6:
			strcpy(str, NOTE_F_NAME);
			break;
		case 7:
			strcpy(str, NOTE_FS_NAME);
			break;
		case 8:
			strcpy(str, NOTE_G_NAME);
			break;
		case 9:
			strcpy(str, NOTE_GS_NAME);
			break;
		case 10:
			strcpy(str, NOTE_A_NAME);
			break;
		case 11:
			strcpy(str, NOTE_AS_NAME);
			break;
		case 12:
			strcpy(str, NOTE_B_NAME);
			break;
		default:
			strcpy(str, NOTE_NA_NAME);
			break;
	}
}

/**
 * @fn update_channel_nbNotes(channel_t *channel, int noteIndex);
 * @brief Mettre à jour le nombre de notes dans un channel
 * @param channel le channel à mettre à jour
 * @param noteIndex l'index de la note courante
 * Ce script met à jour le nombre de notes dans un channel
 * Pour cela il compare la note passée en paramètre avec la dernière note du channel
 */
void update_channel_nbNotes(channel_t *channel, int noteIndex) {
	note_t currentNote = channel->notes[noteIndex];
	if(noteIndex >= channel->nbNotes) {
		if(currentNote.id != NOTE_NA_ID) channel->nbNotes = noteIndex + 1;
		return;
	} 
	if(noteIndex == channel->nbNotes - 1) {
		while(channel->notes[channel->nbNotes - 1].id == NOTE_NA_ID && channel->nbNotes > 0) {
			channel->nbNotes--;
		}
	}

	return;
}