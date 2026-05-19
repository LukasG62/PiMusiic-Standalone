
/**
 * @file view_credits.c
 * @brief Fichier source pour la vue des crédits de l'interface graphique
 * @version 1.0
 * @author Lukas Grando
 */

#include "views.h"

// 1. L'état interne de notre composant d'animation
typedef struct {
    WINDOW *win;
    int x;
    int y;
    int direction;
    int max_x;
    int max_y;
} credits_anim_state_t;

static void credits_anim_draw(ui_component_t *self) {
    if(!self || !self->data) return;
    credits_anim_state_t *state = (credits_anim_state_t *)self->data;
    WINDOW *body = state->win;

    const char *credits[] = {
        "Application created by Lukas Grando",
        "Version : " APP_VERSION,
        "Date : " __DATE__ " " __TIME__,
        "Initially created by Lukas & CaptainBidou as a school project using JOY-PI",
        "Using ncurses libasound",
        "Press any key to return"
    };
    int count = ARRAY_SIZE(credits);

    for(int i = 0; i < count; i++) {
        mvwprintw(body, i*2 + MENU_MARGIN, MENU_MARGIN, "%s", credits[i]);
    }
	
    int colorPair = COLOR_PAIR_MENU + (rand() % (COLOR_PAIR_MENU_END - COLOR_PAIR_MENU));
    
    wattron(body, A_REVERSE);
    wattron(body, COLOR_PAIR(colorPair));
    mvwprintw(body, state->y, state->x, "%d", rand() % 2);
    wattroff(body, COLOR_PAIR(colorPair));
    wattroff(body, A_REVERSE);
}

static bool credits_anim_handle_event(ui_component_t *self, event_t *event) {
    if(!self || !self->data) return false;
    credits_anim_state_t *state = (credits_anim_state_t *)self->data;

    if (event->type == UI_EVENT_TICK) {
        state->x += (state->direction == 0) - (state->direction == 2);
        state->y += (state->direction == 1) - (state->direction == 3);

        if (state->x >= state->max_x && state->direction == 0) state->direction = 1;
        else if (state->y >= state->max_y && state->direction == 1) state->direction = 2;
        else if (state->x <= 0 && state->direction == 2) state->direction = 3;
        else if (state->y <= 0 && state->direction == 3) state->direction = 0;
        
        return true;
    } 

    else if (event->type == UI_EVENT_KEY_PRESSED) {
        event_t *ev = create_event(UI_EVENT_VIEW_CHANGED, (void *)(intptr_t)VIEW_MAIN_MENU, NULL);
        notify_event(GET_UI_QUEUE(), ev);
        return true;
    }

    return false;
}


static void credits_anim_destroy(ui_component_t *self) {
    if(!self || !self->data) return;
    free(self->data);
    free(self);
}


ui_view_t *view_credits() {
    ui_view_t *view = ui_menu_create_view("Credits", "About this project :");
    ui_menu_t *menu = (ui_menu_t *)view->data;

    ui_component_t *animComp = (ui_component_t *)malloc(sizeof(ui_component_t));
    credits_anim_state_t *state = (credits_anim_state_t *)malloc(sizeof(credits_anim_state_t));

    state->win = menu->body;
    state->x = 0;
    state->y = 0;
    state->direction = 0;
    state->max_x = getmaxx(menu->body) - 1;
    state->max_y = getmaxy(menu->body) - 1;

    animComp->data = state;
    animComp->isFocused = true;
    animComp->draw = credits_anim_draw;
    animComp->handle_event = credits_anim_handle_event;
    animComp->destroy = credits_anim_destroy;

    ui_view_add_component(view, animComp);

    return view;
}