/**
 * @file voice.test.c
 * @brief Tests unitaires pour la gestion des voix
 */

#include "unity.h"
#include "sound/engine/voice.h"
#include "sound/engine/instrument.h"
#include <math.h>

#define DELTA 1e-6

void setUp(void) {}
void tearDown(void) {}

/**
 * @brief Teste la création et la destruction d'une voix à partir d'un instrument
 */
void test_voice_create_and_destroy(void) {
    instrument_t *inst = instrument_create_default();
    TEST_ASSERT_NOT_NULL(inst);

    voice_t *voice = voice_create(inst);

    TEST_ASSERT_NOT_NULL(voice);
    TEST_ASSERT_EQUAL_PTR(inst, voice->instrument);
    TEST_ASSERT_NOT_NULL(voice->oscillator);
    TEST_ASSERT_NOT_NULL(voice->envelope);
    
    for(int i = 0; i < MAX_FX_PER_INSTRUMENT; i++) {
        TEST_ASSERT_NULL(voice->effects[i]);
    }

    TEST_ASSERT_FALSE(voice->isActive);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, voice->currentFreq);

    voice_destroy(voice);
    instrument_destroy(inst);
}

/**
 * @brief Teste le cycle de vie des états
 */
void test_voice_lifecycle(void) {
    instrument_t *inst = instrument_create_default();
    voice_t *voice = voice_create(inst);

    TEST_ASSERT_FALSE(voice->isActive);
    TEST_ASSERT_TRUE(voice_is_done(voice));

    voice_note_on(voice, 440.0);
    TEST_ASSERT_TRUE(voice->isActive);
    TEST_ASSERT_EQUAL_DOUBLE(440.0, voice->currentFreq);
    TEST_ASSERT_EQUAL(ADSR_PHASE_ATTACK, voice->envelope->currentPhase);
    TEST_ASSERT_FALSE(voice_is_done(voice));

    voice_note_off(voice);
    TEST_ASSERT_EQUAL(ADSR_PHASE_RELEASE, voice->envelope->currentPhase);
    TEST_ASSERT_FALSE(voice_is_done(voice));

    voice->envelope->currentPhase = ADSR_PHASE_OFF;
    
    TEST_ASSERT_TRUE(voice_is_done(voice));
    TEST_ASSERT_FALSE(voice->isActive);

    voice_destroy(voice);
    instrument_destroy(inst);
}

/**
 * @brief Teste que la fonction process sort des zéros absolus si la voix est inactive
 */
void test_voice_process_inactive(void) {
    instrument_t *inst = instrument_create_default();
    voice_t *voice = voice_create(inst);
    
    double buffer[4] = { 1.0, 1.0, 1.0, 1.0 };

    voice_process(voice, 44100.0, buffer, 4);

    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer[0]);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, buffer[3]);

    voice_destroy(voice);
    instrument_destroy(inst);
}

/**
 * @brief Teste que la fonction process génère bien du son une fois activée
 */
void test_voice_process_active(void) {
    instrument_t *inst = instrument_create_default();
    voice_t *voice = voice_create(inst);
    
    double buffer[4] = { 0.0, 0.0, 0.0, 0.0 };

    voice_note_on(voice, 440.0);
    voice_process(voice, 44100.0, buffer, 4);

    TEST_ASSERT_GREATER_THAN_DOUBLE(0.0, buffer[1]);

    voice_destroy(voice);
    instrument_destroy(inst);
}

/**
 * @brief Teste la protection contre les pointeurs nuls
 */
void test_voice_null_safety(void) {
    voice_t *voice = voice_create(NULL);
    TEST_ASSERT_NULL(voice);

    voice_note_on(NULL, 440.0);
    voice_note_off(NULL);
    
    double buffer[2] = {0.0};
    voice_process(NULL, 44100.0, buffer, 2);
    
    TEST_ASSERT_TRUE(voice_is_done(NULL));

    voice_destroy(NULL);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_voice_create_and_destroy);
    RUN_TEST(test_voice_lifecycle);
    RUN_TEST(test_voice_process_inactive);
    RUN_TEST(test_voice_process_active);
    RUN_TEST(test_voice_null_safety);

    return UNITY_END();
}