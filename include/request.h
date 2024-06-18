/**
 * @file request.h
 * @brief Interface contenant les fonctions pour envoyer des requêtes au serveur MPP
 * @version 1.0
 * @author Lukas Grando
 */

#ifndef REQUEST_H
#define REQUEST_H

#include "mpp.h"

/**
 * @fn mpp_response_t send_connection_request(socket_t *socket, char *rfid)
 * @brief Envoie une requête de connexion
 * @param socket 
 * @param rfid 
 * @return mpp_response_t 
 */
mpp_response_t send_connection_request(socket_t *socket, char *rfid);

/**
 * @fn mpp_response_t send_list_music_request(socket_t *socket, char *rfid)
 * @brief Envoie une requête pour obtenir la liste des musiques
 * @param socket La socket de connexion au serveur
 * @param rfid Le rfid de l'utilisateur
 * @return mpp_response_t 
 */
mpp_response_t send_list_music_request(socket_t *socket, char *rfid);

/**
 * @fn mpp_response_t send_save_music_request(socket_t *socket, char *rfid, music_t *music)
 * @brief Envoie une requête pour sauvegarder une musique
 * @param socket La socket de connexion au serveur
 * @param rfid Le rfid de l'utilisateur
 * @param music La musique à sauvegarder
 * @return mpp_response_t 
 */
mpp_response_t send_save_music_request(socket_t *socket, char *rfid, music_t *music);

/**
 * @fn mpp_response_t send_delete_music_request(socket_t *socket, char *rfid, time_t musicId)
 * @brief Envoie une requête pour supprimer une musique
 * @param socket La socket de connexion au serveur
 * @param rfid Le rfid de l'utilisateur
 * @param musicId L'id musique à supprimer
 * @return mpp_response_t 
 */

mpp_response_t send_delete_music_request(socket_t *socket, char *rfid, time_t musicId);
/**
 * @fn mpp_response_t send_get_music_request(socket_t *socket, char *rfid, time_t musicId)
 * @brief Envoie une requête pour récupérer une musique
 * @param socket La socket de connexion au serveur
 * @param rfid Le rfid de l'utilisateur
 * @param musicId L'identifiant de la musique
 * @return mpp_response_t 
 * @see mpp_response_t
 */
mpp_response_t send_get_music_request(socket_t *socket, char *rfid, time_t musicId);

/**
 * @fn mpp_response_t client_request_handler(mpp_request_code_t code , char *rfid, music_t *music, time_t musicId)
 * @brief Fonction qui gère les requêtes du client
 * @param code Le code de la requête à envoyer
 * @param rfid Le rfid de l'utilisateur
 * @param music La musique à envoyer (si nécessaire)
 * @param musicId L'identifiant de la musique (si nécessaire)
 * @return mpp_response_t La réponse du serveur
*/
mpp_response_t client_request_handler(mpp_request_code_t code , char *rfid, music_t *music, time_t musicId);


#endif