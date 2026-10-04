/**
 * @file io_pmsd.h
 * @brief Lecteur de fichier pmsd (PiMusiicSampleData)
 * @author Lukas Grando
 * @version 3.0
 */
#ifndef IO_PMSD_H
#define IO_PMSD_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define PMSD_MAGIC "PMSD"
#define PMSD_VERSION 1

#define PMSD_METHOD_RESAMPLER     0
#define PMSD_METHOD_PHASE_VOCODER 1 


/**
 * @struct io_pmsd_header_t
 * @brief En-tête du fichier
 */
typedef struct {
    char magic[4];
    uint32_t version;
    uint32_t dataOffset;

    uint16_t channels;
    uint32_t sampleRate;
    uint32_t sampleCount;
    
    uint16_t methodType;
} __attribute__((packed)) io_pmsd_header_t;

/**
 * @struct io_pmsd_meta_resampler_t
 */
typedef struct {
    float baseFreq;
    uint8_t loopEnabled;
    uint32_t loopStart;
    uint32_t loopEnd;
} __attribute__((packed)) io_pmsd_meta_resampler_t;

/**
 * @struct io_pmsd_meta_vocoder_t
 */
typedef struct {
    float baseFreq;
    uint16_t fftSize;
    uint16_t hopSize;
} __attribute__((packed)) io_pmsd_meta_vocoder_t;


/**
 * @union io_pmsd_meta_t
 * @brief Contient les métadonnées selon la méthode choisie
 */
typedef union {
    io_pmsd_meta_resampler_t resampler;
    io_pmsd_meta_vocoder_t vocoder;
} io_pmsd_meta_t;

/**
 * @struct io_pmsd_t
 */
typedef struct {
    io_pmsd_header_t header; /*!< Les infos générales */
    io_pmsd_meta_t meta; /*!< Les information spécifique au type */
    int16_t *audioData; /*!< les données pcm */
} io_pmsd_t;


/**
 * @brief Charge un fichier PMSD complet en mémoire
 * @param filepath Chemin du fichier
 * @param out_pmsd Pointeur vers la structure RAM à remplir
 * @return true si succès
 */
bool io_pmsd_load(const char *filepath, io_pmsd_t *out_pmsd);

/**
 * @brief Libère la mémoire audio allouée par io_pmsd_load
 * @param pmsd L'objet PMSD à nettoyer
 */
void io_pmsd_destroy(io_pmsd_t *pmsd);

#endif // IO_PMSD_H