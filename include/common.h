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
#include <stdlib.h>
#include <string.h>

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
#else
    #define UNUSED(x) (void)(x) 
    #define UNIMPLEMENTED(string) 
    #define DEBUG_PRINT(...)
#endif

#define ERROR(...) fprintf(stderr, __VA_ARGS__) //!< Macro pour afficher un message d'erreur
#define CHECK_ALLOC(ptr) if (!ptr) { ERROR("Memory allocation failed\n"); exit(EXIT_FAILURE); } //!< Macro pour vérifier si l'allocation dynamique a réussi

#endif // COMMON_H