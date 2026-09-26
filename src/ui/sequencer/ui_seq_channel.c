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
    
    ui_seq_nav_ch_t ch = data->channelId;
    ui_seq_nav_t *nav = data->nav;
    music_t *music = data->music;
    int currentMode = *(data->mode);

    if(event->type == UI_EVENT_NOTE_PLAYED) {
        event_note_played_data_t *payload = (event_note_played_data_t *)event->data;
        if ((ui_seq_nav_ch_t) payload->channelId == ch) {
            nav->lines[ch] = payload->lineIndex;
            nav->playMode = 1;
            return true;
        }
        return false;
    }

    if (event->type == UI_EVENT_KEY_PRESSED) {
        int key = (int)(intptr_t)event->data;
        if (nav->ch != ch) return false;
        if (currentMode == NAVIGATION_MODE) {
            switch (key) {
                case KEY_SEQ_NAV_UP: ui_seq_nav_up(nav, ch); return true;
                case KEY_SEQ_NAV_DOWN: ui_seq_nav_down(nav, ch); return true;
                case KEY_SEQ_NAV_LEFT: ui_seq_nav_left(nav); return true;
                case KEY_SEQ_NAV_RIGHT: ui_seq_nav_right(nav); return true;
            }
        } 
        else if (currentMode == EDIT_MODE) {
            music_step_t *current_step = &(music->channels[ch].steps[nav->lines[ch]]);
            switch (key) {
                case KEY_SEQ_NAV_UP:
                    if(nav->col == SEQUENCER_NAV_COL_LINE) {
                        ui_seq_nav_up(nav, ch);
                    } else {
                        ui_seq_change_sequencer_step(current_step, nav->col, 1);
                    }
                    return true;
                case KEY_SEQ_NAV_DOWN:
                    if(nav->col == SEQUENCER_NAV_COL_LINE) {
                        ui_seq_nav_down(nav, ch);
                    } else {
                        ui_seq_change_sequencer_step(current_step, nav->col, 0);
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

    mvwaddch(win, 0, 1, ACS_HLINE);
    mvwprintw(win, 0, 2, "CHANNEL %d", ch + 1);
    mvwprintw(win, 1, 1, "LINE STEP INST SHFT");
    mvwaddch(win, 1, 5, ACS_VLINE);
    mvwaddch(win, 1, 10, ACS_VLINE);
    mvwaddch(win, 1, 15, ACS_VLINE);

    for (int i = 0; i < SEQUENCER_CH_LINES - 3; i++) {
        int currentLine = nav->start[ch] + i;
        int isSelected = ((int) nav->ch == ch && nav->lines[ch] == currentLine) ? 1 : 0;
        int playModeSelected = (nav->playMode && nav->lines[ch] == currentLine) ? 1 : 0;

        if (currentLine >= MUSIC_CHANNEL_MAX_STEPS) {
            wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ) | REVERSE_IF_COL(nav->col, SEQUENCER_NAV_COL_LINE, isSelected));
            mvwprintw(win, 2 + i, 1, "....");
            wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ) | REVERSE_IF_COL(nav->col, SEQUENCER_NAV_COL_LINE, isSelected));
            
            wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ));
            mvwaddch(win, 2+i, 5, ACS_VLINE); mvwaddch(win, 2+i, 10, ACS_VLINE);
            mvwaddch(win, 2+i, 15, ACS_VLINE); 
            wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ));
            continue;
        }

        music_step_t step = music->channels[ch].steps[currentLine];
        char stepName[5] = "....";
        char instName[5] = "....";
        char shiftName[5] = " .. ";
        
        if (step.duration != MUSIC_TIME_ZERO) {
            snprintf(shiftName, 5, " %02d ", step.duration);
        }

        switch (step.type) {
            case MUSIC_STEP_TYPE_REST:
                strcpy(stepName, "----");
                break;
                
            case MUSIC_STEP_TYPE_NOTE:
                {
                    char rawNote[4];
                    note_to_string(step.data.note.noteId, step.data.note.octave, rawNote);
                    snprintf(stepName, 5, "%-4s", rawNote);
                    snprintf(instName, 5, " %02d ", step.data.note.instrumentId); 
                }
                break;
                
            case MUSIC_STEP_TYPE_COMMAND:
                switch(step.data.cmd.type) {
                    case MUSIC_CMD_SET_BPM:
                        strcpy(stepName, "BPM ");
                        snprintf(instName, 5, "%3d ", step.data.cmd.param.bpm.bpm);
                        break;
                    case MUSIC_CMD_RESET_BPM:
                        strcpy(stepName, "RBPM");
                        break;
                    case MUSIC_CMD_SET_VOLUME:
                        strcpy(stepName, "VOL ");
                        snprintf(instName, 5, " %02d ", step.data.cmd.param.volume.volumePercent);
                        break;
                    case MUSIC_CMD_LOOP_START:
                        strcpy(stepName, "LOOP");
                        snprintf(instName, 5, " %02d ", step.data.cmd.param.loopStart.id);
                        break;
                    case MUSIC_CMD_LOOP_END:
                        strcpy(stepName, "LEND");
                        snprintf(instName, 5, " %02d ", step.data.cmd.param.loopEnd.id);
                        break;
                }
                break;
        }

        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ) | REVERSE_IF_COL(nav->col, SEQUENCER_NAV_COL_LINE, isSelected) | REVERSE_IF_COL(nav->col, SEQUENCER_NAV_COL_LINE, playModeSelected));
        mvwprintw(win, 2 + i, 1, "%04X", currentLine);
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ) | REVERSE_IFNOT_PLAYMODE(nav->playMode, playModeSelected));

        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ));
        mvwaddch(win, 2+i, 5, ACS_VLINE);
        mvwaddch(win, 2+i, 10, ACS_VLINE);
        mvwaddch(win, 2+i, 15, ACS_VLINE);
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ));

        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_STEP) | REVERSE_IF_COL(nav->col, SEQUENCER_NAV_COL_STEP, isSelected));
        mvwprintw(win, 2 + i, 6, "%-4s", stepName);
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_STEP) | REVERSE_IFNOT_PLAYMODE(nav->playMode, playModeSelected));

        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_INSTRUMENT) | REVERSE_IF_COL(nav->col, SEQUENCER_NAV_COL_INSTRUMENT, isSelected));
        mvwprintw(win, 2 + i, 11, "%-4s", instName);
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_INSTRUMENT) | REVERSE_IFNOT_PLAYMODE(nav->playMode, playModeSelected));

        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_SHIFT) | REVERSE_IF_COL(nav->col, SEQUENCER_NAV_COL_TIME, isSelected));
        mvwprintw(win, 2 + i, 16, "%-4s", shiftName);
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