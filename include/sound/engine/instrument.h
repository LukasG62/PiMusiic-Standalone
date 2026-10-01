/**
 * @file instrument.h
 * @brief Définition du preset d'Instrument
 * @author Lukas Grando
 * @version 2.0
 */
#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "sound/sound_defs.h"
#include "sound/osc/osc_resampler.h"
#include "sound/env/adsr.h"
#include "sound/fx/fx_fuzz.h"
#include "sound/fx/fx_compressor.h"

#define INSTRUMENT_NAME_LENGTH 64
#define INSTRUMENT_SHORT_NAME 4 


/**
 * @union instrument_osc_config_t
 * @brief Conteneur générique pour la configuration d'un osc
 */
typedef union {
	osc_resampler_config_t resampler;
} instrument_osc_config_t;

/**
 * @union instrument_fx_config_t
 * @brief Conteneur générique pour la configuration d'un effet
 */
typedef union {
    fx_fuzz_config_t fuzz;
    fx_compressor_config_t compressor;
} instrument_fx_config_t;

/**
 * @struct instrument_t
 * @brief Représente un instrument complet
 */
typedef struct {
    char name[INSTRUMENT_NAME_LENGTH + 1]; /*!< Nom de l'instrument pour l'interface graphique */
    char shortName[INSTRUMENT_SHORT_NAME + 1]; /*!< Nom court de 4 lettre*/
    osc_type_t oscType; /*!< Type d'oscillateur utilisé par l'instrument */
    instrument_osc_config_t oscConfig; /*!< Configuration spécifique pour les oscillateurs nécessitant des paramètres */
    adsr_config_t adsrConfig; /*!< Configuration de l'enveloppe ADSR */
    
    int fxCount; /*!< Nombre d'effets actifs sur cet instrument */
    fx_type_t fxTypes[MAX_FX_PER_INSTRUMENT]; /*!< Types d'effets appliqués à l'instrument */
    instrument_fx_config_t fxConfigs[MAX_FX_PER_INSTRUMENT]; /*!< Configurations spécifiques pour les effets actifs */
} instrument_t;

/**
 * @brief Alloue un instrument vide avec des paramètres par défaut
 * @return instrument_t* L'instrument initialisé
 */
instrument_t *instrument_create_default(void);

/**
 * @brief Libère la mémoire du preset d'instrument
 */
void instrument_destroy(instrument_t *inst);

#endif // INSTRUMENT_H