/**
 * @file event_queue.c
 * @brief Fichier source pour la file d'événements
 * @see event_queue.h
 */

#include "event/event_queue.h"


/**
 * @fn event_queue_t *create_event_queue(int size)
 * @brief Crée une file d'événements
 * @param size La taille de la file
 * @return event_queue_t* La file d'événements créée
 */
event_queue_t *create_event_queue() {
	event_queue_t *queue = (event_queue_t *)malloc(sizeof(event_queue_t));
	CHECK_ALLOC(queue);
	queue->head = NULL;
	queue->tail = NULL;
	queue->size = 0;
	// initialisation des sémaphores
	sem_init(&queue->semEmpty, 0, 0); // bloquer la consommation si la file est vide
	sem_init(&queue->semMutex, 0, 1); // mutex pour protéger la file
	return queue;
}

/**
 * @fn void destroy_event_queue(event_queue_t *queue)
 * @brief Détruit une file d'événements
 * @param queue La file d'événements à détruire
 */
void destroy_event_queue(event_queue_t *queue) {
	event_node_t *node = NULL;
	while(queue->head != NULL) {
		node = queue->head;
		queue->head = node->next;
		destroy_event(node->event);
		free(node);
	}
	// Destruction des sémaphores
	sem_destroy(&queue->semEmpty);
	sem_destroy(&queue->semMutex);
	// Destruction de la file
	free(queue);
}

/**
 * @fn void notify_event(event_queue_t *queue, event_t *event)
 * @brief Notifie un événement
 * @param queue La file d'événements
 * @param event L'événement à notifier
 * @note Cette fonction ajoute un événement à la file
 */
void notify_event(event_queue_t *queue, event_t *event) {
	// Creation du noeud
	event_node_t *node = (event_node_t *)malloc(sizeof(event_node_t));
	CHECK_ALLOC(node);
	node->event = event;
	node->next = NULL;

	// Ecriture dans la file
	sem_wait(&queue->semMutex); // on attend que la file soit libre
	if(queue->head == NULL) {
		queue->head = node;
		queue->tail = node;
	} else {
		queue->tail->next = node;
		queue->tail = node;
	}
	queue->size++;
	sem_post(&queue->semMutex); // on libère la file
	sem_post(&queue->semEmpty); // on indique qu'il y a un événement
}

/**
 * @fn event_t *wait_event(event_queue_t *queue)
 * @brief Attend un événement
 * @param queue La file d'événements
 * @return event_t* L'événement reçu
 * @note Cette fonction attend un événement de la file
 */
event_t *wait_event(event_queue_t *queue) {
	sem_wait(&queue->semEmpty); // on attend qu'il y ait un événement
	sem_wait(&queue->semMutex); // on protège la file pour la lecture
	event_node_t *node = queue->head;
	event_t *event = node->event;
	queue->head = node->next;
	queue->size--;
	if(queue->size == 0) {
		queue->tail = NULL;
	}
	free(node);
	sem_post(&queue->semMutex); // on libère la file
	return event;
}

/**
 * @fn event_t trywait_event(event_queue_t *queue)
 * @brief Attend un événement sans bloquer
 * @param queue La file d'événements
 * @return event_t* L'événement reçu ou NULL si la file est vide
 * @note Permet de vérifier si un événement est disponible et pouvoir faire autre chose si ce n'est pas le cas
 */
event_t *trywait_event(event_queue_t *queue) {
	if(sem_trywait(&queue->semEmpty) == -1) {
		return NULL;
	}
	sem_wait(&queue->semMutex); // on protège la file pour la lecture
	event_node_t *node = queue->head;
	event_t *event = node->event;
	queue->head = node->next;
	queue->size--;
	if(queue->size == 0) {
		queue->tail = NULL;
	}
	free(node);
	sem_post(&queue->semMutex); // on libère la file
	return event;
}
