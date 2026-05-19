/**
 * \file ui_view.c
 * \brief Fichier source pour les vues de l'interface graphique
 * \version 1.0
 * \author Lukas Grando
 * \see ui_view.h
 */

#include "ui/base/ui_view.h"

/**
 * \brief Crée une vue de l'interface utilisateur
 * \return ui_view_t* La vue créée
 * \note La vue doit être détruite après utilisation
 */
ui_view_t *ui_view_create(ui_view_draw_fn drawFunc, ui_view_handle_event_fn handleEventFunc, ui_view_destroy_fn destroyFunc, void *data) {
	ui_view_t *view = (ui_view_t *)malloc(sizeof(ui_view_t));
	CHECK_ALLOC(view);
	*view = (ui_view_t) {
		.componentCount = 0,
		.components = NULL,
		.data = data,
		.destroy = destroyFunc,
		.handle_event = handleEventFunc,
		.draw = drawFunc,
		.focusedComponent = 0,
	};
	return view;
}

/**
 * \brief Ajoute un composant à une vue de l'interface utilisateur
 * \param view La vue à laquelle ajouter le composant
 * \param component Le composant à ajouter
 * \return l'index du composant ajouté dans la vue, ou -1 en cas d'erreur
 * \note La vue doit être redessinée après l'ajout du composant
 */
int ui_view_add_component(ui_view_t *view, ui_component_t *component) {
	if(!view || !component) return -1;
	int newCount = view->componentCount + 1;
	ui_component_t **newComponents = (ui_component_t **)realloc(view->components, newCount * sizeof(ui_component_t *));
	CHECK_ALLOC(newComponents);
	view->components = newComponents;
	view->components[view->componentCount] = component;
	view->componentCount = newCount;
	return view->componentCount - 1;
}

/**
 * \brief Supprime un composant d'une vue de l'interface utilisateur
 * \param view La vue de laquelle supprimer le composant
 * \param index L'index du composant à supprimer
 * \return -1 en cas d'erreur, sinon l'index du composant supprimé
 * \note La vue doit être redessinée après la suppression du composant
 */
int ui_view_remove_component(ui_view_t *view, int index) {
	if(!view || index < 0 || index >= view->componentCount) return -1;
	ui_component_t *component = view->components[index];
	if(component && component->destroy) {
		component->destroy(component);
	}
	for(int i = index; i < view->componentCount - 1; i++) {
		view->components[i] = view->components[i + 1];
	}
	view->componentCount--;
	if(view->componentCount == 0) {
		free(view->components);
		view->components = NULL;
	} else {
		ui_component_t **newComponents = (ui_component_t **)realloc(view->components, view->componentCount * sizeof(ui_component_t *));
		CHECK_ALLOC(newComponents);
		view->components = newComponents;
	}
	return index;
}