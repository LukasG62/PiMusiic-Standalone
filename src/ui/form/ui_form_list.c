/**
 * @file ui_form_list.h
 * @brief Fichier d'en-tête pour les listes interactives de l'interface graphique
 * @version 1.0
 */

 #include "ui/form/ui_form_list.h"

 typedef struct {
	WINDOW *win; /*!< La fenêtre de la liste */
	
	const char **labels; /*!< Les étiquettes des éléments de la liste */
	void **values; /*!< Les valeurs associées à chaque élément de la liste */
	int count; /*!< Le nombre d'éléments dans la liste */
	
	int start; /*!< L'index du premier élément affiché dans la liste (pour le scroll) */
	int selected; /*!< L'index de l'élément actuellement sélectionné */
	
	ui_form_list_on_select_fn on_select; /*!< La fonction de rappel à appeler quand un élément est sélectionné */
	void *context; /*!< Le contexte à passer à la fonction de rappel */
} ui_form_list_data_t;

/**
 * \fn ui_list_draw(ui_component_t *self)
 * \brief Fonction de dessin pour le composant de liste
 * \param self Le composant de liste à dessiner
 */
static void ui_list_draw(ui_component_t *self);

/**
 * \fn ui_list_handle_event(ui_component_t *self, event_t *event)
 * \brief Fonction de gestion des événements pour le composant de liste
 * \param self Le composant de liste qui reçoit l'événement
 * \param event L'événement à gérer
 * @return bool true si l'événement a été traité, false sinon
 */
static bool ui_list_handle_event(ui_component_t *self, event_t *event);

static void ui_list_destroy(ui_component_t *self);

/**
 * \fn ui_form_list_create(WINDOW *parent, int x, int y, int width, int height)
 * \brief Crée un composant de liste interactive
 * \param parent la fenêtre sur laquelle créer la liste
 * \param x la position x de la liste
 * \param y la position y de la liste
 * \param width la largeur de la liste
 * \param height la hauteur de la liste
 * @return ui_component_t* Le composant de liste créée
 */
ui_component_t *ui_form_list_create(WINDOW *parent, int x, int y, int width, int height) {
	ui_component_t *comp = malloc(sizeof(ui_component_t));
	CHECK_ALLOC(comp);
	ui_form_list_data_t *data = malloc(sizeof(ui_form_list_data_t));
	CHECK_ALLOC(data);
	
	*data = (ui_form_list_data_t){
		.win = derwin(parent, height, width, y, x),
		.labels = NULL,
		.values = NULL,
		.count = 0,
		.start = 0,
		.selected = 0,
		.on_select = NULL,
		.context = NULL
	};

	*comp = (ui_component_t){
		.data = data,
		.isFocused = false,
		.draw = ui_list_draw,
		.handle_event = ui_list_handle_event,
		.destroy = ui_list_destroy
	};

	return comp;
}

/**
 * \fn void ui_form_list_set_items(ui_component_t *comp, const char **labels, void **values, int count)
 * \brief Assigne les éléments et leurs valeurs de destination à la liste
 * \param labels Tableau de chaînes de caractères (ce qui est affiché)
 * \param values Tableau de pointeurs vers les valeurs associées (ex: des pointeurs vers enum app_view_t)
 */
void ui_form_list_set_items(ui_component_t *comp, const char **labels, void **values, int count) {
	if(!comp || !comp->data) return;
	ui_form_list_data_t *listData = (ui_form_list_data_t*)comp->data;

	listData->labels = labels;
	listData->values = values;
	listData->count = count;
}

/**
 * \fn void ui_form_list_set_on_select(ui_component_t *comp, ui_form_list_on_select_fn callback, void *context)
 * \brief Définit l'action à exécuter quand on appuie sur ENTER
 */
void ui_form_list_set_on_select(ui_component_t *comp, ui_form_list_on_select_fn callback, void *context) {
	if(!comp || !comp->data) return;
	ui_form_list_data_t *listData = (ui_form_list_data_t*)comp->data;

	listData->on_select = callback;
	listData->context = context;
}

static void ui_list_draw(ui_component_t *self) {
	if(!self || !self->data) return;
	ui_form_list_data_t *listData = (ui_form_list_data_t*)self->data;

	int maxItems = getmaxy(listData->win);
	int end = (listData->count > maxItems) ? listData->start + maxItems : listData->count;

	wclear(listData->win);
	for(int i = listData->start; i < end; i++) {
		if(i == listData->selected && self->isFocused) {
			wattron(listData->win, A_REVERSE);
		}
		mvwprintw(listData->win, i - listData->start, 0, "[%d] %s", i + 1, listData->labels[i]);
		if(i == listData->selected && self->isFocused) {
			wattroff(listData->win, A_REVERSE);
		}
	}
	if(end < listData->count) {
		mvwprintw(listData->win, maxItems - 1, 0, "...");	
	}
	wnoutrefresh(listData->win);
}

static bool ui_list_handle_event(ui_component_t *self, event_t *event) {
	if(!self || !self->data || !event) return false;
	if(event->type != UI_EVENT_KEY_PRESSED) return false;

	ui_form_list_data_t *listData = (ui_form_list_data_t*)self->data;
	int key = (int)(intptr_t)event->data;
	int maxItems = getmaxy(listData->win);
	int end = listData->start + maxItems;

	if(key == KEY_MOUSE) {
		MEVENT mouseEvent;
		if(getmouse(&mouseEvent) == OK) {
			if(mouseEvent.bstate & BUTTON1_CLICKED) {
				key = KEY_ENTER;
			}
			else if(mouseEvent.bstate & BUTTON4_PRESSED) {
				key = KEY_UP;
			}
			else if(mouseEvent.bstate & BUTTON5_PRESSED) {
				key = KEY_DOWN;
			}
			else if (mouseEvent.bstate & BUTTON2_CLICKED) {
				key = 27;
			}
		}
	}

	switch(key) {
		case KEY_UP:
			if(listData->selected > listData->start) listData->selected--;
			else if(listData->start > 0) {
				listData->start--;
				listData->selected--;
			}
		return true;

		case KEY_DOWN:
			if(listData->selected < end -1 && listData->selected < listData->count - 1) {
				listData->selected++;
			} else if(end < listData->count) {
				listData->start++;
				listData->selected++;
			}
		return true;

		case 27:
			// todo: gestion de l'annulation via une fonction de callback
		return false;

		case '\n':
		case '\r':
		case KEY_ENTER:
			if(listData->on_select && listData->selected < listData->count) {
				listData->on_select(listData->context, listData->values[listData->selected]);
			}
		return true;

		default:
			return false;
	}
}

static void ui_list_destroy(ui_component_t *self) {
	if(!self && !self->data) return;
	
	ui_form_list_data_t *listData = (ui_form_list_data_t*)self->data;
	if(listData->win) delwin(listData->win);
	free(listData);
	free(self);

}