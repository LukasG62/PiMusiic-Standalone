/**
 * @file adsr.test.c
 * @brief Tests unitaires pour l'enveloppe ADSR
 */

#include "unity.h"
#include "sound/env/adsr.h"

#define DELTA 1e-6

void setUp(void) {}
void tearDown(void) {}

/**
 * @brief Teste la création et la destruction de l'ADSR
 */
void test_adsr_create_and_destroy(void) {
    adsr_config_t config = { .enabled = true };
    adsr_t *adsr = adsr_create(&config);
    
    TEST_ASSERT_NOT_NULL(adsr);
    TEST_ASSERT_EQUAL_PTR(&config, adsr->config);
    TEST_ASSERT_EQUAL(ADSR_PHASE_OFF, adsr->currentPhase);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, adsr->currentVolume);

    adsr_destroy(adsr);
}

/**
 * @brief Teste le comportement des appels note_on et note_off quand activé
 */
void test_adsr_note_on_off_transitions(void) {
    adsr_config_t config = { .enabled = true };
    adsr_t *adsr = adsr_create(&config);

    adsr_note_on(adsr);
    TEST_ASSERT_EQUAL(ADSR_PHASE_ATTACK, adsr->currentPhase);

    adsr_note_off(adsr);
    TEST_ASSERT_EQUAL(ADSR_PHASE_RELEASE, adsr->currentPhase);

    adsr_destroy(adsr);
}

/**
 * @brief Teste la phase d'attaque (Attack)
 */
void test_adsr_attack_phase(void) {
    adsr_config_t config = { 
        .enabled = true, 
        .attackTime = 1.0 
    };
    adsr_t *adsr = adsr_create(&config);
    adsr_note_on(adsr);

    double buffer[4] = {1.0, 1.0, 1.0, 1.0}; 
    
    adsr_process(adsr, 4.0, buffer, 4);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.25, buffer[0]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.50, buffer[1]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.75, buffer[2]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 1.00, buffer[3]);
    TEST_ASSERT_EQUAL(ADSR_PHASE_DECAY, adsr->currentPhase);

    adsr_destroy(adsr);
}

/**
 * @brief Teste la phase de déclin (Decay) et de maintien (Sustain)
 */
void test_adsr_decay_and_sustain_phase(void) {
    adsr_config_t config = { 
        .enabled = true,
        .attackTime = 0.0,
        .decayTime = 2.0,
        .sustainLevel = 0.5
    };
    adsr_t *adsr = adsr_create(&config);
    adsr->currentPhase = ADSR_PHASE_DECAY;
    adsr->currentVolume = 1.0;

    double buffer[4] = {1.0, 1.0, 1.0, 1.0};    
    adsr_process(adsr, 2.0, buffer, 4);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.875, buffer[0]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.750, buffer[1]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.625, buffer[2]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.500, buffer[3]);
    TEST_ASSERT_EQUAL(ADSR_PHASE_SUSTAIN, adsr->currentPhase);

    double buffer_sus[1] = {1.0};
    adsr_process(adsr, 2.0, buffer_sus, 1);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.500, buffer_sus[0]);

    adsr_destroy(adsr);
}

/**
 * @brief Teste la phase de relâchement (Release)
 */
void test_adsr_release_phase(void) {
    adsr_config_t config = { 
        .enabled = true,
        .releaseTime = 1.0
    };
    adsr_t *adsr = adsr_create(&config);
    adsr->currentPhase = ADSR_PHASE_RELEASE;
    adsr->currentVolume = 0.5;

    double buffer[3] = {1.0, 1.0, 1.0};
    
    adsr_process(adsr, 4.0, buffer, 3);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.25, buffer[0]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.00, buffer[1]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.00, buffer[2]);

    TEST_ASSERT_EQUAL(ADSR_PHASE_OFF, adsr->currentPhase);

    adsr_destroy(adsr);
}

/**
 * @brief Teste le comportement quand les temps sont à 0.0
 */
void test_adsr_zero_times(void) {
    adsr_config_t config = { 
        .enabled = true, 
        .attackTime = 0.0,
        .decayTime = 0.0,
        .sustainLevel = 0.7,
        .releaseTime = 0.0
    };
    adsr_t *adsr = adsr_create(&config);
    double buffer[3] = {1.0, 1.0, 1.0};

    adsr_note_on(adsr);
    adsr_process(adsr, 44100.0, buffer, 3);
    
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 1.0, buffer[0]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.7, buffer[1]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.7, buffer[2]);
    TEST_ASSERT_EQUAL(ADSR_PHASE_SUSTAIN, adsr->currentPhase);

    double buffer_off[2] = {1.0, 1.0};
    adsr_note_off(adsr);
    adsr_process(adsr, 44100.0, buffer_off, 2);
    
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer_off[0]);
    TEST_ASSERT_EQUAL(ADSR_PHASE_OFF, adsr->currentPhase);

    adsr_destroy(adsr);
}

/**
 * @brief Teste le comportement quand l'ADSR est désactivé (Mode Gate / Bypass)
 */
void test_adsr_disabled_behavior(void) {
    adsr_config_t config = { .enabled = false };
    adsr_t *adsr = adsr_create(&config);
    
    adsr_note_on(adsr);
    double buffer_on[2] = {0.5, -0.5};
    adsr_process(adsr, 44100.0, buffer_on, 2);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.5 * ADSR_DISABLE_VOLUME, buffer_on[0]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, -0.5 * ADSR_DISABLE_VOLUME, buffer_on[1]);
    
    adsr_note_off(adsr);
    double buffer_off[2] = {0.5, -0.5};
    adsr_process(adsr, 44100.0, buffer_off, 2);
    
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer_off[0]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer_off[1]);
    
    adsr_destroy(adsr);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_adsr_create_and_destroy);
    RUN_TEST(test_adsr_note_on_off_transitions);
    RUN_TEST(test_adsr_attack_phase);
    RUN_TEST(test_adsr_decay_and_sustain_phase);
    RUN_TEST(test_adsr_release_phase);
    RUN_TEST(test_adsr_zero_times);
    RUN_TEST(test_adsr_disabled_behavior); 

    return UNITY_END();
}