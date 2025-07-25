#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * @brief Demonstrates how a parent process sets an environment variable
 *        and a child process retrieves and uses it to execute a command.
 * 
 * The parent sets the environment variable MY_COMMAND to "ls", then forks a child.
 * The child reads the variable and uses it to execute the corresponding program using execlp().
 * 
 * @return 0 on success, 1 on error.
 */
int main() {
    pid_t pid;

    // Parent process sets the environment variable MY_COMMAND = "ls"
    setenv("MY_COMMAND", "ls", 1);

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        char *command = getenv("MY_COMMAND");  // Read the environment variable

        if (command == NULL) {
            fprintf(stderr, "Environment variable MY_COMMAND not found\n");
            exit(1);
        }

        // Execute the command (e.g., "ls" → /bin/ls)
        execlp(command, command, NULL);
        perror("exec failed");
        exit(1);
    } else {
        // Parent process waits for the child to finish
        wait(NULL);
        printf("Parent process: Child has finished.\n");
    }

    return 0;
}
