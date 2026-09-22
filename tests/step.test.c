/**
 * @file step.test.c
 */

#include "unity.h"
#include "music/step.h"
#include <stdint.h>

void setUp(void) {}
void tearDown(void) {}

/**
 * @brief Teste la création d'un step de type NOTE
 */
void test_music_step_create_note(void) {
    uint8_t noteId = 12;
    int8_t octave = 4;
    uint32_t instrumentId = 99;
    music_time_duration_t duration = 240;

    music_step_t step = music_step_create_note(noteId, octave, instrumentId, duration);

    TEST_ASSERT_EQUAL_INT(MUSIC_STEP_TYPE_NOTE, step.type);
    TEST_ASSERT_EQUAL_INT(duration, step.duration);
    
    TEST_ASSERT_EQUAL_UINT8(noteId, step.data.note.noteId);
    TEST_ASSERT_EQUAL_INT8(octave, step.data.note.octave);
    TEST_ASSERT_EQUAL_UINT32(instrumentId, step.data.note.instrumentId);

    TEST_ASSERT_EQUAL_UINT8(100, step.data.note.volumePercent);
}

/**
 * @brief Teste la création d'un step de type REST
 */
void test_music_step_create_rest(void) {
    music_time_duration_t duration = 120;

    music_step_t step = music_step_create_rest(duration);

    TEST_ASSERT_EQUAL_INT(MUSIC_STEP_TYPE_REST, step.type);
    TEST_ASSERT_EQUAL_INT(duration, step.duration);
}

/**
 * @brief Teste la création d'une commande de changement de BPM
 */
void test_music_step_create_cmd_bpm(void) {
    uint16_t newBpm = 140;
    music_time_duration_t duration = 0;

    music_step_t step = music_step_create_cmd_bpm(newBpm, duration);

    TEST_ASSERT_EQUAL_INT(MUSIC_STEP_TYPE_COMMAND, step.type);
    TEST_ASSERT_EQUAL_INT(duration, step.duration);
    
    TEST_ASSERT_EQUAL_INT(MUSIC_CMD_SET_BPM, step.data.cmd.type);
    TEST_ASSERT_EQUAL_UINT16(newBpm, step.data.cmd.param.bpm.bpm);
}

/**
 * @brief Teste la création d'une commande de changement de volume
 */
void test_music_step_create_cmd_volume(void) {
    uint8_t newVolume = 75;
    music_time_duration_t duration = 10;

    music_step_t step = music_step_create_cmd_volume(newVolume, duration);

    TEST_ASSERT_EQUAL_INT(MUSIC_STEP_TYPE_COMMAND, step.type);
    TEST_ASSERT_EQUAL_INT(duration, step.duration);
    
    TEST_ASSERT_EQUAL_INT(MUSIC_CMD_SET_VOLUME, step.data.cmd.type);
    TEST_ASSERT_EQUAL_UINT8(newVolume, step.data.cmd.param.volume.volumePercent);
}

/**
 * @brief Teste la création d'une commande de début de boucle
 */
void test_music_step_create_cmd_loop_start(void) {
    uint8_t loopId = 2;
    music_time_duration_t duration = 0;

    music_step_t step = music_step_create_cmd_loop_start(loopId, duration);

    TEST_ASSERT_EQUAL_INT(MUSIC_STEP_TYPE_COMMAND, step.type);
    TEST_ASSERT_EQUAL_INT(duration, step.duration);
    
    TEST_ASSERT_EQUAL_INT(MUSIC_CMD_LOOP_START, step.data.cmd.type);
    TEST_ASSERT_EQUAL_UINT8(loopId, step.data.cmd.param.loopStart.id);
}

/**
 * @brief Teste la création d'une commande de fin de boucle
 */
void test_music_step_create_cmd_loop_end(void) {
    uint8_t loopId = 2;
    uint16_t repeatCount = 4;
    music_time_duration_t duration = 0;

    music_step_t step = music_step_create_cmd_loop_end(loopId, repeatCount, duration);

    TEST_ASSERT_EQUAL_INT(MUSIC_STEP_TYPE_COMMAND, step.type);
    TEST_ASSERT_EQUAL_INT(duration, step.duration);
    
    TEST_ASSERT_EQUAL_INT(MUSIC_CMD_LOOP_END, step.data.cmd.type);
    TEST_ASSERT_EQUAL_UINT8(loopId, step.data.cmd.param.loopEnd.id);
    TEST_ASSERT_EQUAL_UINT16(repeatCount, step.data.cmd.param.loopEnd.count);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_music_step_create_note);
    RUN_TEST(test_music_step_create_rest);
    RUN_TEST(test_music_step_create_cmd_bpm);
    RUN_TEST(test_music_step_create_cmd_volume);
    RUN_TEST(test_music_step_create_cmd_loop_start);
    RUN_TEST(test_music_step_create_cmd_loop_end);

    return UNITY_END();
}