/**
 * @file core.h
 * @brief Fichier d'en-tête pour les services centraux de l'application
 */
#ifndef CORE_H
#define CORE_H

#include "common.h"
#include "ui/base/ui_menu.h"
#include "inet/request.h"
#include "music/sound.h"
#include "inet/mysyscall.h"
#include "event/event.h"
#include "event/event_queue.h"
#include "core/logger.h"

typedef struct {
	ui_menu_credentials_t credentials; /*!< Les identifiants de connexion de l'utilisateur */
	music_t music; /*!< La musique en cours de création ou de lecture */
	bool isConnected; /*!< L'état de connexion de l'utilisateur */
} app_context_t;

extern volatile bool isRunning; /*!< Indique si l'application est en cours d'exécution */
extern event_queue_t *uiQueue; /*!< File d'événements pour la communication avec l'interface utilisateur */
extern event_queue_t *logicQueue; /*!< File d'événements pour la communication avec la logique métier */
extern app_context_t *appContext; /*!< Contexte global de l'application */
extern logger_t appLogger; /*!< Logger global de l'application */

#define GET_UI_QUEUE() (uiQueue)
#define GET_LOGIC_QUEUE() (logicQueue)
#define GET_APP_CONTEXT() (appContext)

#define UI_NOTIFY(event) notify_event(uiQueue, event)
#define LOGIC_NOTIFY(event) notify_event(logicQueue, event)

#define WAIT_UI_EVENT() wait_event(uiQueue)
#define WAIT_LOGIC_EVENT() wait_event(logicQueue)

#define TRYWAIT_UI_EVENT() trywait_event(uiQueue)
#define TRYWAIT_LOGIC_EVENT() trywait_event(logicQueue)

/**
 * \brief Initialise le cœur de l'application avec ses services
 * \param uiQueue La file d'événements pour la communication avec l'interface utilisateur
 * \param logicQueue La file d'événements pour la communication avec la logique métier
 * \param appContext Le contexte global de l'application à initialiser
 */
void core_init(event_queue_t *uiQueue, event_queue_t *logicQueue, logger_t *logger, app_context_t *appContext);




#endif // CORE_H