/**
 * \file uiManager.h
 * \details Contiens les fonctions de toutes les interfaces de l'application
 * \version 1.0
 * \author Lukas Grando
 */

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "ui_menu.h"
#include "ui_sequencer.h"
#include "note.h"
#include "mpp.h"
#include "request.h"
#include "mysyscall.h"
#include "sound.h"
#include <time.h>

/**
 * \fn choices_t show_main_menu()
 * \brief Affichage du menu principal
 * \details Cette fonction affiche le menu principal et gère la navigation dans le menu
 * \return Le choix de l'utilisateur
 * \see app_choices_t
 */
app_choices_t show_main_menu();

/**
 * @fn choices_t show_connection_menu(char *rfid, char *username)
 * @brief Affichage du menu de connexion et effectue la connexion
 * @param rfid Le rfid de l'utilisateur
 * @param username Le nom d'utilisateur de l'utilisateur
 * @return app_choices_t 
 * @note la fonction remplit les variables rfid et username
 */
app_choices_t show_connection_menu(char *rfid, char *username);

/**
 * @fn choices_t show_list_music(char *rfid, music_t *music)
 * @brief Affiche le menu de liste de musique et effectue les requetes pour fetch la musique
 * @param rfid Le rfid de l'utilisateur
 * @param music la musique choisie
 * @return app_choices_t 
 * @note la fonction remplit la musique
 * @warning la musique et le rfid doivent être alloués et initialisés
 */
app_choices_t show_list_music(char *rfid, music_t *music);

/**
 * @fn choices_t show_date(time_t *timestamp, char *chaine)
 * @brief Affiche la date en format lisible
 * @param timestamp Le timestamp de la date
 * @param chaine La chaine qui va contenir la date
 */
void show_date(time_t timestamp, char *chaine);

/**
 * @fn choices_t show_create_music_menu(music_t *music, char *rfid)
 * @brief La fonction qui affiche le menu de création de musique
 * @param music La musique à créer
 * @param rfid Le rfid de l'utilisateur
 * @return app_choices_t 
 * @note la fonction remplit la musique et le rfid
 * @warning la musique et le rfid doivent être alloués et initialisés
 */
app_choices_t show_create_music_menu(music_t *music, char *rfid);


/**
 * @fn void show_sequencer(music_t *music, char *connected)
 * @brief La fonction qui affiche le séquenceur
 * @param music La musique à afficher
 * @param connected Le rfid de l'utilisateur connecté
 * @return choices_t 
 * @note l'utilisateur est considéré comme connecté si connected est différent de \0
 * @warning la musique et connected doit être allouée et initialisée
 */
app_choices_t show_sequencer(music_t *music, char *connected);

/**
 * @fn void init_colors()
 * @brief Initialisation des couleurs
 */
void init_colors();

#endif // GRAPHIC_SEQ_H

