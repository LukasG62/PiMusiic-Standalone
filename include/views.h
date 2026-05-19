/**
 * \file ui_views.h
 * \brief Fichier d'en-tête pour les vues de l'interface graphique
 * \version 1.0
 * \author Lukas Grando
 * \see ui_view.h
 */
#ifndef UI_VIEWS_H
#define UI_VIEWS_H

#include "event/event_queue.h"
#include "core/core.h"
#include "ui/base/ui_view.h"
#include "ui/base/ui_menu.h"
#include "ui/form/ui_form_input.h"
#include "ui/form/ui_form_button.h"
#include "ui/form/ui_form_list.h"

typedef enum {
    VIEW_MAIN_MENU,
    VIEW_HELP,
    VIEW_CREDENTIALS,
    VIEW_CREATE_MUSIC,
    VIEW_LIST_MUSIC,
    VIEW_SEQUENCER,
    VIEW_CREDITS,
    VIEW_ERROR,
    VIEW_QUIT
} app_view_t;

// todo: Je me suis encore pas mis d'accord sur si les views doivent avoir des paramètres.
// ou si elles doivent juste accéder au contexte global de l'application pour récupérer les données dont elles ont besoin.
// pour l'instant y'a un mélange des deux, mais je pense que ça serait plus propre d'acceder au contexte global (quitte à le fournir en paramètre unique aux vues)
// comme ça si une vue évolue pas besoin de changer sa signature

/**
 * \brief Construit le menu principal.
 * \param queue Pour envoyer l'événement de changement de vue.
 */
ui_view_t *view_main_menu();

/**
 * \brief Construit la vue de connexion.
 * \param creds Pointeur vers la structure à remplir par le formulaire.
 * \param queue Pour envoyer l'événement une fois validé.
 */
ui_view_t *view_credentials(ui_menu_credentials_t *creds);

/**
 * \brief Construit la vue des crédits.
 * \param queue Pour envoyer l'événement de retour.
 */
ui_view_t *view_credits();

/**
 * \brief Construit la vue du séquenceur.
 * \param ctx Contexte de l'application pour accéder à la musique en cours et autres données nécessaires.
 */
ui_view_t *view_sequencer(app_context_t *ctx);

/**
 * \brief Construit la vue de création de musique.
 * \param ctx Contexte de l'application pour accéder aux données nécessaires.
 */
ui_view_t *view_create_music(music_t *music, ui_menu_credentials_t *creds, bool isConnected);

#endif // UI_VIEWS_H
