#ifndef APP_LOGGER_H
#define APP_LOGGER_H

#include "core/logger.h"
#include "core.h"

// Définition de tes modules spécifiques
#define LOG_MOD_LOGIC  "LOGIC"
#define LOG_MOD_UI     "UI"
#define LOG_MOD_INET   "INET"

// Structure qui sera passée en tant que "void *data" au logger générique
typedef struct {
    const char *module;
    const char *func;
    char *message;
} app_log_data_t;

/**
 * @brief Callback de log qui écrit dans un fichier (sécurisé pour Ncurses)
 * @param level Niveau de log.
 * @param data Pointeur vers app_log_data_t.
 */
void file_log_callback(log_level_t level, void *data);

/**
 * @brief Fonction pour envoyer un message de log de manière asynchrone
 * @param level Niveau de log.
 * @param module Module emetteur.
 * @param func Fonction emettrice.
 * @param format Format du message.
 * @param ... arguments du message.
 */
void app_log_send(log_level_t level, const char *module, const char *func, const char *format, ...);

// Tes macros avec le contexte global (appContext->logger) injecté sous le capot
#define LOG_DEBUG_ASYNC(module, format, ...)  app_log_send(LOG_LEVEL_DEBUG, module, __func__, format, ##__VA_ARGS__)
#define LOG_INFO_ASYNC(module, format, ...)   app_log_send(LOG_LEVEL_INFO, module, __func__, format, ##__VA_ARGS__)
#define LOG_WARNING_ASYNC(module, format, ...) app_log_send(LOG_LEVEL_WARNING, module, __func__, format, ##__VA_ARGS__)
#define LOG_ERROR_ASYNC(module, format, ...)  app_log_send(LOG_LEVEL_ERROR, module, __func__, format, ##__VA_ARGS__)
#define LOG_FATAL_ASYNC(module, format, ...)  app_log_send(LOG_LEVEL_FATAL, module, __func__, format, ##__VA_ARGS__)

#endif // APP_LOGGER_H