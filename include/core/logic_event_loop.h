/**
 * @file logic_event_loop.h
 * @brief En-tête de la boucle d'événements logique
 */
#ifndef LOGIC_EVENT_LOOP_H
#define LOGIC_EVENT_LOOP_H

#include "core.h"

/**
 * @fn void *logic_event_loop_run(void *arg)
 * @brief Point d'entrée du thread logique
 * @param arg Arguments du thread
 */
void *logic_event_loop_run(void *arg);

#endif