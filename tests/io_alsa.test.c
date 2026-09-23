/**
 * @file io_alsa.test.c
 * @brief Tests unitaires pour le backend audio ALSA
 */

#include "unity.h"
#include "sound/io/io_alsa.h"
#include "sound/engine/mixer.h"
#include <unistd.h>

static mixer_t *dummy_mixer = NULL;

void setUp(void) {
    mixer_config_t cfg = {
        .sampleRate = 48000.0,
        .masterVolume = 1.0,
        .numChannels = 1,
        .maxFramesPerBuffer = 512
    };
    dummy_mixer = mixer_create(&cfg);
}

void tearDown(void) {
    if (dummy_mixer) {
        mixer_destroy(dummy_mixer);
        dummy_mixer = NULL;
    }
}

/**
 * @brief Vérifie que la création échoue proprement si les paramètres sont invalides
 */
void test_io_alsa_create_invalid(void) {
    io_alsa_t *io = io_alsa_create(NULL, "default");
    TEST_ASSERT_NULL(io);
}

/**
 * @brief Teste le cycle de vie complet (Création, Thread Start, Thread Stop, Destruction)
 */
void test_io_alsa_lifecycle(void) {
    io_alsa_t *io = io_alsa_create(dummy_mixer, "default");
	
	if (!io) {
        TEST_IGNORE_MESSAGE("Test matériel ignoré.");
        return; 
    }

    TEST_ASSERT_NOT_NULL(io);
    TEST_ASSERT_FALSE(io->is_running);
    TEST_ASSERT_EQUAL_PTR(dummy_mixer, io->mixer);

    bool started = io_alsa_start(io);
    TEST_ASSERT_TRUE(started);
    TEST_ASSERT_TRUE(io->is_running);

    usleep(50000); 

    io_alsa_stop(io);
    TEST_ASSERT_FALSE(io->is_running);

    io_alsa_destroy(io);
}

/**
 * @brief Vérifie que la destruction fonctionne correctement
 */
void test_io_alsa_null_safety(void) {
    io_alsa_destroy(NULL);
    TEST_ASSERT_TRUE(true);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_io_alsa_create_invalid);
    RUN_TEST(test_io_alsa_lifecycle);
    RUN_TEST(test_io_alsa_null_safety);

    return UNITY_END();
}