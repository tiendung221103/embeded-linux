#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/mysocket_stream"

/**
 * @brief UNIX domain stream socket client.
 *        Connects to a local server and sends a predefined message.
 *
 * @return int 0 on success, non-zero on failure.
 */
int main() {
    int sockfd;
    struct sockaddr_un addr;
    const char *message = "Hello from client!";

    // Create a UNIX domain stream socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Connect to the server socket
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Send message to the server
    if (send(sockfd, message, strlen(message), 0) < 0) {
        perror("send failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Client sent: %s\n", message);

    // Clean up
    close(sockfd);
    return 0;
}
