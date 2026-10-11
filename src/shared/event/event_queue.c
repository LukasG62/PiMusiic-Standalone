/**
 * @file event_queue.c
 * @brief Fichier source pour la file d'événements
 */

#include "shared/event/event_queue.h"
#include "common.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

event_queue_t *create_event_queue(void) {
    event_queue_t *queue = (event_queue_t *)malloc(sizeof(event_queue_t));
    CHECK_ALLOC(queue);
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    sem_init(&queue->semEmpty, 0, 0);
    sem_init(&queue->semMutex, 0, 1);
    return queue;
}

void destroy_event_queue(event_queue_t *queue) {
    if (queue == NULL) return;
    event_node_t *node = NULL;
    while (queue->head != NULL) {
        node = queue->head;
        queue->head = node->next;
        destroy_event(node->event);
        free(node);
    }
    sem_destroy(&queue->semEmpty);
    sem_destroy(&queue->semMutex);
    free(queue);
}

void notify_event(event_queue_t *queue, event_t *event) {
    if (queue == NULL || event == NULL) return;

    event_node_t *node = (event_node_t *)malloc(sizeof(event_node_t));
    CHECK_ALLOC(node);
    node->event = event;
    node->next = NULL;

    sem_wait(&queue->semMutex);
    if (queue->head == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
    sem_post(&queue->semMutex);
    sem_post(&queue->semEmpty);
}

event_t *wait_event(event_queue_t *queue) {
    while (sem_wait(&queue->semEmpty) == -1) {
        if (errno != EINTR) {
            perror("SEM_WAIT");
            exit(EXIT_FAILURE);
        }
    }
    sem_wait(&queue->semMutex);
    event_node_t *node = queue->head;
    event_t *event = node->event;
    queue->head = node->next;
    queue->size--;
    if (queue->size == 0) {
        queue->tail = NULL;
    }
    sem_post(&queue->semMutex);

    free(node);
    return event;
}

event_t *trywait_event(event_queue_t *queue) {
    if (sem_trywait(&queue->semEmpty) == -1) {
        return NULL;
    }
    sem_wait(&queue->semMutex);
    event_node_t *node = queue->head;
    event_t *event = node->event;
    queue->head = node->next;
    queue->size--;
    if (queue->size == 0) {
        queue->tail = NULL;
    }
    sem_post(&queue->semMutex);

    free(node);
    return event;
}
