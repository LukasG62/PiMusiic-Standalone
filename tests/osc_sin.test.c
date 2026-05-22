/**
 * @file osc_sin.test.c
 * @brief Tests unitaires pour l'oscillateur sinusoïdal
 */

#include "unity.h"
#include "sound/osc/osc_sin.h"
#include <math.h>

#define DELTA 1e-6

void setUp(void) {
}

void tearDown(void) {
}

/**
 * @brief Teste l'allocation, l'initialisation et la destruction de l'oscillateur
 */
void test_osc_sin_create_and_destroy(void) {
    osc_t *osc = osc_sin_create();
    
    TEST_ASSERT_NOT_NULL(osc);
    TEST_ASSERT_NOT_NULL(osc->state);
    
    TEST_ASSERT_NOT_NULL(osc->generate);
    TEST_ASSERT_NOT_NULL(osc->destroy);

    osc->destroy(osc);
}

/**
 * @brief Teste la forme d'onde générée avec un ratio parfait
 * @details Si freq = 1000Hz et sampleRate = 4000Hz, on a 4 échantillons par cycle.
 * Les valeurs attendues de la phase sont : 0, pi/2, pi, 3pi/2
 * Les valeurs attendues du signal sont : 0.0, 1.0, 0.0, -1.0
 */
void test_osc_sin_generate_quarter_rate(void) {
    osc_t *osc = osc_sin_create();
    double buffer[4];
    osc->generate(osc, 1000.0, 4000.0, buffer, 4);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer[0]); // sin(0)
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 1.0, buffer[1]); // sin(pi/2)
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer[2]); // sin(pi)
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, -1.0, buffer[3]); // sin(3pi/2)

    osc->destroy(osc);
}

/**
 * @brief Teste la continuité de la phase et le "wrap around" (retour à 0)
 */
void test_osc_sin_phase_wrap(void) {
    osc_t *osc = osc_sin_create();
    
    double buffer[4];
    osc->generate(osc, 100.0, 400.0, buffer, 4);
    
    double next_sample;
    osc->generate(osc, 100.0, 400.0, &next_sample, 1);
    
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, next_sample);

    osc->destroy(osc);
}

/**
 * @brief Teste que générer un buffer de taille 0 ne fait rien planter
 */
void test_osc_sin_generate_zero_size(void) {
    osc_t *osc = osc_sin_create();
    double buffer[10] = { 42.0 };

    osc->generate(osc, 440.0, 44100.0, buffer, 0);

    TEST_ASSERT_EQUAL_DOUBLE(42.0, buffer[0]);

    osc->destroy(osc);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_osc_sin_create_and_destroy);
    RUN_TEST(test_osc_sin_generate_quarter_rate);
    RUN_TEST(test_osc_sin_phase_wrap);
    RUN_TEST(test_osc_sin_generate_zero_size);

    return UNITY_END();
}