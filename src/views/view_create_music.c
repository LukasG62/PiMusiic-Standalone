#include "views.h"
#include <stdlib.h>

// Callback du bouton Submit
static void on_submit_create_music(void *context) {
    event_queue_t *queue = (event_queue_t *)context;
    event_t *ev = create_event(UI_EVENT_VIEW_CHANGED, (void *)(intptr_t)VIEW_SEQUENCER, NULL);
    notify_event(queue, ev);
}

ui_view_t *view_create_music(music_t *music, ui_menu_credentials_t *creds, bool isConnected) {
    ui_view_t *view = ui_menu_create_view("Music Creation", "Please enter the music name and the BPM :");
    ui_menu_t *menu = (ui_menu_t *)view->data;
    
    // todo: Add unfocusable label components for the warning text when user create a music without being connected.

    ui_component_t *nameField = ui_form_create_input_field(menu->body, MENU_MARGIN, MENU_MARGIN + 2);
    ui_form_set_input_label(nameField, "Music name");
    ui_form_set_input_color(nameField, COLOR_PAIR_MENU, COLOR_PAIR_MENU_PROMPT);
    ui_form_set_input_value(nameField, music->name);
    ui_form_configure_text_input(nameField, MUSIC_NAME_MAX_LENGTH, false);

    ui_component_t *bpmField = ui_form_create_input_field(menu->body, MENU_MARGIN + MUSIC_NAME_MAX_LENGTH + 6, MENU_MARGIN + 2);
    ui_form_set_input_label(bpmField, "BPM");
    ui_form_set_input_color(bpmField, COLOR_PAIR_MENU, COLOR_PAIR_MENU_PROMPT);
    ui_form_set_input_value(bpmField, &(music->bpm));
    ui_form_configure_number_input(bpmField, 60, 240);

    ui_component_t *submitBtn = ui_form_create_button(menu->body, MENU_MARGIN, 12, "[ENTER] Submit");
    ui_form_button_set_on_click(submitBtn, on_submit_create_music, GET_UI_QUEUE());
    

    ui_view_add_component(view, nameField);
    ui_view_add_component(view, bpmField);
    ui_view_add_component(view, submitBtn);

    return view;
}