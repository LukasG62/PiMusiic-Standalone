/**
 * @file ui_menu.c
 * @brief Fichier source pour les menus de l'interface graphique
 * @version 1.0
 * @author Lukas Grando
 * @see ui_menu.h
 */

#include "ui/base/ui_menu.h"

/**********************************************************************************************************************/
/*                                           Private functions                                                        */
/**********************************************************************************************************************/

/**
 * \fn ui_menu_view_draw(ui_view_t *self)
 * \brief Fonction de dessin pour une vue de menu
 * \param self La vue à dessiner
 */
static void ui_menu_view_draw(ui_view_t *self);

/**
 * \fn ui_menu_view_destroy(ui_view_t *self)
 * \brief Fonction de destruction pour une vue de menu
 * \param self La vue à détruire
 */
static void ui_menu_view_destroy(ui_view_t *self);

/**
 * \fn ui_menu_view_handle_event(ui_view_t *self, event_t event)
 * \brief Fonction de gestion des événements pour une vue de menu
 * \param self La vue à laquelle l'événement est destiné
 * \param event L'événement à gérer
 * @return bool true si l'événement a été consommé, false sinon
 */
static bool ui_menu_view_handle_event(ui_view_t *self, event_t *event);

/**
 * \fn ui_menu_t *init_menu(const char *title, const char *text)
 * \brief Initialisation du menu ncurses
 * \details Cette fonction initialise le menu en affichant les bordures, le titre et le texte et le corps du menu
 * \param title Le titre du menu
 * \param text Le texte du menu
 * \return ui_menu_t La structure du menu contenant les différentes fenêtres
 */
static ui_menu_t *init_menu(const char *title, const char *text);

/**
 * \fn WINDOW *init_menu_header(const char *app_name, const char *version)
 * \brief Initialisation de l'en-tête du menu
 * \details Cette fonction initialise l'en-tête du menu en affichant le nom de l'application et la version
 * \param app_name Le nom de l'application
 * \param version La version de l'application
 */
static WINDOW *init_menu_header(const char *app_name, const char *version);

/**
 * WINDOW *init_menu_footer();
 * \brief Initialisation du pied de page du menu
 * \details Cette fonction initialise le pied de page du menu affichant les commandes de navigation
 */
static WINDOW *init_menu_footer();

/**
 * \fn void init_menu(const char *title, const char *text)
 * \brief Initialisation du menu ncurses
 * \details Cette fonction initialise le menu en affichant les bordures, le titre et le texte
 * \param title Le titre du menu
 * \param text Le texte du menu
 */
static WINDOW *init_menu_body(const char *title, const char *text);

/**
 * \fn const char *menu_key2str(int key)
 * \brief Conversion d'une touche en chaîne de caractères
 * \details Cette fonction convertit une touche en chaîne de caractères
 * \param key La touche à convertir
 * \return const char* La chaîne de caractères correspondant à la touche
 */
static const char *menu_key2str(int key);

static void ui_menu_destroy_data(ui_menu_t *menu) {
    if(menu->header) {
        delwin(menu->header);
    }
    if(menu->footer) {
        delwin(menu->footer);
    }
    if(menu->body) {
        delwin(menu->body);
    }
}

/**********************************************************************************************************************/
/*                                           Public Function Definitions                                              */
/**********************************************************************************************************************/

/**
 * \fn void init_menu_colors()
 * \brief Initialisation des couleurs des menus
 * \details Cette fonction initialise les couleurs des menus
 */
void ui_menu_init_colors() {
    init_pair(COLOR_PAIR_MENU, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_MENU_WARNING, COLOR_ORANGE, COLOR_BLACK);
    init_pair(COLOR_PAIR_MENU_PROMPT, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_MENU_INFO, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_PAIR_MENU_ERROR, COLOR_RED, COLOR_BLACK);
}

/**
 * \fn ui_view_t *ui_menu_create_view(const char *title, const char *text)
 * \brief Création d'une vue de menu
 * \param title Le titre du menu
 * \param text Le texte du menu
 */
ui_view_t *ui_menu_create_view(const char *title, const char *text) {
    ui_menu_t *menu = init_menu(title, text);
    ui_view_t *view = ui_view_create(
        ui_menu_view_draw,
        ui_menu_view_handle_event,
        ui_menu_view_destroy,
        (void *)menu
    );
    return view;
}

/**
 * @private
 * @fn ui_menu_view_draw(ui_view_t *self)
 * @brief Fonction de dessin pour une vue de menu
 * @param self La vue à dessiner
 */
static void ui_menu_view_draw(ui_view_t *self) {
    if(!self || !self->data) return;
    ui_menu_t *menuData = (ui_menu_t *)self->data;

    wnoutrefresh(menuData->header);
    wnoutrefresh(menuData->footer);
    wnoutrefresh(menuData->body);

    for(int i = 0; i < self->componentCount; i++) {
        if(i == self->focusedComponent) continue;
        
        ui_component_t *component = self->components[i];
        if(component && component->draw) {
            component->draw(component);
        }
    }
    
    if(self->focusedComponent >= 0 && self->focusedComponent < self->componentCount) {
        ui_component_t *focused = self->components[self->focusedComponent];
        if(focused && focused->draw) {
            focused->draw(focused);
        }
    }
}

/**
 * @private
 * @fn ui_menu_view_destroy(ui_view_t *self)
 * @brief Fonction de destruction pour une vue de menu
 * @param self La vue à détruire
 */
static void ui_menu_view_destroy(ui_view_t *self) {
    if(!self) return;
    ui_menu_t *menuData = (ui_menu_t *)self->data;

    for(int i = 0; i < self->componentCount; i++) {
        ui_component_t *component = self->components[i];
        if(component && component->destroy) {
            component->destroy(component);
        }
    }
    free(self->components);

    if(self->data) {
        ui_menu_t *menuData = (ui_menu_t *)self->data;
        ui_menu_destroy_data(menuData);
    }
    free(self);
}

/**
 * @private
 * @fn ui_menu_view_handle_event(ui_view_t *self, event_t event)
 * @brief Fonction de gestion des événements pour une vue de menu
 * @param self La vue à laquelle l'événement est destiné
 * @param event L'événement à gérer
 * @return bool true si l'événement a été consommé, false sinon
 */
static bool ui_menu_view_handle_event(ui_view_t *self, event_t *event) {
    if(!self || !self->data) return false;
    ui_component_t *focusedComponent = NULL;
    if(self->focusedComponent >= 0 && self->focusedComponent < self->componentCount) {
        focusedComponent = self->components[self->focusedComponent];
    }

    if(self->componentCount > 0 && event->type == UI_EVENT_KEY_PRESSED) {
        int key = (int)(intptr_t) event->data;

        if(key == KEY_BTAB ||key == '\t') {
            if(self->componentCount <= 1) return false;
            if(focusedComponent) {
                focusedComponent->isFocused = false;
            }
            if(key == KEY_BTAB) {
                self->focusedComponent--;
                if(self->focusedComponent < 0) self->focusedComponent = self->componentCount - 1;
            } else {
                self->focusedComponent++;
                if(self->focusedComponent >= self->componentCount) self->focusedComponent = 0;
            };
            self->components[self->focusedComponent]->isFocused = true;
        }
    }
    if(focusedComponent && focusedComponent->handle_event) {
        return focusedComponent->handle_event(focusedComponent, event);
    } 
    return false;
}

/**
 * @private
 * @fn ui_menu_t *init_menu(const char *title, const char *text)
 * @brief Initialisation du menu ncurses
 * @details Cette fonction initialise le menu en affichant les bordures, le titre et le texte et le corps du menu
 * @param title Le titre du menu
 * @param text Le texte du menu
 * @return ui_menu_t La structure du menu contenant les différentes fenêtres
 */
static ui_menu_t *init_menu(const char *title, const char *text) {
    ui_menu_t *menu = (ui_menu_t *)malloc(sizeof(ui_menu_t));
    CHECK_ALLOC(menu);
    menu->header = init_menu_header(APP_NAME, APP_VERSION);
    menu->footer = init_menu_footer();
    menu->body = init_menu_body(title, text);
    return menu;
}

/**
 * @private
 * @fn WINDOW *init_menu_header(const char *app_name, const char *version)
 * @brief Initialisation de l'en-tête du menu
 * @param app_name Le nom de l'application
 * @param version La version de l'application
 * @return WINDOW * La fenêtre de l'en-tête du menu
 */
static WINDOW *init_menu_header(const char *app_name, const char *version) {
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
 * @private
 * @fn WINDOW *init_menu_footer()
 * @brief Initialisation du pied de page du menu
 */
static WINDOW *init_menu_footer() {
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
 * @private
 * @fn WINDOW *init_menu_body(const char *title, const char *text)
 * @brief Initialisation du corps du menu
 * @param title Le titre du menu
 * @param text Le texte du menu
 * @return WINDOW * La fenêtre du corps du menu
 */
static WINDOW *init_menu_body(const char *title, const char *text) {
    int lenTitle = strlen(title);

    WINDOW *body = newwin(MENU_BODY_HEIGHT, MENU_BODY_WIDTH, MENU_BODY_Y0, MENU_BODY_X0);
    init_window(body);

    box(body, 0, 0);
    mvwprintw(body, 1, (MENU_BODY_WIDTH - lenTitle) / 2, "%s", title);
    mvwprintw(body, MENU_MARGIN, MENU_MARGIN, "%s", text);

    return body;
}

/**
 * @private
 * @fn const char *menu_key2str(int key)
 * @brief Conversion d'une touche en chaîne de caractères
 * @param key La touche à convertir
 * @return const char * La chaîne de caractères correspondante
 */
static const char *menu_key2str(int key) {
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
