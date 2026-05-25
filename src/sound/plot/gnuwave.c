/**
 * @file gnuwave.h
 * @brief Fonctions pour visualiser les signaux audio et spectres en utilisant GNUPlot
 * @version 2.0
 */

#include "sound/plot/gnuwave.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief Initialise un nouveau graphique
 * @param title Le titre de la fenêtre
 * @param type Le type d'analyse (Temps ou Spectre)
 * @return gnuplot_t* Le pointeur vers le contexte
 */
gnuplot_t *gnuplot_create(const char *title, gnuplot_type_t type) {
    gnuplot_t *plot = malloc(sizeof(gnuplot_t));
    CHECK_ALLOC(plot);
    
    plot->type = type;
    plot->numCurves = 0;
    if(title) plot->title = strdup(title);
    else plot->title = NULL;

    return plot;
}

/**
 * @brief Ajoute un buffer audio à superposer sur le graphique
 * @param plot Le contexte
 * @param data Le buffer contenant les doubles
 * @param size La taille du buffer
 * @param sampleRate Le taux d'échantillonnage
 * @param legend Le nom de la courbe (ex: "Enveloppe ADSR")
 * @return true si ajouté avec succès, false si la limite MAX_CURVES est atteinte
 */
bool gnuplot_add_curve(gnuplot_t *plot, const double *data, size_t size, double sampleRate, const char *legend) {
    if(!plot || plot->numCurves >= GNUPLOT_MAX_CURVES || !data) return false;

    gnuplot_curve_t *curve = &plot->curves[plot->numCurves];
    curve->data = data;
    curve->size = size;
    curve->sampleRate = sampleRate;
    
    if(legend) curve->legend = strdup(legend);
    else curve->legend = NULL;

    plot->numCurves++;

    return true;
}

/**
 * @brief Fonction interne qui écrit tout dans le flux
 * @param plot Le graphique à écrire
 * @param stream le flux à remplir
 * @return true en cas de succès, faux sinon
 */
bool gnuplot_write_stream(gnuplot_t *plot, FILE *stream) {
    if(!plot || !stream || plot->numCurves == 0) return false;

    const char *plotStyle = "lines lw 1.5";

    if(plot->title) fprintf(stream, "set title \"%s\"\n", plot->title);
    fprintf(stream, "set grid\n");

    switch(plot->type) {
        case GNUPLOT_TYPE_TIME:
            fprintf(stream, "set xlabel \"Temps (s)\"\n");
            fprintf(stream, "set ylabel \"Amplitude\"\n");
            fprintf(stream, "set xrange [0:*]\n");
            fprintf(stream, "set yrange [-1.1:1.1]\n");
            plotStyle = "lines lw 1.5";
            break;
        case GNUPLOT_TYPE_SPECTRUM:
            fprintf(stream, "set xlabel \"Frequence (Hz)\"\n");
            fprintf(stream, "set ylabel \"Magnitude\"\n");
            fprintf(stream, "set xrange [0:*]\n");
            fprintf(stream, "set yrange [0:*]\n");
            plotStyle = "impulses lw 2";
            break;

        default:
            break;
    }

    fprintf(stream, "plot ");
    for(int i = 0; i < plot->numCurves; i++) {
        fprintf(stream, "'-' with %s", plotStyle);

        if(plot->curves[i].legend) fprintf(stream, " title \"%s\"", plot->curves[i].legend);
        else fprintf(stream, " title \"\"");

        if(i < plot->numCurves - 1) {
            fprintf(stream, ", ");
        }
    }
    fprintf(stream, "\n");

    for(int i = 0; i < plot->numCurves; i++) {
        gnuplot_curve_t *c = &plot->curves[i];
        
        for (size_t j = 0; j < c->size; j++) {
            double x_val = 0.0;
            switch(plot->type) {
                case GNUPLOT_TYPE_TIME: x_val = (double)j / c->sampleRate; break;
                case GNUPLOT_TYPE_SPECTRUM: x_val = (double)j * (c->sampleRate / c->size); break;
                default: x_val = 0.0; break;
            }
            fprintf(stream, "%f %f\n", x_val, c->data[j]);
        }
        fprintf(stream, "e\n");
    }
    return true;
}

/**
 * @brief Affiche le graphique en direct dans une fenêtre
 * @param plot le graphique
 */
bool gnuplot_render_live(gnuplot_t *plot) {
    FILE *pipe = popen("gnuplot -persistent", "w");
    if(!pipe) return false;

    bool ret = gnuplot_write_stream(plot, pipe);
    pclose(pipe);

    return ret;
}

/**
 * @brief Exporte le graphique dans un fichier PNG
 */
bool gnuplot_render_file(gnuplot_t *plot, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return false;
    fprintf(file, "set terminal png size 1000,600\n");
    fprintf(file, "set output \"%s\"\n", filename);
    bool ret = gnuplot_write_stream(plot, file);
    fclose(file);
    return ret;
}

/**
 * @brief Libère la mémoire du contexte
 */
void gnuplot_destroy(gnuplot_t *plot) {
    if(!plot) return;

    if(plot->title) free(plot->title);

    for(int i = 0; i < plot->numCurves; i++) {
        if(plot->curves[i].legend) free(plot->curves[i].legend);
    }

    free(plot);
}