/**
 * @file ui_view.test.c
 * @brief Tests unitaires pour la gestion des vues UI (ui_view.c)
 */

#include "unity.h"
#include "ui/base/ui_view.h"
#include <stdlib.h>
#include <stdbool.h>

static int mock_view_draw_called = 0;
static int mock_view_handle_event_called = 0;
static int mock_view_destroy_called = 0;
static int mock_component_destroy_called = 0;

void mock_view_draw(ui_view_t *self) {
    mock_view_draw_called++;
}

bool mock_view_handle_event(ui_view_t *self, event_t *event) {
    mock_view_handle_event_called++;
    return true;
}

void mock_view_destroy(ui_view_t *self) {
    mock_view_destroy_called++;
    if (self) {
        free(self->components);
        free(self);
    }
}

// -- Mocks pour les composants --

void mock_component_destroy(ui_component_t *self) {
    mock_component_destroy_called++;
}

void setUp(void) {
    mock_view_draw_called = 0;
    mock_view_handle_event_called = 0;
    mock_view_destroy_called = 0;
    mock_component_destroy_called = 0;
}

void tearDown(void) {
}

/**
 * @brief Teste la création d'une vue (initialisation des pointeurs et compteurs)
 */
void test_ui_view_create(void) {
    int dummy_data = 42;
    
    ui_view_t *view = ui_view_create(mock_view_draw, mock_view_handle_event, mock_view_destroy, &dummy_data);
    
    TEST_ASSERT_NOT_NULL(view);
    TEST_ASSERT_EQUAL_INT(0, view->componentCount);
    TEST_ASSERT_NULL(view->components);
    TEST_ASSERT_EQUAL_INT(0, view->focusedComponent);
    TEST_ASSERT_EQUAL_PTR(&dummy_data, view->data);
    
    TEST_ASSERT_EQUAL_PTR(mock_view_draw, view->draw);
    TEST_ASSERT_EQUAL_PTR(mock_view_handle_event, view->handle_event);
    TEST_ASSERT_EQUAL_PTR(mock_view_destroy, view->destroy);

    view->destroy(view);
}

/**
 * @brief Teste l'ajout de composants et l'agrandissement du tableau
 */
void test_ui_view_add_component(void) {
    ui_view_t *view = ui_view_create(NULL, NULL, mock_view_destroy, NULL);
    ui_component_t comp1 = {0};
    ui_component_t comp2 = {0};

    TEST_ASSERT_EQUAL_INT(-1, ui_view_add_component(NULL, &comp1));
    TEST_ASSERT_EQUAL_INT(-1, ui_view_add_component(view, NULL));

    int idx1 = ui_view_add_component(view, &comp1);

	TEST_ASSERT_EQUAL_INT(0, idx1);
    TEST_ASSERT_EQUAL_INT(1, view->componentCount);
    TEST_ASSERT_EQUAL_PTR(&comp1, view->components[0]);

    int idx2 = ui_view_add_component(view, &comp2);

    TEST_ASSERT_EQUAL_INT(1, idx2);
    TEST_ASSERT_EQUAL_INT(2, view->componentCount);
    TEST_ASSERT_EQUAL_PTR(&comp2, view->components[1]);

    view->destroy(view);
}

/**
 * @brief Teste la suppression de composants (décalage et destruction automatique)
 */
void test_ui_view_remove_component(void) {
    ui_view_t *view = ui_view_create(NULL, NULL, mock_view_destroy, NULL);
    
    ui_component_t comp1 = { .destroy = NULL };
    ui_component_t comp2 = { .destroy = mock_component_destroy };
    ui_component_t comp3 = { .destroy = NULL };

    ui_view_add_component(view, &comp1);
    ui_view_add_component(view, &comp2);
    ui_view_add_component(view, &comp3);

    TEST_ASSERT_EQUAL_INT(-1, ui_view_remove_component(view, -1));
    TEST_ASSERT_EQUAL_INT(-1, ui_view_remove_component(view, 3));
    TEST_ASSERT_EQUAL_INT(-1, ui_view_remove_component(NULL, 0));

    int removed_idx = ui_view_remove_component(view, 1);
    
	TEST_ASSERT_EQUAL_INT(1, removed_idx);
    TEST_ASSERT_EQUAL_INT(2, view->componentCount);
    TEST_ASSERT_EQUAL_INT(1, mock_component_destroy_called); 
    TEST_ASSERT_EQUAL_PTR(&comp3, view->components[1]);

    ui_view_remove_component(view, 1);

    TEST_ASSERT_EQUAL_INT(1, view->componentCount);
    TEST_ASSERT_EQUAL_PTR(&comp1, view->components[0]);

    ui_view_remove_component(view, 0);

    TEST_ASSERT_EQUAL_INT(0, view->componentCount);
    TEST_ASSERT_NULL(view->components);

    view->destroy(view);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_ui_view_create);
    RUN_TEST(test_ui_view_add_component);
    RUN_TEST(test_ui_view_remove_component);

    return UNITY_END();
}