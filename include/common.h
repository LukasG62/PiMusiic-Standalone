/**
 * @file common.h
 * @brief Common header file
 * Ce fichier contient les macros de debug et d'erreur, ainsi que les couleurs de l'application.
 * @version 1.0
 * @author Lukas Grando
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

// TODO : 
// - Support de ncurses (message d'erreur, debug, etc.) 
// - Adapter les libinet pour utiliser les macros de debug et d'erreur 
// - Trouver un moyen de gérer les erreurs plus propre (renvoyer vers une vue d'erreur ? / code d'erreur ?)
// - Definir ici toutes les couleurs de l'application (fichier à part ?)

#ifdef COMMON_DEBUG //!< Si COMMON_DEBUG est défini, on active le mode debug
    #define UNUSED(x) (void)(x) //!< Macro pour supprimer les avertissements de variables inutilisées
    #define UNIMPLEMENTED(string) fprintf(stderr, "Unimplemented: %s\n", string) //!< Macro pour afficher le nom de la fonction non implémentée
    #define OBSOLETE(string) fprintf(stderr, "Obsolete: %s\n", string) //!< Macro pour afficher le nom de la fonction obsolète

    #define DEBUG_PRINT(...) fprintf(stderr, __VA_ARGS__) //!< Macro pour afficher un message de débug
    
    #define DEBUG_LOG_FILE "debug.log" //!< Nom du fichier de log
    // TODO : Ajouter la date et l'heure dans le fichier de log (utiliser strftime)
    // TODO : Faire en sorte que ça soit thread-safe mutex ou semaphore
    #define DEBUG_LOG(...) do { \
        FILE *log_file = fopen(DEBUG_LOG_FILE, "a"); /* Ouvre le fichier en mode append */ \
        if (log_file) { \
            fprintf(log_file, "[%ld] ", time(NULL)); /* Ajoute le timestamp */ \ 
            fprintf(log_file, __VA_ARGS__); /* Écrit dans le fichier de log */ \
            fprintf(log_file, "\n"); /* Ajoute un retour à la ligne */ \
            fclose(log_file); /* Ferme le fichier après écriture */ \
        } else { \
            DEBUG_PRINT("Failed to open log file\n"); /* Affiche un message d'erreur si l'ouverture a échoué */ \
        } \
    } while (0) //!< Macro pour écrire dans un fichier de log
    
#else
    #define UNUSED(x) (void)(x) //!< Macro pour supprimer les avertissements de variables inutilisées (inutile en mode release)
    #define UNIMPLEMENTED(string) //!< Macro pour afficher le nom de la fonction non implémentée (inutile en mode release)
    #define DEBUG_PRINT(...) //!< Macro pour afficher un message de débug (inutile en mode release)
    #define OBSOLETE(string) //!< Macro pour afficher le nom de la fonction obsolète (inutile en mode release)
    #define DEBUG_LOG(...) //!< Macro pour écrire dans un fichier de log (inutile en mode release)
    
#endif

#define ERROR(...) fprintf(stderr, __VA_ARGS__) //!< Macro pour afficher un message d'erreur
#define CHECK_ALLOC(ptr) if (!ptr) { ERROR("Memory allocation failed\n"); exit(EXIT_FAILURE); } //!< Macro pour vérifier si l'allocation dynamique a réussi
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0])) //!< Macro pour obtenir la taille d'un tableau

#define APP_USERNAME_MAX_LENGTH 15 //!< Longueur maximale du nom d'utilisateur
#define APP_PASSWORD_MAX_LENGTH 20 //!< Longueur maximale du mot de passe


#endif // COMMON_H