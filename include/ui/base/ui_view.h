/**
 * \file ui_view.h
 * \brief Définition d'une vue écran contenant plusieurs composants
 * \author Lukas Grando
 */

#ifndef UI_VIEW_H
#define UI_VIEW_H

#include "ui/base/ui_component.h"
#include "event/event.h"

typedef struct __ui_view_t ui_view_t;

typedef bool (*ui_view_handle_event_fn)(ui_view_t *self, event_t *event);
typedef void (*ui_view_destroy_fn)(ui_view_t *self);
typedef void (*ui_view_draw_fn)(ui_view_t *self);

/**
 * \struct __ui_view_t
 * \brief Structure pour une vue de l'interface utilisateur
 */
struct __ui_view_t {
	void *data;

	ui_component_t **components; //!< Les composants de la vue
	int componentCount; //!< Le nombre de composants
	int focusedComponent; //!< L'index du composant actuellement focus

	ui_view_handle_event_fn handle_event; //!< La fonction pour gérer les événements de la vue*
	ui_view_destroy_fn destroy; //!< La fonction pour détruire la vue
	ui_view_draw_fn draw; //!< La fonction pour dessiner la vue
};

/**
 * \brief Crée une vue de l'interface utilisateur
 * \return ui_view_t* La vue créée
 * \note La vue doit être détruite après utilisation
 */
ui_view_t *ui_view_create(ui_view_draw_fn drawFunc, ui_view_handle_event_fn handleEventFunc, ui_view_destroy_fn destroyFunc, void *data);

/**
 * \brief Ajoute un composant à une vue de l'interface utilisateur
 * \param view La vue à laquelle ajouter le composant
 * \param component Le composant à ajouter
 * \return l'index du composant ajouté dans la vue, ou -1 en cas d'erreur
 * \note La vue doit être redessinée après l'ajout du composant
 */
int ui_view_add_component(ui_view_t *view, ui_component_t *component);

/**
 * \brief Supprime un composant d'une vue de l'interface utilisateur
 * \param view La vue de laquelle supprimer le composant
 * \param index L'index du composant à supprimer
 * \return -1 en cas d'erreur, sinon l'index du composant supprimé
 * \note La vue doit être redessinée après la suppression du composant
 */
int ui_view_remove_component(ui_view_t *view, int index);

#endif // UI_VIEW_H