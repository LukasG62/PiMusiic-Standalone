/**
 * @file pimusiic.c
 * @details Application principale
 * @version 1.0
 * @author Tomas Salvado Robalo & Lukas Grando
*/
#include "sound.h"
#include <pthread.h>

#include "uiManager.h"
#include "request.h"
#include "sound.h"
#include "mysyscall.h"

/**
 * \fn void clean_up()
 * \brief Fonction de nettoyage de l'application
 * \details Cette fonction permet de nettoyer l'application avant de quitter
 */
void clean_up() {
    // On arrête la bibliothèque graphique
    endwin();
    exit(EXIT_SUCCESS);
}

int main() {
    atexit(clean_up);
    
	
    char pseudo[20] = "";
    char rfid[20] = "";
    music_t music;
    init_music(&music, 120);
    choices_t choice = CHOICE_MAIN_MENU;
    // Initialisation de la bibliothèque graphique
    init_ncurses();

    while (choice != CHOICE_QUITAPP) {
        switch (choice) {
            case CHOICE_MAIN_MENU:
                choice = show_main_menu();
                break;
            case CHOICE_HELP:
                //choice = show_help();
                break;

            case CHOICE_CONECTION_MENU:
                if(*pseudo == '\0') choice = show_connection_menu(rfid, pseudo);
                else choice = show_list_music(rfid, &music);
                break;

            case CHOICE_CREATEMUSIC:
                choice = show_create_music_menu(&music, rfid);
                break;

            case CHOICE_MENU_LIST:
                choice = show_list_music(rfid, &music);
                break;

            case CHOICE_SEQUENCER:
                choice = show_sequencer(&music, rfid);
                break;

            case CHOICE_SAVENQUIT:
                choice = CHOICE_MAIN_MENU;
                break;
            
            default:
                choice = CHOICE_MAIN_MENU;
                break;
        }

        // On nettoie l'écran
        clear();
    }
    endwin();
  
    return 0;
}
