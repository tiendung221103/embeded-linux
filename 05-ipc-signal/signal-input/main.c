#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/select.h>

int pipe_fd[2];  // Pipe for communication between signal thread and main thread

/**
 * @brief Signal handling thread function. Waits for SIGINT or SIGTERM and writes to pipe.
 * 
 * @param [in] arg Pointer to sigset_t containing blocked signals
 * @return NULL
 */
void* signal_thread(void* arg) {
    sigset_t* set = (sigset_t*)arg;
    int sig;

    while (1) {
        if (sigwait(set, &sig) == 0) {
            if (sig == SIGINT) {
                const char* msg = "SIGINT\n";
                write(pipe_fd[1], msg, strlen(msg));
            } else if (sig == SIGTERM) {
                const char* msg = "SIGTERM\n";
                write(pipe_fd[1], msg, strlen(msg));
                break;
            }
        }
    }

    return NULL;
}

/**
 * @brief Main function that sets up signal handling via thread and uses select()
 *        to handle input from both user (stdin) and signal thread (pipe).
 * 
 * @return 0 on success, or exits on error
 */
int main() {
    // Create pipe for inter-thread communication
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Block SIGINT and SIGTERM in the main thread
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGTERM);

    if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0) {
        perror("pthread_sigmask");
        exit(EXIT_FAILURE);
    }

    // Create signal-handling thread
    pthread_t tid;
    if (pthread_create(&tid, NULL, signal_thread, (void*)&set) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    printf("Program is running... Press Ctrl+C (SIGINT) or send SIGTERM to terminate.\n");

    char buffer[1024];
    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);      // Monitor standard input
        FD_SET(pipe_fd[0], &readfds);        // Monitor pipe for signal messages

        int maxfd = (STDIN_FILENO > pipe_fd[0]) ? STDIN_FILENO : pipe_fd[0];

        int ready = select(maxfd + 1, &readfds, NULL, NULL, NULL);
        if (ready == -1) {
            perror("select");
            break;
        }

        // Handle user input
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                printf("User input: %s", buffer);
            }
        }

        // Handle signal message from signal thread
        if (FD_ISSET(pipe_fd[0], &readfds)) {
            char msg[64] = {0};
            read(pipe_fd[0], msg, sizeof(msg));

            if (strstr(msg, "SIGINT")) {
                printf("SIGINT received.\n");
            } else if (strstr(msg, "SIGTERM")) {
                printf("SIGTERM received. Exiting program.\n");
                break;
            }
        }
    }

    // Cleanup: close pipe and join thread
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    pthread_join(tid, NULL);

    return 0;
}
