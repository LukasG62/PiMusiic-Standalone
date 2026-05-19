/**
 * @file logger.h
 * @brief Module de journalisation générique pour l'application
 * @author Lukas
 * @date 2025-10-18
 */

#ifndef LOGGER_H
#define LOGGER_H

#include "common.h"
#include "core/fifo.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

/**
 * @enum log_level_t
 * @brief Niveaux de journalisation
 * Définit la criticité des messages de journalisation.
 */
typedef enum {
    LOG_LEVEL_DEBUG, /**< Messages de debugging */
    LOG_LEVEL_INFO, /**< Messages d'information */
    LOG_LEVEL_WARNING, /**< Messages d'avertissement */
    LOG_LEVEL_ERROR, /**< Messages d'erreur */
    LOG_LEVEL_FATAL, /**< Messages d'erreur fatale */
} log_level_t;

/**
 * @typedef log_callback_t
 * @brief Fonction de rappel pour la journalisation
 * Permet de définir un traitement personnalisé pour les messages de journalisation.
 * @param level Le niveau de journalisation du message.
 * @param data Pointeur générique contenant les données du log, à traiter et libérer par le callback.
 */
typedef void (*log_callback_t)(log_level_t level, void *data);

/**
 * @struct logger_t
 * @brief Structure d'état du logger
 * Encapsule tout l'état du logger pour éviter l'utilisation de variables globales.
 */
typedef struct {
    log_level_t currentLogLevel; /*!< Niveau de journalisation actuel. Les messages avec un niveau inférieur seront ignorés. */
    log_callback_t logCallback; /*!< Fonction de rappel pour traiter les messages de journalisation. */
    bool loggerInitialized; /*!< Indique si le logger a été initialisé. */
    
    sem_t logSem; /*!< mutex pour rendre le logger thread-safe */
    fifo_t logFifo; /*!< file d'attente pour les messages de log en mode asynchrone */
    pthread_t loggerThread; /*!< thread pour traiter les messages de log en mode asynchrone */
    int loggerThreadRunning; /*!< indique si le thread de logging est en cours d'exécution */
} logger_t;

/**
 * @brief Initialise le système de journalisation.
 * @param logger Pointeur vers l'instance du logger à initialiser.
 * @param level Le niveau de journalisation à utiliser. Les messages avec un niveau inférieur seront ignorés.
 * @param callback La fonction de rappel pour traiter les messages de journalisation.
 */
void logger_init(logger_t *logger, log_level_t level, log_callback_t callback);

/**
 * @brief Journalise un message en mode synchrone.
 * @param logger Pointeur vers l'instance du logger.
 * @param level Niveau de criticité du message.
 * @param data Pointeur générique contenant les données à journaliser.
 * @note Si le niveau du message est inférieur au niveau configuré, il est ignoré.
 */
void logger_log_sync(logger_t *logger, log_level_t level, void *data);

/**
 * @brief Journalise un message en mode asynchrone.
 * @param logger Pointeur vers l'instance du logger.
 * @param level Niveau de criticité du message.
 * @param data Pointeur générique contenant les données à journaliser.
 * @note Cette fonction retourne immédiatement, le message sera traité par le thread de logging.
 */
void logger_log_async(logger_t *logger, log_level_t level, void *data);

/**
 * @brief Libère les ressources du système de journalisation.
 * @param logger Pointeur vers l'instance du logger.
 * @note Cette fonction doit être appelée pour nettoyer le thread et les sémaphores.
 */
void logger_destroy(logger_t *logger);

/**
 * @brief Convertit un niveau de log en chaîne de caractères.
 * @param level Niveau de log.
 * @return Chaîne représentant le niveau de log.
 */
const char* logger_level_to_string(log_level_t level);

/**
 * @brief Convertit une chaîne de caractères en niveau de log.
 * @param levelStr Chaîne représentant le niveau de log.
 * @return Niveau de log correspondant.
 */
log_level_t logger_string_to_level(const char* levelStr);

#endif // LOGGER_H