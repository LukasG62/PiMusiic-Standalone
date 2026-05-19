/**
 * @file ui_seq_channel.c
 * @brief Composant affichant une piste (channel) du séquenceur
 */

#include "ui/sequencer/ui_seq_components.h"

typedef struct {
    WINDOW *win;
    ui_seq_nav_ch_t channelId;
    music_t *music;
    ui_seq_nav_t *nav;
	int *mode;
} seq_channel_data_t;

static bool ui_seq_channel_handle_event(ui_component_t *self, event_t *event) {
    if (!self || !self->data) return false;
    seq_channel_data_t *data = (seq_channel_data_t *)self->data;
    
    int ch = data->channelId;
    ui_seq_nav_t *nav = data->nav;
    music_t *music = data->music;
    int currentMode = *(data->mode);

    if(event->type == UI_EVENT_NOTE_PLAYED) {
		// todo : faire un struct pour les données de l'event
        int playedChannel = ((int*)event->data)[0];
        int played_line = ((int*)event->data)[1];
        if (playedChannel == ch) {
            nav->lines[ch] = played_line;
            nav->playMode = 1;
            return true;
        }
        return false;
    }

    if (event->type == UI_EVENT_KEY_PRESSED) {
        int key = (int)(intptr_t)event->data;
        if ((int) nav->ch != ch) return false;
        if (currentMode == NAVIGATION_MODE) {
            switch (key) {
                case KEY_SEQ_NAV_UP: ui_seq_nav_up(nav, ch); return true;
                case KEY_SEQ_NAV_DOWN: ui_seq_nav_down(nav, ch); return true;
                case KEY_SEQ_NAV_LEFT: ui_seq_nav_left(nav); return true;
                case KEY_SEQ_NAV_RIGHT: ui_seq_nav_right(nav); return true;
            }
        } 
        else if (currentMode == EDIT_MODE) {
			scale_t scale = init_scale();
            note_t *current_note = &(music->channels[ch].notes[nav->lines[ch]]);
            switch (key) {
                case KEY_SEQ_NAV_UP:
                    if(nav->col == SEQUENCER_NAV_COL_LINE) {
                        ui_seq_nav_up(nav, ch);
                    } else {
                        ui_seq_change_sequencer_note(current_note, nav->col, scale, 1);
                    }
                    return true;
                case KEY_SEQ_NAV_DOWN:
                    if(nav->col == SEQUENCER_NAV_COL_LINE) {
                        ui_seq_nav_down(nav, ch);
                    } else {
                        ui_seq_change_sequencer_note(current_note, nav->col, scale, 0);
                    }
                    return true;
                case KEY_SEQ_NAV_LEFT: ui_seq_nav_left(nav); return true;
                case KEY_SEQ_NAV_RIGHT: ui_seq_nav_right(nav); return true;
            }
        }
    }

    return false;
}

/**
 * @fn ui_seq_channel_draw(ui_component_t *self)
 * @brief Fonction de dessin du channel du séquenceur
 * @param self Le composant à dessiner
 */
static void ui_seq_channel_draw(ui_component_t *self) {
    if (!self || !self->data) return;
    seq_channel_data_t *data = (seq_channel_data_t *)self->data;
    
    WINDOW *win = data->win;
    int ch = data->channelId;
    music_t *music = data->music;
    ui_seq_nav_t *nav = data->nav;

    werase(win);
    box(win, 0, 0);

    mvwprintw(win, 0, 1, "CHANNEL %d", ch + 1);
    mvwprintw(win, 1, 1, "LINE|NOTE|OCTA|INST|SHFT");

    for (int i = 0; i < SEQUENCER_CH_LINES - 3; i++) {
        int currentLine = nav->start[ch] + i;
        int isSelected = ((int) nav->ch == ch && nav->lines[ch] == currentLine) ? 1 : 0;
        int playModeSelected = (nav->playMode && nav->lines[ch] == currentLine) ? 1 : 0;

        if (currentLine >= CHANNEL_MAX_NOTES) {
            wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ) | REVERSE_IF_COL(nav->col, SEQUENCER_NAV_COL_LINE, isSelected));
            mvwprintw(win, 2 + i, 1, "----");
            wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ) | REVERSE_IF_COL(nav->col, SEQUENCER_NAV_COL_LINE, isSelected));
            
            wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ));
            mvwprintw(win, 2+i, 5, "|"); mvwprintw(win, 2+i, 10, "|");
            mvwprintw(win, 2+i, 15, "|"); mvwprintw(win, 2+i, 20, "|");
            wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ));
            continue;
        }

        note_t note = music->channels[ch].notes[currentLine];
        char instrumentName[5];
        char noteName[3];
        
        note2str(note, noteName); 
        instrument2str(note.instrument, instrumentName);

        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ) | REVERSE_IF_COL(nav->col, SEQUENCER_NAV_COL_LINE, isSelected) | REVERSE_IF_COL(nav->col, SEQUENCER_NAV_COL_LINE, playModeSelected));
        mvwprintw(win, 2 + i, 1, "%04X", currentLine);
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ) | REVERSE_IFNOT_PLAYMODE(nav->playMode, playModeSelected));

        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ));
        mvwprintw(win, 2+i, 5, "|"); mvwprintw(win, 2+i, 10, "|");
        mvwprintw(win, 2+i, 15, "|"); mvwprintw(win, 2+i, 20, "|");
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ));

        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_NOTE) | REVERSE_IF_COL(nav->col, SEQUENCER_NAV_COL_NOTE, isSelected));
        mvwprintw(win, 2 + i, 6, " %-2s ", noteName);
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_NOTE) | REVERSE_IFNOT_PLAYMODE(nav->playMode, playModeSelected));

        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_OCTAVE) | REVERSE_IF_COL(nav->col, SEQUENCER_NAV_COL_OCTAVE, isSelected));
        mvwprintw(win, 2 + i, 11, " %02d ", note.octave);
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_OCTAVE) | REVERSE_IFNOT_PLAYMODE(nav->playMode, playModeSelected));

        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_INSTRUMENT) | REVERSE_IF_COL(nav->col, SEQUENCER_NAV_COL_INSTRUMENT, isSelected));
        mvwprintw(win, 2 + i, 16, "%s", instrumentName);
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_INSTRUMENT) | REVERSE_IFNOT_PLAYMODE(nav->playMode, playModeSelected));

        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_SHIFT) | REVERSE_IF_COL(nav->col, SEQUENCER_NAV_COL_TIME, isSelected));
        mvwprintw(win, 2 + i, 21, " %02d ", note.time);
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_SHIFT) | A_REVERSE); // Force l'arrêt du reverse final
    }

    wnoutrefresh(win);
}

/**
 * @fn ui_seq_channel_destroy(ui_component_t *self)
 * @brief Fonction de destruction du channel du séquenceur
 * @param self Le composant à détruire
 */
static void ui_seq_channel_destroy(ui_component_t *self) {
    if (!self || !self->data) return;
    seq_channel_data_t *data = (seq_channel_data_t *)self->data;
    delwin(data->win);
    free(data);
    free(self);
}

/**
 * @fn ui_seq_create_channel(int x, int y, int channelId, music_t *music, ui_seq_nav_t *nav)
 * @brief Fonction de création du channel du séquenceur
 * @param x La position x du channel
 * @param y La position y du channel
 * @param channelId L'id du channel à créer
 * @param music La musique à afficher dans le channel
 * @param nav La structure de navigation partagée entre les channels
 * @return Le composant créé
 */
ui_component_t *ui_seq_create_channel(int x, int y, int channelId, music_t *music, ui_seq_nav_t *nav, int *mode) {
    ui_component_t *comp = (ui_component_t *)malloc(sizeof(ui_component_t));
    seq_channel_data_t *data = (seq_channel_data_t *)malloc(sizeof(seq_channel_data_t));

    data->win = newwin(SEQUENCER_CH_LINES, SEQUENCER_CH_COLS, y, x);
    data->channelId = channelId;
    data->music = music;
    data->nav = nav;
    data->mode = mode;

    comp->data = data;
    comp->isFocused = false; 
    comp->draw = ui_seq_channel_draw;
    comp->handle_event = ui_seq_channel_handle_event;
    comp->destroy = ui_seq_channel_destroy;

    return comp;
}