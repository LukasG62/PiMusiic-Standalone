#include <assert.h>
#include <unistd.h>
#include "event/event_queue.h"
#include "event/event.h"
#include "common.h"
#include <time.h>

// Structurize thread data
typedef struct {
    event_queue_t *sharedQueue;
    size_t threadId;
} thread_data_t;

// Simulating the event receiver (thread_trywait_event)
void *thread_trywait_event(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int event_count = 0;

    while (event_count < 10) {  // Receive 3 events for test purpose
		DEBUG_PRINT("[%ld] Waiting for event\n", data->threadId);
        //event_t *event = trywait_event(data->sharedQueue);
        event_t *event = wait_event(data->sharedQueue);
        if (event != NULL) {
            DEBUG_PRINT("[%ld] Event received: %s\n", data->threadId, event_type2str(event->type));
            destroy_event(event);
            event_count++;
        }
        else {
		    DEBUG_PRINT("[%ld] No event received\n", data->threadId);
        }
        //sleep(rand() % 3);  // Sleep for 0-2 seconds to simulate random event processing
    }

    return NULL;
}

// Simulating the event sender (thread_notify_event)
void *thread_notify_event(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int event_count = 0;
    while (event_count < 10) {  // Send 3 events for test purpose
        event_t *event = create_event(LOGIC_EVENT_REQUEST_SENT, NULL, NULL);
		DEBUG_PRINT("[%ld] Notifying event\n", data->threadId);
        notify_event(data->sharedQueue, event);
        event_count++;
        sleep(1 * (rand() % 3 + 1));  // Sleep for 1-3 seconds to simulate random event generation
    }

    return NULL;
}

void test_event_queue() {
    srand(time(NULL));
    thread_data_t data1, data2;
	DEBUG_PRINT("Creating shared queue\n");
    // Create a shared event queue
    data1.sharedQueue = create_event_queue();
    data2.sharedQueue = data1.sharedQueue;
    data1.threadId = 1;
    data2.threadId = 2;

    pthread_t sender_thread, receiver_thread = 0;
	DEBUG_PRINT("Creating threads\n");
    // Create threads for sending and receiving events
    pthread_create(&sender_thread, NULL, thread_notify_event, &data1);
    pthread_create(&receiver_thread, NULL, thread_trywait_event, &data2);
	
    // Wait for the threads to finish
    pthread_join(sender_thread, NULL);
    pthread_join(receiver_thread, NULL);

	DEBUG_PRINT("Destroying shared queue\n");
    destroy_event_queue(data1.sharedQueue);

    DEBUG_PRINT("Test Finished\n");
}

int main() {
	DEBUG_PRINT("Running event queue test\n");
    // Run the event queue unit test
    test_event_queue();
    return 0;
}