/**
 * \file uiManager.c
 * \details Contiens les fonctions de toutes les interfaces de l'application
 * \note Dedans il y a aussi des fonctions helpers pour l'affichage (à déplacer dans un autre fichier ?)
 * \author Lukas Grando
 * \version 1.0
 */
#include "uiManager.h"

/**********************************************************************************************************************/
/*                                           Private functions                                                        */
/**********************************************************************************************************************/
/**
 * @fn show_sequencer_channels(WINDOW **channelWin, music_t *music, sequencer_nav_t *seqNav)
 * @brief Affichage des channels du séquenceur
 * @param channelWin Les fenêtres des channels
 * @param music La musique à afficher
 * @param seqNav La structure de navigation dans le séquenceur
 * @warning Les fenêtres doivent être initialisées avec init_sequencer_channels
 * @see init_sequencer_channels
 */
void show_sequencer_channels(WINDOW **channelWin, music_t *music, sequencer_nav_t *seqNav);

/**
 * @fn void show_request_error(const char *message)
 * @brief Affichage d'une erreur de requête
 * @details Cette fonction affiche un message d'erreur de requête
 * @param message Le message d'erreur
 */
void show_request_error(const char *message);

/**
 * @fn void wait_for_key()
 * @brief Attendre l'appui sur la touche KEY_BUTTON_CHANGEMODE
 */
void wait_for_key();

/**
 * @fn void init_colors()
 * @brief Initialisation des couleurs
 */
void init_colors();

/**
 * \fn void create_custom_colors(int color_id, const char *color)
 * \brief Création de couleurs personnalisées pour ncurses en passant par des codes hexadécimaux
 * \param color_id L'identifiant de la couleur
 * \param color La couleur en hexadécimal
 * \return La couleur personnalisée
 */
int create_custom_colors(int color_id, const char *color);

/**
 * \fn void init_menu(const char *title, const char *text)
 * \brief Initialisation du menu ncurses
 * \details Cette fonction initialise le menu en affichant les bordures, le titre et le texte
 * \param title Le titre du menu
 * \param text Le texte du menu
 * \param centered Centre le texte verticalement si différent de 0
 * \note Elle ne gère pas la navigation dans le menu
 */
void init_menu(const char *title, const char *text, int centered);

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
choices_t create_menu(const char *title, const char *text, char **choices, int nbChoices, int highlight, choices_t *choices_return);

/**
 * \fn void show_sequencer_info(WINDOW *win, music_t *music, int mode, char need2save)
 * \brief Affichage des informations du séquenceur
 * \details Cette fonction affiche les informations du séquenceur
 * \param win La fenêtre où afficher les informations
 * \param music La musique à afficher
 * \param mode Le mode des boutons (0 pour le mode NAVIGATION, 1 pour le mode EDITION)
 * \param need2save Indication visuelle si la musique doit être sauvegardée
 */
void show_sequencer_info(WINDOW *win, music_t *music, int mode, char need2save);

/**
 * \fn void show_sequencer_help(WINDOW *win)
 * \brief Affichage de l'aide du séquenceur
 * \details Cette fonction affiche l'aide du séquenceur
 * \param win La fenêtre où afficher l'aide
 */
void show_sequencer_help(WINDOW *win);

/**
 * \fn void init_sequencer_channels(WINDOW **channels, music_t *music)
 * \brief Initialisation des channels du séquenceur
 * \details Cette fonction initialise les channels du séquenceur
 * \param channels Les fenêtres des channels
 * \param music La musique à afficher
 * \warning Il doit y avoir MUSIC_MAX_CHANNELS channels
*/
void init_sequencer_channels(WINDOW **channels, music_t *music);

/**
 * \fn void print_sequencer_note(WINDOW *win, note_t note, short ch, int line, sequencer_nav_t *seqNav, int isCurrLine)
 * \brief Affichage d'une ligne de notes du séquenceur
 * \details Cette fonction affiche une ligne de notes du séquenceur pour un unique channel
 * \param win La fenêtre où afficher les notes
 * \param note La note à afficher
 * \param ch L'identifiant du channel
 * \param line La ligne où afficher la note
 * \param seqNav La structure de navigation dans le séquenceur afin de pouvoir afficher un curseur
 * \param isSelected Si la ligne est la ligne selectionnée
 */
void print_sequencer_note(WINDOW *win, note_t note, short ch, int line, sequencer_nav_t *seqNav, int isSelected);

/**
 * \fn void print_sequencer_lines(WINDOW *win, short channelId, music_t *music, sequencer_nav_t *seqNav)
 * \brief Affichage des notes du séquenceur
 * \param win La fenêtre où afficher les notes
 * \param channelId L'identifiant du channel
 * \param seqNav La structure de navigation dans le séquenceur
 * \param music La musique à afficher
 * \see sequencer_nav_t
 */
void print_sequencer_lines(WINDOW *win, short channelId, music_t *music, sequencer_nav_t *seqNav);

/**
 * \fn void change_sequencer_note(note_t *note, short col, scale_t scale, int isUp)
 * \brief Modification d'une note du séquenceur en fonction de la colonne actuel et de la direction de la modification
 * \param note La note à modifier
 * \param col La colonne actuelle
 * \param scale La gamme des notes
 * \param isUp La direction de la modification (0 pour le bas, 1 pour le haut)
 * \see scale_t
*/
void change_sequencer_note(note_t *note, short col, scale_t scale, int isUp);


/**********************************************************************************************************************/
/*                                           Public Fonction Definitions                                              */
/**********************************************************************************************************************/

/**
 * \fn void init_ncurses()
 * \brief Initialisation de ncurses et de la fenêtre
 * \details Cette fonction s'occupe de l'initialisation de ncurses, des couleurs, de la fenêtre et des bordures
 */
void init_ncurses() {
    // Initialisation de la fenêtre
    initscr(); // Initialisation de ncurses
    noecho(); // Désactivation de l'affichage des caractères saisis
    cbreak(); // Désactivation du buffering de ligne
    init_window(stdscr); // Initialisation de la fenêtre principale
    start_color(); // Activation des couleurs
    init_colors(); // Initialisation des couleurs
}

/**
 * @fn init_window(WINDOW *win)
 * @brief Initialisation de la fenêtre principale
 * @param win La fenêtre à initialiser
 */
void init_window(WINDOW *win) {
    keypad(win, TRUE); // activation des touches spéciales
    wtimeout(win, 1); // on met un timeout pour le getch
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
 * \fn choices_t show_main_menu()
 * \brief Affichage du menu principal
 * \details Cette fonction affiche le menu principal et gère la navigation dans le menu
 * \return Le choix de l'utilisateur
 * \see choices_t
 */
choices_t show_main_menu() {
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
    choices_t choices_return[] = {
        CHOICE_CONECTION_MENU,
        CHOICE_CREATEMUSIC,
        CHOICE_QUITAPP,
    };

    // On affiche le menu
    return create_menu(title, text, choices, n_choices, 0, choices_return);

}

/**
 * @fn choices_t show_connection_menu(char *rfid, char *username)
 * @brief Affichage du menu de connexion et effectue la connexion
 * @param rfid Le rfid de l'utilisateur
 * @param username Le nom d'utilisateur de l'utilisateur
 * @return choices_t 
 * @note la fonction remplit les variables rfid et username
 */
choices_t show_connection_menu(char *rfid, char *username) {
    // Affichage du menu
    init_menu("Connection", "Please scan your badge", 0);
    // On affiche le badge RFID
    attron(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT));
    mvprintw(3, 4, "Your badge : %-30s", rfid);
    attroff(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT));
    // On affichage un message pour dire que la connexion est en cours
    attron(COLOR_PAIR(COLOR_PAIR_MENU_WARNING) | A_BOLD);
    mvprintw(5, 4, "Connection in progress...");
    attroff(COLOR_PAIR(COLOR_PAIR_MENU_WARNING) | A_BOLD);
    refresh();

    // On fait une requête de connexion
    mpp_response_t response = client_request_handler(MPP_CONNECT, rfid, NULL, -1);

    // On affiche le résultat de la connexion
    if(response.code < MPP_RESPONSE_BAD_REQUEST) {
        attron(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT) | A_BOLD);
        mvprintw(5, 4, "Connection successful ! Welcome %s", response.username);
        attroff(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT) | A_BOLD);
        refresh();
        strcpy(username, response.username);

        return CHOICE_MENU_LIST;
    } else {
        show_request_error("Error while connecting !");
        return CHOICE_MAIN_MENU;
    }

}

/**
 * @fn choices_t show_list_music(char *rfid, music_t *music)
 * @brief Affiche le menu de liste de musique et effectue les requetes pour fetch la musique
 * @param rfid Le rfid de l'utilisateur
 * @param music la musique choisie
 * @return choices_t 
 * @note la fonction remplit la musique
 * @warning la musique et le rfid doivent être alloués et initialisés
 */
choices_t show_list_music(char *rfid, music_t *music) {
    mpp_response_t response;
    musicId_list_t *musicIds = NULL;
    int current = 0;
    int start = 0;
    int c;
    init_menu("List of music", "Please select a music :", 0);
    if(*rfid == '\0') {
        clear();
        attron(COLOR_PAIR(COLOR_PAIR_MENU_WARNING) | A_BOLD);
        mvprintw(3, 4, "%s", "You're not connected, you can't retrieve music !");
        mvprintw(4, 4, "%s", "Press any key to return to the main menu");
        attroff(COLOR_PAIR(COLOR_PAIR_MENU_WARNING) | A_BOLD);
        wait_for_key();
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
        wait_for_key();
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
        for(i = start; i < (start + MAX_MENU_ITEMS) && (i < musicIds->size); i++) {
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
        timeout(1);
        if(c == ERR) c = getchr_wiringpi();
        switch (c) {
            case KEY_UP:
                if(current > 0) current--;
                if(current < start) start--;
                break;
            case KEY_DOWN:
                if(current < response.musicIds->size - 1) {
                    current++;
                }
                if(current >= MAX_MENU_ITEMS) start++;
                break;
            case KEY_BUTTON_CHANGEMODE:
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


/**
 * @fn choices_t show_create_music_menu(music_t *music, char *rfid)
 * @brief La fonction qui affiche le menu de création de musique
 * @param music La musique à créer
 * @param rfid Le rfid de l'utilisateur
 * @return choices_t 
 * @note la fonction remplit la musique et le rfid
 * @warning la musique et le rfid doivent être alloués et initialisés
 */
choices_t show_create_music_menu(music_t *music, char *rfid) {
    init_menu("Create music", "", 1);
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
    mvprintw(RPI_LINES - 3, 4, "%s", "[BTN0] Return to main menu");
    mvprintw(RPI_LINES - 3, RPI_COLS-22, "%s", "[BTN1] Create music");
    attroff(COLOR_PAIR(COLOR_PAIR_MENU | A_BOLD));
    refresh();
    // On gère la navigation dans le menu
    int c;
    while(1) {
        c = getch();
        if(c == ERR) c = getchr_wiringpi();

        switch(c) {
            case KEY_UP:
                if (music->bpm < 300) music->bpm++;
                break;
            case KEY_DOWN:
                if (music->bpm > 0) music->bpm--;
                break;
            case KEY_BUTTON_CHANGEMODE:
                return CHOICE_SEQUENCER;
            
            case KEY_BUTTON_CH1NSAVE:
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
 * @fn void sequencer_nav_up(sequencer_nav_t *nav)
 * @brief permet de passer d'une ligne à une autre dans le séquenceur
 * @param nav la structure de navigation
 */
void sequencer_nav_up(sequencer_nav_t *nav, int channelId) {
    if(channelId == -1) {
        channelId = nav->ch;
    }
    if (nav->lines[channelId] > 0) nav->lines[channelId]--;
    if (nav->lines[nav->ch] < nav->start[channelId]) nav->start[channelId] = nav->start[channelId] - SEQUENCER_CH_LINES + 3; // On défile vers le haut
}

/**
 * @fn void sequencer_nav_down(sequencer_nav_t *nav)
 * @brief permet de passer d'une ligne à une autre dans le séquenceur
 * @param nav la structure de navigation
 */
void sequencer_nav_down(sequencer_nav_t *nav, int channelId) {
    if(channelId == -1) {
        channelId = nav->ch;
    }
    if (nav->lines[channelId] >= nav->start[channelId] + SEQUENCER_CH_LINES - 4) nav->start[channelId] = nav->start[channelId] + SEQUENCER_CH_LINES - 3; // On défile vers le bas
    if (nav->lines[channelId] < CHANNEL_MAX_NOTES - 1) nav->lines[channelId]++;
}

/**
 * @fn void sequencer_nav_left(sequencer_nav_t *nav)
 * @brief La fonction qui permet de passer d'une colonne à une autre dans le séquenceur (vers la gauche)
 * @param nav la structure de navigation
 */
void sequencer_nav_left(sequencer_nav_t *nav) {
    if (nav->col == SEQUENCER_NAV_COL_LINE) {
        // on change de channel
        int newChannel = (nav->ch - 1) != -1 ? nav->ch-1 : SEQUENCER_NAV_CH_MAX - 1;
        nav->start[newChannel] = nav->start[nav->ch];
        nav->lines[newChannel] = nav->lines[nav->ch];
        nav->ch = newChannel;
        nav->col = SEQUENCER_NAV_COL_TIME; // on revient à la colonne de la durée
        return ;
    }
    // Sinon on change de colonne
    if (nav->col > 0) nav->col--;
}

/**
 * @fn void sequencer_nav_right(sequencer_nav_t *nav)
 * @brief La fonction qui permet de passer d'une colonne à une autre dans le séquenceur (vers la droite)
 * @param nav 
 */
void sequencer_nav_right(sequencer_nav_t *nav) {
    if (nav->col == SEQUENCER_NAV_COL_TIME) {
        // on change de channel
        int newChannel = (nav->ch + 1) % SEQUENCER_NAV_CH_MAX;
        nav->start[newChannel] = nav->start[nav->ch]; // on switch de channel mais on garde la ligne à la même position
        nav->lines[newChannel] = nav->lines[nav->ch];
        nav->ch = newChannel;
        nav->col = SEQUENCER_NAV_COL_LINE; // on revient à la colonne de la ligne
        return ;
    }
    if (nav->col < SEQUENCER_NAV_COL_MAX - 1) nav->col++;
}

/**
 * @fn create_sequencer_nav()
 * @brief Création de la structure de navigation du séquenceur
 * @return sequencer_nav_t 
 */
sequencer_nav_t create_sequencer_nav(int playMode) {
    sequencer_nav_t nav;
    nav.col = SEQUENCER_NAV_COL_LINE;
    nav.ch = SEQUENCER_NAV_CH1;
    int i;
    for (i = 0; i < SEQUENCER_NAV_CH_MAX; i++) {
        nav.start[i] = 0;
        nav.lines[i] = 0;
    }
    nav.playMode = playMode;
    //nav.line = 0;
    return nav;
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
choices_t show_sequencer(music_t *music, char *rfid) {
    mpp_response_t reponse;
    choices_t choice = -1;
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
        if (c == ERR) c = getchr_wiringpi();
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

            case KEY_BUTTON_CHANGEMODE:
                btnMode = btnMode == NAVIGATION_MODE ? EDIT_MODE : NAVIGATION_MODE;
                break;

            case KEY_BUTTON_CH1NSAVE:
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

            case KEY_BUTTON_CH2NQUIT:
                if(btnMode == EDIT_MODE) {
                    choice = CHOICE_MAIN_MENU;
                    break;
                }
                // On change de channel
                seqNav.lines[1] = seqNav.start[1] + seqNav.lines[seqNav.ch] - seqNav.start[seqNav.ch]; // On garde la même ligne (pas forcément le même start)
                seqNav.ch = 1;
                break;

            case KEY_BUTTON_CH3NPLAY:
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

            case KEY_BUTTON_LINEDOWN:
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
}

/**
 * @fn int getchr_wiringpi()
 * @brief Transpose le bitmap des buttons wiringpi en caractère ncurses
 * @return int 
 */
int getchr_wiringpi()
{
    return ERR;
}

/**
 * @fn void play_music(music_t *music)
 * @brief Joue la musique et affiche les lignes jouées
 */
void play_music(WINDOW **channelWin, music_t *music) {
    pthread_t threads[MUSIC_MAX_CHANNELS];
    sem_t show_sem[MUSIC_MAX_CHANNELS];
    sem_t syncSem; // Sémaphore de synchronisation
    sem_t finishSem; // Sémaphore de fin
    sequencer_nav_t seqNav = create_sequencer_nav(1);
    int i, finishCount = 0;
    // On attend que tout est en place pour jouer la musique
    sem_init(&syncSem, 0, 0);
    sem_init(&finishSem, 0, 0);
    for(i = 0; i < MUSIC_MAX_CHANNELS; i++) {
        sem_init(show_sem + i , 0, 0);
        channel_thread_args_t *args = create_channel_thread_args(show_sem + i, &syncSem, &finishSem, &seqNav, music, i);
        pthread_create(&threads[i], NULL, play_channel, (void *) args);
        // mettre la priorité des threads aux maximum
        struct sched_param param;
        param.sched_priority = sched_get_priority_max(SCHED_FIFO);
        pthread_setschedparam(threads[i], SCHED_FIFO, &param);
    }
    show_sequencer_channels(channelWin, music, &seqNav);
    for(i = 0; i < MUSIC_MAX_CHANNELS; i++) {
        // On débloque les threads
        sem_post(&syncSem);
    }

    while(finishCount < MUSIC_MAX_CHANNELS) {
        if(sem_trywait(&finishSem) == 0) finishCount++;

        for(i = 0; i < MUSIC_MAX_CHANNELS; i++) {
            if(sem_trywait(&show_sem[i]) == 0) {
                print_sequencer_lines(channelWin[i], i, music, &seqNav);
            }
        }
    }

    // On libère les sémaphores
    sem_destroy(&syncSem);
    sem_destroy(&finishSem);
    for(i = 0; i < MUSIC_MAX_CHANNELS; i++) {
        sem_destroy(&show_sem[i]);
    }

}

/**
 * @brief play_channel(void *channelId)
 * @brief Joue un channel et modifie le navigateur pour afficher la ligne jouée
 */
void *play_channel(void *args) {
    // On récupère les arguments
    channel_thread_args_t *channelArgs = (channel_thread_args_t *) args;
    int effect = 0;
    int channelId = channelArgs->channel;
    int i;
    sem_t *syncSem = channelArgs->syncSem;
    sem_t *finishSem = channelArgs->finishSem;
    sem_t *show_sem = channelArgs->showSem;
    sequencer_nav_t *seqNav = channelArgs->seqNav;
    music_t *music = channelArgs->music;
    channel_t *channel = &(music->channels[channelId]);
    snd_pcm_t *pcm;
    // On initialise le pcm
    init_sound(&pcm);
    // On attend que tout le monde soit prêt
    sem_wait(syncSem);
    for(i = 0; i < channel->nbNotes; i++) {
        snd_pcm_prepare(pcm);
        // On joue la note
        play_note(channel->notes[i], music->bpm, pcm, effect);
        snd_pcm_drain(pcm);
        sequencer_nav_down(seqNav, channelId);
        // On met à jour la fenêtre
        sem_post(show_sem);
    }
    // On libère le pcm
    end_sound(pcm);
    // On libère le sémaphore de fin
    sem_post(finishSem);
    free(channelArgs);
    pthread_exit(NULL);
}

/**
 * @brief create_channel_thread_args(sem_t *syncSem, sem_t *finishSem, sequencer_nav_t *seqNav, music_t *music, int channel)
 * @brief Crée les arguments pour le thread de channel
 * @return channel_thread_args_t 
 * @note Les arguments doivent être libérés après utilisation
 */
channel_thread_args_t *create_channel_thread_args(sem_t *showSem, sem_t *syncSem, sem_t *finishSem, sequencer_nav_t *seqNav, music_t *music, int channel) {
    channel_thread_args_t *args = malloc(sizeof(channel_thread_args_t));
    args->showSem = showSem;
    args->syncSem = syncSem;
    args->finishSem = finishSem;
    args->seqNav = seqNav;
    args->music = music;
    args->channel = channel;
    return args;
}

/**********************************************************************************************************************/
/*                                           Private Fonction Definitions                                             */
/**********************************************************************************************************************/

/**
 * \fn show_sequencer_info(WINDOW *win, int mode) 
 * \brief Affichage des informations du séquenceur
 * \details Cette fonction affiche les informations du séquenceur
 * \param win La fenêtre où afficher les informations
 * \param music La musique à afficher
 * \param mode Le mode des boutons (0 pour le mode NAVIGATION, 1 pour le mode EDITION)
 */
void show_sequencer_info(WINDOW *win, music_t *music, int mode, char need2save) {
    werase(win);
    char date[20];
    show_date(music->date.tv_sec, date);
    // On crée les bordures
    box(win, 0, 0);
    
    mvwprintw(win, 0, 1, "%s", "INFO");
    mvwprintw(win, 1, 1, "Created :");
    mvwprintw(win, 2, 1, "BPM :");
    mvwprintw(win, 3, 1, "Mode :");
    // On affiche les informations
    wattron(win, ( need2save ? COLOR_PAIR(COLOR_PAIR_SEQ_NOTSAVED) : COLOR_PAIR(COLOR_PAIR_SEQ_SAVED) )  | A_BOLD);
    mvwprintw(win, 1, 10, " %s", date);
    wattroff(win, need2save ? COLOR_PAIR(COLOR_PAIR_MENU_WARNING) : COLOR_PAIR(COLOR_PAIR_SEQ_NOTE) | A_BOLD);
    wattron(win, A_BOLD);
    mvwprintw(win, 2, 6, " %d", music->bpm);
    wattroff(win, A_BOLD);

    if(mode == NAVIGATION_MODE) {
        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_OCTAVE) | A_BOLD);
        mvwprintw(win, 3, 8, "%s", "NAVIGATION");
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_OCTAVE));
        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ) | A_BOLD);
        mvwprintw(win, 4, 1, "[BTN1] CH1         [BTN2] CH2       [BTN3] CH3 ");
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ) | A_BOLD);
    }
    else {
        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_NOTE) | A_BOLD);
        mvwprintw(win, 3, 8, "%s", "EDITION");
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_NOTE));
        wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ));
        mvwprintw(win, 4, 1, "[BTN1] Save        [BTN2] Quit       [BTN3] Play ");
        wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ) | A_BOLD);
    }
    
    wrefresh(win);
}

/**
 * \fn show_sequencer_help(WINDOW *win)
 * \brief Affichage de l'aide du séquenceur
 * \details Cette fonction affiche l'aide du séquenceur
 * \param win La fenêtre où afficher l'aide
 */
void show_sequencer_help(WINDOW *win) {
    // On efface la fenêtre
    werase(win);
    // On crée les bordures
    box(win, 0, 0);
    wattron(win, A_BOLD);
    mvwprintw(win, 0, 1, "%s", "HELP");
    wattroff(win, A_BOLD);
    // Affichage des contrôles (TODO : eventuellement créer des macros pour les positions)
    mvwprintw(win, 1, 1, "%s", " / : Change note/octave/instrument/shift");
    mvwaddch(win, 1, 1, ACS_DARROW);
    mvwaddch(win, 1, 3, ACS_UARROW);

    mvwprintw(win, 2, 1, "%s", " / : Change column in the channel");
    mvwaddch(win, 2, 1, ACS_LARROW);
    mvwaddch(win, 2, 3, ACS_RARROW);

    mvwprintw(win, 3, 1, "%s", "[BTN4] : Change button mode");
    // On rafraichit la fenêtre
    wrefresh(win);
}

/**
 * \fn void init_sequencer_channels(WINDOW **channels, music_t *music)
 * \brief Initialisation des channels du séquenceur
 * \details Cette fonction initialise les channels du séquenceur
 * \param channels Les fenêtres des channels
 * \param music La musique à afficher
 * \warning Il doit y avoir MUSIC_MAX_CHANNELS channels
*/
void init_sequencer_channels(WINDOW **channelWin, music_t *music) {
    int i;
    channelWin[0] = newwin(SEQUENCER_CH_LINES, SEQUENCER_CH_COLS, SEQUENCER_CH_Y0, SEQUENCER_CH1_X0);
    channelWin[1] = newwin(SEQUENCER_CH_LINES, SEQUENCER_CH_COLS, SEQUENCER_CH_Y0, SEQUENCER_CH2_X0);
    channelWin[2] = newwin(SEQUENCER_CH_LINES, SEQUENCER_CH_COLS, SEQUENCER_CH_Y0, SEQUENCER_CH3_X0);
    
    sequencer_nav_t seqNav = create_sequencer_nav(0);
    for(i = 0; i < MUSIC_MAX_CHANNELS; i++) {
        WINDOW *ch = channelWin[i];
        // On efface les fenêtres
        werase(ch);
        // On change les bordures
        box(ch, 0, 0);
        // On affiche les entêtes
        mvwprintw(ch, 0, 1, "%s %d", "CHANNEL", i+1);
        // On affiche l'en-tête des colonnes
        mvwprintw(ch, 1, 1, "%s", "LINE|NOTE|OCTA|INST|SHFT");
        // On affiche les lignes
        print_sequencer_lines(ch, i, music, &seqNav);
        // On rafraichit la fenêtre
        wrefresh(ch);
    }
}

/**
 * \fn void init_colors()
 * \brief Initialisation des couleurs
 * \details Cette fonction s'occupe de l'initialisation des couleurs
 */
void init_colors() {
    // Initialisation des couleurs
    // TODO : CHANGER LES COULEURS
    init_pair(COLOR_PAIR_MENU, COLOR_BLUE, COLOR_WHITE);
    init_pair(COLOR_PAIR_MENU_WARNING, COLOR_RED, COLOR_WHITE);
    init_pair(COLOR_PAIR_MENU_PROMPT, COLOR_GREEN, COLOR_WHITE);
    init_pair(COLOR_PAIR_SEQ, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_NOTSAVED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_SAVED, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_PLAYED, COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR_PAIR_SEQ_OCTAVE, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_NOTE, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_INSTRUMENT, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_SHIFT, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_HEADER_TITLE, COLOR_WHITE, COLOR_BLACK);
    
}

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
 * \fn void init_menu(const char *title, const char *text)
 * \brief Initialisation du menu ncurses
 * \details Cette fonction initialise le menu en affichant les bordures, le titre et le texte
 * \param title Le titre du menu
 * \param text Le texte du menu
 * \param centered Centre le texte verticalement si différent de 0
 * \note Elle ne gère pas la navigation dans le menu
 */
void init_menu(const char *title, const char *text, int centered) {
    int lenTitle = strlen(title);
    int lenText = strlen(text);
    // On change la couleur du background
    wbkgd(stdscr, COLOR_PAIR(COLOR_PAIR_MENU));
    // On ajoute les bordures
    box(stdscr, 0, 0);
    // On affiche le titre centrée en haut dans la bordure en gras
    attron(COLOR_PAIR(COLOR_PAIR_MENU) | A_BOLD);
    mvprintw(0, (RPI_COLS - lenTitle) / 2, "%s", title);
    attroff(COLOR_PAIR(COLOR_PAIR_MENU) | A_BOLD);

    // Si le texte doit être centré
    if (centered) {
        // On centre le texte verticalement
        mvprintw(RPI_LINES/2, (RPI_COLS - lenText) / 2, "%s", text);
    }
    else {
        mvprintw(3, 4, "%s", text);
    }
    refresh();
}

/**
 * \fn choice_t create_menu(const char *choices[], int n_choices, int highlight, choices_t choices)
 * \brief Création d'un menu ncurses
 * \details Cette fonction crée un menu ncurses
 * \param title Le titre du menu
 * \param text Le texte du menu (optionnel)
 * \param choices[] Les choix du menu
 * \param nbChoices Le nombre de choix
 * \param highlight La couleur du choix sélectionné
 * \param choices Valeur de retour de chaque choix
 * 
*/
choices_t create_menu(const char *title, const char *text, char **choices, int nbChoices, int highlight, choices_t *choices_return) {
    clear(); // on nettoie l'écran
    int c; // la touche pressée
    int i;
    int curr = highlight; // le choix actuel
    int start = 0; // pour le mode pagination 
    init_menu(title, text, 0); // Initialisation du menu

    // On gère la navigation dans le menu
    while(1) {
        // On gére à la fois les touches du clavier et les boutons physiques
        // On affiche les choix
        for (i = start; (i < nbChoices) && (i < MAX_MENU_ITEMS); i++) {
            if (i == curr) {
                attron(A_REVERSE);
                mvprintw(5 + i-start, 8, "%s", choices[i]);
            } 
            else {
                mvprintw(5 + i-start, 8, "%s", choices[i]);
            }
            attroff(A_REVERSE);
        }
        refresh();
        c = getch();
        if(c == ERR) c = getchr_wiringpi();
        switch(c) {
            case KEY_UP:
                if (curr > 0) curr--;
                if(curr < start) start--;
                break;
            case KEY_DOWN:
                if (curr < nbChoices - 1) curr++;
                if(curr >= start + MAX_MENU_ITEMS) start++;
                break;
            case KEY_BUTTON_CHANGEMODE:
                return choices_return[curr];
            break;

            case ERR:
                break;
        }
    }
}


/**
 * \fn void print_sequencer_lines(WINDOW *win, short channelId, music_t *music, sequencer_nav_t *seqNav)
 * \brief Affichage des notes du séquenceur
 * \details Cette fonction affiche les notes du séquenceur
 * \param win La fenêtre où afficher les notes
 * \param channelId L'identifiant du channel
 * \param seqNav La structure de navigation dans le séquenceur
 * \param music La musique à afficher
 * \see sequencer_nav_t
 */
void print_sequencer_lines(WINDOW *win, short channelId, music_t *music, sequencer_nav_t *seqNav) {
    // On affiche les informations
    int i;
    for(i = 0; i < SEQUENCER_CH_LINES - 3; i++) {
        if(seqNav->start[channelId] + i >= CHANNEL_MAX_NOTES) {
            note_t emptyNote = create_note(-1, -1, -1, -1, -1);
            print_sequencer_note(win, emptyNote, channelId, i, seqNav, 0);
            continue;
        }
        note_t note = music->channels[channelId].notes[seqNav->start[channelId] + i];
        int isSelected = 0;
        if(seqNav->ch == channelId && seqNav->lines[channelId] == seqNav->start[channelId]+i) isSelected = 1;
        print_sequencer_note(win, note, channelId, i, seqNav, isSelected);
    }
    // On rafraichit la fenêtre
    wrefresh(win);
}

/**
 * \fn void print_sequencer_note(WINDOW *win, note_t note, short ch, int line, sequencer_nav_t *seqNav, int isCurrLine)
 * \brief Affichage d'une ligne de notes du séquenceur
 * \details Cette fonction affiche une ligne de notes du séquenceur pour un unique channel
 * \param win La fenêtre où afficher les notes
 * \param note La note à afficher
 * \param line La ligne où afficher la note
 * \param seqNav La structure de navigation dans le séquenceur afin de pouvoir afficher un curseur
 * \param isSelected Si la ligne est la ligne selectionnée
 */
void print_sequencer_note(WINDOW *win, note_t note, short ch, int line, sequencer_nav_t *seqNav, int isSelected) {
    //TODO : Rajouter position du curseur pour reverse la colonne sélectionnée sur la ligne
    //TODO : Rajouter si la ligne est jouée alors toute la ligne est inversée
    char instrumentName[5]; // Nom de l'instrument
    char noteName[3]; // Nom de la note
    int playModeSelected = seqNav->playMode && seqNav->lines[ch] == seqNav->start[ch] + line ? 1 : 0;
    note2str(note, noteName); // On récupère le nom de la note
    instrument2str(note.instrument, instrumentName); // On récupère le nom de l'instrument
    wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ) | REVERSE_IF_COL(seqNav->col, SEQUENCER_NAV_COL_LINE, isSelected) |  REVERSE_IF_COL(seqNav->col, SEQUENCER_NAV_COL_LINE, playModeSelected));
    if(seqNav->start[ch] + line >= CHANNEL_MAX_NOTES) {
        mvwprintw(win, 2+line, 1, "%s", "----");
    }
    else mvwprintw(win, 2+line, 1, "%04X", seqNav->start[ch] + line);
    wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ) | REVERSE_IFNOT_PLAYMODE(seqNav->playMode, playModeSelected));
    wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ));
    mvwprintw(win, 2+line, 5, "|");
    mvwprintw(win, 2+line, 10, "|");
    mvwprintw(win, 2+line, 15, "|");
    mvwprintw(win, 2+line, 20, "|");
    wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ));
    
    wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_NOTE) | REVERSE_IF_COL(seqNav->col, SEQUENCER_NAV_COL_NOTE, isSelected));
    mvwprintw(win, 2+line, 6, " %-2s ", noteName);
    wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_NOTE) | REVERSE_IFNOT_PLAYMODE(seqNav->playMode, playModeSelected) );
    
    wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_OCTAVE) | REVERSE_IF_COL(seqNav->col, SEQUENCER_NAV_COL_OCTAVE, isSelected));
    mvwprintw(win, 2+line, 11, " %02d ", note.octave);
    wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_OCTAVE) | REVERSE_IFNOT_PLAYMODE(seqNav->playMode, playModeSelected) );
    
    wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_INSTRUMENT) | REVERSE_IF_COL(seqNav->col, SEQUENCER_NAV_COL_INSTRUMENT, isSelected));
    mvwprintw(win, 2+line, 16, "%s", instrumentName);
    wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_INSTRUMENT) | REVERSE_IFNOT_PLAYMODE(seqNav->playMode, playModeSelected) );
    
    wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_SHIFT) | REVERSE_IF_COL(seqNav->col, SEQUENCER_NAV_COL_TIME, isSelected));
    mvwprintw(win, 2+line, 21, " %02d ", note.time);
    wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_SHIFT) | A_REVERSE ); // Dans tous les cas on déactive le reverse à la fin
}

/**
 * \fn void change_sequencer_note(note_t *note, short col, scale_t scale, int isUp)
 * \brief Modification d'une note du séquenceur en fonction de la colonne actuel et de la direction de la modification
 * \param note La note à modifier
 * \param col La colonne actuelle
 * \param scale La gamme des notes
 * \param isUp La direction de la modification (0 pour le bas, 1 pour le haut)
 * \see scale_t
*/
void change_sequencer_note(note_t *note, short col, scale_t scale, int isUp) {
    switch(col) {
        case SEQUENCER_NAV_COL_NOTE:
            if (isUp) get_next_note(note, &scale);
            else get_previous_note(note, &scale);

            if(note->id != NOTE_NA_ID) {
                if (note->instrument == INSTRUMENT_NA) note->instrument = INSTRUMENT_SIN;

            }
            else {
                note->instrument = INSTRUMENT_NA;
            }
            break;
        case SEQUENCER_NAV_COL_OCTAVE:
            if (isUp) note->octave = note->octave + 1 > 8 ? 8 : note->octave + 1;
            else note->octave = note->octave - 1 < 0 ? 8 : note->octave - 1;
            break;
        case SEQUENCER_NAV_COL_INSTRUMENT:
            if (isUp) note->instrument = note->instrument + 1 == INSTRUMENT_NB ? 0 : note->instrument + 1;
            else note->instrument = (note->instrument - 1) == -1 ? INSTRUMENT_NB - 1 : note->instrument - 1;
            break;
        case SEQUENCER_NAV_COL_TIME:
            if(note->time < TIME_END) note->time = isUp ? note->time * 2 : note->time / 2;
            else note->time = isUp ? TIME_CROCHE_DOUBLE : note->time / 2;
            if(note->time == 0) note->time = TIME_RONDE;
            break;
    }
}

/**
 * @fn void wait_for_key()
 * @brief Attendre l'appui sur la touche KEY_BUTTON_CHANGEMODE
 */
void wait_for_key() {
    // On attends que la touche KEY_BUTTON_CHANGEMODE soit pressée
    int c;
    while(1) {
        c = getch();
        if(c == ERR) c = getchr_wiringpi();
        if(c == KEY_BUTTON_CHANGEMODE) break;
    }
}


/**
 * @fn void show_request_error(const char *message)
 * @brief Affichage d'une erreur de requête
 * @details Cette fonction affiche un message d'erreur de requête
 * @param message Le message d'erreur
 */
void show_request_error(const char *message) {
    clear();
    init_menu("Error", "", 1);
    attron(COLOR_PAIR(COLOR_PAIR_MENU_WARNING) | A_BOLD);
    mvprintw(3, 4, "%s", message);
    attroff(COLOR_PAIR(COLOR_PAIR_MENU_WARNING) | A_BOLD);
    // On attend que l'utilisateur appuie sur un bouton

    mvprintw(RPI_LINES - 3, 4, "%s", "[BTN4] Return to main menu");
    refresh();
    wait_for_key();
}


/**
 * @fn show_sequencer_channels(WINDOW **channelWin, music_t *music, sequencer_nav_t *seqNav)
 * @brief Affichage des channels du séquenceur
 * @param channelWin Les fenêtres des channels
 * @param music La musique à afficher
 * @param seqNav La structure de navigation dans le séquenceur
 * @warning Les fenêtres doivent être initialisées avec init_sequencer_channels
 * @see init_sequencer_channels
 */
void show_sequencer_channels(WINDOW **channelWin, music_t *music, sequencer_nav_t *seqNav) {
    int i;
    for(i = 0; i < MUSIC_MAX_CHANNELS; i++) {
        print_sequencer_lines(channelWin[i], i, music, seqNav);
    }
}


