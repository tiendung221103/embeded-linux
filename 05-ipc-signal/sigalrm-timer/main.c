#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int counter = 0;

/**
 * @brief Signal handler for SIGALRM.
 *        Increments the counter every second and prints the elapsed time.
 *        Exits the program after 10 seconds.
 * 
 * @param [in] sig The signal number (expected to be SIGALRM).
 */
void handle_sigalrm(int sig) {
    counter++;
    printf("Timer: %d seconds\n", counter);

    if (counter < 10) {
        alarm(1); // Schedule the next alarm in 1 second
    } else {
        printf("Done.\n");
        exit(0);
    }
}

/**
 * @brief Main function that sets up a timer using SIGALRM.
 *        Registers the signal handler and enters an infinite loop.
 * 
 * @return 0 Always returns 0 (though it never reaches here).
 */
int main() {
    signal(SIGALRM, handle_sigalrm);

    alarm(1); 

    while (1); 

    return 0;
}
