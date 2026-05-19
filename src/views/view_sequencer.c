/**
 * \file view_sequencer.c
 * \brief Vue principale du séquenceur
 */
#include "views.h"

#include "ui/sequencer/ui_seq_components.h"

#define UI_SEQ_INFO_X0 0
#define UI_SEQ_INFO_Y0 0
#define UI_SEQ_HELP_X0 53
#define UI_SEQ_HELP_Y0 0
#define UI_SEQ_BODY_X0 0
#define UI_SEQ_BODY_Y0 6
#define UI_SEQ_CH1_X0 7
#define UI_SEQ_CH2_X0 37
#define UI_SEQ_CH3_X0 67
#define UI_SEQ_CH_Y0 7

typedef struct {
    ui_seq_nav_t nav;
    music_t *music;
    bool need2save; 
    int currentMode; 
} view_seq_state_t;

static void view_sequencer_draw(ui_view_t *self) {
    if (!self) return;

    for (int i = 0; i < self->componentCount; i++) {
        if (i == self->focusedComponent) continue;
        
        ui_component_t *comp = self->components[i];
        if (comp && comp->draw) {
            comp->draw(comp);
        }
    }
    
    if (self->focusedComponent >= 0 && self->focusedComponent < self->componentCount) {
        ui_component_t *focused = self->components[self->focusedComponent];
        if (focused && focused->draw) {
            focused->draw(focused);
        }
    }
}

static bool view_sequencer_handle_event(ui_view_t *self, event_t *event) {
    if (!self || !self->data) return false;
    
    view_seq_state_t *state = (view_seq_state_t *)self->data;
    ui_seq_nav_t *nav = &(state->nav);

    if (event->type == UI_EVENT_KEY_PRESSED) {
        int key = (int)(intptr_t)event->data;

        if (key == KEY_SEQ_NAV_CH1 || key == KEY_SEQ_NAV_CH2 || key == KEY_SEQ_NAV_CH3) {
            nav->ch = key - '1';
            self->focusedComponent = 2 + nav->ch;
            return true;
        }

        if (key == KEY_SEQ_HELP) { 
            state->currentMode = (state->currentMode == NAVIGATION_MODE) ? EDIT_MODE : NAVIGATION_MODE;
            return true;
        }

        if (key == KEY_SEQ_PLAY) {
			// todo: envoyer un event pour jouer la musique
            return true;
        }

        if (key == KEY_SEQ_SAVE) {
			//todo: envoyer un event pour sauvegarder la musique sur le serv mpp
            state->need2save = false; 
            return true;
        }

        if (key == KEY_SEQ_QUIT) {
            event_t *quit_ev = create_event(UI_EVENT_VIEW_CHANGED, (void *)(intptr_t)VIEW_MAIN_MENU, NULL);
            notify_event(GET_UI_QUEUE(), quit_ev); 
            return true;
        }
    }

    ui_component_t *focused = self->components[self->focusedComponent];
    if (focused && focused->handle_event) {
        int old_ch = nav->ch;
        bool consumed = focused->handle_event(focused, event);

        if (nav->ch != old_ch) {
            self->focusedComponent = 2 + nav->ch;
        }
        return consumed;
    }
    return false;
}

static void view_sequencer_destroy(ui_view_t *self) {
    if (self && self->data) {
        free(self->data);
    }
}

static ui_seq_nav_t init_default_nav() {
    ui_seq_nav_t nav;
    nav.col = SEQUENCER_NAV_COL_LINE;
    nav.ch = SEQUENCER_NAV_CH1;
    for (int i = 0; i < SEQUENCER_NAV_CH_MAX; i++) {
        nav.start[i] = 0;
        nav.lines[i] = 0;
    }
    nav.playMode = 0;
    return nav;
}

ui_view_t *view_sequencer(app_context_t *ctx) {
    view_seq_state_t *state = (view_seq_state_t *)malloc(sizeof(view_seq_state_t));
    if (state == NULL) return NULL;
    
    state->nav = init_default_nav();
    state->music = &(ctx->music);
    state->need2save = false;
    state->currentMode = NAVIGATION_MODE;

    ui_view_t *view = ui_view_create(view_sequencer_draw, view_sequencer_handle_event, view_sequencer_destroy, (void *) state);
    if (view == NULL) {
        free(state);
        return NULL;
    }

    ui_component_t *infoBox = ui_seq_create_info_box(UI_SEQ_INFO_X0, UI_SEQ_INFO_Y0, state->music, &(state->currentMode), &(state->need2save));
    ui_view_add_component(view, infoBox);

    ui_component_t *helpBox = ui_seq_create_help_box(UI_SEQ_HELP_X0, UI_SEQ_HELP_Y0);
    ui_view_add_component(view, helpBox);

    int channel_x_positions[] = {UI_SEQ_CH1_X0, UI_SEQ_CH2_X0, UI_SEQ_CH3_X0};
    for (int i = 0; i < MUSIC_MAX_CHANNELS; i++) {
        ui_component_t *channel = ui_seq_create_channel(
            channel_x_positions[i], 
            UI_SEQ_CH_Y0, 
            i, 
            state->music, 
            &(state->nav),
			&(state->currentMode)
        );
        ui_view_add_component(view, channel);
    }

    return view;
}