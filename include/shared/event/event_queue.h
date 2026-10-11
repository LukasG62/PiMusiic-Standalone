/**
 * @file event_queue.h
 * @brief Fichier d'en-tête pour la file d'événements thread-safe générique
 */
#ifndef SHARED_EVENT_QUEUE_H
#define SHARED_EVENT_QUEUE_H

#include "shared/event/event.h"
#include <semaphore.h>
#include <stddef.h>

/**
 * @struct event_node_t
 * @brief Structure pour un noeud de la file d'événements
 */
typedef struct __event_node_t {
    event_t *event;
    struct __event_node_t *next;
} event_node_t;

/**
 * @struct event_queue_t
 * @brief Structure pour une file d'événements thread-safe
 */
typedef struct {
    event_node_t *head;
    event_node_t *tail;
    size_t size;
    sem_t semEmpty;
    sem_t semMutex;
} event_queue_t;

/**
 * @brief Crée une file d'événements
 * @return event_queue_t* La file créée
 */
event_queue_t *create_event_queue(void);

/**
 * @brief Détruit une file d'événements et tous ses événements restants
 * @param queue La file à détruire
 */
void destroy_event_queue(event_queue_t *queue);

/**
 * @brief Ajoute un événement à la file de manière thread-safe
 * @param queue La file d'événements
 * @param event L'événement à ajouter
 */
void notify_event(event_queue_t *queue, event_t *event);

/**
 * @brief Attend et retire le prochain événement de la file (bloquant)
 * @param queue La file d'événements
 * @return event_t* L'événement retiré
 */
event_t *wait_event(event_queue_t *queue);

/**
 * @brief Tente de retirer le prochain événement sans bloquer
 * @param queue La file d'événements
 * @return event_t* L'événement retiré ou NULL si la file est vide
 */
event_t *trywait_event(event_queue_t *queue);

#endif // SHARED_EVENT_QUEUE_H
