/**
 * @file ui_seq_help.c
 * @brief Composant affichant l'aide du séquenceur
 */
#include "ui/sequencer/ui_seq_components.h"
#include <stdlib.h>

typedef struct {
    WINDOW *win;
} seq_help_data_t;

static bool ui_seq_help_handle_event(ui_component_t *self, event_t *event) {
    UNUSED(self);
    UNUSED(event);
    return false;
}

static void ui_seq_help_draw(ui_component_t *self) {
    if (!self || !self->data) return;
    seq_help_data_t *data = (seq_help_data_t *)self->data;
    WINDOW *win = data->win;

    werase(win);
    box(win, 0, 0);
    
    wattron(win, A_BOLD);
    mvwprintw(win, 0, 1, "HELP");
    wattroff(win, A_BOLD);
    
    mvwprintw(win, 1, 1, "   : Change note/octave/instrument/shift");
    mvwaddch(win, 1, 1, ACS_DARROW);
    mvwaddch(win, 1, 3, ACS_UARROW);

    mvwprintw(win, 2, 1, "   : Change column in the channel");
    mvwaddch(win, 2, 1, ACS_LARROW);
    mvwaddch(win, 2, 3, ACS_RARROW);

    mvwprintw(win, 3, 1, "[BTN4] : Change button mode");

    wnoutrefresh(win);
}

static void ui_seq_help_destroy(ui_component_t *self) {
    if (!self || !self->data) return;
    seq_help_data_t *data = (seq_help_data_t *)self->data;
    delwin(data->win);
    free(data);
    free(self);
}

ui_component_t *ui_seq_create_help_box(int x, int y) {
    ui_component_t *comp = (ui_component_t *)malloc(sizeof(ui_component_t));
    seq_help_data_t *data = (seq_help_data_t *)malloc(sizeof(seq_help_data_t));

    data->win = newwin(SEQUENCER_HELP_LINES, SEQUENCER_HELP_COLS, y, x);

    comp->data = data;
    comp->isFocused = false; 
    comp->draw = ui_seq_help_draw;
    comp->handle_event = ui_seq_help_handle_event;
    comp->destroy = ui_seq_help_destroy;

    return comp;
}