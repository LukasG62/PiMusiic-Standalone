/**
 * @file mpp.h
 * @brief Interface pour le protocol MPP (Music Pi Protocol)
 * @version 1.0
 * @author Lukas Grando
 */

#ifndef MPP_H
#define MPP_H

#include <sys/stat.h>
#include <sys/types.h>

#include "note.h"
#include "data.h"

#define USERNAME_SIZE 15 /*!< Taille du nom d'utilisateur */
#define REALLLOC_SIZE 10 /*!< Taille de réallouement de la liste d'identifiants de musiques */
#define NO_MUSIC_ID -1 /*!< Identifiant de musique non défini */

#define MPP_DEFAULT_PORT 12345 /*!< Port par défaut du serveur MPP */
#define MPP_DEFAULT_IP "127.0.0.1" /*!< Adresse IP par défaut du serveur MPP */
#define MPP_MAX_CLIENTS 10 /*!< Nombre maximum de clients connectés au serveur MPP */

#define MPP_DB_FOLDER "ressources" /*!< Dossier de stockage de la base de données */
#define MPP_DB_USER_FILE "users.db" /*!< Chemin de la base de données */
#define MPP_DB_MUSIC_FOLDER "music" /*!< Dossier de stockage des musiques */
#define MPP_DB_MUSIC_FILE "music.db" /*!< Chemin de la base de données des musiques */



#define BAD_REQUEST(requestPtr) (requestPtr->code == MPP_RESPONSE_BAD_REQUEST) /*!< Vérifie si la requête est incorrecte */
#define NOK_REQUEST(requestPtr) (requestPtr->code == MPP_RESPONSE_NOK) /*!< Vérifie si la réponse est NOK */
#define NOT_FOUND(requestPtr) (requestPtr->code == MPP_RESPONSE_NOT_FOUND) /*!< Vérifie si la musique n'a pas été trouvée */

#define CREATE_BAD_REQUEST(responsePtr) responsePtr->code = MPP_RESPONSE_BAD_REQUEST /*!< Crée une réponse BAD_REQUEST */
#define CREATE_NOK(responsePtr) responsePtr->code = MPP_RESPONSE_NOK /*!< Crée une réponse NOK */
#define CREATE_NOT_FOUND(responsePtr) responsePtr->code = MPP_RESPONSE_NOT_FOUND /*!< Crée une réponse NOT_FOUND */


/**
 * \enum mpp_request_code_t
 * \brief Code de requête MPP
*/
typedef enum {
    MPP_CONNECT = 200, /*!< Requête de connexion */
    MPP_LIST_MUSIC = 300, /*!< Requête pour lister les musiques */
    MPP_GET_MUSIC, /*!< Requête pour récupérer une musique */
    MPP_ADD_MUSIC, /*!< Requête pour ajouter une musique */
    MPP_DELETE_MUSIC, /*!< Requête pour supprimer une musique */
} mpp_request_code_t;

/**
 * \enum mpp_response_code_t
 * \brief Code de réponse MPP
*/
typedef enum {
    MPP_RESPONSE_OK = 200, /*!< Réponse OK */
    MPP_RESPONSE_MUSIC_UPDATED = 201, /*!< Réponse pour indiquer que la musique a été mise à jour */
    MPP_RESPONSE_MUSIC_CREATED = 202, /*!< Réponse pour indiquer que la musique a été créée */
    MPP_RESPONSE_NOK = 400, /*!< Réponse NOK */
    MPP_RESPONSE_BAD_REQUEST = 401, /*!< Réponse pour indiquer que la requête est incorrecte */
    MPP_RESPONSE_NOT_FOUND = 404, /*!< Réponse pour indiquer que la musique n'a pas été trouvée */
} mpp_response_code_t;

/**
 * \struct musicId_list_t
 * \brief Structure de liste d'identifiants de musiques
 * \details elle contient une liste de timestamp de musiques et sa taille 
*/
typedef struct {
    time_t *musicIds; /*!< Liste des identifiants de musiques */
    int size;       /*!< Taille de la liste */
} musicId_list_t;

/**
 * \struct mpp_request_t
 * \brief Structure de representant une requête MPP
 * \details elle contient tous les champs pouvant être envoyés dans une requête MPP
*/
typedef struct {
    mpp_request_code_t code; /*!< Code de la requête */
    char rfidId[10]; /*!< Nom d'utilisateur */
    music_t *music ; /*!< Musique à envoyer */
    time_t musicId; /*!< Identifiant de la musique selectionnée (timestamp) */
} mpp_request_t;


/**
 * \struct mpp_response_t
 * \brief Structure de representant une réponse MPP
 * \details elle contient tous les champs pouvant être envoyés dans une réponse MPP
*/
typedef struct {
    mpp_response_code_t code; /*!< Code de la réponse */
    char username[USERNAME_SIZE]; /*!< Nom de l'utilisateur */
    music_t *music ; /*!< Musique à envoyer */
    musicId_list_t *musicIds ; /*!< Liste des identifiants de musiques */
} mpp_response_t;

/**
 * \fn mpp_request_t create_mpp_request(mpp_request_code_t code, char *rfidId, music_t *music, time_t musicId);
 * \brief Créer une requête MPP
 * \param code Le code de la requête
 * \param rfidId L'identifiant RFID
 * \param music La musique
 * \param musicId L'identifiant de la musique
 * \return mpp_request_t 
 */
mpp_request_t create_mpp_request(mpp_request_code_t code, char *rfidId, music_t *music, time_t musicId);

/**
 * @fn mpp_response_t create_mpp_response(mpp_response_code_t code, char *username, music_t *music, musicId_list_t *musicIds);
 * @brief Créer une réponse MPP
 * @param code Le code de la réponse
 * @param username Le nom d'utilisateur
 * @param music Le pointeur vers la musique
 * @param musicIds La liste des identifiants de musiques
 * @return mpp_response_t 
 */
mpp_response_t create_mpp_response(mpp_response_code_t code, char *username, music_t *music, musicId_list_t *musicIds);

/**
 * @fn void print_mpp_request(mpp_request_t *request);
 * @brief Affiche une requête MPP
 * @param request La requête MPP
 */
void print_mpp_request(mpp_request_t *request);

/**
 * @fn void print_mpp_response(mpp_response_t *response);
 * @brief Affiche une réponse MPP
 * @param response 
 * @see mpp_response_t
 */
void print_mpp_response(mpp_response_t *response);

/**
 * @fn void serialize_music(music_t *music, buffer_t buffer);
 * @brief Sérialise une requête MPP
 * La requête est sérialisée de la manière suivante :
 * <code> <rfidId>
 * <music>
 * @param request 
 * @param buffer 
 */
void serialize_mpp_request(mpp_request_t *request, buffer_t buffer);

/**
 * @fn void deserialize_mpp_request(buffer_t buffer, mpp_request_t *request);
 * @brief Désérialise une requête MPP
 * @param buffer Le buffer ou se trouve la requête sérialisée
 * @param request La requête MPP à remplir
 * @see serialize_mpp_request
 */
void deserialize_mpp_request(buffer_t buffer, mpp_request_t *request);

/**
 * @fn void serialize_music(music_t *music, buffer_t buffer);
 * @brief Sérialise une reponse MPP
 * La réponse est sérialisée de la manière suivante :
 * <code> <username>
 * <list_size> <musicId> <musicId> <musicId> ...
 * <music> (même format que pour la requête)
 * @param response La réponse MPP
 * @param buffer Le buffer dans lequel sérialiser la réponse
 */
void serialize_mpp_response(mpp_response_t *response, buffer_t buffer);

/**
 * @fn void deserialize_mpp_response(buffer_t buffer, mpp_response_t *response);
 * @brief Désérialise une réponse MPP
 * @param buffer Le buffer ou se trouve la réponse sérialisée
 * @param response La réponse MPP à remplir
 */
void deserialize_mpp_response(buffer_t buffer, mpp_response_t *response);

/**
 * @fn void serialize_music(music_t *music, buffer_t buffer);
 * @brief Convertit un code de requête MPP en chaîne de caractères
 * @param code 
 * @return char* 
 */
char *code2str_request(mpp_request_code_t code);

/**
 * @fn char *code2str_response(mpp_response_code_t code);
 * @brief Convertit un code de réponse MPP en chaîne de caractères
 * @param code Le code de réponse MPP
 * @return char* La chaîne de caractères
 * @warning La chaine de caractères doit être libérée après utilisation
 * @see mpp_response_code_t
 */
char *code2str_response(mpp_response_code_t code);

/**
 * @fn void init_music(music_t *music, time_t date);
 * @brief initialise une liste d'identifiants de musiques
 * @param list Liste de musiques
 * @note la liste est dynamiquement allouée en utilisant add_music_id
 * @see add_music_id
 */
void init_music_list(musicId_list_t *list);

/**
 * @fn void add_music_id(musicId_list_t *list, int musicId);
 * @brief Ajoute un identifiant de musique à une liste de musiques
 * @param list Liste de musiques
 * @param musicId L'identifiant de la musique à ajouter
 * @note La liste doit être initialisée avant d'appeler cette fonction
 * @warning La liste est réallouée si nécessaire
 * @see REALLLOC_SIZE
 */
void add_music_id(musicId_list_t *list, int musicId);

/**
 * @fn void remove_music_id(musicId_list_t *list, int index);
 * @brief Supprime un identifiant de musique d'une liste de musiques
 * @param list Liste de musiques
 * @param index L'indice de la musique à supprimer
 */
void remove_music_id(musicId_list_t *list, int index);

/**
 * @fn void free_music_list(musicId_list_t *list);
 * @brief Libère la mémoire allouée pour une liste de musiques
 * @param list Liste de musiques
 */
void free_music_list(musicId_list_t *list);

/**
 * @fn void get_music_list_from_db(musicId_list_t *list, char *rfidId);
 * @brief Récupère la liste des musiques d'un utilisateur depuis la base de données
 * @param list La liste des musiques récupérée
 * @param rfidId L'identifiant RFID de l'utilisateur
 * @note Si l'utilisateur n'existe pas, la liste est vide
 */
void get_music_list_from_db(musicId_list_t *list, char *rfidId);

/**
 * @fn int add_music_to_db(music_t *music, char *rfidId);
 * @brief Ajoute une musique à la base de données
 * @param music La musique à ajouter
 * @param rfidId L'identifiant RFID de l'utilisateur
 * @return int 
 */
int add_music_to_db(music_t *music, char *rfidId);

/**
 * @fn void get_music_from_db(music_t *music, time_t musicId, char *rfidId);
 * @brief Récupère une musique depuis la base de données
 * @param music La musique récupérée
 * @param musicId L'identifiant de la musique à récupérer
 * @param rfidId L'identifiant RFID de l'utilisateur
 */
void get_music_from_db(music_t *music, time_t musicId, char *rfidId);


/**
 * @fn void delete_music_from_db(time_t musicId, char *rfidId);
 * @brief Supprime une musique de la base de données
 * @param musicId L'identifiant de la musique à supprimer
 * @param rfidId L'identifiant RFID de l'utilisateur
 */
void delete_music_from_db(time_t musicId, char *rfidId);

/**
 * @fn int search_music(musicId_list_t *list, time_t musicId);
 * @brief Fonction de recherche d'une musique dans une liste
 * @param list Liste d'identifiants de musiques
 * @param musicId L'identifiant de la musique à rechercher
 * @return int L'indice de la musique dans la liste, -1 si la musique n'est pas trouvée
 * @note La liste doit être initialisée avant d'appeler cette fonction
 */
int search_music(musicId_list_t *list, time_t musicId);

/**
 * @fn void get_username_from_db(char *username, char *rfidId);
 * @brief Récupère le nom d'utilisateur associé à un rfidId depuis la base de données
 * @param username Nom d'utilisateur récupéré
 * @param rfidId L'identifiant RFID de l'utilisateur
 * @note Si l'utilisateur n'existe pas, username est une chaîne vide
 * @warning La chaine username doit être allouée avant l'appel à cette fonction
 */
void get_username_from_db(char *username, char *rfidId);

/**
 * @fn void connect_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * @brief Gère une requête de connexion
 * @param sd Socket de dialogue du client
 * @param request  Requête MPP reçue
 * @param response Réponse MPP à envoyer
 */
void connect_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);

/**
 * @fn void list_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * @brief Gère une requête pour lister les musiques
 * @param sd  Socket de dialogue du client
 * @param request Requête MPP reçue
 * @param response Réponse MPP à envoyer
 * @note Si l'utilisateur n'existe pas, une reponse BAD_REQUEST est envoyée
 * @warning La listes des musique doit être libérée après utilisation
 */
void list_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);

/**
 * @fn void add_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * @brief Gère une requête pour ajouter une musique
 * @param sd Socket du client
 * @param request requête MPP reçue
 * @param response reponse MPP à envoyer
 * @note Un fichier est crée pour chaque musique dans le dossier de l'utilisateur
 * @warning Si l'utilisateur n'existe pas, une reponse BAD_REQUEST est envoyée
 */
void add_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);

/**
 * @fn void get_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * @brief Gère une requête pour récupérer une musique
 * @param sd  Socket de dialogue du client
 * @param request Requête MPP reçue
 * @param response Réponse MPP à envoyer
 */
void get_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);

/**
 * @fn void delete_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * @brief Gère une requête pour supprimer une musique
 * @param sd  Socket de dialogue du client
 * @param request Requête MPP reçue
 * @param response Réponse MPP à envoyer
 */
void delete_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);

/**
 * @fn void free_request(mpp_request_t *request);
 * @brief Libère une requête MPP
 * @param request Requête MPP
 */
void free_request(mpp_request_t *request);

/**
 * @fn void free_response(mpp_response_t *response);
 * @brief Libère une réponse MPP
 * @param response Réponse MPP
 */
void free_response(mpp_response_t *response);

/**
 * @fn void create_user_directories();
 * @brief Crée un répertoire pour les utilisateurs
 * @note Si le fichier user.db n'existe pas, il est créé
 */
void create_user_directories();

#endif