/**
 * \file uiManager.c
 * \details Contiens les fonctions de toutes les interfaces de l'application
 * \note Dedans il y a aussi des fonctions helpers pour l'affichage (à déplacer dans un autre fichier ?)
 * \author Lukas Grando
 * \version 1.0
 */
#include "ui_manager.h"

/**********************************************************************************************************************/
/*                                           Private functions                                                        */
/**********************************************************************************************************************/

/**
 * @fn void show_request_error(const char *message)
 * @brief Affichage d'une erreur de requête
 * @details Cette fonction affiche un message d'erreur de requête
 * @param message Le message d'erreur
 */
void show_request_error(const char *message);

/**
 * \fn choice_t create_menu(const char *choices[], int n_choices, int highlight, choices_t choices)
 * \brief Création d'un menu ncurses
 * \details Cette fonction crée un menu ncurses avec choix paginés
 * \param title Le titre du menu
 * \param text Le texte du menu (optionnel)
 * \param choices[] Les choix du menu
 * \param nbChoices Le nombre de choix
 * \param highlight La couleur du choix sélectionné
 * \param choices Valeur de retour de chaque choix
 * 
*/
app_choices_t create_menu(const char *title, const char *text, char **choices, int nbChoices, int highlight, app_choices_t *choices_return);


/**********************************************************************************************************************/
/*                                           Public Fonction Definitions                                              */
/**********************************************************************************************************************/


/**
 * \fn app_choices_t show_main_menu()
 * \brief Affichage du menu principal
 * \details Cette fonction affiche le menu principal et gère la navigation dans le menu
 * \return Le choix de l'utilisateur
 * \see app_choices_t
 */
app_choices_t show_main_menu() {
    // On définit le titre du menu
    char title[] = "Pimusic Application";
    // On définit le texte du menu
    char text[] = "Welcome to Pimusic, please choose an option :";
    // On définit les choix du menu
    char *choices[] = {
        "1. Connect to retrieve music",
        "2. Create music",
        "3. Quit"
    };
    // On récupère le nombre de choix
    int n_choices = sizeof(choices) / sizeof(char *);
    // Les valeurs de retour de chaque choix
    app_choices_t choices_return[] = {
        CHOICE_CONECTION_MENU,
        CHOICE_CREATEMUSIC,
        CHOICE_QUITAPP,
    };

    // On affiche le menu
    return create_menu(title, text, choices, n_choices, 0, choices_return);

}

/**
 * @fn app_choices_t show_connection_menu(char *rfid, char *username)
 * @brief Affichage du menu de connexion et effectue la connexion
 * @param rfid Le rfid de l'utilisateur
 * @param username Le nom d'utilisateur de l'utilisateur
 * @return app_choices_t 
 * @note la fonction remplit les variables rfid et username
 */
app_choices_t show_connection_menu(char *rfid, char *username) {
    UNIMPLEMENTED("show_connection_menu");
    // Affichage du menu
    //init_menu("Connection", "Please enter your login information :", 0);
    // On fait une requête de connexion
    //mpp_response_t response = client_request_handler(MPP_CONNECT, rfid, NULL, -1);
    //wgetch(stdscr);
    return CHOICE_CONECTION_MENU;
}

/**
 * @fn app_choices_t show_list_music(char *rfid, music_t *music)
 * @brief Affiche le menu de liste de musique et effectue les requetes pour fetch la musique
 * @param rfid Le rfid de l'utilisateur
 * @param music la musique choisie
 * @return app_choices_t 
 * @note la fonction remplit la musique
 * @warning la musique et le rfid doivent être alloués et initialisés
 */
app_choices_t show_list_music(char *rfid, music_t *music) {
    mpp_response_t response;
    musicId_list_t *musicIds = NULL;
    int current = 0;
    int start = 0;
    int c;
    // init_menu("Music list", "Please select a music :", 1);
    if(*rfid == '\0') {
        clear();
        attron(COLOR_PAIR(COLOR_PAIR_MENU_WARNING) | A_BOLD);
        mvprintw(3, 4, "%s", "You're not connected, you can't retrieve music !");
        mvprintw(4, 4, "%s", "Press any key to return to the main menu");
        attroff(COLOR_PAIR(COLOR_PAIR_MENU_WARNING) | A_BOLD);
        return CHOICE_MAIN_MENU;
    }

    // On fait une requête pour récupérer la liste des musiques
    response = client_request_handler(MPP_LIST_MUSIC, rfid, NULL, -1);
    if(response.code >= MPP_RESPONSE_BAD_REQUEST) {
        show_request_error("Error while retrieving music list !");
        return CHOICE_MAIN_MENU;
    }
    
    if(response.musicIds->size == 0) {
        show_request_error("No music found !");
        return CHOICE_MAIN_MENU;
    }
    musicIds = response.musicIds;
    int i;
    // On affiche la liste des musiques
    char **choices = malloc(musicIds->size * sizeof(char *));
    for(i = 0; i < musicIds->size; i++) {
        choices[i] = malloc(80 * sizeof(char));
        char dates[30] = "";
        show_date(musicIds->musicIds[i], dates);
        sprintf(choices[i], "%d. %s", i, dates);
    }

    while(1) {
        // On affiche les choix avec celui selectionné en reverse
        //mvprintw(0,0, "%d %d %d", current, start, musicIds->size);
        for(i = start; i < (start + MENU_MAX_ITEMS) && (i < musicIds->size); i++) {
            if(i == current) {
                attron(COLOR_PAIR(COLOR_PAIR_MENU) | A_REVERSE);
                mvprintw(3 + i - start, 4, "%-30s", choices[i]);
            } else {
                attron(COLOR_PAIR(COLOR_PAIR_MENU));
                mvprintw(3 + i - start, 4, "%-30s", choices[i]);
                attroff(COLOR_PAIR(COLOR_PAIR_MENU));
            }
            attroff(A_REVERSE);
        }
        refresh();
        c = getch();
        switch (c) {
            case KEY_UP:
                if(current > 0) current--;
                if(current < start) start--;
                break;
            case KEY_DOWN:
                if(current < response.musicIds->size - 1) {
                    current++;
                }
                if(current >= MENU_MAX_ITEMS) start++;
                break;
            case MENU_KEY_ENTER:
                // On récupère la musique
                response = client_request_handler(MPP_GET_MUSIC, rfid, music, musicIds->musicIds[current]);
                if(response.code == MPP_RESPONSE_OK) {
                    *music = *response.music;

                } else {
                    show_request_error("Error while retrieving music !");
                    return CHOICE_MAIN_MENU;
                }
                free_music_list(musicIds);
                free(response.music);
                return CHOICE_SEQUENCER;
                break;
        }
    }
}

/**
 * @fn app_choices_t show_create_music_menu(music_t *music, char *rfid)
 * @brief La fonction qui affiche le menu de création de musique
 * @param music La musique à créer
 * @param rfid Le rfid de l'utilisateur
 * @return app_choices_t 
 * @note la fonction remplit la musique et le rfid
 * @warning la musique et le rfid doivent être alloués et initialisés
 */
app_choices_t show_create_music_menu(music_t *music, char *rfid) {
    //init_menu("Create music", "", 1);
    UNIMPLEMENTED("show_create_music_menu");
    init_music(music, 120);
    music->bpm = 120;
    char date[20];
    int oldBpm = music->bpm;
    gettimeofday(&music->date, NULL);
    show_date(music->date.tv_sec, date);

    if(*rfid == '\0') {
        attron(COLOR_PAIR(COLOR_PAIR_MENU_WARNING) | A_BOLD);
        mvprintw(3, 4, "%s", "Warning : You're in offline mode, you can't save your music !");
        attroff(COLOR_PAIR(COLOR_PAIR_MENU_WARNING) | A_BOLD);
    }
    attron(COLOR_PAIR(COLOR_PAIR_MENU | A_BOLD));
    mvprintw(5, 4, "%s", "Creation date : ");
    attroff(COLOR_PAIR(COLOR_PAIR_MENU | A_BOLD));
    attron(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT));
    mvprintw(5, 20, "%s", date);
    attroff(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT));

    attron(COLOR_PAIR(COLOR_PAIR_MENU | A_BOLD));
    mvprintw(7, 4, "%s", "Select the bpm by pressing the arrows : ");
    
    attroff(COLOR_PAIR(COLOR_PAIR_MENU | A_BOLD));
    attron(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT));
    mvprintw(7, 45, "%3d", music->bpm);
    attroff(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT));

    // Option pour créer ou retourner au menu principal en bas de l'écran
    attron(COLOR_PAIR(COLOR_PAIR_MENU | A_BOLD));
    mvprintw(APP_LINES - 3, 4, "%s", "[BTN0] Return to main menu");
    mvprintw(APP_LINES - 3, APP_COLS-22, "%s", "[BTN1] Create music");
    attroff(COLOR_PAIR(COLOR_PAIR_MENU | A_BOLD));
    refresh();
    // On gère la navigation dans le menu
    int c;
    while(1) {
        c = getch();

        switch(c) {
            case KEY_UP:
                if (music->bpm < 300) music->bpm++;
                break;
            case KEY_DOWN:
                if (music->bpm > 0) music->bpm--;
                break;
            case MENU_KEY_ENTER:
                return CHOICE_SEQUENCER;
            
            case MENU_KEY_ESCAPE:
                return CHOICE_MAIN_MENU;

            break;
        }
        attron(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT));
        mvprintw(7, 45, "%3d", music->bpm);
        attroff(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT));

        if(oldBpm != music->bpm) {
            oldBpm = music->bpm;
        }
    }
    refresh();
}

/**
 * @fn void show_sequencer(music_t *music, char *connected)
 * @brief La fonction qui affiche le séquenceur
 * @param music La musique à afficher
 * @param connected Le rfid de l'utilisateur connecté
 * @return choices_t 
 * @note l'utilisateur est considéré comme connecté si connected est différent de \0
 * @warning la musique et connected doit être allouée et initialisée
 */
app_choices_t show_sequencer(music_t *music, char *rfid) {
    UNIMPLEMENTED("show_sequencer");
    /*
    mpp_response_t reponse;
    app_choices_t choice = -1;
    note_t *note;
    int i;
    char need2save = 0;
    int btnMode = NAVIGATION_MODE;
    int c = ERR; // la touche pressée
    clear(); // on nettoie l'écran
    bkgd(COLOR_PAIR(COLOR_PAIR_SEQ)); // on change la couleur du background
    WINDOW *seqInfo = newwin(SEQUENCER_INFO_LINES, SEQUENCER_INFO_COLS, SEQUENCER_INFO_Y0, SEQUENCER_INFO_X0);
    WINDOW *seqHelp = newwin(SEQUENCER_HELP_LINES, SEQUENCER_HELP_COLS, SEQUENCER_HELP_Y0, SEQUENCER_HELP_X0);
    WINDOW *seqBody = newwin(SEQUENCER_BODY_LINES, SEQUENCER_BODY_COLS, SEQUENCER_BODY_Y0, SEQUENCER_BODY_X0);
    WINDOW *channelWin[MUSIC_MAX_CHANNELS];

    // Des variables pour la navigation dans le séquenceur
    sequencer_nav_t seqNav = create_sequencer_nav(0);
    scale_t scale = init_scale(); // Initialisation de la gammes
    // On dessine chaque fenêtre
    show_sequencer_info(seqInfo, music, 0, need2save);
    show_sequencer_help(seqHelp);
    box(seqBody, 0, 0);
    mvwprintw(seqBody, 0, 1, "%s", "SEQUENCER");
    wrefresh(seqBody);
    init_sequencer_channels(channelWin, music);
    // Navigation dans le séquenceur
    // activation des touches spéciales
    init_window(seqBody);
    while (choice == -1) {
        c = wgetch(seqBody);
        switch(c) {
            case KEY_UP:
                if(seqNav.col == SEQUENCER_NAV_COL_LINE) {
                    sequencer_nav_up(&seqNav, -1);
                    break;
                }
                note = &(music->channels[seqNav.ch].notes[seqNav.lines[seqNav.ch]]);
                change_sequencer_note(note, seqNav.col, scale, 1);
                update_channel_nbNotes(&(music->channels[seqNav.ch]), seqNav.lines[seqNav.ch]);
                need2save = 1;
                break;

            case KEY_DOWN:
                if(seqNav.col == SEQUENCER_NAV_COL_LINE) {
                    sequencer_nav_down(&seqNav, -1);
                }
                // Sinon modification de la note
                note = &(music->channels[seqNav.ch].notes[seqNav.lines[seqNav.ch]]);
                change_sequencer_note(note, seqNav.col, scale, 0);
                update_channel_nbNotes(&(music->channels[seqNav.ch]), seqNav.lines[seqNav.ch]);
                need2save = 1;
                break;
            case KEY_LEFT:
                sequencer_nav_left(&seqNav);
                break;
            case KEY_RIGHT:
                sequencer_nav_right(&seqNav);
                break;

            case KEY_SEQ_EDIT:
                btnMode = btnMode == NAVIGATION_MODE ? EDIT_MODE : NAVIGATION_MODE;
                break;

            case KEY_SEQ_SAVE:
                if(btnMode == EDIT_MODE) {
                    if(*rfid != '\0') {
                        reponse = client_request_handler(MPP_ADD_MUSIC, rfid, music, -1);
                        if(reponse.code < MPP_RESPONSE_BAD_REQUEST) {
                            need2save = 0;
                        }
                    }
                    break;
                }
                // On change de channel
                seqNav.lines[0] = seqNav.start[0] + seqNav.lines[seqNav.ch] - seqNav.start[seqNav.ch]; // On garde la même ligne (pas forcément le même start)
                seqNav.ch = 0;
                break;

            case KEY_SEQ_NAV_CH2:
                if(btnMode == EDIT_MODE) {
                    choice = CHOICE_MAIN_MENU;
                    break;
                }
                // On change de channel
                seqNav.lines[1] = seqNav.start[1] + seqNav.lines[seqNav.ch] - seqNav.start[seqNav.ch]; // On garde la même ligne (pas forcément le même start)
                seqNav.ch = 1;
                break;

            case KEY_SEQ:
                if(btnMode == EDIT_MODE) {
                    play_music(channelWin, music);
                    break;
                } 
                // On change de channel
                seqNav.lines[2] = seqNav.start[2] + seqNav.lines[seqNav.ch] - seqNav.start[seqNav.ch]; // On garde la même ligne (pas forcément le même start)
                seqNav.ch = 2;
                break;

            case KEY_BUTTON_LINEUP:
                sequencer_nav_up(&seqNav, -1);
                break;
            break;

            case KEY_SEQ_NAV_DOWN:
                sequencer_nav_down(&seqNav, -1);
            break;

            default:
                break;
        }
        // On rafraichit les fenêtres
        show_sequencer_info(seqInfo, music, btnMode, need2save);
        show_sequencer_channels(channelWin, music, &seqNav);
        //mvwprintw(seqBody, 0, 1, "%d, %d, %d %d", music->channels[0].nbNotes, music->channels[1].nbNotes, music->channels[2].nbNotes, seqNav.lines[seqNav.ch]);
    }

    // On libère la mémoire
    delwin(seqInfo);
    delwin(seqHelp);
    delwin(seqBody);
    for(i = 0; i < MUSIC_MAX_CHANNELS; i++) {
        delwin(channelWin[i]);
    }
    // On nettoie l'écran
    clear();
    return choice;
    */
    return CHOICE_MAIN_MENU;
}

/**********************************************************************************************************************/
/*                                           Private Fonction Definitions                                             */
/**********************************************************************************************************************/

/**
 * @fn void show_request_error(const char *message)
 * @brief Affichage d'une erreur de requête
 * @details Cette fonction affiche un message d'erreur de requête
 * @param message Le message d'erreur
 */
void show_request_error(const char *message) {
    UNIMPLEMENTED("show_request_error");
    clear();
    //init_menu("Error", "", 1);
    attron(COLOR_PAIR(COLOR_PAIR_MENU_WARNING) | A_BOLD);
    mvprintw(3, 4, "%s", message);
    attroff(COLOR_PAIR(COLOR_PAIR_MENU_WARNING) | A_BOLD);
    // On attend que l'utilisateur appuie sur un bouton

    mvprintw(APP_LINES - 3, 4, "%s", "[BTN4] Return to main menu");
    refresh();
}

/**
 * @fn void init_colors()
 * @brief Initialisation des couleurs
 */
void init_colors() {
    init_menu_colors();
    init_sequencer_colors();
}

