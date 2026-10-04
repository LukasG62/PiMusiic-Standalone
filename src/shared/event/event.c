/**
 * @file event.c
 * @brief Fichier source pour les événements
 */

#include "shared/event/event.h"
#include "common.h"
#include <stdlib.h>

event_t *create_event(event_type_t type, void *data, fn_destroy_data_t destroy_data) {
    event_t *event = (event_t *)malloc(sizeof(event_t));
    CHECK_ALLOC(event);
    event->type = type;
    event->data = data;
    event->destroy_data = destroy_data;
    return event;
}

void destroy_event(event_t *event) {
    if (event == NULL) return;
    if (event->destroy_data != NULL) {
        event->destroy_data(event->data);
    }
    free(event);
}
