/**
 * @file io_pmsd.h
 * @brief Lecteur de fichier pmsd (PiMusiicSampleData)
 * @author Lukas Grando
 * @version 3.0
 */
#include "sound/io/io_pmsd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Charge un fichier PMSD complet en mémoire
 * @param filepath Chemin du fichier
 * @param out_pmsd Pointeur vers la structure RAM à remplir
 * @return true si succès
 */
bool io_pmsd_load(const char *filepath, io_pmsd_t *out_pmsd) {
    if (!filepath || !out_pmsd) return false;

    memset(out_pmsd, 0, sizeof(io_pmsd_t));
    FILE *f = fopen(filepath, "rb");
    
	if(!f) return false;
    if(fread(&out_pmsd->header, sizeof(io_pmsd_header_t), 1, f) != 1) goto error;
    if(strncmp(out_pmsd->header.magic, PMSD_MAGIC, 4) != 0 || out_pmsd->header.version != PMSD_VERSION) goto error;

    switch(out_pmsd->header.methodType) {
        case PMSD_METHOD_RESAMPLER:
            if (fread(&out_pmsd->meta.resampler, sizeof(io_pmsd_meta_resampler_t), 1, f) != 1) goto error;
            break;
        case PMSD_METHOD_PHASE_VOCODER:
            if (fread(&out_pmsd->meta.vocoder, sizeof(io_pmsd_meta_vocoder_t), 1, f) != 1) goto error;
            break;
        default: goto error;
    }

    out_pmsd->audioData = malloc(out_pmsd->header.sampleCount * sizeof(int16_t));
    if (!out_pmsd->audioData) goto error;

    fseek(f, out_pmsd->header.dataOffset, SEEK_SET);
    if (fread(out_pmsd->audioData, sizeof(int16_t), out_pmsd->header.sampleCount, f) != out_pmsd->header.sampleCount) goto error;

    fclose(f);
    return true;

	error:
		if (out_pmsd->audioData) {
			free(out_pmsd->audioData);
			out_pmsd->audioData = NULL;
		}
		if (f) fclose(f);

    return false;
}

/**
 * @brief Libère la mémoire audio allouée par io_pmsd_load
 * @param pmsd L'objet PMSD à nettoyer
 */
void io_pmsd_destroy(io_pmsd_t *pmsd) {
    if (pmsd && pmsd->audioData) {
        free(pmsd->audioData);
        pmsd->audioData = NULL;
    }
}