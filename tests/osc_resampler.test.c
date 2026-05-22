/**
 * @file osc_resampler.test.c
 * @brief Tests unitaires pour l'oscillateur de resampling (lecture de samples)
 */

#include "unity.h"
#include "sound/osc/osc_resampler.h"
#include <math.h>

#define DELTA 1e-6

static int16_t dummy_sample_data[] = {0, 10000, 20000, 30000, 10000};
static size_t dummy_sample_length = 5;

void setUp(void) {}
void tearDown(void) {}

/**
 * @brief Vérifie le comportement de sécurité avec une configuration NULL ou invalide
 */
void test_osc_resampler_invalid_config(void) {
    osc_resampler_config_t cfg = {
        .sampleData = NULL,
        .sampleLength = 0,
        .baseFrequency = 440.0
    };

    osc_t *osc = osc_resampler_create(&cfg); 
    TEST_ASSERT_NOT_NULL(osc);

    double buffer[4] = {1.0, 1.0, 1.0, 1.0};
    
    osc->generate(osc, 440.0, 44100.0, buffer, 4);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer[0]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer[3]);

    osc->destroy(osc);
}

/**
 * @brief Teste la lecture à vitesse normale (freq == baseFrequency)
 */
void test_osc_resampler_normal_speed(void) {
    osc_resampler_config_t cfg = {
        .sampleData = dummy_sample_data,
        .sampleLength = dummy_sample_length,
        .loopEnabled = false,
        .baseFrequency = 440.0
    };
    
    osc_t *osc = osc_resampler_create(&cfg);
    double buffer[3];

    osc->generate(osc, 440.0, 44100.0, buffer, 3);

	TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer[0]); // Index 0
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 10000.0 / NORMALIZATION_FACTOR_16B, buffer[1]); // Index 1
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 20000.0 / NORMALIZATION_FACTOR_16B, buffer[2]); // Index 2

    osc->destroy(osc);
}

/**
 * @brief Teste l'interpolation linéaire
 */
void test_osc_resampler_interpolation_half_speed(void) {
    osc_resampler_config_t cfg = {
        .sampleData = dummy_sample_data,
        .sampleLength = dummy_sample_length,
        .loopEnabled = false,
        .baseFrequency = 440.0
    };
    
    osc_t *osc = osc_resampler_create(&cfg);
    double buffer[3];

    osc->generate(osc, 220.0, 44100.0, buffer, 3);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer[0]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 5000.0 / NORMALIZATION_FACTOR_16B, buffer[1]); 
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 10000.0 / NORMALIZATION_FACTOR_16B, buffer[2]);

    osc->destroy(osc);
}

/**
 * @brief Teste l'arrêt automatique de la lecture à la fin du sample (sans loop)
 */
void test_osc_resampler_end_of_sample(void) {
    osc_resampler_config_t cfg = {
        .sampleData = dummy_sample_data,
        .sampleLength = dummy_sample_length, // 5
        .loopEnabled = false,
        .baseFrequency = 440.0
    };
    
    osc_t *osc = osc_resampler_create(&cfg);
    double buffer[6];

    osc->generate(osc, 440.0, 44100.0, buffer, 6);

	TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer[4]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer[5]);

    osc->destroy(osc);
}

/**
 * @brief Teste le bouclage (Loop wrapping)
 */
void test_osc_resampler_looping(void) {
    osc_resampler_config_t cfg = {
        .sampleData = dummy_sample_data,
        .sampleLength = dummy_sample_length,
        .loopEnabled = true,
        .loopStart = 1,
        .loopEnd = 4,
        .baseFrequency = 440.0
    };
    
    osc_t *osc = osc_resampler_create(&cfg);
    double buffer[6];

    osc->generate(osc, 440.0, 44100.0, buffer, 6);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer[0]); // Index 0
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 30000.0 / NORMALIZATION_FACTOR_16B, buffer[3]); // Index 3
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 10000.0 / NORMALIZATION_FACTOR_16B, buffer[4]); // Wrap autour de loopStart
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 20000.0 / NORMALIZATION_FACTOR_16B, buffer[5]); // Suite de la boucle

    osc->destroy(osc);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_osc_resampler_invalid_config);
    RUN_TEST(test_osc_resampler_normal_speed);
    RUN_TEST(test_osc_resampler_interpolation_half_speed);
    RUN_TEST(test_osc_resampler_end_of_sample);
    RUN_TEST(test_osc_resampler_looping);

    return UNITY_END();
}