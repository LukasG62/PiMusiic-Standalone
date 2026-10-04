/**
 * @file adsr.h
 * @brief Enveloppe ADSR
 * @author Lukas Grando
 * @version 1.0
 */
#ifndef ADSR_H
#define ADSR_H

#include "common.h"

#include <stddef.h>
#include <stdbool.h>

#define ADSR_DISABLE_VOLUME 1.0 /*!< Valeur de volume à appliquer au buffer lorsque l'enveloppe est désactivée */

/**
 * @enum adsr_phase_enum_t
 * @brief Les différents états possibles de l'enveloppe
 */
typedef enum {
    ADSR_PHASE_OFF = 0, /*!< Silence total, la note est morte */
    ADSR_PHASE_ATTACK,  /*!< Le volume monte */
    ADSR_PHASE_DECAY,   /*!< Le volume redescend vers le niveau de sustain */
    ADSR_PHASE_SUSTAIN, /*!< Le volume est maintenu */
    ADSR_PHASE_RELEASE  /*!< La touche est relâchée, le volume s'éteint */
} adsr_phase_enum_t;

/**
 * @struct adsr_config_t
 * @brief Configuration de l'enveloppe (modifiable par l'UI)
 */
typedef struct {
    bool enabled; /*!< Indique si l'enveloppe est activée */
    double attackTime; /*!< Temps d'attaque en secondes */
    double decayTime; /*!< Temps de descente en secondes */
    double sustainLevel; /*!< Niveau de maintien */
    double releaseTime;  /*!< Temps de relâchement en secondes */
} adsr_config_t;


/**
 * @struct adsr_t
 * @brief L'instance de l'ADSR liée à une Voix (Voice)
 */
typedef struct {
    adsr_phase_enum_t currentPhase; /*!< Phase actuelle de l'enveloppe */
    double currentVolume; /*!< Multiplicateur de volume actuel */
    adsr_config_t *config; /*!< Pointeur vers la configuration */
} adsr_t;

/**
 * @brief Crée une instance d'ADSR avec la configuration donnée
 * @param config La configuration de l'enveloppe (doit être pré-remplie
 * @return adsr_t* L'instance créée
 */
adsr_t *adsr_create(adsr_config_t *config);

/**
 * @brief Libère la mémoire allouée pour l'instance de l'ADSR
 * @param self L'instance de l'ADSR à détruire
 * @warning Ne libère pas la configuration pointée par self->config (gérée par ailleurs)
 */
void adsr_destroy(adsr_t *self);

/**
 * @brief Déclenche le début d'une note
 * @param self L'instance de l'ADSR
 */
void adsr_note_on(adsr_t *self);

/**
 * @brief Déclenche la fin d'une note
 * @param self L'instance de l'ADSR
 */
void adsr_note_off(adsr_t *self);

/**
 * @brief Applique l'enveloppe de volume au buffer audio in-place
 * @param self L'instance de l'ADSR
 * @param sampleRate Taux d'échantillonnage
 * @param buffer Le buffer généré par l'oscillateur
 * @param size Nombre de samples dans le buffer
 */
void adsr_process(adsr_t *self, double sampleRate, double *buffer, size_t size);

#endif // ADSR_H