#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

/**
 * @brief Simple IPv4 UDP client.
 *        Prompts the user for a string and sends it to the server.
 * 
 * @return int 0 on success, exits with failure code on error.
 */
int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char message[1024];

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address (localhost:8080)
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;                  // IPv4
    serv_addr.sin_port = htons(PORT);                // Convert to network byte order
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr); // Server IP address

    // Prompt user to enter a message
    printf("Enter a message to send: ");
    fgets(message, sizeof(message), stdin);
    // message[strcspn(message, "\n")] = '\0';

    // Send the message to the server
    sendto(sockfd, message, strlen(message), 0,
           (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("Client sent: %s\n", message);

    // Close the socket
    close(sockfd);
    return 0;
}
