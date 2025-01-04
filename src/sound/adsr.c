/**
 * @file adsr.c
 * @brief Définition des fonctions pour l'enveloppe ADSR
 * @version 1.0
 */

#include "sound/adsr.h"

/**
 * @fn adsr_t create_adsr(float attack, float decay, float sustain, float release)
 * @brief Créé une enveloppe ADSR avec les param donnés
 * @param attack Temps de montée du son
 * @param decay Temps de descente du son
 * @param sustain Temps de maintien du son
 * @param release Temps de fin du son
 * @return adsr_t
 */
adsr_t *create_adsr(float attack, float decay, float sustain, float release) {
	adsr_t *adsr = malloc(sizeof(adsr_t));
	CHECK_ALLOC(adsr);

	adsr->attack = attack;
	adsr->decay = decay;
	adsr->sustain = sustain;
	adsr->release = release;
	return adsr;
}

/**
 * @fn void set_attack(adsr_t *adsr, float attack)
 * @brief Modifie le temps de montée du son
 * @param adsr Enveloppe ADSR
 * @param attack Temps de montée du son
 * @return void
 */
void set_attack(adsr_t *adsr, float attack) {
	adsr->attack = attack;
}

/**
 * @fn void set_decay(adsr_t *adsr, float decay)
 * @brief Modifie le temps de descente du son
 * @param adsr Enveloppe ADSR
 * @param decay Temps de descente du son
 * @return void
 */
void set_decay(adsr_t *adsr, float decay) {
	adsr->decay = decay;
}

/**
 * @fn void set_sustain(adsr_t *adsr, float sustain)
 * @brief Modifie le temps de maintien du son
 * @param adsr Enveloppe ADSR
 * @param sustain Temps de maintien du son
 * @return void
 */
void set_sustain(adsr_t *adsr, float sustain) {
	adsr->sustain = sustain;
}

/**
 * @fn void set_release(adsr_t *adsr, float release)
 * @brief Modifie le temps de fin du son
 * @param adsr Enveloppe ADSR
 * @param release Temps de fin du son
 * @return void
 */
void set_release(adsr_t *adsr, float release) {
	adsr->release = release;
}

/**
 * float adsr_amplitude(adsr_t *adsr, float time, float duration)
 * @brief Calcule le facteur d'amplification du son en fonction du temps
 * @param adsr Enveloppe ADSR
 * @param time Temps actuel
 * @param duration Durée totale de la note
 * @return float
 */
double adsr_amplitude(adsr_t *adsr, double time, double duration) {
	double attackTime = adsr->attack * duration;
	double decayTime = adsr->decay * duration;
	double sustainTime = adsr->sustain * duration;
	double releaseTime = adsr->release * duration;

	// Attack: L'amplitude augmente linéairement jusqu'à 1
	if (time < attackTime) {
		return time / attackTime;
	}

	// Decay: L'amplitude diminue linéairement jusqu'à sustain
	if (time < attackTime + decayTime) {
		return 1 - (1 - adsr->sustain) * (time - attackTime) / decayTime;
	}

	// Sustain: L'amplitude reste constante
	if (time < attackTime + decayTime + sustainTime) {
		float decayEndFactor = 1 - (1 - adsr->sustain) * (decayTime) / decayTime;
		return decayEndFactor; // on reste à la dernier valeur de decay
	}

	// Release: L'amplitude diminue linéairement jusqu'à 0
	if (time < attackTime + decayTime + sustainTime + releaseTime) {
		return adsr->sustain - adsr->sustain * (time - (attackTime + decayTime + sustainTime)) / releaseTime;
	}

	return 0.0; // Fin du son : Ne devrait pas être atteint normalement
}
