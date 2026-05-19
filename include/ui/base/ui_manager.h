/**
 * @file ui_manager.h
 * @brief Gestionnaire principal de l'interface graphique
 * @author Lukas Grando
 */

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "ui/base/ui_common.h"
#include "ui/base/ui_view.h"
#include "ui/sequencer/ui_seq_components.h"
#include "ui/base/ui_menu.h"
#include "event/event.h"

#define UI_MANAGER_HISTORY_SIZE 10 /*!< Taille de l'historique des vues pour la navigation arrière */

/**
 * \fn void ui_manager_init()
 * \brief Initialise ncurses, la souris, et les couleurs
 */
void ui_manager_init(void);

/**
 * \fn void ui_manager_cleanup()
 * \brief nettoie la vue actuelle et quitte ncurses proprement
 */
void ui_manager_cleanup(void);

/**
 * \fn void ui_manager_set_view(ui_view_t *newView)
 * \brief Remplace la vue actuelle par une nouvelle
 * \param newView La nouvelle vue à afficher
 */
void ui_manager_set_view(ui_view_t *newView);

/**
 * \fn void ui_manager_handle_event(event_t *event)
 * \brief Gère les événements reçus par l'interface graphique et les redirige vers la vue actuelle
 * \param event L'événement à gérer
 */
void ui_manager_handle_event(event_t *event);

/**
 * \fn void ui_manager_draw()
 * \brief Dessine la vue actuelle de l'interface graphique
 */
void ui_manager_draw(void);

/**
 * \fn void ui_manager_go_back()
 * @brief Permet de revenir à la vue précédente en utilisant l'historique des vues
 * @note Si l'historique est vide, cette fonction ne fait rien
 */
void ui_manager_go_back();

#endif // UI_MANAGER_H