/**
 * @file ui_menu.c
 * @brief Fichier source pour les menus de l'interface graphique
 * @version 1.0
 * @author Lukas Grando
 * @see ui_menu.h
 */

#include "ui_menu.h"

/**********************************************************************************************************************/
/*                                           Private functions                                                        */
/**********************************************************************************************************************/

/**
 * \fn WINDOW *init_menu_header(const char *app_name, const char *version)
 * \brief Initialisation de l'en-tête du menu
 * \details Cette fonction initialise l'en-tête du menu en affichant le nom de l'application et la version
 * \param app_name Le nom de l'application
 * \param version La version de l'application
 */
WINDOW *init_menu_header(const char *app_name, const char *version);

/**
 * WINDOW *init_menu_footer();
 * \brief Initialisation du pied de page du menu
 * \details Cette fonction initialise le pied de page du menu affichant les commandes de navigation
 */
WINDOW *init_menu_footer();

/**
 * \fn void init_menu(const char *title, const char *text)
 * \brief Initialisation du menu ncurses
 * \details Cette fonction initialise le menu en affichant les bordures, le titre et le texte
 * \param title Le titre du menu
 * \param text Le texte du menu
 */
WINDOW *init_menu_body(const char *title, const char *text);

/**********************************************************************************************************************/
/*                                           Public Function Definitions                                              */
/**********************************************************************************************************************/

/**
 * \fn void init_menu_colors()
 * \brief Initialisation des couleurs des menus
 * \details Cette fonction initialise les couleurs des menus
 */
void init_menu_colors() {
    init_pair(COLOR_PAIR_MENU, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_MENU_WARNING, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_PAIR_MENU_PROMPT, COLOR_GREEN, COLOR_WHITE);
}

/**
 * \fn ui_menu_t *init_menu(const char *title, const char *text)
 * \brief Initialisation du menu ncurses
 * \details Cette fonction initialise le menu en affichant les bordures, le titre et le texte et le corps du menu
 * \param title Le titre du menu
 * \param text Le texte du menu
 * \return ui_menu_t La structure du menu contenant les différentes fenêtres
 */
ui_menu_t *init_menu(const char *title, const char *text) {
    ui_menu_t *menu = (ui_menu_t *)malloc(sizeof(ui_menu_t));
    CHECK_ALLOC(menu);
    menu->header = init_menu_header(APP_NAME, APP_VERSION);
    menu->footer = init_menu_footer();
    menu->body = init_menu_body(title, text);
    return menu;
}

/**
 * \fn void destroy_menu(ui_menu_t *menu)
 * \brief Destruction du menu
 * \details Cette fonction détruit les fenêtres du menu
 * \param menu La structure du menu
 */
void destroy_menu(ui_menu_t *menu) {
    delwin(menu->header);
    delwin(menu->footer);
    delwin(menu->body);
    free(menu);
}

/**
 * \fn app_choices_t create_menu(const char *choices[], int n_choices, int highlight, choices_t choices)
 * \brief Création d'un menu ncurses
 * \details Cette fonction crée un menu ncurses
 * \param title Le titre du menu
 * \param text Le texte du menu (optionnel)
 * \param choices[] Les choix du menu
 * \param nbChoices Le nombre de choix
 * \param highlight La couleur du choix sélectionné
 * \param choices Valeur de retour de chaque choix
 * 
*/
app_choices_t create_menu(const char *title, const char *text, char **choices, int nbChoices, int highlight, app_choices_t *choices_return) {
    clear(); // on nettoie l'écran
    int c; // la touche pressée
    int i;
    int curr = highlight; // le choix actuel
    int start = 0; // pour le mode pagination 
    ui_menu_t *menu = init_menu(title, text); // Initialisation du menu
    WINDOW *body = menu->body;
    
    // On gère la navigation dans le menu
    while(1) {
        // On gére à la fois les touches du clavier et les boutons physiques
        // On affiche les choix
        for (i = start; (i < nbChoices) && (i < MENU_MAX_ITEMS); i++) {
            if (i == curr) {
                attron(A_REVERSE);
                mvprintw(5 + i-start, 8, "%s", choices[i]);
            } 
            else {
                mvprintw(5 + i-start, 8, "%s", choices[i]);
            }
            attroff(A_REVERSE);
        }
        refresh();
        c = getch();
        switch(c) {
            case KEY_UP:
                if (curr > 0) curr--;
                if(curr < start) start--;
                break;
            case KEY_DOWN:
                if (curr < nbChoices - 1) curr++;
                if(curr >= start + MENU_MAX_ITEMS) start++;
                break;
            case MENU_KEY_ENTER:
                return choices_return[curr];
            break;

            case ERR:
                break;
        }
    }
}


/**********************************************************************************************************************/
/*                                           Private Function Definitions                                             */
/**********************************************************************************************************************/

/**
 * \fn WINDOW *init_menu_header(const char *app_name, const char *version)
 * \brief Initialisation de l'en-tête du menu
 * \details Cette fonction initialise l'en-tête du menu en affichant le nom de l'application et la version
 * \param app_name Le nom de l'application
 * \param version La version de l'application
 */
WINDOW *init_menu_header(const char *app_name, const char *version) {
    int lenAppName = strlen(app_name);
    int lenVersion = strlen(version);

    int startx = (APP_COLS - MENU_HEADER_WIDTH) / 2;
    int starty = 0;
    WINDOW *header = newwin(MENU_HEADER_WIDTH, MENU_HEADER_HEIGHT, starty, startx);
    init_window(header);

    box(header, 0, 0);
    mvwprintw(header, 1, (MENU_HEADER_WIDTH - lenAppName) / 2, "%s", app_name);
    mvwprintw(header, 2, (MENU_HEADER_WIDTH - lenVersion) / 2, "%s", version);

    return header;
}

/**
 * \fn WINDOW *init_menu_footer();
 * \brief Initialisation du pied de page du menu
 * \details Cette fonction initialise le pied de page du menu affichant les commandes de navigation
 */
WINDOW *init_menu_footer() {
    UNIMPLEMENTED("init_menu_footer");
    return NULL;
}

/**
 * \fn void init_menu(const char *title, const char *text)
 * \brief Initialisation du menu ncurses
 * \details Cette fonction initialise le menu en affichant les bordures, le titre et le texte
 * \param title Le titre du menu
 * \param text Le texte du menu
 * \param centered Centre le texte verticalement si différent de 0
 */
WINDOW *init_menu_body(const char *title, const char *text) {
    UNIMPLEMENTED("init_menu_body");
}