/**
 * @file gnuwave.test.c
 * @brief Tests unitaires pour le visualiseur Gnuplot
 */

#include "unity.h"
#include "sound/plot/gnuwave.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/**
 * @brief Teste la création, l'ajout de courbes et la destruction
 */
void test_gnuplot_create_and_add_curves(void) {
    gnuplot_t *plot = gnuplot_create("Mon Graphique", GNUPLOT_TYPE_TIME);
    
    TEST_ASSERT_NOT_NULL(plot);
    TEST_ASSERT_EQUAL(GNUPLOT_TYPE_TIME, plot->type);
    TEST_ASSERT_EQUAL_STRING("Mon Graphique", plot->title);
    TEST_ASSERT_EQUAL(0, plot->numCurves);

    double data1[] = {0.1, 0.2, 0.3};
    bool res1 = gnuplot_add_curve(plot, data1, 3, 44100.0, "Courbe 1");
    
    TEST_ASSERT_TRUE(res1);
    TEST_ASSERT_EQUAL(1, plot->numCurves);
    TEST_ASSERT_EQUAL_STRING("Courbe 1", plot->curves[0].legend);
    TEST_ASSERT_EQUAL_PTR(data1, plot->curves[0].data);

    gnuplot_destroy(plot);
}

/**
 * @brief Teste la limite maximale de courbes
 */
void test_gnuplot_max_curves(void) {
    gnuplot_t *plot = gnuplot_create("Test Max", GNUPLOT_TYPE_TIME);
    double dummy_data[] = {0.0};

    for(int i = 0; i < GNUPLOT_MAX_CURVES; i++) {
        bool res = gnuplot_add_curve(plot, dummy_data, 1, 44100.0, "Legend");
        TEST_ASSERT_TRUE(res);
    }

    bool fail_res = gnuplot_add_curve(plot, dummy_data, 1, 44100.0, "Too Many");
    TEST_ASSERT_FALSE(fail_res);
    TEST_ASSERT_EQUAL(GNUPLOT_MAX_CURVES, plot->numCurves);

    gnuplot_destroy(plot);
}

/**
 * @brief Teste la génération du script Gnuplot pour un domaine Temporel
 */
void test_gnuplot_write_stream_time(void) {
    gnuplot_t *plot = gnuplot_create("Test Temporel", GNUPLOT_TYPE_TIME);
    
    double data[] = { 0.5, -0.5 };
    gnuplot_add_curve(plot, data, 2, 2.0, "Osc");

    FILE *tmp = tmpfile(); 
    TEST_ASSERT_NOT_NULL(tmp);

    bool res = gnuplot_write_stream(plot, tmp);
    TEST_ASSERT_TRUE(res);

    rewind(tmp);
    char buffer[1024] = {0};
    fread(buffer, 1, sizeof(buffer) - 1, tmp);
    fclose(tmp);

    TEST_ASSERT_NOT_NULL(strstr(buffer, "set title \"Test Temporel\""));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "set xlabel \"Temps (s)\""));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "plot '-' with lines lw 1.5 title \"Osc\""));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "0.000000 0.500000"));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "0.500000 -0.500000"));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "e\n"));

    gnuplot_destroy(plot);
}

/**
 * @brief Teste la génération du script Gnuplot pour un domaine Spectral
 */
void test_gnuplot_write_stream_spectrum(void) {
    gnuplot_t *plot = gnuplot_create("Test Spectral", GNUPLOT_TYPE_SPECTRUM);
    
    double data[] = { 10.0, 0.0, 5.0, 0.0 };
    gnuplot_add_curve(plot, data, 4, 4000.0, "FFT");

    FILE *tmp = tmpfile(); 
    gnuplot_write_stream(plot, tmp);

    rewind(tmp);
    char buffer[1024] = {0};
    fread(buffer, 1, sizeof(buffer) - 1, tmp);
    fclose(tmp);

    TEST_ASSERT_NOT_NULL(strstr(buffer, "set xlabel \"Frequence (Hz)\""));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "plot '-' with impulses lw 2 title \"FFT\""));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "0.000000 10.000000"));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "1000.000000 0.000000"));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "2000.000000 5.000000"));

    gnuplot_destroy(plot);
}

/**
 * @brief Vérifie les protections (paramètres invalides)
 */
void test_gnuplot_invalid_params(void) {
    gnuplot_t *plot = gnuplot_create(NULL, GNUPLOT_TYPE_TIME);
    
    TEST_ASSERT_FALSE(gnuplot_add_curve(plot, NULL, 10, 44100.0, "test"));
    TEST_ASSERT_FALSE(gnuplot_write_stream(plot, NULL));
    
    FILE *tmp = tmpfile();
    TEST_ASSERT_FALSE(gnuplot_write_stream(plot, tmp));
    fclose(tmp);

    gnuplot_destroy(plot);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_gnuplot_create_and_add_curves);
    RUN_TEST(test_gnuplot_max_curves);
    RUN_TEST(test_gnuplot_write_stream_time);
    RUN_TEST(test_gnuplot_write_stream_spectrum);
    RUN_TEST(test_gnuplot_invalid_params);

    return UNITY_END();
}