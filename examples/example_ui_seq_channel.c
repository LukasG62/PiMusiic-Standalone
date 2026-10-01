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
#include "music/music.h"
#include "music/step.h"
#include "music/note.h"

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        ui_seq_init_colors();
    }

    music_t mock_music;
    music_init(&mock_music, 120, "Sandbox Track");

    for (int i = 0; i < 16; i++) {
        music_step_t step_ch0;
        
        if (i == 4) {
            step_ch0 = music_step_create_cmd_bpm(140, MUSIC_TIME_ZERO);
        } else if (i == 8) {
            step_ch0 = music_step_create_cmd_loop_start(0, MUSIC_TIME_ZERO);
        } else if (i == 15) {
            step_ch0 = music_step_create_cmd_loop_end(0, 4, MUSIC_TIME_ZERO);
        } else {
            step_ch0 = music_step_create_note(NOTE_C_ID + (i % 7), 4, 1, MUSIC_TIME_NOIRE);
        }
        music_write_step(&mock_music, 0, i, step_ch0);

        if (i % 2 == 0) {
            music_write_step(&mock_music, 1, i, music_step_create_note(NOTE_E_ID, 3, 2, MUSIC_TIME_CROCHE));
        } else {
            music_write_step(&mock_music, 1, i, music_step_create_rest(MUSIC_TIME_CROCHE));
        }
    }
    ui_seq_nav_t shared_nav = ui_seq_init_nav(0);
    int current_mode = NAVIGATION_MODE;

    ui_component_t *channels[3];
    channels[0] = ui_seq_create_channel(2,  3, 0, &mock_music, &shared_nav, &current_mode);
    channels[1] = ui_seq_create_channel(25, 3, 1, &mock_music, &shared_nav, &current_mode);
    channels[2] = ui_seq_create_channel(48, 3, 2, &mock_music, &shared_nav, &current_mode);

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
        mvprintw(26, 2, "[ESC] Quitter | [SPACE] Basculer Mode | [ARROWS] Naviguer / Éditer");
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