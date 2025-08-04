#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/mysocket_stream"

/**
 * @brief UNIX domain stream socket server.
 *        Waits for a client to connect via a local UNIX socket, then receives data.
 *
 * @return int 0 on success, non-zero on failure.
 */
int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[1024] = {0};

    // Remove existing socket file, if any
    unlink(SOCKET_PATH);

    // Create a UNIX domain stream socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure socket address
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Bind the socket to the file system path
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for a single connection
    if (listen(server_fd, 1) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("UNIX Stream Server waiting for connection at %s...\n", SOCKET_PATH);

    // Accept a connection from a client
    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Receive data from client
    if (read(client_fd, buffer, sizeof(buffer) - 1) < 0) {
        perror("read failed");
    } else {
        buffer[sizeof(buffer) - 1] = '\0'; // Null-terminate buffer
        printf("Server received: %s\n", buffer);
    }

    // Clean up
    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
