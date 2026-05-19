/**
 * @file ui_form.c
 * @brief Définition des fonctions pour manipuler les formulaires
 */
#include "ui/form/ui_form_input.h"

#define GET_INPUT_FIELD_COLOR(comp, param) (comp->isFocused && param->focusColorPair != -1) ? param->focusColorPair : param->colorPair;

/**
 * \struct form_input_win_t
 * \brief Structure pour un champ de formulaire encapsulant la fenetre ainsi que les paramètres
 */
typedef struct {
	WINDOW *win; /*!< Fenêtre du champ de formulaire */
	form_input_params_t params; /*!< Paramètres du champ de formulaire */
} form_input_win_t;

/**
 * \fn remove_char(char *str, int index)
 * \brief Suppression d'un caractère dans une chaîne de caractères
 * \details Cette fonction supprime un caractère dans une chaîne de caractères à l'index spécifié
 */
static void remove_char(char *str, int index);

/**
 * \fn add_char(char *str, int index, char c)
 * \brief Ajout d'un caractère dans une chaîne de caractères
 * \details Cette fonction ajoute un caractère dans une chaîne de caractères à l'index spécifié
 * \param str La chaîne de caractères
 * \param index L'index où ajouter le caractère
 * @warning La chaîne doit être suffisamment grande pour accueillir le caractère supplémentaire
 */
static void add_char(char *str, int index, char c);

/**
 * \fn ui_form_draw(ui_component_t *self)
 */
static void ui_form_draw(ui_component_t *self);

/**
 * \fn ui_form_handle_event(ui_component_t *self, event_t *event)
 * \brief Fonction de gestion des événements pour un composant de formulaire
 * \param self Le composant de formulaire
 * \param event L'événement à gérer
 * \return bool true si l'événement a été consommé, false sinon
 */
static bool ui_form_text_handle_event(ui_component_t *self, event_t *event);

/**
 * \fn ui_form_number_handle_event(ui_component_t *self, event_t *event)
 * \brief Gestion des événements pour un champ numérique
 */
static bool ui_form_number_handle_event(ui_component_t *self, event_t *event);

/**
 * \fn ui_form_destroy(ui_component_t *self)
 * \brief Méthode de destruction du composant
 */
static void ui_form_destroy(ui_component_t *self);

/**********************************************************************************************************************/
/*                                           Public Functions Definition                                              */
/**********************************************************************************************************************/

/**
 * \fn ui_component_t *ui_form_create_input_field(WINDOW *parent, int x, int y)
 * \brief Instancie un composant formulaire de base
 * \param parent La fenêtre ncurses parente
 * \param x Position X du champ
 * \param y Position Y du champ
 * \return ui_component_t* Le composant crée
 */
ui_component_t *ui_form_create_input_field(WINDOW *parent, int x, int y) {
    if(parent == NULL) return NULL;

    ui_component_t *comp = (ui_component_t *)malloc(sizeof(ui_component_t));
    CHECK_ALLOC(comp);

    form_input_win_t *field = (form_input_win_t *)malloc(sizeof(form_input_win_t));
    CHECK_ALLOC(field);

    field->win = derwin(parent, FORM_DEFAULT_HEIGHT, FORM_DEFAULT_WIDTH, y, x);
    keypad(field->win, TRUE);

    field->params.type = FORM_INPUT_UNKNOWN;
    field->params.x = x;
    field->params.y = y;
    field->params.isFocused = false;
    field->params.label = NULL;
    field->params.value = NULL;
    field->params.cursorPos = 0;
    field->params.focusColorPair = -1;
    field->params.colorPair = -1;

    comp->data = (void *)field;
    comp->isFocused = false;
    comp->draw = ui_form_draw;
    comp->handle_event = NULL;
    comp->destroy = ui_form_destroy;

    return comp;
}

/**
 * \fn void ui_form_set_input_label(ui_component_t *comp, const char *label)
 * \brief Définit le texte affiché sur la bordure supérieure du champ
 * \param comp Le composant formulaire
 * \param label La chaîne de caractères à afficher
 */
void ui_form_set_input_label(ui_component_t *comp, const char *label) {
    if(comp == NULL || comp->data == NULL) return;
    form_input_win_t *field = (form_input_win_t *)comp->data;
    if(field->params.label) free(field->params.label);
    field->params.label = strdup(label);

    int labelLen = strlen(label);
    int valueLen = 0;
    switch (field->params.type) {
        case FORM_INPUT_NUMBER: valueLen = snprintf(NULL, 0, "%d", field->params.number.maxValue); break;
        case FORM_INPUT_TEXT: valueLen = field->params.text.maxLength; break;
        default: break;
    }
    int newWidth = (labelLen > valueLen) ? labelLen : valueLen;
    newWidth += FORM_PADDING * 2;
    wresize(field->win, FORM_DEFAULT_HEIGHT, newWidth);
}

/**
 * \fn void ui_form_set_input_value(ui_component_t *comp, void *value)
 * \brief Relie le champ à la variable métier qui sera modifiée en temps réel
 * \param comp Le composant formulaire
 * \param value Pointeur vers la variable 
 * \warning Le type de la variable doit correspondre à la configuration du champ (char* ou int*)
 * \warning Le formulaire ne crée pas de copie de la variable, il faut s'assurer que la variable reste valide tant que le champ est utilisé
 */
void ui_form_set_input_value(ui_component_t *comp, void *value) {
    if(comp == NULL || comp->data == NULL) return;
    form_input_win_t *field = (form_input_win_t *)comp->data;
    field->params.value = value;
}

/**
 * \fn void ui_form_set_input_position(ui_component_t *comp, int x, int y)
 * \brief Déplace le champ dans sa fenêtre parente
 * \param comp Le composant formulaire
 * \param x Nouvelle position X
 * \param y Nouvelle position Y
 */
void ui_form_set_input_position(ui_component_t *comp, int x, int y) {
    if(comp == NULL || comp->data == NULL) return;
    form_input_win_t *field = (form_input_win_t *)comp->data;
    mvwin(field->win, y, x);
}

/**
 * \fn void ui_form_set_input_color(ui_component_t *comp, int colorPair)
 * \brief Définit la couleur du champ lorsqu'il n'a pas le focus
 * \param comp Le composant formulaire
 * \param fieldColorPair Identifiant ncurses de la paire de couleurs
 * \param focusColorPair Identifiant ncurses de la paire de couleurs lorsque le champ est sélectionné
 */
void ui_form_set_input_color(ui_component_t *comp, int fieldColorPair, int focusColorPair) {
    if(comp == NULL || comp->data == NULL) return;
    form_input_win_t *field = (form_input_win_t *)comp->data;
    field->params.colorPair = fieldColorPair;
    field->params.focusColorPair = focusColorPair;
}


/**
 * \fn void ui_form_configure_number_input(ui_component_t *comp, int minValue, int maxValue)
 * \brief Transforme le composant en champ de saisie numérique
 * \param comp Le composant formulaire
 * \param minValue Borne inférieure de la saisie
 * \param maxValue Borne supérieure de la saisie
 * \warning ui_form_set_input_value doit pointer vers un `int` valide
 */
void ui_form_configure_number_input(ui_component_t *comp, int minValue, int maxValue) {
    if (!comp || !comp->data) return;
    form_input_win_t *field = (form_input_win_t *)comp->data;

    field->params.type = FORM_INPUT_NUMBER;
    field->params.number.minValue = minValue;
    field->params.number.maxValue = maxValue;
    
    char numStr[MAX_NB_OF_DIGITS + 1];
    if (field->params.value) {
        snprintf(numStr, sizeof(numStr), "%d", *((int *)field->params.value));
        field->params.cursorPos = strlen(numStr);
    } else {
        field->params.cursorPos = 0;
    }

    comp->handle_event = ui_form_number_handle_event;
    
    char maxStr[MAX_NB_OF_DIGITS + 1];
    int lenMax = snprintf(maxStr, sizeof(maxStr), "%d", maxValue);
    int labelLen = field->params.label ? strlen(field->params.label) : 0;
    int neededWidth = (labelLen > lenMax ? labelLen : lenMax) + FORM_PADDING * 2;
    
    wresize(field->win, FORM_DEFAULT_HEIGHT, neededWidth);
}

/**
 * \fn void ui_form_configure_text_input(ui_component_t *comp, int maxLength, bool isPassword)
 * \brief Transforme le composant en champ de saisie de texte
 * \param comp Le composant formulaire
 * \param maxLength Longueur maximale autorisée (sans le \0)
 * \param isPassword Si true, masque les caractères avec des étoiles '*'
 * \warning ui_form_set_input_value doit pointer vers un `char*` valide
 */
void ui_form_configure_text_input(ui_component_t *comp, int maxLength, bool isPassword) {
    if (!comp || !comp->data) return;
    form_input_win_t *field = (form_input_win_t *)comp->data;

    field->params.type = FORM_INPUT_TEXT;
    field->params.text.maxLength = maxLength;
    field->params.text.isPassword = isPassword;
    
    if (field->params.value) {
        field->params.cursorPos = strlen((char *)field->params.value);
    } else {
        field->params.cursorPos = 0;
    }

    comp->handle_event = ui_form_text_handle_event;

    int labelLen = field->params.label ? strlen(field->params.label) : 0;
    int neededWidth = (labelLen > maxLength ? labelLen : maxLength) + FORM_PADDING * 2;
    
    wresize(field->win, FORM_DEFAULT_HEIGHT, neededWidth);
}

/**********************************************************************************************************************/
/*                                           Private Functions                                                        */
/**********************************************************************************************************************/

/**
 * \fn remove_char(char *str, int index)
 * \brief Suppression d'un caractère dans une chaîne de caractères
 * \details Cette fonction supprime un caractère dans une chaîne de caractères à l'index spécifié
 */
static void remove_char(char *str, int index) {
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
static void add_char(char *str, int index, char c) {
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



/**
 * \fn ui_form_draw(ui_component_t *self)
 */
static void ui_form_draw(ui_component_t *self) {
    if(!self || !self->data) return;

    form_input_win_t *field = (form_input_win_t *)self->data;
    form_input_params_t *params = &(field->params);
    WINDOW *win = field->win;

    wclear(win);
    int currentColor = GET_INPUT_FIELD_COLOR(self, params);
    if(currentColor != -1) wattron(win, COLOR_PAIR(currentColor));

	box(win, 0, 0);
	if(params->label != NULL) {
		mvwprintw(field->win, 0, 1, "%s", params->label);
	}

    // Déactivation de la couleur
    if(currentColor != -1) wattroff(win, COLOR_PAIR(currentColor));

    mvwprintw(win, 1, FORM_PADDING, "%*s", getmaxx(win) - FORM_PADDING * 2, "");

    // Affichage de la valeur du champs de saisie spécifique au type
    switch (params->type) {
        case FORM_INPUT_TEXT:
            char *strVal = (char *)params->value;
            if(params->text.isPassword) {
                for(size_t i = 0; i < strlen(strVal); i++) {
                    mvwprintw(win, 1, FORM_PADDING + i, "*");
                }
            }
            else {
                mvwprintw(win, 1, FORM_PADDING, "%s", strVal);
            }

            if (self->isFocused) {
                curs_set(1);
                wmove(win, 1, FORM_PADDING + params->cursorPos);
            }
        break;

        case FORM_INPUT_NUMBER:
            char numStr[MAX_NB_OF_DIGITS + 1];
            snprintf(numStr, sizeof(numStr), "%d", *((int *)params->value));
            mvwprintw(win, 1, FORM_PADDING, "%s", numStr);

            if(self->isFocused) {
                curs_set(1);
                wmove(win, 1, FORM_PADDING + params->cursorPos);
                leaveok(win, FALSE);
            }
            else {
                leaveok(win, TRUE);
            }
        break;

        default: break;
    }
    wnoutrefresh(field->win);
}

static bool ui_form_text_handle_event(ui_component_t *self, event_t *event) {
    if(event->type != UI_EVENT_KEY_PRESSED) return false;

    int keypressed = (int)(intptr_t)event->data;
    form_input_win_t *field = (form_input_win_t *)self->data;
    form_input_params_t *params = &(field->params);
    char *strVal = (char*)params->value;
    int lenValue = strlen(strVal);
    int *cursorPos = &params->cursorPos;
    
    switch(keypressed) {
        case KEY_LEFT:
            if(*cursorPos > 0) (*cursorPos)--;
        break;

        case KEY_RIGHT:
            if(*cursorPos < lenValue) (*cursorPos)++;
        break;

        case KEY_BACKSPACE:
        case 127: // DEL
            if(lenValue > 0 && *cursorPos > 0) {
                (*cursorPos)--;
                remove_char(strVal, *cursorPos);
            }
        break;

        case KEY_DC: // Delete
            if(lenValue > 0 && *cursorPos < lenValue) {
                remove_char(strVal, *cursorPos);
            }
        break;

        default:
            if(lenValue < params->text.maxLength && isprint(keypressed)) {
                add_char(strVal, *cursorPos, keypressed);
                (*cursorPos)++;
            }
        break;
    }
    return true;
}

/**
 * \fn _ui_form_number_handle_event(ui_component_t *self, event_t *event)
 * \brief Gestion des événements pour un champ numérique (inclus la souris)
 */
static bool ui_form_number_handle_event(ui_component_t *self, event_t *event) {
    if(event->type != UI_EVENT_KEY_PRESSED) return false;

    int keypressed = (int)(intptr_t)event->data;
    form_input_win_t *field = (form_input_win_t *)self->data;
    form_input_params_t *params = &(field->params);

    int *numVal = (int *)params->value;
    int *cursorPos = &params->cursorPos;

    char numStr[MAX_NB_OF_DIGITS + 1];
    snprintf(numStr, sizeof(numStr), "%d", *numVal);
    int lenValue = strlen(numStr);
    
    char maxValStr[MAX_NB_OF_DIGITS + 1];
    snprintf(maxValStr, sizeof(maxValStr), "%d", params->number.maxValue);
    int maxLen = strlen(maxValStr);

    switch(keypressed) {
        case KEY_LEFT: if(*cursorPos > 0) (*cursorPos)--; break;
        case KEY_RIGHT: if(*cursorPos < lenValue) (*cursorPos)++; break;

        case KEY_BACKSPACE:
        case 127:
            if(lenValue > 0 && *cursorPos > 0) {
                (*cursorPos)--;
                remove_char(numStr, *cursorPos);
                *numVal = atoi(numStr);
            }
        break;

        case KEY_DC:
            if(lenValue > 0 && *cursorPos < lenValue) {
                remove_char(numStr, *cursorPos);
                *numVal = atoi(numStr);
                if(*cursorPos == lenValue - 1 && *cursorPos > 0) (*cursorPos)--;
            }
        break;

        case KEY_UP:
            if(*numVal < params->number.maxValue) {
                (*numVal)++;
                snprintf(numStr, sizeof(numStr), "%d", *numVal);
                if(strlen(numStr) > maxLen) *numVal = atoi(maxValStr);
            }
        break;

        case KEY_DOWN:
            if(*numVal > params->number.minValue) {
                (*numVal)--;
                snprintf(numStr, sizeof(numStr), "%d", *numVal);
                *cursorPos = strlen(numStr);
            }
        break;

        case KEY_MOUSE: {
            MEVENT mevent;
            if(getmouse(&mevent) == OK) {
                if(mevent.bstate & BUTTON1_PRESSED) {
                    if(*numVal < params->number.maxValue) {
                        (*numVal)++;
                        snprintf(numStr, sizeof(numStr), "%d", *numVal);
                        *cursorPos = strlen(numStr);
                    }
                }
                else if(mevent.bstate & BUTTON3_PRESSED) {
                    if(*numVal > params->number.minValue) {
                        (*numVal)--;

                        snprintf(numStr, sizeof(numStr), "%d", *numVal);
                        *cursorPos = strlen(numStr);
                    }
                }
            }
            break;
        }

        default:
            if(lenValue < maxLen && isdigit(keypressed)) {
                add_char(numStr, *cursorPos, keypressed);
                int newVal = atoi(numStr);
                if(newVal <= params->number.maxValue) {
                    *numVal = newVal;
                    (*cursorPos)++;
                }
                *cursorPos = strlen(numStr);
            }
        break;
    }
    return true;
}

/**
 * \fn _ui_form_destroy(ui_component_t *self)
 * \brief Méthode de destruction du composant
 */
static void ui_form_destroy(ui_component_t *self) {
    if(!self || !self->data) return;

    form_input_win_t *field = (form_input_win_t *)self->data;
    delwin(field->win);
    free(field->params.label);
    free(field);
}