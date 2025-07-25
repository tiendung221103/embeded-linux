#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

// Shared data and state flag
int data = 0;
bool data_ready = false;  // Flag to indicate if new data is available

// Synchronization primitives
pthread_mutex_t mutex;
pthread_cond_t cond;

/**
 * @brief Producer thread function.
 *
 * Produces 10 random integers and notifies the consumer each time data is ready.
 *
 * @param [in] arg Unused.
 * @return NULL
 */
static void* producer(void* arg) {
    for (int i = 0; i < 10; i++) {
        int new_data = rand() % 100;  // Generate random data (0–99)

        pthread_mutex_lock(&mutex);
        data = new_data;
        data_ready = true;
        printf("Producer produced data: %d\n", data);

        pthread_cond_signal(&cond);  // Signal the consumer
        pthread_mutex_unlock(&mutex);

        sleep(1);  // Simulate time delay for production
    }

    pthread_exit(NULL);
}

/**
 * @brief Consumer thread function.
 *
 * Waits for data to be available and consumes it 10 times.
 *
 * @param [in] arg Unused.
 * @return NULL
 */
static void* consumer(void* arg) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);

        // Wait for the producer to signal that data is ready
        while (!data_ready) {
            pthread_cond_wait(&cond, &mutex);
        }

        printf("Consumer received data: %d\n", data);
        data_ready = false;  // Reset the flag after consuming

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));  // Seed for random number generator

    pthread_t t_producer, t_consumer;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&t_producer, NULL, producer, NULL);
    pthread_create(&t_consumer, NULL, consumer, NULL);

    pthread_join(t_producer, NULL);
    pthread_join(t_consumer, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
