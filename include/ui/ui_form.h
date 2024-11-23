
/**
 * @file ui_form.H
 * @brief Définition des composants interactifs
 * @author Lukas Grando
 */

#ifndef UI_FORM_H
#define UI_FORM_H

#include "ui_common.h"

#define PARENT_WIN(field) (field->win->_parent)

#define FORM_DEFAULT_HEIGHT 3 /*!< Hauteur d'un champ de formulaire */
#define FORM_DEFAULT_WIDTH 20 /*!< Largeur par défaut d'un champ de formulaire */
#define FORM_PADDING 2 /*!< Marge intérieure d'un champ de formulaire */

// todo: calculer la longueur max selon la taille d'un int sur l'architecture de la machine
#define MAX_NB_OF_DIGITS 10 /*!< Nombre maximum de chiffres pour un champ numérique */


/**
 * \enum form_input_type_t
 * \brief Enumération des types de champs de formulaire
 */
typedef enum {
    FORM_INPUT_UNKNOWN, /*!< Champ inconnu */
    FORM_INPUT_TEXT, /*!< Champ de texte */
    FORM_INPUT_NUMBER, /*!< Champ numérique */
    FORM_INPUT_DATE, /*!< Champ de date */
    FORM_INPUT_TIME, /*!< Champ d'heure */
    FORM_INPUT_PASSWORD /*!< Champ de mot de passe */
} form_input_type_t;

typedef struct {
    char *value; /*!< Valeur du champ */
    bool isPassword; /*!< Indique si le champ est un mot de passe */
    int maxLength; /*!< Longueur maximale du champ */
} form_input_text_params_t;

typedef struct {
    int *value; /*!< Valeur du champ */
    int minValue; /*!< Valeur minimale du champ */
    int maxValue; /*!< Valeur maximale du champ */
} form_input_number_params_t;

/**
 * \struct form_input_params_t
 * \brief Structure pour les paramètres d'un champ de formulaire
 * \details Cette structure permet de définir les paramètres d'un champ de formulaire
 */
typedef struct {
    form_input_type_t type; /*!< Type du champ */
    int x; /*!< Position X du champ */
    int y; /*!< Position Y du champ */
    char isFocused; /*!< Indique si le champ est en focus */
    char *label; /*!< Label du champ */
    void *value; /*!< Valeur du champ */
    union {
        form_input_text_params_t text; /*!< Paramètres pour un champ de texte */
    	form_input_number_params_t number; /*!< Paramètres pour un champ numérique */
    }; /*!< Paramètres spécifiques au type de champ */
    int maxLength; /*!< Longueur maximale du champ */
    int focusColorPair; /*!< Paire de couleurs pour le focus */
    int colorPair; /*!< Paire de couleurs par défaut (champs texte et contour (unfocus)) */
} form_input_params_t;

/**
 * \struct form_input_win_t
 * \brief Structure pour un champ de formulaire encapsulant la fenetre ainsi que les paramètres
 */
typedef struct {
	WINDOW *win; /*!< Fenêtre du champ de formulaire */
	form_input_params_t *params; /*!< Paramètres du champ de formulaire */
} form_input_win_t;


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
form_input_win_t *create_input_field(WINDOW *parent, int x, int y);

/**
 * \fn void destroy_input_field(form_input_win_t *field)
 * \brief Destruction d'un champ de saisie
 * \details Cette fonction détruit un champ de saisie en libérant la mémoire allouée
 */
void destroy_input_field(form_input_win_t *field);

/**
 * \fn void display_input_field(form_input_win_t *field)
 * \brief Affichage d'un champ de saisie
 * \details Cette fonction affiche un champ de saisie
 * \param field Le champ de saisie à afficher
 */
void display_input_field(form_input_win_t *field);

/**
 * \fn handle_input_field(form_input_win_t *field, int escapeKey)
 * \brief Gestion des touches pour un champ de saisie
 * \note appel bloquant 
 */
void handle_input_field(form_input_win_t *field, int escapeKey);

/**
 * \fn void focus_input_field(form_input_win_t **field)
 * \brief Focus sur un champ de saisie
 * \details Cette fonction met en focus un champ de saisie
 * \param field Le champ de saisie à mettre en focus
 * \note Effective seulement après un refresh 
 * @see display_input_field
 */
void focus_input_field(form_input_win_t *field);

/**
 * \fn void unfocus_input_field(form_input_win_t **field)
 * \brief Enlève le focus d'un champ de saisie
 * \details Cette fonction enlève le focus d'un champ de saisie
 * \param field Le champ de saisie à enlever le focus
 * \note Effective seulement après un refresh 
 * @see display_input_field
 */
void unfocus_input_field(form_input_win_t *field);

/**
 * \fn set_input_label(form_input_win_t *field, char *label)
 * \brief Définir le label d'un champ de saisie
 * \details Cette fonction définit le label d'un champ de saisie
 * \param field Le champ de saisie
 * \param label Le label à définir
 * \note Effective seulement après un refresh
 */
void set_input_label(form_input_win_t *field, char *label);

/**
 * \fn set_input_value(form_input_win_t *field, void *value)
 * \brief Définir la variable qui contiendra la valeur du champ de saisir
 * \param field Le champ de saisie
 * \param value pointeur vers la variable qui contiendra la valeur du champ de saisir
 */
void set_input_value(form_input_win_t *field, void *value);

/**
 * \fn set_input_position(form_input_win_t *field, int x, int y)
 * \brief Définir la position d'un champ de saisie
 * \param field Le champ de saisie
 * \param x Position X
 * \param y Position Y
 * \note Effective seulement après un refresh
 * \note La position est relative à la fenêtre parent du champ de saisie
 */
void set_input_position(form_input_win_t *field, int x, int y);

/**
 * \fn set_input_color(form_input_win_t *field, int colorPair)
 * \brief Définir la paire de couleurs d'un champ de saisie
 * \param field Le champ de saisie
 * \param colorPair La paire de couleurs à définir
 * \note Effective seulement après un refresh
 */
void set_input_color(form_input_win_t *field, int colorPair);

/**
 * \fn set_input_focus_color(form_input_win_t *field, int colorPair)
 * \brief Définir la paire de couleurs du focus d'un champ de saisie
 * \param field Le champ de saisie
 * \param colorPair La paire de couleurs à définir
 * \note Effective seulement après un refresh
 */
void set_input_focus_color(form_input_win_t *field, int colorPair);

/**
 * \fn configure_number_input(form_input_win_t *field, int minValue, int maxValue)
 * \brief configurer un champ de saisie numérique
 * \param field Le champ de saisie
 * \param minValue Valeur minimale
 * \param maxValue Valeur maximale
 * \warning value doit être un pointeur vers un entier valide
 */
void configure_number_input(form_input_win_t *field, int minValue, int maxValue);

/**
 * \fn configure_text_input(form_input_win_t *field, int maxLength)
 * \brief configurer un champ de saisie de texte
 * \param field Le champ de saisie
 * \param maxLength Longueur maximale
 * \warning value doit être un pointeur vers une chaîne de caractères valide
 * \warning Elle ne vérifie pas la longueur max réelle de la chaine. Elle doit être >= maxLength
 * \note La longueur maximale est définie par maxLength
 */
void configure_text_input(form_input_win_t *field, int maxLength, bool isPassword);

#endif // UI_FORM_H