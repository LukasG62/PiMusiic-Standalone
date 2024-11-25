/**
 * @file event.h
 * @brief Fichier d'en-tête pour les événements
 * Les events sont des structures qui permettent d'envoyer des informations entre differents threads
 * Dans ce projet, les events sont utilisés pour envoyer des informations entre le thread de l'interface graphique et le thread logique
 * Il est bidirectionnel, c'est à dire que les deux threads peuvent envoyer et recevoir des events
 */

#ifndef EVENT_H
#define EVENT_H

typedef void (*fn_destroy_data_t)(void *data); /*!< Fonction pour détruire les données associées à un événement */

/**
 * @enum event_type_t
 * @brief Enumération des types d'événements
 * @details Cette énumération permet de définir les différents types d'événements
 * @note les types d'events sont divisée en 2 catégories: les events de l'interface graphique et les events logiques
 */
typedef enum {
	// Events provenant de l'interface graphique (UI)
	UI_EVENT_NONE, /*!< Emis quand il n'y a pas d'event (sert à diviser les types d'events) */
	UI_EVENT_REQUEST_SUBMITTED, /*!< Emis quand l'utilisateur soumet un formulaire */
	UI_EVENT_VIEW_CHANGED, /*!< Emis quand l'utilisateur change de vue */
	UI_EVENT_MUSICPLAYBACK_STARTED, /*!< Emis quand la lecture de la musique commence */
	UI_EVENT_MUSICPLAYBACK_STOPPED, /*!< Emis quand la lecture de la musique s'arrête */
	UI_NOTE_DISPLAYED, /*!< Emis quand une note est affichée en mode lecture */

	// Evenment provenant de la logique (LOGIC)
	LOGIC_EVENT_NONE, /*!< Emis quand il n'y a pas d'event (sert à diviser les types d'events) */
	LOGIC_EVENT_REQUEST_SENT, /*!< Emis quand une requête a été envoyé au serveur */
	LOGIC_EVENT_RESPONSE_RECEIVED, /*!< Emis quand une reponse a été reçue du serveur */
	LOGIC_EVENT_REQUEST_FAILED, /*!< Emis quand une requête a échoué */
	LOGIC_EVENT_NOTE_PLAYED, /*!< Emis quand une note a été jouée sur la carte son */
	LOGIC_EVENT_SOUNDCARD_ERROR, /*!< Emis quand il y a une erreur avec la carte son */
	LOGIC_EVENT_AUTOSAVE_STARTED, /*!< Emis quand l'autosave commence */
	LOGIC_EVENT_MUSIC_SAVED, /*!< Emis quand la musique a été sauvegardée sur le serveur */

	// Evenement provenant du client inet (INET)
	// Todo: ça serait cool que le client inet soit dans un thread séparé et qu'il envoie des events à la logique
	INET_EVENT_NONE, /*!< Type d'événement réseau inconnu */
	INET_REQUEST_SENT, /*!< Emis quand une requête a été envoyée au serveur */
	INET_RESPONSE_RECEIVED, /*!< Emis quand une réponse a été reçue du serveur */
	INET_ERROR, /*!< Emis quand il y a une erreur avec le serveur */
} event_type_t;

/**
 * @struct event_t
 * @brief structure d'un événement
 */
typedef struct {
	event_type_t type; /*!< Type de l'événement */
	void *data; /*!< Données de l'événement */
	fn_destroy_data_t destroy_data; /*!< Fonction pour détruire les données */
} event_t;

/**
 * @fn event_t *create_event(event_type_t type, void *data, fn_destroy_data_t destroy_data)
 * @brief Crée un événement avec un type et des données associées
 * @param type Le type de l'événement
 * @param data Les données de l'événement (peuvent être NULL)
 * @param destroy_data La fonction pour détruire les données (peut être NULL)
 * @return event_t* L'événement créé
 */
event_t *create_event(event_type_t type, void *data, fn_destroy_data_t destroy_data);

/**
 * @fn void destroy_event(event_t *event)
 * @brief Détruit un événement
 * @param event L'événement à détruire
 * @note Cette fonction détruit les données associées à l'événement
 */
void destroy_event(event_t *event);


/**
 * @fn const char *event_type2str(event_type_t type)
 * @brief Convertit un type d'événement en chaîne de caractères
 * @param type Le type d'événement
 * @return const char* La chaîne de caractères correspondant au type d'événement
 * @note Cette fonction est utile pour le débogage
 */
const char *event_type2str(event_type_t type);

#endif // EVENT_H

