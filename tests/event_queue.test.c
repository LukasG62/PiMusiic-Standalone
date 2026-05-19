/**
 * @file event_queue.test.c
 * @brief Tests unitaires pour la file d'événements
 */

#include "unity.h"
#include <pthread.h>
#include <unistd.h>
#include "event/event_queue.h"
#include "event/event.h"

typedef struct {
    event_queue_t *sharedQueue;
    size_t threadId;
    int events_processed;
} thread_data_t;

void *thread_trywait_event(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    while (data->events_processed < 10) {
        event_t *event = wait_event(data->sharedQueue);
        if (event != NULL) {
            destroy_event(event);
            data->events_processed++;
        }
    }
    return NULL;
}

void *thread_notify_event(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int count = 0;
    while (count < 10) {
        event_t *event = create_event(LOGIC_EVENT_REQUEST_SENT, NULL, NULL);
        notify_event(data->sharedQueue, event);
        count++;
        usleep(10000);
    }
    return NULL;
}

void setUp(void) {

}

void tearDown(void) {

}

/**
 * @brief Teste la création et l'initialisation de la file
 */
void test_create_event_queue(void) {
    event_queue_t *queue = create_event_queue();
    
    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NULL(queue->head);
    TEST_ASSERT_NULL(queue->tail);
    TEST_ASSERT_EQUAL_INT(0, queue->size);
    
    destroy_event_queue(queue);
}

/**
 * @brief Teste l'ajout et le retrait en respectant l'ordre FIFO
 */
void test_queue_fifo_behavior(void) {
    event_queue_t *queue = create_event_queue();

    event_t *empty_res = trywait_event(queue);
    TEST_ASSERT_NULL(empty_res);

    event_t *ev1 = create_event(UI_EVENT_NOTE_PLAYED, NULL, NULL);
    event_t *ev2 = create_event(LOGIC_EVENT_RESPONSE_RECEIVED, NULL, NULL);
    
    notify_event(queue, ev1);
    TEST_ASSERT_EQUAL_INT(1, queue->size);
    TEST_ASSERT_EQUAL_PTR(ev1, queue->head->event);
    TEST_ASSERT_EQUAL_PTR(ev1, queue->tail->event);

    notify_event(queue, ev2);
    TEST_ASSERT_EQUAL_INT(2, queue->size);
    TEST_ASSERT_EQUAL_PTR(ev1, queue->head->event);
    TEST_ASSERT_EQUAL_PTR(ev2, queue->tail->event);
    
    event_t *res1 = trywait_event(queue);
    TEST_ASSERT_EQUAL_PTR(ev1, res1);
    TEST_ASSERT_EQUAL_INT(1, queue->size);
    
    event_t *res2 = trywait_event(queue);
    TEST_ASSERT_EQUAL_PTR(ev2, res2);
    TEST_ASSERT_EQUAL_INT(0, queue->size);
    TEST_ASSERT_NULL(queue->head);
    TEST_ASSERT_NULL(queue->tail);
    
    destroy_event(ev1);
    destroy_event(ev2);
    destroy_event_queue(queue);
}

/**
 * @brief Teste que wait_event retourne immédiatement si la file n'est pas vide
 */
void test_wait_event_non_blocking_if_not_empty(void) {
    event_queue_t *queue = create_event_queue();
    event_t *ev = create_event(INET_REQUEST_SENT, NULL, NULL);
    
    notify_event(queue, ev);
    
    event_t *res = wait_event(queue);
    TEST_ASSERT_EQUAL_PTR(ev, res);
    
    destroy_event(ev);
    destroy_event_queue(queue);
}

/**
 * @brief Teste la file avec un thread producteur et un thread consommateur
 */
void test_event_queue_multithread(void) {
    thread_data_t data1 = { .threadId = 1, .events_processed = 0 };
    thread_data_t data2 = { .threadId = 2, .events_processed = 0 };
    
    data1.sharedQueue = create_event_queue();
    data2.sharedQueue = data1.sharedQueue;
    
    TEST_ASSERT_NOT_NULL(data1.sharedQueue);

    pthread_t sender_thread, receiver_thread;
    
    pthread_create(&sender_thread, NULL, thread_notify_event, &data1);
    pthread_create(&receiver_thread, NULL, thread_trywait_event, &data2);

    pthread_join(sender_thread, NULL);
    pthread_join(receiver_thread, NULL);

    TEST_ASSERT_EQUAL_INT(10, data2.events_processed);
    TEST_ASSERT_EQUAL_INT(0, data1.sharedQueue->size);

    destroy_event_queue(data1.sharedQueue);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_create_event_queue);
    RUN_TEST(test_queue_fifo_behavior);
    RUN_TEST(test_wait_event_non_blocking_if_not_empty);
    RUN_TEST(test_event_queue_multithread);
    
    return UNITY_END();
}