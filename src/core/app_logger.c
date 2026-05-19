#include "core/app_logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#define LOG_FILE_PATH "pimusiic.log"

void file_log_callback(log_level_t level, void *data) {
    if (!data) return;

    app_log_data_t *log_data = (app_log_data_t *)data;

    char dateStr[25];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", t);

    FILE *logFile = fopen(LOG_FILE_PATH, "a");
    if (logFile) {
        fprintf(logFile, "[%s][%s][%s] %s: %s\n", 
                dateStr, 
                logger_level_to_string(level), 
                log_data->module, 
                log_data->func, 
                log_data->message);
        fclose(logFile);
    }

    free(log_data->message);
    free(log_data);
}

void app_log_send(log_level_t level, const char *module, const char *func, const char *format, ...) {
    logger_t *logger = &appLogger;

    if (!logger->loggerInitialized || level < logger->currentLogLevel) {
        return;
    }

    app_log_data_t *data = malloc(sizeof(app_log_data_t));
    CHECK_ALLOC(data);

    data->module = module;
    data->func = func;

    va_list args, args_copy;
    va_start(args, format);
    va_copy(args_copy, args);

    int size_needed = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);
    if (size_needed < 0) {
        free(data);
        va_end(args);
        return;
    }
    data->message = malloc(size_needed + 1);
    CHECK_ALLOC(data->message);

    vsnprintf(data->message, size_needed + 1, format, args);
    va_end(args);

    logger_log_async(logger, level, (void *)data);
}