/**
 * @file event.h
 * @brief Fichier d'en-tête pour les événements spécifiques au client PiMusiic
 */

#ifndef PIMUSIIC_EVENT_H
#define PIMUSIIC_EVENT_H

#include "shared/event/event.h"
#include "shared/event/event_queue.h"

/**
 * @enum pimusiic_event_type_t
 * @brief Types d'événements spécifiques au client PiMusiic
 */
typedef enum {
    // Événements de l'interface graphique (UI)
    UI_EVENT_NONE = 0,
    UI_EVENT_REQUEST_SUBMITTED,
    UI_EVENT_VIEW_CHANGED,
    UI_EVENT_VIEW_GO_BACK,
    UI_EVENT_MUSICPLAYBACK_STARTED,
    UI_EVENT_MUSICPLAYBACK_STOPPED,
    UI_EVENT_NOTE_PLAYED,
    UI_EVENT_KEY_PRESSED,
    UI_EVENT_TICK,

    // Événements de la logique métier (LOGIC)
    LOGIC_EVENT_NONE = 100,
    LOGIC_EVENT_REQUEST_SENT,
    LOGIC_EVENT_RESPONSE_RECEIVED,
    LOGIC_EVENT_REQUEST_FAILED,
    LOGIC_EVENT_NOTE_PLAYED,
    LOGIC_EVENT_SOUNDCARD_ERROR,
    LOGIC_EVENT_AUTOSAVE_STARTED,
    LOGIC_EVENT_MUSIC_SAVED,

    // Événements réseau (INET)
    INET_EVENT_NONE = 200,
    INET_REQUEST_SENT,
    INET_RESPONSE_RECEIVED,
    INET_ERROR,
} pimusiic_event_type_t;

/**
 * @struct event_note_played_data_t
 * @brief Données associées à l'événement UI_EVENT_NOTE_PLAYED
 */
typedef struct {
    int channelId;
    int lineIndex;
} event_note_played_data_t;

/**
 * @brief Convertit un type d'événement PiMusiic en chaîne de caractères
 * @param type Le type d'événement
 * @return const char* La représentation textuelle du type
 */
const char *event_type2str(event_type_t type);

/* 
 * Fonctions de création d'événements spécialisés pour PiMusiic
 */

/**
 * @brief Crée un événement de changement de vue
 * @param target_view L'identifiant de la vue cible
 * @return event_t* L'événement créé
 */
event_t *pimusiic_event_create_view_changed(intptr_t target_view);

/**
 * @brief Crée un événement de retour à la vue précédente
 * @return event_t* L'événement créé
 */
event_t *pimusiic_event_create_view_go_back(void);

/**
 * @brief Crée un événement de touche pressée
 * @param key Le code de la touche pressée
 * @return event_t* L'événement créé
 */
event_t *pimusiic_event_create_key_pressed(int key);

/**
 * @brief Crée un événement de tick d'animation / timer
 * @return event_t* L'événement créé
 */
event_t *pimusiic_event_create_tick(void);

/**
 * @brief Crée un événement de démarrage de la lecture musicale
 * @return event_t* L'événement créé
 */
event_t *pimusiic_event_create_playback_started(void);

/**
 * @brief Crée un événement d'arrêt de la lecture musicale
 * @return event_t* L'événement créé
 */
event_t *pimusiic_event_create_playback_stopped(void);

/**
 * @brief Crée un événement de note jouée
 * @param channel Le canal MIDI / piste
 * @param line La ligne / step joué
 * @return event_t* L'événement créé
 */
event_t *pimusiic_event_create_note_played(int channel, int line);

#endif // PIMUSIIC_EVENT_H
