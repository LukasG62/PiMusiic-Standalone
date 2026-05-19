/**
 * @file ui_event_loop.h
 * @brief En-tête de la boucle d'événements de l'interface graphique
 */

#ifndef UI_EVENT_LOOP_H
#define UI_EVENT_LOOP_H

#include "core.h"
#include "event/event.h"
#include "event/event_queue.h"
#include "ui/base/ui_common.h"
#include "ui/base/ui_manager.h"
#include "views.h"


/**
 * @fn void controller_load_view(app_choices_t target)
 * @brief Routeur de vues de l'application
 * @param target La vue cible à charger
 */
void controller_load_view(app_context_t *ctx, app_view_t target);

/**
 * @fn void ui_event_loop_run(void *arg)
 * @brief Lance la boucle infinie de l'interface graphique
 * @param ctx Le contexte global de l'application
 */
void ui_event_loop_run(void *arg);

#endif // UI_EVENT_LOOP_H