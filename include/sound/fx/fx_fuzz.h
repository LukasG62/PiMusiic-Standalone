/**
 * @file fx_fuzz.h
 * @brief Générateur d'effet fuzz
 * @author Lukas Grando
 * @version 1.0
 */
#ifndef FX_FUZZ_H
#define FX_FUZZ_H

#include "sound/fx/fx.h"

/**
 * @struct fx_fuzz_config_t
 * @brief Configuration de l'effet Fuzz
 */
typedef struct {
    double drive; /*!< Le taux d'écrasement du signal. Doit être >= 1.0 */
} fx_fuzz_config_t;

/**
 * @brief Crée un générateur d'effet fuzz
 * @param config structure de configuration du fuzz
 * @return fx_t* le générateur d'effet créé
 * @warning Le générateur doit être détruit à avec la méthode destroy
 */
fx_t *fx_fuzz_create(fx_fuzz_config_t *config);

#endif