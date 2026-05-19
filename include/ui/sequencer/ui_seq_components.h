/**
 * @file ui_seq_components.h
 * @brief Fichier d'en-tête pour les composants modulaires du séquenceur
 * @version 1.0
 * @author Lukas Grando
 */
#ifndef UI_SEQ_COMPONENTS_H
#define UI_SEQ_COMPONENTS_H

#include "ui/base/ui_common.h"
#include "ui/base/ui_component.h"
#include "music/note.h"
 
#define SEQUENCER_INFO_LINES 6 /*!< Nombre de lignes pour la zone d'information du séquenceur */
#define SEQUENCER_INFO_COLS 53 /*!< Nombre de colonnes pour la zone d'information du séquenceur */
#define SEQUENCER_HELP_LINES 6 /*!< Nombre de lignes pour la zone d'aide du séquenceur */
#define SEQUENCER_HELP_COLS 53 /*!< Nombre de colonnes pour la zone d'aide du séquenceur */
#define SEQUENCER_CH_LINES 22  /*!< Nombre de lignes pour les channels du séquenceur */
#define SEQUENCER_CH_COLS 26  /*!< Nombre de colonnes pour les channels du séquenceur */

#define KEY_SEQ_NAV_UP KEY_UP /*!< Touche pour monter dans le séquenceur */
#define KEY_SEQ_NAV_DOWN KEY_DOWN /*!< Touche pour descendre dans le séquenceur */
#define KEY_SEQ_NAV_LEFT KEY_LEFT /*!< Touche pour aller à gauche dans le séquenceur */
#define KEY_SEQ_NAV_RIGHT KEY_RIGHT /*!< Touche pour aller à droite dans le séquenceur */
#define KEY_SEQ_NAV_CH1 '1' /*!< Touche pour aller au channel 1 */
#define KEY_SEQ_NAV_CH2 '2' /*!< Touche pour aller au channel 2 */
#define KEY_SEQ_NAV_CH3 '3' /*!< Touche pour aller au channel 3 */
#define KEY_SEQ_PLAY ' ' /*!< Touche pour jouer la musique */
#define KEY_SEQ_SAVE 's' /*!< Touche pour sauvegarder la musique */
#define KEY_SEQ_HELP 'h' /*!< Touche pour afficher l'aide */
#define KEY_SEQ_QUIT 27 /*!< Touche pour quitter le séquenceur */

#define NAVIGATION_MODE 0 /*!< Mode de navigation */
#define EDIT_MODE 1 /*!< Mode d'édition */

#define REVERSE_IF_COL(col, navcol, isSelected) (((col) == (navcol)) && (isSelected) ? A_REVERSE : 0) /*!< Inversion de la couleur si la colonne est sélectionnée */
#define REVERSE_IFNOT_PLAYMODE(playMode, isSelected) ((playMode && isSelected) ? 0 : A_REVERSE) /*!< Inversion de la couleur si le mode de lecture est activé */

/**
 * @enum ui_seq_nav_col_t
 * @brief Enumération des colonnes de navigation du séquenceur
 */
typedef enum {
    SEQUENCER_NAV_COL_LINE = 0, /*!< Colonne de l'identifiant de la ligne */
    SEQUENCER_NAV_COL_NOTE, /*!< Colonne de la note */
    SEQUENCER_NAV_COL_OCTAVE, /*!< Colonne de l'octave */
    SEQUENCER_NAV_COL_INSTRUMENT, /*!< Colonne de l'instrument */
    SEQUENCER_NAV_COL_TIME, /*!< Colonne du temps de la note */
    SEQUENCER_NAV_COL_MAX, /*!< Nombre de colonnes de navigation */
} ui_seq_nav_col_t;

/**
 * @enum ui_seq_nav_ch_t
 * @brief Enumération des channels de navigation du séquenceur
 */
typedef enum {
    SEQUENCER_NAV_CH1 = 0, /*!< Channel 1 */
    SEQUENCER_NAV_CH2, /*!< Channel 2 */
    SEQUENCER_NAV_CH3, /*!< Channel 3 */
    SEQUENCER_NAV_CH_MAX /*!< Nombre de channels de navigation */
} ui_seq_nav_ch_t;

/**
 * @struct ui_seq_color_pairs_t
 * @brief Enumération des paires de couleurs utilisées dans le séquenceur
 */
typedef enum {
    COLOR_PAIR_SEQ = 20, /*!< Plage de couleur du séquenceur */
    COLOR_PAIR_SEQ_NOTSAVED, /*!< Couleur pour indiquer que la musique n'est pas sauvegardée */
    COLOR_PAIR_SEQ_SAVED, /*!< Couleur pour indiquer que la musique est sauvegardée */
    COLOR_PAIR_SEQ_PLAYED, /*!< Couleur pour indiquer la ligne jouée */
    COLOR_PAIR_SEQ_OCTAVE, /*!< Couleur pour la colonne d'octave */
    COLOR_PAIR_SEQ_NOTE, /*!< Couleur pour la colonne de note */
    COLOR_PAIR_SEQ_INSTRUMENT, /*!< Couleur pour la colonne d'instrument */
    COLOR_PAIR_SEQ_SHIFT, /*!< Couleur pour la colonne de temps */
    COLOR_PAIR_SEQ_HEADER_INFO, /*!< Couleur pour l'entête d'information */
    COLOR_PAIR_SEQ_HEADER_TITLE, /*!< Couleur pour l'entête de titre */
    COLOR_PAIR_SEQ_WARNING, /*!< Couleur pour l'avertissement */
} ui_seq_color_pairs_t;

/**
 * @brief Structure de navigation interne du séquenceur
 */
typedef struct {
    ui_seq_nav_col_t col; /*!< Colonne de navigation actuelle */
    ui_seq_nav_ch_t ch; /*!< Channel de navigation actuel */
    int start[SEQUENCER_NAV_CH_MAX]; /*!< Position de départ pour chaque channel */
    int lines[SEQUENCER_NAV_CH_MAX]; /*!< Lignes sélectionnées pour chaque channel */
    int playMode;
} ui_seq_nav_t;


/**
 * @fn ui_seq_init_colors()
 * @brief Initialisation des couleurs du séquenceur
 * @details Cette fonction initialise les couleurs du séquenceur avec les paires de couleurs
 */
void ui_seq_init_colors();

/**
 * @fn ui_seq_init_nav(int playMode)
 * @brief Création de la structure de navigation du séquenceur
 * @param playMode Le mode de lecture (NAVIGATION_MODE ou EDIT_MODE)
 * @return ui_seq_nav_t 
 */
ui_seq_nav_t ui_seq_init_nav(int playMode);

/**
 * @fn void ui_seq_nav_up(ui_seq_nav_t *nav, int channelId)
 * @brief permet de passer d'une ligne à une autre dans le séquenceur
 * @param nav la structure de navigation
 * @param channelId L'id du channel
 */
void ui_seq_nav_up(ui_seq_nav_t *nav, int channelId);

/**
 * @fn ui_seq_nav_down()
 * @brief permet de passer d'une ligne à une autre dans le séquenceur
 * @param nav la structure de navigation
 * @param channelId L'id du channel
 */
void ui_seq_nav_down(ui_seq_nav_t *nav, int channelId);

/**
 * @fn ui_seq_nav_left(ui_seq_nav_t *nav)
 * @brief La fonction qui permet de passer d'une colonne à une autre
 * @param nav la structure de navigation
 */
void ui_seq_nav_left(ui_seq_nav_t *nav);

/**
 * @fn ui_seq_nav_right(ui_seq_nav_t *nav)
 * @brief La fonction qui permet de passer d'une colonne à une autre
 * @param nav la structure de navigation
 */
void ui_seq_nav_right(ui_seq_nav_t *nav);

/**
 * @fn ui_seq_change_sequencer_note(note_t *note, short col, scale_t scale, int isUp)
 * @brief Modification d'une note du séquenceur
 * @param note La note à modifier
 * @param col La colonne actuel
 * @param scale La gamme des notes
 * @param isUp La direction de la modification (0 pour le bas, 1 pour
 */
void ui_seq_change_sequencer_note(note_t *note, short col, scale_t scale, int isUp);

/**
 * @fn ui_seq_create_info_box(int x, int y, music_t *music, int *currentMode, bool *need2save)
 * @brief Création de la zone d'information du séquenceur
 * @param x La position x de la zone
 * @param y La position y de la zone
 * @param music La musique à afficher
 * @param currentMode Le mode actuel
 * @param need2save Un pointeur vers un booléen indiquant si la musique doit être sauvegardée
 * @return ui_component_t* Le composant de la zone d'information
 */
ui_component_t *ui_seq_create_info_box(int x, int y, music_t *music, int *currentMode, bool *need2save);

/**
 * @fn ui_seq_create_help_box(int x, int y)
 * @brief Création de la zone d'aide du séquenceur
 * @param x La position x de la zone
 * @param y La position y de la zone
 * @return ui_component_t* Le composant de la zone d'aide
 */
ui_component_t *ui_seq_create_help_box(int x, int y);

/**
 * @fn ui_seq_create_channel(int x, int y, int channelId, music_t *music, ui_seq_nav_t *nav, int *mode)
 * @brief Création d'un channel du séquenceur
 * @param x La position x du channel
 * @param y La position y du channel
 * @param channelId L'id du channel
 * @param music La musique à afficher
 * @param nav La structure de navigation
 * @param mode Le mode actuel
 * @return ui_component_t* Le composant du channel
 */
ui_component_t *ui_seq_create_channel(int x, int y, int channelId, music_t *music, ui_seq_nav_t *nav, int *mode);

#endif // UI_SEQ_COMPONENTS_H