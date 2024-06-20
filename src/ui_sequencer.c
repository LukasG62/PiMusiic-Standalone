/**
 * @file ui_sequencer.c
 * @brief Fichier source pour l'interface du séquenceur
 * @version 1.0
 * @author Lukas Grando
 */
#include "ui_sequencer.h"
#include "sound.h"

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
 * @fn void play_music(music_t *music)
 * @brief Joue la musique et affiche les lignes jouées
 */
void play_music(WINDOW **channelWin, music_t *music);

/**
 * @brief play_channel(void *channelId)
 * @brief Joue un channel et modifie le navigateur pour afficher la ligne jouée
 */
void *play_channel(void *channelId);


/**********************************************************************************************************************/
/*                                           Public Functions Definition                                              */
/**********************************************************************************************************************/

/**
 * @fn init_sequencer_colors()
 * @brief Initialisation des couleurs du séquenceur
 * @details Cette fonction initialise les couleurs du séquenceur avec les paires de couleurs
 */
void init_sequencer_colors() {
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
    wattroff(win, need2save ? COLOR_PAIR(COLOR_PAIR_SEQ_WARNING) : COLOR_PAIR(COLOR_PAIR_SEQ_NOTE) | A_BOLD);
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