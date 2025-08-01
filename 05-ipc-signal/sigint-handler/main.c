#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int sigint_count = 0;

/**
 * @brief Signal handler for SIGINT (Ctrl+C).
 *        Increments the counter each time SIGINT is received.
 *        Terminates the program after receiving SIGINT three times.
 */
void handle_sigint() {
    sigint_count++;
    printf("SIGINT received (%d)\n", sigint_count);
    if (sigint_count >= 3) {
        printf("Received SIGINT 3 times. Exiting.\n");
        exit(0);
    }
}

/**
 * @brief Main function that registers the SIGINT handler
 *        and enters a loop waiting for signals.
 * 
 * @return Always returns 0 upon normal program exit.
 */
int main() {
    signal(SIGINT, handle_sigint);
    while (1) {
        pause();
    }

    return 0;
}
