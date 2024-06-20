/**
 * @file uiMenu.h
 * @brief Fichier d'en-tête pour les menus de l'interface graphique
 * @version 1.0
 * @author Lukas Grando
 */
#ifndef UI_MENU_H
#define UI_MENU_H

#include "ui_common.h" //!< Inclusion des macros communes à l'interface graphique


#define MENU_MARGIN 2 /*!< Marge du menu */
#define MENU_HEADER_HEIGHT 6 /*!< Hauteur de l'entête du menu */
#define MENU_HEADER_WIDTH 54 /*!< Largeur de l'entête du menu */
#define MENU_MAX_ITEMS 6 /*!< Nombre maximum d'éléments sur une page de menu */
#define MENU_FOOTER_HEIGHT 3 /*!< Hauteur du pied de page du menu */
#define MENU_FOOTER_WIDTH 106 /*!< Largeur du pied de page du menu */
#define MENU_BODY_X0 MENU_MARGIN /*!< Position X du corps du menu */
#define MENU_BODY_Y0 MENU_HEADER_HEIGHT + MENU_MARGIN /*!< Position Y du corps du menu */
#define MENU_BODY_HEIGHT (APP_LINES - MENU_HEADER_HEIGHT - MENU_FOOTER_HEIGHT) /*!< Hauteur du corps du menu */
#define MENU_BODY_WIDTH (APP_COLS - MENU_MARGIN) /*!< Largeur du corps du menu */

// Touches de navigation des menus
#define MENU_KEY_UP KEY_UP /*!< Touche pour monter dans le menu */
#define MENU_KEY_DOWN KEY_DOWN /*!< Touche pour descendre dans le menu */
#define MENU_KEY_ENTER KEY_ENTER /*!< Touche pour valider un choix */
#define MENU_KEY_ESCAPE 27 /*!< Touche pour annuler un choix */


/**
 * \struct ui_menu_t
 * \brief Structure pour les menus
 * \details Cette structure permet de définir un menu
 * Un menu est composé d'un header, d'un footer et d'un corps
 */
typedef struct {
    WINDOW *header;  /*!< En-tête du menu */
    WINDOW *footer;  /*!< Pied de page du menu */
    WINDOW *body;    /*!< Corps du menu */
} ui_menu_t;

/**
 * \enum menu_color_pairs_t
 * \brief Enumération des paires de couleurs utilisées dans les menus
 */
typedef enum
{
    // Couleurs pour les menus
    COLOR_PAIR_MENU = 10,    /*!< Couleur du menu */
    COLOR_PAIR_MENU_BACKGROUND, /*!< Couleur de fond du menu */
    COLOR_PAIR_MENU_WARNING, /*!< Couleur du menu en cas d'erreur */
    COLOR_PAIR_MENU_PROMPT,  /*!< Couleur du menu d'entrée utilisateur */
} menu_color_pairs_t;

/**
 * \fn void init_menu_colors()
 * \brief Initialisation des couleurs des menus
 * \details Cette fonction initialise les couleurs des menus
 */
void init_menu_colors();

/**
 * \fn ui_menu_t init_menu(const char *title, const char *text)
 * \brief Initialisation du menu ncurses
 * \details Cette fonction initialise le menu en affichant les bordures, le titre et le texte et le corps du menu
 * \param title Le titre du menu
 * \param text Le texte du menu
 * \return ui_menu_t La structure du menu contenant les différentes fenêtres
 */
ui_menu_t *init_menu(const char *title, const char *text);

/**
 * \fn void destroy_menu(ui_menu_t menu)
 * \brief Destruction du menu
 * \details Cette fonction détruit les fenêtres du menu
 * \param menu La structure du menu
 */
void destroy_menu(ui_menu_t *menu);


#endif // UI_MENU_H