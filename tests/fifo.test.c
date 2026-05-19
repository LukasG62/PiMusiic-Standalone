/**
 * @file fifo.test.c
 * @brief Tests unitaires et d'intégration multithread pour la file FIFO
 */

#include "unity.h"
#include "core/fifo.h"
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

void setUp(void) {}
void tearDown(void) {}

/**
 * @brief Vérifie l'initialisation correcte de la FIFO
 */
void test_fifo_init_and_empty(void) {
    fifo_t fifo;
    fifo_init(&fifo);

    TEST_ASSERT_TRUE(fifo_is_empty(&fifo));
    TEST_ASSERT_EQUAL_INT(0, fifo.size);
    TEST_ASSERT_NULL(fifo.head);
    TEST_ASSERT_NULL(fifo.tail);

    fifo_destroy(&fifo);
}

/**
 * @brief Vérifie le fonctionnement basique de push et pop dans la FIFO
 */
void test_fifo_push_pop_basics(void) {
    fifo_t fifo;
    fifo_init(&fifo);

    int val1 = 10, val2 = 20, val3 = 30;

    fifo_push(&fifo, &val1);

    TEST_ASSERT_FALSE(fifo_is_empty(&fifo));
    TEST_ASSERT_EQUAL_INT(1, fifo.size);

    fifo_push(&fifo, &val2);
    fifo_push(&fifo, &val3);
    
	TEST_ASSERT_EQUAL_INT(3, fifo.size);
    TEST_ASSERT_EQUAL_PTR(&val1, fifo_pop(&fifo));
    TEST_ASSERT_EQUAL_INT(2, fifo.size);
    TEST_ASSERT_EQUAL_PTR(&val2, fifo_pop(&fifo));
    TEST_ASSERT_EQUAL_INT(1, fifo.size);
    TEST_ASSERT_EQUAL_PTR(&val3, fifo_pop(&fifo));
    TEST_ASSERT_TRUE(fifo_is_empty(&fifo));

    fifo_destroy(&fifo);
}

/**
 * @brief Vérifie le comportement lorsqu'on pop une FIFO vide
 */
void test_fifo_pop_empty(void) {
    fifo_t fifo;
    fifo_init(&fifo);

    TEST_ASSERT_NULL(fifo_pop(&fifo));

    int val = 42;
    fifo_push(&fifo, &val);
    TEST_ASSERT_EQUAL_PTR(&val, fifo_pop(&fifo));

    fifo_destroy(&fifo);
}

/**
 * @brief Vérifie que destroy libère bien la FIFO même si elle n'est pas vide
 */
void test_fifo_destroy_with_elements(void) {
    fifo_t fifo;
    fifo_init(&fifo);

    int val = 99;
    fifo_push(&fifo, &val);
    fifo_push(&fifo, &val);

    fifo_destroy(&fifo);

    TEST_ASSERT_EQUAL_INT(0, fifo.size);
    TEST_ASSERT_NULL(fifo.head);
    TEST_ASSERT_NULL(fifo.tail);
}

typedef struct {
    fifo_t *fifo;
    int items_to_process;
    long long sum_pushed;
    long long sum_popped;
} thread_test_data_t;

/**
 * @brief thread qui ajoute N éléments à la FIFO
 */
void *producer_thread(void *arg) {
    thread_test_data_t *data = (thread_test_data_t *)arg;
    for (intptr_t i = 1; i <= data->items_to_process; i++) {
        fifo_push(data->fifo, (void *)i);
        data->sum_pushed += i;
        usleep(500);
    }
    return NULL;
}

/**
 * @brief Thread qui retire N éléments
 */
void *consumer_thread(void *arg) {
    thread_test_data_t *data = (thread_test_data_t *)arg;
    int items_popped = 0;
    
    while (items_popped < data->items_to_process) {
        void *val = fifo_pop(data->fifo);
        if (val != NULL) {
            data->sum_popped += (intptr_t)val;
            items_popped++;
        } else {
            usleep(1000);
        }
    }
    return NULL;
}

/**
 * @brief Teste la FIFO avec des accès concurrents pour valider le thread-safety
 */
void test_fifo_multithread_safety(void) {
    fifo_t fifo;
    fifo_init(&fifo);

    thread_test_data_t data = {
        .fifo = &fifo,
        .items_to_process = 50,
        .sum_pushed = 0,
        .sum_popped = 0
    };
    pthread_t producer, consumer;

    pthread_create(&producer, NULL, producer_thread, &data);
    pthread_create(&consumer, NULL, consumer_thread, &data);

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    TEST_ASSERT_EQUAL_INT64(data.sum_pushed, data.sum_popped);
    TEST_ASSERT_TRUE(fifo_is_empty(&fifo));

    fifo_destroy(&fifo);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_fifo_init_and_empty);
    RUN_TEST(test_fifo_push_pop_basics);
    RUN_TEST(test_fifo_pop_empty);
    RUN_TEST(test_fifo_destroy_with_elements);
    RUN_TEST(test_fifo_multithread_safety);
    
    return UNITY_END();
}