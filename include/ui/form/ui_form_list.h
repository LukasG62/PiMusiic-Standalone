/**
 * @file ui_form_list.h
 * @brief Fichier d'en-tête pour les listes interactives de l'interface graphique
 * @version 1.0
 */

#ifndef UI_FORM_LIST_H
#define UI_FORM_LIST_H

#include "ui/base/ui_common.h" //!< Inclusion des macros communes à l'interface graphique
#include "ui/base/ui_component.h" //!< Inclusion des composants de l'interface graphique
#include "event/event.h" //!< Inclusion de la structure event_t

/**
 * \brief Signature de la fonction de rappel (callback) quand un élément de la liste est sélectionné
 * \param context Pointeur générique pour passer des données
 * \param selectedValue La valeur associée à l'élément sélectionné
 */
typedef void (*ui_form_list_on_select_fn)(void *context, void *selectedValue);

/**
 * \fn ui_form_list_create(WINDOW *parent, int x, int y, int width, int height)
 * \brief Crée un composant de liste interactive
 * \param parent la fenêtre sur laquelle créer la liste
 * \param x la position x de la liste
 * \param y la position y de la liste
 * \param width la largeur de la liste
 * \param height la hauteur de la liste
 * @return ui_component_t* Le composant de liste créée
 */
ui_component_t *ui_form_list_create(WINDOW *parent, int x, int y, int width, int height);

/**
 * @fn void ui_form_list_set_items(ui_component_t *comp, const char **labels, void **values, int count);
 * @brief définit les éléments de la liste
 * @param comp Le composant de liste à configurer
 * @param labels Tableau de chaînes de caractères à afficher dans la liste
 * @param values Tableau de pointeurs vers les valeurs associées à chaque élément (ex:
 * @param labels Tableau de chaînes de caractères (ce qui est affiché)
 * @param values Tableau de pointeurs vers les valeurs associées
 * @param count Le nombre d'éléments dans les tableaux labels et values
 * 
 */
void ui_form_list_set_items(ui_component_t *comp, const char **labels, void **values, int count);

/**
 * @fn void ui_form_list_set_on_select(ui_component_t *comp, ui_form_list_on_select_fn callback, void *context)
 * @brief Définit l'action à exécuter lorsqu'un élément de la liste est sélectionné
 * @param comp Le composant de liste à configurer
 * @param callback La fonction de rappel à appeler lors de la sélection
 * @param context Le contexte à passer à la fonction de rappel
 */
void ui_form_list_set_on_select(ui_component_t *comp, ui_form_list_on_select_fn callback, void *context);

#endif // UI_FORM_LIST_H