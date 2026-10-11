/**
 * @file event.h
 * @brief Fichier d'en-tête pour les événements génériques
 */
#ifndef SHARED_EVENT_H
#define SHARED_EVENT_H

#include <stdint.h>
#include <stddef.h>

typedef uint32_t event_type_t;
typedef void (*fn_destroy_data_t)(void *data);

/**
 * @struct event_t
 * @brief Structure générique d'un événement
 */
typedef struct {
    event_type_t type;              /*!< Identifiant ou type de l'événement */
    void *data;                     /*!< Données associées à l'événement */
    fn_destroy_data_t destroy_data; /*!< Fonction pour libérer les données */
} event_t;

/**
 * @brief Crée un événement générique
 * @param type Le type de l'événement
 * @param data Les données associées (peut être NULL)
 * @param destroy_data Fonction de nettoyage des données (peut être NULL)
 * @return event_t* L'événement créé
 */
event_t *create_event(event_type_t type, void *data, fn_destroy_data_t destroy_data);

/**
 * @brief Détruit un événement et libère ses données si destroy_data est fourni
 * @param event L'événement à détruire
 */
void destroy_event(event_t *event);

#endif // SHARED_EVENT_H
