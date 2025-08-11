#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define NAME_SIZE 64
#define SHM_NAME "/my_shared_mem"

/**
 * @brief Structure to represent a product stored in shared memory.
 */
typedef struct {
    int id;
    char name[NAME_SIZE];
    float price;
} Product;

/**
 * @brief Main function to create and write data into POSIX shared memory.
 *
 * This program:
 * - Creates or opens a POSIX shared memory region.
 * - Sets its size to fit one `Product` structure.
 * - Maps it into the address space.
 * - Writes a sample product into the memory.
 *
 * @return int Returns EXIT_SUCCESS (0) on success, or EXIT_FAILURE (1) on error.
 */
int main() {
    // 1. Create or open the shared memory object
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // 2. Set the size of the shared memory to fit one Product struct
    if (ftruncate(shm_fd, sizeof(Product)) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    // 3. Map the shared memory into the process's address space
    Product* ptr = mmap(0, sizeof(Product), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // 4. Write product data to the shared memory
    ptr->id = 101;
    strncpy(ptr->name, "MacBook Pro M3", NAME_SIZE);
    ptr->price = 2499.99f;

    // 5. Print confirmation
    printf("Product written to shared memory:\n");
    printf("ID: %d\n", ptr->id);
    printf("Name: %s\n", ptr->name);
    printf("Price: %.2f\n", ptr->price);

    // 6. Close the shared memory file descriptor (mapping still remains)
    close(shm_fd);

    return EXIT_SUCCESS;
}
