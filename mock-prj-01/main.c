#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "peer.h"
#include "connection.h"
#include "command.h"

int server_port;
int listener_socket;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <listening_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    server_port = atoi(argv[1]);

    listener_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listener_socket < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(server_port)
    };

    if (bind(listener_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(listener_socket, MAX_PEERS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("[INFO] Listening on port %d...\n", server_port);

    pthread_t listener_tid;
    pthread_create(&listener_tid, NULL, listener_thread, NULL);

    process_user_input();

    close(listener_socket);
    return 0;
}
