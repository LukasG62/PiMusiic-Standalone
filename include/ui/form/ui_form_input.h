/**
 * @file ui_form.h
 * @brief Définition des composants interactifs
 * @author Lukas Grando
 */

#ifndef UI_FORM_H
#define UI_FORM_H

#include "ui/base/ui_common.h"
#include "ui/base/ui_component.h"
#include "event/event.h"

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

/**
 * \struct form_input_text_params_t
 * \brief Paramètres spécifiques pour un champ de type texte
 */
typedef struct {
    bool isPassword; /*!< Indique si le champ est un mot de passe */
    int maxLength; /*!< Longueur maximale du champ */
} form_input_text_params_t;

/**
 * \struct form_input_number_params_t
 * \brief Paramètres spécifiques pour un champ de type numérique
 */
typedef struct {
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
    int cursorPos; /*!< Position du curseur pour la saisie */
    union {
        form_input_text_params_t text; /*!< Paramètres pour un champ de texte */
    	form_input_number_params_t number; /*!< Paramètres pour un champ numérique */
    }; /*!< Paramètres spécifiques au type de champ */
    int maxLength; /*!< Longueur maximale du champ */
    int focusColorPair; /*!< Paire de couleurs pour le focus */
    int colorPair; /*!< Paire de couleurs par défaut (champs texte et contour (unfocus)) */
} form_input_params_t;

/**
 * \fn ui_component_t *ui_form_create_input_field(WINDOW *parent, int x, int y)
 * \brief Instancie un composant formulaire de base (non configuré)
 * \param parent La fenêtre ncurses parente
 * \param x Position X du champ
 * \param y Position Y du champ
 * \return ui_component_t* Le composant générique prêt à être paramétré
 */
ui_component_t *ui_form_create_input_field(WINDOW *parent, int x, int y);

/**
 * \fn void ui_form_set_input_label(ui_component_t *comp, const char *label)
 * \brief Définit le texte affiché sur la bordure supérieure du champ
 * \param comp Le composant formulaire
 * \param label La chaîne de caractères à afficher
 */
void ui_form_set_input_label(ui_component_t *comp, const char *label);

/**
 * \fn void ui_form_set_input_value(ui_component_t *comp, void *value)
 * \brief Relie le champ à la variable métier qui sera modifiée en temps réel
 * \param comp Le composant formulaire
 * \param value Pointeur vers la variable (char* ou int* selon la configuration)
 */
void ui_form_set_input_value(ui_component_t *comp, void *value);

/**
 * \fn void ui_form_set_input_position(ui_component_t *comp, int x, int y)
 * \brief Déplace le champ dans sa fenêtre parente
 * \param comp Le composant formulaire
 * \param x Nouvelle position X
 * \param y Nouvelle position Y
 */
void ui_form_set_input_position(ui_component_t *comp, int x, int y);

/**
 * \fn void ui_form_set_input_color(ui_component_t *comp, int colorPair)
 * \brief Définit la couleur du champ lorsqu'il n'a pas le focus
 * \param comp Le composant formulaire
 * \param fieldColorPair Identifiant ncurses de la paire de couleurs
 * \param focusColorPair Identifiant ncurses de la paire de couleurs lorsque le champ est sélectionné
 */
void ui_form_set_input_color(ui_component_t *comp, int fieldColorPair, int focusColorPair);


/**
 * \fn void ui_form_configure_number_input(ui_component_t *comp, int minValue, int maxValue)
 * \brief Transforme le composant en champ de saisie numérique
 * \param comp Le composant formulaire
 * \param minValue Borne inférieure de la saisie
 * \param maxValue Borne supérieure de la saisie
 * \warning ui_form_set_input_value doit pointer vers un `int` valide
 */
void ui_form_configure_number_input(ui_component_t *comp, int minValue, int maxValue);

/**
 * \fn void ui_form_configure_text_input(ui_component_t *comp, int maxLength, bool isPassword)
 * \brief Transforme le composant en champ de saisie de texte
 * \param comp Le composant formulaire
 * \param maxLength Longueur maximale autorisée (sans le \0)
 * \param isPassword Si true, masque les caractères avec des étoiles '*'
 * \warning ui_form_set_input_value doit pointer vers un `char*` valide
 */
void ui_form_configure_text_input(ui_component_t *comp, int maxLength, bool isPassword);

#endif // UI_FORM_H