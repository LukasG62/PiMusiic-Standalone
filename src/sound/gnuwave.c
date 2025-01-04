/**
 * @file gnuwave.c
 * @brief Implementation des fonctions pour visualiser les signaux audio et spectres en utilisant GNUPlot
 * @version 1.0
 */

#include "sound/gnuwave.h"

/**
 * @fn int plot_wave(char *filename, double *wave, size_t waveSize,unsigned int sampleRate, char *title) 
 * @brief créer un fichier de script GNUPlot pour visualiser un signal audio
 * @param filename Nom du fichier de sortie
 * @param wave Signal audio
 * @param waveSize Taille du signal audio
 * @param sampleRate Taux d'échantillonnage
 * @param title Titre du graphique
 * @return int 0 si tout s'est bien passé, -1 sinon
 */
int plot_wave(char *filename, double *wave, size_t waveSize,unsigned int sampleRate, char *title) {
	FILE *file = fopen(filename, "w");
	if (file == NULL) {
		ERROR("plot_wave: Could not create file %s\n", filename);
		return -1;
	}
	
	fprintf(file, "set terminal png\n"); // Format de sortie
	if(title != NULL) {
		fprintf(file, "set title \"%s\"\n", title); // Titre du graphique
	}
	fprintf(file, "set xlabel \"Time\"\n"); // Axe des abscisses
	fprintf(file, "set ylabel \"Amplitude\"\n"); // Axe des ordonnées
	fprintf(file, "set xrange [0:%f]\n", waveSize * 1.0 / sampleRate); // Limiter l'axe des abscisses
	fprintf(file, "set yrange [-1:1]\n"); // Limiter l'axe des ordonnées
	fprintf(file, "unset key\n"); // Pas de légende
	fprintf(file, "plot '-' with lines\n"); // Tracer le signal audio

	// Écrire les données du signal audio
	for (size_t i = 0; i < waveSize; i++) {
		fprintf(file, "%f %f\n", i * 1.0 / sampleRate, wave[i]);
	}
	fprintf(file, "e\n"); // Fin des données

	fclose(file);
	return 0;
}
