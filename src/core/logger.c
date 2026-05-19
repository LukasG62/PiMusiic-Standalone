/**
 * @file logger.c
 * @brief Module de journalisation pour les différents services (sync et async).
 * @author Lukas
 * @date 2025-10-18
 */
#include "core/logger.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct log_node_t
 * @brief Nœud interne pour stocker un événement de log dans la FIFO
 */
typedef struct {
    void *data;
    log_level_t level;
} log_node_t;

/**
 * @private
 * @brief Fonction du thread de journalisation asynchrone.
 * @param arg L'instance du logger_t.
 * @return NULL
 */
static void *logger_thread_function(void *arg) {
    logger_t *logger = (logger_t *)arg;
    
    while(1) {
        sem_wait(&logger->logSem);
        if(!logger->loggerThreadRunning) break;
        
        log_node_t *node = fifo_pop(&logger->logFifo);
        if(node) {
            if(logger->logCallback) {
                logger->logCallback(node->level, node->data);
            }
            free(node);
        }
    }

    // Nettoyage des messages restants
    while(!fifo_is_empty(&logger->logFifo)) {
        log_node_t *node = fifo_pop(&logger->logFifo);
        if(node) {
            if(logger->logCallback) {
                logger->logCallback(node->level, node->data);
            }
            free(node);
        }
    }
    return NULL;
}

/**
 * @brief  Initialise le système de log (callback + thread async prêt)
 * @param logger L'instance du logger.
 * @param level Niveau de journalisation global.
 * @param callback Fonction de rappel pour traiter les messages.
 * @note Les messages sont stockés dans une queue et traités par un thread dédié.
 */
void logger_init(logger_t *logger, log_level_t level, log_callback_t callback) {
    if (!logger) return;

    logger->currentLogLevel = level;
    logger->logCallback = callback;
    logger->loggerInitialized = true;

    fifo_init(&logger->logFifo);
    CHECK_SYS_CALL(sem_init(&logger->logSem, 0, 0));

    logger->loggerThreadRunning = 1;
    CHECK_SYS_CALL(pthread_create(&logger->loggerThread, NULL, logger_thread_function, logger));
}

/**
 * @brief Journalise un message en mode synchrone.
 * @param logger L'instance du logger.
 * @param level Niveau de criticité du message.
 * @param data Les données génériques à transmettre au callback.
 * @note Si le niveau du message est inférieur au niveau configuré, il est ignoré.
 */
void logger_log_sync(logger_t *logger, log_level_t level, void *data) {
    if(!logger || !logger->loggerInitialized || level < logger->currentLogLevel) {
        return;
    }
    
    if(logger->logCallback) {
        logger->logCallback(level, data);
    }
}

/**
 * @brief Journalise un message en mode asynchrone.
 * @param logger L'instance du logger.
 * @param level Niveau de criticité du message.
 * @param data Les données génériques à transmettre au callback.
 * @note Cette fonction retourne immédiatement, le message sera traité par le thread de logging.
 */
void logger_log_async(logger_t *logger, log_level_t level, void *data) {
    if(!logger || level < logger->currentLogLevel) {
        return;
    }
    
    log_node_t *node = malloc(sizeof(log_node_t));
    CHECK_ALLOC(node);
    
    node->data = data;
    node->level = level;

    fifo_push(&logger->logFifo, node);
    sem_post(&logger->logSem);
}

/**
 * @brief Libère les ressources du système de journalisation.
 * @param logger L'instance du logger.
 * @note Cette fonction doit être appelée pour nettoyer le thread et les sémaphores.
 */
void logger_destroy(logger_t *logger) {
    if(!logger || !logger->loggerInitialized) {
        return;
    }

    logger->loggerThreadRunning = 0;
    sem_post(&logger->logSem);
    pthread_join(logger->loggerThread, NULL);

    CHECK_SYS_CALL(sem_destroy(&logger->logSem));
    fifo_destroy(&logger->logFifo);
    logger->loggerInitialized = false;
}

/**
 * @brief Convertit un niveau de log en chaîne de caractères.
 * @param level Niveau de log.
 * @return Chaîne représentant le niveau de log.
 */
const char* logger_level_to_string(log_level_t level) {
    switch (level) {
        case LOG_LEVEL_DEBUG:   return "DEBUG";
        case LOG_LEVEL_INFO:    return "INFO";
        case LOG_LEVEL_WARNING: return "WARNING";
        case LOG_LEVEL_ERROR:   return "ERROR";
        case LOG_LEVEL_FATAL:   return "FATAL";
        default:                return "UNKNOWN";
    }
}

/**
 * @brief Convertit une chaîne de caractères en niveau de log.
 * @param levelStr Chaîne représentant le niveau de log.
 * @return Niveau de log correspondant.
 */
log_level_t logger_string_to_level(const char* levelStr) {
    if (strcmp(levelStr, "DEBUG") == 0) {
        return LOG_LEVEL_DEBUG;
    } else if (strcmp(levelStr, "INFO") == 0) {
        return LOG_LEVEL_INFO;
    } else if (strcmp(levelStr, "WARNING") == 0) {
        return LOG_LEVEL_WARNING;
    } else if (strcmp(levelStr, "ERROR") == 0) {
        return LOG_LEVEL_ERROR;
    } else if (strcmp(levelStr, "FATAL") == 0) {
        return LOG_LEVEL_FATAL;
    } else {
        return LOG_LEVEL_INFO;
    }
}