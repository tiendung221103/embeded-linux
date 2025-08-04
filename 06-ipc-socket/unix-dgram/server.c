#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_SOCKET_PATH "/tmp/mysocket_dgram_server"

/**
 * @brief UNIX domain datagram socket server.
 *        Listens for a message from a datagram client via a local UNIX socket.
 *
 * @return int 0 on success, non-zero on failure.
 */
int main() {
    int sockfd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[1024];
    socklen_t client_len = sizeof(client_addr);

    // Remove old socket file if it exists
    unlink(SERVER_SOCKET_PATH);

    // Create a UNIX domain datagram socket
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configure server socket address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Bind the socket to the specified file path
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UNIX Datagram Server listening at %s...\n", SERVER_SOCKET_PATH);

    // Receive message from client
    if (recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                 (struct sockaddr *)&client_addr, &client_len) < 0) {
        perror("recvfrom");
        close(sockfd);
        unlink(SERVER_SOCKET_PATH);
        exit(EXIT_FAILURE);
    }

    buffer[sizeof(buffer) - 1] = '\0'; // Ensure null-termination
    printf("Server received: %s\n", buffer);

    // Cleanup
    close(sockfd);
    unlink(SERVER_SOCKET_PATH);
    return 0;
}
