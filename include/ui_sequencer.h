/**
 * @file ui_sequencer.h
 * @brief Fichier d'en-tête pour l'interface du séquenceur
 * @version 1.0
 * @author Lukas Grando
 */
#ifndef UI_SEQUENCER_H
#define UI_SEQUENCER_H

#include "ui_common.h" //!< Inclusion des macros communes à l'interface graphique
#include "note.h"     //!< Inclusion de la structure music_t

// Constantes pour le l'entête d'information du séquenceur
#define SEQUENCER_INFO_X0 0    /*!< Position X de l'entête d'information */
#define SEQUENCER_INFO_Y0 0    /*!< Position Y de l'entête d'information */
#define SEQUENCER_INFO_LINES 6 /*!< Largeur de l'entête d'information */
#define SEQUENCER_INFO_COLS 53 /*!< Hauteur de l'entête d'information */
// Constantes pour le l'entête d'aide du séquenceur
#define SEQUENCER_HELP_X0 53   /*!< Position X de l'entête d'aide */
#define SEQUENCER_HELP_Y0 0    /*!< Position Y de l'entête d'aide */
#define SEQUENCER_HELP_LINES 6 /*!< Largeur de l'entête d'aide */
#define SEQUENCER_HELP_COLS 53 /*!< Hauteur de l'entête d'aide */
// Constantes pour le body du séquenceur
#define SEQUENCER_BODY_X0 0     /*!< Position X du body */
#define SEQUENCER_BODY_Y0 6     /*!< Position Y du body */
#define SEQUENCER_BODY_LINES 24 /*!< Largeur du body */
#define SEQUENCER_BODY_COLS 106 /*!< Hauteur du body */
// Constantes pour les channels du séquenceur
#define SEQUENCER_CH1_X0 7    /*!< Position X du channel 1 */
#define SEQUENCER_CH2_X0 37   /*!< Position X du channel 2 */
#define SEQUENCER_CH3_X0 67   /*!< Position X du channel 3 */
#define SEQUENCER_CH_Y0 7     /*!< Position Y du channel 1, 2 et 3 */
#define SEQUENCER_CH_LINES 22 /*!< Largeur des channels */
#define SEQUENCER_CH_COLS 26  /*!< Hauteur des channels */

// Constante de logique d'affichage
#define REVERSE_IF_COL(col, navcol, isSelected) (((col) == (navcol)) && (isSelected) ? A_REVERSE : 0) /*!< Inversion de la couleur si la colonne est sélectionnée */
#define REVERSE_IFNOT_PLAYMODE(playMode, isSelected) ((playMode && isSelected) ? 0 : A_REVERSE) /*!< Inversion de la couleur si le mode de lecture est activé */

// Constantes de navigation (on en aura plus besoin dans le futur)
#define NAVIGATION_MODE 0 /*!< Mode de navigation */
#define EDIT_MODE 1       /*!< Mode d'édition */

// Constante pour les touches de navigation
#define KEY_SEQ_NAV_UP KEY_UP    /*!< Touche pour monter dans le séquenceur */
#define KEY_SEQ_NAV_DOWN KEY_DOWN /*!< Touche pour descendre dans le séquenceur */
#define KEY_SEQ_NAV_LEFT KEY_LEFT /*!< Touche pour aller à gauche dans le séquenceur */
#define KEY_SEQ_NAV_RIGHT KEY_RIGHT /*!< Touche pour aller à droite dans le séquenceur */
#define KEY_SEQ_NAV_CH1 '1'        /*!< Touche pour aller au channel 1 */
#define KEY_SEQ_NAV_CH2 '2'        /*!< Touche pour aller au channel 2 */
#define KEY_SEQ_NAV_CH3 '3'        /*!< Touche pour aller au channel 3 */
#define KEY_SEQ_PLAY ' '           /*!< Touche pour jouer la musique */
#define KEY_SEQ_SAVE 's'           /*!< Touche pour sauvegarder la musique */
#define KEY_SEQ_HELP 'h'           /*!< Touche pour afficher l'aide */
#define KEY_SEQ_QUIT 27        /*!< Touche pour quitter le séquenceur */



/**
 * \enum sequencer_color_pairs_t
 * \brief Enumération des paires de couleurs utilisées dans le séquenceur
 */
typedef enum {
    // Couleurs pour le séquenceur
    COLOR_PAIR_SEQ = 20,         /*!< Couleur du séquenceur */
    COLOR_PAIR_SEQ_NOTSAVED,     /*!< Couleur pour dire que la musique n'est pas sauvegardée */
    COLOR_PAIR_SEQ_SAVED,        /*!< Couleur pour dire que la musique est sauvegardée */
    COLOR_PAIR_SEQ_PLAYED,       /*!< Couleur de la ligne jouée */
    COLOR_PAIR_SEQ_OCTAVE,       /*!< Couleur de la ligne d'octave */
    COLOR_PAIR_SEQ_NOTE,         /*!< Couleur de la ligne de note */
    COLOR_PAIR_SEQ_INSTRUMENT,   /*!< Couleur de la ligne d'instrument */
    COLOR_PAIR_SEQ_SHIFT,        /*!< Couleur de la ligne de temps */
    COLOR_PAIR_SEQ_HEADER_INFO,  /*!< Couleur de l'entête d'information */
    COLOR_PAIR_SEQ_HEADER_TITLE, /*!< Couleur de l'entête de titre */
    COLOR_PAIR_SEQ_WARNING,      /*!< Couleur d'avertissement */
} sequencer_color_pairs_t;


/**
 * \enum sequencer_nav_col_t
 * \brief Enumération des colonnes du séquenceur
 * \details Cette énumération permet de définir les colonnes du séquenceur
 */
typedef enum
{
    SEQUENCER_NAV_COL_LINE = 0,   /*!< Ligne */
    SEQUENCER_NAV_COL_NOTE,       /*!< Colonne de note */
    SEQUENCER_NAV_COL_OCTAVE,     /*!< Colonne d'octave */
    SEQUENCER_NAV_COL_INSTRUMENT, /*!< Colonne d'instrument */
    SEQUENCER_NAV_COL_TIME,       /*!< Colonne de temps */
    SEQUENCER_NAV_COL_MAX,        /*!< Nombre de colonnes */
} sequencer_nav_col_t;

/**
 * \enum sequencer_nav_ch_t
 * \brief Enumération des channels du séquenceur
 * \details Cette énumération permet de définir les channels du séquenceur graphiquement
 */
typedef enum {
    SEQUENCER_NAV_CH1 = 0, /*!< Channel 1 */
    SEQUENCER_NAV_CH2,     /*!< Channel 2 */
    SEQUENCER_NAV_CH3,     /*!< Channel 3 */
    SEQUENCER_NAV_CH_MAX   /*!< Nombre de channels */
} sequencer_nav_ch_t;

/**
 * \struct sequencer_nav_t
 * \brief Structure de navigation dans le séquenceur
 * \details Cette structure permet de définir la position de navigation dans le séquenceur
 */
typedef struct
{
    sequencer_nav_col_t col;          /*!< Colonne */
    sequencer_nav_ch_t ch;            /*!< Channel */
    int start[SEQUENCER_NAV_CH_MAX]; /*!< Position de départ [col] */
    int lines[SEQUENCER_NAV_CH_MAX];   /*!< Ligne [ch] */
    //int line;
    int playMode;                    /*!< Mode de lecture */
} sequencer_nav_t;

/**
 * \struct channel_thread_args_t
 * \brief Structure des arguments pour les threads de channels
 * \details Cette structure permet de définir les arguments pour les threads de channels utilisé pour le playback
 */
typedef struct {
    sem_t *showSem;
    sem_t *syncSem;
    sem_t *finishSem;
    sequencer_nav_t *seqNav;
    music_t *music;
    int channel;
} channel_thread_args_t;

/**
 * void init_sequencer_colors()
 * \brief Initialisation des couleurs du séquenceur
 * \details Cette fonction initialise les couleurs du séquenceur en utilisant les paires de couleurs
 */
void init_sequencer_colors();


/**
 * @fn void show_sequencer(music_t *music, char *connected)
 * @brief La fonction qui affiche le séquenceur
 * @param music La musique à afficher
 * @param connected Le rfid de l'utilisateur connecté
 * @return choices_t 
 * @note l'utilisateur est considéré comme connecté si connected est différent de \0
 * @warning la musique et connected doit être allouée et initialisée
 */
app_choices_t show_sequencer(music_t *music, char *connected);

/**
 * \fn void show_sequencer_info(WINDOW *win, music_t *music, int mode, char need2save)
 * \brief Affichage des informations du séquenceur
 * \details Cette fonction affiche les informations du séquenceur
 * \param win La fenêtre où afficher les informations
 * \param music La musique à afficher
 * \param mode Le mode des boutons (0 pour le mode NAVIGATION, 1 pour le mode EDITION)
 * \param need2save Indication visuelle si la musique doit être sauvegardée
 */
void show_sequencer_info(WINDOW *win, music_t *music, int mode, char need2save);

/**
 * \fn void show_sequencer_help(WINDOW *win)
 * \brief Affichage de l'aide du séquenceur
 * \details Cette fonction affiche l'aide du séquenceur
 * \param win La fenêtre où afficher l'aide
 */
void show_sequencer_help(WINDOW *win);

/**
 * \fn void init_sequencer_channels(WINDOW **channels, music_t *music)
 * \brief Initialisation des channels du séquenceur
 * \details Cette fonction initialise les channels du séquenceur
 * \param channels Les fenêtres des channels
 * \param music La musique à afficher
 * \warning Il doit y avoir MUSIC_MAX_CHANNELS channels
*/
void init_sequencer_channels(WINDOW **channels, music_t *music);
/**
 * \fn void print_sequencer_note(WINDOW *win, note_t note, short ch, int line, sequencer_nav_t *seqNav, int isCurrLine)
 * \brief Affichage d'une ligne de notes du séquenceur
 * \details Cette fonction affiche une ligne de notes du séquenceur pour un unique channel
 * \param win La fenêtre où afficher les notes
 * \param note La note à afficher
 * \param ch L'identifiant du channel
 * \param line La ligne où afficher la note
 * \param seqNav La structure de navigation dans le séquenceur afin de pouvoir afficher un curseur
 * \param isSelected Si la ligne est la ligne selectionnée
 */
void print_sequencer_note(WINDOW *win, note_t note, short ch, int line, sequencer_nav_t *seqNav, int isSelected);

/**
 * \fn void print_sequencer_lines(WINDOW *win, short channelId, music_t *music, sequencer_nav_t *seqNav)
 * \brief Affichage des notes du séquenceur
 * \param win La fenêtre où afficher les notes
 * \param channelId L'identifiant du channel
 * \param seqNav La structure de navigation dans le séquenceur
 * \param music La musique à afficher
 * \see sequencer_nav_t
 */
void print_sequencer_lines(WINDOW *win, short channelId, music_t *music, sequencer_nav_t *seqNav);

/**
 * \fn void change_sequencer_note(note_t *note, short col, scale_t scale, int isUp)
 * \brief Modification d'une note du séquenceur en fonction de la colonne actuel et de la direction de la modification
 * \param note La note à modifier
 * \param col La colonne actuelle
 * \param scale La gamme des notes
 * \param isUp La direction de la modification (0 pour le bas, 1 pour le haut)
 * \see scale_t
*/
void change_sequencer_note(note_t *note, short col, scale_t scale, int isUp);

/**
 * @fn void sequencer_nav_up(sequencer_nav_t *nav)
 * @brief permet de passer d'une ligne à une autre dans le séquenceur
 * @param nav la structure de navigation
 */
void sequencer_nav_up(sequencer_nav_t *nav, int channelId);

/**
 * @fn void sequencer_nav_down(sequencer_nav_t *nav)
 * @brief permet de passer d'une ligne à une autre dans le séquenceur
 * @param nav la structure de navigation
 */
void sequencer_nav_down(sequencer_nav_t *nav, int channelId);

/**
 * @fn void sequencer_nav_left(sequencer_nav_t *nav)
 * @brief La fonction qui permet de passer d'une colonne à une autre dans le séquenceur (vers la gauche)
 * @param nav la structure de navigation
 */
void sequencer_nav_left(sequencer_nav_t *nav);

/**
 * @fn void sequencer_nav_right(sequencer_nav_t *nav)
 * @brief La fonction qui permet de passer d'une colonne à une autre dans le séquenceur (vers la droite)
 * @param nav 
 */
void sequencer_nav_right(sequencer_nav_t *nav);

/**
 * @fn create_sequencer_nav()
 * @brief Création de la structure de navigation du séquenceur
 * @return sequencer_nav_t 
 */
sequencer_nav_t create_sequencer_nav(int playMode);

/**
 * @brief create_channel_thread_args(sem_t *syncSem, sem_t *finishSem, sequencer_nav_t *seqNav, music_t *music, int channel)
 * @brief Crée les arguments pour le thread de channel
 * @return channel_thread_args_t 
 * @note Les arguments doivent être libérés après utilisation
 */
channel_thread_args_t *create_channel_thread_args(sem_t *showSem, sem_t *syncSem, sem_t *finishSem, sequencer_nav_t *seqNav, music_t *music, int channel);

#endif // UI_SEQUENCER_H