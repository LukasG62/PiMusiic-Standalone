/**
 * @file fx_compressor.h
 * @brief Effet Compresseur avec suiveur d'enveloppe
 * @author Lukas Grando
 * @version 2.0
 */
#ifndef FX_COMPRESSOR_H
#define FX_COMPRESSOR_H

#include "sound/fx/fx.h"

/**
 * @struct fx_compressor_config_t
 * @brief Configuration live du compresseur
 */
typedef struct {
	double sampleRate; /*!< Taux d'échantillonnage */
    double threshold; /*!< Seuil de compression*/
    double ratio; /*!< Ratio de réduction */
    double attackTime; /*!< Temps d'attaque en secondes */
    double releaseTime; /*!< Temps de relâchement en secondes */
    double makeupGain; /*!< Gain de compensation appliqué à la fin */
} fx_compressor_config_t;

/**
 * @brief Crée une instance du compresseur
 * @param config pointeur vers la configuration
 * @param sampleRate Le taux d'échantillonnage
 * @return fx_t* L'effet créé
 */
fx_t *fx_compressor_create(fx_compressor_config_t *config);

#endif // FX_COMPRESSOR_H