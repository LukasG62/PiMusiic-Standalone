/**
 * @file session.c
 * @brief Couche session pour les sockets
 * @version 1.0
 * @author Lukas Grando
*/

#include "session.h"

#define CHECK(sts,msg) if ((sts) == -1) {perror(msg);exit(-1);} /*!< Macro de vérification d'erreur */
#define CHECK_MALLOC(sts,msg) if ((sts) == NULL) {perror(msg);exit(-1);} /*!< Macro de vérification d'erreur d'allocation mémoire */

#ifdef SESSION_DEBUG /* Si SESSION_DEBUG est défini, on affiche les messages de debug */
    #define SESSION_DEBUG_PRINT(...) if (SESSION_DEBUG) { fprintf(stderr, "[SESSION_DEBUG] "); fprintf(stderr, __VA_ARGS__); }
#else
    #define SESSION_DEBUG_PRINT(...)
#endif

#define SET_COLOR(color) printf("\033[%sm", color) /*!< Macro pour changer la couleur du texte */
#define RESET_COLOR() printf("\033[0m") /*!< Macro pour réinitialiser la couleur du texte */
#define LIGHT_BLUE_COLOR "94" /*!< Couleur bleu clair */


/**
 * \fn      int createSocket(int mode);
 * \brief   Création d'une socket en mode TCP ou UDP
 * \param   mode : mode de la socket à créer (SOCK_STREAM/SOCK_DGRAM)
 * \return  Un pointeur vers une structure socket_t contenant la socket créée
 * \note    Cette fonction est utilisée pour les sockets TCP et UDP
 * \see     socket_t
 */
socket_t *createSocket(int mode) {
    socket_t *sock;
    int fd;
    CHECK(fd=socket(PF_INET, mode, 0), "__SOCKET__");
    sock = initSocket(mode, fd);
    return sock;
}

/**
 * \fn      createAddress(char *ip, int port);
 * \brief   Création d'une structure sockaddr_in à partir d'une adresse IP et d'un port
 * \param   ip : adresse ip
 * \param   port : port
 * \return  structure sockaddr_in 
*/
struct sockaddr_in createAddress(char *ip, int port) {
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons (port);
    addr.sin_addr.s_addr = inet_addr(ip);
    memset(&addr.sin_zero, 0, 8);
    return addr;
}

/**
 * \fn      socket_t *createBindedSocket(int mode, char *ip, int port);
 * \brief   Création d'une socket en mode TCP ou UDP et bindée à une adresse IP et un port
 * \param   mode : mode de la socket à créer (SOCK_STREAM/SOCK_DGRAM)
 * \param   ip : adresse ip
 * \param   port : port
 * \return  Un pointeur vers une structure socket_t contenant la socket créée
 * \see    socket_t
 */
socket_t *createBindedSocket(int mode, char *ip, int port) {
    socket_t *sock = createSocket(mode);
    sock->localAddr = createAddress(ip, port);
    CHECK(bind(sock->fd, (struct sockaddr *)&sock->localAddr, sizeof(sock->localAddr)), "__BIND__");

    // On récupère l'adresse locale de la socket
    setLocalAddress(sock);

    return sock;
}

/**
 * \fn      socket_t *createBindedSocket(int mode, char *ip, int port);
 * \brief   Création d'une socket en mode TCP ou UDP et bindée à une adresse IP et un port
 * \param   mode : mode de la socket à créer (SOCK_STREAM/SOCK_DGRAM)
 * \param   ip : adresse ip
 * \param   port : port
 * \return  Un pointeur vers une structure socket_t contenant la socket créée
 * \see    socket_t
 */
socket_t *createListeningSocket(char *ip, int port, short maxClients) {
    socket_t *sock = createBindedSocket(SOCK_STREAM, ip, port);
    CHECK(listen(sock->fd, maxClients), "__LISTEN__");

    // On récupère l'adresse locale de la socket
    setLocalAddress(sock);

    return sock;
}

/**
 * \fn     socket_t *connectToServer(char *ip, int port);
 * \brief  Demande de connexion à un serveur
 * \param  ip : adresse ip du serveur
 * \param  port : port du serveur
 * \return Un pointeur vers une structure socket_t contenant la socket créée
 * \see    socket_t
 */
socket_t *connectToServer(char *ip, int port) {
    socket_t *sock = createSocket(SOCK_STREAM);
    struct sockaddr_in addr = createAddress(ip, port);
    CHECK(connect(sock->fd, (struct sockaddr *)&addr, sizeof addr), "__CONNECT__");
    // On récupère l'adresse distante de la socket
    setRemoteAddress(sock);
    // On recupère l'adresse locale de la socket
    setLocalAddress(sock);

    return sock;
}

/**
 * \fn     readFromSocket(socket_t *sock, int size);
 * \brief  Lecture d'un message sur une socket
 * \param  sock : la structure socket_t contenant la socket
 * \param  size : taille du message à lire
 * \param  optBuff : buffer dans lequel on va stocker le message
 * \return buff : buffer dans lequel on a stocké le message
 * \note   Si buff est NULL, on alloue la mémoire
 * \warning Si buff n'est pas NULL, il faut libérer la mémoire après utilisation
*/
char *readFromSocket(socket_t *sock, int size, char *optBuff) {
    char *buff;
    // Si buff est NULL, on alloue la mémoire
    if(optBuff == NULL) buff = (char *)malloc(size);
    else buff = optBuff;

    CHECK_MALLOC(buff, "__MALLOC__");
    CHECK(read(sock->fd, buff, size), "__READ__");
    return buff;
}
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
char *recvFromSocket(socket_t *sock, int size, char *buff) {
    if(sock->mode != SOCK_DGRAM) {
        fprintf(stderr, "La socket n'est pas en mode UDP\n");
        exit(-1);
    }
    socklen_t len = sizeof(sock->remoteAddr);
    CHECK(recvfrom(sock->fd, buff, size, 0, (struct sockaddr *)&sock->remoteAddr, &len), "__RECVFROM__");
    SESSION_DEBUG_PRINT("Message recu de [%s]:[%d]\n", inet_ntoa(sock->remoteAddr.sin_addr), ntohs(sock->remoteAddr.sin_port));
    return buff;
}

/**
 * \fn     void writeToSocket(socket_t *sock, char *buff, int size);
 * \brief  Ecriture d'un message sur une socket
 * \param  sock : la structure socket_t contenant la socket
 * \param  buff : message à envoyer
*/
void writeToSocket(socket_t *sock, char *buff) {
    int len = strlen(buff)+1;
    CHECK(write(sock->fd, buff, len), "__WRITE__");
}

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
void sendToSocket(socket_t *sock, char *buff, char *ip, int port) {
    if(sock->mode != SOCK_DGRAM) {
        fprintf(stderr, "La socket n'est pas en mode UDP\n");
        exit(-1);
    }
    
    struct sockaddr_in addr = createAddress(ip, port);
    CHECK(sendto(sock->fd, buff, strlen(buff)+1, 0, (struct sockaddr *)&addr, sizeof addr), "__SENDTO__");
}

/**
 * \fn    int acceptClient(int sock);
 * \brief Acceptation d'un client
 * \param sock : file descriptor de la socket
 * \return file descriptor de la socket du client accepté
*/
socket_t *acceptClient(socket_t *sock) {
    struct sockaddr_in addr;
    int len = sizeof(addr);
    int client = accept(sock->fd, (struct sockaddr *)&addr, &len);
    CHECK(client, "__ACCEPT__");
    SESSION_DEBUG_PRINT("Client connecté [%s]:[%d]\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    socket_t *clientSocket = initSocket(SOCK_STREAM, client);
    clientSocket->remoteAddr = addr;

    // Recuperation de l'adresse locale de la socket
    setLocalAddress(clientSocket);

    return clientSocket;
}

/**
 * \fn   void getServerAddress(int argc, char **argv, char **ip, int *port);
 * \brief recupère l'adresse ip et le port du serveur depuis la ligne de commande
 * \param argc : nombre d'arguments
 * \param argv : tableau d'arguments 
 * \param ip : adresse ip du serveur (passée par adresse)
 * \param port : port du serveur (passé par adresse)
*/
void getServerAddress(int argc, char **argv, char **ip, short *port) {
    // Vérification du nombre d'arguments
    if (argc < 3) {
        // Si pas assez d'arguments, on affiche l'usage et on quitte
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(-1);
    }
    
    SESSION_DEBUG_PRINT("Adresse du serveur: %s:%d\n", argv[1], atoi(argv[2]));
    *ip = argv[1];
    *port = (short) atoi(argv[2]);
}

/**
 * \fn      socket_t *initSocket(int mode, int fd);
 * \brief   Initialisation d'une structure socket_t
 * \param   mode : mode de la socket à créer (SOCK_STREAM/SOCK_DGRAM)
 * \param   fd : file descriptor de la socket
 * \return  Un pointeur vers une structure socket_t contenant la socket créée
 * \note    Cette fonction est utilisée pour les sockets TCP et UDP
*/
socket_t *initSocket(int mode, int fd) {
    socket_t *sock = (socket_t *) malloc(sizeof(socket_t));
    CHECK_MALLOC(sock, "__MALLOC__");
    sock->fd = fd;
    sock->mode = mode;
    memset(&sock->localAddr, 0, sizeof(sock->localAddr));
    memset(&sock->remoteAddr, 0, sizeof(sock->remoteAddr));
    return sock;
}

/**
 * \fn      void freeSocket(socket_t *sock);
 * \brief   Libération de la mémoire allouée pour une structure socket_t ainsi que la fermeture de la socket
 * \param   sock : pointeur vers la structure socket_t à libérer
 * \note    Cette fonction est utilisée pour les sockets TCP et UDP
*/
void freeSocket(socket_t *sock) {
    if( sock == NULL) return;
    close(sock->fd);
    // On libère la mémoire
    free(sock);
}

/**
 * \fn    void printSocket(socket_t *sock);
 * \brief Affichage des informations d'une socket (mode, adresse locale, adresse distante)
 * \param sock : pointeur vers la structure socket_t à afficher
 * \note  Cette fonction est utilisée pour les sockets TCP et UDP
 * \see   socket_t
*/
void printSocket(socket_t *sock) {
    SET_COLOR(LIGHT_BLUE_COLOR);
    printf("<Socket:{\n");
    printf(" Mode : ");
    RESET_COLOR();
    if(sock->mode == SOCK_STREAM) printf("TCP\n");
    else if(sock->mode == SOCK_DGRAM) printf("UDP\n");
    else printf("UNKNOWN\n");
    SET_COLOR(LIGHT_BLUE_COLOR);
    printf(" Local: ");
    RESET_COLOR();
    printf("%s:%d\n", inet_ntoa(sock->localAddr.sin_addr), ntohs(sock->localAddr.sin_port));
    SET_COLOR(LIGHT_BLUE_COLOR);
    printf(" Remote: ");
    RESET_COLOR();
    printf("%s:%d\n", inet_ntoa(sock->remoteAddr.sin_addr), ntohs(sock->remoteAddr.sin_port));
    SET_COLOR(LIGHT_BLUE_COLOR);
    printf("}>");
    RESET_COLOR();
    printf("\n");
}


/**
 * \fn    addr_t getLocalAddress(socket_t *sock);
 * \brief Récupération de l'adresse locale d'une socket et stockage dans la structure socket_t
 * \param sock : pointeur vers la structure socket_t
 * \return addr_t : structure contenant l'adresse locale
 * \note  Cette fonction est utilisée pour les sockets TCP et UDP
 * \see   socket_t
*/
struct sockaddr_in setLocalAddress(socket_t *sock) {
    // On récupère l'adresse locale de la socket
    socklen_t len = sizeof(sock->localAddr);
    CHECK(getsockname(sock->fd, (struct sockaddr *)&sock->localAddr, &len), "__GETSOCKNAME__");
    return sock->localAddr;

}

/**
 * \fn    addr_t getRemoteAddress(socket_t *sock);
 * \brief Récupération de l'adresse distante d'une socket et stockage dans la structure socket_t
 * \param sock : pointeur vers la structure socket_t
 * \return addr_t : structure contenant l'adresse distante
 * \note  Cette fonction est utilisée pour les sockets TCP et UDP
 * \see   socket_t
*/
struct sockaddr_in setRemoteAddress(socket_t *sock) {
    // On récupère l'adresse distante de la socket
    socklen_t len = sizeof(sock->remoteAddr);
    CHECK(getpeername(sock->fd, (struct sockaddr *)&sock->remoteAddr, &len), "__GETPEERNAME__");
    return sock->remoteAddr;
}
