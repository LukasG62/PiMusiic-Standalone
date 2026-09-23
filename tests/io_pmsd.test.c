/**
 * @file io_pmsd.test.c
 * @brief Tests unitaires pour le lecteur de format PMSD
 */
#include "unity.h"
#include "sound/io/io_pmsd.h"

#include <stdio.h>
#include <string.h>

static const char *test_filepath = "test_temp.pmsd";
static const int16_t dummy_pcm[4] = { 1024, -2048, 4096, -8192 };
static io_pmsd_t pmsd;

void setUp(void) {
    memset(&pmsd, 0, sizeof(io_pmsd_t));
}

void tearDown(void) {
    io_pmsd_destroy(&pmsd);
    remove(test_filepath);
}

/**
 * @brief Fonction utilitaire pour générer un fichier PMSD de test
 */
static void forge_pmsd_file(uint16_t methodType) {
    FILE *f = fopen(test_filepath, "wb");
    TEST_ASSERT_NOT_NULL(f);

    io_pmsd_header_t hdr;
    memset(&hdr, 0, sizeof(io_pmsd_header_t));
    memcpy(hdr.magic, PMSD_MAGIC, 4);
    hdr.version = PMSD_VERSION;
    hdr.sampleCount = 4;
    hdr.methodType = methodType;

    size_t meta_size = (methodType == PMSD_METHOD_RESAMPLER) ? 
                       sizeof(io_pmsd_meta_resampler_t) : sizeof(io_pmsd_meta_vocoder_t);
    hdr.dataOffset = sizeof(io_pmsd_header_t) + meta_size;

    fwrite(&hdr, sizeof(io_pmsd_header_t), 1, f);

    if (methodType == PMSD_METHOD_RESAMPLER) {
        io_pmsd_meta_resampler_t meta = { .baseFreq = 440.0f, .loopEnabled = 1 };
        fwrite(&meta, sizeof(io_pmsd_meta_resampler_t), 1, f);
    } else {
        io_pmsd_meta_vocoder_t meta = { .baseFreq = 220.0f, .fftSize = 1024 };
        fwrite(&meta, sizeof(io_pmsd_meta_vocoder_t), 1, f);
    }

    fwrite(dummy_pcm, sizeof(int16_t), 4, f);
    fclose(f);
}

/**
 * @brief Vérifie le chargement d'un type Resampler
 */
void test_io_pmsd_load_resampler(void) {
    forge_pmsd_file(PMSD_METHOD_RESAMPLER);
    
    bool result = io_pmsd_load(test_filepath, &pmsd);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_UINT32(4, pmsd.header.sampleCount);
    TEST_ASSERT_EQUAL_UINT16(PMSD_METHOD_RESAMPLER, pmsd.header.methodType);
    TEST_ASSERT_EQUAL_FLOAT(440.0f, pmsd.meta.resampler.baseFreq);
    TEST_ASSERT_EQUAL_UINT8(1, pmsd.meta.resampler.loopEnabled);
    TEST_ASSERT_NOT_NULL(pmsd.audioData);
    TEST_ASSERT_EQUAL_INT16(dummy_pcm[0], pmsd.audioData[0]);
}

/**
 * @brief Vérifie le chargement d'un type Phase Vocoder
 */
void test_io_pmsd_load_vocoder(void) {
    forge_pmsd_file(PMSD_METHOD_PHASE_VOCODER);
    
    bool result = io_pmsd_load(test_filepath, &pmsd);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_UINT16(PMSD_METHOD_PHASE_VOCODER, pmsd.header.methodType);
    TEST_ASSERT_EQUAL_FLOAT(220.0f, pmsd.meta.vocoder.baseFreq);
    TEST_ASSERT_EQUAL_UINT16(1024, pmsd.meta.vocoder.fftSize);
    TEST_ASSERT_NOT_NULL(pmsd.audioData);
}

/**
 * @brief Vérifie le comportement face à un fichier inexistant
 */
void test_io_pmsd_load_not_found(void) {
    bool result = io_pmsd_load("ghost.pmsd", &pmsd);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_NULL(pmsd.audioData);
}

/**
 * @brief Vérifie le comportement face à un fichier avec de mauvais arguments
 */
void test_io_pmsd_load_invalid_args(void) {
    bool result = io_pmsd_load(NULL, &pmsd);
    TEST_ASSERT_FALSE(result);
    
    forge_pmsd_file(PMSD_METHOD_RESAMPLER);
    result = io_pmsd_load(test_filepath, NULL);
    TEST_ASSERT_FALSE(result);
}

/**
 * @brief Vérifie le comportement face à un fichier corrompu (mauvais magic)
 */
void test_io_pmsd_load_bad_magic(void) {
    FILE *f = fopen(test_filepath, "wb");
    fprintf(f, "FAKE_DATA");
    fclose(f);

    bool result = io_pmsd_load(test_filepath, &pmsd);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_NULL(pmsd.audioData);
}

/**
 * @brief Vérifie la destruction
 */
void test_io_pmsd_null_safety(void) {
    io_pmsd_destroy(NULL);
    io_pmsd_t empty_pmsd = {0};
    io_pmsd_destroy(&empty_pmsd);
    TEST_ASSERT_TRUE(true);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_io_pmsd_load_resampler);
    RUN_TEST(test_io_pmsd_load_vocoder);
    RUN_TEST(test_io_pmsd_load_not_found);
    RUN_TEST(test_io_pmsd_load_invalid_args);
    RUN_TEST(test_io_pmsd_load_bad_magic);
    RUN_TEST(test_io_pmsd_null_safety);

    return UNITY_END();
}