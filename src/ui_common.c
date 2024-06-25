/**
 * @file ui_common.c
 * @brief Fichier source pour les fonctions communes à l'interface graphique
 * @version 1.0
 * @author Lukas Grando
 */

#include "ui_common.h"

/**********************************************************************************************************************/
/*                                           Public Fonction Definitions                                              */
/**********************************************************************************************************************/

/**
 * \fn void create_custom_colors(int color_id, const char *color)
 * \brief Création de couleurs personnalisées pour ncurses en passant par des codes hexadécimaux
 * \param color_id L'identifiant de la couleur
 * \param color La couleur en hexadécimal
 * \return La couleur personnalisée
 */
int create_custom_colors(int color_id, const char *color) {
    int r, g, b;
    sscanf(color, "#%02x%02x%02x", &r, &g, &b);
    // transformation des valeurs hexadécimales en valeurs de 0 à 1000
    r = r * 1000 / 255;
    g = g * 1000 / 255;
    b = b * 1000 / 255;
    // création de la couleur
    init_color(color_id, r, g, b);
    return color_id;      
}

/**
 * \fn void init_ncurses()
 * \brief Initialisation de ncurses et de la fenêtre
 * \details Cette fonction s'occupe de l'initialisation de ncurses, des couleurs, de la fenêtre et des bordures
 */
void init_ncurses() {
    // Initialisation de la fenêtre
    initscr(); // Initialisation de ncurses
    start_color(); // Activation des couleurs
    init_window(stdscr); // Initialisation de la fenêtre principale
}

/**
 * @fn init_window(WINDOW *win)
 * @brief Initialisation de la fenêtre principale
 * @param win La fenêtre à initialiser
 */
void init_window(WINDOW *win) {
    keypad(win, TRUE); // activation des touches spéciales
    noecho(); // Désactivation de l'affichage des caractères saisis
    cbreak(); // Désactivation du buffering de ligne
    curs_set(0); // Désactivation du curseur
}

/**
 * \fn void exit_ncurses()
 * \brief Fin de ncurses
 * \details Cette fonction s'occupe de la fin de ncurses
 */
void exit_ncurses() {
    // Fin de la fenêtre
    endwin(); // Fin de ncurses
}

/**
 * @fn choices_t show_date(time_t *timestamp, char *chaine)
 * @brief Affiche la date en format lisible
 * @param timestamp Le timestamp de la date
 * @param chaine La chaine qui va contenir la date
 */
void show_date(time_t timestamp, char *chaine) {
    struct tm *timeinfo;
    timeinfo = localtime(&timestamp);
    strftime(chaine, 20, "%d/%m/%Y %H:%M:%S", timeinfo);
}