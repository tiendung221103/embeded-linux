#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <time.h>

#define BUFFER_SIZE 10

// Item types
#define EMPTY 0
#define VEGAN 1
#define NON_VEGAN 2

/**
 * @brief Shared structure representing a food tray buffer
 *
 * Contains an array of items and three semaphores for synchronization:
 * - mutex: to ensure mutual exclusion
 * - full: counts the number of filled slots
 * - empty: counts the number of empty slots
 */
typedef struct {
    int items[BUFFER_SIZE];
    sem_t mutex;
    sem_t full;
    sem_t empty;
} SharedBuffer;

/**
 * @brief Print the current number of items in each tray
 *
 * @param[in] vegan     Pointer to the vegan dish buffer
 * @param[in] non_vegan Pointer to the non-vegan dish buffer
 * @return None
 */
void print_buffer_state(SharedBuffer* vegan, SharedBuffer* non_vegan) {
    int vegan_count = 0, non_vegan_count = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (vegan->items[i] != EMPTY) vegan_count++;
        if (non_vegan->items[i] != EMPTY) non_vegan_count++;
    }
    printf("[STATUS] Vegan tray: %d/10 | Non-vegan tray: %d/10\n", vegan_count, non_vegan_count);
}

/**
 * @brief Add a dish to the given buffer
 *
 * @param[in] buffer     Pointer to the target buffer
 * @param[in] item_type  Type of dish (VEGAN or NON_VEGAN)
 * @param[in] chef_name  Name of the chef adding the dish
 * @param[in] dish_name  Name of the dish
 * @return None
 */
void add_item(SharedBuffer* buffer, int item_type, const char* chef_name, const char* dish_name) {
    sem_wait(&buffer->empty);
    sem_wait(&buffer->mutex);

    // Add item to buffer
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (buffer->items[i] == EMPTY) {
            buffer->items[i] = item_type;
            break;
        }
    }

    printf("[CHEF] %s added \"%s\"\n", chef_name, dish_name);

    sem_post(&buffer->mutex);
    sem_post(&buffer->full);
}

/**
 * @brief Remove a dish from the given buffer
 *
 * @param[in] buffer        Pointer to the target buffer
 * @param[in] item_type     Type of dish to remove (VEGAN or NON_VEGAN)
 * @param[in] customer_name Name of the customer taking the dish
 * @return None
 */
void remove_item(SharedBuffer* buffer, int item_type, const char* customer_name) {
    sem_wait(&buffer->full);
    sem_wait(&buffer->mutex);

    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (buffer->items[i] == item_type) {
            buffer->items[i] = EMPTY;
            break;
        }
    }

    printf("[CUSTOMER] %s took a %s dish\n", customer_name, (item_type == VEGAN) ? "vegan" : "non-vegan");

    sem_post(&buffer->mutex);
    sem_post(&buffer->empty);
}

/**
 * @brief Routine for chef Donatello (prepares non-vegan dishes)
 *
 * @param[in] non_vegan Pointer to the non-vegan dish buffer
 * @return None
 */
void chef_donatello(SharedBuffer* non_vegan) {
    const char* dishes[] = {"Fettuccine Chicken Alfredo", "Garlic Sirloin Steak"};
    srand(getpid());

    while (1) {
        int idx = rand() % 2;
        int delay = 1 + rand() % 5;
        sleep(delay);
        add_item(non_vegan, NON_VEGAN, "Donatello", dishes[idx]);
    }
}

/**
 * @brief Routine for chef Portecelli (prepares vegan dishes)
 *
 * @param[in] vegan Pointer to the vegan dish buffer
 * @return None
 */
void chef_portecelli(SharedBuffer* vegan) {
    const char* dishes[] = {"Pistachio Pesto Pasta", "Avocado Fruit Salad"};
    srand(getpid());

    while (1) {
        int idx = rand() % 2;
        int delay = 1 + rand() % 5;
        sleep(delay);
        add_item(vegan, VEGAN, "Portecelli", dishes[idx]);
    }
}

/**
 * @brief Behavior of customer 1 (takes only non-vegan dishes)
 *
 * @param[in] non_vegan Pointer to the non-vegan dish buffer
 * @return None
 */
void customer1(SharedBuffer* non_vegan) {
    srand(getpid());
    while (1) {
        remove_item(non_vegan, NON_VEGAN, "Customer 1");
        sleep(10 + rand() % 6);
    }
}

/**
 * @brief Behavior of customer 2 (takes only vegan dishes)
 *
 * @param[in] vegan Pointer to the vegan dish buffer
 * @return None
 */
void customer2(SharedBuffer* vegan) {
    srand(getpid());
    while (1) {
        remove_item(vegan, VEGAN, "Customer 2");
        sleep(10 + rand() % 6);
    }
}

/**
 * @brief Behavior of customer 3 (takes both vegan and non-vegan dishes)
 *
 * @param[in] vegan     Pointer to the vegan dish buffer
 * @param[in] non_vegan Pointer to the non-vegan dish buffer
 * @return None
 */
void customer3(SharedBuffer* vegan, SharedBuffer* non_vegan) {
    srand(getpid());
    while (1) {
        remove_item(vegan, VEGAN, "Customer 3");
        sleep(10 + rand() % 6);
        remove_item(non_vegan, NON_VEGAN, "Customer 3");
        sleep(10 + rand() % 6);
    }
}

/**
 * @brief Main entry point of the program
 *
 * Initializes shared memory, semaphores, and forks chef and customer processes.
 * The parent process periodically logs tray status.
 *
 * @return 0 on successful execution
 */
int main() {
    // Create shared memory for buffers
    SharedBuffer* vegan_buffer = mmap(NULL, sizeof(SharedBuffer), PROT_READ | PROT_WRITE,
                                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    SharedBuffer* non_vegan_buffer = mmap(NULL, sizeof(SharedBuffer), PROT_READ | PROT_WRITE,
                                          MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // Initialize buffers
    for (int i = 0; i < BUFFER_SIZE; i++) {
        vegan_buffer->items[i] = EMPTY;
        non_vegan_buffer->items[i] = EMPTY;
    }

    // Initialize semaphores
    sem_init(&vegan_buffer->mutex, 1, 1);
    sem_init(&vegan_buffer->empty, 1, BUFFER_SIZE);
    sem_init(&vegan_buffer->full, 1, 0);

    sem_init(&non_vegan_buffer->mutex, 1, 1);
    sem_init(&non_vegan_buffer->empty, 1, BUFFER_SIZE);
    sem_init(&non_vegan_buffer->full, 1, 0);

    pid_t pids[5];

    // Create chef processes
    if ((pids[0] = fork()) == 0) {
        chef_donatello(non_vegan_buffer);
        exit(0);
    }
    if ((pids[1] = fork()) == 0) {
        chef_portecelli(vegan_buffer);
        exit(0);
    }

    // Create customer processes
    if ((pids[2] = fork()) == 0) {
        customer1(non_vegan_buffer);
        exit(0);
    }
    if ((pids[3] = fork()) == 0) {
        customer2(vegan_buffer);
        exit(0);
    }
    if ((pids[4] = fork()) == 0) {
        customer3(vegan_buffer, non_vegan_buffer);
        exit(0);
    }

    // Parent process logs tray status every 10 seconds
    while (1) {
        sleep(10);
        print_buffer_state(vegan_buffer, non_vegan_buffer);
    }

    return 0;
}
