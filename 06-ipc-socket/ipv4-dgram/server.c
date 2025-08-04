// ipv4_dgram/server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

/**
 * @brief Entry point for a simple IPv4 UDP server.
 *        Listens on a specific port and prints the received message.
 * 
 * @return int 0 on success, exits on error.
 */
int main() {
    int sockfd;
    struct sockaddr_in serv_addr, client_addr;
    char buffer[1024] = {0};
    socklen_t addr_len = sizeof(client_addr);

    // Create a UDP socket (SOCK_DGRAM for datagram)
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Zero out the server address structure
    memset(&serv_addr, 0, sizeof(serv_addr));

    // Configure server address: IPv4, any incoming interface, specific port
    serv_addr.sin_family = AF_INET;             // IPv4
    serv_addr.sin_addr.s_addr = INADDR_ANY;     // Accept connections from any address
    serv_addr.sin_port = htons(PORT);           // Convert port to network byte order

    // Bind the socket to the specified IP and port
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP Server is waiting for data on port %d...\n", PORT);

    // Receive a message from any client
    recvfrom(sockfd, buffer, sizeof(buffer), 0,
             (struct sockaddr *)&client_addr, &addr_len);

    // Print the received message
    printf("Server received: %s\n", buffer);

    // Close the socket
    close(sockfd);
    return 0;
}
