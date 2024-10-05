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

/**
 * \fn remove_char(char *str, int index)
 * \brief Suppression d'un caractère dans une chaîne de caractères
 * \details Cette fonction supprime un caractère dans une chaîne de caractères à l'index spécifié
 */
void remove_char(char *str, int index);

/**
 * \fn add_char(char *str, int index, char c)
 * \brief Ajout d'un caractère dans une chaîne de caractères
 * \details Cette fonction ajoute un caractère dans une chaîne de caractères à l'index spécifié
 * \param str La chaîne de caractères
 * \param index L'index où ajouter le caractère
 */
void add_char(char *str, int index, char c);

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
    init_pair(COLOR_PAIR_MENU_WARNING, COLOR_ORANGE, COLOR_BLACK);
    init_pair(COLOR_PAIR_MENU_PROMPT, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_MENU_INFO, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_PAIR_MENU_ERROR, COLOR_RED, COLOR_BLACK);
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
 * \param menu la structure du menu à modifier
 * \param app_choices_t le choix de l'utilisateur
 * \param count le nombre de choix
 * \param label l'affichage des choix
 * \param escapeChoice le choix de retour en arrière 
 * \warning le menu doit être initialisé avant d'appeler cette fonction
 * \see init_menu()
 */
app_choices_t create_enumerable_body(ui_menu_t *menu, app_choices_t *choices, int count, const char **labels, app_choices_t escapeChoice) {
    int keyPressed;
    int start = 0; // pour le mode pagination
    int highlight = 0; // le choix actuel
    int end = count > MENU_MAX_ITEMS ? MENU_MAX_ITEMS : count; // pour le mode pagination
    const int offset = 4; // décalage pour l'affichage des choix (Titre, Texte, margin) // TODO:remplacer par des macros
    WINDOW *body = menu->body;
    display_menu(menu);

    // On gère la navigation dans le menu
    while(1) {

        // On efface les anciens choix
        // todo: remplacer par un werase (créer une nouvelle fenetre à l'intérieur de body ?)
        for (int i = 0; i <= MENU_MAX_ITEMS; i++) {
            mvwprintw(body, i + offset, MENU_MARGIN*2, "%*s", getmaxx(body) - 2*MENU_MARGIN-1, "");
        }

        for(int i = start; i < end; i++) {
            if(i == highlight) {
                wattron(body, A_REVERSE);
            }
            mvwprintw(body, i - start + offset, MENU_MARGIN*2, "[%d] %s", i + 1, labels[i]);
            wattroff(body, A_REVERSE);
        }

        if(end != count) {
            mvwprintw(body, end - start + offset, MENU_MARGIN*2, "...");
        }

        wrefresh(body);
        keyPressed = wgetch(body);

        // Todo: mettre ça dans une fonction à part (mouse_handler)
        if(keyPressed == KEY_MOUSE) {
            // gestion de la molette de la souris
            MEVENT event;
            if(getmouse(&event) == OK) {
                if(event.bstate & BUTTON1_PRESSED) { // clic gauche
                    keyPressed = KEY_ENTER;
                }
                else if(event.bstate & BUTTON4_PRESSED) { // molette vers le haut
                    keyPressed = KEY_UP;
                }
                else if(event.bstate & BUTTON5_PRESSED) { // molette vers le bas
                    keyPressed = KEY_DOWN;
                }
                else if(event.bstate & BUTTON2_CLICKED) { // clic droit
                    keyPressed = MENU_KEY_ESCAPE;
                }
            }
        }
        
        switch(keyPressed) {
            case KEY_UP:
                if(highlight > start) highlight--;
                else if(start > 0) {
                    start--;
                    end = start + MENU_MAX_ITEMS;
                    highlight--;
                }
            break;
            
            case KEY_DOWN:
                if(highlight < end-1) highlight++;
                else if(end < count) {
                    start++;
                    end = start + MENU_MAX_ITEMS < count ? start + MENU_MAX_ITEMS : count;
                    highlight++;
                }
            break;

            case MENU_KEY_ENTER:
                return choices[highlight];
            break;

            case MENU_KEY_ESCAPE:
                return escapeChoice;
        }
    }
}


/**
 * \fn void create_input_field(int win, int y, int x, const char *label, char *value, int length, int isPassword)
 * \brief Création d'un champ de saisie
 * \details Cette fonction crée un champ de saisie en affichant un label et la valeur saisie
 * \param win La fenêtre du champ de saisie
 * \param params Les paramètres du champ de saisie
 * \return WINDOW* La fenêtre du champ de saisie
 * \note Si le pointeur win est NULL, la fonction crée une nouvelle fenêtre et l'assigne à win
 * \warning La fenêtre doit être détruite après utilisation
 */
WINDOW *create_input_field(WINDOW **win, WINDOW *parent, menu_form_input_params_t *params) {
    int fieldWidth = params->maxLength + 2*2;  // todo : remplacer 2 par MENU_FORM_INPUT_PADDING (ou autre)
    int cursorPos = strlen(params->value) + 2; // position du curseur
    int lenValue = strlen(params->value);

    // Création de la fenêtre
    if(*win == NULL) {
        *win = derwin(parent, 3, fieldWidth, params->y, params->x);
        init_window(*win);
    }
    box(*win, 0, 0);

    // affichage de la valeur du champ
    if(params->isPassword) {
        for(int i = 0; i < lenValue; i++) {
            mvwprintw(*win, 1, 2 + i, "*");
        }
    }
    else mvwprintw(*win, 1, 2, "%s", params->value);

    // affichage du label du champ dans la bordure supérieure
    mvwprintw(*win, 0, 2, "%s", params->label);
    wmove(*win, 1, cursorPos);
    while(params->isFocused == 1) {
        curs_set(1);
        wrefresh(*win);
        int keypressed = wgetch(*win);
        if(keypressed == params->stopFocusKey) {
            params->isFocused = 0;
            break;
        }

        switch(keypressed) {
            case KEY_LEFT:
                if(cursorPos > 2) cursorPos--;
            break;

            case KEY_RIGHT:
                if(cursorPos < lenValue + 2) cursorPos++;
            break;

            case KEY_BACKSPACE:
                // suppression du caractère précédent si possible (chaine et affichage)
                if(lenValue > 0 && cursorPos > 2) {
                    cursorPos--;
                    remove_char(params->value, cursorPos - 2);
                    lenValue--;
                }
            break;

            case KEY_DC:
                // suppression du caractère sous le curseur (chaine et affichage)
                if(lenValue > 0 && cursorPos < fieldWidth - 1) {
                    remove_char(params->value, cursorPos - 2);
                    lenValue--;
                    if(cursorPos == lenValue + 2) {
                        cursorPos--;
                    }
                }
            break;

            case MENU_KEY_ENTER:
                params->isFocused = 0;
            break;

            default:
                // ajout du caractère à la position du curseur si imprimable (chaine et affichage)
                if(lenValue < params->maxLength && isprint(keypressed)) {
                    add_char(params->value, cursorPos - 2, keypressed);
                    lenValue++;
                    cursorPos++;
                }
            break;
        }
        // réaffichage de la valeur du champ
        mvwprintw(*win, 1, 2, "%*s", fieldWidth - 3, " ");
        if(params->isPassword) {
            for(int i = 0; i < lenValue; i++) {
                mvwprintw(*win, 1, 2 + i, "*");
            }
        }
        else {
            mvwprintw(*win, 1, 2, "%s", params->value);
        }
        wmove(*win, 1, cursorPos);

    }
    // désactivation du curseur
    curs_set(0);
    // désactivation du mode inverse si activé
    return *win;
}


/**
 * \fn menu_credentials_t create_credentials_body(WINDOW *body)
 * \brief Création du corps d'un menu de connexion (identifiants)
 * \details Cette fonction crée le corps d'un menu de connexion en affichant le titre, le texte et les champs d'entrée pour les identifiants
 * \param body La fenêtre du corps du menu
 * \return menu_credentials_t Les identifiants de connexion
 */
menu_credentials_t create_credentials_body(WINDOW *body) {
    int keyPressed = 0;
    int lastFocus = 0; // 0 = username, 1 = password, 2 = submit
    WINDOW *usernameField = NULL;
    WINDOW *passwordField = NULL;
    menu_credentials_t credentials;
    credentials.username[0] = '\0';
    credentials.password[0] = '\0';

    menu_form_input_params_t usernameParams = {
        .x = MENU_MARGIN,
        .y = MENU_MARGIN + 2,
        .isPassword = 0,
        .isFocused = 0,
        .label = "Username",
        .value = credentials.username,
        .maxLength = APP_USERNAME_MAX_LENGTH,
        .stopFocusKey = '\t'
    };
    menu_form_input_params_t passwordParams = {
        .x = MENU_MARGIN,
        .y = MENU_MARGIN + 6,
        .isPassword = 1,
        .isFocused = 0,
        .label = "Password",
        .value = credentials.password,
        .maxLength = APP_PASSWORD_MAX_LENGTH,
        .stopFocusKey = '\t'
    };
    create_input_field(&usernameField, body, &usernameParams);
    create_input_field(&passwordField, body, &passwordParams);

    while(1) {
        mvwprintw(body, 12, MENU_MARGIN, "[ENTER] Submit");
        wrefresh(body);
        wrefresh(usernameField);
        wrefresh(passwordField);

        usernameParams.isFocused = 1;
        create_input_field(&usernameField, body, &usernameParams);

        passwordParams.isFocused = 1;
        create_input_field(&passwordField, body, &passwordParams);

        wattron(body, A_REVERSE);
        mvwprintw(body, 12, MENU_MARGIN, "[ENTER] Submit");
        wattroff(body, A_REVERSE);

        // Si on valide la saisie
        keyPressed = wgetch(body);
        if(keyPressed == MENU_KEY_ENTER) {
            delwin(usernameField);
            delwin(passwordField);
            return credentials;
        }
        // On reboucle sur le premier champ
        usernameParams.isFocused = 1;
        
    }

    return credentials;
}

/**
 * \fn menu_create_credits_body(WINDOW *menu, app_choices_t choice)
 * \brief Création du corps d'un menu de crédits
 * \details Cette fonction crée le corps d'un menu de crédits en affichant le titre, le texte et les crédits
 * \param body La structure du menu
 * \return app_choices_t retourne le choix passé en paramètre si l'utilisateur appuie sur n'importe quelle touche
 */
app_choices_t create_credits_body(WINDOW *body, app_choices_t choice) {
    int keyPressed = 0;
    int x = 0;
    int y = 0;
    int direction = 0;
    int colorPair = 0;
    
    int max_x = getmaxx(body)-1; // -2 pour exclure la bordure
    int max_y = getmaxy(body)-1; // -2 pour exclure la bordure
    const char *credits[] = {
        "Application created by Lukas Grando",
        "Version : " APP_VERSION,
        "Date : " __DATE__ " " __TIME__,
        "Initialy created by Lukas & CaptainBidou as a school project using JOY-PI",
        "Using ncurses libasound",
        "Press any key to return"
    };

    int count = ARRAY_SIZE(credits);
    
    for(int i = 0; i < count; i++) {
        mvwprintw(body, i*2 + MENU_MARGIN, MENU_MARGIN, "%s", credits[i]);
    }
    wrefresh(body);
    wtimeout(body, 10);
    while (1)
    {
        keyPressed = wgetch(body);
        if(keyPressed != ERR) {
            return choice;
        }
        DEBUG_LOG("X: %d, Y: %d", x, y);

        x += (direction == 0) - (direction == 2); // +1 droite, -1 gauche
        y += (direction == 1) - (direction == 3); // +1 bas, -1 haut

        // Changer de direction quand on atteint les bords
        if (x >= max_x && direction == 0) direction = 1;  // Droite -> Bas
        else if (y >= max_y && direction == 1) direction = 2;  // Bas -> Gauche
        else if (x <= 0 && direction == 2) direction = 3;  // Gauche -> Haut
        else if (y <= 0 && direction == 3) direction = 0;  // Haut -> Droite

        // Changer de couleur aléatoirement
        // Les menu pairs vaut d'id COLOR_PAIR_MENU à COLOR_PAIR_MENU_END (end non inclus)
        colorPair = COLOR_PAIR_MENU + (rand() % (COLOR_PAIR_MENU_END - COLOR_PAIR_MENU));

        wattron(body, A_REVERSE);
        wattron(body, COLOR_PAIR(colorPair));
        mvwprintw(body, y, x, "%d", rand() % 2);
        wattroff(body, A_REVERSE);
        wattroff(body, COLOR_PAIR(colorPair));
    }
    return choice;
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

    WINDOW *body = newwin(MENU_BODY_HEIGHT, MENU_BODY_WIDTH, MENU_BODY_Y0, MENU_BODY_X0);
    init_window(body);

    box(body, 0, 0);
    mvwprintw(body, 1, (MENU_BODY_WIDTH - lenTitle) / 2, "%s", title);
    mvwprintw(body, MENU_MARGIN, MENU_MARGIN, "%s", text);

    return body;
}

/**
 * \fn remove_char(char *str, int index)
 * \brief Suppression d'un caractère dans une chaîne de caractères
 * \details Cette fonction supprime un caractère dans une chaîne de caractères à l'index spécifié
 */
void remove_char(char *str, int index) {
    size_t len = strlen(str);
    // on décale tous les caractères d'un cran vers la gauche à partir de l'index
    for(size_t i = index; i < len; i++) {
        str[i] = str[i+1];
    }
    // on supprime le dernier caractère
    str[len-1] = '\0';
}

/**
 * \fn add_char(char *str, int index, char c)
 * \brief Ajout d'un caractère dans une chaîne de caractères
 * \details Cette fonction ajoute un caractère dans une chaîne de caractères à l'index spécifié
 * \param str La chaîne de caractères
 * \param index L'index où ajouter le caractère
 * @warning La chaîne doit être suffisamment grande pour accueillir le caractère supplémentaire
 */
void add_char(char *str, int index, char c) {
    size_t len = strlen(str);
    // on décale tous les caractères d'un cran vers la droite à partir de l'index
    for(int i = len; i > index; i--) {
        str[i] = str[i-1];
    }
    // on ajoute le caractère à l'index
    str[index] = c;
    // on ajoute le caractère de fin de chaîne
    str[len+1] = '\0';
}