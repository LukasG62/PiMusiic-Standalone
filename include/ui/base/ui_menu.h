/**
 * @file uiMenu.h
 * @brief Fichier d'en-tête pour les menus de l'interface graphique
 * @version 2.0
 * @author Lukas Grando
 */
#ifndef UI_MENU_H
#define UI_MENU_H

#include "ui/base/ui_common.h"
#include "ui/base/ui_view.h"
#include "ui/form/ui_form_input.h"


#define MENU_MARGIN 2 /*!< Marge du menu */
#define MENU_HEADER_HEIGHT 4 /*!< Hauteur de l'entête du menu */
#define MENU_HEADER_WIDTH 54 /*!< Largeur de l'entête du menu */
#define MENU_MAX_ITEMS 9 /*!< Nombre maximum d'éléments sur une page de menu */
#define MENU_FOOTER_HEIGHT 5 /*!< Hauteur du pied de page du menu */
#define MENU_FOOTER_X0 MENU_MARGIN /*!< Position X du pied de page du menu */
#define MENU_FOOTER_Y0 APP_LINES - MENU_FOOTER_HEIGHT /*!< Position Y du pied de page du menu */
#define MENU_FOOTER_WIDTH APP_COLS - MENU_MARGIN*2 /*!< Largeur du pied de page du menu */
#define MENU_BODY_X0 MENU_MARGIN /*!< Position X du corps du menu */
#define MENU_BODY_Y0 MENU_HEADER_HEIGHT + MENU_MARGIN /*!< Position Y du corps du menu */
#define MENU_BODY_HEIGHT (APP_LINES - MENU_HEADER_HEIGHT - MENU_FOOTER_HEIGHT - 2 * MENU_MARGIN) /*!< Hauteur du corps du menu */
#define MENU_BODY_WIDTH (APP_COLS - MENU_MARGIN*2) /*!< Largeur du corps du menu */

// Touches de navigation des menus
#define MENU_KEY_UP KEY_UP /*!< Touche pour monter dans le menu */
#define MENU_KEY_DOWN KEY_DOWN /*!< Touche pour descendre dans le menu */
#define MENU_KEY_ENTER 10 /*!< Touche pour valider un choix */
#define MENU_KEY_ESCAPE 27 /*!< Touche pour annuler un choix */

/**
 * \struct ui_menu_t
 * \brief Structure pour les menus
 * \details Cette structure permet de définir un menu
 * Un menu est composé d'un header, d'un footer et d'un corps
 */
typedef struct {
    WINDOW *header; /*!< En-tête du menu */
    WINDOW *footer; /*!< Pied de page du menu */
    WINDOW *body; /*!< Corps du menu */
} ui_menu_t;

/**
 * \struct menu_credentials_t
 * \brief Structure pour les identifiants de connexion
 * \details Cette structure permet de définir les identifiants de connexion
 */
typedef struct {
    char username[APP_USERNAME_MAX_LENGTH + 1]; /*!< Nom d'utilisateur */
    char password[APP_PASSWORD_MAX_LENGTH + 1]; /*!< Mot de passe */
} ui_menu_credentials_t;

/**
 * \enum menu_color_pairs_t
 * \brief Enumération des paires de couleurs utilisées dans les menus
 */
typedef enum {
    COLOR_PAIR_MENU = 10, /*!< Couleur du menu */
    COLOR_PAIR_MENU_WARNING, /*!< Couleur du menu pour du texte d'avertissement */
    COLOR_PAIR_MENU_PROMPT, /*!< Couleur du menu pour du texte de saisie */
    COLOR_PAIR_MENU_INFO, /*!< Couleur du menu pour du texte d'information */
    COLOR_PAIR_MENU_ERROR, /*!< Couleur du menu pour du texte d'erreur */
    COLOR_PAIR_MENU_END, /*!< Fin des couleurs du menu */
} ui_menu_color_pairs_t;

/**
 * \fn void init_menu_colors()
 * \brief Initialisation des couleurs des menus
 * \details Cette fonction initialise les couleurs des menus
 */
void ui_menu_init_colors();

/**
 * \fn ui_view_t *ui_menu_create_view(const char *title, const char *text)
 * \brief Création d'une vue de menu
 * \param title Le titre du menu
 * \param text Le texte du menu
 */
ui_view_t *ui_menu_create_view(const char *title, const char *text);

#endif // UI_MENU_H