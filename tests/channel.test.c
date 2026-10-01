/**
 * @file channel.test.c
 */

#include "unity.h"
#include "music/channel.h"
#include <stdbool.h>

#define DUMMY_STEP_TYPE 1 
#define DUMMY_DURATION 120

void setUp(void) {}
void tearDown(void) {}

/**
 * @brief Teste l'initialisation du channel (valeurs par défaut et remplissage)
 */
void test_music_channel_init(void) {
    music_channel_t channel;
    
    music_channel_init(&channel, 42);
    
    TEST_ASSERT_EQUAL_UINT8(42, channel.id);
    TEST_ASSERT_EQUAL_UINT32(0, channel.nbSteps);
    
    TEST_ASSERT_EQUAL_INT(MUSIC_STEP_TYPE_REST, channel.steps[0].type);
    TEST_ASSERT_EQUAL_INT(MUSIC_TIME_ZERO, channel.steps[0].duration);
    
    TEST_ASSERT_EQUAL_INT(MUSIC_STEP_TYPE_REST, channel.steps[MUSIC_CHANNEL_MAX_STEPS - 1].type);
    TEST_ASSERT_EQUAL_INT(MUSIC_TIME_ZERO, channel.steps[MUSIC_CHANNEL_MAX_STEPS - 1].duration);
}

/**
 * @brief Teste l'écriture d'un step valide et la mise à jour de nbSteps
 */
void test_music_channel_write_step_valid(void) {
    music_channel_t channel;
    music_channel_init(&channel, 1);
    
    music_step_t step = { .type = DUMMY_STEP_TYPE, .duration = DUMMY_DURATION };
    
    bool res = music_channel_write_step(&channel, 5, step);
    
    TEST_ASSERT_TRUE(res);
    TEST_ASSERT_EQUAL_UINT32(6, channel.nbSteps);
    TEST_ASSERT_EQUAL_INT(DUMMY_STEP_TYPE, channel.steps[5].type);
    TEST_ASSERT_EQUAL_INT(DUMMY_DURATION, channel.steps[5].duration);
    
    music_step_t step2 = { .type = DUMMY_STEP_TYPE, .duration = 60 };
    res = music_channel_write_step(&channel, 2, step2);
    
    TEST_ASSERT_TRUE(res);
    TEST_ASSERT_EQUAL_UINT32(6, channel.nbSteps);
}

/**
 * @brief Teste la récupération d'un pointeur vers un step valide
 */
void test_music_channel_get_step_valid(void) {
    music_channel_t channel;
    music_channel_init(&channel, 1);
    
    music_step_t step = { .type = DUMMY_STEP_TYPE, .duration = DUMMY_DURATION };
    music_channel_write_step(&channel, 10, step);
    
    music_step_t *retrieved_step = music_channel_get_step(&channel, 10);
    
    TEST_ASSERT_NOT_NULL(retrieved_step);
    TEST_ASSERT_EQUAL_INT(DUMMY_STEP_TYPE, retrieved_step->type);
    TEST_ASSERT_EQUAL_INT(DUMMY_DURATION, retrieved_step->duration);
}

/**
 * @brief Teste la suppression d'un step (remplacement par REST mais conservation de la durée)
 */
void test_music_channel_delete_step(void) {
    music_channel_t channel;
    music_channel_init(&channel, 1);
    
    music_step_t step = { .type = DUMMY_STEP_TYPE, .duration = DUMMY_DURATION };
    music_channel_write_step(&channel, 3, step);
    
    music_channel_delete_step(&channel, 3);
    
    /* Le type doit être remis à REST, mais la durée doit être conservée (DUMMY_DURATION) */
    TEST_ASSERT_EQUAL_INT(MUSIC_STEP_TYPE_REST, channel.steps[3].type);
    TEST_ASSERT_EQUAL_INT(DUMMY_DURATION, channel.steps[3].duration);
}

/**
 * @brief Teste les protections contre les pointeurs NULL
 */
void test_music_channel_null_pointers(void) {
    music_step_t step = { .type = DUMMY_STEP_TYPE, .duration = DUMMY_DURATION };
    
    music_channel_init(NULL, 1);
    music_channel_delete_step(NULL, 0);
    
    bool write_res = music_channel_write_step(NULL, 0, step);
    TEST_ASSERT_FALSE(write_res);
    
    music_step_t *get_res = music_channel_get_step(NULL, 0);
    TEST_ASSERT_NULL(get_res);
}

/**
 * @brief Teste les protections contre les dépassements de capacité (Out of Bounds)
 */
void test_music_channel_out_of_bounds(void) {
    music_channel_t channel;
    music_channel_init(&channel, 1);
    
    music_step_t step = { .type = DUMMY_STEP_TYPE, .duration = DUMMY_DURATION };
    
    bool write_res = music_channel_write_step(&channel, MUSIC_CHANNEL_MAX_STEPS, step);
    TEST_ASSERT_FALSE(write_res);
    
    music_step_t *get_res = music_channel_get_step(&channel, MUSIC_CHANNEL_MAX_STEPS);
    TEST_ASSERT_NULL(get_res);
    
    /* Ne doit pas crasher */
    music_channel_delete_step(&channel, MUSIC_CHANNEL_MAX_STEPS); 
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_music_channel_init);
    RUN_TEST(test_music_channel_write_step_valid);
    RUN_TEST(test_music_channel_get_step_valid);
    RUN_TEST(test_music_channel_delete_step);
    RUN_TEST(test_music_channel_null_pointers);
    RUN_TEST(test_music_channel_out_of_bounds);

    return UNITY_END();
}