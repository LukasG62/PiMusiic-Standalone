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
 * \fn const char *menu_key2str(int key)
 * \brief Conversion d'une touche en chaîne de caractères
 * \details Cette fonction convertit une touche en chaîne de caractères
 * \param key La touche à convertir
 * \return const char* La chaîne de caractères correspondant à la touche
 */
const char *menu_key2str(int key) {
    const char str[2];
    switch(key) {
        case MENU_KEY_UP:
            return " ^ ";
        case MENU_KEY_DOWN:
            return " v ";
        case MENU_KEY_ENTER:
            return "[ENTER] Confirm";
        case MENU_KEY_ESCAPE:
            return "[ESC] Return";
        default:
            return "UNKNOWN";
    }
}

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
 * \fn void display_menu(ui_menu_t *menu, int highlight, int start, int end)
 * \brief Affichage du menu
 * \details Cette fonction affiche le menu dans son intégralité
 */
void display_menu(ui_menu_t *menu) {
    wrefresh(menu->header);
    wrefresh(menu->footer);
    wrefresh(menu->body);
}

/**
 * \fn void refresh_menu(ui_menu_t *menu);
 * \brief Rafraîchissement du menu
 * \details Cette fonction redessine uniquement le corps du menu
 * \param menu La structure du menu
 */
void refresh_menu(ui_menu_t *menu) {
    wrefresh(menu->body);
}

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
app_choices_t create_enumerable_body(ui_menu_t *menu, app_choices_t *choices, int count, const char **labels, app_choices_t escapeChoice) {
    int c; // la touche pressée
    int highlight = 0; // le choix actuel
    int start = 0; // pour le mode pagination
    int end = count; // pour le mode pagination
    const int offset = 4; // décalage pour l'affichage des choix (Titre, Texte, margin) // TODO:remplacer par des macros
    WINDOW *body = menu->body;
    display_menu(menu);
    // On gère la navigation dans le menu
    while(1) {
        for(int i = start; i < end; i++) {
            if(i == highlight) {
                wattron(body, A_REVERSE);
            }
            mvwprintw(body, i + offset, MENU_MARGIN*2, "[%d] %s", i + 1, labels[i]);
            wattroff(body, A_REVERSE);
        }
        wrefresh(body);

        c = wgetch(body);
        switch(c) {
            case KEY_UP:
                if(highlight > 0) highlight--;
            break;

            case KEY_DOWN:
                if(highlight < count - 1) highlight++;
            break;

            case KEY_ENTER:
                return choices[start + highlight];
            break;

            case MENU_KEY_ESCAPE:
                return escapeChoice;
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
    WINDOW *header = newwin(MENU_HEADER_HEIGHT, MENU_HEADER_WIDTH, starty, startx);
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
    WINDOW *footer = newwin(MENU_FOOTER_HEIGHT, MENU_FOOTER_WIDTH, MENU_FOOTER_Y0, MENU_FOOTER_X0);
    init_window(footer);
    box(footer, 0, 0);

    attron(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT) | A_BOLD);
    mvwprintw(footer, 1, 1, "Navigation");
    mvwprintw(footer, 1, MENU_FOOTER_WIDTH - MENU_MARGIN - 25, "Selection");
    attroff(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT) | A_BOLD);

    // Navigation
    mvwprintw(footer, 2, 1, "%s", menu_key2str(MENU_KEY_UP));
    mvwprintw(footer, 3, 1, "%s", menu_key2str(MENU_KEY_DOWN));

    // Validation
    mvwprintw(footer, 2, MENU_FOOTER_WIDTH - MENU_MARGIN - 25, "%s", menu_key2str(MENU_KEY_ENTER));
    mvwprintw(footer, 3, MENU_FOOTER_WIDTH - MENU_MARGIN - 25, "%s", menu_key2str(MENU_KEY_ESCAPE));

    return footer;
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
    int lenTitle = strlen(title);
    int lenText = strlen(text);

    WINDOW *body = newwin(MENU_BODY_HEIGHT, MENU_BODY_WIDTH, MENU_BODY_Y0, MENU_BODY_X0);
    init_window(body);

    box(body, 0, 0);
    mvwprintw(body, 1, (MENU_BODY_WIDTH - lenTitle) / 2, "%s", title);
    mvwprintw(body, MENU_MARGIN, MENU_MARGIN, "%s", text);

    return body;
}