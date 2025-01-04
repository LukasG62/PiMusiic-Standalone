/**
 * @file gnuwave.h
 * @brief Definition des fonctions pour visualiser les signaux audio et spectres en utilisant GNUPlot
 * @version 1.0
 */

#ifndef GNUWAVE_H
#define GNUWAVE_H

#include "common.h"

/**
 * @fn int plot_wave(char *filename, double *wave, size_t waveSize, unsigned int sampleRate, char *title);
 * @brief créer un fichier de script GNUPlot pour visualiser un signal audio
 * @param filename Nom du fichier de sortie
 * @param wave Signal audio
 * @param waveSize Taille du signal audio
 * @param sampleRate Taux d'échantillonnage
 * @param title Titre du graphique
 * @return int
 */
int plot_wave(char *filename, double *wave, size_t waveSize, unsigned int sampleRate, char *title);

#endif // GNUWAVE_H