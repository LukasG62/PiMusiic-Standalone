/**
 * @file fx_compressor.test.c
 * @brief Tests unitaires pour l'effet compresseur
 */

#include "unity.h"
#include "sound/fx/fx_compressor.h"
#include <math.h>

#define DELTA 1e-5

void setUp(void) {}
void tearDown(void) {}

/**
 * @brief Teste les cas d'erreur à la création
 */
void test_fx_compressor_create_invalid(void) {
    fx_compressor_config_t valid_cfg = { .sampleRate = 44100.0 };
    fx_compressor_config_t invalid_cfg = { .sampleRate = -10.0 };

    fx_t *fx1 = fx_compressor_create(NULL);
    TEST_ASSERT_NULL(fx1);

    fx_t *fx2 = fx_compressor_create(&invalid_cfg);
    TEST_ASSERT_NULL(fx2);

    fx_t *fx3 = fx_compressor_create(&valid_cfg);
    TEST_ASSERT_NOT_NULL(fx3);
    
    fx3->destroy(fx3);
}

/**
 * @brief Teste que le signal sous le seuil n'est pas compressé
 */
void test_fx_compressor_below_threshold(void) {
    fx_compressor_config_t cfg = {
        .sampleRate = 44100.0,
        .attackTime = 0.0,
        .releaseTime = 0.0,
        .threshold = 0.8,
        .ratio = 2.0,
        .makeupGain = 1.0
    };
    
    fx_t *fx = fx_compressor_create(&cfg);
    
    double buffer[2] = { 0.5, -0.5 };
    fx->process(fx, buffer, 2);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.5, buffer[0]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, -0.5, buffer[1]);

    fx->destroy(fx);
}

/**
 * @brief Teste la réduction de gain avec des temps à zéro
 */
void test_fx_compressor_instant_compression(void) {
    fx_compressor_config_t cfg = {
        .sampleRate = 44100.0,
        .attackTime = 0.0,
        .releaseTime = 0.0,
        .threshold = 0.5,
        .ratio = 2.0,
        .makeupGain = 1.0
    };
    
    fx_t *fx = fx_compressor_create(&cfg);
    
    double buffer[1] = { 1.0 }; 
    fx->process(fx, buffer, 1);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.75, buffer[0]);

    fx->destroy(fx);
}

/**
 * @brief Teste l'action du temps d'attaque
 */
void test_fx_compressor_attack_envelope(void) {
    fx_compressor_config_t cfg = {
        .sampleRate = 100.0,
        .attackTime = 1.0,
        .releaseTime = 0.0,
        .threshold = 0.5,
        .ratio = 10.0,
        .makeupGain = 1.0
    };
    
    fx_t *fx = fx_compressor_create(&cfg);
    double buffer[1] = { 1.0 };
    
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 1.0, buffer[0]); 

    double longBuffer[500];
    for (int i = 0; i < 500; i++) longBuffer[i] = 1.0;
    
    fx->process(fx, longBuffer, 500);

    TEST_ASSERT_LESS_THAN_DOUBLE(0.6, longBuffer[499]);

    fx->destroy(fx);
}

/**
 * @brief Teste l'application du makeup gain
 */
void test_fx_compressor_makeup_gain(void) {
    fx_compressor_config_t cfg = {
        .sampleRate = 44100.0,
        .attackTime = 0.0,
        .releaseTime = 0.0,
        .threshold = 1.0,
        .ratio = 1.0,
        .makeupGain = 3.0
    };
    
    fx_t *fx = fx_compressor_create(&cfg);
    
    double buffer[2] = { 0.2, -0.3 }; 
    fx->process(fx, buffer, 2);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.6, buffer[0]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, -0.9, buffer[1]);

    fx->destroy(fx);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_fx_compressor_create_invalid);
    RUN_TEST(test_fx_compressor_below_threshold);
    RUN_TEST(test_fx_compressor_instant_compression);
    RUN_TEST(test_fx_compressor_attack_envelope);
    RUN_TEST(test_fx_compressor_makeup_gain);

    return UNITY_END();
}