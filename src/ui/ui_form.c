/**
 * @file ui_form.c
 * @brief Définition des fonctions pour manipuler les formulaires
 */

#include "ui/ui_form.h"

/**********************************************************************************************************************/
/*                                           Private Functions                                                        */
/**********************************************************************************************************************/

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
 * @warning La chaîne doit être suffisamment grande pour accueillir le caractère supplémentaire
 */
void add_char(char *str, int index, char c);

/**
 * \fn handle_text_input(form_input_win_t *field, int escapeKey)
 * \brief Gestion des touches pour un champ de texte
 * \details Cette fonction gère la logique de saisie pour un champ de texte
 * \param field Le champ de texte
 * \param escapeKey La touche pour quitter la saisie
 * \note Cette fonction est appelée par handle_input_field
 * @see handle_input_field
 */
void handle_text_input(form_input_win_t *field, int escapeKey);

/**
 * \fn handle_number_input(form_input_win_t *field, int escapeKey)
 * \brief Gestion des touches pour un champ de formulaire numérique
 * \details Cette fonction gère la logique de saisie pour un champ de formulaire numériques
 */
void handle_number_input(form_input_win_t *field, int escapeKey);

/**********************************************************************************************************************/
/*                                           Public Functions Definition                                              */
/**********************************************************************************************************************/

/**
 * \fn WINDOW *create_input_field(WINDOW **win, WINDOW *parent,  form_input_params_t *params)
 * \brief Création d'un champ de saisie
 * \details Cette fonction crée un champ de saisie en affichant un label et la valeur saisie
 * Elle ne s'occupe pas de la gestion des touches ni de la saisie seulement de l'affichage
 * \param win La fenêtre du champ de saisie
 * \param params Les paramètres du champ de saisie
 * \param parent La fenêtre parent du champ de saisie
 * \param x Position X du champ de saisie
 * \param y Position Y du champ de saisie
 * \return WINDOW* La fenêtre du champ de saisie
 * \note Si le pointeur win est NULL, la fonction crée une nouvelle fenêtre et l'assigne à win
 * \warning La fenêtre doit être détruite après utilisation
 */
form_input_win_t *create_input_field(WINDOW *parent, int x, int y) {
	form_input_win_t *field = (form_input_win_t *)malloc(sizeof(form_input_win_t));
	CHECK_ALLOC(field);
	if(parent == NULL) {
        ("Parent window is NULL");
		return NULL; // Si la fenêtre parent est NULL, on ne peut pas créer le champ de saisie
	}
    // Création de la fenêtre du champ de saisie
	WINDOW *fieldWin = derwin(parent, FORM_DEFAULT_HEIGHT, FORM_DEFAULT_WIDTH, y, x);
    keypad(fieldWin, TRUE);
	CHECK_ALLOC(fieldWin);
	field->win = fieldWin;
	field->params = (form_input_params_t *)malloc(sizeof(form_input_params_t));
	CHECK_ALLOC(field->params);
	field->params->label = NULL;
	field->params->value = NULL;
	field->params->type = FORM_INPUT_UNKNOWN;
    // Aucune paire de couleurs définie (couleurs par défaut du parent)
    field->params->colorPair = -1;
    field->params->focusColorPair = -1;

    return field;
}

/**
 * \fn void destroy_input_field(form_input_win_t *field)
 * \brief Destruction d'un champ de saisie
 * \details Cette fonction détruit un champ de saisie en libérant la mémoire allouée
 */
void destroy_input_field(form_input_win_t *field) {
	if(field != NULL) {
		if(field->params != NULL) {
			free(field->params);
		}
		if(field->win != NULL) {
			delwin(field->win);
		}
		free(field);
	}
}

/**
 * \fn void display_input_field(form_input_win_t *field)
 * \brief Affichage d'un champ de saisie
 * \details Cette fonction affiche un champ de saisie
 * \param field Le champ de saisie à afficher
 */
void display_input_field(form_input_win_t *field) {
	if(field == NULL || field->params->type == FORM_INPUT_UNKNOWN) return;
	wclear(field->win);

	// Affichage du label
    if(field->params->isFocused && field->params->focusColorPair != -1) {
        wattron(field->win, COLOR_PAIR(field->params->focusColorPair));
    }
    else if(field->params->colorPair != -1) {
        wattron(field->win, COLOR_PAIR(field->params->colorPair));
    }

    // Creation de la bordure du champ de saisie
	box(field->win, 0, 0);
    // Affichage du label
	if(field->params->label != NULL) {
		mvwprintw(field->win, 0, 1, "%s", field->params->label);
	}

    // Déactivation de la couleur
    if(field->params->isFocused && field->params->focusColorPair != -1) {
        wattroff(field->win, COLOR_PAIR(field->params->focusColorPair));
    }
    else if(field->params->colorPair != -1) {
        wattroff(field->win, COLOR_PAIR(field->params->colorPair));
    }

    // Affichage de la valeur du champs de saisie spécifique au type
    if(field->params->value == NULL) {
        mvwprintw(field->win, 1, FORM_PADDING, "%*s", field->win->_maxx - FORM_PADDING, "");
    }

    switch (field->params->type)
	{
	case FORM_INPUT_NUMBER:
        if(field->params->text.isPassword) {
            // affichage des étoiles pour les mots de passe
            int len = strlen((char *)field->params->value);
            for(int i = 0; i < len; i++) {
                mvwprintw(field->win, 1, FORM_PADDING + i, "*");
            }
        }
        mvwprintw(field->win, 1, FORM_PADDING, "%d", *((int *)field->params->value));
		break;

	case FORM_INPUT_TEXT:
        mvwprintw(field->win, 1, FORM_PADDING, "%s", (char *)field->params->value);
		break;
	default:
		break;
	}

	wrefresh(field->win);
}

/**
 * \fn handle_input_field(form_input_win_t *field, int escapeKey)
 * \brief Gestion des touches pour un champ de saisie
 * \note appel bloquant 
 */
void handle_input_field(form_input_win_t *field, int escapeKey) {
    if(field == NULL || field->params->type == FORM_INPUT_UNKNOWN) return;

    switch (field->params->type)
    {
    case FORM_INPUT_NUMBER:
        handle_number_input(field, escapeKey);
        break;
    case FORM_INPUT_TEXT:
        handle_text_input(field, escapeKey);
        break;
    }
}

/**
 * \fn void focus_input_field(form_input_win_t **field)
 * \brief Focus sur un champ de saisie
 * \details Cette fonction met en focus un champ de saisie
 * \param field Le champ de saisie à mettre en focus
 * \note Effective seulement après un refresh 
 * @see display_input_field
 */
void focus_input_field(form_input_win_t *field) {
	if(field == NULL) return;
	field->params->isFocused = true;
}

/**
 * \fn void unfocus_input_field(form_input_win_t **field)
 * \brief Enlève le focus d'un champ de saisie
 * \details Cette fonction enlève le focus d'un champ de saisie
 * \param field Le champ de saisie à enlever le focus
 * \note Effective seulement après un refresh 
 * @see display_input_field
 */
void unfocus_input_field(form_input_win_t *field) {
	if(field == NULL) return;
	field->params->isFocused = false;
}

/**
 * \fn set_input_label(form_input_win_t *field, char *label)
 * \brief Définir le label d'un champ de saisie
 * \details Cette fonction définit le label d'un champ de saisie
 * \param field Le champ de saisie
 * \param label Le label à définir
 * \note Effective seulement après un refresh
 */
void set_input_label(form_input_win_t *field, char *label) {
	if(field == NULL) return;
	field->params->label = label;
    
    // Mettre à jour si nécessaire la taille du champ
    int labelLen = strlen(label);
    int valueLen = 0;
    switch (field->params->type)
    {
        case FORM_INPUT_NUMBER:
            valueLen = snprintf(NULL, 0, "%d", field->params->number.maxValue);
        break;

        case FORM_INPUT_TEXT:
            valueLen = field->params->text.maxLength;
        break;

        default:
            break;
    }
    int newWidth = (labelLen > valueLen) ? labelLen : valueLen;
    newWidth += FORM_PADDING * 2; // padding
    wresize(field->win, FORM_DEFAULT_HEIGHT, newWidth);
}

/**
 * \fn set_input_value(form_input_win_t *field, void *value)
 * \brief Définir la variable qui contiendra la valeur du champ de saisir
 * \param field Le champ de saisie
 * \param value pointeur vers la variable qui contiendra la valeur du champ de saisir
 */
void set_input_value(form_input_win_t *field, void *value) {
	if(field == NULL) return;
	field->params->value = value;
}

/**
 * \fn set_input_position(form_input_win_t *field, int x, int y)
 * \brief Définir la position d'un champ de saisie
 * \param field Le champ de saisie
 * \param x Position X
 * \param y Position Y
 * \note Effective seulement après un refresh
 * \note La position est relative à la fenêtre parent du champ de saisie
 */
void set_input_position(form_input_win_t *field, int x, int y) {
	if(field == NULL) return;
	mvwin(field->win, y, x);
}

/**
 * \fn set_input_color(form_input_win_t *field, int colorPair)
 * \brief Définir la paire de couleurs d'un champ de saisie
 * \param field Le champ de saisie
 * \param colorPair La paire de couleurs à définir
 * \note Effective seulement après un refresh
 */
void set_input_color(form_input_win_t *field, int colorPair) {
    if(field == NULL) return;
    field->params->colorPair = colorPair;
}

/**
 * \fn set_input_focus_color(form_input_win_t *field, int colorPair)
 * \brief Définir la paire de couleurs du focus d'un champ de saisie
 * \param field Le champ de saisie
 * \param colorPair La paire de couleurs à définir
 * \note Effective seulement après un refresh
 */
void set_input_focus_color(form_input_win_t *field, int colorPair) {
    if(field == NULL) return;
    field->params->focusColorPair = colorPair;
}

/**
 * \fn configure_number_input(form_input_win_t *field, int minValue, int maxValue)
 * \brief configurer un champ de saisie numérique
 * \param field Le champ de saisie
 * \param minValue Valeur minimale
 * \param maxValue Valeur maximale
 * \warning value doit être un pointeur vers un entier valide
 */
void configure_number_input(form_input_win_t *field, int minValue, int maxValue) {
	if(field == NULL) return;
	field->params->type = FORM_INPUT_NUMBER;
	field->params->number.maxValue = maxValue;
	field->params->number.minValue = minValue;
}

/**
 * \fn configure_text_input(form_input_win_t *field, int maxLength)
 * \brief configurer un champ de saisie de texte
 * \param field Le champ de saisie
 * \param maxLength Longueur maximale
 * \warning value doit être un pointeur vers une chaîne de caractères valide
 * \warning Elle ne vérifie pas la longueur max réelle de la chaine. Elle doit être >= maxLength
 * \note La longueur maximale est définie par maxLength
 */
void configure_text_input(form_input_win_t *field, int maxLength, bool isPassword) {
	if(field == NULL) return;

	field->params->type = FORM_INPUT_TEXT;
	field->params->text.maxLength = maxLength;
	field->params->text.isPassword = isPassword;

    // On met à jour la taille du field si la taille max de la valeur est plus grande que la taille actuelle
    int valueLen = field->params->text.maxLength;
    if(field->params->label != NULL) {
        int labelLen = strlen(field->params->label);
        valueLen = (valueLen > labelLen) ? valueLen : labelLen;
    }
    valueLen += FORM_PADDING * 2; // padding
    wresize(field->win, FORM_DEFAULT_HEIGHT, valueLen);
}


/**********************************************************************************************************************/
/*                                           Private Functions                                                        */
/**********************************************************************************************************************/

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


/**
 * \fn handle_text_input(form_input_win_t *field, int escapeKey)
 * \brief Gestion des touches pour un champ de texte
 * \details Cette fonction gère la logique de saisie pour un champ de texte
 * \param field Le champ de texte
 * \param escapeKey La touche pour quitter la saisie
 * \note Cette fonction est appelée par handle_input_field
 * @see handle_input_field
 */
void handle_text_input(form_input_win_t *field, int escapeKey) {
    if(field == NULL || field->params->type != FORM_INPUT_TEXT) return;
    if(field->params->isFocused == false) return;
    int lenValue = strlen((char *)field->params->value);
    int cursorPos = lenValue + FORM_PADDING;

    // Affichage du champs de saisie avec le curseur
    display_input_field(field);
    wmove(field->win, 1, cursorPos);
    while(field->params->isFocused) {
        curs_set(1);
        wrefresh(field->win);
        int keypressed = wgetch(field->win);
        if(keypressed == escapeKey) {
            unfocus_input_field(field);
            break;
        }

        switch(keypressed) {
            case KEY_LEFT:
                if(cursorPos > FORM_PADDING) cursorPos--;
            break;

            case KEY_RIGHT:
                if(cursorPos < lenValue + FORM_PADDING) cursorPos++;
            break;

            case KEY_BACKSPACE:
                // suppression du caractère précédent si possible (chaine et affichage)
                if(lenValue > 0 && cursorPos > FORM_PADDING) {
                    cursorPos--;
                    remove_char((char *)field->params->value, cursorPos - 1);
                    lenValue--;
                }
            break;

            case KEY_DC:
                // suppression du caractère sous le curseur (chaine et affichage)
                if(lenValue > 0 && cursorPos < field->win->_maxx - 1) {
                    remove_char((char *)field->params->value, cursorPos - 2);
                    lenValue--;
                    if(cursorPos == lenValue + FORM_PADDING) {
                        cursorPos--;
                    }
                }
            break;

            case KEY_ENTER:
                unfocus_input_field(field);
            break;

            default:
                // ajout du caractère à la position du curseur si imprimable (chaine et affichage)
                if(lenValue < field->params->text.maxLength && isprint(keypressed)) {
                    add_char((char *)field->params->value, cursorPos - 2, keypressed);
                    lenValue++;
                    cursorPos++;
                }
            break;
        }
        // réaffichage de la valeur du champ
        // ici on evite de réappeler display_input_field pour eviter les opérations inutiles
        mvwprintw(field->win, 1, FORM_PADDING, "%*s", field->win->_maxx - FORM_PADDING, " ");
        if(field->params->text.isPassword) {
            // affichage des étoiles pour les mots de passe
            for(int i = 0; i < lenValue; i++) {
                mvwprintw(field->win, 1, FORM_PADDING + i, "*");
            }
        }
        else {
            mvwprintw(field->win, 1, FORM_PADDING, "%s", (char *)field->params->value);
        }
        wmove(field->win, 1, cursorPos);
    }
    // désactivation du curseur
    curs_set(0);
    display_input_field(field);
}

/**
 * \fn handle_number_input(form_input_win_t *field, int escapeKey)
 * \brief Gestion des touches pour un champ de formulaire numérique
 * \details Cette fonction gère la logique de saisie pour un champ de formulaire numériques
 */
void handle_number_input(form_input_win_t *field, int escapeKey) {
    if(field == NULL || field->params->type != FORM_INPUT_NUMBER) return;
    if(field->params->isFocused == false) return;
    char value2str[MAX_NB_OF_DIGITS + 1];
    char max2str[MAX_NB_OF_DIGITS + 1];
    snprintf(value2str, MAX_NB_OF_DIGITS + 1, "%d", *((int *)field->params->value));
    int lenValue = strlen(value2str);
    int lenMax = snprintf(max2str, MAX_NB_OF_DIGITS + 1, "%d", field->params->number.maxValue);
    int cursorPos = lenValue + FORM_PADDING-1;

    // Affichage du champs de saisie avec le curseur
    display_input_field(field);
    wmove(field->win, 1, cursorPos);
    while(field->params->isFocused) {
        curs_set(1);
        wrefresh(field->win);
        int keypressed = wgetch(field->win);
        if(keypressed == escapeKey) {
            unfocus_input_field(field);
            break;
        }

        switch(keypressed) {
            case KEY_LEFT:
                if(cursorPos > FORM_PADDING) cursorPos--;
            break;

            case KEY_RIGHT:
                if(cursorPos < lenValue + FORM_PADDING-1) cursorPos++;
            break;

            case KEY_BACKSPACE:
                // suppression du caractère précédent si possible (chaine et affichage)
                if(lenValue > 0 && cursorPos > FORM_PADDING) {
                    cursorPos--;
                    remove_char(value2str, cursorPos - 1);
                    lenValue--;
                    *((int *)field->params->value) = atoi(value2str);
                }
            break;

            case KEY_UP:
                // incrémentation de la valeur si possible
                if(*((int *)field->params->value) < field->params->number.maxValue) {
                    (*((int *)field->params->value))++;
                    snprintf(value2str, MAX_NB_OF_DIGITS + 1, "%d", *((int *)field->params->value));
                    lenValue = strlen(value2str);
                    cursorPos = lenValue + 1;
                }
            break;

            case KEY_DOWN:
                // décrémentation de la valeur si possible
                if(*((int *)field->params->value) > field->params->number.minValue) {
                    (*((int *)field->params->value))--;
                    snprintf(value2str, MAX_NB_OF_DIGITS + 1, "%d", *((int *)field->params->value));
                    lenValue = strlen(value2str);
                    cursorPos = lenValue + 1;
                }
            break;

            case KEY_DC:
                // suppression du caractère sous le curseur (chaine et affichage)
                if(lenValue > 0 && cursorPos < lenValue + FORM_PADDING) {
                    remove_char((char *)field->params->value, cursorPos - 1);
                    lenValue--;
                    if(cursorPos == lenValue + 1) {
                        cursorPos--;
                    }
                }
            break;

            case KEY_ENTER:
                unfocus_input_field(field);
            break;

            default:
                // Gestion de la molette de la souris pour inc / dec la valeur
                if(keypressed == KEY_MOUSE) {
                    MEVENT event;
                    if(getmouse(&event) == OK) {
                        if(event.bstate & BUTTON1_PRESSED) {
                            if(*((int *)field->params->value) < field->params->number.maxValue) {
                                (*((int *)field->params->value))++;
                                snprintf(value2str, MAX_NB_OF_DIGITS + 1, "%d", *((int *)field->params->value));
                                lenValue = strlen(value2str);
                                cursorPos = lenValue + 1;
                            }
                        }
                        else if(event.bstate & BUTTON3_PRESSED) {
                            if(*((int *)field->params->value) > field->params->number.minValue) {
                                (*((int *)field->params->value))--;
                                snprintf(value2str, MAX_NB_OF_DIGITS + 1, "%d", *((int *)field->params->value));
                                lenValue = strlen(value2str);
                                cursorPos = lenValue + 1;
                            }
                        }
                    }
                }
                // ajout du caractère à la position du curseur
                if(lenValue < lenMax && isdigit(keypressed)) {
                    add_char(value2str, cursorPos - 1, keypressed);
                    int temp = atoi(value2str);
                    if(temp <= field->params->number.maxValue) {
                        *((int *)field->params->value) = temp;
                    }
                    else {
                        snprintf(value2str, MAX_NB_OF_DIGITS + 1, "%d", *((int *)field->params->value));
                    }
                    lenValue++;
                    cursorPos++;
                }
            break;
        }
        // réaffichage de la valeur du champ
        // ici on evite de réappeler display_input_field pour eviter les opérations inutiles
        mvwprintw(field->win, 1, FORM_PADDING, "%*s", field->win->_maxx - 2, " ");
        mvwprintw(field->win, 1, FORM_PADDING, "%s", value2str);
        wmove(field->win, 1, cursorPos);
    }
    // désactivation du curseur
    curs_set(0);
    display_input_field(field);
}