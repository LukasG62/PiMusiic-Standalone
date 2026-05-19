/**
 * @file event_queue.h
 * @brief Fichier d'en-tête pour la file d'événements
 * Cette file permet de stocker des événements de manière sécurisée (thread-safe)
 * Ce fichier contient egalement une structure avec 2 files pour la communication bidirectionnelle entre les threads
 * todo: refléchir si on devrait pas la foutre dans un autre fichier. Ou une seul file avec une distinction entre les event addressés à l'interface et ceux adressés à la logique
 * @see event.h
 */

#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "event/event.h" //!< Inclusion de la structure event_t
#include "common.h" //!< Inclusion des macros communes
#include <semaphore.h> //!< Inclusion de la bibliothèque pour les sémaphores

/**
 * @struct event_node_t
 * @brief Structure pour un noeud de file d'événements
 * @details Cette structure permet de définir un noeud de file d'événements
 */
typedef struct __event_node_t {
	event_t *event; /*!< L'événement */
	struct __event_node_t *next; /*!< Le prochain noeud */
} event_node_t;

/**
 * @struct event_queue_t
 * @brief Structure pour une file d'événements
 * @details Cette structure permet de définir une file d'événements
 * Elle est thread-safe
 */
typedef struct {
	event_node_t *head; /*!< La tête de la file */
	event_node_t *tail; /*!< La queue de la file */
	size_t size; /*!< La taille de la file */
	sem_t semEmpty; /*!< Sémaphore pour indiquer que la file est vide */
	sem_t semMutex; /*!< Sémaphore pour protéger la file */
} event_queue_t;

/**
 * @fn event_queue_t *create_event_queue(int size)
 * @brief Crée une file d'événements
 * @return event_queue_t* La file d'événements créée
 */
event_queue_t *create_event_queue();

/**
 * @fn void destroy_event_queue(event_queue_t *queue)
 * @brief Détruit une file d'événements
 * @param queue La file d'événements à détruire
 */
void destroy_event_queue(event_queue_t *queue);

/**
 * @fn void notify_event(event_queue_t *queue, event_t *event)
 * @brief Notifie un événement
 * @param queue La file d'événements
 * @param event L'événement à notifier
 * @note Cette fonction ajoute un événement à la file
 */
void notify_event(event_queue_t *queue, event_t *event);

/**
 * @fn event_t *wait_event(event_queue_t *queue)
 * @brief Attend un événement
 * @param queue La file d'événements
 * @return event_t* L'événement reçu
 * @note Cette fonction attend un événement de la file
 */
event_t *wait_event(event_queue_t *queue);

/**
 * @fn event_t trywait_event(event_queue_t *queue)
 * @brief Attend un événement sans bloquer
 * @param queue La file d'événements
 * @return event_t* L'événement reçu ou NULL si la file est vide
 * @note Permet de vérifier si un événement est disponible et pouvoir faire autre chose si ce n'est pas le cas
 */
event_t *trywait_event(event_queue_t *queue);


#endif // EVENT_QUEUE_H