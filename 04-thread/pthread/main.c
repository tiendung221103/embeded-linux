#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/**
 * @brief Function executed by each thread.
 * 
 * @param [in] arg A pointer passed to the thread, expected to be castable to a long (thread ID).
 * @return void* Always returns NULL in this example.
 */
static void* thread_function(void* arg) {
    long thread_id = (long)arg;
    printf("Thread with ID %ld is running!\n", thread_id);

}

/**
 * @brief Main function that creates and joins two threads.
 * 
 * Each thread prints its own ID. The main thread waits for both threads to finish
 * before exiting the program.
 * 
 * @return 0 on successful execution, 1 if thread creation fails.
 */
int main() {
    pthread_t threads[2];

    // Create two threads
    for (long i = 0; i < 2; i++) {
        int rc = pthread_create(&threads[i], NULL, thread_function, (void*)i);
        if (rc != 0) {
            fprintf(stderr, "Error: Unable to create thread %ld\n", i);
            exit(1);
        }
    }

    // Wait for both threads to finish
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Both threads have finished. Program completed.\n");
    return 0;
}
