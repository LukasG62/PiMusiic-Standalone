/**
 * @file gnuwave.h
 * @brief Fonctions pour visualiser les signaux audio et spectres en utilisant GNUPlot
 * @version 2.0
 */
#ifndef GNUWAVE_H
#define GNUWAVE_H

#include "common.h"

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

// todo: allocation dynamique
#define GNUPLOT_MAX_CURVES 8 /*!< Nombre de courbe affichable en même temps */

/**
 * @enum gnuplot_type_t
 * @brief Le type de graphique à générer
 */
typedef enum {
    GNUPLOT_TYPE_TIME, /*!< Domaine temporel (Amplitude en fonction du temps) */
    GNUPLOT_TYPE_SPECTRUM /*!< Domaine fréquentiel (Magnitude en fonction de la fréquence) */
} gnuplot_type_t;

/**
 * @struct gnuplot_curve_t
 * @brief Représente une courbe individuelle sur le graphique
 */
typedef struct {
    const double *data; /*!< Pointeur vers les données à tracer */
    size_t size; /*!< Taille du buffer de données */
    double sampleRate; /*!< Taux d'échantillonnage */
    char *legend; /*!< Légende de la courbe */
} gnuplot_curve_t;

/**
 * @struct gnuplot_t
 * @brief Contexte du graphique contenant toutes les courbes
 */
typedef struct {
    char *title; /*!< Titre du graphique */
    gnuplot_type_t type; /*!< Type de graphique */
    
    gnuplot_curve_t curves[GNUPLOT_MAX_CURVES]; /*!< Tableau des courbes à afficher */
    int numCurves; /*!< Nombre de courbes actuellement ajoutées */
} gnuplot_t;

/**
 * @brief Initialise un nouveau graphique
 * @param title Le titre de la fenêtre
 * @param type Le type d'analyse (Temps ou Spectre)
 * @return gnuplot_t* Le pointeur vers le contexte
 */
gnuplot_t *gnuplot_create(const char *title, gnuplot_type_t type);

/**
 * @brief Ajoute un buffer audio à superposer sur le graphique
 * @param plot Le contexte
 * @param data Le buffer contenant les doubles
 * @param size La taille du buffer
 * @param sampleRate Le taux d'échantillonnage
 * @param legend Le nom de la courbe
 * @return true si ajouté avec succès, false si la limite MAX_CURVES est atteinte
 */
bool gnuplot_add_curve(gnuplot_t *plot, const double *data, size_t size, double sampleRate, const char *legend);

/**
 * @brief Fonction interne qui écrit tout dans le flux
 * @param plot Le graphique à écrire
 * @param stream le flux à remplir
 * @return true en cas de succès, faux sinon
 */
bool gnuplot_write_stream(gnuplot_t *plot, FILE *stream);

/**
 * @brief Affiche le graphique en direct dans une fenêtre
 */
bool gnuplot_render_live(gnuplot_t *plot);

/**
 * @brief Exporte le graphique dans un fichier PNG
 */
bool gnuplot_render_file(gnuplot_t *plot, const char *filename);

/**
 * @brief Libère la mémoire du contexte
 */
void gnuplot_destroy(gnuplot_t *plot);

#endif // GNUWAVE_H