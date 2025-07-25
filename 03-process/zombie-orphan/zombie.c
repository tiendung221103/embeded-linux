#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

/**
 * @brief Demonstrates zombie process behavior.
 * 
 * The child process terminates immediately after printing its message.
 * The parent does not call wait(), so the child's exit status is not collected,
 * causing the child process to remain as a zombie for a while.
 * 
 * @return 0 on success.
 */
int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        printf("Child (PID: %d) is terminating.\n", getpid());
        exit(0);
    } else {
        // Parent process does not call wait()
        printf("Parent (PID: %d) is not calling wait(). Sleeping for 30 seconds...\n", getpid());
        sleep(30); // During this time, the child becomes a zombie
    }

    return 0;
}
