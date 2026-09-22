/**
 * @file music.test.c
 * @brief Tests unitaires pour l'orchestrateur principal de musique (music.c)
 */

#include "unity.h"
#include "music/music.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/**
 * @brief Teste l'initialisation nominale avec des paramètres valides
 */
void test_music_init_valid(void) {
    music_t music;
    music_init(&music, 140, "My Awesome Track");

    TEST_ASSERT_EQUAL_STRING("My Awesome Track", music.name);
    TEST_ASSERT_EQUAL_UINT16(140, music.baseBpm);
    
    /* Vérifie que la date et l'ID ont bien été générés (non nuls) */
    TEST_ASSERT_NOT_EQUAL(0, music.id);
    TEST_ASSERT_NOT_EQUAL(0, music.date.tv_sec);

    /* Vérifie que la boucle d'initialisation a bien touché tous les channels */
    TEST_ASSERT_EQUAL_UINT8(0, music.channels[0].id);
    TEST_ASSERT_EQUAL_UINT8(MUSIC_MAX_CHANNELS - 1, music.channels[MUSIC_MAX_CHANNELS - 1].id);
}

/**
 * @brief Teste les valeurs de repli (fallbacks) lors de l'initialisation
 */
void test_music_init_fallbacks(void) {
    music_t music;
    
    /* Initialisation avec BPM à 0 et nom NULL */
    music_init(&music, 0, NULL);

    /* Le BPM doit basculer sur la valeur par défaut (120) */
    TEST_ASSERT_EQUAL_UINT16(120, music.baseBpm);
    
    /* Le nom doit être "Untitled" */
    TEST_ASSERT_EQUAL_STRING("Untitled", music.name);
}

/**
 * @brief Teste la protection contre les pointeurs NULL à l'initialisation
 */
void test_music_init_null_pointer(void) {
    /* Ne doit pas causer de segmentation fault */
    music_init(NULL, 120, "Test");
}

/**
 * @brief Teste l'écriture et la lecture d'un step via les fonctions raccourcis
 */
void test_music_write_and_get_step_valid(void) {
    music_t music;
    music_init(&music, 120, "Test");

    /* Création d'un step factice (une pause de durée 50) */
    music_step_t step;
    step.type = MUSIC_STEP_TYPE_REST;
    step.duration = 50;

    /* Écriture sur le channel 2, à la ligne 10 */
    bool write_res = music_write_step(&music, 2, 10, step);
    TEST_ASSERT_TRUE(write_res);

    /* Récupération et vérification que c'est bien le même step au bon endroit */
    music_step_t *retrieved_step = music_get_step(&music, 2, 10);
    
    TEST_ASSERT_NOT_NULL(retrieved_step);
    TEST_ASSERT_EQUAL_INT(MUSIC_STEP_TYPE_REST, retrieved_step->type);
    TEST_ASSERT_EQUAL_INT(50, retrieved_step->duration);
}

/**
 * @brief Teste les limites de l'aiguillage (Out of Bounds) pour write et get
 */
void test_music_routing_out_of_bounds(void) {
    music_t music;
    music_init(&music, 120, "Test");
    
    music_step_t step = {0}; // Step vide

    /* Tentative d'écriture sur un channel qui n'existe pas */
    bool write_res = music_write_step(&music, MUSIC_MAX_CHANNELS, 0, step);
    TEST_ASSERT_FALSE(write_res);

    /* Tentative de lecture sur un channel qui n'existe pas */
    music_step_t *get_res = music_get_step(&music, MUSIC_MAX_CHANNELS, 0);
    TEST_ASSERT_NULL(get_res);
}

/**
 * @brief Teste la sécurité des pointeurs NULL sur write et get
 */
void test_music_routing_null_pointers(void) {
    music_step_t step = {0};

    bool write_res = music_write_step(NULL, 0, 0, step);
    TEST_ASSERT_FALSE(write_res);

    music_step_t *get_res = music_get_step(NULL, 0, 0);
    TEST_ASSERT_NULL(get_res);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_music_init_valid);
    RUN_TEST(test_music_init_fallbacks);
    RUN_TEST(test_music_init_null_pointer);
    RUN_TEST(test_music_write_and_get_step_valid);
    RUN_TEST(test_music_routing_out_of_bounds);
    RUN_TEST(test_music_routing_null_pointers);

    return UNITY_END();
}