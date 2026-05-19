/**
 * @file ui_seq_info.c
 * @brief Composant affichant les informations globales de la musique dans le séquenceur
 */
#include "ui/sequencer/ui_seq_components.h"
#include <stdlib.h>
#include <time.h>

typedef struct {
    WINDOW *win;
    music_t *music;
    int *mode;
    bool *need2save;
} seq_info_data_t;

static bool ui_seq_info_handle_event(ui_component_t *self, event_t *event) {
    UNUSED(self);
    UNUSED(event);
    return false;
}

static void ui_seq_info_draw(ui_component_t *self) {
    if (!self || !self->data) return;
    seq_info_data_t *data = (seq_info_data_t *)self->data;
    WINDOW *win = data->win;
    music_t *music = data->music;
    int mode = *(data->mode);
    bool need2save = *(data->need2save);

    werase(win);

    char date[20];
    struct tm *timeinfo = localtime(&music->date.tv_sec);
    strftime(date, 20, "%d/%m/%Y %H:%M:%S", timeinfo);

    box(win, 0, 0);
    
    mvwprintw(win, 0, 1, "INFO");
    mvwprintw(win, 1, 1, "Created :");
    mvwprintw(win, 2, 1, "BPM :");
    mvwprintw(win, 3, 1, "Mode :");

    wattron(win, (need2save ? COLOR_PAIR(COLOR_PAIR_SEQ_NOTSAVED) : COLOR_PAIR(COLOR_PAIR_SEQ_SAVED)) | A_BOLD);
    mvwprintw(win, 1, 10, " %s", date);
    wattroff(win, (need2save ? COLOR_PAIR(COLOR_PAIR_SEQ_NOTSAVED) : COLOR_PAIR(COLOR_PAIR_SEQ_SAVED)) | A_BOLD);

    wattron(win, A_BOLD);
    mvwprintw(win, 2, 6, " %d", music->bpm);
    wattroff(win, A_BOLD);

    if(mode == NAVIGATION_MODE) {
        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_OCTAVE) | A_BOLD);
        mvwprintw(win, 3, 8, "NAVIGATION");
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_OCTAVE));
        
        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ) | A_BOLD);
        mvwprintw(win, 4, 1, "[BTN1] CH1         [BTN2] CH2       [BTN3] CH3 ");
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ) | A_BOLD);
    }
    else {
        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_NOTE) | A_BOLD);
        mvwprintw(win, 3, 8, "EDITION");
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_NOTE));
        
        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ) | A_BOLD);
        mvwprintw(win, 4, 1, "[BTN1] Save        [BTN2] Quit       [BTN3] Play ");
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ) | A_BOLD);
    }
    
    wnoutrefresh(win);
}

static void ui_seq_info_destroy(ui_component_t *self) {
    if (!self || !self->data) return;
    seq_info_data_t *data = (seq_info_data_t *)self->data;
    delwin(data->win);
    free(data);
    free(self);
}

ui_component_t *ui_seq_create_info_box(int x, int y, music_t *music, int *currentMode, bool *need2save) {
    ui_component_t *comp = (ui_component_t *)malloc(sizeof(ui_component_t));
    seq_info_data_t *data = (seq_info_data_t *)malloc(sizeof(seq_info_data_t));

    data->win = newwin(SEQUENCER_INFO_LINES, SEQUENCER_INFO_COLS, y, x);
    data->music = music;
    data->mode = currentMode;
    data->need2save = need2save;

    comp->data = data;
    comp->isFocused = false; 
    comp->draw = ui_seq_info_draw;
    comp->handle_event = ui_seq_info_handle_event;
    comp->destroy = ui_seq_info_destroy;

    return comp;
}