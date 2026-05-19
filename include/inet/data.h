/**
 * @file data.h
 * @author Lukas Grando
 * @brief Fichier d'en-tête pour la bliothèque data.
 * @version 1.0
 * @author Lukas Grando
 */

#ifndef DATA_H
#define DATA_H

#include "session.h"
#include <stdarg.h>

#ifdef DATA_DEBUG /*!< Si DATA_DEBUG est défini */
    #define DATA_DEBUG_PRINT(...) fprintf(stderr, "[DATA_DEBUG] "); fprintf(stderr, __VA_ARGS__)
#else
    #define DATA_DEBUG_PRINT(...)
#endif


// Creation des types de données
#define MAX_BUFF 262144 //!< Taille maximale du buffer (2^18) puissance proche de 2 de la taille maximale

typedef char buffer_t[MAX_BUFF]; //!< Type de buffer
typedef void* generic_t; //!< Type de donnée générique

typedef void (*serialize_t)(generic_t, generic_t); //!< Fonction de sérialisation prenant en paramètre un pointeur vers une donnée générique et un pointeur vers un buffer.

/**
 * @fn void send_data(socket_t *socket, generic_t data, serialize_t serializeFunc, ...)
 * @brief Fonction permettant d'envoyer des données sérialisées.
 * @param socket La socket sur lequel envoyer les données.
 * @param data Les données à envoyer.
 * @param serializeFunc La fonction de sérialisation à utiliser pour envoyer les données.
 * @param ... Les arguments supplémentaires
 * @note Si serializeFunc est NULL, cela considére que data est un buffer_t
 * @note Si serializeFunc est NULL, cela considére que data est un buffer_t
 * @see send_data_DGRAM
 * @see send_data_stream
 */
void send_data(socket_t *socket, generic_t data, serialize_t serializeFunc, ...);

/**
 * @fn void recv_data(socket_t *socket, generic_t data, serialize_t dserializeFunc, ...)
 * @brief La fonction permettant de recevoir des données sérialisées.
 * @param socket La socket sur lequel recevoir les données.
 * @param data Les données à recevoir.
 * @param dserializeFunc La fonction de désérialisation à utiliser pour recevoir les données.
 * @param ... Les arguments supplémentaires
 * @note Si dserializeFunc est NULL, cela considére que data est un buffer_t
 * @see recv_data_DGRAM
 * @see recv_data_stream
 */
void recv_data(socket_t *socket, generic_t data, serialize_t dserializeFunc, ...);

#endif // DATA_H