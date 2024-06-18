/** 
 * @file note.h
 * @brief Définition des fonctions pour manipuler une note de musique
 * @version 1.0
 * @author Tomas Salvado Robalo & Lukas Grando
 */

#ifndef NOTE_H
#define NOTE_H
/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */
#include <string.h>
#include <sys/time.h>
/* ------------------------------------------------------------------------ */
/*              C O N S T A N T E S     S Y M B O L I Q U E S               */
/* ------------------------------------------------------------------------ */
#define CHANNEL_MAX_NOTES 4096 /*!< Nombre de notes maximum dans un channel doit tenir sur n symboles hexadécimaux */
#define MUSIC_MAX_CHANNELS 3 /*!< Nombre de channels maximum dans une musique */

//Fréquences des notes
#define REF_OCTAVE 3 /*!< Octave de référence */
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
#define NOTE_NA_NAME "--"/*!< Nom d'une non note */
#define NB_NOTES 13 /*!< Nombre de notes dans une octave en incluant la non note */

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

// Nom des instruments
#define INSTRUMENT_STEPMOTOR_NAME "STEP" /*!< Nom de l'instrument moteur pas à pas */
#define INSTRUMENT_SIN_NAME "SIN " /*!< Nom de l'instrument signal sinusoïdale */
#define INSTRUMENT_SAWTOOTH_NAME "SAW " /*!< Nom de l'instrument signal en dent de scie */
#define INSTRUMENT_TRIANGLE_NAME "TRI " /*!< Nom de l'instrument signal en triangle */
#define INSTRUMENT_SQUARE_NAME "SQR " /*!< Nom de l'instrument signal carré */
#define INSTRUMENT_ORGAN_NAME "ORGN" /*!< Nom de l'instrument orgue */
#define INSTRUMENT_PIANO_NAME "PIAN" /*!< Nom de l'instrument piano */
#define INSTRUMENT_SINPHASER_NAME "SPHS" /*!< Nom de l'instrument signal sinusoïdale avec phaser */
#define INSTRUMENT_NA_NAME " -- " /*!< Nom de l'instrument non disponible */

/* ------------------------------------------------------------------------ */
/*              D É F I N I T I O N S   D E   T Y P E S                     */
/* ------------------------------------------------------------------------ */

/**
 * \enum instrument_t
 * \brief Enumeration des instruments 
 *
 */
typedef enum {
	INSTRUMENT_NA=0, /*!< Pas d’instrument*/
	INSTRUMENT_SIN,/*!< Utilisation d’un signal sinusoïdale */
	INSTRUMENT_SAWTOOTH,/*!< Utilisation d’un signal en dent de scie */
	INSTRUMENT_TRIANGLE, /*!< Utilisation d’un signal en triangle */
	INSTRUMENT_SQUARE, /*!< Utilisation d’un signal carré */
	INSTRUMENT_ORGAN,  /*!< Utilisation d’un orgue */
	INSTRUMENT_PIANO,  /*!< Utilisation d’un piano */
	INSTRUMENT_SINPHASER, /*!< Utilisation d’un signal sinusoïdale avec phaser */
	INSTRUMENT_NB /*!< Nombre d’instruments disponibles */
}instrument_t;


/**
 * \enum time_duration_t
 * \brief Enumeration des durées des notes
 */
typedef enum {

	TIME_CROCHE_DOUBLE = 1,/*!< elle vaut ¼ temps*/
	TIME_CROCHE = 2,/*!< elle vaut ½ temps*/
	TIME_NOIRE = 4,/*!< Valeur par défaut, elle vaut 1 temps*/
	TIME_BLANCHE = 8,/*!< elle vaut 2 temps*/
	TIME_RONDE = 16,/*!< elle vaut 4 temps*/
	TIME_END = 10 /*!< Valeur de fin de la liste*/

}time_duration_t;

/**
 * \struct note_t
 * \brief Structure des notes
 */
typedef struct {
	short id; /*!< Identifiant de la note (position dans la gamme)*/
	double frequency; /*!< Fréquence en Hz à l’octave de référence*/
	short octave;/*!< octave de la note*/
	instrument_t instrument; /*!< Instrument sur lequel la jouer*/
	time_duration_t time;/*!<  Durée de la note */
}note_t;

/**
 * \struct scale_t
 * \brief Structure representant une gamme
*/
typedef struct {
	double freqScale[NB_NOTES];/*!< Fréquence en Hz à l’octave de référence*/
}scale_t;


/**
 * \struct channel_t
 * \brief Structure pour jouer les notes dans les channels
 */
typedef struct {
	short id ; /*!< Identifiant du channel*/ 
	note_t notes[CHANNEL_MAX_NOTES];/*!< Nombre de note (dernière note non vide)*/
	int nbNotes;/*!< Fréquence en Hz à l’octave de référence*/
}channel_t;

/**
 * \struct music_t
 * \brief Structure de la musique
 */
typedef struct {
	struct timeval date;/*!< Date de création de la musique*/
	channel_t channels [MUSIC_MAX_CHANNELS];/*!< Les canaux disponibles */
	short bpm;/*!< Le bpm de la musique*/
}music_t;


/* ------------------------------------------------------------------------ */
/*            P R O T O T Y P E S    D E    F O N C T I O N S               */
/* ------------------------------------------------------------------------ */
/**
 * \fn note_t create_note(short id, double frequency, short octave,instrument_t instrument, time_t time);
 * \brief Créé une note avec les param donnés
 * \param id identifiant de la note
 * \param frequency Fréquence en Hz à l’octave de référence
 * \param octave octave de la note
 * \param instrument Istrument sur lequel la jouer
 * \param time Durée de la note
 */
note_t create_note(short id, double frequency, short octave,instrument_t instrument, time_duration_t time);

/**
 * \fn note_t *mod_note(note_t *noteModif,char note[3], double frequency, short octave,instrument_t instrument, time_duration_t time);
 * \brief modifie la note avec les param donnés
 * \param noteModif note à modifier
 * \param id identifiant de la note
 * \param frequency Fréquence en Hz à l’octave de référence
 * \param octave octave de la note
 * \param instrument Istrument sur lequel la jouer
 * \param time Durée de la note
 */
note_t *mod_note(note_t *noteModif, short id, double frequency, short octave,instrument_t instrument, time_duration_t time);


/**
 * \fn scale_t init_scale()
 * \brief Initialiser la gamme
 * \return la gamme initialisée
 * \details Cette fonction initialise la gamme avec le nom des notes
*/
scale_t init_scale();

/**
 * \fn char* get_next_note(note_t *note, scale_t *scale);
 * \brief récupérer la note suivante
 * \param scale la gamme
 * \param note note de référence
 * \return la note suivante
 * \note Elle met à jour la position dans la gamme
 */
void get_next_note(note_t *note, scale_t *scale);

/**
 * \fn char* get_previous_note(note_t *note, scale_t *scale);
 * \brief récupérer la note précédente avec les paramètres donnés
 * \param note note de référence 
 */
void get_previous_note(note_t *note, scale_t *scale);

/**
 * \fn void get_note_freq(note_t *note, scale_t *scale);
 * \brief récupérer la fréquence de la note pointée par la position courante
 * \param scale la gamme
 * \return la fréquence de la note pointée par la position courante
 */ 
double get_note_freq(note_t *note, scale_t *scale);

/**
 * \fn note_t *cp_note(note_t *dest, note_t src);
 * \brief copier les données d'une note vers une autre
 * \param dest note qui va etre modifiée
 * \param src note qui va etre copiée 
 */
note_t *cp_note(note_t *dest, note_t src);

/**
 * \fn void init_channel(channel_t *channel);
 * \brief Initialiser un channel avec des notes vides
 * \param channel le channel à initialiser
 * \param id l'identifiant du channel
 * \see channel_t
 */
void init_channel(channel_t *channel, int id);

/**
 * \fn init_music(music_t *music, short bpm);
 * \brief Initialiser une musique avec des channels vides
 * \param music la musique à initialiser
 * \param bpm le bpm de la musique
*/
void init_music(music_t *music, short bpm);

/**
 * \fn void instrument2str(instrument_t instrument, char *str);
 * \brief Convertir un instrument en chaine de caractère
 * \param instrument l'instrument à convertir
 * \param str la chaine de caractère qui va contenir le nom de l'instrument
 */
void instrument2str(instrument_t instrument, char *str);

/**
 * \fn note2str(note_t note, char *str);
 * \brief Convertir une note en chaine de caractère
 * \param note la note à convertir
*/
void note2str(note_t note, char *str);

/**
 * @fn update_channel_nbNotes(channel_t *channel, int noteIndex);
 * @brief Mettre à jour le nombre de notes dans un channel
 * @param channel le channel à mettre à jour
 * @param noteIndex l'index de la note courante
 * Ce script met à jour le nombre de notes dans un channel
 * Pour cela il compare la note passée en paramètre avec la dernière note du channel
 */
void update_channel_nbNotes(channel_t *channel, int noteIndex);

#endif