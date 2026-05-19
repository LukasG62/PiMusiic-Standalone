/**
 * @file test_ui_form.c
 * @brief Test de l'implémentation des composants de formulaire
 * @author Lukas Grando
 */

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "ui/base/ui_common.h"
#include "ui/base/ui_component.h"
#include "ui/form/ui_form_input.h"

char username[30] = "Lukas";
char password[30];
int bpm = 25;

int main() {

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
    }

    ui_component_t *nameInput = ui_form_create_input_field(stdscr, 5, 2);
    ui_form_set_input_label(nameInput, "Username");
    ui_form_set_input_color(nameInput, 1, 2);
    ui_form_set_input_value(nameInput, username);
    ui_form_configure_text_input(nameInput, 20, false);

    ui_component_t *bpmInput = ui_form_create_input_field(stdscr, 5, 8);
    ui_form_set_input_label(bpmInput, "BPM");
    ui_form_set_input_color(bpmInput, 1, 2);
    ui_form_set_input_value(bpmInput, &bpm);
    ui_form_configure_number_input(bpmInput, 20, 200);


    ui_component_t *components[2] = {nameInput, bpmInput};
    int active_index = 0;
    
    components[0]->isFocused = true;
    components[1]->isFocused = false;

    bool running = true;
    event_t mock_event; 
    mock_event.type = UI_EVENT_KEY_PRESSED;

    while (running) {

        erase();
        mvprintw(0, 2, "UI Form Test - Form Components");
        mvprintw(15, 2, "[ESC] Quit | [TAB] Switch Focus | [Mouse Click] Increment/Decrement BPM");
        mvprintw(17, 2, "Current Values -> Username: %s | BPM: %d", username, bpm);
        refresh();

        for (int i = 0; i < 2; i++) {
            components[i]->draw(components[i]);
        }
        doupdate();

        int ch = getch();

        if (ch == 27) {
            running = false;
        } 
        else if (ch == '\t') {
            components[active_index]->isFocused = false;
            active_index = (active_index + 1) % 2;
            components[active_index]->isFocused = true;
        } 
        else {
            mock_event.data = (void *)(intptr_t)ch;
            components[active_index]->handle_event(components[active_index], &mock_event);
        }
    }
    nameInput->destroy(nameInput);
    bpmInput->destroy(bpmInput);
    
    endwin();

    printf("Test termine !\n");
    printf("Nouvelles valeurs -> Username: %s | BPM: %d\n", username, bpm);

    return 0;
}