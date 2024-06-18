/**
 * @file data.c
 * @brief Fichier source pour la bibliothèque data.
 * @version 1.0
 * @author Lukas Grando
 */

#include "data.h"

/**
 * @fn void send_data_DGRAM(socket_t *socket, generic_t data, serialize_t serializeFunc, char *ip, int port)
 * @brief Envoie des données sérialisées en datagramme. 
 * @param socket La socket sur lequel envoyer les données.
 * @param data Les données à envoyer.
 * @param serializeFunc La fonction de sérialisation à utiliser pour envoyer les données.
 * @param ip L'adresse IP de destination.
 * @param port Le port de destination.
 * @note A n'utiliser que pour les sockets UDP
 * @note Si serializeFunc est NULL, cela considére que data est un buffer_t
 * @see send_data
 */
void send_data_DGRAM(socket_t *socket, generic_t data, serialize_t serializeFunc, char *ip, int port) {
    char *buff = (char *) malloc(sizeof(buffer_t));
    if(serializeFunc == NULL) {
        DATA_DEBUG_PRINT("[SEND_DATA]\n[BEGIN]\n%s\n[END]\n", (char *)data);
        sendToSocket(socket, (char *)data, ip, port);
        return;
    } 
    serializeFunc(data, buff);
    DATA_DEBUG_PRINT("[SEND_DATA]\n[BEGIN]\n%s\n[END]\n", buff);
    sendToSocket(socket, buff, ip, port);
    free(buff);
}

/**
 * @fn void send_data_stream(socket_t *socket, generic_t data, serialize_t serializeFunc)
 * @brief La fonction privé permettant d'envoyer des données sérialisées en mode stream.
 * @param socket La socket sur lequel envoyer les données.
 * @param data Les données à envoyer.
 * @param serializeFunc La fonction de sérialisation à utiliser pour envoyer les données.
 * @note A n'utiliser que pour les sockets TCP
 * @note Si serializeFunc est NULL, cela considére que data est un buffer_t
 */
void send_data_stream(socket_t *socket , generic_t data, serialize_t serializeFunc) {
    // On evite d'allouer de la mémoire sur la stack car buffer_t est assez grand (262144)
    char *buff = (char *) malloc(sizeof(buffer_t));
    if(serializeFunc == NULL) {
        writeToSocket(socket, (char *)data);
        DATA_DEBUG_PRINT("[SEND_DATA]\n[BEGIN]\n%s\n[END]\n", (char *)data);
        return;
    }

    serializeFunc(data, buff);
    DATA_DEBUG_PRINT("[SEND_DATA]\n[BEGIN]\n%s\n[END]\n", buff);
    writeToSocket(socket, buff);
    free(buff);
}

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
void send_data(socket_t *socket, generic_t data, serialize_t serializeFunc, ...) {
    if(socket->mode == SOCK_STREAM) {
        send_data_stream(socket, data, serializeFunc);

    } else if(socket->mode == SOCK_DGRAM) {
        // TODO : Envoyer des données sérialisées en datagramme
        // Récupération des arguments
        va_list args;
        va_start(args, serializeFunc);
        char *ip = va_arg(args, char*);
        int port = va_arg(args, int);
        va_end(args);
        send_data_DGRAM(socket, data, serializeFunc, ip, port);
    }

}

/**
 * @fn void recv_data_stream(socket_t *socket, generic_t data, serialize_t dserializeFunc)
 * @brief Fonction privé permettant de recevoir des données sérialisées en mode stream.
 * @param socket Socket sur lequel recevoir les données.
 * @param data pointeur vers la donnée générique à remplir
 * @param dserializeFunc Fonction de désérialisation à utiliser pour remplir la donnée générique.
 * @see recv_data
*/
void recv_data_stream(socket_t *socket, generic_t data, serialize_t dserializeFunc) {
    char *buff = (char *) malloc(sizeof(buffer_t));
    readFromSocket(socket, MAX_BUFF, buff);
    DATA_DEBUG_PRINT("[RECV_DATA]\n[BEGIN]\n%s\n[END]\n", buff);
    if(dserializeFunc == NULL) {
        strcpy((char *)data, buff);
    } else {
        dserializeFunc(buff, data);
    }
    free(buff);
}

/**
 * @fn void recv_data_DGRAM(socket_t *socket, generic_t data, serialize_t dserializeFunc)
 * @brief Recevoir des données sérialisées en datagramme.
 * @param socket La socket sur lequel recevoir les données.
 * @param data Les données à recevoir.
 * @param dserializeFunc La fonction de désérialisation à utiliser pour recevoir les données.
 * @note A n'utiliser que pour les sockets UDP
 */
void recv_data_DGRAM(socket_t *socket, generic_t data, serialize_t dserializeFunc) {
    char *buff = (char *) malloc(sizeof(buffer_t));
    recvFromSocket(socket, MAX_BUFF, buff);
    DATA_DEBUG_PRINT("[RECV_DATA]\n[BEGIN]\n%s\n[END]\n", buff);
    if(dserializeFunc == NULL) {
        strcpy((char *)data, buff);
    } else {
        dserializeFunc(buff, data);
    }
    free(buff);
}

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
void recv_data(socket_t *socket, generic_t data, serialize_t dserializeFunc, ...) {
    if(socket->mode == SOCK_STREAM) {
        recv_data_stream(socket, data, dserializeFunc);
    } else if(socket->mode == SOCK_DGRAM) {
        // TODO : Recevoir des données sérialisées en datagramme
        // Récupération des arguments
        va_list args;
        va_start(args, dserializeFunc);
        va_end(args);
        recv_data_DGRAM(socket, data, dserializeFunc);
    }
    
}
