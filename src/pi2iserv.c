/**
 * @file pi2iserv.c
 * @brief Serveur de musique MPP
 * @version 1.0
 * @author Lukas Grando
 */

#include "mpp.h"
#include "data.h"
#include "session.h"
#include "mysyscall.h"

socket_t *se = NULL; /*!< Socket d'écoute */
sem_t *accessDB; /*!< Sémaphore pour l'accès à la base de données */

/**
 * @fn void exitFunction()
 * @brief Fonction de fermeture
 */
void exitFunction();

/**
 * @fn void *handleClient(void *arg)
 * @brief Fonction qui gère un client
 * @param arg 
 * @return void* 
 */
void *handleClient(void *arg) {
    // On récupère la socket
    socket_t *sd = (socket_t *) arg;
    // On alloue la mémoire pour la requête et la réponse 
    mpp_request_t *request = malloc(sizeof(mpp_request_t));
    mpp_response_t *response = malloc(sizeof(mpp_response_t));

    // On initialise la réponse et la requête à une erreur par défaut
    *response = create_mpp_response(MPP_RESPONSE_BAD_REQUEST, "", NULL, NULL);
    *request = create_mpp_request(MPP_CONNECT, "", NULL, NO_MUSIC_ID);

    // On attend la requête du client
    recv_data(sd, request, (serialize_t) deserialize_mpp_request);
    wait_sem(accessDB);
    // On traite la requête
    switch (request->code) {
        case MPP_CONNECT:
            connect_handler(sd, request, response);
            break;

        case MPP_LIST_MUSIC:
            list_music_handler(sd, request, response);
            break;

        case MPP_ADD_MUSIC:
            add_music_handler(sd, request, response);
            break;

        case MPP_GET_MUSIC:
            get_music_handler(sd, request, response);
            break;

        case MPP_DELETE_MUSIC:
            delete_music_handler(sd, request, response);
            break;

        default:
            *response = create_mpp_response(MPP_RESPONSE_BAD_REQUEST, "", NULL, NULL);
            break;
    }

    // Envoi de la réponse au client
    send_data(sd, response, (serialize_t) serialize_mpp_response);
    post_sem(accessDB);
    // Fermeture de la socket
    freeSocket(sd);

    // Libération de la mémoire
    free_response(response);
    free_request(request);
    // Fin du thread
    pthread_exit(NULL);
}

int main() {
    // Ajout de la fonction de fermeture
    atexit(exitFunction);
    // Initialisation du masque de signaux et ajout du signal SIGINT
    install_signal_handler(SIGINT, exitFunction, 0);
    // Initialisation de la sémaphore
    accessDB = create_sem(1);
    // Creation des répertoires personnels pour les utilisateurs
    create_user_directories();
    // Création de la socket d'écoute
    se = createListeningSocket(MPP_DEFAULT_IP, MPP_DEFAULT_PORT, MPP_MAX_CLIENTS);
    socket_t *sd;

    while(1) {
        sd = acceptClient(se);
        pthread_t thread; 
        // Create a new thread for each client
        pthread_create(&thread, NULL, handleClient, (void *) sd);
        pthread_detach(thread);
    }

    return EXIT_SUCCESS;
}

/**
 * @fn void exitFunction()
 * @brief Fonction de fermeture
 */
void exitFunction() {
    fprintf(stderr, "[PI2ISERV] Fermeture du serveur\n");
    if (se != NULL) {
        freeSocket(se);
        se = NULL;
    }
    if(accessDB != NULL) {
        destroy_sem(accessDB);
        accessDB = NULL;
    }

    exit(EXIT_SUCCESS);
}