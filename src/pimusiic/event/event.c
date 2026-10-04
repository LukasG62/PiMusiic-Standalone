/**
 * @file event.c
 * @brief Fichier source pour les événements spécifiques à PiMusiic
 */

#include "pimusiic/event/event.h"
#include "common.h"
#include <stdlib.h>

const char *event_type2str(event_type_t type) {
    switch (type) {
        // UI Events
        case UI_EVENT_NONE: return "UI_NONE";
        case UI_EVENT_REQUEST_SUBMITTED: return "UI_REQUEST_SUBMITTED";
        case UI_EVENT_VIEW_CHANGED: return "UI_VIEW_CHANGED";
        case UI_EVENT_VIEW_GO_BACK: return "UI_VIEW_GO_BACK";
        case UI_EVENT_MUSICPLAYBACK_STARTED: return "UI_MUSICPLAYBACK_STARTED";
        case UI_EVENT_MUSICPLAYBACK_STOPPED: return "UI_MUSICPLAYBACK_STOPPED";
        case UI_EVENT_NOTE_PLAYED: return "UI_EVENT_NOTE_PLAYED";
        case UI_EVENT_KEY_PRESSED: return "UI_KEY_PRESSED";
        case UI_EVENT_TICK: return "UI_TICK";

        // LOGIC Events
        case LOGIC_EVENT_NONE: return "LOGIC_NONE";
        case LOGIC_EVENT_REQUEST_SENT: return "LOGIC_REQUEST_SENT";
        case LOGIC_EVENT_RESPONSE_RECEIVED: return "LOGIC_RESPONSE_RECEIVED";
        case LOGIC_EVENT_REQUEST_FAILED: return "LOGIC_REQUEST_FAILED";
        case LOGIC_EVENT_NOTE_PLAYED: return "LOGIC_NOTE_PLAYED";
        case LOGIC_EVENT_SOUNDCARD_ERROR: return "LOGIC_SOUNDCARD_ERROR";
        case LOGIC_EVENT_AUTOSAVE_STARTED: return "LOGIC_AUTOSAVE_STARTED";
        case LOGIC_EVENT_MUSIC_SAVED: return "LOGIC_MUSIC_SAVED";

        // INET Events
        case INET_EVENT_NONE: return "INET_NONE";
        case INET_REQUEST_SENT: return "INET_REQUEST_SENT";
        case INET_RESPONSE_RECEIVED: return "INET_RESPONSE_RECEIVED";
        case INET_ERROR: return "INET_ERROR";
        default: return "EVENT_UNKNOWN";
    }
}

event_t *pimusiic_event_create_view_changed(intptr_t target_view) {
    return create_event(UI_EVENT_VIEW_CHANGED, (void *)target_view, NULL);
}

event_t *pimusiic_event_create_view_go_back(void) {
    return create_event(UI_EVENT_VIEW_GO_BACK, NULL, NULL);
}

event_t *pimusiic_event_create_key_pressed(int key) {
    return create_event(UI_EVENT_KEY_PRESSED, (void *)(intptr_t)key, NULL);
}

event_t *pimusiic_event_create_tick(void) {
    return create_event(UI_EVENT_TICK, NULL, NULL);
}

event_t *pimusiic_event_create_playback_started(void) {
    return create_event(UI_EVENT_MUSICPLAYBACK_STARTED, NULL, NULL);
}

event_t *pimusiic_event_create_playback_stopped(void) {
    return create_event(UI_EVENT_MUSICPLAYBACK_STOPPED, NULL, NULL);
}

event_t *pimusiic_event_create_note_played(int channel, int line) {
    event_note_played_data_t *payload = (event_note_played_data_t *)malloc(sizeof(event_note_played_data_t));
    CHECK_ALLOC(payload);
    payload->channelId = channel;
    payload->lineIndex = line;
    return create_event(UI_EVENT_NOTE_PLAYED, payload, free);
}
