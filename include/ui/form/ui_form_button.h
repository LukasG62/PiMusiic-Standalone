/**
 * \file ui_form_button.h
 * \brief Fichier d'en-tête pour les boutons de formulaire de l'interface graphique
 * \version 1.0
 * \author Lukas Grando
 */

#ifndef UI_FORM_BUTTON_H
#define UI_FORM_BUTTON_H

#include "ui/base/ui_common.h" //!< Inclusion des macros communes à l'interface graphique
#include "ui/base/ui_component.h" //!< Inclusion des composants de l'interface graphique
#include "event/event.h" //!< Inclusion de la structure event_t

typedef void (*ui_button_on_click_fn)(void *context);

/**
 * @brief Crée un composant Bouton
 * @param parent La fenêtre sur lequel le bouton doit être dessiné
 * @param x La position X du bouton
 * @param y La position Y du bouton
 * @param label Le texte à afficher sur le bouton
 * @return ui_component_t* Le composant bouton créé
 */
ui_component_t *ui_form_create_button(WINDOW *parent, int x, int y, const char *label);

/**
 * @brief Définit l'action du bouton quand on appuie sur Entrée
 * @param comp Le composant bouton
 * @param callback La fonction à appeler quand le bouton est cliqué
 * @param context Le contexte à passer à la fonction de rappel
 */
void ui_form_button_set_on_click(ui_component_t *comp, ui_button_on_click_fn callback, void *context);

#endif // UI_FORM_BUTTON_H