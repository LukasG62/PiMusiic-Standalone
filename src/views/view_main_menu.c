
/**
 * @file view_main_menu.c
 * @brief Fichier source pour la vue du menu principal de l'interface graphique
 * @version 1.0
 * @author Lukas Grando
 */

#include "views.h"


static void on_menu_select(void *context, void *selectedValue) {
	event_queue_t *queue = (event_queue_t *)context;
	app_view_t target = (app_view_t)(intptr_t)selectedValue;

	event_t *event = create_event(UI_EVENT_VIEW_CHANGED, (void *)(intptr_t)target, NULL);
	notify_event(queue, event);
}

ui_view_t *view_main_menu() {
	ui_view_t *view = ui_menu_create_view("Welcome to PiMusiic", "Please select an option :");
	ui_menu_t *menuData = (ui_menu_t *)view->data;

	static const char *labels[] = {
		"Connection", 
		"Create music", 
		"Help", 
		"Credits",
		"Quit", 
	};
	static void *values[] = {
		(void *)(intptr_t)VIEW_CREDENTIALS, 
		(void *)(intptr_t)VIEW_CREATE_MUSIC, 
		(void *)(intptr_t)VIEW_MAIN_MENU,
		(void *)(intptr_t)VIEW_CREDITS,
		(void *)(intptr_t)VIEW_QUIT
	};

	int listWidth = getmaxx(menuData->body) - (MENU_MARGIN * 4);
    ui_component_t *list = ui_form_list_create(menuData->body, MENU_MARGIN * 2, 4, listWidth, 6);
    
    ui_form_list_set_items(list, labels, values, 5);
    ui_form_list_set_on_select(list, on_menu_select, GET_UI_QUEUE());
    
    ui_view_add_component(view, list);
    return view;
}