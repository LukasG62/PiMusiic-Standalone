/**
 * @file sound_defs.h
 * @brief Définitions globales et énumérations du moteur audio
 * @author Lukas Grando
 * @version 1.0
 */
#ifndef SOUND_DEFS_H
#define SOUND_DEFS_H

#define MAX_FX_PER_INSTRUMENT 4

/**
 * @enum osc_type_t
 * @brief Types d'oscillateurs disponibles dans le moteur
 */
typedef enum {
    OSC_TYPE_SIN,
    OSC_TYPE_SQUARE,
    OSC_TYPE_SAWTOOTH,
    OSC_TYPE_TRIANGLE,
    OSC_TYPE_NOISE,
    OSC_TYPE_RESAMPLER
} osc_type_t;

/**
 * @enum fx_type_t
 * @brief Types d'effets disponibles dans le moteur
 */
typedef enum {
    FX_TYPE_NONE = 0,
    FX_TYPE_FUZZ,
    FX_TYPE_COMPRESSOR
} fx_type_t;

#endif // SOUND_DEFS_H