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
#define MENU_HEADER_HEIGHT 4 /*!< Hauteur de l'entête du menu */
#define MENU_HEADER_WIDTH 54 /*!< Largeur de l'entête du menu */
#define MENU_MAX_ITEMS 6 /*!< Nombre maximum d'éléments sur une page de menu */
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
typedef enum {
    COLOR_PAIR_MENU = 10,    /*!< Couleur du menu */
    COLOR_PAIR_MENU_BACKGROUND, /*!< Couleur de fond du menu */
    COLOR_PAIR_MENU_WARNING, /*!< Couleur du menu en cas d'erreur */
    COLOR_PAIR_MENU_PROMPT,  /*!< Couleur du menu d'entrée utilisateur */
} menu_color_pairs_t;

/**
 * \fn const char *menu_key2str(int key)
 * \brief Conversion d'une touche en chaîne de caractères
 * \details Cette fonction convertit une touche en chaîne de caractères
 * \param key La touche à convertir
 * \return const char* La chaîne de caractères correspondant à la touche
 */
const char *menu_key2str(int key);

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

/**
 * \fn void display_menu(ui_menu_t *menu, int highlight, int start, int end)
 * \brief Affichage du menu
 * \details Cette fonction affiche le menu dans son intégralité
 */
void display_menu(ui_menu_t *menu);

/**
 * \fn void refresh_menu(ui_menu_t *menu);
 * \brief Rafraîchissement du menu
 * \details Cette fonction redessine uniquement le corps du menu
 * \param menu La structure du menu
 */
void refresh_menu(ui_menu_t *menu);



/**
 * \fn app_choice_t create_enumerable_body(ui_menu_t *menu, const char *title, const char *text, menu_enum_t *enumerable)
 * \brief Création du corps d'un menu énumérable
 * \details Cette fonction crée le corps d'un menu énumérable en affichant le titre, le texte et les choix (labels)
 * \param menu elle retourne le choix de l'utilisateur
 * \param app_choices_t le choix de l'utilisateur
 * \param count le nombre de choix
 * \param label l'affichage des choix
 * \param escapeChoice le choix de retour en arrière 
 * \warning le menu doit être initialisé avant d'appeler cette fonction
 * \see init_menu()
 */
app_choices_t create_enumerable_body(ui_menu_t *menu, app_choices_t *choices, int count, const char **labels, app_choices_t escapeChoice);



#endif // UI_MENU_H