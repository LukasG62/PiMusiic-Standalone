/**
 * @file ui_manager.c
 * @brief Gestionnaire principal de l'interface graphique
 * @author Lukas Grando
 */

#include "ui/base/ui_manager.h"

static ui_view_t *currentView = NULL;
static ui_view_t *viewHistory[UI_MANAGER_HISTORY_SIZE];
static int viewHistoryIndex = 0;

#define DESTROY_CURRENT_VIEW() do { \
	if(currentView) { \
		if(currentView->destroy) currentView->destroy(currentView); \
		currentView = NULL; \
	} \
} while(0)

/**
 * @fn static ui_view_t *ui_manager_pop_view_from_history()
 * @brief Récupère la dernière vue de l'historique pour la navigation arrière
 * @return ui_view_t* La vue précédente ou NULL si l'historique est vide
 */
static ui_view_t *ui_manager_pop_view_from_history() {
	if(viewHistoryIndex <= 0) return NULL;
	viewHistoryIndex--;
	ui_view_t *view = viewHistory[viewHistoryIndex];
	viewHistory[viewHistoryIndex] = NULL;
	return view;
}

/**
 * @fn static void ui_manager_push_view_to_history(ui_view_t *view)
 * @brief Ajoute une vue à l'historique pour la navigation arrière
 * @param view La vue à ajouter à l'historique
 * @note Si l'historique est plein, la vue la plus ancienne sera détruite pour faire de la place à la nouvelle vue
 */
static void ui_manager_push_view_to_history(ui_view_t *view) {
	if(viewHistoryIndex >= UI_MANAGER_HISTORY_SIZE) {
		if(viewHistory[0] && viewHistory[0]->destroy) {
			viewHistory[0]->destroy(viewHistory[0]);
		}
		for(int i = 1; i < UI_MANAGER_HISTORY_SIZE; i++) {
			viewHistory[i - 1] = viewHistory[i];
		}
		viewHistoryIndex--;
	}
	viewHistory[viewHistoryIndex] = view;
	viewHistoryIndex++;
}

/**
 * \fn void ui_manager_init()
 * \brief Initialise ncurses, la souris, et les couleurs
 */
void ui_manager_init(void) {
	init_ncurses();
	// initialiser l'historique des vues
	for(int i = 0; i < UI_MANAGER_HISTORY_SIZE; i++) {
		viewHistory[i] = NULL;
	};
	timeout(50);
	if(has_colors()) {
		ui_menu_init_colors();
		ui_seq_init_colors();
	}
}

/**
 * \fn void ui_manager_cleanup()
 * \brief nettoie la vue actuelle et quitte ncurses proprement
 */
void ui_manager_cleanup(void) {
	DESTROY_CURRENT_VIEW();
	// Nettoyer l'historique des vues
	for(int i = 0; i < viewHistoryIndex; i++) {
		if(viewHistory[i] && viewHistory[i]->destroy) {
			viewHistory[i]->destroy(viewHistory[i]);
		}
	}
	exit_ncurses();
}

/**
 * \fn void ui_manager_set_view(ui_view_t *newView)
 * \brief Remplace la vue actuelle par une nouvelle
 * \param newView La nouvelle vue à afficher
 */
void ui_manager_set_view(ui_view_t *newView) {
    if (currentView != NULL) {
        ui_manager_push_view_to_history(currentView);
    }
    currentView = newView;

    if(currentView && currentView->componentCount > 0) {
        currentView->focusedComponent = 0;
        currentView->components[0]->isFocused = true;
    }
    erase();
}

/**
 * \fn void ui_manager_go_back()
 * @brief Permet de revenir à la vue précédente en utilisant l'historique des vues
 * @note Si l'historique est vide, cette fonction ne fait rien
 */
void ui_manager_go_back() {
	ui_view_t *previousView = ui_manager_pop_view_from_history();
	if(previousView) {
		DESTROY_CURRENT_VIEW();
		currentView = previousView;
		erase();
	}
}

/**
 * \fn void ui_manager_handle_event(event_t *event)
 * \brief Gère les événements reçus par l'interface graphique et les redirige vers la vue actuelle
 * \param event L'événement à gérer
 */
void ui_manager_handle_event(event_t *event) {
	if(!event || !currentView) return;

	if(currentView->handle_event) {
		currentView->handle_event(currentView, event);
	}
}

/**
 * \fn void ui_manager_draw()
 * \brief Dessine la vue actuelle de l'interface graphique
 */
void ui_manager_draw(void) {
	if(currentView && currentView->draw) {
		currentView->draw(currentView);
	}
	doupdate();
}