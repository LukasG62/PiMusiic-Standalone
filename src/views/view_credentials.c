/**
 * \file view_credentials.c
 * \brief Fichier source pour la vue de connexion de l'interface graphique
 * \version 1.0
 * \author Lukas Grando
 */


#include "views.h"
#include <stdlib.h>

static void on_submit_credentials(void *context) {
	event_queue_t *queue = (event_queue_t *)context;
    event_t *ev = create_event(UI_EVENT_VIEW_CHANGED, (void *)(intptr_t)VIEW_MAIN_MENU, NULL);
    notify_event(queue, ev);
}


ui_view_t *view_credentials(ui_menu_credentials_t *creds) {
    ui_view_t *view = ui_menu_create_view("Connection", "Please enter your login information :");
    ui_menu_t *menu = (ui_menu_t *)view->data;

    ui_component_t *userField = ui_form_create_input_field(menu->body, MENU_MARGIN, MENU_MARGIN + 2);
    ui_form_set_input_label(userField, "Username");
    ui_form_set_input_color(userField, COLOR_PAIR_MENU, COLOR_PAIR_MENU_PROMPT);
    ui_form_set_input_value(userField, creds->username);
    ui_form_configure_text_input(userField, APP_USERNAME_MAX_LENGTH, false);

    ui_component_t *passField = ui_form_create_input_field(menu->body, MENU_MARGIN, MENU_MARGIN + 6);
    ui_form_set_input_label(passField, "Password");
    ui_form_set_input_color(passField, COLOR_PAIR_MENU, COLOR_PAIR_MENU_PROMPT);
    ui_form_set_input_value(passField, creds->password);
    ui_form_configure_text_input(passField, APP_PASSWORD_MAX_LENGTH, true);

	ui_component_t *submitBtn = ui_form_create_button(menu->body, MENU_MARGIN, 12, "[ENTER] Submit");
    ui_form_button_set_on_click(submitBtn, on_submit_credentials, GET_UI_QUEUE());
    mvwprintw(menu->body, 12, MENU_MARGIN, "[ENTER] Submit");

    ui_view_add_component(view, userField);
    ui_view_add_component(view, passField);
	ui_view_add_component(view, submitBtn);

    return view;
}