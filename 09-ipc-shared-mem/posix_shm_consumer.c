#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define NAME_SIZE 64
#define SHM_NAME "/my_shared_mem"

/**
 * @brief Structure that matches the one written to shared memory.
 */
typedef struct {
    int id;
    char name[NAME_SIZE];
    float price;
} Product;

/**
 * @brief Main function that reads data from shared memory created by another process.
 *
 * This program:
 * - Opens an existing shared memory region (read-only).
 * - Maps it into the process's address space.
 * - Reads and prints the stored product data.
 * - Unmaps and optionally unlinks the shared memory.
 *
 * @return int Returns EXIT_SUCCESS (0) on success, or EXIT_FAILURE (1) on error.
 */
int main() {
    // 1. Open the existing shared memory object in read-only mode
    int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // 2. Map the shared memory into this process (read-only)
    Product* ptr = mmap(NULL, sizeof(Product), PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // 3. Read the data from shared memory
    printf("Read from shared memory:\n");
    printf("ID: %d\n", ptr->id);
    printf("Name: %s\n", ptr->name);
    printf("Price: %.2f\n", ptr->price);

    // 4. Unmap memory and close file descriptor
    munmap(ptr, sizeof(Product));
    close(shm_fd);

    shm_unlink(SHM_NAME);

    return EXIT_SUCCESS;
}
