/**
 * @file note.test.c
 * @brief Tests unitaires pour la gestion et le calcul des notes
 */

#include "unity.h"
#include "music/note.h"
#include <string.h>

#define DELTA 1e-6

void setUp(void) {}
void tearDown(void) {}

/**
 * @brief Teste que l'initialisation de la gamme renvoie bien des données
 */
void test_note_init_scale(void) {
    note_scale_t scale = note_init_scale();
    
	TEST_ASSERT_GREATER_THAN(0.0, scale.freqScale[NOTE_C_ID]);
    TEST_ASSERT_EQUAL_DOUBLE(NOTE_NA_FQ, scale.freqScale[NOTE_NA_ID]);
}

/**
 * @brief Teste le calcul des fréquences en fonction des changements d'octave
 */
void test_note_get_frequency_octaves(void) {
    double freq_ref = note_get_frequency(NOTE_A_ID, NOTE_OCTAVE_REF);
    
    TEST_ASSERT_GREATER_THAN(0.0, freq_ref);

    double freq_up = note_get_frequency(NOTE_A_ID, NOTE_OCTAVE_REF + 1);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, freq_ref * 2.0, freq_up);

    double freq_down = note_get_frequency(NOTE_A_ID, NOTE_OCTAVE_REF - 1);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, freq_ref / 2.0, freq_down);
    
    double freq_down_3 = note_get_frequency(NOTE_A_ID, NOTE_OCTAVE_REF - 3);
    TEST_ASSERT_DOUBLE_WITHIN(DELTA, freq_ref / 8.0, freq_down_3);
}

/**
 * @brief Teste la sécurité du calcul de fréquence avec des identifiants invalides
 */
void test_note_get_frequency_invalid(void) {
    double freq_na = note_get_frequency(NOTE_NA_ID, NOTE_OCTAVE_REF);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, freq_na);

    double freq_oob = note_get_frequency(NOTE_COUNT, NOTE_OCTAVE_REF);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, freq_oob);
    
    double freq_oob2 = note_get_frequency(NOTE_COUNT + 5, NOTE_OCTAVE_REF);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, freq_oob2);
}

/**
 * @brief Teste la conversion d'une note valide en chaîne de caractères
 */
void test_note_to_string_valid(void) {
    char str[4];
    
    note_to_string(NOTE_C_ID, 4, str);

    TEST_ASSERT_EQUAL_CHAR('4', str[strlen(str) - 1]);
}

/**
 * @brief Teste la conversion en chaîne de caractères avec des entrées invalides
 */
void test_note_to_string_invalid(void) {
    char str[4];
    
    note_to_string(NOTE_NA_ID, 4, str);
    TEST_ASSERT_EQUAL_STRING(NOTE_NA_NAME, str);
    
    note_to_string(NOTE_COUNT, 4, str);
    TEST_ASSERT_EQUAL_STRING(NOTE_NA_NAME, str);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_note_init_scale);
    RUN_TEST(test_note_get_frequency_octaves);
    RUN_TEST(test_note_get_frequency_invalid);
    RUN_TEST(test_note_to_string_valid);
    RUN_TEST(test_note_to_string_invalid);

    return UNITY_END();
}