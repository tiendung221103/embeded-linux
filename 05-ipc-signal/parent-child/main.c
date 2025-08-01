#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int signal_count = 0;

/**
 * @brief Signal handler for SIGUSR1 in the child process.
 *        Increments the signal counter and exits after 5 signals.
 * 
 * @param [in] sig The received signal (expected: SIGUSR1).
 */
void handle_sigusr1(int sig) {
    signal_count++;
    printf("Child: Received signal from parent (%d)\n", signal_count);

    if (signal_count >= 5) {
        printf("Child: Received 5 signals. Exiting.\n");
        exit(0);
    }
}

/**
 * @brief Creates a child process and sends SIGUSR1 signals
 *        from the parent to the child every 2 seconds.
 * 
 * @return 0 on success, 1 on fork failure.
 */
int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process registers SIGUSR1 handler
        signal(SIGUSR1, handle_sigusr1);

        while (1);
    } else {
        // Parent process sends SIGUSR1 to child every 2 seconds
        for (int i = 0; i < 5; i++) {
            sleep(2);
            kill(pid, SIGUSR1);
        }

        wait(NULL);
        printf("Parent: Child exited. Program done.\n");
    }

    return 0;
}
