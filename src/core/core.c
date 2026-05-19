/**
 * \file core.c
 * \brief Fichier source pour les services centraux de l'application
 * \author Lukas Grando
 */

#include "core/core.h"

/**
 * \brief Initialise le cœur de l'application avec ses services
 * \details Cette fonction initialise les files d'événements pour la communication entre les différents services de l'application
 * @param ui La file d'événements pour la communication avec l'interface utilisateur
 * @param logic La file d'événements pour la communication avec la logique métier
 * @note Cette fonction doit être appelée avant de lancer les différents services de l'application
 */
void core_init(event_queue_t *ui, event_queue_t *logic, logger_t *logger, app_context_t *appContext) {
	uiQueue = ui;
	logicQueue = logic;
	appLogger = *logger;
	appContext = appContext;
}