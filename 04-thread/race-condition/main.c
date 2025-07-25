#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3
#define INCREMENTS 1000000

// Shared global variable
int counter = 0;

// Mutex for synchronizing access to 'counter'
pthread_mutex_t counter_mutex;

/**
 * @brief Thread function to increment the shared counter.
 *
 * Each thread performs INCREMENTS increments on the shared counter.
 * Access is protected by a mutex to avoid race conditions.
 *
 * @param [in] arg Unused in this example (can be NULL).
 * @return NULL Always returns NULL.
 */
void* increment_counter(void* arg) {
    for (int i = 0; i < INCREMENTS; i++) {
        pthread_mutex_lock(&counter_mutex);   // Lock before modifying shared data
        counter++;
        pthread_mutex_unlock(&counter_mutex); // Unlock immediately after modification
    }
    return NULL;
}

/**
 * @brief Main function that creates multiple threads to increment a shared counter.
 *
 * Demonstrates correct synchronization using a mutex to avoid race conditions.
 *
 * @return 0 on success, 1 on thread creation failure.
 */
int main() {
    pthread_t threads[NUM_THREADS];

    // Initialize the mutex
    pthread_mutex_init(&counter_mutex, NULL);

    // Create NUM_THREADS threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, increment_counter, NULL) != 0) {
            perror("Error creating thread");
            exit(1);
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print expected and actual counter values
    printf("Expected counter value: %d\n", NUM_THREADS * INCREMENTS);
    printf("Actual counter value  : %d\n", counter);

    return 0;
}
