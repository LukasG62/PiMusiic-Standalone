/**
 * @file logger.test.c
 * @brief Tests unitaires pour le module de journalisation (logger)
 */

#include "unity.h"
#include "core/logger.h"
#include <string.h>
#include <unistd.h>
#include <stdint.h>

static volatile int callback_call_count = 0;
static volatile log_level_t last_received_level = LOG_LEVEL_DEBUG;
static void * volatile last_received_data = NULL;

void test_log_callback(log_level_t level, void *data) {
    callback_call_count++;
    last_received_level = level;
    last_received_data = data;
}

void setUp(void) {
    callback_call_count = 0;
    last_received_level = LOG_LEVEL_DEBUG;
    last_received_data = NULL;
}

void tearDown(void) {
}

/**
 * @brief Teste la conversion de niveau de log en chaîne de caractères
 */
void test_logger_level_to_string(void) {
    TEST_ASSERT_EQUAL_STRING("DEBUG", logger_level_to_string(LOG_LEVEL_DEBUG));
    TEST_ASSERT_EQUAL_STRING("INFO", logger_level_to_string(LOG_LEVEL_INFO));
    TEST_ASSERT_EQUAL_STRING("WARNING", logger_level_to_string(LOG_LEVEL_WARNING));
    TEST_ASSERT_EQUAL_STRING("ERROR", logger_level_to_string(LOG_LEVEL_ERROR));
    TEST_ASSERT_EQUAL_STRING("FATAL", logger_level_to_string(LOG_LEVEL_FATAL));
    
    TEST_ASSERT_EQUAL_STRING("UNKNOWN", logger_level_to_string((log_level_t)99));
}

/**
 * @details Teste la conversion de chaîne de caractères en niveau de log
 */
void test_logger_string_to_level(void) {
    TEST_ASSERT_EQUAL(LOG_LEVEL_DEBUG, logger_string_to_level("DEBUG"));
    TEST_ASSERT_EQUAL(LOG_LEVEL_INFO, logger_string_to_level("INFO"));
    TEST_ASSERT_EQUAL(LOG_LEVEL_WARNING, logger_string_to_level("WARNING"));
    TEST_ASSERT_EQUAL(LOG_LEVEL_ERROR, logger_string_to_level("ERROR"));
    TEST_ASSERT_EQUAL(LOG_LEVEL_FATAL, logger_string_to_level("FATAL"));
    
    TEST_ASSERT_EQUAL(LOG_LEVEL_INFO, logger_string_to_level("NIMPORTE_QUOI"));
}

/**
 * @brief Teste la journalisation synchrone avec un message qui doit être traité
 */
void test_logger_sync_pass(void) {
    logger_t logger;
	logger_init(&logger, LOG_LEVEL_WARNING, test_log_callback);

    char *msg = "Message synchrone d'erreur";
    
	logger_log_sync(&logger, LOG_LEVEL_ERROR, msg);

    TEST_ASSERT_EQUAL_INT(1, callback_call_count);
    TEST_ASSERT_EQUAL(LOG_LEVEL_ERROR, last_received_level);
    TEST_ASSERT_EQUAL_PTR(msg, last_received_data);

    logger_destroy(&logger);
}

/**
 * @brief Teste la journalisation synchrone avec un message qui doit être ignoré
 */
void test_logger_sync_ignored(void) {
    logger_t logger;

	logger_init(&logger, LOG_LEVEL_ERROR, test_log_callback);
    logger_log_sync(&logger, LOG_LEVEL_INFO, "Ce message doit être ignoré");

    TEST_ASSERT_EQUAL_INT(0, callback_call_count);

    logger_destroy(&logger);
}

/**
 * @brief Teste la journalisation asynchrone avec un message qui doit être traité
 */
void test_logger_async_pass(void) {
    logger_t logger;
    logger_init(&logger, LOG_LEVEL_INFO, test_log_callback);

    char *msg = "Message asynchrone";
	
	logger_log_async(&logger, LOG_LEVEL_FATAL, msg);
    usleep(10000); 

    TEST_ASSERT_EQUAL_INT(1, callback_call_count);
    TEST_ASSERT_EQUAL(LOG_LEVEL_FATAL, last_received_level);
    TEST_ASSERT_EQUAL_PTR(msg, last_received_data);

    logger_destroy(&logger);
}

void test_logger_async_ignored(void) {
    logger_t logger;
    logger_init(&logger, LOG_LEVEL_WARNING, test_log_callback);

    logger_log_async(&logger, LOG_LEVEL_DEBUG, "Message asynchrone ignoré");

    usleep(10000);

    TEST_ASSERT_EQUAL_INT(0, callback_call_count);

    logger_destroy(&logger);
}

void test_logger_async_cleanup_on_destroy(void) {
    logger_t logger;
    logger_init(&logger, LOG_LEVEL_DEBUG, test_log_callback);

    for (intptr_t i = 0; i < 10; i++) {
        logger_log_async(&logger, LOG_LEVEL_DEBUG, (void *)i);
    }
    logger_destroy(&logger);

	TEST_ASSERT_EQUAL_INT(10, callback_call_count);
    TEST_ASSERT_EQUAL_PTR((void*)9, last_received_data); 
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_logger_level_to_string);
    RUN_TEST(test_logger_string_to_level);

    RUN_TEST(test_logger_sync_pass);
    RUN_TEST(test_logger_sync_ignored);

    RUN_TEST(test_logger_async_pass);
    RUN_TEST(test_logger_async_ignored);
    RUN_TEST(test_logger_async_cleanup_on_destroy);

    return UNITY_END();
}