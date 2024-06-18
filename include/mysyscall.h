/**
 * @file mysyscall.h
 * @author Lukas Grando
 * @brief Interface de la couche d'abstraction pour les appels systèmes
 * @version 1.0
 * @author Lukas Grando
 */
#ifndef __MY_SYSCALL_H__
#define __MY_SYSCALL_H__

/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <sched.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

/* ------------------------------------------------------------------------ */
/*              C O N S T A N T E S     S Y M B O L I Q U E S               */
/* ------------------------------------------------------------------------ */
#define TEMPO_1MS 1000000L
/* ------------------------------------------------------------------------ */
/*              D É F I N I T I O N S   D E   T Y P E S                     */
/* ------------------------------------------------------------------------ */
typedef void *(*pf_t)(void *);
/* ------------------------------------------------------------------------ */
/*                      M A C R O - F O N C T I O N S                       */
/* ------------------------------------------------------------------------ */
#define CHECK_T(status, msg)          \
    if ((status) != 0)                  \
    {                                 \
        fprintf(stderr, "%s\n", msg); \
    }

#define CHECK(sts, msg) \
    if ((sts) == -1)    \
    {                   \
        perror(msg);    \
        exit(-1);       \
    }
// pour un sem_open()
#define CHECK_IFFAILED(sts, msg) \
    if (SEM_FAILED == (sts))     \
    {                            \
        perror(msg);             \
        exit(EXIT_FAILURE);      \
    }

#define CHECK_SEM_OPEN(sem, name) \
    if (sem == SEM_FAILED)        \
{                             \
        perror(name);             \
        exit(EXIT_FAILURE);       \
    }

// pour un fopen()
#define CHECK_FILE(sts, msg) \
    if (NULL == (sts))       \
    {                        \
        perror(msg);         \
        exit(EXIT_FAILURE);  \
    }

#define DISPLAY_PID() printf("[%d] ", getpid()) // Affiche le PID du processus

/* ------------------------------------------------------------------------ */
/*            P R O T O T Y P E S    D E    F O N C T I O N S               */
/* ------------------------------------------------------------------------ */

/**
 * @fn void init_mask(char *msg);
 * @brief Fonction qui initialise un masque de signaux
 * @details La fonction initialise un masque de signaux ou tous les signaux sont bloqués
 * @return void
 */
void init_mask();

/**
 * @fn void add_signal_to_mask(int signalNumber);
 * @brief Fonction qui ajoute un signal au masque de signaux
 * @details La fonction ajoute un signal au masque de signaux
 * @param signalNumber Le numéro du signal à ajouter
 * @return void
 */
void add_signal_to_mask(int signalNumber);

/**
 * @fn void remove_signal_from_mask(int signalNumber);
 * @brief Fonction qui ajoute un signal au masque de signaux
 * @details La fonction ajoute un signal au masque de signaux
 * @param signalNumber Le numéro du signal à ajouter
 * @param handler Le handler à utiliser pour le signal
 * @param flags Les flags à utiliser pour le signal
 */
void install_signal_handler(int signalNumber, void (*handler)(int), int flags);

/**
 * @fn void random_usleep(int min, int max);
 * @brief Fonction qui fait une temporisation aléatoire entre 2 valeurs
 * @details Les valeurs sont en microsecondes
 * @param min La valeur minimale de la temporisation
 * @param max La valeur maximale de la temporisation
 * @return void
 */
void random_usleep(int min, int max);

/* SECTION : THREADS */

/**
 * @fn void display_pthread_attr(pthread_attr_t *attr, char *prefix);
 * @brief Fonction qui affiche les attributs d'un thread
 * @param attr Les attributs du thread
 * @param prefix Le préfixe à afficher
 * @details La fonction provient du man init
 * @param attr Les attributs du thread
 */
void display_pthread_attr(pthread_attr_t *attr, char *prefix);

/**
 * @fn pthread_t create_thread(pthread_t *thread, void *(*start_routine)(void *), long thread_number);
 * @brief Fonction qui créer un thread avec un numéro
 * @param thread Le thread à créer
 * @param start_routine La fonction à exécuter
 * @param thread_number Le numéro du thread
 * @param state L'état du thread (detached ou joinable)
 * @return Le thread créé
 * @details La fonction créer un thread
 */
pthread_t create_thread(pthread_t *thread, void *(*start_routine)(void *), long thread_number, int state);

/* SECTION : SEM */

/**
 * @fn sem_t *create_named_sem(char *name, int value);
 * @param name Nom de la sémaphore
 * @param value Valeur initiale de la sémaphore
 * @return La sémaphore créée
 * @brief Fonction qui créer une sémaphore nommé
 */
sem_t *create_named_sem(char *name, int value);

/**
 * @fn sem_t *open_named_sem(char *name);
 * @brief Fonction qui ouvre une sémaphore nommé
 * @param name Nom de la sémaphore
 * @return La sémaphore ouverte
 * @details la sémpahore doit être créé avant
 */
sem_t *open_named_sem(char *name);

/**
 * @fn void close_named_sem(sem_t *sem);
 * @param sem Sémaphore à fermer
 * @brief Fonction qui ferme une sémaphore nommé
 */
void close_named_sem(sem_t *sem);

/**
 * @fn void unlink_named_sem(char *name);
 * @param name Nom de la sémaphore
 * @brief Fonction qui supprime une sémaphore nommé
 */
void unlink_named_sem(char *name);

/**
 * \fn sem_t *create_sem(int value);
 * \brief Fonction de creation d'une semaphore anonyme
 * \param value Valeur initiale de la semaphore
 * \return La semaphore creee
 */
sem_t *create_sem(int value);

/**
 * \fn void destroy_sem(sem_t *sem);
 * \brief Fonction de destruction d'une semaphore
 * \param sem Semaphore a detruire
 */
void destroy_sem(sem_t *sem);

/**
 * \fn void wait_sem(sem_t *sem);
 * \brief Fonction d'attente sur une semaphore
 * \param sem Semaphore sur laquelle attendre
 */
void wait_sem(sem_t *sem);

/**
 * \fn void post_sem(sem_t *sem);
 * \brief Fonction de post sur une semaphore
 * \param sem Semaphore sur laquelle poster
 */
void post_sem(sem_t *sem);

#endif
