#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_SOCKET_PATH "/tmp/mysocket_dgram_server"
#define CLIENT_SOCKET_PATH "/tmp/mysocket_dgram_client"

/**
 * @brief UNIX domain datagram socket client.
 *        Sends a fixed message to the server via a UNIX domain datagram socket.
 *
 * @return int 0 on success, non-zero on failure.
 */
int main() {
    int sockfd;
    struct sockaddr_un server_addr, client_addr;
    const char *message = "Hello from client!";

    // Remove old socket file if it exists
    unlink(CLIENT_SOCKET_PATH);

    // Create a UNIX domain datagram socket
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configure client address
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, CLIENT_SOCKET_PATH, sizeof(client_addr.sun_path) - 1);

    // Bind client socket to a path
    if (bind(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Send message to server
    if (sendto(sockfd, message, strlen(message), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("sendto");
        close(sockfd);
        unlink(CLIENT_SOCKET_PATH);
        exit(EXIT_FAILURE);
    }

    printf("Client sent: %s\n", message);

    // Cleanup
    close(sockfd);
    unlink(CLIENT_SOCKET_PATH);
    return 0;
}
