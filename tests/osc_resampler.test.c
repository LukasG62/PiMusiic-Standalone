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

/**
 * @brief Teste la conversion d'un fichier PMSD valide vers une config d'oscillateur
 */
void test_osc_resampler_config_from_pmsd_valid(void) {
    io_pmsd_t pmsd;
    memset(&pmsd, 0, sizeof(io_pmsd_t));
    
    pmsd.header.methodType = PMSD_METHOD_RESAMPLER;
    pmsd.header.sampleCount = 256;
    pmsd.meta.resampler.baseFreq = 440.0f;
    pmsd.meta.resampler.loopEnabled = 1;
    pmsd.meta.resampler.loopStart = 10;
    pmsd.meta.resampler.loopEnd = 200;
    pmsd.audioData = dummy_sample_data; // Pointeur factice

    osc_resampler_config_t cfg;
    memset(&cfg, 0, sizeof(osc_resampler_config_t));

    osc_resampler_config_from_pmsd(&cfg, &pmsd);

    TEST_ASSERT_EQUAL_PTR(dummy_sample_data, cfg.sampleData);
    TEST_ASSERT_EQUAL_size_t(256, cfg.sampleLength);
    TEST_ASSERT_EQUAL_DOUBLE(440.0, cfg.baseFrequency);
    TEST_ASSERT_TRUE(cfg.loopEnabled);
    TEST_ASSERT_EQUAL_size_t(10, cfg.loopStart);
    TEST_ASSERT_EQUAL_size_t(200, cfg.loopEnd);
}

/**
 * @brief Vérifie le rejet sécurisé des pointeurs NULL et des mauvais types de PMSD
 */
void test_osc_resampler_config_from_pmsd_invalid(void) {
    osc_resampler_config_t cfg;
    memset(&cfg, 0, sizeof(osc_resampler_config_t));

    osc_resampler_config_from_pmsd(NULL, NULL);
    osc_resampler_config_from_pmsd(&cfg, NULL);

    io_pmsd_t pmsd_vocoder;
    memset(&pmsd_vocoder, 0, sizeof(io_pmsd_t));
    pmsd_vocoder.header.methodType = PMSD_METHOD_PHASE_VOCODER;
    pmsd_vocoder.audioData = dummy_sample_data;

    osc_resampler_config_from_pmsd(&cfg, &pmsd_vocoder);

    TEST_ASSERT_NULL(cfg.sampleData);
    TEST_ASSERT_EQUAL_size_t(0, cfg.sampleLength);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, cfg.baseFrequency);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_osc_resampler_invalid_config);
    RUN_TEST(test_osc_resampler_normal_speed);
    RUN_TEST(test_osc_resampler_interpolation_half_speed);
    RUN_TEST(test_osc_resampler_end_of_sample);
    RUN_TEST(test_osc_resampler_looping);
    RUN_TEST(test_osc_resampler_config_from_pmsd_valid);
    RUN_TEST(test_osc_resampler_config_from_pmsd_invalid);

    return UNITY_END();
}