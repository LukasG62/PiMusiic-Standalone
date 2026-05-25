/**
 * @file instrument.test.c
 * @brief Tests unitaires pour le modèle de données Instrument
 */

#include "unity.h"
#include "sound/engine/instrument.h"
#include <string.h>

#define DELTA 1e-6

void setUp(void) {}
void tearDown(void) {}

/**
 * @brief Teste la création d'un instrument par défaut et l'initialisation de ses valeurs
 */
void test_instrument_create_default(void) {
    instrument_t *inst = instrument_create_default();
    TEST_ASSERT_NOT_NULL(inst);

    TEST_ASSERT_EQUAL_STRING("Default Sine", inst->name);
    TEST_ASSERT_EQUAL_STRING("SINE", inst->shortName);
    TEST_ASSERT_EQUAL(OSC_TYPE_SIN, inst->oscType);

    TEST_ASSERT_TRUE(inst->adsrConfig.enabled);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.01, inst->adsrConfig.attackTime);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.1,  inst->adsrConfig.decayTime);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.7,  inst->adsrConfig.sustainLevel);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.2,  inst->adsrConfig.releaseTime);

    TEST_ASSERT_EQUAL(0, inst->fxCount);
    for (int i = 0; i < MAX_FX_PER_INSTRUMENT; i++) {
        TEST_ASSERT_EQUAL(FX_TYPE_NONE, inst->fxTypes[i]);
    }

    instrument_destroy(inst);
}

/**
 * @brief Teste que la fonction destroy
 */
void test_instrument_destroy_null_safe(void) {
    instrument_destroy(NULL);
    TEST_ASSERT_TRUE(true);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_instrument_create_default);
    RUN_TEST(test_instrument_destroy_null_safe);

    return UNITY_END();
}