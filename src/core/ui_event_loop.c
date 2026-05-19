/**
 * @file ui_event_loop.c
 * @brief En-tête de la boucle d'événements de l'interface graphique
 */
#include "core/ui_event_loop.h"

/**
 * @fn void controller_load_view(app_choices_t target)
 * @brief Routeur de vues de l'application
 * @param target La vue cible à charger
 */
void controller_load_view(app_context_t *ctx, app_view_t target) {
	ui_view_t *nextView = NULL;
    switch (target) {
        case VIEW_MAIN_MENU:
            nextView = view_main_menu();
            break;
        case VIEW_HELP:
            //nextView = view_help();
            break;
        case VIEW_CREDENTIALS:
            nextView = view_credentials(&(ctx->credentials));
            break;
        case VIEW_CREATE_MUSIC:
            nextView = view_create_music(&(ctx->music), &(ctx->credentials), ctx->isConnected);
            break;
        case VIEW_LIST_MUSIC:
            //nextView = view_list_music();
            break;
        case VIEW_SEQUENCER:
            nextView = view_sequencer(ctx);
            break;
        case VIEW_CREDITS:
            nextView = view_credits();
            break;
        case VIEW_ERROR:
            //nextView = view_error();
            break;
        case VIEW_QUIT:
            //nextView = view_quit();
            break;
        default:
            nextView = view_main_menu();
            break;
    }

    if(nextView) ui_manager_set_view(nextView);
}

/**
 * @fn void ui_event_loop_run(void *arg)
 * @brief Lance la boucle infinie de l'interface graphique
 * @param ctx Le contexte global de l'application
 */
void ui_event_loop_run(void *arg) {
	app_context_t *ctx = (app_context_t *)arg;
	controller_load_view(ctx, VIEW_MAIN_MENU);

	while (isRunning) {
        event_t *ev;
        while ((ev = trywait_event(uiQueue)) != NULL) {
            if (ev->type == UI_EVENT_VIEW_CHANGED) {
                app_view_t target = (app_view_t)(intptr_t)ev->data;
                controller_load_view(ctx, target);
            } 
            else if (ev->type == UI_EVENT_VIEW_GO_BACK) {
                ui_manager_go_back();
            }
            else {
                ui_manager_handle_event(ev);
            }
            destroy_event(ev);
        }

		int ch = getch();
		if (ch != ERR) {
			event_t *key_ev = create_event(UI_EVENT_KEY_PRESSED, (void *)(intptr_t)ch, NULL);
			ui_manager_handle_event(key_ev);
			destroy_event(key_ev);
		} else {
			event_t *tick_ev = create_event(UI_EVENT_TICK, NULL, NULL); 
			ui_manager_handle_event(tick_ev);
			destroy_event(tick_ev);
		}
		ui_manager_draw();
	}
}