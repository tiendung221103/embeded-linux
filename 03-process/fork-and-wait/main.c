#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @brief Demonstrates the creation of a child process using fork(),
 *        and how the parent waits for the child to terminate.
 *        The child exits with a specific exit code, which the parent retrieves.
 * 
 * @return 0 on successful execution, 1 if fork() fails.
 */
int main() {
    pid_t pid;    // Variable to store the PID of the child process
    int status;   // Variable to store the termination status of the child process

    pid = fork(); // Create a new process

    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // This is the child process
        printf("Child process: My PID is %d\n", getpid());
        exit(10); // Exit with status code 10
    } else {
        // This is the parent process
        printf("Parent process: My PID is %d\n", getpid());
        printf("Parent process: Child PID is %d\n", pid);

        // Wait for the child process to terminate
        wait(&status);

        // Check if the child exited normally
        if (WIFEXITED(status)) {
            printf("Parent process: Child exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Parent process: Child did not exit normally.\n");
        }
    }

    return 0;
}
