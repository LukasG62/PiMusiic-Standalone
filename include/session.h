/**
 * \file    session.h
 * \brief   Fichier d'en-tête pour la bibliothèque session
 * \author  Lukas Grando
 * \version 2.0
*/
#ifndef _SESSION_H_
#define _SESSION_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

/**
 * \struct socket_t
 * \brief  Structure contenant les informations d'une socket
 * \note   Cette structure est utilisée pour les sockets TCP et UDP 
 * \see   createSocket, createBindedSocket, connectToServer, createListeningSocket
*/
typedef struct {
    int fd;  /*!< file descriptor de la socket */
    short mode; /*!< mode de la socket (SOCK_STREAM/SOCK_DGRAM) */
    struct sockaddr_in localAddr; /*!< structure sockaddr_in contenant l'adresse locale */
    struct sockaddr_in remoteAddr; /*!< structure sockaddr_in contenant l'adresse distante */
} socket_t;

/**
 * \fn      socket_t *initSocket(int mode, int fd);
 * \brief   Initialisation d'une structure socket_t
 * \param   mode : mode de la socket à créer (SOCK_STREAM/SOCK_DGRAM)
 * \param   fd : file descriptor de la socket
 * \return  Un pointeur vers une structure socket_t contenant la socket créée
 * \note    Cette fonction est utilisée pour les sockets TCP et UDP
*/
socket_t *initSocket(int mode, int fd);

/**
 * \fn      void freeSocket(socket_t *sock);
 * \brief   Libération de la mémoire allouée pour une structure socket_t ainsi que la fermeture de la socket
 * \param   sock : pointeur vers la structure socket_t à libérer
 * \note    Cette fonction est utilisée pour les sockets TCP et UDP
*/
void freeSocket(socket_t *sock);

/**
 * \fn      socket_t *createSocket(int mode);
 * \brief   Création d'une socket en mode TCP ou UDP
 * \param   mode : mode de la socket à créer (SOCK_STREAM/SOCK_DGRAM)
 * \return  Un pointeur vers une structure socket_t contenant la socket créée
 * \note    Cette fonction est utilisée pour les sockets TCP et UDP
 * \see     socket_t
 */
socket_t *createSocket(int mode);

/**
 * \fn      struct sockaddr_in createAddress(char *ip, int port);
 * \brief   Création d'une structure sockaddr_in à partir d'une adresse IP et d'un port
 * \param   ip : adresse ip
 * \param   port : port
 * \return  structure sockaddr_in 
*/
struct sockaddr_in createAddress(char *ip, int port);

/**
 * \fn      socket_t *createListeningSocket(int mode, char *ip, int port, shot maxClients);
 * \brief   Création d'une socket en mode TCP ou UDP et bindée à une adresse IP et un port
 * \param   ip : adresse ip
 * \param   port : port
 * \param   maxClients : nombre maximum de clients en attente de connexion
 * \return  socket_t : structure contenant la socket créée
 * \see    socket_t
 */
socket_t *createListeningSocket(char *ip, int port, short maxClients);


/**
 * \fn      socket_t *createBindedSocket(int mode, char *ip, int port);
 * \brief   Création d'une socket en mode TCP ou UDP et bindée à une adresse IP et un port
 * \param   mode : mode de la socket à créer (SOCK_STREAM/SOCK_DGRAM)
 * \param   ip : adresse ip
 * \param   port : port
 * \return  Un pointeur vers une structure socket_t contenant la socket créée
 * \see    socket_t
 */
socket_t *createBindedSocket(int mode, char *ip, int port);

/**
 * \fn     int connectToServer(char *ip, int port);
 * \brief  Demande de connexion à un serveur
 * \param  ip : adresse ip du serveur
 * \param  port : port du serveur
 * \return Un pointeur vers une structure socket_t contenant la socket créée
 * \see    socket_t
 */
socket_t *connectToServer(char *ip, int port);

/**
 * \fn     char *readFromSocket(socket_t *sock, int size, char *buff);
 * \brief  Lecture d'un message sur une socket
 * \param  sock : la structure socket_t contenant la socket
 * \param  size : taille du message à lire
 * \param  buff : buffer dans lequel on va stocker le message
 * \return buff : buffer dans lequel on a stocké le message
 * \note   Si buff est NULL, on alloue la mémoire
 * \warning Si buff n'est pas NULL, il faut libérer la mémoire après utilisation
*/
char *readFromSocket(socket_t *sock, int size, char *buff);

/**
 * \fn     char *recvFromSocket(socket_t *sock, int size, char *buff);
 * \brief  Lecture d'un message sur une socket UDP
 * \param  sock : la structure socket_t contenant la socket
 * \param  size : taille du message à lire
 * \param  buff : buffer dans lequel on va stocker le message
 * \note  Si buff est NULL, on alloue la mémoire
 * \note A n'utiliser que pour les sockets UDP
 * \warning Si buff n'est pas NULL, il faut libérer la mémoire après utilisation
 * \return buff : buffer dans lequel on a stocké le message
*/
char *recvFromSocket(socket_t *sock, int size, char *buff);

/**
 * \fn     void writeToSocket(socket_t *sock, char *buff, int size);
 * \brief  Ecriture d'un message sur une socket
 * \param  sock : la structure socket_t contenant la socket
 * \param  buff : message à envoyer
*/
void writeToSocket(socket_t *sock, char *buff);


/**
 * \fn void sendToSocket(socket_t *sock, char *buff, char *ip, int port);
 * \brief Envoi d'un message à une adresse IP et un port donnés pour une socket UDP
 * \param sock : file descriptor de la socket
 * \param buff : message à envoyer
 * \param ip : adresse ip du destinataire
 * \param port : port du destinataire
 * \note Cette fonction est utilisée pour les sockets UDP
 * \see socket_t
*/
void sendToSocket(socket_t *sock, char *buff, char *ip, int port);

/**
 * \fn    int acceptClient(int sock);
 * \brief Acceptation d'un client
 * \param sock : file descriptor de la socket
 * \return file descriptor de la socket du client accepté
*/
socket_t *acceptClient(socket_t *sock);

/**
 * \fn   void getServerAddress(int argc, char **argv, char **ip, int *port);
 * \brief recupère l'adresse ip et le port du serveur depuis la ligne de commande
 * \param argc : nombre d'arguments
 * \param argv : tableau d'arguments 
 * \param ip : adresse ip du serveur (passée par adresse)
 * \param port : port du serveur (passé par adresse)
*/
void getServerAddress(int argc, char **argv, char **ip, short *port);


/**
 * \fn    void printSocket(socket_t *sock);
 * \brief Affichage des informations d'une socket (mode, adresse locale, adresse distante)
 * \param sock : pointeur vers la structure socket_t à afficher
 * \note  Cette fonction est utilisée pour les sockets TCP et UDP
 * \see   socket_t
*/
void printSocket(socket_t *sock);


/**
 * \fn    addr_t setLocalAddress(socket_t *sock);
 * \brief Récupération de l'adresse locale d'une socket et stockage dans la structure socket_t
 * \param sock : pointeur vers la structure socket_t
 * \return addr_t : structure contenant l'adresse locale
 * \note  Cette fonction est utilisée pour les sockets TCP et UDP
 * \see   socket_t
*/
struct sockaddr_in setLocalAddress(socket_t *sock);

/**
 * \fn    addr_t getRemoteAddress(socket_t *sock);
 * \brief Récupération de l'adresse distante d'une socket et stockage dans la structure socket_t
 * \param sock : pointeur vers la structure socket_t
 * \return addr_t : structure contenant l'adresse distante
 * \note  Cette fonction est utilisée pour les sockets TCP et UDP
 * \see   socket_t
*/
struct sockaddr_in setRemoteAddress(socket_t *sock);

#endif