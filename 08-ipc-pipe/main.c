#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * @brief Demonstrates interprocess communication using a pipe.
 * 
 * This program creates two child processes where:
 * - The first executes `ls -l` and writes its output into a pipe.
 * - The second reads from the pipe and runs `wc -l` to count the number of lines.
 * 
 * The program simulates the shell command: `ls -l | wc -l`
 * 
 * @return EXIT_SUCCESS on success, or EXIT_FAILURE on error.
 */
int main() {
    int fd[2];  // fd[0] for reading, fd[1] for writing

    // Create a pipe
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork the first child process for `ls -l`
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // First child: writer process (ls -l)
        close(fd[0]);                     
        dup2(fd[1], STDOUT_FILENO);       
        close(fd[1]);                     

        execlp("ls", "ls", "-l", NULL);   
        perror("execlp ls");              
        exit(EXIT_FAILURE);
    }

    // Fork the second child process for `wc -l`
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // Second child: reader process (wc -l)
        close(fd[1]);                     
        dup2(fd[0], STDIN_FILENO);        
        close(fd[0]);                     

        execlp("wc", "wc", "-l", NULL);
        perror("execlp wc");              
        exit(EXIT_FAILURE);
    }

    // Parent process: close both ends of the pipe
    close(fd[0]);
    close(fd[1]);

    // Wait for both child processes to finish
    wait(NULL);
    wait(NULL);

    return EXIT_SUCCESS;
}
