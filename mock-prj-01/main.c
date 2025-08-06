/**
 * @file main.c
 * @brief Entry point for the P2P chat application. Initializes listener and handles user commands.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "peer.h"
#include "connection.h"
#include "command.h"

int server_port;           ///< Port number on which the server will listen
int listener_socket;       ///< Listening socket descriptor

/**
 * @brief Main function to start the P2P chat node.
 * 
 * Initializes the listening socket, binds it to a port, starts the listener thread, 
 * and begins processing user input.
 * 
 * @param argc Argument count (expects 2).
 * @param argv Argument vector (expects listening port as argument).
 * @return int Exit status.
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <listening_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Convert port argument to integer
    server_port = atoi(argv[1]);

    // Create a TCP socket
    listener_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listener_socket < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,         // Listen on all interfaces
        .sin_port = htons(server_port)         // Convert port to network byte order
    };

    // Bind the socket to the specified port
    if (bind(listener_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(listener_socket, MAX_PEERS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("[INFO] Listening on port %d...\n", server_port);

    // Launch a thread to accept incoming peer connections
    pthread_t listener_tid;
    pthread_create(&listener_tid, NULL, listener_thread, NULL);

    // Begin handling user input (connect, send, terminate, etc.)
    process_user_input();

    // Clean up resources after user exits
    close(listener_socket);
    return 0;
}
