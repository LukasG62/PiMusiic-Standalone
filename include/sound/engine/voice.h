/**
 * @file voice.h
 * @brief Gestion d'une voix
 * @author Lukas Grando
 * @version 1.0
 */
#ifndef VOICE_H
#define VOICE_H

#include <stddef.h>
#include <stdbool.h>

#include "sound/engine/instrument.h"
#include "sound/osc/osc.h"
#include "sound/env/adsr.h"
#include "sound/fx/fx.h"

/**
 * @struct voice_t
 * @brief Représente un canal de lecture audio indépendant
 */
typedef struct {
    instrument_t *instrument; /*!< L'instrument dont la voix est issue */
    osc_t *oscillator; /*!< Oscillateur générant le signal de base */
    adsr_t *envelope; /*!< Enveloppe ADSR */
    fx_t *effects[MAX_FX_PER_INSTRUMENT]; /*!< Effets audio appliqués à la voix */
    double currentFreq; /*!< Fréquence actuelle de la voix */
    bool isActive; /*!< Indicateur de l'état de la voix */
} voice_t;

/**
 * @brief Crée une voix en lisant l'instrument à utiliser
 * @param instrument L'instrument à jouer
 * @return voice_t* La voix prête à être déclenchée
 */
voice_t *voice_create(instrument_t *instrument);

/**
 * @brief Fonction de nettoyage de la voix
 * @param voice La voix à détruire
 */
void voice_destroy(voice_t *voice);

/**
 * @brief Déclenche la note
 * @param voice La voix à déclencher
 * @param freq La fréquence cible
 */
void voice_note_on(voice_t *voice, double freq);

/**
 * @brief Relâche la note
 * @param voice La voix à relâcher
 */
void voice_note_off(voice_t *voice);

/**
 * @brief Demande à la voix de générer son signal et d'appliquer ses effets
 * @param voice La voix
 * @param sampleRate Le taux d'échantillonnage global
 * @param buffer Le buffer de destination à remplir et traiter
 * @param size Le nombre d'échantillons à traiter
 */
void voice_process(voice_t *voice, double sampleRate, double *buffer, size_t size);

/**
 * @brief Vérifie si la voix a totalement terminé son cycle
 * @param voice La voix
 * @return true si la voix est terminée et peut être nettoyée, false sinon
 */
bool voice_is_done(voice_t *voice);

#endif // VOICE_H