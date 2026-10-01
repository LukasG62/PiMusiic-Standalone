/**
 * @file osc_square.test.c
 * @brief Tests unitaires pour l'oscillateur d'onde carrée
 */

#include "unity.h"
#include "sound/osc/osc_square.h"
#include <math.h>

#define DELTA 1e-6

void setUp(void) {}
void tearDown(void) {}

/**
 * @brief Teste l'allocation, l'initialisation et la destruction de l'oscillateur
 */
void test_osc_square_create_and_destroy(void) {
    osc_t *osc = osc_square_create();
    
    TEST_ASSERT_NOT_NULL(osc);
    TEST_ASSERT_NOT_NULL(osc->state);
    
    TEST_ASSERT_NOT_NULL(osc->generate);
    TEST_ASSERT_NOT_NULL(osc->destroy);

    osc->destroy(osc);
}

/**
 * @brief Teste la forme d'onde générée avec 4 échantillons par cycle
 * @details Fréquence = 1000 Hz, SampleRate = 4000 Hz.
 * L'incrément de phase est de PI/2.
 * - Sample 0 (phase 0) -> 1.0
 * - Sample 1 (phase PI/2) -> 1.0
 * - Sample 2 (phase PI) -> -1.0
 * - Sample 3 (phase 3*PI/2) -> -1.0
 */
void test_osc_square_generate_quarter_rate(void) {
    osc_t *osc = osc_square_create();
    double buffer[4];

    osc->generate(osc, 1000.0, 4000.0, buffer, 4);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 1.0, buffer[0]); // Début du cycle (Haut)
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 1.0, buffer[1]); // 1/4 du cycle (Haut)
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, -1.0, buffer[2]); // Moitié du cycle (Bas)
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, -1.0, buffer[3]); // 3/4 du cycle (Bas)

    osc->destroy(osc);
}

/**
 * @brief Teste la continuité de la phase et le retour à zéro (wrap around)
 */
void test_osc_square_phase_wrap(void) {
    osc_t *osc = osc_square_create();
    
    double buffer[4];
    osc->generate(osc, 100.0, 400.0, buffer, 4);

    double next_sample;
    osc->generate(osc, 100.0, 400.0, &next_sample, 1);
    
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 1.0, next_sample);

    osc->destroy(osc);
}

/**
 * @brief Vérifie qu'un appel avec size = 0 ne modifie rien
 */
void test_osc_square_generate_zero_size(void) {
    osc_t *osc = osc_square_create();
    double buffer[1] = { 42.0 };

    osc->generate(osc, 440.0, 44100.0, buffer, 0);

    TEST_ASSERT_EQUAL_DOUBLE(42.0, buffer[0]);

    osc->destroy(osc);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_osc_square_create_and_destroy);
    RUN_TEST(test_osc_square_generate_quarter_rate);
    RUN_TEST(test_osc_square_phase_wrap);
    RUN_TEST(test_osc_square_generate_zero_size);

    return UNITY_END();
}