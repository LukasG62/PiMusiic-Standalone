/** 
 * @file note.h
 * @brief Définition des fonctions pour manipuler une note de musique
 * @version 2.0
 * @author Tomas Salvado Robalo & Lukas Grando
 */

#ifndef NOTE_H
#define NOTE_H

#include <stdint.h>

//Fréquences des notes
#define NOTE_OCTAVE_REF 3 /*!< Octave de référence */
#define NOTE_C_FQ 261.63 /*!< Fréquence du DO à l’octave de référence */
#define NOTE_CS_FQ 277.18 /*!< Fréquence du DO# à l’octave de référence */
#define NOTE_D_FQ 293.66 /*!< Fréquence du RÉ à l’octave de référence */
#define NOTE_DS_FQ 311.13 /*!< Fréquence du RÉ# à l’octave de référence */
#define NOTE_E_FQ 329.63 /*!< Fréquence du MI à l’octave de référence */
#define NOTE_F_FQ 349.23 /*!< Fréquence du FA à l’octave de référence */
#define NOTE_FS_FQ 369.99 /*!< Fréquence du FA# à l’octave de référence */
#define NOTE_G_FQ 392.00 /*!< Fréquence du SOL à l’octave de référence */
#define NOTE_GS_FQ 415.30 /*!< Fréquence du SOL# à l’octave de référence */
#define NOTE_A_FQ 440.00 /*!< Fréquence du LA à l’octave de référence */
#define NOTE_AS_FQ 466.16 /*!< Fréquence du LA# à l’octave de référence */
#define NOTE_B_FQ 493.88 /*!< Fréquence du SI à l’octave de référence */
#define NOTE_NA_FQ 0 /*!< Fréquence d’une non note */

//Noms des notes
#define NOTE_C_NAME "C-" /*!< Nom du DO à l’octave de référence */
#define NOTE_CS_NAME "C#"/*!< Nom du DO# à l’octave de référence */
#define NOTE_D_NAME "D-"/*!< Nom du RÉ à l’octave de référence */
#define NOTE_DS_NAME "D#"/*!< Nom du RÉ# à l’octave de référence */
#define NOTE_E_NAME "E-"/*!< Nom du MI à l’octave de référence */
#define NOTE_F_NAME "F-"/*!< Nom du FA à l’octave de référence */
#define NOTE_FS_NAME "F#"/*!< Nom du FA# à l’octave de référence */
#define NOTE_G_NAME "G-"/*!< Nom du SOL à l’octave de référence */
#define NOTE_GS_NAME "G#"/*!< Nom du SOL# à l’octave de référence */
#define NOTE_A_NAME "A-"/*!< Nom du LA à l’octave de référence */
#define NOTE_AS_NAME "A#"/*!< Nom du LA# à l’octave de référence */
#define NOTE_B_NAME "B-"/*!< Nom du SI à l’octave de référence */
#define NOTE_NA_NAME "---"/*!< Nom d'une non note */
#define NOTE_COUNT 13 /*!< Nombre de notes dans une octave en incluant la non note */

/**
 * \enum note_id_t
 * \brief Enumeration des notes
*/
typedef enum {
	NOTE_NA_ID = 0, /*!< Pas de note*/
	NOTE_C_ID, /*!< Identifiant du DO à l’octave de référence */
	NOTE_CS_ID, /*!< Identifiant du DO# à l’octave de référence */
	NOTE_D_ID, /*!< Identifiant du RÉ à l’octave de référence */
	NOTE_DS_ID, /*!< Identifiant du RÉ# à l’octave de référence */
	NOTE_E_ID, /*!< Identifiant du MI à l’octave de référence */
	NOTE_F_ID, /*!< Identifiant du FA à l’octave de référence */
	NOTE_FS_ID, /*!< Identifiant du FA# à l’octave de référence */
	NOTE_G_ID, /*!< Identifiant du SOL à l’octave de référence */
	NOTE_GS_ID, /*!< Identifiant du SOL# à l’octave de référence */
	NOTE_A_ID, /*!< Identifiant du LA à l’octave de référence */
	NOTE_AS_ID, /*!< Identifiant du LA# à l’octave de référence */
	NOTE_B_ID /*!< Identifiant du SI à l’octave de référence */
} note_id_t;

/**
 * \struct note_scale_t
 * \brief Structure representant une gamme
*/
typedef struct {
	double freqScale[NOTE_COUNT];/*!< Fréquence en Hz à l’octave de référence*/
	char *noteNames[NOTE_COUNT];/*!< Noms des notes à l’octave de référence*/
} note_scale_t;

/**
 * \fn note_scale_t note_init_scale()
 * \brief Initialiser la gamme
 * \return la gamme initialisée
 * \details Cette fonction initialise la gamme avec le nom des notes
*/
note_scale_t note_init_scale();

/**
 * @brief Calcule la fréquence réelle d'une note selon son id et son octave
 * @param noteId L'identifiant de la note
 * @param octave L'octave désirée
 * @return La fréquence en Hz
 */
double note_get_frequency(note_id_t noteId, int8_t octave);

/**
 * @brief Convertit une note et une octave en chaîne de 3 caractères (ex: "C-4", "D#4", "OFF")
 * @param noteId L'identifiant de la note
 * @param octave L'octave
 * @param str Le buffer de destination (doit faire au moins 4 octets)
 */
void note_to_string(note_id_t noteId, int8_t octave, char *str);

#endif // NOTE_H