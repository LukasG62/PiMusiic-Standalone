/**
 * @file app_logger.test.c
 * @brief Tests unitaires pour la couche applicative du logger
 */

#include "unity.h"
#include "core/app_logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_LOG_FILE "pimusiic.log"

logger_t appLogger = {0};

/**
 * @brief Lit le contenu entier d'un fichier texte dans un buffer statique
 */
static void read_file_content(const char *filepath, char *buffer, size_t max_len) {
    buffer[0] = '\0';
    FILE *f = fopen(filepath, "r");
    if (f) {
        size_t read_size = fread(buffer, 1, max_len - 1, f);
        buffer[read_size] = '\0';
        fclose(f);
    }
}

void setUp(void) {
    remove(TEST_LOG_FILE);
}

void tearDown(void) {
    remove(TEST_LOG_FILE);
}


/**
 * @brief Teste que la fonction de callback écrit correctement dans le fichier
 * et respecte le format "[DATE][LEVEL][MODULE] FUNC: MESSAGE"
 */
void test_file_log_callback_formatting(void) {
    app_log_data_t *data = malloc(sizeof(app_log_data_t));
    data->module = "TEST_MOD";
    data->func = "test_func";
    
    const char *test_msg = "Ceci est un test unitaire";
    data->message = malloc(strlen(test_msg) + 1);
    strcpy(data->message, test_msg);

    file_log_callback(LOG_LEVEL_WARNING, data);

	char file_content[512];
    read_file_content(TEST_LOG_FILE, file_content, sizeof(file_content));

	int year, month, day, hour, min, sec;
    int parsed = sscanf(file_content, "[%4d-%2d-%2d %2d:%2d:%2d]", &year, &month, &day, &hour, &min, &sec);
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(6, parsed, "Le format de la date est incorrect");
	TEST_ASSERT_TRUE_MESSAGE(year >= 2024, "L'année générée semble incohérente");
    TEST_ASSERT_TRUE(month >= 1 && month <= 12);
    TEST_ASSERT_NOT_NULL(strstr(file_content, "][WARNING][TEST_MOD] test_func: Ceci est un test unitaire\n"));
}

/**
 * @brief Teste que le callback ne plante pas si on lui passe NULL
 */
void test_file_log_callback_null_data(void) {
    file_log_callback(LOG_LEVEL_INFO, NULL); // Ne doit pas crasher
    
    FILE *f = fopen(TEST_LOG_FILE, "r");
    TEST_ASSERT_NULL(f);
}

/**
 * @brief Formate la string, envoie à la queue async, et le thread l'écrit dans le fichier.
 */
void test_app_log_send_integration(void) {
    logger_init(&appLogger, LOG_LEVEL_DEBUG, file_log_callback);

	app_log_send(LOG_LEVEL_ERROR, "NETWORK", "connect_server", "Échec de connexion au port %d (timeout: %f s)", 8080, 5.5);
    
	logger_destroy(&appLogger);

    char file_content[512];
    read_file_content(TEST_LOG_FILE, file_content, sizeof(file_content));

    TEST_ASSERT_NOT_NULL(strstr(file_content, "[ERROR]"));
    TEST_ASSERT_NOT_NULL(strstr(file_content, "[NETWORK]"));
    TEST_ASSERT_NOT_NULL(strstr(file_content, "connect_server: Échec de connexion au port 8080 (timeout: 5.500000 s)\n"));
}

/**
 * @brief Teste le filtrage de niveau : un log inférieur au currentLogLevel est ignoré.
 */
void test_app_log_send_level_filtering(void) {
    logger_init(&appLogger, LOG_LEVEL_ERROR, file_log_callback);

    app_log_send(LOG_LEVEL_INFO, "UI", "draw", "Redessine l'écran");

    logger_destroy(&appLogger);

    FILE *f = fopen(TEST_LOG_FILE, "r");
    TEST_ASSERT_NULL(f);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_file_log_callback_formatting);
    RUN_TEST(test_file_log_callback_null_data);
    RUN_TEST(test_app_log_send_integration);
    RUN_TEST(test_app_log_send_level_filtering);

    return UNITY_END();
}