/**
 * @file osc_noise.test.c
 * @brief Tests unitaires pour l'oscillateur de bruit (LFSR)
 */

#include "unity.h"
#include "sound/osc/osc_noise.h"
#include <math.h>

#define DELTA 1e-6

void setUp(void) {}
void tearDown(void) {}

/**
 * @brief Teste l'allocation, l'initialisation et la destruction de l'oscillateur
 */
void test_osc_noise_create_and_destroy(void) {
    osc_t *osc = osc_noise_create();
    
    TEST_ASSERT_NOT_NULL(osc);
    TEST_ASSERT_NOT_NULL(osc->state);
    
    TEST_ASSERT_NOT_NULL(osc->generate);
    TEST_ASSERT_NOT_NULL(osc->destroy);

    osc->destroy(osc);
}

/**
 * @brief Teste la phase initiale avant le premier déclenchement
 * @details Le bruit LFSR est un signal "Sample & Hold". 
 * Tant que la phase n'atteint pas 1.0, l'oscillateur conserve sa valeur précédente.
 * Au démarrage, ton code initialise currentValue à 0.0.
 */
void test_osc_noise_generate_quarter_rate(void) {
    osc_t *osc = osc_noise_create();
    double buffer[4];

    osc->generate(osc, 1000.0, 4000.0, buffer, 4);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer[0]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer[1]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer[2]);
    
    // Le 4ème échantillon déclenche le LFSR, la sortie vaut donc soit 1.0 soit -1.0
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, -1.0, buffer[3]);

    osc->destroy(osc);
}

/**
 * @brief Teste la séquence exacte générée par le LFSR
 * @details Si Freq == SampleRate, phaseInc = 1.0, le LFSR est mis à jour à CHAQUE échantillon.
 * 1. 0xACE1 -> next: 0xD670 -> bit0=0 -> -1.0
 * 2. 0xD670 -> next: 0xEB38 -> bit0=0 -> -1.0
 * 3. 0xEB38 -> next: 0xF59C -> bit0=0 -> -1.0
 * 4. 0xF59C -> next: 0x7ACE -> bit0=0 -> -1.0
 * 5. 0x7ACE -> next: 0xBD67 -> bit0=1 ->  1.0
 */
void test_osc_noise_lfsr_exact_sequence(void) {
    osc_t *osc = osc_noise_create();
    double buffer[5];

    osc->generate(osc, 44100.0, 44100.0, buffer, 5);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, -1.0, buffer[0]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, -1.0, buffer[1]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, -1.0, buffer[2]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, -1.0, buffer[3]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA,  1.0, buffer[4]);

    osc->destroy(osc);
}

/**
 * @brief Vérifie qu'un appel avec size = 0 ne modifie rien
 */
void test_osc_noise_generate_zero_size(void) {
    osc_t *osc = osc_noise_create();
    double buffer[1] = { 42.0 };

    osc->generate(osc, 440.0, 44100.0, buffer, 0);

    TEST_ASSERT_EQUAL_DOUBLE(42.0, buffer[0]);

    osc->destroy(osc);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_osc_noise_create_and_destroy);
    RUN_TEST(test_osc_noise_generate_quarter_rate);
    RUN_TEST(test_osc_noise_lfsr_exact_sequence);
    RUN_TEST(test_osc_noise_generate_zero_size);

    return UNITY_END();
}