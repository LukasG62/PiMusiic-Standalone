/**
 * \file uiManager.h
 * \details Contiens les fonctions de toutes les interfaces de l'application
 * \version 1.0
 * \author Lukas Grando
 */

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <ncurses.h>
#include <string.h>
#include "note.h"
#include "mpp.h"
#include "request.h"
#include "mysyscall.h"
#include "sound.h"
#include <time.h>   

#define RPI_COLS 106 /*!< Nombre de colonnes de la fenêtre sur le RPI */
#define RPI_LINES 29 /*!< Nombre de lignes de la fenêtre sur le RPI */
#define MAX_MENU_ITEMS 18 /*!< Nombre maximum d'items dans un menu */

#define REVERSE_IF_COL(col, navcol, isSelected) (((col) == (navcol)) && (isSelected) ? A_REVERSE : 0) /*!< Inversion de la couleur si la colonne est sélectionnée */
#define REVERSE_IFNOT_PLAYMODE(playMode, isSelected) ((playMode && isSelected) ? 0 : A_REVERSE) /*!< Inversion de la couleur si le mode de lecture est activé */

#define NAVIGATION_MODE 0 /*!< Mode de navigation */
#define EDIT_MODE 1       /*!< Mode d'édition */
// X : Colonne Y : Ligne

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

#define SEQUENCER_CH1_X0 7    /*!< Position X du channel 1 */
#define SEQUENCER_CH2_X0 37   /*!< Position X du channel 2 */
#define SEQUENCER_CH3_X0 67   /*!< Position X du channel 3 */
#define SEQUENCER_CH_Y0 7     /*!< Position Y du channel 1, 2 et 3 */
#define SEQUENCER_CH_LINES 22 /*!< Largeur des channels */
#define SEQUENCER_CH_COLS 26  /*!< Hauteur des channels */

#define KEY_BUTTON_CHANGEMODE 't'
#define KEY_BUTTON_CH3NPLAY 'r'
#define KEY_BUTTON_CH2NQUIT 'e'
#define KEY_BUTTON_CH1NSAVE 'z'
#define KEY_BUTTON_LINEDOWN 'c'
#define KEY_BUTTON_LINEUP 'v'



/**
 * \enum choices_t
 * \brief Enumération des choix possibles dans les menus
 * \details Cette énumération permet de définir les choix possibles dans les menus
 */
typedef enum
{
    CHOICE_QUITAPP,        /*!< Quitter l'application */
    CHOICE_HELP,           /*!< Afficher l'aide */
    CHOICE_CONECTION_MENU, /*!< Aller au menu de connexion */
    CHOICE_MENU_LIST,      /*!< Aller au menu de listes */
    CHOICE_SEQUENCER,      /*!< Aller au séquenceur */
    CHOICE_SAVENQUIT,      /*!< Sauvegarder et quitter */
    CHOICE_CREATEMUSIC,    /*!< Créer une musique */
    CHOICE_MAIN_MENU,      /*!< Retourner au menu principal */
} choices_t;

/**
 * \enum color_pairs_t
 * \brief Enumération des paires de couleurs utilisées dans l'application
 */
typedef enum
{
    // Couleurs pour les menus
    COLOR_PAIR_MENU = 10,    /*!< Couleur du menu */
    COLOR_PAIR_MENU_WARNING, /*!< Couleur du menu en cas d'erreur */
    COLOR_PAIR_MENU_PROMPT,  /*!< Couleur du menu d'entrée utilisateur */

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
} color_pairs_t;

/**
 * @enum custom_colors_t
 * @brief Couleurs personnalisées qui n'existent pas déjà dans ncurses
 * @note Ici on ne définit les identifiants des couleurs personnalisées, pas les couleurs elles-mêmes
 */
typedef enum
{
    COLOR_LIGHTGREY = 10, /*!< Gris clair */
} custom_colors_t;

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
    SEQUENCER_NAV_CH_MAX = MUSIC_MAX_CHANNELS,  /*!< Nombre de channels */
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


// void *args[] = {&syncSem, &finishSem, &seqNav, music, i};
typedef struct {
    sem_t *showSem;
    sem_t *syncSem;
    sem_t *finishSem;
    sequencer_nav_t *seqNav;
    music_t *music;
    int channel;
} channel_thread_args_t;

/**
 * \fn void init_ncurses()
 * \brief Initialisation de ncurses et de la fenêtre
 * \details Cette fonction s'occupe de l'initialisation de ncurses, des couleurs, de la fenêtre et des bordures
 */
void init_ncurses();

/**
 * @fn init_window(WINDOW *win)
 * @brief Initialisation de la fenêtre principale
 * @param win La fenêtre à initialiser
 */
void init_window(WINDOW *win);

/**
 * \fn void exit_ncurses()
 * \brief Fin de ncurses
 * \details Cette fonction s'occupe de la fin de ncurses
 */
void exit_ncurses();

/**
 * \fn choices_t show_main_menu()
 * \brief Affichage du menu principal
 * \details Cette fonction affiche le menu principal et gère la navigation dans le menu
 * \return Le choix de l'utilisateur
 * \see choices_t
 */
choices_t show_main_menu();

/**
 * @fn choices_t show_connection_menu(char *rfid, char *username)
 * @brief Affichage du menu de connexion et effectue la connexion
 * @param rfid Le rfid de l'utilisateur
 * @param username Le nom d'utilisateur de l'utilisateur
 * @return choices_t 
 * @note la fonction remplit les variables rfid et username
 */
choices_t show_connection_menu(char *rfid, char *username);

/**
 * @fn choices_t show_list_music(char *rfid, music_t *music)
 * @brief Affiche le menu de liste de musique et effectue les requetes pour fetch la musique
 * @param rfid Le rfid de l'utilisateur
 * @param music la musique choisie
 * @return choices_t 
 * @note la fonction remplit la musique
 * @warning la musique et le rfid doivent être alloués et initialisés
 */
choices_t show_list_music(char *rfid, music_t *music);

/**
 * @fn choices_t show_date(time_t *timestamp, char *chaine)
 * @brief Affiche la date en format lisible
 * @param timestamp Le timestamp de la date
 * @param chaine La chaine qui va contenir la date
 */
void show_date(time_t timestamp, char *chaine);

/**
 * @fn choices_t show_create_music_menu(music_t *music, char *rfid)
 * @brief La fonction qui affiche le menu de création de musique
 * @param music La musique à créer
 * @param rfid Le rfid de l'utilisateur
 * @return choices_t 
 * @note la fonction remplit la musique et le rfid
 * @warning la musique et le rfid doivent être alloués et initialisés
 */
choices_t show_create_music_menu(music_t *music, char *rfid);

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
 * @fn void show_sequencer(music_t *music, char *connected)
 * @brief La fonction qui affiche le séquenceur
 * @param music La musique à afficher
 * @param connected Le rfid de l'utilisateur connecté
 * @return choices_t 
 * @note l'utilisateur est considéré comme connecté si connected est différent de \0
 * @warning la musique et connected doit être allouée et initialisée
 */
choices_t show_sequencer(music_t *music, char *connected);

/**
 * @fn int getchr_wiringpi()
 * @brief Transpose le bitmap des buttons wiringpi en caractère ncurses
 * @return int 
 */
int getchr_wiringpi();

/**
 * @fn void play_music(music_t *music)
 * @brief Joue la musique et affiche les lignes jouées
 */
void play_music(WINDOW **channelWin, music_t *music);

/**
 * @brief play_channel(void *channelId)
 * @brief Joue un channel et modifie le navigateur pour afficher la ligne jouée
 */
void *play_channel(void *channelId);

/**
 * @brief create_channel_thread_args(sem_t *syncSem, sem_t *finishSem, sequencer_nav_t *seqNav, music_t *music, int channel)
 * @brief Crée les arguments pour le thread de channel
 * @return channel_thread_args_t 
 * @note Les arguments doivent être libérés après utilisation
 */
channel_thread_args_t *create_channel_thread_args(sem_t *showSem, sem_t *syncSem, sem_t *finishSem, sequencer_nav_t *seqNav, music_t *music, int channel);

#endif // GRAPHIC_SEQ_H

