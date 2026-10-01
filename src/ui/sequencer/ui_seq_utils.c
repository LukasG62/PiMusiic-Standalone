/**
 * @file ui_sequencer.c
 * @brief Fichier source pour la logique et les fonctions utilitaires du séquenceur
 */
#include "ui/sequencer/ui_seq_components.h"
#include "music/music.h"

/**
 * @brief Édition de la colonne de step dans le séquenceur
 * @param step Le pointeur vers le step à modifier
 * @param isUp La direction de la modification
 */
static void ui_seq_edit_col_step(music_step_t *step, int isUp) {
    music_time_duration_t duration = step->duration;

    switch (step->type) {
        case MUSIC_STEP_TYPE_REST:
            if(isUp) *step = music_step_create_note(NOTE_C_ID, 4, 1, duration);
            else *step = music_step_create_cmd_loop_end(0, 1, duration);
            break;

        case MUSIC_STEP_TYPE_NOTE:
            if (isUp) {
                if(step->data.note.noteId == NOTE_B_ID && step->data.note.octave == 8) {
                    *step = music_step_create_cmd_bpm(120, duration);
                } else {
                    step->data.note.noteId++;
                    if (step->data.note.noteId > NOTE_B_ID) {
                        step->data.note.noteId = NOTE_C_ID;
                        step->data.note.octave++;
                    }
                }
            } else {
                if(step->data.note.noteId == NOTE_C_ID && step->data.note.octave == 0) {
                    *step = music_step_create_rest(duration);
                } else {
                    step->data.note.noteId--;
                    if (step->data.note.noteId < NOTE_C_ID) {
                        step->data.note.noteId = NOTE_B_ID;
                        step->data.note.octave--;
                    }
                }
            }
            break;

        case MUSIC_STEP_TYPE_COMMAND:
            if(isUp) {
                switch (step->data.cmd.type) {
                    case MUSIC_CMD_SET_BPM:    
                        step->data.cmd.type = MUSIC_CMD_RESET_BPM; 
                        break;
                    case MUSIC_CMD_RESET_BPM:  
                        *step = music_step_create_cmd_volume(100, duration); 
                        break;
                    case MUSIC_CMD_SET_VOLUME: 
                        *step = music_step_create_cmd_loop_start(0, duration); 
                        break;
                    case MUSIC_CMD_LOOP_START: 
                        *step = music_step_create_cmd_loop_end(0, 1, duration); 
                        break;
                    case MUSIC_CMD_LOOP_END:   
                        *step = music_step_create_rest(duration);
                        break;
                }
            } else {
                switch (step->data.cmd.type) {
                    case MUSIC_CMD_LOOP_END:   
                        *step = music_step_create_cmd_loop_start(0, duration); 
                        break;
                    case MUSIC_CMD_LOOP_START: 
                        *step = music_step_create_cmd_volume(100, duration); 
                        break;
                    case MUSIC_CMD_SET_VOLUME: 
                        step->data.cmd.type = MUSIC_CMD_RESET_BPM; 
                        break;
                    case MUSIC_CMD_RESET_BPM:  
                        *step = music_step_create_cmd_bpm(120, duration); 
                        break;
                    case MUSIC_CMD_SET_BPM:    
                        *step = music_step_create_note(NOTE_B_ID, 8, 1, duration);
                        break;
                }
            }
            break;
    }
}

/**
 * @fn ui_seq_edit_col_instrument()
 * @brief Édition de la colonne d'instrument
 * @param step Pointeur vers l'étape à éditer
 * @param isUp Indique si l'édition est vers le haut
 */
static void ui_seq_edit_col_instrument(music_step_t *step, int isUp) {
    switch (step->type) {
        case MUSIC_STEP_TYPE_NOTE:
            if (isUp) step->data.note.instrumentId++;
            else if (step->data.note.instrumentId > 0) step->data.note.instrumentId--;
            break;

        case MUSIC_STEP_TYPE_COMMAND:
            switch (step->data.cmd.type) {
                case MUSIC_CMD_SET_BPM:
                    if(isUp) step->data.cmd.param.bpm.bpm += 5;
                    else if(step->data.cmd.param.bpm.bpm > 5) step->data.cmd.param.bpm.bpm -= 5;
                    break;
                case MUSIC_CMD_SET_VOLUME:
                    if (isUp && step->data.cmd.param.volume.volumePercent <= 95) step->data.cmd.param.volume.volumePercent += 5;
                    else if(!isUp && step->data.cmd.param.volume.volumePercent >= 5) step->data.cmd.param.volume.volumePercent -= 5;
                    break;
                case MUSIC_CMD_LOOP_START:
                    if(isUp) step->data.cmd.param.loopStart.id++;
                    else if(step->data.cmd.param.loopStart.id > 0) step->data.cmd.param.loopStart.id--;
                    break;
                case MUSIC_CMD_LOOP_END:
                    if(isUp) step->data.cmd.param.loopEnd.count++;
                    else if(step->data.cmd.param.loopEnd.count > 0) step->data.cmd.param.loopEnd.count--;
                    break;
                case MUSIC_CMD_RESET_BPM:
                    break;
            }
            break;
            
        case MUSIC_STEP_TYPE_REST:
        default:
            break;
    }
}

/**
 * @fn ui_seq_edit_col_time()
 * @brief Édition de la colonne de temps
 * @param step Pointeur vers l'étape à éditer
 * @param isUp Indique si l'édition est vers le haut
 */
static void ui_seq_edit_col_time(music_step_t *step, int isUp) {
    if(isUp) {
        if(step->duration == MUSIC_TIME_ZERO) step->duration = MUSIC_TIME_CROCHE_DOUBLE;
        else if(step->duration < MUSIC_TIME_RONDE) step->duration *= 2;
        else step->duration = MUSIC_TIME_ZERO;
    } else {
        if(step->duration == MUSIC_TIME_ZERO) step->duration = MUSIC_TIME_RONDE;
        else if(step->duration > MUSIC_TIME_CROCHE_DOUBLE) step->duration /= 2;
        else step->duration = MUSIC_TIME_ZERO;
    }
}

/**
 * @fn ui_seq_init_colors()
 * @brief Initialisation des couleurs du séquenceur
 * @details Cette fonction initialise les couleurs du séquenceur avec les paires de couleurs
 */
void ui_seq_init_colors() {
    init_pair(COLOR_PAIR_SEQ, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_NOTSAVED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_SAVED, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_PLAYED, COLOR_BLACK, COLOR_WHITE);
    // init_pair(COLOR_PAIR_SEQ_OCTAVE, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_STEP, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_INSTRUMENT, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_SHIFT, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_HEADER_TITLE, COLOR_WHITE, COLOR_BLACK);
}

/**
 * @fn ui_seq_init_nav()
 * @brief Création de la structure de navigation du séquenceur
 * @return ui_seq_nav_t 
 */
ui_seq_nav_t ui_seq_init_nav(int playMode) {
    ui_seq_nav_t nav;
    nav.col = SEQUENCER_NAV_COL_LINE;
    nav.ch = SEQUENCER_NAV_CH1;
    for (int i = 0; i < SEQUENCER_NAV_CH_MAX; i++) {
        nav.start[i] = 0;
        nav.lines[i] = 0;
    }
    nav.playMode = playMode;
    return nav;
}

/**
 * @fn void ui_seq_nav_up(ui_seq_nav_t *nav, int channelId)
 * @brief permet de passer d'une ligne à une autre dans le séquenceur
 * @param nav la structure de navigation
 * @param channelId L'id du channel
 */
void ui_seq_nav_up(ui_seq_nav_t *nav, int channelId) {
    if(channelId == -1) {
        channelId = nav->ch;
    }
    if (nav->lines[channelId] > 0) nav->lines[channelId]--;
    if (nav->lines[nav->ch] < nav->start[channelId]) {
        nav->start[channelId] = nav->start[channelId] - SEQUENCER_CH_LINES + 3;
    }
}

/**
 * @fn ui_seq_nav_down()
 * @brief permet de passer d'une ligne à une autre dans le séquenceur
 * @param nav la structure de navigation
 * @param channelId L'id du channel
 */
void ui_seq_nav_down(ui_seq_nav_t *nav, int channelId) {
    if(channelId == -1) {
        channelId = nav->ch;
    }
    if (nav->lines[channelId] >= nav->start[channelId] + SEQUENCER_CH_LINES - 4) {
        nav->start[channelId] = nav->start[channelId] + SEQUENCER_CH_LINES - 3;
    }
    if (nav->lines[channelId] < MUSIC_CHANNEL_MAX_STEPS - 1) nav->lines[channelId]++;
}

/**
 * @fn ui_seq_nav_left(ui_seq_nav_t *nav)
 * @brief La fonction qui permet de passer d'une colonne à une autre
 * @param nav la structure de navigation
 */
void ui_seq_nav_left(ui_seq_nav_t *nav) {
    if (nav->col == SEQUENCER_NAV_COL_LINE) {
        int newChannel = ((int) nav->ch - 1) != -1 ? nav->ch - 1 : SEQUENCER_NAV_CH_MAX - 1;
        nav->start[newChannel] = nav->start[nav->ch];
        nav->lines[newChannel] = nav->lines[nav->ch];
        nav->ch = newChannel;
        nav->col = SEQUENCER_NAV_COL_TIME;
        return;
    }
    if (nav->col > 0) nav->col--;
}

/**
 * @fn ui_seq_nav_right(ui_seq_nav_t *nav)
 * @brief La fonction qui permet de passer d'une colonne à une autre
 * @param nav la structure de navigation
 */
void ui_seq_nav_right(ui_seq_nav_t *nav) {
    if (nav->col == SEQUENCER_NAV_COL_TIME) {
        int newChannel = (nav->ch + 1) % SEQUENCER_NAV_CH_MAX;
        nav->start[newChannel] = nav->start[nav->ch];
        nav->lines[newChannel] = nav->lines[nav->ch];
        nav->ch = newChannel;
        nav->col = SEQUENCER_NAV_COL_LINE;
        return;
    }
    if (nav->col < SEQUENCER_NAV_COL_MAX - 1) nav->col++;
}

/**
 * @fn ui_seq_change_sequencer_step(music_step_t *step, short col, int isUp)
 * @brief Modification "in-place" d'un step dans l'interface Ncurses
 * @param step Le pointeur vers le step courant
 * @param col La colonne active
 * @param isUp La direction de la modification
 */
void ui_seq_change_sequencer_step(music_step_t *step, short col, int isUp) {
    // TODO: changer le fonctionnement pas très UX friendly
    if (!step) return;

    switch (col) {
        case SEQUENCER_NAV_COL_STEP:
            ui_seq_edit_col_step(step, isUp);
            break;
        case SEQUENCER_NAV_COL_INSTRUMENT:
            ui_seq_edit_col_instrument(step, isUp);
            break;
        case SEQUENCER_NAV_COL_TIME:
            ui_seq_edit_col_time(step, isUp);
            break;
        default:
            break;
    }
}