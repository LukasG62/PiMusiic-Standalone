/**
 * @file note.c
 * @brief Fichier source pour la bibliothèque note.
 * @version 2.0
 * @author Tomas Salvado Robalo & Lukas Grando
 */

#include "common.h"
#include "music/note.h"

#include <string.h>
#include <math.h>

static const note_scale_t note_scale = {
    .freqScale = {
        [NOTE_NA_ID] = NOTE_NA_FQ,
        [NOTE_C_ID]  = NOTE_C_FQ,
        [NOTE_CS_ID] = NOTE_CS_FQ,
        [NOTE_D_ID]  = NOTE_D_FQ,
        [NOTE_DS_ID] = NOTE_DS_FQ,
        [NOTE_E_ID]  = NOTE_E_FQ,
        [NOTE_F_ID]  = NOTE_F_FQ,
        [NOTE_FS_ID] = NOTE_FS_FQ,
        [NOTE_G_ID]  = NOTE_G_FQ,
        [NOTE_GS_ID] = NOTE_GS_FQ,
        [NOTE_A_ID]  = NOTE_A_FQ,
        [NOTE_AS_ID] = NOTE_AS_FQ,
        [NOTE_B_ID]  = NOTE_B_FQ
    },
    .noteNames = {
        [NOTE_NA_ID] = NOTE_NA_NAME,
        [NOTE_C_ID]  = NOTE_C_NAME,
        [NOTE_CS_ID] = NOTE_CS_NAME,
        [NOTE_D_ID]  = NOTE_D_NAME,
        [NOTE_DS_ID] = NOTE_DS_NAME,
        [NOTE_E_ID]  = NOTE_E_NAME,
        [NOTE_F_ID]  = NOTE_F_NAME,
        [NOTE_FS_ID] = NOTE_FS_NAME,
        [NOTE_G_ID]  = NOTE_G_NAME,
        [NOTE_GS_ID] = NOTE_GS_NAME,
        [NOTE_A_ID]  = NOTE_A_NAME,
        [NOTE_AS_ID] = NOTE_AS_NAME,
        [NOTE_B_ID]  = NOTE_B_NAME
    }
};

/**
 * \fn note_scale_t note_init_scale()
 * \brief Initialiser la gamme
 * \return la gamme initialisée
 * \details Cette fonction initialise la gamme avec le nom des notes
*/
note_scale_t note_init_scale() {
    return note_scale;
}

/**
 * @brief Calcule la fréquence réelle d'une note selon son id et son octave
 * @param noteId L'identifiant de la note
 * @param octave L'octave désirée
 * @param scale La gamme de référence
 * @return La fréquence en Hz
 */
double note_get_frequency(note_id_t noteId, int8_t octave) {
	if(noteId == NOTE_NA_ID || noteId >= NOTE_COUNT) return 0.0;

	double baseFreq = note_scale.freqScale[noteId];
	int diff = octave - NOTE_OCTAVE_REF;

	if(diff > 0) {
		baseFreq *= (double)(1 << diff); 
	} else if (diff < 0) {
		baseFreq /= (double)(1 << (-diff)); 
	}

	return baseFreq;
}

/**
 * @brief Convertit une note et une octave en chaîne de 3 caractères (ex: "C-4", "D#4", "OFF")
 * @param noteId L'identifiant de la note
 * @param octave L'octave
 * @param str Le buffer de destination (doit faire au moins 4 octets)
 */
void note_to_string(note_id_t noteId, int8_t octave, char *str) {
	if(noteId == NOTE_NA_ID || noteId >= NOTE_COUNT) {
		strcpy(str, NOTE_NA_NAME);
		return;
	}
	snprintf(str, 4, "%s%d", note_scale.noteNames[noteId], octave);
}