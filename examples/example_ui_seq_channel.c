/**
 * @file example_ui_seq.c
 * @brief Application d'exemple interactive pour tester les composants du séquenceur
 * @author Lukas Grando
 */

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ui/base/ui_common.h"
#include "ui/base/ui_component.h"
#include "ui/sequencer/ui_seq_components.h"
#include "music/note.h"

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(COLOR_PAIR_SEQ, COLOR_WHITE, COLOR_BLACK);
        init_pair(COLOR_PAIR_SEQ_NOTE, COLOR_GREEN, COLOR_BLACK);
        init_pair(COLOR_PAIR_SEQ_OCTAVE, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(COLOR_PAIR_SEQ_INSTRUMENT, COLOR_YELLOW, COLOR_BLACK);
        init_pair(COLOR_PAIR_SEQ_SHIFT, COLOR_CYAN, COLOR_BLACK);
    }

    music_t mock_music;
    memset(&mock_music, 0, sizeof(music_t));
    mock_music.bpm = 120;

    for (int i = 0; i < 16; i++) {
        mock_music.channels[0].notes[i].id = i % 12;
        mock_music.channels[0].notes[i].octave = 4;
        mock_music.channels[0].notes[i].instrument = i % 4;
        mock_music.channels[0].notes[i].time = 4;

        mock_music.channels[1].notes[i].id = 255;
        mock_music.channels[2].notes[i].id = 255;
    }

    ui_seq_nav_t shared_nav;
    shared_nav.ch = 0;
    shared_nav.col = 0;
    shared_nav.playMode = 0;
    for (int i = 0; i < 3; i++) {
        shared_nav.start[i] = 0;
        shared_nav.lines[i] = 0;
    }

    int current_mode = NAVIGATION_MODE;

    ui_component_t *channels[3];
    channels[0] = ui_seq_create_channel(2,  3, 0, &mock_music, &shared_nav, &current_mode);
    channels[1] = ui_seq_create_channel(28, 3, 1, &mock_music, &shared_nav, &current_mode);
    channels[2] = ui_seq_create_channel(54, 3, 2, &mock_music, &shared_nav, &current_mode);

    bool running = true;
    event_t ev;
    ev.type = UI_EVENT_KEY_PRESSED;

    while (running) {
        erase();
        attron(A_BOLD);
        mvprintw(0, 2, "PIMUSIIC - SEQUENCER COMPONENT SANDBOX");
        attroff(A_BOLD);
        mvprintw(1, 2, "Mode: %s | Active Channel: %d | Active Col: %d", 
                 (current_mode == NAVIGATION_MODE) ? "NAVIGATION" : "EDITION", 
                 shared_nav.ch + 1, shared_nav.col);
        mvprintw(20, 2, "[ESC] Quitter | [SPACE] Basculer Mode | [ARROWS] Naviguer / Éditer");
        refresh();

        for (int i = 0; i < 3; i++) {
            channels[i]->draw(channels[i]);
        }
        doupdate();

        int ch = getch();

        if (ch == 27) {
            running = false;
        } 
        else if (ch == ' ') {
            current_mode = (current_mode == NAVIGATION_MODE) ? EDIT_MODE : NAVIGATION_MODE;
        } 
        else {
            ev.data = (void *)(intptr_t)ch;
            int active_ch = shared_nav.ch;
            if (active_ch >= 0 && active_ch < 3) {
                channels[active_ch]->handle_event(channels[active_ch], &ev);
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        channels[i]->destroy(channels[i]);
    }

    endwin();
    return 0;
}