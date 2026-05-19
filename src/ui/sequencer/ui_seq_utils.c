/**
 * @file ui_sequencer.c
 * @brief Fichier source pour la logique et les fonctions utilitaires du séquenceur
 */
#include "ui/sequencer/ui_seq_components.h"
#include "music/sound.h"

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
    init_pair(COLOR_PAIR_SEQ_OCTAVE, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_NOTE, COLOR_GREEN, COLOR_BLACK);
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
    if (nav->lines[channelId] < CHANNEL_MAX_NOTES - 1) nav->lines[channelId]++;
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
 * @fn ui_seq_change_sequencer_note(note_t *note, short col, scale_t scale, int isUp)
 * @brief Modification d'une note du séquenceur
 * @param note La note à modifier
 * @param col La colonne actuel
 * @param scale La gamme des notes
 * @param isUp La direction de la modification (0 pour le bas, 1 pour
 */
void ui_seq_change_sequencer_note(note_t *note, short col, scale_t scale, int isUp) {
    switch(col) {
        case SEQUENCER_NAV_COL_NOTE:
            if (isUp) get_next_note(note, &scale);
            else get_previous_note(note, &scale);

            if(note->id != NOTE_NA_ID) {
                if (note->instrument == INSTRUMENT_NA) note->instrument = INSTRUMENT_SIN;
            }
            else {
                note->instrument = INSTRUMENT_NA;
            }
            break;
        case SEQUENCER_NAV_COL_OCTAVE:
            if (isUp) note->octave = note->octave + 1 > 8 ? 8 : note->octave + 1;
            else note->octave = note->octave - 1 < 0 ? 8 : note->octave - 1;
            break;
        case SEQUENCER_NAV_COL_INSTRUMENT:
            if (isUp) note->instrument = note->instrument + 1 == INSTRUMENT_NB ? 0 : note->instrument + 1;
            else note->instrument = ((int) note->instrument - 1) == -1 ? INSTRUMENT_NB - 1 : note->instrument - 1;
            break;
        case SEQUENCER_NAV_COL_TIME:
            if(note->time < TIME_END) note->time = isUp ? note->time * 2 : note->time / 2;
            else note->time = isUp ? TIME_CROCHE_DOUBLE : note->time / 2;
            if(note->time == 0) note->time = TIME_RONDE;
            break;
    }
}