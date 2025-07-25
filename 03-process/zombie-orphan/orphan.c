#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>

/**
 * @brief Demonstrates orphan process behavior.
 * 
 * The parent process terminates immediately after forking.
 * The child process sleeps for a while, and during that time,
 * it becomes an orphan and is reparented to the init process (or another reaper like systemd).
 * 
 * @return 0 on successful execution.
 */
int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        sleep(10); // During this time, the parent will terminate

        // After waking up, getppid() will likely return 1 (init) or systemd's PID
        printf("Child process (PID: %d), new parent (PPID): %d\n", getpid(), getppid());
    } else {
        // Parent process exits immediately
        printf("Parent process (PID: %d) is terminating.\n", getpid());
        exit(0);
    }

    return 0;
}
