#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

/**
 * @brief Simple TCP server using IPv4.
 *        Accepts a client connection and receives a message.
 * 
 * @param [in] argc Number of command-line arguments (must be 3)
 * @param [in] argv Argument vector: argv[1] = IP address, argv[2] = port
 * 
 * @return int 0 on success, 1 on failure
 */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <IP> <PORT>\n", argv[0]);
        return 1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);

    int server_fd, client_fd;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE] = {0};
    socklen_t addrlen = sizeof(address);

    // Create a TCP socket (IPv4)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        return 1;
    }

    // Set up server address struct
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);     // Convert IP string to binary
    address.sin_port = htons(port);              // Convert port to network byte order

    // Bind socket to IP and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }

    // Listen for incoming connections (max 1 pending)
    if (listen(server_fd, 1) < 0) {
        perror("listen failed");
        return 1;
    }

    printf("Server is listening on %s:%d...\n", ip, port);

    // Accept a client connection
    client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if (client_fd < 0) {
        perror("accept failed");
        return 1;
    }

    // Receive data from client
    read(client_fd, buffer, sizeof(buffer));
    printf("Server received: %s\n", buffer);

    // Close sockets
    close(client_fd);
    close(server_fd);
    return 0;
}
