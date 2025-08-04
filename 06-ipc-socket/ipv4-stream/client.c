#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

/**
 * @brief Simple TCP client using IPv4.
 *        Connects to a server and sends a message entered by the user.
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
    char message[BUFFER_SIZE];

    int sockfd;
    struct sockaddr_in serv_addr;

    // Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return 1;
    }

    // Set up server address struct
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IP string to binary form
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return 1;
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    // Read input from user
    printf("Enter message to send: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0'; // Remove trailing newline

    // Send message to server
    send(sockfd, message, strlen(message), 0);
    printf("Client sent: %s\n", message);

    close(sockfd);
    return 0;
}
