/**
 * @file fx_fuzz.test.c
 * @brief Tests unitaires pour l'effet Fuzz
 */

#include "unity.h"
#include "sound/fx/fx_fuzz.h"
#include <math.h>

#define DELTA 1e-6

void setUp(void) {}
void tearDown(void) {}

/**
 * @brief Teste la création avec et sans configuration valide
 */
void test_fx_fuzz_create_and_destroy(void) {
    fx_t *fx_null = fx_fuzz_create(NULL);
    TEST_ASSERT_NULL(fx_null);

    fx_fuzz_config_t config = { .drive = 5.0 };
    fx_t *fx = fx_fuzz_create(&config);
    
    TEST_ASSERT_NOT_NULL(fx);
    TEST_ASSERT_EQUAL_PTR(&config, fx->state);
    TEST_ASSERT_NOT_NULL(fx->process);
    TEST_ASSERT_NOT_NULL(fx->destroy);

    fx->destroy(fx);
}

/**
 * @brief Teste que le paramètre "drive" est bien clampé à un minimum de 1.0
 */
void test_fx_fuzz_clamp_drive(void) {
    fx_fuzz_config_t config = { .drive = 0.5 };
    fx_t *fx = fx_fuzz_create(&config);
    
    double buffer[1] = { 0.5 };
    fx->process(fx, buffer, 1);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, tanh(0.5), buffer[0]);

    fx->destroy(fx);
}

/**
 * @brief Teste l'application de la saturation avec un drive élevé (Soft Clipping)
 */
void test_fx_fuzz_high_drive(void) {
    fx_fuzz_config_t config = { .drive = 10.0 };
    fx_t *fx = fx_fuzz_create(&config);
    
    double buffer[2] = { 0.2, -0.2 };
    fx->process(fx, buffer, 2);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, tanh(2.0), buffer[0]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, tanh(-2.0), buffer[1]);

    fx->destroy(fx);
}

/**
 * @brief Vérifie qu'un appel avec size = 0 ne modifie rien
 */
void test_fx_fuzz_process_zero_size(void) {
    fx_fuzz_config_t config = { .drive = 5.0 };
    fx_t *fx = fx_fuzz_create(&config);
    
    double buffer[1] = { 0.8 };
    fx->process(fx, buffer, 0);

    TEST_ASSERT_EQUAL_DOUBLE(0.8, buffer[0]);

    fx->destroy(fx);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_fx_fuzz_create_and_destroy);
    RUN_TEST(test_fx_fuzz_clamp_drive);
    RUN_TEST(test_fx_fuzz_high_drive);
    RUN_TEST(test_fx_fuzz_process_zero_size);

    return UNITY_END();
}