/**
 * @file ui_component.h
 * @brief Définition des composants de l'interface utilisateur
 * @details Définit la structure d'un composant pour générer une interface événementielle
 * @author Lukas Grando
 */

#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include "stdbool.h"
#include "ui_common.h"
#include "event/event.h"


typedef struct __ui_component_t ui_component_t;

/**
 * @fn draw_func_t
 * @brief Type de fonction pour dessiner un composant de l'interface utilisateur
 * @param self Le composant à dessiner
 * @return void
 */
typedef void (*ui_component_draw_fn)(ui_component_t *self);

/**
 * @fn handle_event_func_t
 * @brief Type de fonction pour consommer les événements d'un composant de l'interface utilisateur
 * @param self Le composant à consommer
 * @param event L'événement à consommer
 * @return bool true si l'événement a été consommé, false sinon
 */
typedef bool (*ui_component_handle_event_fn)(ui_component_t *self, event_t *event); 

/**
 * @fn destroy_func_t
 * @brief Type de fonction pour détruire un composant de l'interface utilisateur
 * @param self Le composant à détruire
 * @return void
 */
typedef void (*ui_component_destroy_fn)(ui_component_t *self);


/**
 * @struct ui_component_t
 * @brief Structure pour un composant de l'interface utilisateur
 */
struct __ui_component_t {
	void *data; 
	bool isFocused;
	bool isFocusable;
	ui_component_draw_fn draw;
	ui_component_handle_event_fn handle_event;
	ui_component_destroy_fn destroy;
};

#endif // UI_COMPONENT_H