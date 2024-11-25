/**
 * @file event.c
 * @brief Fichier source pour les événements
 * @see event.h
 */

#include "event/event.h"
#include "common.h"

/**
 * @fn event_t *create_event(event_type_t type, void *data, fn_destroy_data_t destroy_data)
 * @brief Crée un événement avec un type et des données associées
 * @param type Le type de l'événement
 * @param data Les données de l'événement (peuvent être NULL)
 * @param destroy_data La fonction pour détruire les données (peut être NULL)
 * @return event_t* L'événement créé
 */
event_t *create_event(event_type_t type, void *data, fn_destroy_data_t destroy_data) {
	event_t *event = (event_t *)malloc(sizeof(event_t));
	CHECK_ALLOC(event);
	event->type = type;
	event->data = data;
	event->destroy_data = destroy_data;
	return event;
}

/**
 * @fn void destroy_event(event_t *event)
 * @brief Détruit un événement
 * @param event L'événement à détruire
 * @note Cette fonction détruit les données associées à l'événement
 */
void destroy_event(event_t *event) {
	if(event == NULL) return;
	if(event->destroy_data != NULL) {
		event->destroy_data(event->data);
	}
	free(event);
}

/**
 * @fn const char *event_type2str(event_type_t type)
 * @brief Convertit un type d'événement en chaîne de caractères
 * @param type Le type d'événement
 * @return const char* La chaîne de caractères correspondant au type d'événement
 * @note Cette fonction est utile pour le débogage
 */
const char *event_type2str(event_type_t type) {
	switch (type) {
		// Events provenant de l'interface graphique (UI)
		case UI_EVENT_NONE: return "UI_NONE";
		case UI_EVENT_REQUEST_SUBMITTED: return "UI_REQUEST_SUBMITTED";
		case UI_EVENT_VIEW_CHANGED: return "UI_VIEW_CHANGED";
		case UI_EVENT_MUSICPLAYBACK_STARTED: return "UI_MUSICPLAYBACK_STARTED";
		case UI_EVENT_MUSICPLAYBACK_STOPPED: return "UI_MUSICPLAYBACK_STOPPED";
		case UI_NOTE_DISPLAYED: return "UI_NOTE_DISPLAYED";

		// Events provenant de la logique (LOGIC)
		case LOGIC_EVENT_NONE: return "LOGIC_NONE";
		case LOGIC_EVENT_REQUEST_SENT: return "LOGIC_REQUEST_SENT";
		case LOGIC_EVENT_RESPONSE_RECEIVED: return "LOGIC_RESPONSE_RECEIVED";
		case LOGIC_EVENT_REQUEST_FAILED: return "LOGIC_REQUEST_FAILED";
		case LOGIC_EVENT_NOTE_PLAYED: return "LOGIC_NOTE_PLAYED";
		case LOGIC_EVENT_SOUNDCARD_ERROR: return "LOGIC_SOUNDCARD_ERROR";
		case LOGIC_EVENT_AUTOSAVE_STARTED: return "LOGIC_AUTOSAVE_STARTED";
		case LOGIC_EVENT_MUSIC_SAVED: return "LOGIC_MUSIC_SAVED";

		// Events provenant du client inet (INET)
		case INET_EVENT_NONE: return "INET_NONE";
		case INET_REQUEST_SENT: return "INET_REQUEST_SENT";
		case INET_RESPONSE_RECEIVED: return "INET_RESPONSE_RECEIVED";
		case INET_ERROR: return "INET_ERROR";
		default: return "EVENT_UNKNOWN";
	}
}

