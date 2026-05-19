/**
 * \file ui_form_button.c
 * \brief Fichier source pour les boutons de formulaire de l'interface graphique
 * \version 1.0
 * \author Lukas Grando
 */

#include "ui/form/ui_form_button.h"

typedef struct {
    WINDOW *parent;
    char *label;
    int x, y;
    ui_button_on_click_fn onClick;
    void *context;
} form_button_data_t;

static void ui_button_draw(ui_component_t *self) {
    form_button_data_t *data = (form_button_data_t *)self->data;
    
    if (self->isFocused) {
        wattron(data->parent, A_REVERSE);
    }
    mvwprintw(data->parent, data->y, data->x, "%s", data->label);
    if (self->isFocused) {
        wattroff(data->parent, A_REVERSE);
    }
    if (self->isFocused) curs_set(0); 
}

static bool ui_button_handle_event(ui_component_t *self, event_t *event) {
    form_button_data_t *data = (form_button_data_t *)self->data;

    if (event->type == UI_EVENT_KEY_PRESSED) {
        int key = (int)(intptr_t)event->data;
        if (key == '\n' || key == '\r' || key == KEY_ENTER) {
            if (data->onClick) {
                data->onClick(data->context);
            }
            return true;
        }
    }
    return false;
}

static void ui_button_destroy(ui_component_t *self) {
	if (!self) return;
	form_button_data_t *data = (form_button_data_t *)self->data;
	free(data->label);
	free(data);
	free(self);
}

/**
 * \brief Crée un composant Bouton
 * \param label Le texte du bouton (ex: "[ENTER] Submit")
 */
ui_component_t *ui_form_create_button(WINDOW *parent, int x, int y, const char *label) {
	ui_component_t *button = (ui_component_t *)malloc(sizeof(ui_component_t));
	CHECK_ALLOC(button);
	form_button_data_t *data = (form_button_data_t *)malloc(sizeof(form_button_data_t));
	CHECK_ALLOC(data);
	*data = (form_button_data_t) {
		.parent = parent,
		.label = strdup(label),
		.x = x,
		.y = y,
		.onClick = NULL,
		.context = NULL,
	};

	*button = (ui_component_t) {
		.data = data,
		.isFocused = false,
		.draw = ui_button_draw,
		.handle_event = ui_button_handle_event,
		.destroy = ui_button_destroy,
	};
	return button;
}

/**
 * \brief Définit l'action du bouton quand on appuie sur Entrée
 */
void ui_form_button_set_on_click(ui_component_t *comp, ui_button_on_click_fn callback, void *context) {
	if (!comp || !comp->data) return;
	form_button_data_t *data = (form_button_data_t *)comp->data;
	data->onClick = callback;
	data->context = context;
}
