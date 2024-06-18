/**
 * @file mpp.c
 * @brief Definition des fonctions pour le serveur MPP
 * @version 1.0
 * @author Lukas Grando
 */
#include "mpp.h"


/**********************************************************************************************************************/
/*                                           Private functions                                                        */
/**********************************************************************************************************************/

/**
 * @fn void write_music(music_t *music, FILE *file);
 * @param music  La musique à écrire
 * @param file   Le fichier dans lequel écrire la musique
 */
void write_music(music_t *music, FILE *file);

/**
 * @fn void read_music(music_t *music, FILE *file);
 * @brief Lit une musique depuis un fichier 
 * @param music La musique à remplir
 * @param file Le fichier depuis lequou peuvenel lire la musique
 */
void read_music(music_t *music, FILE *file);

/**
 * @fn void write_list_music(musicId_list_t *list, FILE *file);
 * @brief Ecrit une liste d'identifiants de musiques dans un fichier
 * @param list Liste d'identifiants de musiques
 * @param file Le fichier dans lequel écrire la liste
 */
void write_list_music(musicId_list_t *list, FILE *file);
/**
 * @fn void read_list_music(musicId_list_t *list, FILE *file);
 * @brief Lit une liste d'identifiants de musiques depuis un fichier
 * @param list La liste d'identifiants de musiques
 * @param file Le fichier depuis lequel lire la liste
 */
void read_list_music(musicId_list_t *list, FILE *file);

/**
 * @fn serialize_music(music_t *music, buffer_t buffer);
 * @brief Sérialise une musique
 * @param music La musique à sérialiser
 * @param buffer Le buffer dans lequel sérialiser la musique
 * @note La musique est sérialisée de la manière suivante :
 * <date> <bpm>
 * <line> <noteid> <octave> <instrument> <time>
 * ...
 * P
 * <line> <noteid> <octave> <instrument> <time>
 * ...
 * P
 * <line> <noteid> <octave> <instrument> <time>
 * P
 * @warning La musique doit être initialisée avant d'appeler cette fonction
 */
void serialize_music(music_t *music, buffer_t buffer);

/**
 * @fn deserialize_music(char *token, music_t *music, char *saveptr);
 * @brief Désérialise une musique contenue dans un buffer
 * @param token La position dans le buffer ou se trouve la musique sérialisée
 * @param music La musique désérialisée
 * @warning La musique doit être initialisée avant d'appeler cette fonction
 */
void deserialize_music(char *token, music_t *music);


/**********************************************************************************************************************/
/*                                           Public  functions                                                        */
/**********************************************************************************************************************/


/**
 * \fn mpp_request_t create_mpp_request(mpp_request_code_t code, char *rfidId, music_t *music, time_t musicId);
 * \brief Créer une requête MPP
 * \param code Le code de la requête
 * \param rfidId L'identifiant RFID
 * \param music La musique
 * \param musicId L'identifiant de la musique
 * \return mpp_request_t 
 */
mpp_request_t create_mpp_request(mpp_request_code_t code, char *rfidId, music_t *music, time_t musicId) {
    mpp_request_t request;
    request.code = code;
    strcpy(request.rfidId, rfidId);
    request.music = music;
    request.musicId = musicId;
    return request;
}

/**
 * @fn mpp_response_t create_mpp_response(mpp_response_code_t code, char *username, music_t *music, musicId_list_t *musicIds);
 * @brief Créer une réponse MPP
 * @param code Le code de la réponse
 * @param username Le nom d'utilisateur
 * @param music Le pointeur vers la musique
 * @param musicIds La liste des identifiants de musiques
 * @return mpp_response_t 
 */
mpp_response_t create_mpp_response(mpp_response_code_t code, char *username, music_t *music, musicId_list_t *musicIds) {
    mpp_response_t response;
    response.code = code;
    strcpy(response.username, username);
    response.music = music;
    response.musicIds = musicIds;
    return response;
}


/**
 * @fn print_mpp_request(mpp_request_t *request);
 * @brief Affiche une requête MPP
 * @param request La requête MPP
 */
void print_mpp_request(mpp_request_t *request) {
    char *code = code2str_request(request->code);
    printf("Request code: %s\n", code);
    printf("RFID: %s\n", request->rfidId);
    printf("Music ID: %ld\n", request->musicId);
    free(code);
}

/**
 * @fn print_mpp_response(mpp_response_t *response);
 * @brief Affiche une réponse MPP
 * @param response 
 * @see mpp_response_t
 */
void print_mpp_response(mpp_response_t *response) {
    int i; 
    char *code = code2str_response(response->code);
    printf("Response code: %s\n", code);
    printf("Username: %s\n", response->username);
    if(response->musicIds != NULL) {
        printf("Music IDs: ");
        for(i = 0; i < response->musicIds->size; i++) {
            printf("%ld ", response->musicIds->musicIds[i]);
        }
        printf("\n");
    }
    free(code);
}

/**
 * @fn serialize_music(music_t *music, buffer_t buffer);
 * @brief Sérialise une requête MPP
 * La requête est sérialisée de la manière suivante :
 * <code> <rfidId>
 * <music>
 * @param request 
 * @param buffer 
 */
void serialize_mpp_request(mpp_request_t *request, buffer_t buffer) {
    sprintf(buffer, "%d %s %ld\n", request->code, request->rfidId, request->musicId);
    if(request->music != NULL) serialize_music(request->music, buffer);  
}

/**
 * @fn deserialize_mpp_request(buffer_t buffer, mpp_request_t *request);
 * @brief Désérialise une requête MPP
 * @param buffer Le buffer ou se trouve la requête sérialisée
 * @param request La requête MPP à remplir
 * @see serialize_mpp_request
 */
void deserialize_mpp_request(buffer_t buffer, mpp_request_t *request) {
    // strtok n'est pas thread safe, on utilise strtok_r donc pour cela on doit déclarer un pointeur saveptr
    char *saveptr = NULL;
    char *token = NULL;
    char *bufferCp = malloc(strlen(buffer) + 1);
    strcpy(bufferCp, buffer);
    // lecture de la première ligne
    token = strtok_r(buffer, "\n", &saveptr);
    sscanf(token, "%d %s %ld", (int *) &request->code, request->rfidId, &request->musicId);

    // lecture des musiques si elles existent
    token = strtok_r(NULL, "\n", &saveptr);
    if(token != NULL) {
        // On trouve ou commence la musique dans la copie du buffer
        char *tokenCp = strstr(bufferCp, token);
        request->music = (music_t *)malloc(sizeof(music_t));
        init_music(request->music, 0);
        deserialize_music(tokenCp, request->music);
    }
    else {
        request->music = NULL;
    }
    free(bufferCp);
}

/**
 * @brief Sérialise une reponse MPP
 * La réponse est sérialisée de la manière suivante :
 * <code> <username>
 * <list_size> <musicId> <musicId> <musicId> ...
 * <music> (même format que pour la requête)
 * @param response La réponse MPP
 * @param buffer Le buffer dans lequel sérialiser la réponse
 */
void serialize_mpp_response(mpp_response_t *response, buffer_t buffer) {
    int i;
    sprintf(buffer, "%d %s\n", response->code, response->username);
    if(response->musicIds != NULL) {
        sprintf(buffer, "%s%d\n", buffer, response->musicIds->size);
        for(i = 0; i < response->musicIds->size; i++) {
            sprintf(buffer, "%s%ld\n", buffer, response->musicIds->musicIds[i]);
        }
        sprintf(buffer, "%s", buffer);
    } else {
        // permet de discriminer les cas ou la liste est vide 
        sprintf(buffer, "%s0\n", buffer);
    }

    if(response->music != NULL) serialize_music(response->music, buffer);
}

/**
 * @fn deserialize_mpp_response(buffer_t buffer, mpp_response_t *response);
 * @brief Désérialise une réponse MPP
 * @param buffer Le buffer ou se trouve la réponse sérialisée
 * @param response La réponse MPP à remplir
 */
void deserialize_mpp_response(buffer_t buffer, mpp_response_t *response) {
    // strtok n'est pas thread safe, on utilise strtok_r donc pour cela on doit déclarer un pointeur saveptr
    // TODO : robustesse !!
    char *saveptr = NULL;
    char *bufferCp = malloc(strlen(buffer) + 1);
    int i;
    strcpy(bufferCp, buffer);
    // lecture de la première ligne
    char *token = strtok_r(buffer, "\n", &saveptr);
    sscanf(token, "%d %s", (int *) &response->code, response->username);

    // Creation de la liste de musiques 
    token = strtok_r(NULL, "\n", &saveptr);
    if(token != NULL) {
        response->musicIds = (musicId_list_t *)malloc(sizeof(musicId_list_t));
        init_music_list(response->musicIds);
        int size;
        sscanf(token, "%d", &size);
        for(i = 0; i < size; i++) {
            token = strtok_r(NULL, "\n", &saveptr);
            time_t musicId;
            sscanf(token, "%ld", &musicId);
            add_music_id(response->musicIds, musicId);
        }
        // On lit la musique
        token = strtok_r(NULL, "\n", &saveptr);
        if(token != NULL) {
            // On trouve ou commence la musique dans la copie du buffer
            char *tokenCp = strstr(bufferCp, token);
            response->music = (music_t *)malloc(sizeof(music_t));
            init_music(response->music, 0);
            deserialize_music(tokenCp, response->music);
        } 
        else response->music = NULL;

    } else {
        // Si token est NULL, alors il n'y a pas de liste de musiques ni de musique
        response->musicIds = NULL;
        response->music = NULL;
    }
    free(bufferCp);
}

/**
 * @fn serialize_music(music_t *music, buffer_t buffer);
 * @brief Convertit un code de requête MPP en chaîne de caractères
 * @param code 
 * @return char* 
 */
char *code2str_request(mpp_request_code_t code) {
    char *str = (char *)malloc(50);
    switch (code) {
        case MPP_ADD_MUSIC:
            strcpy(str, "Ajouter une musique");
            break;
        case MPP_GET_MUSIC:
            strcpy(str, "Récupérer une musique");
            break;
        case MPP_LIST_MUSIC:
            strcpy(str, "Lister les musiques");
            break;

        case MPP_CONNECT:
            strcpy(str, "Connexion");
            break;
        
        default:
            strcpy(str, "Code inconnu");
            break;

    }
    return str;

}

/**
 * @fn code2str_response(mpp_response_code_t code);
 * @brief Convertit un code de réponse MPP en chaîne de caractères
 * @param code Le code de réponse MPP
 * @return char* La chaîne de caractères
 * @warning La chaine de caractères doit être libérée après utilisation
 * @see mpp_response_code_t
 */
char *code2str_response(mpp_response_code_t code) {
    char *str = (char *)malloc(50);
    switch (code) {
        case MPP_RESPONSE_OK:
            strcpy(str, "L'opération a réussi");
            break;
        case MPP_RESPONSE_NOK:
            strcpy(str, "L'opération a échoué");
            break;

        case MPP_RESPONSE_MUSIC_CREATED:
            strcpy(str, "La musique a été créée");
            break;

        case MPP_RESPONSE_BAD_REQUEST:
            strcpy(str, "La requête est mal formée");
            break;

        case MPP_RESPONSE_MUSIC_UPDATED:
            strcpy(str, "La musique a été mise à jour");
            break;
        
        case MPP_RESPONSE_NOT_FOUND:
            strcpy(str, "La musique n'a pas été trouvée");
            break;

        default:
            strcpy(str, "Code inconnu");
            break;
    }
    return str;

}

/**
 * @fn init_music(music_t *music, time_t date);
 * @brief initialise une liste d'identifiants de musiques
 * @param list Liste de musiques
 * @note la liste est dynamiquement allouée en utilisant add_music_id
 * @see add_music_id
 */
void init_music_list(musicId_list_t *list) {
    list->size = 0;
    list->musicIds = NULL;
}

/**
 * @fn add_music_id(musicId_list_t *list, int musicId);
 * @brief Ajoute un identifiant de musique à une liste de musiques
 * @param list Liste de musiques
 * @param musicId L'identifiant de la musique à ajouter
 * @note La liste doit être initialisée avant d'appeler cette fonction
 * @warning La liste est réallouée si nécessaire
 * @see REALLLOC_SIZE
 */
void add_music_id(musicId_list_t *list, int musicId) {
    if(list->size % REALLLOC_SIZE == 0) {
        list->musicIds = (time_t *)realloc(list->musicIds, (list->size + REALLLOC_SIZE) * sizeof(time_t));
    }
    list->musicIds[list->size] = musicId;
    list->size++;
}
/**
 * @fn remove_music_id(musicId_list_t *list, int index);
 * @brief Supprime un identifiant de musique d'une liste de musiques
 * @param list Liste de musiques
 * @param index L'indice de la musique à supprimer
 */
void remove_music_id(musicId_list_t *list, int index) {
    int i;
    for(i = index; i < list->size - 1; i++) {
        list->musicIds[i] = list->musicIds[i + 1];
    }
    list->size--;
}

/**
 * @fn free_music_list(musicId_list_t *list);
 * @brief Libère la mémoire allouée pour une liste de musiques
 * @param list Liste de musiques
 */
void free_music_list(musicId_list_t *list) {
    free(list->musicIds);
    list->musicIds = NULL;
    list->size = 0;
}

/**
 * @fn get_music_list_from_db(musicId_list_t *list, char *rfidId);
 * @brief Récupère la liste des musiques d'un utilisateur depuis la base de données
 * @param list La liste des musiques récupérée
 * @param rfidId L'identifiant RFID de l'utilisateur
 * @note Si l'utilisateur n'existe pas, la liste est vide
 */
void get_music_list_from_db(musicId_list_t *list, char *rfidId) {
    char filename[255];
    sprintf(filename, "%s/%s/%s/%s", MPP_DB_FOLDER, MPP_DB_MUSIC_FOLDER, rfidId, MPP_DB_MUSIC_FILE);
    FILE *file = fopen(filename, "rb");

    // Si le fichier n'existe pas, on initialise la liste
    if(file == NULL) {
        init_music_list(list);
        file = fopen(filename, "wb");
        write_list_music(list, file);
        fclose(file);
        return;
    }
    read_list_music(list, file);
    fclose(file);
}

/**
 * @fn add_music_to_db(music_t *music, char *rfidId);
 * @brief Ajoute une musique à la base de données
 * @param music La musique à ajouter
 * @param rfidId L'identifiant RFID de l'utilisateur
 * @return int 
 */
int add_music_to_db(music_t *music, char *rfidId) {
    char filename[255];
    sprintf(filename, "%s/%s/%s/%s", MPP_DB_FOLDER, MPP_DB_MUSIC_FOLDER, rfidId, MPP_DB_MUSIC_FILE);
    FILE *file = fopen(filename, "r+");
    if(file == NULL) {
        file = fopen(filename, "w+"); // On crée le fichier s'il n'existe pas
        if(file == NULL) return -1;
    }
    // On écrit l'identifiant de la musique dans le fichier
    musicId_list_t list;
    read_list_music(&list, file);
    int music_exists = search_music(&list, music->date.tv_sec);
    if (music_exists == -1) add_music_id(&list, music->date.tv_sec);

    fseek(file, 0, SEEK_SET); 
    write_list_music(&list, file);
    fclose(file);

    // On écrit la musique dans un fichier séparé
    sprintf(filename, "%s/%s/%s/%ld.mipi", MPP_DB_FOLDER, MPP_DB_MUSIC_FOLDER, rfidId, music->date.tv_sec);
    file = fopen(filename, "w");

    if(file == NULL) return -1;

    write_music(music, file);
    fclose(file);
    return 0;
}

/**
 * @fn get_music_from_db(music_t *music, time_t musicId, char *rfidId);
 * @brief Récupère une musique depuis la base de données
 * @param music La musique récupérée
 * @param musicId L'identifiant de la musique à récupérer
 * @param rfidId L'identifiant RFID de l'utilisateur
 */
void get_music_from_db(music_t *music, time_t musicId, char *rfidId) {
    char filename[255];
    sprintf(filename, "%s/%s/%s/%ld.mipi", MPP_DB_FOLDER, MPP_DB_MUSIC_FOLDER, rfidId, musicId);
    FILE *file = fopen(filename, "rb");
    if(file == NULL) {
        return;
    }
    read_music(music, file);
    fclose(file);

}

/**
 * @fn delete_music_from_db(time_t musicId, char *rfidId);
 * @brief Supprime une musique de la base de données
 * @param musicId L'identifiant de la musique à supprimer
 * @param rfidId L'identifiant RFID de l'utilisateur
 */
void delete_music_from_db(time_t musicId, char *rfidId) {
    char filename[255];
    musicId_list_t list;

    sprintf(filename, "%s/%s/%s/%ld.mipi", MPP_DB_FOLDER, MPP_DB_MUSIC_FOLDER, rfidId, musicId);
    remove(filename);
    sprintf(filename, "%s/%s/%s/%s", MPP_DB_FOLDER, MPP_DB_MUSIC_FOLDER, rfidId, MPP_DB_MUSIC_FILE);
    FILE *file = fopen(filename, "rb+");

    if(file == NULL) return; // Si le fichier n'existe pas, on ne fait rien
    read_list_music(&list, file);
    int index = search_music(&list, musicId);
    if(index == -1) return; // Si la musique n'existe pas, on ne fait rien
    remove_music_id(&list, index);
    fseek(file, 0, SEEK_SET);
    write_list_music(&list, file);
    fclose(file);
    free_music_list(&list);
}

/**
 * @fn search_music(musicId_list_t *list, time_t musicId);
 * @brief Fonction de recherche d'une musique dans une liste
 * @param list Liste d'identifiants de musiques
 * @param musicId L'identifiant de la musique à rechercher
 * @return int L'indice de la musique dans la liste, -1 si la musique n'est pas trouvée
 * @note La liste doit être initialisée avant d'appeler cette fonction
 */
int search_music(musicId_list_t *list, time_t musicId) {
    int i;
    for(i = 0; i < list->size; i++) {
        if(list->musicIds[i] == musicId) {
            return i;
        }
    }
    return -1;
}

/**
 * @fn get_username_from_db(char *username, char *rfidId);
 * @brief Récupère le nom d'utilisateur associé à un rfidId depuis la base de données
 * @param username Nom d'utilisateur récupéré
 * @param rfidId L'identifiant RFID de l'utilisateur
 * @note Si l'utilisateur n'existe pas, username est une chaîne vide
 * @warning La chaine username doit être allouée avant l'appel à cette fonction
 */
void get_username_from_db(char *username, char *rfidId) {
    char filename[255];  
    sprintf(filename, "%s/%s", MPP_DB_FOLDER, MPP_DB_USER_FILE);
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        username[0] = '\0';
        return;
    }
    char line[50];
    while(fgets(line, 50, file) != NULL) {
        char *saveptr;
        char *token = strtok_r(line, ":", &saveptr);
        if(strcmp(token, rfidId) == 0) {
            token = strtok_r(NULL, ":", &saveptr);
            strcpy(username, token);
            fclose(file);
            return;
        }
    }
}

/**
 * @fn connect_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * @brief Gère une requête de connexion
 * @param sd Socket de dialogue du client
 * @param request  Requête MPP reçue
 * @param response Réponse MPP à envoyer
 */
void connect_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response) {
    // On vérifie que l'utilisateur est présent dans user.db
    get_username_from_db(response->username, request->rfidId);
    if(*response->username == '\0') {
        CREATE_BAD_REQUEST(response);
        return;
    }
    response->code = MPP_RESPONSE_OK;
}

/**
 * @fn list_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * @brief Gère une requête pour lister les musiques
 * @param sd  Socket de dialogue du client
 * @param request Requête MPP reçue
 * @param response Réponse MPP à envoyer
 * @note Si l'utilisateur n'existe pas, une reponse BAD_REQUEST est envoyée
 * @warning La listes des musique doit être libérée après utilisation
 */
void list_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response) {
    // On vérifie que l'utilisateur est présent dans user.db
    get_username_from_db(response->username, request->rfidId);
    if(*response->username == '\0') {
        CREATE_BAD_REQUEST(response);
        return;
    }
    // On récupère la liste des musiques
    response->musicIds = (musicId_list_t *)malloc(sizeof(musicId_list_t));
    get_music_list_from_db(response->musicIds, request->rfidId);
    response->code = MPP_RESPONSE_OK;
}

/**
 * @fn add_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * @brief Gère une requête pour ajouter une musique
 * @param sd Socket du client
 * @param request requête MPP reçue
 * @param response reponse MPP à envoyer
 * @note Un fichier est crée pour chaque musique dans le dossier de l'utilisateur
 * @warning Si l'utilisateur n'existe pas, une reponse BAD_REQUEST est envoyée
 */
void add_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response) {
    // On vérifie que l'utilisateur est présent dans user.db
    get_username_from_db(response->username, request->rfidId);
    if(*response->username == '\0') {
        CREATE_BAD_REQUEST(response);
        return;
    }
    // On ajoute la musique à la base de données
    add_music_to_db(request->music, request->rfidId);
    response->code = MPP_RESPONSE_MUSIC_CREATED;
}

/**
 * @fn get_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * @brief Gère une requête pour récupérer une musique
 * @param sd  Socket de dialogue du client
 * @param request Requête MPP reçue
 * @param response Réponse MPP à envoyer
 */
void get_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response) {
    // On vérifie que l'utilisateur est présent dans user.db
    get_username_from_db(response->username, request->rfidId);
    if(*response->username == '\0') {
        CREATE_BAD_REQUEST(response);
        return;
    }
    // On récupère la musique de la base de données
    response->music = (music_t *)malloc(sizeof(music_t));
    get_music_from_db(response->music, request->musicId, request->rfidId);
    response->code = MPP_RESPONSE_OK;
}

/**
 * @fn delete_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * @brief Gère une requête pour supprimer une musique
 * @param sd  Socket de dialogue du client
 * @param request Requête MPP reçue
 * @param response Réponse MPP à envoyer
 */
void delete_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response) {
    // On vérifie que l'utilisateur est présent dans user.db
    get_username_from_db(response->username, request->rfidId);
    if(*response->username == '\0') {
        CREATE_BAD_REQUEST(response);
        return;
    }
    // On supprime la musique de la base de données
    delete_music_from_db(request->musicId, request->rfidId);
    response->code = MPP_RESPONSE_OK;
}

/**
 * @fn free_request(mpp_request_t *request);
 * @brief Libère une requête MPP
 * @param request Requête MPP
 */
void free_request(mpp_request_t *request) {
    if(request->music != NULL) free(request->music);
    free(request);
}

/**
 * @fn free_response(mpp_response_t *response);
 * @brief Libère une réponse MPP
 * @param response Réponse MPP
 */
void free_response(mpp_response_t *response) {
    if(response->music != NULL) free(response->music);
    if(response->musicIds != NULL) free_music_list(response->musicIds);
    free(response);
}

/**
 * @fn void create_user_directories();
 * @brief Crée un répertoire pour les utilisateurs
 * @note Si le fichier user.db n'existe pas, il est créé
 */
void create_user_directories() {
    char folder[255];
    // read user.db
    sprintf(folder, "%s/%s", MPP_DB_FOLDER, MPP_DB_USER_FILE);
    // On vérifie que le fichier existe sinon on le crée
    FILE *file = fopen(folder, "r");
    if(file == NULL) {
        file = fopen(folder, "w");
        fclose(file);
        return;
    }
    // Pour chaque ligne du fichier, on crée un répertoire
    char line[50];
    while(fgets(line, 50, file) != NULL) {
        char *saveptr;
        char *token = strtok_r(line, ":", &saveptr);
        sprintf(folder, "%s/%s/%s", MPP_DB_FOLDER, MPP_DB_MUSIC_FOLDER, token);
        mkdir(folder, 0777);
    }
    fclose(file);
}

/**********************************************************************************************************************/
/*                                           Private Fonction Definitions                                             */
/**********************************************************************************************************************/

/**
 * @fn serialize_music(music_t *music, buffer_t buffer);
 * @brief Sérialise une musique
 * @param music La musique à sérialiser
 * @param buffer Le buffer dans lequel sérialiser la musique
 * @note La musique est sérialisée de la manière suivante :
 * <date> <bpm>
 * <line> <noteid> <octave> <instrument> <time>
 * ...
 * P
 * <line> <noteid> <octave> <instrument> <time>
 * ...
 * P
 * <line> <noteid> <octave> <instrument> <time>
 * P
 * @warning La musique doit être initialisée avant d'appeler cette fonction
 */
void serialize_music(music_t *music, buffer_t buffer) {
    int i, j;
    sprintf(buffer, "%s%ld %d\n", buffer, music->date.tv_sec, music->bpm);
    // On parcourt chaque channel et on écrit seulement les notes non vides
    for(i = 0; i < MUSIC_MAX_CHANNELS; i++) {
        channel_t *channel = &music->channels[i];
        for(j = 0; j < channel->nbNotes; j++) {
            note_t *note = &channel->notes[j];
            sprintf(buffer, "%s%d %d %d %d %d\n", buffer, j, note->id, note->octave, note->instrument, note->time);
        }
        // On marque la fin du channel
        sprintf(buffer, "%sP\n", buffer);
    }
}

/**
 * @fn deserialize_music(char *token, music_t *music, char *saveptr);
 * @brief Désérialise une musique contenue dans un buffer
 * @param token La position dans le buffer ou se trouve la musique sérialisée
 * @param music La musique désérialisée
 * @warning La musique doit être initialisée avant d'appeler cette fonction
 */
void deserialize_music(char *token, music_t *music) {
    int channelCount = 0;
    char *line = NULL;
    char *saveptr = NULL;
    scale_t scale = init_scale();
    line = strtok_r(token, "\n", &saveptr);
    sscanf(line, "%ld %hd", &music->date.tv_sec, &music->bpm);

    while (line != NULL && channelCount < MUSIC_MAX_CHANNELS) {
        line = strtok_r(NULL, "\n", &saveptr);
        if (line != NULL) {
            int channelId = channelCount;
            channel_t *channel = &music->channels[channelId];
            while (line != NULL && *line != 'P') {
                int index = 0;
                // on récupère d'abord la ligne
                sscanf(line, "%d", &index);
                // on récupère les notes
                note_t *note = &channel->notes[index];
                sscanf(line, "%d %hd %hd %d %d", &index, &note->id, &note->octave, (int *)&note->instrument, (int *)&note->time);
                note->frequency = get_note_freq(note, &scale);
                update_channel_nbNotes(channel, index);
                line = strtok_r(NULL, "\n", &saveptr);
            }
            channelCount++;
        }
    }
}

/**
 * @fn void write_list_music(musicId_list_t *list, FILE *file);
 * @brief Ecrit une liste d'identifiants de musiques dans un fichier
 * @param list Liste d'identifiants de musiques
 * @param file Le fichier dans lequel écrire la liste
 */
void write_list_music(musicId_list_t *list, FILE *file) {
    int i;
    int a = fwrite(&list->size, sizeof(int), 1, file);
    for(i = 0; i < list->size; i++) {
        a += fwrite(&list->musicIds[i], sizeof(time_t), 1, file);
    }
}

/**
 * @fn void read_list_music(musicId_list_t *list, FILE *file);
 * @brief Lit une liste d'identifiants de musiques depuis un fichier
 * @param list La liste d'identifiants de musiques
 * @param file Le fichier depuis lequel lire la liste
 */
void read_list_music(musicId_list_t *list, FILE *file) {
    init_music_list(list);
    int size;
    time_t musicId = 0;
    fread(&size, sizeof(int), 1, file);
    if(size == 0) return;
    while(fread(&musicId, sizeof(time_t), 1, file) == 1) {
        add_music_id(list, musicId);
    }
}

/**
 * @fn void write_music(music_t *music, FILE *file);
 * @param music  La musique à écrire
 * @param file   Le fichier dans lequel écrire la musique
 */
void write_music(music_t *music, FILE *file) {
    // On change de stragégie pour l'écriture des musiques
    // On écrit la version sérialisée de la musique dans le fichier
    // Plus légère et plus modulaire (si la structure de la musique change, on pourra toujours lire les anciennes musiques)
    //fwrite(music, sizeof(music_t), 1, file);
    char *buffer = (char *) malloc(sizeof(buffer_t));
    serialize_music(music, buffer);
    fprintf(file, "%s", buffer);
    free(buffer);
}

/**
 * @fn void read_music(music_t *music, FILE *file);
 * @brief Lit une musique depuis un fichier 
 * @param music La musique à remplir
 * @param file Le fichier depuis lequel lire la musique
 */
void read_music(music_t *music, FILE *file) {
    // On change de stragégie pour la lecture des musiques
    // On lit la version sérialisée de la musique dans le fichier
    // Plus légère et plus modulaire (si la structure de la musique change, on pourra toujours lire les anciennes musiques)
    char *buffer = (char *) malloc(sizeof(buffer_t));
    fread(buffer, sizeof(buffer_t), 1, file);
    fprintf(stderr, "READ_MUSIC : %s\n", buffer);
    fflush(stderr);
    deserialize_music(buffer, music);
    free(buffer);
}

