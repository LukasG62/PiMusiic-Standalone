/**
 * @file instrument.c
 * @brief Définition du preset d'Instrument
 * @author Lukas Grando
 * @version 2.0
 */
#include "sound/engine/instrument.h"

/**
 * @brief Alloue un instrument vide avec des paramètres par défaut
 * @return instrument_t* L'instrument initialisé
 */
instrument_t *instrument_create_default(void) {
    instrument_t *inst = malloc(sizeof(instrument_t));
    if (!inst) return NULL;

    strncpy(inst->name, "Default Sine", INSTRUMENT_NAME_LENGTH);
    inst->name[INSTRUMENT_NAME_LENGTH] = '\0';

    strncpy(inst->shortName, "SINE", INSTRUMENT_SHORT_NAME);
    inst->shortName[INSTRUMENT_SHORT_NAME] = '\0'; 

    inst->oscType = OSC_TYPE_SIN;
    
    inst->adsrConfig.enabled = true;
    inst->adsrConfig.attackTime = 0.01;
    inst->adsrConfig.decayTime = 0.1;
    inst->adsrConfig.sustainLevel = 0.7;
    inst->adsrConfig.releaseTime = 0.2;

    inst->fxCount = 0;
    for (int i = 0; i < MAX_FX_PER_INSTRUMENT; i++) {
        inst->fxTypes[i] = FX_TYPE_NONE;
    }

    return inst;
}

/**
 * @brief Libère la mémoire du preset d'instrument
 */
void instrument_destroy(instrument_t *inst) {
	if(inst) free(inst);
}
