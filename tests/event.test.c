#include "unity.h"
#include "event/event.h"
#include <stdlib.h>

static int custom_data_destroyed = 0;

void dummy_destroy_data(void *data) {
    if (data != NULL) {
        free(data);
        custom_data_destroyed = 1;
    }
}

void setUp(void) {
    custom_data_destroyed = 0;
}

void tearDown(void) {
}

void test_create_event_no_data(void) {
    event_t *event = create_event(UI_EVENT_VIEW_CHANGED, NULL, NULL);
    
    TEST_ASSERT_NOT_NULL(event);
    TEST_ASSERT_EQUAL(UI_EVENT_VIEW_CHANGED, event->type);
    TEST_ASSERT_NULL(event->data);
    TEST_ASSERT_NULL(event->destroy_data);
    
    destroy_event(event);
}

void test_create_and_destroy_event_with_data(void) {
    int *my_data = (int*)malloc(sizeof(int));
    *my_data = 42;
    
    event_t *event = create_event(LOGIC_EVENT_NOTE_PLAYED, my_data, dummy_destroy_data);
    
    TEST_ASSERT_NOT_NULL(event);
    TEST_ASSERT_EQUAL(LOGIC_EVENT_NOTE_PLAYED, event->type);
    TEST_ASSERT_EQUAL_PTR(my_data, event->data);
    
    destroy_event(event);
    
    TEST_ASSERT_EQUAL_INT(1, custom_data_destroyed);
}

void test_event_type2str(void) {	
    TEST_ASSERT_EQUAL_STRING("UI_NONE", event_type2str(UI_EVENT_NONE));
	TEST_ASSERT_EQUAL_STRING("UI_REQUEST_SUBMITTED", event_type2str(UI_EVENT_REQUEST_SUBMITTED));
	TEST_ASSERT_EQUAL_STRING("UI_VIEW_CHANGED", event_type2str(UI_EVENT_VIEW_CHANGED));
	TEST_ASSERT_EQUAL_STRING("UI_MUSICPLAYBACK_STARTED", event_type2str(UI_EVENT_MUSICPLAYBACK_STARTED));
	TEST_ASSERT_EQUAL_STRING("UI_MUSICPLAYBACK_STOPPED", event_type2str(UI_EVENT_MUSICPLAYBACK_STOPPED));
	TEST_ASSERT_EQUAL_STRING("UI_EVENT_NOTE_PLAYED", event_type2str(UI_EVENT_NOTE_PLAYED));
	
	TEST_ASSERT_EQUAL_STRING("LOGIC_NONE", event_type2str(LOGIC_EVENT_NONE));
    TEST_ASSERT_EQUAL_STRING("LOGIC_MUSIC_SAVED", event_type2str(LOGIC_EVENT_MUSIC_SAVED));
	TEST_ASSERT_EQUAL_STRING("LOGIC_SOUNDCARD_ERROR", event_type2str(LOGIC_EVENT_SOUNDCARD_ERROR));
	TEST_ASSERT_EQUAL_STRING("LOGIC_AUTOSAVE_STARTED", event_type2str(LOGIC_EVENT_AUTOSAVE_STARTED));
	TEST_ASSERT_EQUAL_STRING("LOGIC_REQUEST_FAILED", event_type2str(LOGIC_EVENT_REQUEST_FAILED));


    TEST_ASSERT_EQUAL_STRING("INET_ERROR", event_type2str(INET_ERROR));
	TEST_ASSERT_EQUAL_STRING("INET_RESPONSE_RECEIVED", event_type2str(INET_RESPONSE_RECEIVED));
	TEST_ASSERT_EQUAL_STRING("INET_REQUEST_SENT", event_type2str(INET_REQUEST_SENT));
	TEST_ASSERT_EQUAL_STRING("INET_ERROR", event_type2str(INET_ERROR));

    TEST_ASSERT_EQUAL_STRING("EVENT_UNKNOWN", event_type2str((event_type_t)999));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_event_no_data);
    RUN_TEST(test_create_and_destroy_event_with_data);
    RUN_TEST(test_event_type2str);
    return UNITY_END();
}