/**
 * @file uiCommon.h
 * @brief Common header file for the UI
 * Ce fichier contient les macros communes pour les interfaces graphiques.
 */
#ifndef UI_COMMON_H
#define UI_COMMON_H

#include "common.h" //!< Inclusion des macros communes à l'application
#include <ncurses.h> //!< Inclusion de la bibliothèque ncurses (commune à toutes les interfaces graphiques)
#include <pthread.h> //!< Inclusion de la bibliothèque pthread (commune à toutes les interfaces graphiques)


// Constantes pour la fenêtre de l'application
#define APP_COLS 106 /*!< Nombre de colonnes de la fenêtre de l'application */
#define APP_LINES 29 /*!< Nombre de lignes de la fenêtre de l'application */
#define APP_NAME "PiMusiic - Standalone" //!< Nom de l'application
#define APP_VERSION "1.0" //!< Version de l'application

/**
 * \enum choices_t
 * \brief Enumération des vues de l'interface graphique
 * \details Cette énumération permet les différentes vues de l'interface graphique
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
} app_choices_t;

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
 * \fn void create_custom_colors(int color_id, const char *color)
 * \brief Création de couleurs personnalisées pour ncurses en passant par des codes hexadécimaux
 * \param color_id L'identifiant de la couleur
 * \param color La couleur en hexadécimal
 * \return La couleur personnalisée
 */
int create_custom_colors(int color_id, const char *color);

/**
 * @fn choices_t show_date(time_t *timestamp, char *chaine)
 * @brief Affiche la date en format lisible
 * @param timestamp Le timestamp de la date
 * @param chaine La chaine qui va contenir la date
 */
void show_date(time_t timestamp, char *chaine);

#endif // UI_COMMON_H