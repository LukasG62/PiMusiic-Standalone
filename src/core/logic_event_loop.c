/**
 * @file logic_event_loop.c
 * @brief Implémentation de la boucle d'événements logique
 */
#include "core/logic_event_loop.h"

/**
 * @fn void *logic_event_loop_run(void *arg)
 * @brief Point d'entrée du thread logique
 * @param arg Arguments du thread
 */
void *logic_event_loop_run(void *arg) {
	UNUSED(arg);
    while (isRunning) {
        event_t *ev = WAIT_LOGIC_EVENT();
        if (ev) {
            switch (ev->type) {
				case LOGIC_EVENT_NONE: /*!< Emis quand il n'y a pas d'event (sert à diviser les types d'events) */
					break;
				case LOGIC_EVENT_REQUEST_SENT: /*!< Emis quand une requête a été envoyé au serveur */
					break;
				case LOGIC_EVENT_RESPONSE_RECEIVED: /*!< Emis quand une reponse a été reçue du serveur */
					break;
				case LOGIC_EVENT_REQUEST_FAILED: /*!< Emis quand une requête a échoué */
					break;
				case LOGIC_EVENT_NOTE_PLAYED: /*!< Emis quand une note a été jouée sur la carte son */
					break;
				case LOGIC_EVENT_SOUNDCARD_ERROR: /*!< Emis quand il y a une erreur avec la carte son */
					break;
				case LOGIC_EVENT_AUTOSAVE_STARTED: /*!< Emis quand l'autosave commence */
					break;
				case LOGIC_EVENT_MUSIC_SAVED: /*!< Emis quand la musique a été sauvegardée sur le serveur */
					break;
                default:
                    break;
            }
            destroy_event(ev);
        }
    }
    return NULL;
}