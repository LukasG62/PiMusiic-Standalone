/**
 * @file osc_triangle.test.c
 * @brief Tests unitaires pour l'oscillateur d'onde triangulaire
 */

#include "unity.h"
#include "sound/osc/osc_triangle.h"
#include <math.h>

#define DELTA 1e-6

void setUp(void) {
}

void tearDown(void) {
}

/**
 * @brief Teste l'allocation, l'initialisation et la destruction de l'oscillateur
 */
void test_osc_triangle_create_and_destroy(void) {
    osc_t *osc = osc_triangle_create();
    
    TEST_ASSERT_NOT_NULL(osc);
    TEST_ASSERT_NOT_NULL(osc->state);
    
    TEST_ASSERT_NOT_NULL(osc->generate);
    TEST_ASSERT_NOT_NULL(osc->destroy);

    osc->destroy(osc);
}

/**
 * @brief Teste la forme d'onde générée
 * @details y = 2 * abs((phase/pi) - 1) - 1
 * phase = 0 -> y = 1.0
 * phase = pi/2 -> y = 0.0
 * phase = pi -> y = -1.0
 * phase = 3pi/2 -> y = 0.0
 */
void test_osc_triangle_generate_quarter_rate(void) {
    osc_t *osc = osc_triangle_create();
    double buffer[4];

    osc->generate(osc, 1000.0, 4000.0, buffer, 4);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 1.0, buffer[0]); // Début (crête haute)
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer[1]); // Descente (croisement zéro)
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, -1.0, buffer[2]); // Milieu (crête basse)
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer[3]); // Montée (croisement zéro)

    osc->destroy(osc);
}

/**
 * @brief Teste la continuité de la phase et le retour à zéro
 */
void test_osc_triangle_phase_wrap(void) {
    osc_t *osc = osc_triangle_create();
    
    double buffer[4];
    osc->generate(osc, 100.0, 400.0, buffer, 4);

    double next_sample;
    osc->generate(osc, 100.0, 400.0, &next_sample, 1);
    
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 1.0, next_sample);

    osc->destroy(osc);
}

/**
 * @brief Vérifie qu'un appel avec size = 0 ne modifie pas le buffer ni la phase
 */
void test_osc_triangle_generate_zero_size(void) {
    osc_t *osc = osc_triangle_create();
    double buffer[1] = { 99.0 };

    osc->generate(osc, 440.0, 44100.0, buffer, 0);

    TEST_ASSERT_EQUAL_DOUBLE(99.0, buffer[0]);
    
    osc->generate(osc, 100.0, 400.0, buffer, 1);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 1.0, buffer[0]);

    osc->destroy(osc);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_osc_triangle_create_and_destroy);
    RUN_TEST(test_osc_triangle_generate_quarter_rate);
    RUN_TEST(test_osc_triangle_phase_wrap);
    RUN_TEST(test_osc_triangle_generate_zero_size);

    return UNITY_END();
}