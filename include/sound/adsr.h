/**
 * @file adsr.h
 * @brief Définition des fonctions pour l'enveloppe ADSR
 * @version 1.0
 */

#ifndef ADSR_H
#define ADSR_H

#include "common.h"

/**
 * @struct adsr_t
 * @brief Structure des paramètres ADSR
 * @details les parametre de l'enveloppe ADSR (Attack, Decay, Sustain, Release) sont des valeurs en pourcentage de la durée totale de la note
 * Attack : temps de montée du son
 * Decay : temps de descente du son
 * Sustain : temps de maintien du son
 * Release : temps de fin du son
 */
typedef struct {
	double attack; /*!< Temps de montée du son */
	double decay; /*!< Temps de descente du son */
	double sustain; /*!< Temps de maintien du son */
	double release; /*!< Temps de fin du son */
} adsr_t;


/**
 * @fn adsr_t create_adsr(float attack, float decay, float sustain, float release)
 * @brief Créé une enveloppe ADSR avec les param donnés
 * @param attack Temps de montée du son
 * @param decay Temps de descente du son
 * @param sustain Temps de maintien du son
 * @param release Temps de fin du son
 * @return adsr_t
 */
adsr_t *create_adsr(float attack, float decay, float sustain, float release);

/**
 * @fn void set_attack(adsr_t *adsr, float attack)
 * @brief Modifie le temps de montée du son
 * @param adsr Enveloppe ADSR
 * @param attack Temps de montée du son
 * @return void
 */
void set_attack(adsr_t *adsr, float attack);

/**
 * @fn void set_decay(adsr_t *adsr, float decay)
 * @brief Modifie le temps de descente du son
 * @param adsr Enveloppe ADSR
 * @param decay Temps de descente du son
 * @return void
 */
void set_decay(adsr_t *adsr, float decay);

/**
 * @fn void set_sustain(adsr_t *adsr, float sustain)
 * @brief Modifie le temps de maintien du son
 * @param adsr Enveloppe ADSR
 * @param sustain Temps de maintien du son
 * @return void
 */
void set_sustain(adsr_t *adsr, float sustain);

/**
 * @fn void set_release(adsr_t *adsr, float release)
 * @brief Modifie le temps de fin du son
 * @param adsr Enveloppe ADSR
 * @param release Temps de fin du son
 * @return void
 */
void set_release(adsr_t *adsr, float release);

/**
 * float adsr_amplitude(adsr_t *adsr, float time, float duration)
 * @brief Calcule le facteur d'amplification du son en fonction du temps
 * @param adsr Enveloppe ADSR
 * @param time Temps actuel
 * @param duration Durée totale de la note
 * @return float
 */
double adsr_amplitude(adsr_t *adsr, double time, double duration);



#endif // ADSR_H