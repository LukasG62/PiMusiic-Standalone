/**
 * @file osc_resampler.h
 * @brief Oscillateur de sample utilisant la méthode de resampling linéaire
 * @author Lukas Grando
 * @version 1.0
 */

#ifndef OSC_RESAMPLER_H
#define OSC_RESAMPLER_H

#include "sound/osc/osc.h"
#include "sound/io/io_pmsd.h"

// todo: serait intéressant de pouvoir configurer quel type de données le sample utilise (8, 16, 24, 32 bits) et d'adapter la lecture et la normalisation en conséquence

#define NORMALIZATION_FACTOR_16B 32768.0 /*!< Facteur de normalisation pour les échantillons 16 bits */

typedef struct {
	short *sampleData; /*!< Pointeur vers les données audio du sample */
	size_t sampleLength; /*!< Longueur du sample en échantillons */
	double baseFrequency; /*!< Fréquence de base du sample (en Hz) */

	size_t loopStart; /*!< Index de début de la boucle (en échantillons) */
	size_t loopEnd; /*!< Index de fin de la boucle (en échantillons) */

	bool loopEnabled; /*!< Indique si la boucle est activée */
} osc_resampler_config_t;

/**
 * @brief Crée un oscillateur de sample avec resampling linéaire
 * @param config Configuration de l'oscillateur (doit être pré-remplie)
 * @return osc_t* l'oscillateur créé
 * @warning L'oscillateur doit être détruit avec sa méthode destroy pour libérer la mémoire
 */
osc_t *osc_resampler_create(osc_resampler_config_t *config);

/**
 * @brief Remplit la configuration de l'oscillateur à partir d'un fichier pmsd chargé
 * @param cfg Pointeur vers la configuration à remplir
 * @param pmsd Pointeur vers l'objet PMSD déjà chargé en mémoire
 */
void osc_resampler_config_from_pmsd(osc_resampler_config_t *cfg, const io_pmsd_t *pmsd);

#endif // OSC_RESAMPLER_H