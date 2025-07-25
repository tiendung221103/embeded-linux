#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Shared resource
int resource = 0;

// Read-write lock for synchronizing access
pthread_rwlock_t rwlock;

/**
 * @brief Reader thread function.
 * 
 * Continuously acquires a read lock, reads the shared resource,
 * prints it, then releases the lock.
 * 
 * @param [in] arg Pointer to an integer representing the reader's ID.
 * @return NULL
 */
void* reader(void* arg) {
    int id = *(int*)arg;
    while (1) {
        pthread_rwlock_rdlock(&rwlock); 
        printf("Reader %d read resource: %d\n", id, resource);
        pthread_rwlock_unlock(&rwlock);
        sleep(1); // Simulate read interval
    }
    return NULL;
}

/**
 * @brief Writer thread function.
 * 
 * Continuously acquires a write lock, increments the shared resource,
 * prints it, then releases the lock.
 * 
 * @param [in] arg Pointer to an integer representing the writer's ID.
 * @return NULL
 */
void* writer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        pthread_rwlock_wrlock(&rwlock);
        resource++;
        printf("Writer %d wrote resource: %d\n", id, resource);
        pthread_rwlock_unlock(&rwlock);
        sleep(2); // Simulate write interval
    }
    return NULL;
}

int main() {
    pthread_t rtid[5], wtid[2];
    int rids[5] = {1, 2, 3, 4, 5};
    int wids[2] = {1, 2};

    pthread_rwlock_init(&rwlock, NULL);

    // Create reader threads
    for (int i = 0; i < 5; i++) {
        pthread_create(&rtid[i], NULL, reader, &rids[i]);
    }

    // Create writer threads
    for (int i = 0; i < 2; i++) {
        pthread_create(&wtid[i], NULL, writer, &wids[i]);
    }

    // Join threads (these will run infinitely)
    for (int i = 0; i < 5; i++) {
        pthread_join(rtid[i], NULL);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(wtid[i], NULL);
    }

    return 0;
}
